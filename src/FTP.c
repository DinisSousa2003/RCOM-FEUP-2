#include "../include/FTP.h"


//given a idAddress and a port, connect a socket 
int connectSocket(const char* ipAddress, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ipAddress);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }
    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        return -1;
    }

    return sockfd;
}

//Given an and a port, connect to a socket and save its file descriptor in the ftp struct
int ftpConnect(ftp* ftp, const char* ip, int port) {
	int socketfd;

	if ((socketfd = connectSocket(ip, port)) < 0) {
		printf("ERROR: Cannot connect socket.\n");
		return 1;
	}

	ftp->control_socket_fd = socketfd;
	ftp->data_socket_fd = 0;

	if (ftpReadAndPrint(ftp->control_socket_fd)) {
		printf("ERROR: ftpReadAndPrint failure.\n");
		return 1;
	}

	return 0;
}

//Given a socket file descriptor, a user and a password, login to the ftp server
int ftpLogin(ftp* ftp, const char* usermame, const char* password){
    char buf[1024];

    //USER
    sprintf(buf, "USER %s\r\n", usermame);

    if (ftpWrite(ftp->control_socket_fd, buf, strlen(buf))) {
        printf("ERROR: ftpWrite failure on login username.\n");
        return 1;
    }

    if (ftpReadAndPrint(ftp->control_socket_fd)) {
        printf("ERROR: ftpReadAndPrint failure on login uername.\n");
        return 1;
    }

    memset(buf, 0, sizeof(buf));

    //PASS
    sprintf(buf, "PASS %s\r\n", usermame);

    if (ftpWrite(ftp->control_socket_fd, buf, strlen(buf))) {
        printf("ERROR: ftpWrite failure on login password.\n");
        return 1;
    }

    if (ftpReadAndPrint(ftp->control_socket_fd)) {
        printf("ERROR: ftpReadAndPrint failure on login password.\n");
        return 1;
    }

    return 0;
}

//Given a socket file descriptor, enter passive mode
int ftpPasv(ftp* ftp) {
    char buf[1024];
    char* ip;
    int port;

    if (ftpWrite(ftp->control_socket_fd, "PASV\r\n", strlen("PASV\r\n"))) {
        printf("ERROR: ftpWrite failure on PASV.\n");
        return 1;
    }

    if (ftpRead(ftp->control_socket_fd, buf, sizeof(buf))) {
        printf("ERROR: ftpRead failure on PASV.\n");
        return 1;
    }

    ip = strtok(buf, "(");
    ip = strtok(NULL, "(");
    ip = strtok(ip, ")");
    char ip2[16];
    bzero(ip2, 16);
    strcpy(ip2, strtok(ip, ","));
    for (int i = 0; i < 3; i++) {
        strcat(ip2, ".");
        strcat(ip2, strtok(NULL, ","));
    }

    port = atoi(strtok(NULL, ",")) * 256;
    port += atoi(strtok(NULL, ","));

    printf("IP: %s\n", ip2);
    printf("PORT: %d\n", port);

    if ((ftp->data_socket_fd = connectSocket(ip2, port)) < 0) {
        printf("ERROR: Cannot connect socket.\n");
        return 1;
    }

    printf("Data socket connected.\n");
    return 0;
}

//Given a socket file descriptor, and a path, change the current directory
int ftpCWD(ftp* ftp, const char* path) {
    char buf[1024];

    sprintf(buf, "CWD %s\r\n", path);

    if (ftpWrite(ftp->control_socket_fd, buf, strlen(buf))) {
        printf("ERROR: ftpWrite failure on CWD.\n");
        return 1;
    }

    if (ftpReadAndPrint(ftp->control_socket_fd)) {
        printf("ERROR: ftpReadAndPrint failure on CWD.\n");
        return 1;
    }

    return 0;
}

//Given a socket file descriptor, get the current directory
int ftpPWD(ftp* ftp) {
    char buf[1024];

    sprintf(buf, "PWD\r\n");

    if (ftpWrite(ftp->control_socket_fd, buf, strlen(buf))) {
        printf("ERROR: ftpWrite failure on PWD.\n");
        return 1;
    }

    if (ftpReadAndPrint(ftp->control_socket_fd)) {
        printf("ERROR: ftpReadAndPrint failure on PWD.\n");
        return 1;
    }

    return 0;
}

//Given a socket file descriptor and a filename, retrieve a file
int ftpRetr(ftp* ftp, const char* filename) {
    char buf[1024];

    sprintf(buf, "RETR %s\r\n", filename);

    if (ftpWrite(ftp->control_socket_fd, buf, strlen(buf))) {
        printf("ERROR: ftpWrite failure on Retr.\n");
        return 1;
    }

    if (ftpReadAndPrint(ftp->control_socket_fd)) {
        printf("ERROR: ftpReadAndPrint failure on Retr.\n");
        return 1;
    }

    return 0;
}

//Given a socket file descriptor and a filename, download a file
int ftpDownload(ftp* ftp, const char* filename) {

    if (ftpReadToFile(ftp->data_socket_fd, filename)) {
        printf("ERROR: ftpReadToFile failure on Download.\n");
        return 1;
    }

    return 0;
}

//Given a socket file descriptor, write a string to it
int ftpWrite(int sockfd, char* buf, size_t size) {
    /*send a string to the server*/
    size_t bytes = write(sockfd, buf, size);

    if (bytes > 0){
        printf("Bytes escritos %ld\n", bytes);
    }
    else {
        perror("write()");
        exit(-1);
    }

    return 0;
}

//Given a socket file descriptor, read from it
int ftpRead(int sockfd, char* buf, size_t size) {
	FILE* fp = fdopen(sockfd, "r");

	do {
		memset(buf, 0, size);
		fgets(buf, size, fp);
		printf("%s", buf);
	} while (!('1' <= buf[0] && buf[0] <= '5') || buf[3] != ' ');

	return 0;
}

int ftpReadAndPrint(int sockfd) {
	FILE* fp = fdopen(sockfd, "r");
    size_t size = 1024;
    char* buf = malloc(size);

	do {
		memset(buf, 0, size);
		fgets(buf, size, fp);
		printf("%s", buf);
	} while (!('1' <= buf[0] && buf[0] <= '5') || buf[3] != ' ');

    free(buf);
	return 0;
}

//Given a socket file descriptor, read from it and save it to a file with the given name

int ftpReadToFile(int sockfd, const char* filename) {
    size_t size = 1024;
    char* buf = malloc(size);
    FILE* file = fopen(filename, "w");
    int bytes = 0;

	while ((bytes = read(sockfd, buf, size)) != 0) {

        if (bytes < 0){
            printf("Error reading from file.\n");
        }

        printf("%s", buf);

        if ((bytes = fwrite(buf, bytes, 1, file)) == 0){
            printf("Error writing to file from file.\n");
        }
        memset(buf, 0, size);
    }

    free(buf);

    fclose(file);
	close(sockfd);

	return 0;
}

//Given a socket file descriptor, close it
int ftpClose(int sockfd) {
    if (close(sockfd)<0) {
        perror("close()");
        exit(-1);
    }
    return 0;
}

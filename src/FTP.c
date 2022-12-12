#include "../include/FTP.h"


//given a idAddress and a port, connect a socket 
int connectSocket(const char* ipAddress, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    char buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";
    size_t bytes;

    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ipAddress);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }

    return sockfd;
}

//Given an and a port, connect to a socket and save its file descriptor in the ftp struct
int ftpConnect(ftp* ftp, const char* ip, int port) {
	int socketfd;
	char buf[1024];

	if ((socketfd = connectSocket(ip, port)) < 0) {
		printf("ERROR: Cannot connect socket.\n");
		return 1;
	}

	ftp->control_socket_fd = socketfd;
	ftp->data_socket_fd = 0;

	if (ftpRead(ftp, buf, sizeof(buf))) {
		printf("ERROR: ftpRead failure.\n");
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

    if (ftpRead(ftp, buf, sizeof(buf))) {
        printf("ERROR: ftpRead failure on login uername.\n");
        return 1;
    }

    memset(buf, 0, sizeof(buf));

    //PASS
    sprintf(buf, "PASS %s\r\n", usermame);

    if (ftpWrite(ftp->control_socket_fd, buf, strlen(buf))) {
        printf("ERROR: ftpWrite failure on login password.\n");
        return 1;
    }

    if (ftpRead(ftp, buf, sizeof(buf))) {
        printf("ERROR: ftpRead failure on login password.\n");
        return 1;
    }

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
	FILE* fp = fdopen(ftp->control_socket_fd, "r");

	do {
		memset(buf, 0, size);
		str = fgets(buf, size, fp);
		printf("%s", str);
	} while (!('1' <= str[0] && str[0] <= '5') || str[3] != ' ');

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

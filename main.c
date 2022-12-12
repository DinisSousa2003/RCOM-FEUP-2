#include "include/URL.h"
#include "include/FTP.h"


//unique use case: connect, login host, passive, get path, success (file saved in CWD) or un-success (indicating failing phase)
//parseUrl
//getIpByHostName
//connectSocket
//ftpLogin
//ftpCWD
//ftpPasv
//ftpRetr
//ftpDownload
//ftpClose

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s ftp://[<user>:<password>@]host[:port]/path/to/filename\n", argv[0]);
        exit(-1);
    }

    //URL PARSEno match
    url u;
    initUrl(&u);
    parseUrl(&u, argv[1]);
    getIpByHostName(&u);

    printf("host: %s\n with ip: %s", u.host, u.ip);
    
    //FTP CONNECT
    ftp ftp;
    ftpConnect(&ftp, u.ip, u.port);

    //FTP LOGIN
    //Verify if it is a user or if it is anonymous
    if(strlen(u.user) > 0) {
        ftpLogin(&ftp, u.user, u.password);
    } else {
        ftpLogin(&ftp, "anonymous", "anonymous");
    }

    ftpPasv(&ftp);

    ftpCWD(&ftp, u.path);

    ftpPWD(&ftp);

    ftpRetr(&ftp, u.filename);

    ftpDownload(&ftp, u.filename);

    ftpClose(ftp.control_socket_fd);

    return 0;
}
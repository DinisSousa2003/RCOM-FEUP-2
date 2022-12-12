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

    //URL PARSE
    url u;
    initUrl(&u);
    //parseUrl(&u, "ftp://user:password@host:port/path/filename");
    parseUrl(&u, "A");
    getIpByHostName(&u);

    //FTP CONNECT
    ftp ftp;
    ftpConnect(&ftp, u.host, u.port);

    //FTP LOGIN

    //Verify if it is a user or if it is anonymous
    if(strlen(u.user) > 0) {
        ftpLogin(&ftp, u.user, u.password);
    } else {
        ftpLogin(&ftp, "anonymous", "anonymous");
    }

    return 0;
}
#include "includes/URL.h"
#include "includes/FTP.h"

int main(int argc, char *argv[]) {
    url u;
    initUrl(&u);
    parseUrl(&u, "ftp://user:password@host:port/path/filename");
    getIpByHostName(&u);
    int sockfd = connectSocket(u.host, u.port);
    ftpWrite(sockfd, "Mensagem de teste na travessia da pilha TCP/IP\n");
    return 0;
}
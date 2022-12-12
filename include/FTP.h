#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>


typedef struct FTP
{
    int control_socket_fd; // file descriptor to control socket
    int data_socket_fd; // file descriptor to data socket
} ftp;

int connectSocket(const char* IPAddress, int port);
int ftpConnect(ftp* ftp, const char* ip, int port);
int ftpLogin(ftp* ftp, const char* usermame, const char* password);
int ftpPasv(ftp* ftp);
int ftpCWD(ftp* ftp, const char* path);
int ftpPWD(ftp* ftp);
int ftpRetr(ftp* ftp, const char* filename);
int ftpDownload(ftp* ftp, const char* filename);
int ftpWrite(int sockfd, char* buf, size_t buf_size);
int ftpRead(int sockfd, char* buf, size_t size);
int ftpReadAndPrint(int sockfd);
int ftpReadToFile(int sockfd, const char* filename);
int ftpClose(int sockfd);

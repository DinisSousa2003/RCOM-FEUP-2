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
int ftpWrite(int sockfd, char* buf, size_t buf_size);
int ftpRead(int sockfd, char* buf, size_t buf_size);
int ftpClose(int sockfd);

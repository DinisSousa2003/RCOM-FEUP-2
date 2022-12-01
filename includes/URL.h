#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include<arpa/inet.h>

#define MAX_STRING_LENGTH 256
typedef char url_data[MAX_STRING_LENGTH];

typedef struct URL {
    url_content user;
    url_content password;
    url_content host;
    url_content path;
    url_content filename;
    int port;
} url;

void initUrl(url *u);
int parseUrl(url *u, const char *urlString);
int getIpByHostName(url *u);

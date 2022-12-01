#include "../include/URL.h"


/**
 * The struct hostent (host entry) with its terms documented

    struct hostent {
        char *h_name;    // Official name of the host.
        char **h_aliases;    // A NULL-terminated array of alternate names for the host.
        int h_addrtype;    // The type of address being returned; usually AF_INET.
        int h_length;    // The length of the address in bytes.
        char **h_addr_list;    // A zero-terminated array of network addresses for the host.
        // Host addresses are in Network Byte Order.
    };

    #define h_addr h_addr_list[0]	The first address in h_addr_list.
*/

void initUrl(url *u) {
    memset(u->user, 0, MAX_STRING_LENGTH);
    memset(u->password, 0, MAX_STRING_LENGTH);
    memset(u->host, 0, MAX_STRING_LENGTH);
    memset(u->path, 0, MAX_STRING_LENGTH);
    memset(u->filename, 0, MAX_STRING_LENGTH);
    u->port = 0;
}

//TO DO: parse URL
//Function able to parse the ulr string and return the different parts of the url in the url struct
//Return 0 if the url is valid, -1 if not

int parseUrl(url *u, const char *urlString){
    //to do
}


int getIpByHostName(url *u) {
    struct hostent *h;

    if ((h = gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname()");
        exit(-1);
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    char *ip = inet_ntoa(*((struct in_addr *) h->h_addr));
    strcpy(u->ip, ip);

    return 0;
}

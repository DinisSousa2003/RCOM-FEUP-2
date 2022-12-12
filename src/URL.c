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
    u->port = 21; //FTP is on port 21
}

//Function able to parse the ulr string and return the different parts of the url in the url struct
//Return 0 if the url is valid, -1 if not

int parseUrl(url *u, const char *urlString){
    regex_t re;
  
    int res = 0;
    res = regcomp(&re, "^ftp://(([a-zA-Z0-9]+):([a-zA-Z0-9]+)@)?([a-zA-Z0-9$_.+!*'(),-]+)(:([0-9]+))?([a-zA-Z0-9 /()_,.-]*?)/([a-zA-Z0-9 ()_,.-]+)$", REG_EXTENDED);
    if (res != 0) {
        char buffer[256];
        regerror(res, &re, buffer, sizeof(buffer));
        printf("regcomp() failed with '%s'\n", buffer);
        exit(EXIT_FAILURE);
    }
  
    regmatch_t m[re.re_nsub + 1];
    if (regexec(&re, urlString, re.re_nsub + 1, m, 0) == 0) {

        int start = m[2].rm_so, end = m[2].rm_eo;
        if(start > 0 && end > 0){
            strncpy(u->user, urlString + start, end - start);
            printf("user: %s\n", u->user);
        }
            
        start = m[3].rm_so, end = m[3].rm_eo;
        if(start > 0 && end > 0){
            strncpy(u->password, urlString + start, end - start);
            printf("password: %s\n", u->password);
        }
        
        start = m[4].rm_so, end = m[4].rm_eo;
        strncpy(u->host, urlString + start, end - start);
        printf("host: %s\n", u->host);
        
        
        start = m[7].rm_so, end = m[7].rm_eo;
        strncpy(u->path, urlString + start, end - start);
        printf("path: %s\n", u->path);

        start = m[8].rm_so, end = m[8].rm_eo;
        strncpy(u->filename, urlString + start, end - start);
        printf("filename: %s\n", u->filename);
    }
    else
        printf("No match! Correct usage: ftp://[<user>:<password>@]host[:port]/path/to/filename\n");
  
    regfree(&re);
    return 0;
}


int getIpByHostName(url *u) {
    struct hostent *h;

    if ((h = gethostbyname(u->host)) == NULL) {
        herror("gethostbyname()");
        exit(-1);
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    char *ip = inet_ntoa(*((struct in_addr *) h->h_addr));
    strcpy(u->ip, ip);

    return 0;
}

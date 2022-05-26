#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    struct hostent *host_info; 
    struct in_addr *address;

    char input[200];
    //copy string value from keyboard to variable to process
    strcpy(input, argv[1]);
    
    
    host_info = gethostbyname(input);
    if (host_info == NULL) {
        perror("gethostbyname");
        exit(1);
    }

    for (int i = 0; host_info->h_addr_list[i] != NULL; i++) {
        address = (struct in_addr *)(host_info->h_addr_list[i]);
        printf("%s IPv4: %s\n", input, inet_ntoa(*address));
    }
}
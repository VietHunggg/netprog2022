#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8784

int main(int argc, char *argv[]){

    struct sockaddr_in saddr;
    struct hostent *h;
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket\n");
        return 1;
    }
    if ((h = gethostbyname("35.222.105.161")) == NULL) {
        printf("Unknown host\n");
        return 1;
    }
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *)&saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(PORT);
    
    if(connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0){
        printf("Connect failed\n");
    }
    
    char sendbuffer[1024];
    char rcvbuffer[1024];
    printf("Input something:\n");
    scanf("%s", sendbuffer);
    send(sockfd, sendbuffer, sizeof(sendbuffer),0);
    recv(sockfd, rcvbuffer, sizeof(rcvbuffer),0);
    printf("%s", rcvbuffer);
    return 0;
}

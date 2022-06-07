#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>

#define PORT 8784

int main(int argc, char *argv[]){

    struct sockaddr_in saddr;
    struct hostent *h;
    int on = 1;
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket\n");
        return 1;
    }
    if ((h = gethostbyname("127.0.0.1")) == NULL) {
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

    if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))) < 0) {
        perror("Set socket opt failed\n");
        close(sockfd);
        exit(-1);
    }
    
    int fl = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL,fl | O_NONBLOCK) < 0){
        perror("fcntl");
    }
    
    char sendbuffer[1024];
    char rcvbuffer[1024];
    
    bool condition = true;
    while (condition){
        printf("\nInput something:\n");
        do {
            bzero(sendbuffer, sizeof(sendbuffer));
            fgets(sendbuffer, sizeof(sendbuffer), stdin);
            if (strncmp(sendbuffer,"/quit\n", 6) == 0) {
                    printf("Disconnected\n");
                    close(sockfd);
                    condition = false;
                    break;
            }
            
            send(sockfd, sendbuffer, strlen(sendbuffer) +1,0);
        } while (sendbuffer[strlen(sendbuffer) - 1] != '\n');
        //clear send buffer
        bzero(sendbuffer, sizeof(sendbuffer));
        
        do {
            bzero(rcvbuffer, sizeof(rcvbuffer));
            recv(sockfd, rcvbuffer, sizeof(rcvbuffer),0);
            if (strncmp(sendbuffer,"/dc\n",4) == 0) {
                    condition = false;
                    break;
            }
            printf("%s", rcvbuffer);
        } while (rcvbuffer[strlen(rcvbuffer) - 1] != '\n');
        //clear received buffer
        bzero(rcvbuffer, sizeof(rcvbuffer));
    }
    printf("bye");
    close(sockfd);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PORT 8784

int main(int argc, char *argv[]){

    // variable and structures to setup socket
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    //define buffer size
    char buffer[1024];
    
    //server socket
    // AF_INET (ipv4), SOCK_STREAM (TCP), protocol 0
    if ((server_fd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        perror("Error in creating socket\n");
    } 

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0){
        perror("Error binding\n");
    }

    if ((listen(server_fd, 5)) < 0){
        perror("Error listening\n");
    }
    else {
        printf("LISTENING port: %d\n", PORT);
    }

    printf("Waitting for connection\n");
    
    printf("Connecting\n");
    while(1){
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
        printf("CONNECTED !\n");
        //
    
        char sendbuffer[1024];
        char rcvbuffer[1024];
        
        bool condition = true;
        while (condition) {
            do {
                recv(client_fd, rcvbuffer, sizeof(rcvbuffer), 0);
                printf("%s", rcvbuffer);
            } while (rcvbuffer[strlen(rcvbuffer) - 1] != '\n');
            bzero(rcvbuffer, sizeof(rcvbuffer));

            printf("\nInput something:\n");
            do {
                fgets(sendbuffer, sizeof(sendbuffer), stdin);
                send(client_fd, sendbuffer, strlen(sendbuffer) +1,0);
            } while (sendbuffer[strlen(sendbuffer) - 1] != '\n');
            bzero(sendbuffer, sizeof(sendbuffer));
        }
        // clode 1 client and wait another
        close(client_fd);
    }
    //close(server_fd);
    return 0;
}
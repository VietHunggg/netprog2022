#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    server_addr.sin_addr.s_addr = inet_addr("35.222.105.161");

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
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    printf("CONNECTED !\n");
        //close(client_fd);
    char rcvbuffer[1024];
    char sendbuffer[1024];
    recv(client_fd, rcvbuffer, sizeof(rcvbuffer), 0);
    printf("%s", rcvbuffer);
    printf("Input something:\n");
    scanf("%s", sendbuffer);
    send(client_fd, sendbuffer, sizeof(sendbuffer),0);



    
    //close(server_fd);
    
    return 0;
}
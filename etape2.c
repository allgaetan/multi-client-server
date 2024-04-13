// ETAPE 2

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 1234
#define MAX_MESSAGE_SIZE 512

int main() {
    // Variable declaration
    int fd, newfd; // Socket file descriptor 
    int client_len; // length of client's address
    int binding; // Variable for binding socket 
    int sending; // Variable for sending message 
    int receiving; // Variable for receiving message 
    struct sockaddr_in server_address; // Server address
    struct sockaddr_in client_address; // Client address
    char message[MAX_MESSAGE_SIZE]; // Message buffer
    
    // Sets the fields of server_address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;    
    server_address.sin_port = htons(PORT);

    // Creates a TCP socket
    fd = socket(AF_INET, SOCK_STREAM, 0); 

    // Try binding the socket with the server address
    binding = bind(fd, (struct sockaddr *) &server_address, sizeof(server_address)); 

    // Start listening
    listen(fd, 5);
    printf("Start listening on port %d:\n", PORT);
    client_len = sizeof(client_address);

    // Accepts connection with client 
    newfd = accept(fd, (struct sockaddr *) &client_address, &client_len);
    printf("Client connected\n");

    // Communication loop
    while(1) {
        // Try receiving the message
        bzero(message, MAX_MESSAGE_SIZE);
        receiving = recv(newfd, message, MAX_MESSAGE_SIZE, 0);
        // Client disconnected or error
        if (receiving <= 0) { 
            printf("Client disconnected\n");
            break;
        }
        printf("Client: %s\n", message);
        
        // Try sending back the message
        sending = send(newfd, message, strlen(message), 0);
    }
    close(newfd);
    close(fd);
    return 0;
}
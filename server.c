// ETAPE 3

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/select.h>

#define PORT 1024
#define MAX_MESSAGE_SIZE 512

int main() {
    // Variable declaration
    int socketfd, newsocketfd; // Socket file descriptor (-1 if socket can't be read)
    int client_len; // length of client's address
    int binding; // Variable for binding socket (-1 if binding failed)
    int sending; // Variable for sending message (-1 if sending failed)
    int receiving; // Variable for receiving message (-1 if receiving failed)
    struct sockaddr_in server_address; // Server address
    struct sockaddr_in client_address; // Client address
    char message[MAX_MESSAGE_SIZE]; // Message buffer
    fd_set rfds; // File descriptor
    
    // Sets the fields of server_address
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;    
    server_address.sin_port = htons(PORT);

    // Creates a TCP socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (socketfd == -1) {
        perror("Socket error\n");
        return 1;
    }

    // Try binding the socket with the server address
    binding = bind(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)); 
    if (binding == -1) {
        perror("Binding error\n");
        return 2;
    }

    // Start listening
    listen(socketfd, 5);
    printf("Server listening on port %d...\n", PORT);
    client_len = sizeof(client_address);
    
    // Accepts connection with client and returns a new socket uupon connection
    newsocketfd = accept(socketfd, (struct sockaddr *) &client_address, &client_len);
    if (newsocketfd == -1) {
        perror("Accepting error");
        return 3;
    }
    printf("Client connected\n");

    // Communication loop
    while(1) {
        // Initialization
        FD_ZERO(&rfds); // Initialize the file descriptor set
        FD_SET(newsocketfd, &rfds); // Add the socket to the set
        FD_SET(STDIN_FILENO, &rfds); // Add stdin to the set
        int nfds = newsocketfd > STDIN_FILENO ? newsocketfd : STDIN_FILENO; // Max file descriptor value
        
        if (select(nfds + 1, &rfds, NULL, NULL, NULL) == -1) {
            perror("Select error");
            return 4;
        }

        // Handles message from the socket
        if (FD_ISSET(newsocketfd, &rfds)) {
            // Try receiving the message
            bzero(message, MAX_MESSAGE_SIZE);
            receiving = recv(newsocketfd, message, MAX_MESSAGE_SIZE - 1, 0);
            if (receiving == -1) {
                perror("Receiving error\n");
                return 5;
            }
            printf("Client: %s", message);

            // Try sending back the message
            sending = send(newsocketfd, message, strlen(message), 0);
            if (sending == -1) {
                perror("Sending error\n");
                return 6;
            }
        }

        // Handles message from the standard input
        if (FD_ISSET(STDIN_FILENO, &rfds)) {
            fgets(message, MAX_MESSAGE_SIZE, stdin);
            sending = send(newsocketfd, message, strlen(message), 0);
            if (sending == -1) {
                perror("Sending error\n");
                return 7;
            }
        }
    }
    close(newsocketfd);
    close(socketfd);
    return 0;
}
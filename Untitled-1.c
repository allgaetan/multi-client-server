#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX_MESSAGE_SIZE 512

int main(int argc, char *argv[]) {
    // Variable declaration
    int fd; // Socket file descriptor
    int connection; // Connection variable
    int sending; // Variable for sending message
    int receiving; // Variable for receiving message 
    struct sockaddr_in server_address; // Server address
    struct hostent* server; // Server host
    char message[MAX_MESSAGE_SIZE]; // Message buffer

    // Takes as an argument and parses hostname:port 
    char *hostname, *temp; 
    unsigned int port;
    if (argc == 2) {
        temp = strdup(argv[1]);
        hostname = strsep(&temp, ":");
        if(temp) {
            port = atoi(temp);
        }
    }
    fprintf(stderr, "Connection to %s:%d\n", hostname, port);

    // Creates a TCP socket
    fd = socket(AF_INET, SOCK_STREAM, 0); 

    // Gets host information
    server = gethostbyname(hostname); 

    // Sets the fields of server_address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy(server->h_addr_list[0], &server_address.sin_addr, sizeof(server_address.sin_addr));
    server_address.sin_port = htons(port);

    // Try connection
    connection = connect(fd, (struct sockaddr *) &server_address, sizeof(server_address));
    printf("Successfully connected to %s:%d\n", hostname, port);

    // Set the socket to non-blocking mode
    fcntl(fd, F_SETFL, O_NONBLOCK);

    // Communication loop
    while(1) {
        // File descriptor set for select
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Wait for activity on either the socket or standard input
        int activity = select(fd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // If there is activity on the socket, receive message from server
        if (FD_ISSET(fd, &readfds)) {
            bzero(message, MAX_MESSAGE_SIZE);
            receiving = recv(fd, message, MAX_MESSAGE_SIZE - 1, 0);
            if (receiving > 0) {
                printf("Server: %s\n", message);
            } else if (receiving == 0) {
                printf("Server disconnected\n");
                break;
            }
        }

        // If there is activity on standard input, send message to server
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            bzero(message, MAX_MESSAGE_SIZE);
            if (fgets(message, MAX_MESSAGE_SIZE, stdin) != NULL) {
                sending = send(fd, message, strlen(message), 0);
            }
        }
    }
    
    close(fd);
    return 0;
}

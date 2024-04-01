// ETAPE 1

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

    int socketfd; // Socket file descriptor (-1 if socket can't be read)
    int connection; // Connection variable (-1 if connection failed)
    int sending; // Variable for sending message (-1 if sending failed)
    int receiving; // Variable for receiving message (-1 if receiving failed)
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
            port = atoi( temp );
        }
    }
    fprintf(stderr, "Connection to %s:%d\n", hostname, port);

    socketfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a TCP socket
    if (socketfd == -1) {
        perror("Socket error\n");
        return 1;
    }
    server = gethostbyname(hostname); // Gets host information
    if (server == NULL) {
        perror("Host error\n");
        return 2;
    }
    // Sets the fields of server_address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy(server->h_addr, &server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);
    // Try connection
    connection = connect(socketfd, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connection == -1) {
        perror("Connection error\n");
        return 3;
    }
    fprintf(stderr, "Succesfully connected to %s:%d\n", hostname, port);

    while(1) {
        printf("Enter message : ");
        bzero(message, MAX_MESSAGE_SIZE);
        fgets(message, MAX_MESSAGE_SIZE - 1, stdin);
        // Try sending the message
        sending = send(socketfd, message, strlen(message), 0);
        if (sending == -1) {
            perror("Sending error\n");
            return 4;
        }
        // Try receiving the message
        bzero(message, MAX_MESSAGE_SIZE);
        receiving = recv(socketfd, message, MAX_MESSAGE_SIZE - 1, 0);
        if (receiving == -1) {
            perror("Receiving error\n");
            return 5;
        }
        printf("Server : %s\n", message);
    }
    close(socketfd);
    return 0;
}

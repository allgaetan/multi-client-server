#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
    int fd;
    int err;
    int connection;
    struct sockaddr_in address;
    struct host;
    socklen_t remotelen;

    char *hostname, *temp;
    unsigned int port ;
    if ( argc == 2 ) {
    temp = strdup( argv[1] );
    hostname = strsep(&temp,":");
    if(temp) {
    port = atoi( temp );
    }
    }
 fprintf( stderr, "Connection to %s:%d\n", hostname, port);

    host = gethostbyname(hostname);

    bzero(&address, sizeof(address));
    bcopy(host->h_addr_list[0],&address.sin_addr,sizeof(address.sin_addr));
        
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    fd=socket(AF_INET, SOCK_STREAM,0);
    connection = connect(fd,sockadr_in adress,remotelen);

    /*envoyer des msg*/
    
    while(1){
    while(1){
        char word[100];
        printf("Entrer msg :");
        scanf("%s", word);
        return 0;
    }
    send(fd,word,100,0);
    }
    
}





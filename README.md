# Toolbox 2 - Réseau : TP1 Introduction aux sockets

## Run the project 

In a terminal, compile with GCC [server.c](/server.c) and [client.c](/client.c)
```
gcc -o server server.c
gcc -o client client.c
```
Then run the server file with no arguments and the client file with "hostname:port" argument
```
./server
./client localhost:1024
```
Right now it is configured on the port 1024 of localhost.
Then you can write any message in the client terminal and it will appear in the server terminal.
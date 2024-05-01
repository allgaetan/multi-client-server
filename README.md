# Simple multi-client echo server

## Run the project 

In a terminal, compile with GCC [server.c](/server.c) and [client.c](/client.c)
```
gcc -o server server.c
gcc -o client client.c
```
Then run the server file with no arguments in a terminal.
```
./server
```
To test the multi-client functionnality, open as much terminal as you want and launch run the client file with arguments address:port.
```
./client 127.0.0.1:1234
```
Right now it is configured on the port 1234.

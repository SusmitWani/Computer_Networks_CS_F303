Two files are given. 
1. client.c 
2. server.c

The client has to type in IP address and port number of the server to which it wants to connect.
The client will try to connect. 

If the connection isn't successful, the client will just exit
If connection is successful, then the client has to enter a filename and the server will send the file contents to the client which then makes a file in its directory and copies the content there.

The client will check if the server is online everytime it asks for the filename. Once the connection is established, it will go forward with the entire process of retrieving the file before checking for the connection again. 

The server will always stay online and continue to serve the clients.

The server will need to go online first else client will exit (for obvious reasons)

Commands to run:

1. Server terminal:
>>cd "Desktop/CN Lab 5/Server_dir"
>>gcc -o server.out server.c
>>./server.out

2. Client terminal:
>>cd "Desktop/CN Lab 5/Client_dir"
>>gcc -o client.out client.c
>>./client.out





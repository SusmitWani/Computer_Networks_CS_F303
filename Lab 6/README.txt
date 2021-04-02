Two files are given.
1. client.c
2. server.c


Commands to run for c programs:

1. Server terminal:
>>cd "Desktop/CN_Midsem_Lab"
>>gcc -o server.out server.c
>>./server.out 8000

2. Client terminals (Make 5 to test):
>>cd "Desktop/CN_Midsem_Lab"
>>gcc -o client.out client.c
>>./client.out 127.0.0.1 8000



The server accepts the port on which it wants to be online as a command line input
The server will always stay online and continue to serve the clients.

The client accepts the IP address and port number of the server to which it wants to connect as a command line input.
The client will try to connect to server. If 4 or more clients are already connected to the server, the server will reject the new client and client program will exit.

The client will get a prompt to send a message to the server. The client types in a message. Message is received by the server. Then server gets a prompt to send client a message. This process will go on till the client exits by typing exit.

While this is going on, the server continues to listen for more clients.

If multiple clients message the server, the server will respond in the order in which messages were sent to it.

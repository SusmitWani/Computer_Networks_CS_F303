Three files are given. 
1. client.c 
2. server.c
3. midsem_lab.pca.png

Commands to run for c programs:

1. Server terminal:
>>cd "Desktop/CN_Midsem_Lab"
>>gcc -o server.out server.c
>>./server.out 8000

2. Client terminal:
>>cd "Desktop/CN_Midsem_Lab"
>>gcc -o client.out client.c
>>./client.out 127.0.0.1 8000



The server accepts the port on which it wants to be online as a command line input
The server will always stay online and continue to serve the clients.

The client accepts the IP address and port number of the server to which it wants to connect as a command line input.
The client will try to connect to server. 

The client will get a prompt to send a message to the server. The client types in a message. Message is received by the server. Then server gets a prompt to send client a message. When server sends client a message, it begins to listen for more clients. Receiving and sending a message is a atomic operation. After receiving a message, the client will exit.

#######################################################################################################################################

For the pcap file, please feel free to reach out to me.

Steps to run the wireshark program:
For the pcapng file, just open the file in Wireshark. It was obtained by typing the following filter in the filter box:

tcp.dstport==8000 && ip.src==127.0.0.1 && ip.dst==127.0.0.1

Wireshark will show all the tcp packets sent from client to server. 

We can check size of data in the tcp packet from the length columns in wireshark (length can be found by adding tcp.len and tcp.hdr_len)

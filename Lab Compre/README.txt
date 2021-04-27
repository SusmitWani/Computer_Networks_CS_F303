README file for Compre Lab Computer Networks Second Semester 2020-2021
Author: Susmit Wani
ID No.: 2018A7PS0116G
Date of Submission: 26/04/2021

The code submitted has been tested and is working without any errors on Arch Linux kernel version 5.11.16-arch1-1 on gcc version 10.2.0

The Question Statement:
There are two clients and a server in the application. The two clients communicate privately with each other via the server. When any client sends an encrypted message, the message and its decryption are displayed in the other client.

1. The server as a command line argument accepts the port number to which it should bind. (2 mark)
2. Each client, as command line arguments, accepts the IP address at which it will find the server, the port number of the server at that IP address, the filename containing its private key, and the filename containing the other client's public key. (2 marks)
3. After connecting to the server, the client keeps on reading a line from the standard input. (2 marks) It encrypts the line and sends it to the server. (4 mark)
4. The server forwards the message received from one client to another. (4 marks)
5. When a client receives a message from the other client, (4 marks) it decrypts and displays both ciphertext and plaintext. (4 marks)
6. Both the clients exit if the user types "exit" in any client. (2 marks)

Submit code of client and server, two files containing the private keys (1 mark), two files containing the corresponding public keys (1 mark), and a README file (2 marks).

The README file should mention how to compile your code, how to execute your code, and a sample run of the code.You may use OpenSSL library for encryption and decryption.

You cannot use any existing commands, such as GnuPG, for encryption and decryption.

Files Included:
1. client.c
2. server.c
3. PrivateKey1.c
4. PublicKey1.c
5. PrivateKey2.c
6. PublicKey2.c
7. README.txt
8. makefile

Steps to run using makefile(Assuming keys are present):
1. server:
	>>>> make server

2. client
	Terminal 1:
		>>>> make client1
	Terminal 2:
		>>>> make client2
	Terminal 3:
		>>>> make client2

Steps to run the code:
1. Generating Keys (Public and Private):
	>>>> openssl genrsa -out private.pem 2048
	>>>> openssl rsa -in private.pem -outform PEM -pubout -out public.pem
	>>>> cp private.pem PrivateKey1.txt
	>>>> cp public.pem PublicKey1.txt

	>>>> openssl genrsa -out private.pem 2048
	>>>> openssl rsa -in private.pem -outform PEM -pubout -out public.pem
	>>>> cp private.pem PrivateKey2.txt
	>>>> cp public.pem PublicKey2.txt

2. server.c
	>>>> gcc -o server.out server.c -lpthread -Wall
	>>>> ./server.out 6000

3. client.c
	Run this code in three separate terminals
	Terminal 1:
		>>>> gcc -o client.out client.c -lcrypto -Wall 
		>>>> ./client.out 127.0.0.1 6000 PrivateKey1.txt PublicKey2.txt
 	Terminal 2: 
		>>>> gcc -o client.out client.c -lcrypto -Wall 
		>>>> ./client.out 127.0.0.1 6000 PrivateKey1.txt PublicKey2.txt
 	Terminal 3: 
		>>>> gcc -o client.out client.c -lcrypto -Wall 
		>>>> ./client.out 127.0.0.1 6000 PrivateKey1.txt PublicKey2.txt



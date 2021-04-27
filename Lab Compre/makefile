server: server.out
	./server.out 6000

client1: client.out
	./client.out 127.0.0.1 6000 PrivateKey1.txt PublicKey2.txt

client2: client.out
	./client.out 127.0.0.1 6000 PrivateKey2.txt PublicKey1.txt

server.out: server.c
	gcc -o server.out server.c -lpthread -Wall

client.out: client.c
	gcc -o client.out client.c -lcrypto -Wall

clean:
	rm -f client.out server.out

server: server.out
	./server.out

client: client.out
	./client.out

client.out: client.c
	gcc -o client.out client.c

server.out: server.c
	gcc -o server.out server.c

clean:
	rm -rf server.out client.out

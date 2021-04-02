#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>

#define SA struct sockaddr

void chat(int soc_stat) {
    //send message
    char send[80], rec[81];
    printf("Enter your message:");
    fgets(send, sizeof send, stdin);
    write(soc_stat, send, strlen(send));

    //receive message back
    bzero(rec, sizeof(rec));
    read(soc_stat, rec, 80);
    printf("Message from server: \n");
    for (int i = 1; i <= sizeof(rec); i++) {
        printf("%c", rec[sizeof(rec)-i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {
    //Accept inputs
    int port = atoi(argv[2]);
    printf("Connecting to IP address: %s\n", argv[1]);
    printf("Attempting to connect to port: %d\n", port);

    //Create socket
    int soc_stat = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_stat==-1){
        printf("Socket not created. Exiting\n");
        return 0;
    }
    else{
        printf("Socket created successfully.\n");
    }

    //Define server parameters
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(port);

    //actually connect
    if (connect(soc_stat, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to server failed. Server unreachable.\n");
        return 0;
    }
    else
        printf("connected to the server successfully\n");

    //Do the chat here
    chat(soc_stat);

    return 0;
}

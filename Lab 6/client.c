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

int chat(int soc_stat) {
    //send message
    char send[80], rec[81];
    printf("Enter your message:");
    fgets(send, sizeof send, stdin);
    write(soc_stat, send, strlen(send));
    if(strcmp(send, "exit\n")==0){
        return 0;
    }

    //receive message back
    bzero(rec, sizeof(rec));
    read(soc_stat, rec, 80);
    printf("Message from server: \n");
    for (int i = 1; i <= sizeof(rec); i++) {
        printf("%c", rec[sizeof(rec)-i]);
    }
    printf("\n");
    return 1;
}

int check_dc(int soc_stat) {
    int conn = 0;
    read(soc_stat, &conn, sizeof(conn));
    printf("Connections to server are %d\n", ntohl(conn));
    if(ntohl(conn)>4){
        printf("Too many clients connected. Try again later.\n");
        return 0;
    }
    return 1;

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

    if(check_dc(soc_stat)==0){
        close(soc_stat);
        return 0;
    }
    //Do the chat here
    while(1)
        if(chat(soc_stat)==0){
            printf("Closing connection\n");
            break;
        }

    return 0;
}

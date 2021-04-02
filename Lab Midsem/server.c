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

void chat_once(int sock_stat){
    //get message
    char rec[81];
    bzero(rec, 81);
    read(sock_stat, rec, sizeof(rec));
    for (int i = 1; i <= sizeof(rec); i++) {
        printf("%c", rec[sizeof(rec)-i]);
    }
    printf("\n");
    //Send message
    char send[80];
    printf("Enter your message:");
    fgets(send, sizeof send, stdin);
    write(sock_stat, send, strlen(send));
    printf("\n");
}

int main(int argc, char const *argv[]){
    int port = atoi(argv[1]);
    socklen_t len;
    printf("Attempting to create socket on port number %s\n", argv[1]);

    //Socket Creation
    int soc_stat = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_stat==-1){
        printf("Socket not created. Exiting...\n");
        return 0;
    }
    else{
        printf("Socket created successfully.\n");
    }

    //Server params
    struct sockaddr_in servaddr, cliaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(soc_stat, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed\n");
        exit(0);
    }
    else
        printf("Binded to the server successfully\n");

    // Now server is ready to listen and verification
    if ((listen(soc_stat, 5)) != 0) {
        printf("Listen failed\n");
        exit(0);
    }
    else
        printf("Server listening\n");

    while(1){
        len = sizeof(cliaddr);
        // Accept the data packet from client and verification
        int connfd = accept(soc_stat, (SA*)&cliaddr, &len);
        if (connfd < 0) {
            printf("server acccept failed...\n");
            exit(0);
        }
        else
            printf("server has acccepted the client\n");

        //Do the chat here
        chat_once(connfd);
        printf("Waiting for new client.\n\n");
    }
    //Disconnect
    close(soc_stat);
    printf("Socket closed\n");

    return 0;
}

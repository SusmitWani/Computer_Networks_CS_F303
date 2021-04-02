#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SA struct sockaddr
int connections = 0;
int chat_once(int sock_stat, struct sockaddr_in cliaddr){
    //get message
    char rec[81];
    bzero(rec, 81);
    read(sock_stat, rec, sizeof(rec));
    if(strcmp(rec, "exit\n")==0){
        printf("Connection terminated from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        return 0;
    }
    printf("\nText from %s:%d: ", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
    for (int i = 2; i <= sizeof(rec); i++) {
        printf("%c", rec[sizeof(rec)-i]);
    }
    printf("\n");
    //Send message
    char send[80];
    printf("Enter your message:");
    fgets(send, sizeof send, stdin);
    write(sock_stat, send, strlen(send));
    printf("\n");
    return 1;
}

int send_dc(int sock_stat, struct sockaddr_in cliaddr, int* clients){
    //Send message
    int conn = htonl(clients[0]);
    write(sock_stat, &conn, sizeof(conn));
    if(clients[0]>4){
        printf("Connection to %s:%d rejected because of client capacity\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        printf("\n");
        clients[0]--;
        return 0;
    }
    return 1;
}

int main(int argc, char const *argv[]){
    pid_t childpid;
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

    int shmid = shmget(IPC_PRIVATE, 1 * sizeof(int), 0777 | IPC_CREAT);
    int *clients = (int *)shmat(shmid, 0, 0);

    while(1){
        len = sizeof(cliaddr);
        // Accept the data packet from client and verification
        int connfd = accept(soc_stat, (SA*)&cliaddr, &len);
        if (connfd < 0) {
            // printf("Server acccept failed...\n");
            exit(0);
        }
        else{
            clients[0]++;
            if(send_dc(connfd, cliaddr, clients)==0){
                continue;
            }
            else{
                printf("Connection accepted from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                //Do the chat here
                if((childpid = fork()) == 0){
                    close(soc_stat);
                    printf("Number of connections: %d\n", clients[0]);
                    while(1){
                        if(chat_once(connfd, cliaddr)==0){
                            clients[0]--;
                            break;
                        }
                    }
                }
            }
        }
    }
    //Disconnect
    // close(soc_stat);
    // printf("Socket closed\n");

    return 0;
}

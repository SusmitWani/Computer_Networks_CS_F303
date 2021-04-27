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
#include <pthread.h>

#define SA struct sockaddr
#define MAX_CLIENTS 2
int clients = 0;
int connfd_arr[MAX_CLIENTS];
//Colours
void red () {
  printf("\033[1;31m");
}
void blue(){
	printf("\033[0;34m");
}
void yellow() {
  printf("\033[1;33m");
}
void green(){
	printf("\033[0;32m");
}
void reset () {
  printf("\033[0m");
}

void* chat_once(void* sock_stat_rec){
    //get message
    while(1){
        unsigned long long int var = (unsigned long long int)sock_stat_rec;
        char rec[256];
        bzero(rec, 256);
        // printf("Preparing to read\n");
        int rr = read(var, rec, 256);
        green();
        printf(">> Read message from client\n");
        reset();
        if(rr<=0){
            close(var);
            for(int i=0; i<MAX_CLIENTS; i++){
                if(connfd_arr[i]==var){
                    connfd_arr[i] = -1;
                }
            }
            clients = clients-1;
            yellow();
            printf(">> Clients connected = %d\n", clients);
            reset();
            break;
        }
        for(int i=0; i<MAX_CLIENTS; i++){
            if(connfd_arr[i]!=var){
                //Send message
                write(connfd_arr[i], rec, 256);
                // printf(">> Forwarding message once\n");
            }
        }
        sleep(1);
    }

    return NULL;
}

int send_dc(int sock_stat, struct sockaddr_in cliaddr){
    //Send message
    int conn = htonl(clients);
    write(sock_stat, &conn, sizeof(conn));
    if(clients>MAX_CLIENTS){
        yellow();
        printf("\n>> Connection to %s:%d rejected because of client capacity.", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        reset();
        clients = clients-1;
        green();
        printf("\n>> Clients connected: %d\n", clients);
        reset();
        fflush(stdout);
        return 0;
    }

    return 1;
}

int main(int argc, char const *argv[]){
    for(int i=0; i<MAX_CLIENTS; i++){
        // printf("%d ", connfd_arr[i]);
        connfd_arr[i] = -1;
    }
    printf("\n");

    int port = atoi(argv[1]);
    socklen_t len;
    yellow();
    printf("\n>> Attempting to create socket on port number %s", argv[1]);
    reset();

    //Socket Creation
    int soc_stat = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_stat==-1){
        red();
        printf("\n>> Socket not created. Exiting...");
        reset();
        return 0;
    }
    else{
        green();
        printf("\n>> Socket created successfully.\n");
        reset();
    }

    //Server params
    struct sockaddr_in servaddr, cliaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(soc_stat, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        red();
        printf("\n>> Socket bind failed\n");
        reset();
        exit(0);
    }
    else{
        green();
        printf("\n>> Binded to the server successfully\n");
        reset();
    }


    // Now server is ready to listen and verification
    if ((listen(soc_stat, 5)) != 0) {
        red();
        printf("\n>> Listen failed\n");
        reset();
        exit(0);
    }
    else{
        yellow();
        printf("\n>> Server listening\n");
        reset();
    }


    while(1){
        len = sizeof(cliaddr);
        // Accept the data packet from client and verification
        long long int connfd = accept(soc_stat, (SA*)&cliaddr, &len);
        if (connfd < 0) {
            red();
            printf("\n>> Server acccept failed...\n");
            reset();
            exit(0);
        }
        clients++;
        if(send_dc(connfd, cliaddr)==0){
            ;
        }
        else{
            for(int i=0; i<MAX_CLIENTS; i++){
                if(connfd_arr[i] == -1){
                    connfd_arr[i] = connfd;
                    break;
                }
            }
            green();
            printf("\n>> Connection accepted from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
            reset();
            yellow();
            printf("\n>> Number of connections: %d\n", clients);
            reset();
            //Do the chat here
            pthread_t thread_;
            pthread_create(&thread_, NULL, chat_once, (void*)connfd);
        }
        // printf("Sleeping for 1 sec");
        sleep(1);
    }

    return 0;
}

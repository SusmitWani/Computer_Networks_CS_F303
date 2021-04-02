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

void request_file(int soc_stat, char* filename) {
    //send filename
    printf("In the request function\n");
    char transfer[11];
    char temp[80];
    strcpy(temp, filename);

    write(soc_stat, temp, strlen(temp));
    printf("Getting %s from server\n", filename);
    //receive content of file back
    bzero(transfer, sizeof(transfer));
    read(soc_stat, transfer, 10);
    transfer[10] = '\0';
    // printf("Read data from server: %s\n", transfer);
    FILE* f = fopen(filename, "w+");
    if(strlen(transfer)>0){
        fprintf(f, "%s\n", transfer);
    }
    fclose(f);
}

int main() {
    //Accept inputs
    char ip[15];
    int port;
    char filename[80];
    printf("Enter the IP address of server: ");
    fgets(ip, 15, stdin);
    printf("Enter the port number: ");
    scanf("%d", &port);
    getchar();

    // ip[strcspn(ip, "\n")] = 0;

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
    struct sockaddr_in servaddr, cli;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    if (connect(soc_stat, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to server failed. Server unreachable.\n");
        return 0;
    }
    else
        printf("connected to the server successfully\n");

    //Do the file transfer here
    again: printf("Enter the file you want to access: ");
    fgets(filename, 80, stdin);
    filename[strcspn(filename, "\n")] = 0;
    request_file(soc_stat, filename);
    int rep;
    printf("Do you want to request for another file? Answer 0 for no and 1 for yes.\n");
    scanf("%d", &rep);
    getchar();

    if(rep==0){
        //Disconnect
        close(soc_stat);
        printf("Closed the connection\n");
    }
    else{
        goto again;
    }

    return 0;
}

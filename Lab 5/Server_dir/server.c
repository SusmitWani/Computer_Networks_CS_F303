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

void transfer_file(int sock_stat){
    // printf("In the transfer function\n");
    //get filename
    char filename[81];
    bzero(filename, 81);
    printf("Reading the filename from client\n");
    read(sock_stat, filename, sizeof(filename));
    printf("Requested file is: %s\n", filename);
    fflush(stdout);
    char transfer[10];
    bzero(transfer, 10);
    //Open file and transfer data
    FILE* f = fopen(filename, "r");
    if(f==NULL){
        printf("File not found\n");
        write(sock_stat, "\n", 1);
    }
    else{
        printf("Opening file\n");
        for(int i=0; i<10; i++){
            if ((transfer[i]=fgetc(f)) != '\n')
                ;
        }
        fclose(f);
        printf("File has been successfully transferred\n");
        write(sock_stat, transfer, strlen(transfer));
    }
}

int main(){
    int port;
    socklen_t len;
    printf("Enter the port number you want to be active on: ");
    scanf("%d", &port);
    getchar();

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
            printf("server acccept the client...\n");

        //Do the file transfer here
        transfer_file(connfd);
        printf("Waiting for new client.\n");
    }
    //Disconnect
    close(soc_stat);
    printf("Socket closed\n");

    return 0;
}

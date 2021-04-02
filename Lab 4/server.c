#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/stat.h>
#include <fcntl.h>
int main(){
    int ez;
    char* fifo = "/tmp/myfifo";
    mkfifo(fifo, 0666);
    char send2[200], receive2[200];
    int client_online = 1;
    while(1){
        //Receiving a message
        if(client_online == 0){
            // printf("Client: I am OFFLINE text back later\n");
        }
        else{
            ez = open(fifo, O_RDONLY);
            read(ez, receive2, 200);
            if(strcmp(receive2, "exit\n") == 0){
                client_online=0;
                printf("Client has gone offline\n\n");
            }
            else{
                printf("Client: %s\n", receive2);
            }
            close(ez);
        }

        //Sending a message
        if(client_online==0){
            printf("Server: ");
            fgets(send2, 200, stdin);
            if(strcmp(send2, "exit\n") == 0){
                close(ez);
                break;
            }
            printf("Message not delivered. Client is offline\n\n");
        }
        else{
            ez = open(fifo, O_WRONLY);
            printf("Server: ");
            fgets(send2, 200, stdin);
            write(ez, send2, strlen(send2)+1);
            close(ez);
            if(strcmp(send2, "exit\n") == 0){
                close(ez);
                break;
            }
            printf("\n");
        }
    }
    return 0;
}

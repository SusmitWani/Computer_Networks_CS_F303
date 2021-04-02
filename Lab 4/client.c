#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/stat.h>
#include <fcntl.h>
int main(){
    int gg;
    char* fifo = "/tmp/myfifo";
    mkfifo(fifo, 0666);
    char send1[200], receive1[200];
    int server_online = 1;
    while(1){
        //Sending a message
        if(server_online==0){
            printf("Client: ");
            fgets(send1, 200, stdin);
            if(strcmp(send1, "exit\n") == 0){
                close(gg);
                break;
            }
            printf("Message not delivered. Client is offline\n\n");
        }
        else{
            gg = open(fifo, O_WRONLY);
            printf("Client: ");
            fgets(send1, 200, stdin);
            write(gg, send1, strlen(send1)+1);
            close(gg);
            if(strcmp(send1, "exit\n") == 0){
                close(gg);
                break;
            }
            printf("\n");
        }

        //Receiving a message
        if(server_online == 0){
            // printf("Server: I am OFFLINE text back later\n");
        }
        else{
            gg = open(fifo, O_RDONLY);
            read(gg, receive1, 200);
            if(strcmp(receive1, "exit\n") == 0){
                server_online=0;
                printf("Server has gone offline\n\n");
            }
            else{
                printf("Server: %s\n", receive1);
            }
            close(gg);
        }
    }
    return 0;
}

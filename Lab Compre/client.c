#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<signal.h>

#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define SA struct sockaddr
#define MAX_CLIENTS 2

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

int padding = RSA_PKCS1_PADDING;
RSA * createRSA(unsigned char * key,int public) {
    RSA *rsa= NULL;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL) {
        red();
        printf( "\n>>>> Failed to create key BIO");
        reset();
        return 0;
    }
    if(public) {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    }
    else {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }
    if(rsa == NULL) {
        red();
        printf("\n>>>> Could not create RSA");
        reset();
    }
    return rsa;
}

int encode(RSA *rsa, unsigned char message[300], unsigned char buffer[300]){
    bzero(buffer, 300);
    int encrypted_len = RSA_public_encrypt(RSA_size(rsa)-11, message,buffer,rsa,padding);
    if(encrypted_len==-1) {
        red();
        printf("\n>>>> Encryption failed");
        reset();
        return 1;
    }
    // printf("\n");
    return encrypted_len;
}
int decode(RSA* rsa, unsigned char message[300], unsigned char buffer[300]){
    bzero(buffer, 300);
    int decrypted_len = RSA_private_decrypt(RSA_size(rsa),message,buffer,rsa,padding);
    if(decrypted_len==-1){
        red();
        printf("\n>>>> Decryption failed");
        reset();
        return 1;
    }
    // printf("\n>>>> Plaintext Message: ");
    // fwrite(buffer, 1, 256, stdout);
    // printf("\nDecrypted Message: %s\n", buffer);
    return 0;
}

int check_dc(int soc_stat) {
    int conn = 0;
    read(soc_stat, &conn, sizeof(conn));
    yellow();
    printf("\n>>>> Connections to server are %d", ntohl(conn));
    reset();
    if(ntohl(conn)>MAX_CLIENTS){
        yellow();
        printf("\n>>>> Too many clients connected. Try again later.\n");
        reset();
        return 0;
    }
    return 1;
}
int send_chat(int soc_stat, RSA *public_key) {
    //send message
    unsigned char message[300];
    unsigned char send[300];
    bzero(send, 300);
    bzero(message, 300);
    printf("\n>>>> ");
    fgets((char* restrict)send, 220, stdin);
    encode (public_key, send, message);
    // printf("\n>>>> Length of encoded message: %d", len_enc);
    write(soc_stat, message, 256);
    // printf("GG value: %d\n", gg);
    // printf("Errno: %d\n", errno);
    if(strcmp((const char*)send, "exit\n")==0){
        return 0;
    }
    return 1;
}
int receive_chat(int soc_stat, RSA *private_key) {
    //receive message back
    fflush(stdout);
    unsigned char rec[300];
    unsigned char message[300];
    bzero(rec, 300);
    bzero(message, 300);
    // printf("reading message\n");
    read(soc_stat, rec, 256);
    decode(private_key, rec, message);
    if(strcmp((const char*)message, "exit\n")==0){
        red();
        printf("\n>>>> Connection terminated from other client\n");
        reset();
        return 0;
    }
    green();
    printf("\n>>>> Ciphertext Message: ");
    reset();
    fwrite(rec, 1, 256, stdout);
    green();
    printf("\n>>>> Plaintext message: ");
    reset();
    printf("%s\n>>>> ", message);
    fflush(stdout);
    return 1;
}

int main(int argc, char const *argv[]) {
    if (argc!=5){
        printf("Expected 4 arguments. Did not get 4.\nSyntax: ./client <ip> <port> <private key> <public key>\n");
        exit(EXIT_FAILURE);
    }
    pid_t childpid;
    pid_t pid = getpid();
    //Accept inputs
    int port = atoi(argv[2]);
    yellow();
    printf("\n>>>> Connecting to IP address: %s", argv[1]);
    printf("\n>>>> Attempting to connect to port: %d", port);
    reset();
    //Create socket
    int soc_stat = socket(AF_INET, SOCK_STREAM, 0);
    //error in creating socket
    if(soc_stat==-1){
        red();
        printf("\n>>>> Socket not created. Exiting.");
        reset();
        return 0;
    }
    //socket created successfully
    else{
        green();
        printf("\n>>>> Socket created successfully.");
        reset();
    }

    //Define server parameters
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(port);

    //actually connect
    if (connect(soc_stat, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        red();
        printf("\n>>>> Connection to server failed. Server unreachable.\n");
        reset();
        return 0;
    }
    else if(check_dc(soc_stat)==0){
        close(soc_stat);
        return 0;
    }
    else{
        green();
        printf("\n>>>> Connected to the server successfully\n");
        reset();
    }
    FILE* f_private  = fopen(argv[3], "r");
    if(f_private==NULL){
        red();
        printf("\n>>>> Error opening file\n");
        reset();
        exit(0);
    }
    unsigned char private_key_str[1700];
    fread(private_key_str, 1, 1700, f_private);
    fclose(f_private);

    FILE* f_public  = fopen(argv[4], "r");
    if(f_public==NULL){
        red();
        printf("\n>>>> Error opening file\n");
        reset();
        exit(0);
    }
    unsigned char public_key_str[500];
    fread(public_key_str, 1, 500, f_public);
    fclose(f_public);

    RSA *public_key = createRSA(public_key_str,1);
    // printf("\n>>>> Created public_key");
    RSA *private_key = createRSA(private_key_str,0);
    // printf("\n>>>> Created private_key");
    //Do the chat here
    if((childpid = fork()) == 0){
        while(1){
            if(send_chat(soc_stat, public_key)==0){
                red();
                printf("\n>>>> Closing connection\n");
                reset();
                kill(pid,SIGKILL);
                break;
            }
        }
    }
    else{
        while(1){
            if (receive_chat(soc_stat, private_key)==0){
                kill(childpid,SIGKILL);
                break;
            }
        }
    }

    return 0;
}

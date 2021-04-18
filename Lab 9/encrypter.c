#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int padding = RSA_PKCS1_PADDING;

RSA * createRSA(unsigned char * key,int public) {
    RSA *rsa= NULL;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL) {
        printf( "Failed to create key BIO");
        return 0;
    }
    if(public) {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    }
    else {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }
    if(rsa == NULL) {
        printf("Could not create RSA\n");
    }
    return rsa;
}

int main(int argc, char const *argv[]){
    //public key, input file, output file
    if(argc>4){
        printf("Too many arguments!\n");
        return 1;
    }
    if(argc<4){
        printf("Too few arguments. Must include public key, name of file and name of output file.\n");
        return 1;
    }

    // char public_key[] = argv[1];
    FILE* f = fopen(argv[2], "r");
    if(f==NULL){
        printf("Error opening input file\n");
        return 1;
    }
    printf("Input file opened successfully\n");
    char data[2000];
    bzero(data, 2000);
    fread(data, sizeof(data), 1, f);

    // printf("Input data: %s\n", data);
    fclose(f);
    // printf("Size of file: %dB\n", sizeof(data));
    printf("Size of file: %dB\n", strlen(data));
    if(strlen(data)>1024){
        printf("Warning! Data Size>1KB. Limit data size to 2KB. Data may be truncated if bigger files are used.\n");
    }
    RSA *rsa = createRSA((unsigned char*)argv[1],1);
    // printf("Size of RSA: %d\n", RSA_size(rsa));

    unsigned char encrypted_message[2600];
    unsigned char buffer[2200];
    //reset the file contents and then open for appending.
    f = fopen(argv[3], "w");
    if(f==NULL){
        printf("Error opening output file\n");
        return 1;
    }
    fclose(f);
    f = fopen(argv[3], "a+");
    if(f==NULL){
        printf("Error opening output file\n");
        return 1;
    }
    int c = (strlen(data)/200)+1;
    // printf("Number of iterations: %d\n", c);
    int i = c;
    printf("Starting the encryption Process\n");
    while(i>0){
        // printf("\n");
        bzero(encrypted_message, 2600);
        bzero(buffer, 2200);
        strncpy(buffer, data+(c-i)*200, 200);
        // printf("Length of buffer: %d\n", strlen(buffer));
        // printf("Buffer: %s\n", buffer);
        int encrypted_len = RSA_public_encrypt(strlen(buffer),buffer,encrypted_message,rsa,padding);
        if(encrypted_len==-1) {
            printf("Encryption failed\n");
            return 1;
        }
        // printf("Length of encrypted part %d: %d\n", c-i, encrypted_len);
        fwrite(encrypted_message, sizeof(char), 256, f);
        i--;
    }
    printf("Successfully encrypted message!\n");
    fclose(f);
    return 0;
}

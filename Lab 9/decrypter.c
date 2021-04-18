#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SA struct sockaddr

int padding = RSA_PKCS1_PADDING;

RSA * createRSA(unsigned char * key,int public) {
    RSA *rsa= NULL;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL) {
        printf( "Failed to create key BIO");
        return 0;
    }
    if(public==1) {
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

int main(int argc, char const *argv[]) {
    //private key, encrypted file, decrypted file
    if(argc>4){
        printf("Too many arguments!\n");
        return 1;
    }
    if(argc<4){
        printf("Too few arguments. Must include private key, name of encrypted file and name of output file.\n");
        return 1;
    }

    printf("Encrypted file opened successfully\n");
    RSA * rsa = createRSA((unsigned char*)argv[1],0);
    unsigned char encrypted_message[2600] = {"\0"};
    unsigned char buffer[220] = {"\0"};
    FILE* f = fopen(argv[3], "w");
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
    int i = 0;
    printf("Starting the decryption Process\n");
    while(1){
        bzero(buffer, 220);
        bzero(encrypted_message, 2600);
        FILE* f_read = fopen(argv[2], "r");
        fseek(f_read, 256*i, SEEK_SET);
        if(f_read==NULL) {
            printf("Error opening file containing encrypted message\n");
            return 1;
        }
        int b_read = fread(encrypted_message, sizeof(char), 256, f_read);
        // printf("Length of Encrypted packet: %d\n", sizeof(encrypted_message));
        int decrypted_len = RSA_private_decrypt(RSA_size(rsa),encrypted_message,buffer,rsa,padding);
        if(decrypted_len==-1){
            printf("Decryption failed\n");
            return 1;
        }
        // fprintf(f, "%s\n", decrypted_message);
        fwrite(buffer, sizeof(char), decrypted_len, f);
        // printf("Length of decrypted message: %d\n", decrypted_len);
        if(decrypted_len!=200){
            break;
        }
        i++;
        fclose(f_read);
    }
    fclose(f);
    printf("Successfully decrypted message!\n");
    return 0;
}

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define SA struct sockaddr

int main(int argc, char const *argv[])
{
    if(argc>2){
        printf("Too many arguments. Just the website needed.\n");
        return 0;
    }

    if(argc==1){
        printf("Please enter the website!\n");
        return 0;
    }


    // http://www.columbia.edu/~fdc/sample.html
    char url[1024];
    strcpy(url, argv[1]);

    // char * url=argv[1];
    char * proto=strtok(url,"://");
    char * domain=strtok(NULL,"/");
    char * path=strtok(NULL,"\n");

    printf("\nURL: %s\n",argv[1]);
    printf("Protocol: %s\n", proto);
    printf("Domain: %s\n", domain);
    printf("Path: %s\n", path);

    struct hostent *ghbn = gethostbyname(domain);
    if (ghbn) {
        printf("Host Name->%s\n", ghbn->h_name);
        printf("IP ADDRESS->%s\n",inet_ntoa(*(struct in_addr *)ghbn->h_name) );
    }

    if (strcmp(proto, "http")==0) {
        char port[] = "80";

        //Define server parameters
        struct sockaddr_in server_addr;

        struct addrinfo *result;
        struct addrinfo temp = {0};


        temp.ai_family = AF_INET;
        temp.ai_socktype = SOCK_STREAM;
        int error = getaddrinfo(domain, port, &temp, &result);
        if (error != 0){
            fprintf(stderr, "Website unavailable/unreachable.\nERROR CODE: %d", error);
            return EXIT_FAILURE;
        }

        //Create socket
        int soc_stat = socket(AF_INET, SOCK_STREAM, 0);
        if(soc_stat==-1){
            printf("Socket not created. Exiting\n");
            return 0;
        }
        else{
            printf("Socket created successfully.\n");
        }

        // actually connect
        if (connect(soc_stat, result->ai_addr, result->ai_addrlen) != 0) {
            printf("Connection to server failed. Server unreachable.\n");
            return 0;
        }
        else
            printf("connected to the server successfully\n");


        char send1[] = "GET /";
        char send2[] = " HTTP/1.1\r\nHost: ";
        char send3[] = "\r\nConnection: close\r\n\r\n";
        int lenn = strlen(send1) + strlen(path) + strlen(send2) + strlen(domain) + strlen(send3);
        char send_[lenn];
        for(int i=0; i<lenn; i++){
            if(i<strlen(send1)){
                send_[i] = send1[i];
            }
            else if(i<strlen(send1) + strlen(path) && i>=strlen(send1)){
                send_[i] = path[i-strlen(send1)];
            }
            else if(i<strlen(send1) + strlen(path) + strlen(send2) && i>=strlen(send1) + strlen(path)){
                send_[i] = send2[i-(strlen(send1) + strlen(path))];
            }
            else if(i<strlen(send1) + strlen(path) + strlen(send2) + strlen(domain) && i>=strlen(send1) + strlen(path) + strlen(send2)){
                send_[i] = domain[i-(strlen(send1) + strlen(path) + strlen(send2))];
            }
            else{
                send_[i] = send3[i-(strlen(send1) + strlen(path) + strlen(send2) + strlen(domain))];
            }
        }
        printf("The send req is:");

        for(int i=0; i<lenn; i++){
            printf("%c", send_[i]);
        }
        printf("Sending the GET request to the website\n");

        send(soc_stat, send_, strlen(send_), 0);

        const char s[2] = "/";
        char* token;
        char token_prev[100];
        token = strtok(argv[1], s);
        while(token != NULL) {
            strcpy(token_prev, token);
            token = strtok(NULL, s);
        }
        printf("%s\n", token_prev);

        FILE *f;
        f = fopen(token_prev, "wb");
        char receive[20000];
        int ret_size=1;
        char* p;

        int count = 0;
        while (ret_size > 0) {
            ret_size = recv(soc_stat, receive, 10000, 0);
            if(count==0) {
                p = strstr(receive, "\r\n\r\n");
                p=p+4;
                printf("size of p: %ld\n", strlen(p));
            }
            if (ret_size == -1) {
                printf("Can't download file. No entities received.\n");
                return -1;
            }
            else {
                if(count==0){
                    fwrite(p,strlen(p), 1,f);
                    count++;
                }
                else
                    fwrite(receive, 1, sizeof(receive), f);
                printf("File download successful!\n");
            }
        }

        close(soc_stat);
    }

    if (strcmp(proto, "https")==0) {
        SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
        // SSL_CTX *ctx = SSL_CTX_new(TLSv1_2_client_method());
        // SSL_CTX_set_ecdh_auto(ctx, 1);
        if (ctx == NULL) {
            printf("errored; unable to load context.\n");
            ERR_print_errors_fp(stderr);
            return -3;
        }

        BIO *bio = BIO_new_ssl_connect(ctx);

        SSL *ssl;
        BIO_get_ssl(bio, &ssl);
        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

        printf("hostname: %s\n", domain);
        BIO_set_conn_hostname(bio, strcat(domain, ":https"));
        printf("Hostname set\n");

        if (BIO_do_connect(bio) <= 0) {
            BIO_free_all(bio);
            printf("errored; unable to connect.\n");
            ERR_print_errors_fp(stderr);
            return -2;
        }


        char url[1024];
        strcpy(url, argv[1]);

        // char * url=argv[1];
        char * proto=strtok(url,"://");
        char * domain=strtok(NULL,"/");
        char * path=strtok(NULL,"\n");
        printf("\nURL: %s\n",argv[1]);
        printf("Protocol: %s\n", proto);
        printf("Domain: %s\n", domain);
        printf("Path: %s\n", path);

        char send1[] = "GET /";
        char send2[] = " HTTP/1.0\r\nHost: ";
        char send3[] = "\r\nConnection: close\r\n\r\n";
        int lenn = strlen(send1) + strlen(path) + strlen(send2) + strlen(domain) + strlen(send3);
        char send_[lenn];
        for(int i=0; i<lenn; i++){
            if(i<strlen(send1)){
                send_[i] = send1[i];
            }
            else if(i<strlen(send1) + strlen(path) && i>=strlen(send1)){
                send_[i] = path[i-strlen(send1)];
            }
            else if(i<strlen(send1) + strlen(path) + strlen(send2) && i>=strlen(send1) + strlen(path)){
                send_[i] = send2[i-(strlen(send1) + strlen(path))];
            }
            else if(i<strlen(send1) + strlen(path) + strlen(send2) + strlen(domain) && i>=strlen(send1) + strlen(path) + strlen(send2)){
                send_[i] = domain[i-(strlen(send1) + strlen(path) + strlen(send2))];
            }
            else{
                send_[i] = send3[i-(strlen(send1) + strlen(path) + strlen(send2) + strlen(domain))];
            }
        }
        printf("The send req is:");

        for(int i=0; i<lenn; i++){
            printf("%c", send_[i]);
        }
        printf("Sending the GET request to the website\n");

        if (BIO_puts(bio, send_) <= 0) {
            BIO_free_all(bio);
            printf("errored; unable to write.\n");
            ERR_print_errors_fp(stderr);
            return -1;
        }

        const char s[2] = "/";
        char* token;
        char token_prev[100];
        token = strtok(argv[1], s);

        while(token != NULL) {
            strcpy(token_prev, token);
            token = strtok(NULL, s);
        }
        printf("%s\n", token_prev);

        FILE *f;
        f = fopen(token_prev, "wb");
        char receive[20000];

        // //Remove header
        int len = BIO_read(bio, receive, 10000);
        char* p;
        p = strstr(receive, "\r\n\r\n");
        p=p+4;
        printf("size of p: %ld\n", strlen(p));
        if (len == 0) {
            printf("Nothing received!\n");
            return 0;
        }
        else if (len < 0) {
            if (!BIO_should_retry(bio)) {
                printf("errored; read failed.\n");
                ERR_print_errors_fp(stderr);
                return 0;
            }
        }
        else {
            fwrite(p,strlen(p), 1,f);
            printf("File download successful!\n");
        }

        //Don't worry about header
        for (;;) {
            int len = BIO_read(bio, receive, 10000);
            if (len == 0) {
                break;
            }
            else if (len < 0) {
                if (!BIO_should_retry(bio)) {
                    printf("errored; read failed.\n");
                    ERR_print_errors_fp(stderr);
                    break;
                }
            }
            else {
                fwrite(receive, 1, sizeof(receive), f);
                printf("File download successful!\n");
            }
        }

        BIO_free_all(bio);
    }

    return 0;
}

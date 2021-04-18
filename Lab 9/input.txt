One file is given
1. client.c

Commands to run for c programs:

1. Client terminal:
>>cd "Desktop/CN Lab 7"
>>gcc -o client.out client.c -lssl -lcrypto -w
>>./client.out [url]

Format followed for the URL: http(s)://www.websitename.com/folder/subfolder/subfolder/filename.extension

Websitename can be anything. The website extension need not be .com. It can be .in or .org or anything. .com just as an example. Folders, subfolders should eventually lead to the file we want to download. Extension can be .txt, .html, .jpg, .png, .pdf or anything.

eg: https://www.openssl.org/docs/man1.1.0/man3/BIO_set_conn_hostname.html
eg: https://www.bits-pilani.ac.in/uploads/PSD/PS_I_Chronicles/2020/IT.pdf


The client accepts the path to teh file as a CLI. It establishes a connection to the website, if possible, and downloads and saves the file in the same directory as the executable. If not, it will give an error message. After execution, the client exits the program.

README file for Lab 9 Computer Networks Lab Second Semester 2020-2021 
Author: Susmit Wani
ID No.: 2018A7PS0116G
Date of Submission: 18/04/2021

The code submitted has been tested and is working without any errors on Arch Linux kernel version 5.11.14-arch1-1 on gcc version 10.2.0

The Question Statement: 
Write a program that uses public key-private key encryption to encrypt and decrypt a text file.
The maximum size of the file is 1Kbytes.

1. The encryption program accepts public key (1 mark), name of the file (1 mark), and name of the output file (1 mark) as command line input parameters. It encrypts the file (4 marks)and stores it as the given name of the output file (1 mark).

2. The decryption program accepts private key (1 mark), name of the encrypted file (1 mark), and name of the decrypted file (1 mark) as command line input parameters. It decrypts the file (4 marks) and stores it as the given name of the output file (1 mark).

Submit code of encryption program, description program, a file containing private key (1 mark), a file containing corresponding public key (1 mark), a text file (1 mark), its encryption using the submitted private key (1 mark), and README file (2 marks).

The README file should mention how to compile your code and how to execute your code.

You may use OpenSSL library for encryption and decryption. You cannot use any existing commands, such as GnuPG, for encryption and decryption.

The list of files provided:
1. encrypter.c
2. decrypter.c
3. PublicKey.txt
4. PrivateKey.txt
5. input.txt
6. encrypted.txt
7. README.txt

Steps to run the code
1. Generating the Public and Private Keys: 
	
>>> openssl genrsa -out private.pem 2048
>>> openssl rsa -in private.pem -outform PEM -pubout -out public.pem

This will generate the public and private keys which were copied into the text files. 

2. Encrypting the input file:
	Make sure the input.txt file is in the same directory as encrypter.c. 
	
Steps to compile and run the file are as follows

>>> gcc -o encrypt encrypter.c -lcrypto -lssl
>>>./encrypt "-----BEGIN PUBLIC KEY-----     
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw/vQizScjBTI1LRVmmDr 
9RLOLWhz0mZKFys+VD1aRLapCtAuj0kT6tUHe/lDzy85Woa4HxYnKuegVzcjFZTE 
nTLO3dZ8lshYfGZQxB/6dEUoe03ydzJzTkyAUW36hT3Oal7PwPk0B2H3QHB/jOaq 
x7h6Jcpampgpy/SjahphpZOqL1zll6DfyBvzGIBneXPlh/HiPWkoIJpffO2KMJSy 
Db8gb5RTTUMaPi+hK6bHoGNohmjYPZRkpj7YDflojsf+PI0oFnXaON608kPGEuS+ 
EMQcuO5Yd5ysBb/eWyDMS59di8kTnX2stcwCTaPJEum8pCBUyBUmk/o3R9u4DYsq 
hwIDAQAB 
-----END PUBLIC KEY----- 
" input.txt encrypted.txt

	This will generate the encrypted file which we will later decrypt.

3. Decrypting the encrypted file:
	Make sure the encrypted.txt file is in the same directory as decrypter.c

Steps to compile and run the file are as follows

>>> gcc -o decrypt decrypter.c -lcrypto
>>> ./decrypt "-----BEGIN RSA PRIVATE KEY----- 
MIIEpAIBAAKCAQEAw/vQizScjBTI1LRVmmDr9RLOLWhz0mZKFys+VD1aRLapCtAu 
j0kT6tUHe/lDzy85Woa4HxYnKuegVzcjFZTEnTLO3dZ8lshYfGZQxB/6dEUoe03y 
dzJzTkyAUW36hT3Oal7PwPk0B2H3QHB/jOaqx7h6Jcpampgpy/SjahphpZOqL1zl 
l6DfyBvzGIBneXPlh/HiPWkoIJpffO2KMJSyDb8gb5RTTUMaPi+hK6bHoGNohmjY 
PZRkpj7YDflojsf+PI0oFnXaON608kPGEuS+EMQcuO5Yd5ysBb/eWyDMS59di8kT 
nX2stcwCTaPJEum8pCBUyBUmk/o3R9u4DYsqhwIDAQABAoIBAEmMsFf9t55ukmwY 
/WNBamv/0baC06yrTLTjOrH8kGrnLgtr031cjIf9yKBDDKqta1XI50JrWumkIa15 
eMY5nYvtLGteekVQDL0q3Lat1Z1a8vVLbGpFWbaRyTpGDtWAO3359PfTzi3TGypo 
zcWAL6liVxzlkxkfb9gmCEnierDbUKRq5mr32I1A94zmVU1pxv1C1RkeTP2d5oyq 
+H4zI9ZsVqy6OKqYzXsz/WmFeYX0bmLCiDw0Pr0dpXRl2ZSJTt/yi/1J9LCoalqE 
2I+u9j2QiQybDpWPoGV6/J33vn1EGiXlnTkJr8Gd2gHiEaAEF9h05nUXFMyPNFuk 
Rb2NMHECgYEA6eJA00o2F0YLQnvr+JqWeupw0Cv3IDZAxh5X5CCTCwM33NUP8O4F 
hTIbjIuilsXP8+fiqVvjAzTZ6RndfK1LTfenPR3JpoLJf6XIJzdJtoO4X5rMGf0l 
Xj5b9lPKtMX9GM37cM67HnMLYi7c95OXQTrUvjn1/Ko38X2GbvviPx8CgYEA1oQX 
vkO7/wxWm0vSD3fBnGDi4GtnKcaI+jc8DLM8/d0tGoUjjybc0qfeqDlwyz2KqB59 
E10s/nawMTQ2bAcYNbnNp/ELMPen8uR3rMIcA48ZQ3iAE2atUzNdg7NQbMISEC2K 
0mDwiGBxINtWoQNbxJwCOqMBmA+VcYy/yvhrb5kCgYEAnCVrtiSznDItTkPBtQYw 
NPvUOYdAxAa0lBCqCSoeQZnyb8Lnk6i1kslzTw/iitqiDWrhDsTD+N4OJewYzSY9 
Ql53DvyWJk7nDAu4Z0YoNmwWae3FGgzCJ/tmFjy29p3AbhEHL7c1ggy4quIysC+f 
KfOHGUFFE1zpYJa3a91KnW0CgYBNcoFT+wZrrZuzE6uyU+09tSkUFxK23Wvk0n5G 
o4uYp/q5wWm2LDS7hUn4+EwFxKFD00Ya8J/m+xjfmtcvJ1yRQRTD1l3Q/wjOw7eF 
UstkaYyYdG/hEUfwavIPM3UM3wBAgQDGtaMBybfuQeJD3X+LOzv+yY0wxieRSFyS 
PiAjuQKBgQDDLriwSHt9s2cEapuVKeKWwUqpgBOoXGwMHnrhH7VD/9bRyXGFVf4N 
FKpwh1jPE0o4OOVgD757MFedNNw6lcWpQQNYnCh3h/aEFL8Fx5Exz+Ir1V5iZvc/ 
ipmuwRTvJN3gpYOqrPAf/3T2dknQ923ULgGFzmHuXCRrakKRP8nXBQ== 
-----END RSA PRIVATE KEY----- 
" encrypted.txt decrypted.txt

This will generate the decrypted file which will be same as input.txt given it is within the given buffer size.

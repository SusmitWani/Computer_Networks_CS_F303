Three files are given.
1) client.c
2) server.c
3) makefile

Compile and run both files in two separate terminals.

Steps to compile and run:
1. client.c
    >>> make client

2. server.c
    >>> make server

3. Deleting executables
    >>> make clean

This code assumes that both client and server will be online initially.

The client terminal will be the first to message the server. Type in your message and once we press enter, the message will appear in both the terminals. The next message will be sent by the server.

Once we type exit in either terminal, the respective program will terminate and an indication will be sent to the other terminal. The other terminal can send messages but will not be able to get them delivered. This may get the pipe clogged and cause delay in sending further messages.

Note: The pdf file contains screenshots of an older implementation. Newer implementation may look a little different.

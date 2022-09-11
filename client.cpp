#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr server_addr;
    if (sock < 0)
    {
        perror("Creating the socket had the following error\n");
    }
    printf("Hello from client\n");
    return 0;
}

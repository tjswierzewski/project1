#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

#define MAX_LENGTH 16384
#define PORT 27995

int main(int argc, char const *argv[])
{
    if (argc < 4 || argc > 6)
    {
        cout << "Usage: <-p port> <-s> [hostname] [Northeastern-username]" << endl;
    }
    string username;
    struct hostent *host = NULL;
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    int encrypted = 0, port = PORT;
    for (size_t i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            server_addr.sin_port = (short)stoi(argv[i++]);
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            encrypted = 1;
        }
        else
        {
            if (host == NULL)
            {
                host = gethostbyname(argv[i]);
                if (host == NULL)
                {
                    perror("Getting host by name caused the following error:\n");
                    return -1;
                }
                server_addr.sin_addr = *(struct in_addr *)host->h_addr_list[0];
            }
            else
            {
                username = argv[i];
            }
        }
    }

    if (!encrypted)
    {
        cout << "Must be encrypted. Run with -s.";
        return -1;
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Creating the socket had the following error:\n");
        return -1;
    }
    int client_fd = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (client_fd < 0)
    {
        perror("Socket connection threw the following error\n");
        return -1;
    }

    cout << "Hello from client" << endl;
    return 0;
}

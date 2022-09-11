#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace std;

#define MAX_LENGTH 16384
#define PORT 27995

int main(int argc, char const *argv[])
{
    if (argc < 3 || argc > 5)
    {
        cout << "Usage: <-p port> <-s> [hostname] [Northeastern-username]" << endl;
    }
    string username, hostname;
    int encrypted = 0, port = PORT;
    for (size_t i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            port = stoi(argv[i++]);
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            encrypted = 1;
        }
        else
        {
            if (hostname.empty())
            {
                hostname = argv[i];
            }
            else
            {
                username = argv[i];
            }
        }
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr server_addr;
    if (sock < 0)
    {
        perror("Creating the socket had the following error\n");
    }

    cout << "Hello from client" << endl;

    return 0;
}

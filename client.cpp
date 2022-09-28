#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <array>
#include "ap.hpp"

using namespace std;

#define MAX_LENGTH 16384
#define PORT 27995

ap_int<1024> evaluate(string txpr);
/**
 * Function is responsible for creating sock file descriptor and connecting to server.
 */
int connectSocket(struct sockaddr_in addr)
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Creating the socket had the following error");
        abort();
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Socket connection threw the following error");
        abort();
    }

    return sock;
}
/**
 * Function is responsible for setting up SSL context to be used in session.
 */
SSL_CTX *InitCTX(void)
{
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
    }
    return ctx;
}
/**
 * Function is responsible for parsing server messages into 3 strings.
 */
void parseMessage(string message, string *output)
{
    for (size_t i = 0; i < 2; i++)
    {
        int parse = message.find(" ");
        output[i] = message.substr(0, parse);
        message.erase(0, parse + 1);
    }
    output[2] = message;
}

int main(int argc, char const *argv[])
{
    char buffer[MAX_LENGTH];
    // Check args are correct length
    if (argc < 4 || argc > 6)
    {
        exit(1);
    }
    // Parse args for TLS status, Host, Port and username
    string username;
    struct hostent *host = NULL;
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    int encrypted = 0;
    for (size_t i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            server_addr.sin_port = htons(atoi(argv[++i]));
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
                server_addr.sin_addr.s_addr = *(long *)host->h_addr_list[0];
            }
            else
            {
                username = argv[i];
            }
        }
    }
    // Connect to socket
    int sock = connectSocket(server_addr);

    // Set up TLS if encrypted
    SSL_CTX *ctx;
    SSL *ssl;
    if (encrypted)
    {
        SSL_library_init();
        ctx = InitCTX();

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, sock);
        if (SSL_connect(ssl) == -1)
        {
            ERR_print_errors_fp(stderr);
            abort();
        }
    }

    string prefix = "cs5700fall2022";

    // Handler for messages
    string message[3];
    while (message[1] != "BYE")
    {
        ap_int<1024> answer;
        string output;
        string status;
        if (message[1] == "EVAL")
        {
            try
            {
                status = "STATUS";
                // Found in evaluate.cpp
                answer = evaluate(message[2]);
                output = string(answer);
            }
            catch (const std::exception &e)
            {
                status = "ERR";
                output = "#DIV/0";
            }
        }
        else if (message[1] == "")
        {
            status = "HELLO";
            output = username;
        }
        else if (message[1] == "BYE")
        {
            break;
        }
        else
        {
            break;
        }

        // Create full message
        string full_message = prefix + " " + status + " " + output + '\n';

        // Send and receive message over TLS or basic TCP depending on encryption flag.
        int rc;
        if (encrypted)
        {
            SSL_write(ssl, full_message.c_str(), full_message.length());
            rc = SSL_read(ssl, buffer, MAX_LENGTH);
            while (rc < MAX_LENGTH && buffer[rc - 1] != '\n')
            {
                rc += SSL_read(ssl, buffer + rc, MAX_LENGTH - rc);
            }
        }
        else
        {
            write(sock, full_message.c_str(), full_message.length());
            rc = read(sock, buffer, MAX_LENGTH);
            while (rc < MAX_LENGTH && buffer[rc - 1] != '\n')
            {
                rc += read(sock, buffer + rc, MAX_LENGTH - rc);
            }
        }

        buffer[rc] = '\0';
        // Parse received message for processing
        parseMessage(buffer, message);

        // Save last argument of message for debug
        if (message[2] != "\n")
        {
            ofstream myfile;
            myfile.open("problem.txt", ios::trunc);
            myfile << message[2];
            myfile.close();
        }
    }

    // Print secret_flag
    cout << message[2] << endl;

    // Close connection
    close(sock);
    return 0;
}

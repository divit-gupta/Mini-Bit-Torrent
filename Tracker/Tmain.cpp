#include "Theader.h"

using namespace std;

int main(int argc, char *argv[])
{

    // For checking number of command line arguments
    // if(argc != 3)
    // {
    //     cout << "Invalid number of arguments";
    //     return 0;
    // }
    pthread_t thread_id;


    char tra_port[] = "8080";
    char tra_ip[] = "127.0.0.1";
    // cout <<tra_port << " " << tra_ip;


    int trackerIP = inet_addr(tra_ip);
    int trackerPort = atoi(tra_port);
    int serverFD, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockoptIP");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockoptPORT");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; // Protocol = IPv4
    address.sin_addr.s_addr = trackerIP;
    address.sin_port = htons(trackerPort);


    if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // 10 - Maxm number of clients
    if (listen(serverFD, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        cout << "[*] Listening\n";
        if ((newSocket = accept(serverFD, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Error in accept connection");
            exit(EXIT_FAILURE);
        }
        char buffer[1024];
        if(read(newSocket , buffer, 1024) < 0)
        {
            perror("Error reading message");
        }
        cout << "[*] Connected to " << buffer << endl;

    }
    return 0;
}

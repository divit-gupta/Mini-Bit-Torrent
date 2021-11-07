#include "Cheader.h"

using namespace std;

int main(int argc, char *argv[])
{

    char tra_ip[] = "127.0.0.1";
    char tra_port[] = "8080";
    char cl_ip[] = "127.0.0.1";
    char cl_port[] = "6000";

    string trackerIPstr(tra_ip);
    string trackerPORTstr(tra_port);
    string clientIPstr(cl_ip);
    string clientPORTstr(cl_port);

    int trackerIP = inet_addr(tra_ip);
    int trackerPORT = atoi(tra_port);
    int clientIP = inet_addr(cl_ip);
    int clientPORT = atoi(cl_port);

    string passIP_PORT = clientIPstr + ":" + clientPORTstr;

    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        cout << "Client Side: Socket creation error\n";
        return -1;
    }

    struct sockaddr_in serv_addr;

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(trackerPORT);

    //Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, cl_ip, &serv_addr.sin_addr) <= 0)
    {
        cout << "Client: Invalid address/ Address not supported\n";
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "Client: Connection Failed\n";
        return -1;
    }
    cout << "[*]Connected to Tracker: " << trackerIPstr << ":" << trackerPORTstr << "\n";
    if(send(sock, &passIP_PORT[0], passIP_PORT.length(),0) < 0)
    {
        cout << "Client: send error\n";
        return -1;
    }
    return 0;
}

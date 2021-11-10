#include "Cheader.h"

using namespace std;

void tokenize(vector<string> &cmd, string command)
{
    unsigned long int ind = 0;
    string temp = "";
    while (ind < command.length())
    {
        if (command[ind] == ' ')
        {
            if (temp != "")
                cmd.push_back(temp);
            temp = "";
            ind++;
            continue;
        }
        if (command[ind] == '\\')
            ind++;
        temp += command[ind];
        ind++;
    }
    if (temp != "")
        cmd.push_back(temp);

    // TO TEST
    // cout << "command:" << command << endl;
    // for(int i = 0; i < cmd.size(); i++)
    //     cout << cmd[i] << endl;
}

string check_send_read_reply(int sock, string command, int n, int m)
{
    if (n != m)
        return "INCORRECT NUMBER OF ARGUMENTS";
    if (send(sock, &command[0], command.length(), 0) < 0)
        return "Client: Error sending message";
    char buffer[1024] = {0};
    if (read(sock, buffer, 1024) < 0)
        return "Client: Error reading message";
    string reply(buffer);
    // if(reply == "TRACKER QUIT")
    // {
    //     cout << "[*] TRACKER QUIT";
    //     exit(0);
    // }
    return reply;
}

//****************************************************************************************************

void create_user(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 3);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void login(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 3);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void list_groups(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 1);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void create_group(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 2);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void join_group(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 2);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void leave_group(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 2);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void list_requests(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 2);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void accept_request(int sock, vector<string> &cmd, string command)
{
    string replyFromTracker = check_send_read_reply(sock, command, cmd.size(), 3);
    cout << "[*] " << replyFromTracker << endl << endl;
}

//****************************************************************************************************

void list_files(int sock, vector<string> &cmd, string command)
{
}

//****************************************************************************************************

void upload_file(int sock, vector<string> &cmd, string command)
{
}

//****************************************************************************************************

void download_file(int sock, vector<string> &cmd, string command)
{
}

//****************************************************************************************************

void show_downloads(int sock, vector<string> &cmd, string command)
{
}

//****************************************************************************************************

void stop_share(int sock, vector<string> &cmd, string command)
{
}

//****************************************************************************************************

void logout(int sock, vector<string> &cmd, string command)
{
}

//****************************************************************************************************

void close_con(int sock, vector<string> &cmd, string command)
{
    if (cmd.size() != 1)
        cout << "INCORRECT NUMBER OF ARGUMENTS\n\n";
    if (send(sock, &command[0], command.length(), 0) < 0)
        cout << "Client: Error sending message\n\n";
    exit(0);
}

//****************************************************************************************************

void execute_cmd(string op, int sock, vector<string> &cmd, string command)
{
    if (op == "EXIT")
        close_con(sock, cmd, command);
    else if (op == "create_user")
        create_user(sock, cmd, command);
    else if (op == "login")
        login(sock, cmd, command);
    else if (op == "list_groups")
        list_groups(sock, cmd, command);
    else if (op == "create_group")
        create_group(sock, cmd, command);
    else if (op == "join_group")
        join_group(sock, cmd, command);
    else if (op == "leave_group")
        leave_group(sock, cmd, command);
    else if (op == "list_requests")
        list_requests(sock, cmd, command);
    else if (op == "accept_request")
        accept_request(sock, cmd, command);
    else if (op == "list_files")
        list_files(sock, cmd, command);
    else if (op == "upload_file")
        upload_file(sock, cmd, command);
    else if (op == "download_file")
        download_file(sock, cmd, command);
    else if (op == "show_downloads")
        show_downloads(sock, cmd, command);
    else if (op == "stop_share")
        stop_share(sock, cmd, command);
    else if (op == "logout")
        logout(sock, cmd, command);
    else
        cout << "[*] INVALID COMMAND\n\n";
}

//****************************************************************************************************

int main(int argc, char *argv[])
{
    system("clear");

    // For checking number of command line arguments
    if (argc != 5)
    {
        cout << "Invalid number of arguments";
        return 0;
    }

    // char tra_ip[] = "127.0.0.1";
    // char tra_port[] = "8080";
    // char cl_ip[] = "127.0.0.1";
    // char cl_port[] = "6000";

    // char tra_ip[] = "127.0.0.1";
    // char tra_port[] = "8080";
    // char cl_ip[] = "127.0.0.1";
    // char cl_port[] = "6000";

    string trackerIPstr(argv[1]);
    string trackerPORTstr(argv[2]);
    string clientIPstr(argv[3]);
    string clientPORTstr(argv[4]);

    int trackerIP = inet_addr(argv[1]);
    int trackerPORT = atoi(argv[2]);
    int clientIP = inet_addr(argv[3]);
    int clientPORT = atoi(argv[4]);

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

    //////////////// CHANGE KRNA
    //Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, argv[3], &serv_addr.sin_addr) <= 0)
    {
        cout << "Client: Invalid address/ Address not supported\n";
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "Client: Connection Failed\n";
        return -1;
    }
    cout << "[*] Connected to Tracker: " << trackerIPstr << ":" << trackerPORTstr << "\n\n";
    if (send(sock, &passIP_PORT[0], passIP_PORT.length(), 0) < 0)
    {
        cout << "Client: send error\n";
        return -1;
    }

    string command;
    vector<string> cmd;
    cout << "Reminder: To delimit ' ' use '\\ ' and for '\\' use '\\\\' \n\n";
    while (1)
    {
        command = "";
        cmd.clear();
        cout << ": ";
        // flush(cout);
        getline(cin, command);
        tokenize(cmd, command);
        if (cmd.size())
            execute_cmd(cmd[0], sock, cmd, command);
        else
            cout << "[*] NO INPUT COMMAND\n\n";
    }
    return 0;
}

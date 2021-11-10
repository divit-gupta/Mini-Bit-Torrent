#include "Theader.h"

CLIENT client[max_client];
map<int, int> sock2idx;
map<string, int> uname2idx;
map<string, string> uname2pass;
map<string, int> grpOwner;
map<string, set<int> > grpMembers;
map<string, set<int> > pendingJoinReq;
// bool trackerQuit;

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

string create_user(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == true)
        status = "ALREADY CREATED A USER";
    else if (uname2pass.find(cmd[1]) != uname2pass.end())
        status = "USERNAME ALREADY IN USE";
    else
    {
        client[idx].isUser = true;
        client[idx].userName = cmd[1];
        uname2pass[cmd[1]] = cmd[2];
        status = "USER '" + cmd[1] + "' CREATED";
    }
    return status;
}

string login(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == false)
        status = "CREATE A USER FIRST";
    else if (client[idx].isLoggedIn == true)
        status = "ALREADY LOGGED IN AS '" + client[idx].userName + "'";
    else if (uname2pass.find(cmd[1]) == uname2pass.end() || uname2pass[cmd[1]] != cmd[2])
        status = "USERNAME OR PASSWORD IS INCORRECT";
    else if (uname2pass.find(cmd[1]) != uname2pass.end() && uname2pass[cmd[1]] == cmd[2])
    {
        client[idx].isLoggedIn = true;
        uname2idx[cmd[1]] = idx;
        status = "LOGGED IN AS '" + cmd[1] + "'";
    }
    return status;
}

string list_groups(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == false)
        status = "CREATE A USER FIRST";
    else if (client[idx].isLoggedIn == false)
        status = "YOU ARE NOT LOGGED IN";
    else if (grpOwner.size() == 0)
        status = "NO GROUPS AVAILABLE";
    else
    {
        status = "GROUP\t| OWNER\t\t| MEMBER COUNT";
        for (auto grp : grpOwner)
            status += "\n    " + grp.first + "\t| " + client[grp.second].userName + "\t| " + to_string(grpMembers[grp.first].size());
    }
    return status;
}

string create_group(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == false)
        status = "CREATE A USER FIRST";
    else if (client[idx].isLoggedIn == false)
        status = "YOU ARE NOT LOGGED IN";
    else if (grpOwner.find(cmd[1]) != grpOwner.end())
        status = "GROUP ID '" + cmd[1] + "' NOT UNIQUE";
    else if (grpOwner.find(cmd[1]) == grpOwner.end())
    {
        grpOwner[cmd[1]] = idx;
        grpMembers[cmd[1]].insert(idx);
        client[idx].grpID.insert(cmd[1]);
        status = "GROUP '" + cmd[1] + "' CREATED";
    }
    return status;
}

string join_group(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == false)
        status = "CREATE A USER FIRST";
    else if (client[idx].isLoggedIn == false)
        status = "YOU ARE NOT LOGGED IN";
    else if (grpOwner.find(cmd[1]) == grpOwner.end())
        status = "GROUP '" + cmd[1] + "' NOT FOUND";
    else if (client[idx].grpID.find(cmd[1]) != client[idx].grpID.end())
        status = "ALREADY A MEMBER OF GROUP '" + cmd[1] + "'";
    else if (grpOwner.find(cmd[1]) != grpOwner.end())
    {
        int own = grpOwner[cmd[1]];
        pendingJoinReq[cmd[1]].insert(idx);
        status = "JOIN REQUEST SENT TO '" + cmd[1] + "'";
    }
    return status;
}

string leave_group(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == false)
        status = "CREATE A USER FIRST";
    else if (client[idx].isLoggedIn == false)
        status = "YOU ARE NOT LOGGED IN";
    else if (grpOwner.find(cmd[1]) == grpOwner.end())
        status = "GROUP '" + cmd[1] + "' NOT FOUND";
    else if (client[idx].grpID.find(cmd[1]) == client[idx].grpID.end())
        status = "ALREADY NOT A MEMBER OF GROUP '" + cmd[1] + "'";
    else if (client[idx].grpID.find(cmd[1]) != client[idx].grpID.end())
    {
        if(grpOwner[cmd[1]] == idx)
        {
            for(auto memberIDX : grpMembers[cmd[1]])
                client[memberIDX].grpID.erase(cmd[1]);
            grpMembers.erase(cmd[1]);
            grpOwner.erase(cmd[1]);
            status = "GROUP '" + cmd[1] + "' NO MORE EXISTS";
        }
        else
        {
            client[idx].grpID.erase(cmd[1]);
            grpMembers[cmd[1]].erase(idx);
            status = "GROUP '" + cmd[1] + "' LEFT";
        }
    }
    return status;
}

string list_requests(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == false)
        status = "CREATE A USER FIRST";
    else if (client[idx].isLoggedIn == false)
        status = "YOU ARE NOT LOGGED IN";
    else if (grpOwner.find(cmd[1]) == grpOwner.end())
        status = "GROUP '" + cmd[1] + "' NOT FOUND";
    else if (idx != grpOwner[cmd[1]])
        status = "ACCESS DENIED. YOU ARE NOT OWNER";
    else if(pendingJoinReq[cmd[1]].size() == 0)
        status = "NO PENDING REQUESTS";
    else if(pendingJoinReq[cmd[1]].size() > 0)
    {
        status = "REQUESTS:";
        for(auto ind : pendingJoinReq[cmd[1]])
            status += "\n" + client[ind].userName;
    }
    return status;
}

string accept_request(vector<string> cmd, int idx)
{
    string status = "";
    if (client[idx].isUser == false)
        status = "CREATE A USER FIRST";
    else if (client[idx].isLoggedIn == false)
        status = "YOU ARE NOT LOGGED IN";
    else if (grpOwner.find(cmd[1]) == grpOwner.end())
        status = "GROUP '" + cmd[1] + "' NOT FOUND";
    else if (idx != grpOwner[cmd[1]])
        status = "ACCESS DENIED. YOU ARE NOT OWNER";
    // BEFORE CHECKING OWNER CHECKED PUBLIC INFO(info available for all)
    // IF OWNER THEN ONLY HE'LL SEE FOLLOWING OPTIONS
    else if(uname2idx.find(cmd[2]) == uname2idx.end())
        status = "USER '" + cmd[2] + "' DOES NOT EXIST";
    // uname2idx[cmd[2]] => idx of user to be accepted
    else if(pendingJoinReq[cmd[1]].find(uname2idx[cmd[2]]) == pendingJoinReq[cmd[1]].end())
        status = "NO SUCH PENDING REQUEST FOUND";
    else if(pendingJoinReq[cmd[1]].find(uname2idx[cmd[2]]) != pendingJoinReq[cmd[1]].end())
    {
        grpMembers[cmd[1]].insert(uname2idx[cmd[2]]);
        client[uname2idx[cmd[2]]].grpID.insert(cmd[1]);
        pendingJoinReq[cmd[1]].erase(uname2idx[cmd[2]]);
        status = "REQUEST ACCEPTED. \nUSER '" + cmd[2] + "' IS NOW A PART OF GROUP '" + cmd[1] + "'";
    }
    return status;
}

string list_files(vector<string> cmd, int idx)
{
    return "";
}

string upload_file(vector<string> cmd, int idx)
{
    return "";
}

string download_file(vector<string> cmd, int idx)
{
    return "";
}

string show_downloads(vector<string> cmd, int idx)
{
    return "";
}

string stop_share(vector<string> cmd, int idx)
{
    return "";
}

string logout(vector<string> cmd, int idx)
{
    return "";
}

void *serverservice(void *newClientSocket)
{
    // cout << "inside serverservice\n";
    int clientSocket = *(int *)newClientSocket;
    int idx = sock2idx[clientSocket];
    while (1)
    {
        // if(trackerQuit = true)
        // {
        //     string replyToClient = "TRACKER QUIT";
        //     if (send(clientSocket, &replyToClient[0], replyToClient.length(), 0) < 0)
        //     {
        //         cout << "Tracker Server Service: Error sending message\n";
        //         return (void *)-1;
        //     }
        // }
        char buffer[1024] = {0};
        if (read(clientSocket, buffer, 1024) < 0)
        {
            break;
            perror("Tracker Server Service: Error reading message");
            return (void *)-1;
        }
        cout << client[idx].ip << ":" << client[idx].port << endl;
        string command(buffer);
        cout << command << endl;
        vector<string> cmd;
        tokenize(cmd, command);
        for (unsigned long i = 0; i < cmd.size(); i++)
            cout << cmd[i] << endl;
        cout << endl;
        string replyToClient = "";
        if (cmd[0] == "EXIT")
            break;
        else if (cmd[0] == "create_user")
            replyToClient = create_user(cmd, idx);
        else if (cmd[0] == "login")
            replyToClient = login(cmd, idx);
        else if (cmd[0] == "list_groups")
            replyToClient = list_groups(cmd, idx);
        else if (cmd[0] == "create_group")
            replyToClient = create_group(cmd, idx);
        else if (cmd[0] == "join_group")
            replyToClient = join_group(cmd, idx);
        else if (cmd[0] == "leave_group")
            replyToClient = leave_group(cmd, idx);
        else if (cmd[0] == "list_requests")
            replyToClient = list_requests(cmd, idx);
        else if (cmd[0] == "accept_request")
            replyToClient = accept_request(cmd, idx);
        else if (cmd[0] == "list_files")
            replyToClient = list_files(cmd, idx);
        else if (cmd[0] == "upload_file")
            replyToClient = upload_file(cmd, idx);
        else if (cmd[0] == "download_file")
            replyToClient = download_file(cmd, idx);
        else if (cmd[0] == "show_downloads")
            replyToClient = show_downloads(cmd, idx);
        else if (cmd[0] == "stop_share")
            replyToClient = stop_share(cmd, idx);
        else if (cmd[0] == "logout")
            replyToClient = logout(cmd, idx);
        else
            cout << "INVALID COMMAND\n";
        if (send(clientSocket, &replyToClient[0], replyToClient.length(), 0) < 0)
        {
            cout << "Tracker Server Service: Error sending message\n";
            return (void *)-1;
        }
    }
}

int main(int argc, char *argv[])
{
    system("clear");
    // trackerQuit = false;

    // For checking number of command line arguments
    if(argc != 3)
    {
        cout << "Invalid number of arguments";
        return 0;
    }

    int idx = 0;
    pthread_t forClient;

    // char tra_port[] = "8080";
    // char tra_ip[] = "127.0.0.1";
    // cout <<tra_port << " " << tra_ip;

    // char tra_port[] = argv[2];
    // char tra_ip[] = argv[1];

    // int trackerIP = inet_addr(tra_ip);
    // int trackerPort = atoi(tra_port);

    int trackerIP = inet_addr(argv[1]);
    int trackerPort = atoi(argv[2]);

    int serverFD, newClientSocket;
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

    while (1)
    {
        cout << "[*] Listening\n\n";
        if ((newClientSocket = accept(serverFD, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Error in accept connection");
            exit(EXIT_FAILURE);
        }
        char buffer[1024] = {0};
        if (read(newClientSocket, buffer, 1024) < 0)
        {
            perror("Error reading message");
        }
        string clientIP_PORT(buffer);
        cout << "[*] Connected to " << clientIP_PORT << endl
             << endl;
        // newClientSocket will be unique as it is the socket created on tracker for Client
        sock2idx[newClientSocket] = idx;
        client[idx].isUser = false;
        client[idx].isLoggedIn = false;
        int ind = 0;
        while (clientIP_PORT[ind++] != ':');
        client[idx].ip = clientIP_PORT.substr(0, ind - 1);
        client[idx].port = stoi(clientIP_PORT.substr(ind, clientIP_PORT.length() - (ind)));
        // cout << client[idx].ip << " " << client[idx].port << endl;
        idx++;

        // Create thread after accepting connection of client
        if (pthread_create(&forClient, NULL, serverservice, (void *)&newClientSocket) < 0)
        {
            perror("\ncould not create thread\n");
        }

        // thread forClient(serverservice, newClientSocket);

        // string input;
        // while (1)
        // {
        //     cin >> input;
        //     if (input == "quit")
        //     {
        //         cout << "TRACKER QUIT";
        //         trackerQuit = true;
        //         return 0;
        //     }
        // }
    }
    return 0;
}

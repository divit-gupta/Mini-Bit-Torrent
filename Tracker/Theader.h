#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <thread>

#define max_client 10

using namespace std;

struct CLIENT
{
	int port;
	string ip;
	bool isUser;
	string userName;
	bool isLoggedIn;
	set<string> grpID;
	// char IP[15];
	// int portNo;
	// int sock;
	// struct sockaddr_in clientAddr; 
};
typedef struct CLIENT CLIENT;


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <iterator>

#include "RPCServer.h"

#define PORT 8080

using namespace std;

RPCServer::RPCServer()
{
    m_rpcCount = 0; 
};

RPCServer::~RPCServer() {};

/*
* StartServer will create a server on a Port that was passed in, and create a socket
*/

bool RPCServer::StartServer()
{
    int opt = 1;
    struct sockaddr_in address;


    // Creating socket file descriptor
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(m_server_fd, (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(m_server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return true;
}

/*
* Will accept a new connection by listening on it's address
*
*/

bool RPCServer::ListenForClient()
{
    int new_socket;
    struct sockaddr_in address;    
    int addrlen = sizeof(address);

    if ((new_socket = accept(m_server_fd, (struct sockaddr*)&address,
        (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    this->ProcessRPC();
    return true;
}

/*
* Going to populate a String vector with tokens extracted from the string the client sent.
* The delimter will be a ; 
* An example buffer could be "connect;1;5;abc;def;hello world"
*/
void RPCServer::ParseTokens(char * buffer, std::vector<std::string> & a)
{
    char* token;
    char* rest = (char *) buffer;

    while ((token = strtok_r(rest, ";", &rest)))
    {
        printf("%s\n", token);
        a.push_back(token);
    }

    return;
}

/*
* ProcessRPC will examine buffer and will essentially control
*/
bool RPCServer::ProcessRPC()
{
    const char* rpcs[] = { "connect", "disconnect", "status"};
    char buffer[8192];
    std::vector<std::string> arrayTokens;
    int valread = 0;
    bool bConnected = false;
    bool bStatusOk = true;
    const int RPCTOKEN = 0;
    bool bContinue = true;

    while ((bContinue) && (bStatusOk))
    {
        // Should be blocked when a new RPC has not called us yet
        if ((valread = read(this->m_server_fd, buffer, sizeof(buffer))) <= 0)
        {
            break;
        }
        printf("%s\n", buffer);

        arrayTokens.clear();
        this->ParseTokens(buffer, arrayTokens);

        // Enumerate through the tokens. The first token is always the specific RPC
        for (vector<string>::iterator t = arrayTokens.begin(); t != arrayTokens.end(); ++t)
        {
            printf("token = %s\n", t);
        }

        // string statements are not supported with a switch, so using if/else logic to dispatch
        string aString = arrayTokens[RPCTOKEN];

        if ((bConnected == false) && (aString == "connect"))
            bStatusOk = ProcessConnectRPC(arrayTokens);  // Connect RPC

        else if ((bConnected == true) && (aString == "disconnect"))
        {
            bStatusOk = ProcessDisconnectRPC();
            bContinue = false; // We are going to leave this loop, as we are done
        }

        else if ((bConnected == true) && (aString == "status"))
            bStatusOk = ProcessStatusRPC();   // Status RPC

        else 
        {
            // Not in our list, perhaps, print out what was sent
        }

    }

    return true;
}

bool RPCServer::ProcessConnectRPC(std::vector<std::string> & arrayTokens)
{
    const int USERNAMETOKEN = 1;
    const int PASSWORDTOKEN = 2;

    // Strip out tokens 1 and 2 (username, password)
    string userNameString = arrayTokens[USERNAMETOKEN];
    string passwordString = arrayTokens[PASSWORDTOKEN];
    char szBuffer[80];

    // Our Authentication Logic. Looks like Mike/Mike is only valid combination
    if ((userNameString == "mike") && (passwordString == "mike"))
    {
        strcpy(szBuffer, "1;"); // Connected
    }
    else
    {
        strcpy(szBuffer, "0;"); // Not Connected
    }

    // Send Response back on our socket

    send(this->m_server_fd, szBuffer, strlen(szBuffer), 0);

    return true;
}

bool RPCServer::ProcessStatusRPC()
{
    return true;
}

bool RPCServer::ProcessDisconnectRPC()
{
    return true;
}
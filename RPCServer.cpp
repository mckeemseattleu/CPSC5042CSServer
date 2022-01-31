#include "RPCServer.h"



#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

RPCServer::RPCServer()
{
    m_rpcCount = 0; 
};

RPCServer::~RPCServer() {};

// Will do all the startup tasks
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

// Will wait for a Client it can accept
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


// First one in this function should be a connect, and it 
// will continue try to process RPC's until a Disconnect happens
bool RPCServer::ProcessRPC()
{
    const char* rpcs[] = { "connect", "disconnect" };
    bool bStatusOk = true;

    while (bStatusOk == true)
    {
        // Read Buffer
        // Parse Buffer until it gets to RPC=
        // Parse Buffer until it gets to Next SemiColon
    }
    // read the first argument: {RPC=<>,InputArgs=<;;> }

    // If firstRPC is not a "connect" then error
    // If RPC's don't match with our list, then error
    // If we get a "disconnect" then leave function

    //Write the following: {OutputArgs=<>}
    return true;
}

bool RPCServer::ProcessConnectRPC()
{
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
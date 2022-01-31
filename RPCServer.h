/* RPCServer.cpp : This file contains the 'main' function.Program execution begins and ends there.
 This is a very very simple example of a CPSC5042 Server that will listen to
 a CPSC5042 Client
 Version 1 will have the server handle one client at a time. The server will:
 - Wait for connection from client
 - Process the Connect API  once connect
 - Process all RPC requests until the client does a Disconnect RPC
 - This intial server will handle 3 RPC's:
 -      Connect
 -      HowManyChars (will return HowManyChars are in the input message)
 -      Disconnnect
 server, then run the various RPC's that might happen between the server and
 client
*/


#pragma once
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

class RPCServer
{
public:
    RPCServer();
    ~RPCServer();
    bool StartServer();
    bool ListenForClient();
    bool ProcessRPC();
    int getRpcCount() { return m_rpcCount; }

private:
    int m_rpcCount;
    int m_server_fd;

    // First one in this function should be a connect, and it 
    // will continue try to process RPC's until a Disconnect happens
    bool ProcessConnectRPC();
    bool ProcessStatusRPC();
    bool ProcessDisconnectRPC();

};

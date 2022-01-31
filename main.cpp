#include <cstdio>
#include "RPCServer.h"

int main()
{

    bool statusOk = true;
    RPCServer* serverObj = new RPCServer();


    statusOk = serverObj->StartServer();
    printf("Endless loop, as server is up always");
    while (statusOk)
    {
        statusOk = serverObj->ListenForClient();
        statusOk = serverObj->ProcessRPC(); // Launch thread
    }

    delete serverObj;
    return 0;
}
#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <thread>

#include "ConnectionManager.h"
#include "TCPConnection.h"
#include "TCPListener.h"

class GameServer {

public:

    GameServer();
    ~GameServer();

    void Initialize();
    void Run();

private:

    ConnectionManager* clients;

};


#endif
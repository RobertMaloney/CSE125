#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <deque>

#include "TCPListener.h"
#include "TCPConnection.h"

using std::deque;

class ConnectionManager {

public:

    ConnectionManager();
    ~ConnectionManager();

    void SendUpdates(deque<Packet> & gameState);
    void ReceiveUpdates(deque<Packet> & updates);

private:

    int maxConnections;
    TCPListener* listener;
    vector<TCPConnection*> connections;

};


#endif
#include "ConnectionManager.h"

ConnectionManager::ConnectionManager() {
    this->listener = new TCPListener();
    this->listener->Bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    this->listener->Listen(4);
    this->listener->SetNonBlocking(true);
    maxConnections = 1;
}


ConnectionManager::~ConnectionManager() {
    if (listener) {
        listener->Close();
        delete listener;
    }
    for (int i = 0; i < connections.size(); ++i) {
        connections[i]->Close();
        delete connections[i];
    }
}


void ConnectionManager::SendUpdates(deque<Packet> & gameState) {
    for (auto conn = connections.begin(); conn != connections.end(); ++conn) {
        for (auto chunk = gameState.begin(); chunk != gameState.end(); ++chunk) {
            (*conn)->Send(*chunk);
        }
    }
    gameState.clear();
}


void ConnectionManager::ReceiveUpdates(deque<Packet> & updates) {
    if (connections.size() < maxConnections) {
        TCPConnection* newConn = listener->Accept();
        if (newConn) {
            newConn->SetNoDelay(true);
             newConn->SetNonBlocking(true);
            connections.push_back(newConn);
        }
    }

    for (auto it = connections.begin(); it != connections.end(); ++it) {
        (*it)->Receive(updates);
    }
}



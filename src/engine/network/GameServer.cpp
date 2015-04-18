#include "GameServer.h"


GameServer::GameServer() {
    this->clients = new unordered_map<ClientId, TCPConnection*>();
}


GameServer::~GameServer() {
    if (listener) {
        listener->Close();
        delete listener;
        listener = nullptr;
    }
    for (auto it = clients->begin(); it != clients->end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    delete clients;
    clients = nullptr;
}


void GameServer::Initialize() {
    Socket::Initialize();
    this->listener = new TCPListener();
    this->listener->Bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    this->listener->Listen(4);
    this->listener->SetNonBlocking(true);
    maxConnections = 1;
    nextCid = 1;
}


void GameServer::Run() {
    deque<Packet> updates;
    TCPConnection* newClient = nullptr;

    while (true) {
        if (clients->size() < maxConnections) {
            newClient = listener->Accept();
            if (newClient) {
                newClient->SetNoDelay(true);
                newClient->SetNonBlocking(true);
                clients->insert(make_pair(nextCid++, newClient));
            }
        }

        for (auto it = clients->begin(); it != clients->end(); ++it) {

            SocketError err = it->second->Receive(updates);
            
            if (this->ShouldTerminate(err)) {
                it->second->Close();
                delete it->second;
                clients->erase(it->first);
            } else {
                this->PrintUpdates(updates);
                it->second->Send(updates);
            }
        }
        sleep_for(milliseconds(200));
    }
}


bool GameServer::ShouldTerminate(SocketError err) {
    switch (err) {
    case SE_NOERR:
        return false;
    case SE_WOULDBLOCK: 
        return false;
    case SE_NODATA:
        return false;
    default:
        return true;
    }
}


void GameServer::PrintUpdates(deque<Packet> & updates) {
    for (auto it = updates.begin(); it != updates.end(); ++it) {
        for (auto p = it->begin(); p != it->end(); ++p) {
            cout << to_string(*p) << " ";
        }
        cout << "\n";
    }
}

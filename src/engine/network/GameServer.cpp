#include "GameServer.h"


GameServer::GameServer() {
    this->clients = new unordered_map<ClientId, TCPConnection*>();
    nextCid = 1;
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


void GameServer::Initialize(int maxPlayers) {
    Socket::Initialize();
    this->listener = new TCPListener();
    this->listener->Bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    this->listener->Listen(maxPlayers);
    this->listener->SetNonBlocking(true);
    maxConnections = maxPlayers;
}


void GameServer::Run() {
    deque<Packet> events;

    while (true) {
        if (clients->size() < maxConnections) {
            this->AcceptWaitingClient();
        }
        this->ReceiveEvents(events);
        this->PrintUpdates(events);
        this->SendUpdates(events);
        events.clear();
   //     sleep_for(milliseconds(200));
    }
}


void GameServer::AcceptWaitingClient() {
    TCPConnection* newClient = listener->Accept();
            if (newClient) {
                newClient->SetNoDelay(true);
                newClient->SetNonBlocking(true);
                clients->insert(make_pair(nextCid++, newClient));
            }
        }




void GameServer::SendUpdates(deque<Packet> & updates) {
    for (auto it = clients->begin(); it != clients->end();) {
        SocketError err = it->second->Send(updates);
            if (this->ShouldTerminate(err)) {
                it->second->Close();
                delete it->second;
            it = clients->erase(it);
        } else {
            ++it;
        }
    }
}


void GameServer::ReceiveEvents(deque<Packet> & events) {
    for (auto it = clients->begin(); it != clients->end();) {
        SocketError err = it->second->Receive(events);
        if (this->ShouldTerminate(err)) {
            it->second->Close();
            delete it->second;
            it = clients->erase(it);
        } else {
            ++it;
        }
    }
    this->PrintUpdates(updates);
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

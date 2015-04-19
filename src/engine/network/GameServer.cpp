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


void GameServer::Initialize(int maxPlayers) {
    Socket::Initialize();
    this->listener = new TCPListener();
    this->listener->Bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    this->listener->Listen(maxPlayers);
    this->listener->SetNonBlocking(true);
    maxConnections = maxPlayers;
    nextCid = 1;
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
        sleep_for(milliseconds(200));
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


void GameServer::CheckError(pair<ClientId, TCPConnection*> conn, SocketError err) {
    if (this->ShouldTerminate(err)) {
        conn.second->Close();
        delete conn.second;
        clients->erase(conn.first);
    }
}


void GameServer::SendUpdates(deque<Packet> & updates) {
    for (auto it = clients->begin(); it != clients->end(); ++it) {
        this->CheckError(*it, it->second->Send(updates));
    }
}


void GameServer::ReceiveEvents(deque<Packet> & events) {
    for (auto it = clients->begin(); it != clients->end(); ++it) {
        this->CheckError(*it, it->second->Receive(events));
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

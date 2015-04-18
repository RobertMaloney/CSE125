#include "GameServer.h"

/*
namespace std {
    template<>
    struct hash<ClientId> {
    public:
        size_t operator()(const ClientId & id) const {
            return static_cast<int>(id);
        }
    };
}*/


GameServer::GameServer() {
    this->clients = new unordered_map<ClientId, TCPConnection*>();
}


GameServer::~GameServer() {
    if (listener) {
        listener->Close();
        delete listener;
    }
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
                clients->insert(make_pair(nextCid, newClient));
            }
        }

        for (auto it = clients->begin(); it != clients->end(); ++it) {
            it->second->Receive(updates);
            this->PrintUpdates(updates);
          //  it->second->Send(updates);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

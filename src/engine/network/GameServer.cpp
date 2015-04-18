#include "GameServer.h"


GameServer::GameServer() {
    
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
}


void GameServer::Run() {


}
/*
deque<Packet> updates;

while (true) {
clients->ReceiveUpdates(updates);
for (auto it = updates.begin(); it != updates.end(); ++it) {
for (auto p = it->begin(); p != it->end(); ++p) {
std::cout << std::to_string(*p) << " ";
}
}
std::cout << "\n";
// clients->SendUpdates(updates);
std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

*/
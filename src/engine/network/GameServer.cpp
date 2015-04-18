#include "GameServer.h"


GameServer::GameServer() {
    
}


GameServer::~GameServer() {
    if (clients) {
        delete clients;
    }
}


void GameServer::Initialize() {
    Socket::Initialize();
    clients = new ConnectionManager();
}


void GameServer::Run() {

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
}
/*
TCPListener listener;
TCPConnection* nclient = nullptr;

listener.Bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
listener.Listen(5);

Packet p;
char buffer[1024];
int numRecvd = 0;

while (true) {
if (!nclient) {
nclient = listener.Accept();
std::cout << "accepted new client" << std::endl;
//nclient->SetNonBlocking(true);
}

if (nclient->Receive(p) == SE_NOERR) {

std::cout << "Packet." << std::endl;
for (auto it = p.begin(); it != p.end(); ++it) {
std::cout << std::to_string(*it);
}

std::cout << "" << std::endl;
}
p.clear();
std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
*/
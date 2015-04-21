#include "GameClient.h"


GameClient::GameClient() {
    connection = new TCPConnection();
}


GameClient::~GameClient() {
    if (connection) {
        delete connection;
        connection = nullptr;
    }
}


void GameClient::Initialize() {
    Socket::Initialize();
    SocketError err = connection->Connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    if (this->ShouldTerminate(err)) {
        connection->Close();
        delete connection;
        connection = nullptr;
        throw SocketException("Connection Failure.");
    }
    connection->SetNoDelay(true);
    connection->SetNonBlocking(true);
}


void GameClient::ReceiveUpdates(deque<Packet> & updates) {
    this->CheckError(connection->Receive(updates));
}


void GameClient::SendEvents(deque<Packet> & events) {
    this->CheckError(connection->Send(events));    
}


void GameClient::CheckError(SocketError err) {
    if (this->ShouldTerminate(err)) {
        connection->Close();
        throw SocketException("Fatal error while communicating over TCPConnection.");
    }
}


bool GameClient::ShouldTerminate(SocketError err) {
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

void GameClient::PrintUpdates(deque<Packet> & updates) {
    for (auto it = updates.begin(); it != updates.end(); ++it) {
        for (auto p = it->Begin(); p != it->End(); ++p) {
            cout << to_string(*p) << " ";
            
        }
        cout << "\n";
    }
}

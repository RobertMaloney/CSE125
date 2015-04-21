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
	deque<Packet> sends;

    while (true) {
        if (clients->size() < maxConnections) {
            this->AcceptWaitingClient();
        }
        this->ReceiveEvents(events);
		this->ParsePlayer(events, sends);
        //this->PrintUpdates(events);
        this->SendUpdates(sends);
        events.clear();
		sends.clear();
   //     sleep_for(milliseconds(200));
    }
}

void GameServer::ParsePlayer(deque<Packet> & in, deque<Packet> & out) {
	for (unsigned int i = 0; i < in.size(); ++i) {
		if (in[i].Size() > 0) {
			Packet p;
			switch (in[i][0]) {
			case 0:
				cout << "Moving player forward..." << endl;
				m_player = glm::translate(m_player, glm::vec3(0, -1.f, 0));
				break;
			case 1:
				cout << "Moving player left..." << endl;
				m_player = glm::translate(m_player, glm::vec3(1.f, 0, 0));
				break;
			case 2:
				cout << "Moving player backward..." << endl;
				m_player = glm::translate(m_player, glm::vec3(0, 1.f, 0));
				break;
			case 3:
				cout << "Moving player right..." << endl;
				m_player = glm::translate(m_player, glm::vec3(-1.f, 0, 0));
				break;
			}
			float * matP = glm::value_ptr(m_player);
			for (int j = 0; j < 16 ; ++j)
				p.WriteFloat(matP[j]);

			out.push_back(p);
		}
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
        for (unsigned int i = 0; i < it->Size(); ++it) {
            cout << to_string(it->At(i)) << " ";
        }
        cout << "\n";
    }
}

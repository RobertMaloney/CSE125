#include "GameServer.h"


GameServer::GameServer() {
	this->clients = new unordered_map<TCPConnection*, ObjectId>();
    this->handler = new PacketHandler();
    nextObjId = 1;
}


GameServer::~GameServer() {
    if (listener) {
        listener->close();
        delete listener;
        listener = nullptr;
    }
    for (auto it = clients->begin(); it != clients->end(); ++it) {
        if (it->first) {
            delete it->first;
        }
    }
    delete clients;
    clients = nullptr;
}


void GameServer::initialize(int maxConns) {
    Socket::initialize();
    this->listener = new TCPListener();
    this->listener->bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    this->listener->listen(maxConns);
    this->listener->setNonBlocking(true);
    maxConnections = maxConns;
}


void GameServer::run() {

    long long elapsed;
    high_resolution_clock::time_point begin;
    high_resolution_clock::time_point end;

    while (true) {
        begin = high_resolution_clock::now();

        if (clients->size() < maxConnections) {
            this->acceptWaitingClient();
        }

        this->receiveAndUpdate();
        this->tick();

        end = high_resolution_clock::now();
        elapsed = duration_cast<milliseconds>(end - begin).count();
 
    }
}

void GameServer::parsePlayer(deque<Packet> & in, deque<Packet> & out) {
	glm::mat4 m_player;
	for (unsigned int i = 0; i < in.size(); ++i) {
		if (in[i].size() > 0) {
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
				p.writeFloat(matP[j]);

			out.push_back(p);
		}
	}
}


void GameServer::acceptWaitingClient() {
    TCPConnection* newClient = listener->accept();
    if (newClient) {
        newClient->setNoDelay(true);
        newClient->setNonBlocking(true);
		clients->insert(make_pair(newClient, nextObjId++));
    }
}




void GameServer::tick() {

}


void GameServer::receiveAndUpdate() {
    deque<Packet> events;

    for (auto it = clients->begin(); it != clients->end();) {
        SocketError err = it->first->receive(events);

        if (this->shouldTerminate(err)) {
            it->first->close();
            delete it->first;
            it = clients->erase(it);
            events.clear();
            continue;
        }

        handler->dispatch(it->second, events);
        events.clear();
        ++it;
    }
}


void GameServer::printUpdates(deque<Packet> & updates) {
    for (auto it = updates.begin(); it != updates.end(); ++it) {
        for (unsigned int i = 0; i < it->size(); ++it) {
            cout << to_string(it->at(i)) << " ";
        }
        cout << "\n";
    }
}

#include "GameClient.h"


deque<Packet> GameClient::input;


GameClient::GameClient() {
    connection = new TCPConnection();
}


GameClient::~GameClient() {
    if (connection) {
        delete connection;
        connection = nullptr;
    }
}


// used for debugging
void fillWithDebugPackets(deque<Packet> & packets) {
	Packet p;
	for (int i = 0; i < 26; ++i) {
		p.resize(0);
		int len = rand() % 1000;
		for (int j = 0; j < len; ++j) {
			p.writeChar('a' + i);
		}
		packets.push_back(p);
	}
}


void GameClient::run() {
	bool DEBUG = true;

	GraphicsEngine::Initialize();
	//GraphicsEngine::SetKeyCallback(keyCallback);
	//client = new GameClient();
	//client->initialize();
	this->initialize();

	deque<Packet> packets;
	deque<Packet> updates;
	fillWithDebugPackets(packets);
	Packet p;

	int i = 0;

	//main game loop
	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		if (DEBUG) {

			//send user input
			this->sendEvents(GameClient::input);

			this->receiveUpdates(updates);
			//client->PrintUpdates(updates);
			GraphicsEngine::UpdatePlayer(updates);

			//clear events
			updates.clear();
			GameClient::input.clear();
		}
	}

	GraphicsEngine::Destroy();
	system("pause");
}


void GameClient::initialize() {
    Socket::initialize();
    SocketError err = connection->connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    if (this->shouldTerminate(err)) {
        connection->close();
        delete connection;
        connection = nullptr;
        throw SocketException("Connection Failure.");
    }
    connection->setNoDelay(true);
    connection->setNonBlocking(true);
}


void GameClient::receiveUpdates(deque<Packet> & updates) {
    this->checkError(connection->receive(updates));
}


void GameClient::sendEvents(deque<Packet> & events) {
    this->checkError(connection->send(events));    
}


void GameClient::printUpdates(deque<Packet> & updates) {
    for (auto it = updates.begin(); it != updates.end(); ++it) {
        for (unsigned int i = 0; i < it->size(); ++it) {
            cout << to_string(it->at(i)) << " ";
        }
        cout << "\n";
    }
}


void GameClient::checkError(SocketError err) {
	if (this->shouldTerminate(err)) {
		connection->close();
		throw SocketException("Fatal error while communicating over TCPConnection.");
	}
}


bool GameClient::shouldTerminate(SocketError err) {
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

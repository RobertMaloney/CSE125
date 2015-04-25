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
	this->initialize();

	deque<Packet> updates;

	//main game loop
	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		
		if (DEBUG) {	
			this->sendEvents(InputHandler::input);		
			this->receiveUpdates(updates);
			GraphicsEngine::UpdatePlayer(updates);
			updates.clear();
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


void GameClient::sendEvents(deque<Event> & events) {
	Packet p;
	for (auto it = events.begin(); it != events.end(); ++it){
		it->serialize(p);
		this->checkError(connection->send(p));
		p.clear();
	}
	events.clear();
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

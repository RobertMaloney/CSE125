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


void GameClient::run() {
	bool DEBUG = true;
	bool loggedIn = false;
    deque<Packet> updates;

	GraphicsEngine::Initialize();
	this->initialize();
	Packet p;

	connection->setNonBlocking(false);
	connection->receive(p);

	ObjectId playerId = p.readUInt();
    GameObject* player = new Player();
	std::cout << "logging in id " << playerId << std::endl;
	player = gstate.map.add(playerId, player);


	GraphicsEngine::Login(player);//TODO remove node, remove Login..


	connection->setNonBlocking(true);

	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		
		if (DEBUG) {	
			this->sendEvents(InputHandler::input);		
			this->receiveUpdates(updates);
			GraphicsEngine::UpdatePlayer(updates, gstate);
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
	gstate.init();
}


void GameClient::receiveUpdates(deque<Packet> & updates) {
    this->checkError(connection->receive(updates));
}


void GameClient::sendEvents(deque<Packet> & events) {
	connection->send(events);
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

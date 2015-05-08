#include "GameServer.h"


GameServer::GameServer() {
	this->clients = new unordered_map<TCPConnection*, ObjectId>();
	this->handler = new PacketHandler();
	this->idGen = &IdGenerator::getInstance();
	this->gameState = &GameState::getInstance();
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
   gstate->init();
   gstate->generateResources(100);
}


void GameServer::run() {
	long long elapsedTime;
	high_resolution_clock::time_point start;

	while (true) {
		start = high_resolution_clock::now();
		if (clients->size() < maxConnections) {
			this->acceptWaitingClient();
		}
		this->processClientEvents();
		this->tick();
		elapsedTime = chrono::duration_cast<chrono::milliseconds>(high_resolution_clock::now() - start).count();
		if (elapsedTime > TIME_PER_FRAME) {
			cerr << "Server loop took long than a frame." << endl;
		}
		sleep_for(milliseconds(TIME_PER_FRAME - elapsedTime));
	}
}



void GameServer::acceptWaitingClient() {
	Packet response;
	TCPConnection* connection = listener->accept();
	if (!connection) {
		return;
	}
	//Note: Server generates id for client/player, and addes the player to gamestate
	//Note: default position foor player is 505,0,0,0
	ObjectId playerId = idGen->createId();
	Player* newPlayer = new Player();
	if (!gameState->addPlayer(playerId, newPlayer)){
		delete newPlayer;
		return;
	}
	connection->setNoDelay(true);
	connection->setNonBlocking(true);
	clients->insert(make_pair(connection, playerId));	
	response.writeUInt(playerId);
	connection->send(response);
}



void GameServer::tick() {
	deque<Packet> updates;
	ObjectDB::getInstance().getObjectState(updates);

	for (auto it = clients->begin(); it != clients->end(); ) {
		SocketError err = it->first->send(updates);
		if (this->shouldTerminate(err)){
			it->first->close();
			delete it->first;
			it = clients->erase(it);
        } else {
            ++it;
        }
	}
}


void GameServer::processClientEvents() {
	deque<Packet> events;

	for (auto it = clients->begin(); it != clients->end(); ) {
		SocketError err = it->first->receive(events);
		if (this->shouldTerminate(err)) {
			it->first->close();
			delete it->first;
			it = clients->erase(it);
        } else {
            handler->dispatch(it->second, events);
            ++it;
        }
        events.clear();
	}
	gameState->updateMovingPlayers();
}


void GameServer::printUpdates(deque<Packet> & updates) {
	for (auto it = updates.begin(); it != updates.end(); ++it) {
		for (unsigned int i = 0; i < it->size(); ++i) {
			cout << to_string((*it)[i]) << " ";
		}
		cout << "\n";
	}
}


#include "GameServer.h"


GameServer::GameServer() {
	this->clients = new unordered_map<TCPConnection*, ObjectId>();
	this->handler = new PacketHandler();
	this->idGen = IdGenerator::getInstance();
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

	gstate.init();
}


void GameServer::run() {
	while (true) {
		if (clients->size() < maxConnections) {
			this->acceptWaitingClient();
		}

		this->receiveAndUpdate();
		this->tick();
		sleep_for(milliseconds(34));
	}
}



void GameServer::acceptWaitingClient() {
	TCPConnection* connection = listener->accept();
	if (!connection) {
		return;
	}
	ObjectId playerId = idGen.getNextId();
	GameObject* player = gstate.addPlayer(playerId, new Player());// ObjectDB::getInstance().add(playerId, new GameObject());
	if (!player){
		//throw exception (get NULL => not added)
	}
	idGen.update(playerId);

	connection->setNoDelay(true);
	connection->setNonBlocking(true);
	clients->insert(make_pair(connection, playerId));	
	Packet response;
	response.writeUInt(playerId);
	connection->send(response);
}



void GameServer::tick() {
	deque<Packet> updates;
	//ObjectDB::getInstance().getObjectState(updates);
	gstate.map.getObjectState(updates);

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


void GameServer::receiveAndUpdate() {
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
}


void GameServer::printUpdates(deque<Packet> & updates) {
	for (auto it = updates.begin(); it != updates.end(); ++it) {
		for (unsigned int i = 0; i < it->size(); ++i) {
			cout << to_string((*it)[i]) << " ";
		}
		cout << "\n";
	}
}


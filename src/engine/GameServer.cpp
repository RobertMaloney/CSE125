#include "GameServer.h"


GameServer::GameServer() {
	this->clients = new unordered_map<TCPConnection*, ObjectId>();
	this->handler = new PacketHandler();
	this->idGen = &IdGenerator::getInstance();
	this->gameState = &GameState::getInstance();
	this->physics = new PhysicsEngine();
	this->engine = new GameEngine();
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
	delete physics;
}


void GameServer::loadConfiguration(Json::Value & values) {
	Json::Reader reader;
	ifstream inStream;
	inStream.open("../server/config_server.json");

	if (!reader.parse(inStream, values, true)) {
		inStream.close();
		cerr << "Problem parsing json config file" << endl;
		throw runtime_error("Problem parsing json config.");
	}
	inStream.close();
	maxConnections = configFile["max players"].asInt();
	TIME_PER_FRAME = (long long)(1000000.f / configFile["fps"].asFloat());
	PHYSICS_DT = (float)(TIME_PER_FRAME / 1000000.f);
}


void GameServer::initialize() {
	Socket::initialize();

	this->loadConfiguration(configFile);

	this->listener = new TCPListener();
	this->listener->bind(configFile["ip"].asString(), configFile["port"].asString());
	this->listener->listen(maxConnections);
	this->listener->setNonBlocking(true);

	physics->loadConfiguration(configFile);
	
	gameState->initWithServer(configFile);
	engine->generateResources(configFile["num resources"].asInt(),
		configFile["num clouds"].asInt(), configFile["num pills"].asInt());
}


void GameServer::run() {
	long long elapsedTime;
	high_resolution_clock::time_point start;
	running = true;

	while (running) {
		start = high_resolution_clock::now();

		if (clients->size() < maxConnections) {
			this->acceptWaitingClient();
		}

		this->processClientEvents(); 		// process the client input events
		physics->update(PHYSICS_DT);      // do a physics step
		engine->calculatePercent();
		this->tick();                       // send state back to client

		//calculates the ms from start until here.
		elapsedTime = chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count();
		if (elapsedTime > TIME_PER_FRAME) {  // this is so know if we need to slow down the loop
			cerr << "Server loop took long than a frame." << endl;
			cout << "dustyplanet:-$ ";
		}

		sleep_for(microseconds(TIME_PER_FRAME - elapsedTime));
	}
}


void GameServer::stop() {
	running = false;
}


void GameServer::reset() {
	serverLock.lock();
	this->loadConfiguration(configFile);
	gameState->setResetting(true);
	serverLock.unlock();
}


void GameServer::acceptWaitingClient() {
	Packet response;
	TCPConnection* connection = listener->accept();
	if (!connection) {
		return;
	}

	ObjectId playerId = idGen->createId();

	Player* newPlayer = new Player(TREE, 505.f, 0.f, 0.f, 0.f);
	newPlayer->loadConfiguration(configFile);

	if (!gameState->addPlayer(playerId, newPlayer)){
		delete newPlayer;
		return;
	}
	physics->registerInteraction(newPlayer, DRAG | GRAVITY);
	connection->setNoDelay(true);
	connection->setNonBlocking(true);
	clients->insert(make_pair(connection, playerId));

	newPlayer->serialize(response);
	connection->send(response);

	vector<Packet> initial;
	ObjectDB::getInstance().getObjectState(initial);
	connection->send(initial);
}


void GameServer::getUpdates(vector<Packet> & updates) {
	Packet p;
	vector<GameObject*> & changed = physics->getChangedObjects();
	for (GameObject* object : changed) {
		object->serialize(p);
		updates.push_back(p);
		p.clear();
	}
}


void GameServer::tick() {
	vector<Packet> updates;
	
	if (gameState->isResetting()) {
		serverLock.lock();
		ObjectDB & odb = ObjectDB::getInstance();
		odb.reloadObjects(configFile);
		odb.getObjectState(updates);
		gameState->setResetting(false);
		serverLock.unlock();
	} else {
		this->getUpdates(updates);
	}

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
	vector<Packet> events;

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

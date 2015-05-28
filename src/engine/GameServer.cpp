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


void GameServer::initialize() {
	Socket::initialize();
	Json::Reader reader;
	ifstream inStream;
	inStream.open("../server/config_server.json");

	if (!reader.parse(inStream, configFile, true)) {
		inStream.close();
		cerr << "Problem parsing json config file" << endl;
		throw runtime_error("Problem parsing json config.");
	}
	inStream.close();

	maxConnections = configFile["max players"].asInt();
	TIME_PER_FRAME = (long long) (1000000.f / configFile["fps"].asFloat());
	PHYSICS_DT = (float) (TIME_PER_FRAME / 1000000.f);

	this->listener = new TCPListener();
	this->listener->bind(configFile["ip"].asString(), configFile["port"].asString());
	this->listener->listen(maxConnections);
	this->listener->setNonBlocking(true);

	physics->loadConfiguration(configFile["physics engine"]);
	
	gameState->initWithServer();
	engine->generateResources(configFile["num resources"].asInt());
}


void GameServer::run() {
	long long elapsedTime;
	high_resolution_clock::time_point start;

	while (true) {
		start = high_resolution_clock::now();
		// try to allow a new player to join

		if (clients->size() < maxConnections) {
			this->acceptWaitingClient();
		}
	//	std::cout << " accept : " << chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count() << std::endl;

	//	start = high_resolution_clock::now();
		this->processClientEvents(); 		// process the client input events

	//	std::cout << " accept : " << chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count() << std::endl;
	//	start = high_resolution_clock::now();
		physics->update(PHYSICS_DT);      // do a physics step
	//	std::cout << " physics : " << chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count() << std::endl;
	//	start = high_resolution_clock::now();
		engine->calculatePercent();
	//	std::cout << " calculate percent : " << chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count() << std::endl;
	//	start = high_resolution_clock::now();
		this->tick();                       // send state back to client
	//	std::cout << " tick : " << chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count() << std::endl;

		//calculates the ms from start until here.
		elapsedTime = chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count();
		if (elapsedTime > TIME_PER_FRAME) {  // this is so know if we need to slow down the loop
			cerr << "Server loop took long than a frame." << endl;
		}

		
		// sleep for unused time
	//	start = high_resolution_clock::now();
		sleep_for(microseconds(TIME_PER_FRAME - elapsedTime));
		//std::cout << " sleep for : " << chrono::duration_cast<chrono::microseconds>(high_resolution_clock::now() - start).count() << std::endl;

		//sleep_for(milliseconds(2000));

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

	float radius = 505;
	float theta = (float)(rand() % 180);
	float azimuth = (float)(rand() % 360);
	float direction = (float)(rand() % 360);

	Player* newPlayer = new Player(TREE, radius, theta, azimuth, direction);
	newPlayer->loadConfiguration(configFile["player"]);

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



void GameServer::tick() {
	Packet p;
	vector<Packet> updates;
	vector<GameObject*> & changed = physics->getChangedObjects();
	for (GameObject* object : changed) {
		object->serialize(p);
		updates.push_back(p);
		p.clear();
	}
	changed.clear();
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

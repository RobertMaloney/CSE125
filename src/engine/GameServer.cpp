#include "GameServer.h"


GameServer::GameServer() {
	this->clients = new unordered_map<TCPConnection*, ObjectId>();
	this->handler = new PacketHandler();
	this->idGen = &IdGenerator::getInstance();
	this->gameState = &GameState::getInstance();
	this->physics = new PhysicsEngine();
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


void GameServer::initialize(int maxConns) {
	Socket::initialize();
	this->listener = new TCPListener();
	this->listener->bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
	this->listener->listen(maxConns);
	this->listener->setNonBlocking(true);
	maxConnections = maxConns;
   gameState->init();
   generateResources(5000);
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
		std::cout << " accept : " << chrono::duration_cast<chrono::milliseconds>(high_resolution_clock::now() - start).count() << std::endl;
		this->processClientEvents(); 		// process the client input events

		std::cout << " process : " << chrono::duration_cast<chrono::milliseconds>(high_resolution_clock::now() - start).count() << std::endl;
		physics->update(TIME_PER_FRAME);      // do a physics step

		std::cout << " physics : " << chrono::duration_cast<chrono::milliseconds>(high_resolution_clock::now() - start).count() << std::endl;
		this->tick();                       // send state back to client

		std::cout << " tick : " << chrono::duration_cast<chrono::milliseconds>(high_resolution_clock::now() - start).count() << std::endl;
		//calculates the ms from start until here.
		elapsedTime = chrono::duration_cast<chrono::milliseconds>(high_resolution_clock::now() - start).count();
		if (elapsedTime > TIME_PER_FRAME) {  // this is so know if we need to slow down the loop
	//		cerr << "Server loop took long than a frame." << endl;
		}
		std::cout << " sleep for : " << TIME_PER_FRAME - elapsedTime << std::endl;

		// sleep for unused time
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
	physics->registerMoveable(newPlayer);
	connection->setNoDelay(true);
	connection->setNonBlocking(true);
	clients->insert(make_pair(connection, playerId));	
	response.writeUInt(playerId);
	connection->send(response);
	vector<Packet> initial;
	//initial.push_back(response);
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

void GameServer::generateResources(int num) {
   for (int i = 0; i < num; i++)
   {
      float radius = 505;
      float theta = (float)(rand() % 180);
      float azimuth = (float)(rand() % 360);
      float direction = (float)(rand() % 360);
      Model model = TREE;

      int pick = rand() % 6;
      if (pick == 1)
         model = ROCK;
      else if (pick == 2)
         model = STUMP;
      else if (pick == 3)
         model = GRASS;
      else if (pick == 4)
         model = MUSHROOM;
      else if (pick == 5)
         model = FLOWER;

      Resource * newRe = new Resource(model, 5, radius, theta, azimuth, direction);
      ObjectId resourceId = IdGenerator::getInstance().createId();
      gameState->addResource(resourceId, newRe);
      //radius is always 505
      //randomize resource model?? (maybe we should separate blob model from resource model)
      //randomize other coords
   }
}

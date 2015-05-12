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


void GameClient::login() {
	//Note: Client receives id from server, creates a copy of player (same id) and adds it into the game state of client
	//Note: TODO: current position of player is default 505,0,0,0, need to get real position from server
	Packet p;
	connection->setNonBlocking(false);
	connection->receive(p);

	ObjectId playerId = p.readUInt();
	Player* player = new Player();

	std::cout << "logging in id " << playerId << std::endl;

	assert(gstate.addPlayer(playerId, player));

	//Initializes GraphicsEngine for this client with playerId (i.e. ClientID)
	GraphicsEngine::Initialize(playerId);

	//Binds player game object with the player node in Graphics engine
	GraphicsEngine::bindPlayerNode(player);
	connection->setNonBlocking(true);
}


void GameClient::run() {
	bool DEBUG = true;
	bool loggedIn = false;
    deque<Packet> updates;

	this->initialize();
	this->login();

	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		
		if (DEBUG) {	
			this->sendEvents(InputHandler::input);		
			this->receiveUpdates(updates);

			//Note: currently, it only updates game objects. Each package only includes the id and position of a game object.
			//Note: This method reads updates, translates update, updates game states (in client) and scene graph (in GraphicsEngine)
			this->updateGameState(updates);
			updates.clear();
		}
	}

	GraphicsEngine::Destroy();
	system("pause");
}


void GameClient::updateGameState(deque<Packet> & data) {
	if (data.size() <= 0) {
		return;
	}

	ObjectId objId;
	GameObject* obj = nullptr;

	//Note: Loop through all packets(gameobjects for now), identify which object it relates to or if it is a new object
	for (auto packet = data.begin(); packet != data.end(); ++packet) {
		if (packet->size() <= 0) {
			continue;
		}

		objId = packet->readUInt();
		packet->reset();
		obj = gstate.getObject(objId);

		//If this game object is new 
		if (!obj) {
			obj = new GameObject();
			
			if (!gstate.addObject(objId, obj)){ // Adds to game state in client
				delete obj;
				obj = nullptr;
				continue;
			}else{
                obj->deserialize(*packet);//deserialize here to get the model
            }

			//Add node in scene graph (in GraphicsEngine) and add object-node mapping (in GraphicsEngine)
			GraphicsEngine::insertObject(obj->getId(), GraphicsEngine::addNode(GraphicsEngine::selectModel(obj->getModel())));
		}
      else {
		   //Update the object in game state
		   obj->deserialize(*packet);//For now it only updates obj (pos) in game state
      }

		//Update the object in node (in GraphicsEngine)
		GraphicsEngine::updateObject(obj->getId(), obj->getLoc()); 
	}
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

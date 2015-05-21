#ifndef GAME_STATE_H
#define GAME_STATE_H


#include <vector>
#include <deque>

#include "IGameState.h"
#include "..\graphics\GraphicsEngine.h"
#include "ObjectDB.h"
#include "Player.h"
#include "Resource.h"
#include "Model.h"
#include "Timer.h"
//#include "..\network\Packet.h"


//forward declarations



using namespace std;

class GameState : public IGameState 
{
protected:

	ObjectDB * map;
	vector<Player*> players;
    vector<Resource *> resources;
	int total;


public:
	vector<Packet> updates;

	GameState();
	~GameState();

	void init();
	void initWithServer();
	void cleanup();

	void handleEvents();
	void update();
	void draw();

	bool addPlayer(ObjectId theId, Player* p);
	bool addObject(ObjectId id, GameObject* o);
	GameObject* getObject(ObjectId id);
	int getNumPlayers();
	vector<Player*> & getPlayers();
	int getTotal();
	void setTotal(int t);
	
	static GameState & getInstance();

    Model selectPlayerModel(ObjectId playerId);
    bool addResource(ObjectId theId, Resource * ptr);

	void sendEvents(vector<Packet> & events);
	void receiveUpdates();
	void updateGameState();
};

#endif
#ifndef GAME_STATE_H
#define GAME_STATE_H


#include <vector>
#include <deque>
#include <set>
#include "ObjectDB.h"
#include "Player.h"
#include "Resource.h"
#include "Model.h"
//#include "..\network\Packet.h"

using namespace std;

class GameState
{
protected:

	Json::Value configFile;
	ObjectDB * map;
	vector<Player*> players;
    vector<Resource *> resources;
	int total;
	bool resetting;

public:
	Player* top;

	GameState();
	~GameState();

	void init();
	void initWithServer(Json::Value & config);

	bool addPlayer(ObjectId theId, Player* p);
	bool addObject(ObjectId id, GameObject* o);
	GameObject* getObject(ObjectId id);
	int getNumPlayers();
	vector<Player*> & getPlayers();
	int getTotal();
	void setTotal(int t);
	
	void reset(ObjectId clientId);
	bool isResetting();
	void setResetting(bool b);
	void reset();

	static GameState & getInstance();

    Model selectPlayerModel(ObjectId playerId);
    bool addResource(ObjectId theId, Resource * ptr);
};

#endif
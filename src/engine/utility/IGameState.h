#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H


//#include "..\GameClient.h"
#include "..\network\TCPConnection.h"


//forward declarations
class GameClient;


/*
 * IGameState.h
 * Description: This class is the interface for all defferent game states. Clients 
 * should only have multiple games states, while the server has only one. 
 */
class IGameState
{
public:
	IGameState * changeTo;
	GameClient * gameclient;

	IGameState() : changeTo(nullptr) {}

	virtual ~IGameState()		= 0;

	virtual void init()			= 0;
	virtual void cleanup()		= 0;

	virtual void handleEvents() = 0;
	virtual void update()		= 0;
	virtual void draw()			= 0;

	//non-virtual methods
	//void changeState(IGameState *state);
	//IGameState * checkState();
};


inline IGameState::~IGameState() {}

#endif
#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H


//#include "..\GameClient.h"


/*
 * IGameState.h
 * Description: This class is the interface for all defferent game states. Clients 
 * should only have multiple games states, while the server has only one. 
 */
class IGameState
{
public:
	IGameState * changeTo;

	IGameState() : changeTo(NULL) {}

	virtual ~IGameState()		= 0;

	virtual void init()			= 0;
	virtual void cleanup()		= 0;

	virtual void handleEvents() = 0;
	virtual void update()		= 0;
	virtual void draw()			= 0;

	//only gameclient is used by this method
	void changeState(IGameState *state)
	{
		changeTo = state;
	}

	IGameState * checkState()
	{
		return changeTo;
	}
};


inline IGameState::~IGameState() {}

#endif
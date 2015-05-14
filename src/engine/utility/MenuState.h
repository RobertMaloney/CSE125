#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "IGameState.h"


class MenuState : public IGameState
{
public:
	MenuState();
	~MenuState();

	void init();
	void cleanup();

	void handleEvents();
	void update();
	void draw();
};



#endif
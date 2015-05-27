#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "IMenuState.h"
#include "..\network\Packet.h"

#define MENU_SELECTIONS_NUM 2


class MenuState : public IMenuState
{
public:

	enum MenuSelections{
		PLAY,
		QUIT,
		REPLAY
	};

	vector<Packet> updates;

	MenuState();
	~MenuState();

	void init(GameClient *);
	void cleanup();

	void handleEvents();
	void update();
	void draw();

	void connectToServer();
	void login();


	void MenuState::updateMenuState();

private:
	int menu_select;

	void menuUp();
	void menuDown();
	void menuEnter();

	void play();
	void quit();

	void receiveInput();

	//debugging
	void checkMenu();
};



#endif
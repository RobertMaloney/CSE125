#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "IMenuState.h"
#include "..\network\Packet.h"

#define MENU_SELECTIONS_NUM 2
#define MENU_BGM_SELECTIONS_NUM 2


class MenuState : public IMenuState  //Only one menu state, cannot have more ><
{
public:

	enum MenuSelections{
		PLAY,
		QUIT,
		REPLAY
	};

	enum MenuBgmSelections {
		BGM1,
		BGM2
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
	int music_select;

	void menuUp();
	void menuDown();
	void menuEnter();
	void changeBgm();

	void play();
	void quit();
	void replay();
	void conti();

	void receiveInput();

	//debugging
	void checkMenu();
};



#endif
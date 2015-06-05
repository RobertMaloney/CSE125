#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <deque>
#include "..\network\Packet.h"
#include "..\utility\Event.h"
#include "..\GameClient.h"

using std::deque;

class InputHandler
{
public:
	static vector<Packet> input;
	static vector<Packet> clientInput;
	//static InputHandler & getInstance();
	static void handleKey(int key, int action, int mods);
	static void handleMouse(float x, float y);

private:


};

#endif
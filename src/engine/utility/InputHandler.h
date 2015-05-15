#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <deque>
#include "../network/Packet.h"
#include "..\utility\Event.h"

using std::deque;

class InputHandler
{
public:
	
   static deque<Packet> input;
   static void handleKey(int key, int action, int mods);


private:


};

#endif
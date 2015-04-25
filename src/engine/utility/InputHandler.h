#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <deque>

#include "..\utility\Event.h"
#include "..\graphics\GraphicsEngine.h"

using std::deque;

class InputHandler
{
public:
	
   static deque<Event> input;
   static void handleKey(int key, int action, int mods);


private:



};

#endif
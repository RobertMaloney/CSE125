#ifndef INPUT_HANLDER_H
#define INPUT_HANDLER_H
#include <glfw3.h>
#include "Event.h"
#include <deque>

class InputHandler
{
public:
   static std::deque<Event> eventList;

   static Event handleKey(int key, int state, int mods);
   static void processEvents();
};

#endif
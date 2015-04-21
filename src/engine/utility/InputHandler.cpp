#include "InputHandler.h"
#include "Event.h"
#include <deque>

std::deque<Event> InputHandler::eventList;

Event InputHandler::handleKey(int key, int state, int mods)
{
   Event newevent;
   newevent.theevent = EventType::DO_NOTHING;

   if (state == GLFW_PRESS || state == GLFW_REPEAT)
   {
      switch (key) {
      case (GLFW_KEY_UP) :
         newevent.theevent = EventType::MOVE_UP;
         return newevent;
         break;
      case (GLFW_KEY_DOWN) :
         newevent.theevent = EventType::MOVE_DOWN;
         return newevent;
         break;
      case (GLFW_KEY_LEFT) :
         newevent.theevent = EventType::MOVE_LEFT;
         return newevent;
         break;
      case (GLFW_KEY_RIGHT) :
         newevent.theevent = EventType::MOVE_RIGHT;
         return newevent;
         break;
      }
   }
   return newevent;
}

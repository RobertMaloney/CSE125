#include "InputHandler.h"
#include "Event.h"
#include <vector>

Event * handleKey(int key, int state, int mods)
{
   if (state == GLFW_PRESS || state == GLFW_REPEAT)
   {
      switch (key) {
      case (GLFW_KEY_UP) :
         return new MoveEvent(MoveEvent::MoveDirection::UP);
         break;
      case (GLFW_KEY_DOWN) :
         return new MoveEvent(MoveEvent::MoveDirection::DOWN);
         break;
      case (GLFW_KEY_LEFT) :
         return new MoveEvent(MoveEvent::MoveDirection::LEFT);
         break;
      case (GLFW_KEY_RIGHT) :
         return new MoveEvent(MoveEvent::MoveDirection::RIGHT);
         break;
      }
   }
}
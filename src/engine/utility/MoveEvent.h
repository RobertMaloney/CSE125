#ifndef MOVE_EVENT_H
#define MOVE_EVENT_H

#include "Event.h"

class MoveEvent : public Event
{
public:
   enum MoveDirection {
      UP,
      DOWN,
      LEFT,
      RIGHT
   };
   
   MoveDirection dir;

   MoveEvent::MoveEvent(MoveDirection dir) :
      dir (dir){};
   ~MoveEvent() {};
};

#endif
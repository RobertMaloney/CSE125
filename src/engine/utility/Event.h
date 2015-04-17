#ifndef EVENT_H
#define EVENT_H

#include <string.h>
// Note: move to client game class
enum EventType {
   MOVE_UP,
   MOVE_DOWN,
   MOVE_LEFT,
   MOVE_RIGHT
};

struct Event {

   EventType theevent;

   void serialize(char * buffer)
   {
      memcpy(buffer, this, sizeof(EventType));
   }

   void deserialize(char* buffer)
   {
      memcpy(this, buffer, sizeof(EventType));
   }
};
#endif
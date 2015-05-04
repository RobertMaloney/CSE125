#ifndef EVENT_H
#define EVENT_H

#include <string.h>

#include "Serializable.h"

enum EventType {
   MOVE_FORWARD,
   STOP_FORWARD,
   MOVE_BACKWARD,
   STOP_BACKWARD,
   MOVE_LEFT,
   STOP_LEFT,
   MOVE_RIGHT,
   STOP_RIGHT,
   OBJECT_UPDATE
};

struct Event
{
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




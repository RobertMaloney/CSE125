#ifndef EVENT_H
#define EVENT_H

#include <string.h>

#include "Serializable.h"

enum EventType {
   MOVE_FORWARD,
   MOVE_BACKWARD,
   MOVE_LEFT,
   MOVE_RIGHT
};

class Event : public Serializable {

public:

	Event();
	Event(EventType e);
	~Event();

	void setType(EventType t);
	EventType getType();

	void serialize(Packet & p);
	void deserialize(Packet & p);

protected:
	
	EventType type;

};

#endif



/*

void serialize(char * buffer)
{
	memcpy(buffer, this, sizeof(EventType));
}

void deserialize(char* buffer)
{
	memcpy(this, buffer, sizeof(EventType));
}*/
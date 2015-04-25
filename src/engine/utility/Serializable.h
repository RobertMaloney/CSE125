#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "../network/Packet.h"

class Serializable {

public:

	virtual void serialize(Packet & p) = 0;
	virtual void deserialize(Packet & p) = 0;

};

#endif
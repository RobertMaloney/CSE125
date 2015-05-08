#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

typedef unsigned int ObjectId;

class IdGenerator {

private:

	ObjectId numOfObjects;

public:

	static IdGenerator & getInstance();
	ObjectId createId();
	
};

#endif
#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

typedef unsigned int ObjectId;

class IdGenerator {

private:
	ObjectId numOfObjects;

public:

	static IdGenerator & getInstance();
	void update(ObjectId current);
	ObjectId getNextId();
	ObjectId createId();
	
};

#endif
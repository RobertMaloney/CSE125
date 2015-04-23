#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include "Location.h"

using namespace std;

class GameObject {
	//TODO put it outside
protected:

	string id;
	Location loc;
	static int numOfObjects;

public:

	GameObject() :GameObject(0, 0, 0) {};
	GameObject(int x, int y, int z);
	GameObject(Location loc);

	virtual ~GameObject();

	string getId();

	int getX();
	int getY();
	int getZ();

	Location getLoc();

	void setX(int newX);
	void setY(int newY);
	void setZ(int newZ);

	void setLoc(Location newLoc);

};

#endif
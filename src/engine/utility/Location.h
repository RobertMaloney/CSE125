#ifndef LOCATION_H
#define LOCATION_H

#include <string>

using namespace std;

class Location {

protected:

	int x;
	int y;
	int z;

public:
	Location() :Location(0, 0, 0) {};
	Location(int nx, int ny, int nz);
	virtual ~Location();

	int getX();
	int getY();
	int getZ();

	void setX(int newX);
	void setY(int newY);
	void setZ(int newZ);
};

#endif
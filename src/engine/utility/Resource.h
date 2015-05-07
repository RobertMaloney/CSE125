#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "Model.h"
#include "GameObject.h"

class Resource : public GameObject {
protected:   
   int points; // for score system?

public:
   Resource() : Resource(TREE, 5) {};
   Resource(Model rm, int points) : Resource(rm, points, 500, 0, 0, 0){};
   Resource(Model rm, int points, float radius, float theta, float azimuth, float direction);

   ~Resource();
   
   int getPoints();

   void setPoints(int points);
};

#endif
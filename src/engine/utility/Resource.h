#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "Model.h"
#include "GameObject.h"

class Resource : public GameObject {
public:
   Resource() : Resource(TREE) {};
   Resource(Model rm) : Resource(rm, 500, 0, 0, 0){};
   Resource(Model rm, float radius, float theta, float azimuth, float direction);

   ~Resource() {};
};
#endif
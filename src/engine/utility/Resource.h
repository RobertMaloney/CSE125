#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "Model.h"
#include "GameObject.h"
// tree      xy 6.f z 16.f
// trunk     xyz      4.f
// rock      xy 8.f z 4.f
// mushroom  xy 2.f z 4.f
// float     xy 2.f z 1.5f
class Resource : public GameObject {
public:
   Resource() : Resource(TREE) {};
   Resource(Model rm) : Resource(rm, 500, 0, 0, 0){};
   Resource(Model rm, float radius, float theta, float azimuth, float direction);

   ~Resource() {};

   virtual void loadConfiguration(Json::Value config);
};
#endif
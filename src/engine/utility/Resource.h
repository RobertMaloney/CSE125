#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "ResourceModel.h"

class Resource : public GameObject {
protected:   
   
   ResourceModel rm;
   int points; // for score system?

public:
   Resource() : Resource(TREE, 5) {};
   Resource(ResourceModel rm, int points);

   ~Resource();
   
   int getPoints();

   void setPoints(int points);
};

#endif
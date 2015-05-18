#ifndef EATABLE_H
#define EATABLE_H
#include "Resource.h"
#include "GameObject.h"

class Eatable {
private:
   int points;

public:
   Eatable() : Eatable(10) {};
   Eatable(int points);

   virtual ~Eatable() {};
   
   int getPoints();
   void setPoints(int points);
};

#endif
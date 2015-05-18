#include "Resource.h"
#include "Eatable.h"

class Mushroom : public Resource, public Eatable {
public:

   Mushroom() : Resource(MUSHROOM), Eatable(10) { this->type = EATABLE; };
   Mushroom(float radius, float theta, float azimuth, float direction) :
      Resource(MUSHROOM, radius, theta, azimuth, direction), Eatable(10){
      this->type = EATABLE;
   };

   virtual ~Mushroom() { };
};

class Tree : public Resource {
public:
   Tree() : Resource(TREE) {};
   Tree(float radius, float theta, float azimuth, float direction) :
      Resource(TREE, radius, theta, azimuth, direction) {};

   virtual ~Tree() {};
};

class Rock : public Resource, public Eatable {
public:
   Rock() : Resource(ROCK), Eatable(15) { this->type = EATABLE; };
   Rock(float radius, float theta, float azimuth, float direction) :
      Resource(ROCK, radius, theta, azimuth, direction), Eatable(15){
      this->type = EATABLE;
   };

   virtual ~Rock() {};
};

class Stump : public Resource, public Eatable {
public:
   Stump() : Eatable(20), Resource(STUMP) { this->type = EATABLE; };
   Stump(float radius, float theta, float azimuth, float direction) :
      Resource(STUMP, radius, theta, azimuth, direction), Eatable(20){
      this->type = EATABLE;
   };

   virtual ~Stump() {};
};

class Grass : public Resource {
public:
   Grass() : Resource(GRASS) {};
   Grass(float radius, float theta, float azimuth, float direction) :
      Resource(GRASS, radius, theta, azimuth, direction) {};;

   virtual ~Grass() {};
};

class Flower : public Resource, public Eatable {
public:

   Flower() : Resource(FLOWER), Eatable(10) { this->type = EATABLE; };
   Flower(float radius, float theta, float azimuth, float direction) :
      Resource(MUSHROOM, radius, theta, azimuth, direction), Eatable(10){
      this->type = EATABLE;
   };

   virtual ~Flower() { };
};
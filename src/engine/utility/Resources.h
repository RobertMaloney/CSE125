#include "Resource.h"
#include "IEatable.h"

class EatableResource : public Resource, public IEatable {
	protected:
		int points;

		EatableResource(int p, Model m) : Resource(m){ 
			this->type = IEATABLE; 
			this->points = p;
		};//TREE?
		EatableResource(int p, Model m, float radius, float theta, float azimuth, float direction) :
			Resource(m, radius, theta, azimuth, direction){//TREE?
			this->type = IEATABLE;
			this->points = p;
		};

		virtual ~EatableResource() { };

	public:
		int getPoints(){
			return this->points;
		}

		void setPoints(int p){
			this->points = p;
		}
};

class Mushroom : public EatableResource {
public:
    Mushroom() : Mushroom(10){};
	Mushroom(int points) : EatableResource(points, MUSHROOM){};
	Mushroom(int points, float radius, float theta, float azimuth, float direction) :
		EatableResource(points, MUSHROOM, radius, theta, azimuth, direction){};

    virtual ~Mushroom() { };
};

class Tree : public EatableResource {
public:
	Tree() : Tree(10){};
	Tree(int points) : EatableResource(points, TREE){};
	Tree(int points, float radius, float theta, float azimuth, float direction) :
       EatableResource(points, TREE, radius, theta, azimuth, direction) {};

   virtual ~Tree() {};
};

class Rock : public Resource{
public:
	Rock() : Resource(ROCK){};
	Rock(float radius, float theta, float azimuth, float direction) :
		Resource(ROCK, radius, theta, azimuth, direction) {};

   virtual ~Rock() {};
};

class Stump : public EatableResource{
public:
	Stump() : Stump(10){};
	Stump(int points) : EatableResource(points, STUMP) {};
	Stump(int points, float radius, float theta, float azimuth, float direction) :
		EatableResource(points, STUMP, radius, theta, azimuth, direction){};

   virtual ~Stump() {};
};

class Grass : public Resource{
public:
	Grass() : Resource(GRASS) {};
    Grass(float radius, float theta, float azimuth, float direction) :
      Resource(GRASS, radius, theta, azimuth, direction) {};;

   virtual ~Grass() {};
};

class Flower : public EatableResource{
public:
	Flower() : Flower(10){};
	Flower(int points) : EatableResource(points, FLOWER){};
	Flower(int points, float radius, float theta, float azimuth, float direction) :
		EatableResource(points, FLOWER, radius, theta, azimuth, direction){};

   virtual ~Flower() { };
};
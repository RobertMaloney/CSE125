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

class PowerUpResource : public Resource {
protected:
   float jumpForce;
   float moveForce;
   float mass;
   int durationInSeconds;

   PowerUpResource(Model m) : Resource(m)
   {
      this->type = POWERUP;
      this->jumpForce = 0;
      this->moveForce = 0;
      this->mass = 0;
      this->durationInSeconds = 0;
   }

   PowerUpResource(float jumpForce, float moveForce, float mass, Model m) : Resource(m){
      this->type = POWERUP;
      this->jumpForce = jumpForce;
      this->moveForce = moveForce;
      this->mass = mass;
      this->durationInSeconds = 0;
   };

   PowerUpResource(float jumpForce, float moveForce, float mass,
         Model m, float radius, float theta, float azimuth, float direction) :
      Resource(m, radius, theta, azimuth, direction){
      this->type = POWERUP;
      this->jumpForce = jumpForce;
      this->moveForce = moveForce;
      this->mass = mass;
      this->durationInSeconds = 0;
   };

public:
   float getJumpForce() {
      return this->jumpForce;
   }

   float getMoveForce() {
      return this->moveForce;
   }

   float getMass() {
      return this->mass;
   }

   int getDurationInSeconds() {
      return this->durationInSeconds;
   }

   void setJumpForce(float jf) {
      this->jumpForce = jf;
   }

   void setMoveForce(float mf) {
      this->moveForce = mf;
   }

   void setMass(float m) {
      this->mass = m;
   }

   void setDurationInSeconds(int seconds) {
      this->durationInSeconds = seconds;
   }
};

class Mushroom : public EatableResource {
public:
    Mushroom() : Mushroom(10){};
	Mushroom(int points) : EatableResource(points, MUSHROOM){};
	Mushroom(int points, float radius, float theta, float azimuth, float direction) :
		EatableResource(points, MUSHROOM, 500.f, theta, azimuth, direction)
   {
      int pick = rand() % 4;

      if (pick == 0)
         this->setModel(MUSHROOM_1);
      else if (pick == 1)
         this->setModel(MUSHROOM_2);
      else if (pick == 2)
         this->setModel(MUSHROOM_3);
      else if (pick == 3)
         this->setModel(MUSHROOM_4);

      this->randScale(.5f, 3.f);
   };

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

class Rock : public EatableResource{
public:
	Rock() : Rock(1){};
	Rock(int points) : EatableResource(1, ROCK_1){};
	Rock(int points, float radius, float theta, float azimuth, float direction) :
		EatableResource(points, ROCK_1, 500.f, theta, azimuth, direction) {
      int pick = rand() % 3;

      if (pick == 0)
         this->setModel(ROCK_1);
      else if (pick == 1)
         this->setModel(ROCK_2);
      else if (pick == 2)
         this->setModel(ROCK_3);

      this->randScale(.5f, 3.f);
   };

   virtual ~Rock() {};
};

class TallRock : public EatableResource {
public:
	TallRock() : EatableResource(0,TALL_ROCK_1){
      int pick = rand() % 3;

      if (pick == 0)
         this->setModel(TALL_ROCK_1);
      else if (pick == 1)
         this->setModel(TALL_ROCK_2);
   };

   TallRock(float radius, float theta, float azimuth, float direction) :
	   EatableResource(1,TALL_ROCK_1, 500.f, theta, azimuth, direction) {
      int pick = rand() % 3;

      if (pick == 0)
         this->setModel(TALL_ROCK_1);
      else if (pick == 1)
         this->setModel(TALL_ROCK_2);
   };

   virtual ~TallRock() {};
};


class Stump : public EatableResource{
public:
	Stump() : Stump(10){};
	Stump(int points) : EatableResource(points, STUMP) {};
	Stump(int points, float radius, float theta, float azimuth, float direction) :
		EatableResource(points, STUMP, 500.f, theta, azimuth, direction)
   {
      this->randScale(1.f, 2.f);
   };

   virtual ~Stump() {};
};

class Grass : public Resource{
public:
	Grass() : Resource(GRASS) {};
    Grass(float radius, float theta, float azimuth, float direction) :
      Resource(GRASS, radius, theta, azimuth, direction) {};

   virtual ~Grass() {};
};

class Flower : public EatableResource{
public:
	Flower() : Flower(10){};
	Flower(int points) : EatableResource(points, FLOWER_1){};
	Flower(int points, float radius, float theta, float azimuth, float direction) :
		EatableResource(points, FLOWER_1, 500.f, theta, azimuth, direction)
   {
      int pick = rand() % 6;

      if (pick == 0)
         this->setModel(FLOWER_1);
      else if (pick == 1)
         this->setModel(FLOWER_2);
      else if (pick == 2)
         this->setModel(FLOWER_3);
      else if (pick == 3)
         this->setModel(FLOWER_4);
      else if (pick == 4)
         this->setModel(FLOWER_5);
      else if (pick == 5)
         this->setModel(FLOWER_6);

      this->randScale(1.5f, 3.f);
   };

   virtual ~Flower() { };
};

class Cloud : public EatableResource {
public:
   Cloud() : Cloud(15) {};
   Cloud(int points) : EatableResource(points, CLOUD){};
   Cloud(int points, float radius, float theta, float azimuth, float direction) :
      EatableResource(points, CLOUD, radius, theta, azimuth, direction) 
   {
      this->randScale(1.f, 3.f);
   };

   virtual ~Cloud() { };
};

class Pill : public PowerUpResource {
public:
   Pill() : PowerUpResource(0, 0, .5f, PILL) {};
   Pill(float radius, float theta, float azimuth, float direction) :
      PowerUpResource(0, 0, .5f, PILL, 503.f, theta, azimuth, direction){};

   virtual ~Pill() { };
};
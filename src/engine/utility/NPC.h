#ifndef NPC_H
#define NPC_H

#include <string>
#include "Model.h"
#include "IEatable.h"
#include "../physics/MoveableObject.h"
#include "Config.h"

class NPC : public MoveableObject, public Configurable, public IEatable {
protected:
   bool moves[5];
   bool isJumping;
   int step;
   int points;

public:
   enum {
      UP = 0,
      RIGHT,
      DOWN,
      LEFT,
      JUMP
   };

   NPC() : NPC(BUNNY) {};
   NPC(Model m) : NPC(m, 500, 0, 0, 0) {};
   NPC(Model m, float radius, float theta, float azimuth, float direction);

   ~NPC();

   bool getMoving(int);
   void setMoving(int, bool);
   void resetMoves();

   void setJumping(bool b);
   bool getJumping();

   virtual void integrate(float dt);
   virtual void collide(float dt, GameObject & target) override;
   
   void serialize(Packet & p);
   void deserialize(Packet & p);

   virtual void updateStep() = 0;
   virtual void loadConfiguration(Json::Value config);

   int getPoints();
   void setPoints(int points);
};

class Bunny : public NPC {
public:
   Bunny(float radius, float theta, float azimuth, float direction) :
      NPC(BUNNY, radius, theta, azimuth, direction) {};

   void NPC::updateStep() {
      int maxSteps = 100000;
      step = (step + 1) % maxSteps;

      // determine whether to turn or jump
      bool jump = !(this->isJumping);
      bool forward = step % 100 == 0;
      bool turn = step == maxSteps - 1;

      if (jump)
      {
         moves[JUMP] = true;
      }
      moves[UP] = true;
   }
};

class Bird : public NPC {
public:
   Bird(float radius, float theta, float azimuth, float direction) :
      NPC(BIRD, radius, theta, azimuth, direction) {};

   void NPC::updateStep() {
      int maxSteps = 1000;
      step = (step + 1) % maxSteps;

      // determine whether to turn or jump
      bool forward = true;

      if (forward)
      {
         moves[UP] = true;
      }
   }
};

#endif
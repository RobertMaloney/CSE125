#ifndef NPC_H
#define NPC_H

#include <string>
#include "Model.h"
#include "../physics/MoveableObject.h"
#include "Config.h"

class NPC : public MoveableObject, public Configurable {
protected:
   bool moves[5];
   bool isJumping;
   int step;

public:
   enum {
      UP = 0,
      RIGHT,
      DOWN,
      LEFT,
      JUMP
   };

   NPC() : NPC(MUSHROOM) {};
   NPC(Model m) : NPC(m, 505, 0, 0, 0) {};
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

   virtual void updateStep();
   void loadConfiguration(Json::Value config);
};

#endif
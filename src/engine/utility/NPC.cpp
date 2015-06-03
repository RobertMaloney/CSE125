#include "NPC.h"

NPC::NPC(Model m, float radius, float theta, float azimuth, float direction) :
   MoveableObject(radius, theta, azimuth, direction) {
   this->rm = m;
   this->type = NPCOBJ;
   this->resetMoves();
   this->isJumping = false;
   this->points = 0;
}

NPC::~NPC() {

}

bool NPC::getMoving(int index) {
   return moves[index];
}

void NPC::setMoving(int index, bool b) {
   moves[index] = b;
}

void NPC::integrate(float dt) {
   this->updateStep(); // determine where to go based on the step number

   float forceScaleByMass = (log(this->getMass()) == 0) ? 1 : log(this->getMass());
   float forceConst = this->moveForce * forceScaleByMass;

   if (moves[RIGHT]) {
      float cosa = glm::cos(glm::radians(angle - 90.f));
      float sina = glm::sin(glm::radians(angle - 90.f));
      this->addForce(cosa * forceConst, sina * forceConst, 0.f);
      //	angle -= this->angleSpeed;
      //this->velocity = this->rotateInXYPlane(this->velocity, -this->angleSpeed);
   }

   if (moves[LEFT]) {
      float cosa = glm::cos(glm::radians(angle + 90.f));
      float sina = glm::sin(glm::radians(angle + 90.f));
      this->addForce(cosa * forceConst, sina * forceConst, 0.f);
      /*angle += this->angleSpeed;
      this->velocity = this->rotateInXYPlane(this->velocity, this->angleSpeed);*/
   }


   float cosa = glm::cos(glm::radians(angle));
   float sina = glm::sin(glm::radians(angle));

   if (moves[UP]) {
      this->addForce(cosa * forceConst, sina * forceConst, 0.f);
   }

   if (moves[DOWN]) {
      this->addForce(cosa * -forceConst, sina * -forceConst, 0.f);
   }

   if (moves[JUMP] && !this->isJumping) {
      this->verticalComponent.force = this->jumpForce * forceScaleByMass;
      this->isJumping = true;
   }

   this->resetMoves();

   MoveableObject::integrate(dt);
}

void NPC::setJumping(bool b) {
   this->isJumping = b;
}

bool NPC::getJumping() {
   return this->isJumping;
}

void NPC::serialize(Packet & p) {
   GameObject::serialize(p);
}

void NPC::deserialize(Packet & p) {
   GameObject::deserialize(p);
}

void NPC::updateStep() {
   int maxSteps = 100000;
   step = (step + 1) % maxSteps;

   // determine whether to turn or jump
   bool jump = !(this->isJumping);
   bool forward = step % 100 == 0;
   bool turn = step == maxSteps - 1;

   if (turn) {
      moves[LEFT] = true;
   }
   else if (jump)
   {
      moves[JUMP] = true;
      moves[UP] = true;
   }
}

void NPC::resetMoves() {
   for (int i = 0; i < 5; i++)
      this->moves[i] = false;
}

void NPC::collide(float dt, GameObject & target) {

}

void NPC::loadConfiguration(Json::Value config) {
   // orientation not here yet
   this->angle = config["angle"].asFloat();
   this->height = config["height"].asFloat();
   this->verticalComponent.height = this->height;
   //this->type = this->typeFromString(config["type"].asString());
   this->modelRadius = config["modelRadius"].asFloat();
   this->modelHeight = config["modelHeight"].asFloat();
   this->visible = config["visible"].asBool();
   this->angleSpeed = config["angleSpeed"].asFloat();
   this->jumpForce = config["jumpForce"].asFloat();
   this->moveForce = config["moveForce"].asFloat();
   this->inverseMass = 1.f / config["mass"].asFloat();
   this->restitution = config["restitution"].asFloat();
   this->points = config["points"].asInt();
}

int NPC::getPoints()
{
   return this->points;
}

void NPC::setPoints(int p)
{
   this->points = p;
}
#include "GravityGenerator.h"

GravityGenerator::GravityGenerator() {
	gravity = -80.f;
}


GravityGenerator::~GravityGenerator() {

}

#include "../utility/Player.h"
#include "../utility/NPC.h"
void GravityGenerator::updateForce(MoveableObject* target, float dt) {
	VerticalMovement & verticalComponent = target->getVerticalComponent();
	if (verticalComponent.height < 505.f) {
		verticalComponent.height = 505.f;
		verticalComponent.velocity *= -.5f;
		verticalComponent.force = 0.f;
		if (target->getType() == PLAYER) {
			Player* p = dynamic_cast<Player*>(target);
			p->setJumping(false);
      }
      else if (target->getType() == NPCOBJ) {
         NPC * npc = dynamic_cast<NPC*>(target);
         npc->setJumping(false);
      }
		return;
	}
	
	float netForce = verticalComponent.force;
	verticalComponent.force = 0.f;
	float netAcceleration = netForce * target->getInverseMass() + gravity;
	verticalComponent.velocity += netAcceleration * dt;
	verticalComponent.height += verticalComponent.velocity * dt;
}


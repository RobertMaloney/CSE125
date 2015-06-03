#include "Player.h"
#include <iostream>
#include <gtx\string_cast.hpp>
#include "GameSound.h"

//TODO Config file
Player::Player(Model thebm, float radius, float theta, float azimuth, float direction) : MoveableObject(radius, theta, azimuth, direction) {
	//this->loc = vec4(radius, theta, azimuth, direction);
	this->rm = thebm;

	for (int i = 0; i < 5; ++i)
		this->moves[i] = false;

  	this->type = PLAYER;
	this->isJumping = false;
	this->score = 0;
	this->percent = 0;
	/*this->modelRadius = 7.f;
	this->setMass(10.f);
	this->height = 550.f;
	this->modelRadius = 5.f;
	this->modelHeight = 5.f;*/
	this->status = PENDING;
}

Player::~Player() {

}

int Player::getScore() {
	return this->score;
}

void Player::setScore(int s) {
	this->score = s;
}

int Player::getPercent() {
	return this->percent;
}

void Player::setPercent(int p) {
	this->percent = p;
}

GStatus Player::getStatus() {
	return this->status;
}

void Player::setStatus(GStatus s) {
	this->status = s;
}

bool Player::getMoving(int index) {
	return moves[index];
}

void Player::setMoving(int index, bool b) {
	moves[index] = b;
}


void Player::integrate(float dt) {
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

	MoveableObject::integrate(dt);
}


void Player::setJumping(bool b) {
	this->isJumping = b;
}

bool Player::getJumping() {
	return this->isJumping;
}

void Player::collide(float dt, GameObject & target) {
	if (!target.getVisible()) {
		return;
	}

   switch (target.getType()) {
   case PLAYER:
      this->velocity *= -1;
      break;
   case GAMEOBJECT:
      this->velocity *= -1;
      break;
   case IEATABLE:
         {
            std::cout << "EAT " << endl;
            IEatable* eatable = dynamic_cast<IEatable*>(&target);
            if (eatable) {
               std::cout << this->getId() << " old score: " << this->getScore() << endl;
               this->setScore(this->getScore() + eatable->getPoints());
               std::cout << this->getId() << " new score: " << this->getScore() << endl;
				float mass = this->getMass() / this->getMassScale();
				this->setScale((this->getScore() + SCORE_SCALE_RATIO) / SCORE_SCALE_RATIO);
				this->setMassScale((this->getScore() + SCORE_MASS_RATIO) / SCORE_MASS_RATIO);
				this->setMass(mass * this->getMassScale());
            }
            else {
               std::cout << "Error: EATABLE is null: " << typeid(target).name() << endl;
            }
            target.setVisible(false);
            //TODO Render needs to figure out (not) rendering dead/invisible object
         }
			break;
   case POWERUP:
         {
            std::cout << "POWER UP" << endl;
            PowerUpResource * powerUp = dynamic_cast<PowerUpResource *>(&target);
            if (powerUp) {
               this->setJumpForce(this->getJumpForce() + powerUp->getJumpForce());
               this->setMoveForce(this->getMoveForce() + powerUp->getMoveForce());

               float newMass = this->getMass() + powerUp->getMass();
               if (newMass > 0.f) this->setMass(newMass);
            }
            target.setVisible(false);
         }
         break;
	}
}


void Player::loadConfiguration(Json::Value config) {
	// orientation not here yet
	this->angle = config["angle"].asFloat();
	this->height = config["height"].asFloat();
	this->verticalComponent.height = this->height;
	this->type = this->typeFromString(config["type"].asString());
	this->modelRadius = config["modelRadius"].asFloat();
	this->modelHeight = config["modelHeight"].asFloat();
	this->visible = config["visible"].asBool();
	this->angleSpeed = config["angleSpeed"].asFloat();
	this->jumpForce = config["jumpForce"].asFloat();
	this->moveForce = config["moveForce"].asFloat();
	this->inverseMass = 1.f / config["mass"].asFloat();
	this->restitution = config["restitution"].asFloat();
}


void Player::serialize(Packet & p) {

	GameObject::serialize(p);

	p.writeInt(this->score);
	p.writeInt(this->percent);
	p.writeInt(this->status);
	//TODO: moves??? no. just no.
}


void Player::deserialize(Packet & p) {
	int oldscore = this->score;

	GameObject::deserialize(p);
	this->score = p.readInt();
	this->percent = p.readInt();
	this->status = static_cast<GStatus>(p.readInt());

	int newscore = this->score;

	//check if need to burp
	if (oldscore < 40 && newscore >= 40)
		GameSound::regburp->play();
	else if ( oldscore < 80 && newscore >= 80)
		GameSound::bigburp->play();
}
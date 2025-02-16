#include "Player.h"
#include <iostream>
#include <gtx\string_cast.hpp>
#include "GameSound.h"

//TODO Config file
Player::Player(Model thebm, float radius, float theta, float azimuth, float direction) : MoveableObject(radius, theta, azimuth, direction) {
	this->rm = thebm;

	for (int i = 0; i < 5; ++i)
		this->moves[i] = false;
	jumpSlow = .5f;
	SCORE_SCALE_RATIO = 250.f;
	SCORE_MASS_RATIO = 750.f;
	this->type = PLAYER;
	this->isJumping = false;
	this->score = 0;
	this->percent = 0;
	this->stomach = 0;
	this->burp_count = 0;
	this->status = PENDING;
	this->sec = 0;
	this->min = 0;
	this->dynamicScore = 0;
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

	if (moves[RIGHT]  /*&&!(verticalComponent.height > 505.f)*/) {
		float cosa = glm::cos(glm::radians(angle - 90.f));
		float sina = glm::sin(glm::radians(angle - 90.f));
		if (verticalComponent.height > 505.f) {
			cosa *= jumpSlow;
			sina *= jumpSlow;
		}
		this->addForce(cosa * forceConst, sina * forceConst, 0.f);
	}

	if (moves[LEFT] /*&& !(verticalComponent.height > 505.f)*/) {
		float cosa = glm::cos(glm::radians(angle + 90.f));
		float sina = glm::sin(glm::radians(angle + 90.f));
		if (verticalComponent.height > 505.f) {
			cosa *= jumpSlow;
			sina *= jumpSlow;
		}
		this->addForce(cosa * forceConst, sina * forceConst, 0.f);
	}


	float cosa = glm::cos(glm::radians(angle));
	float sina = glm::sin(glm::radians(angle));
	if (verticalComponent.height > 505.f) {
		cosa *= jumpSlow;
		sina *= jumpSlow;
	}
	if (moves[UP] /*&& !(verticalComponent.height > 505.f)*/) {
		this->addForce(cosa * forceConst, sina * forceConst, 0.f);
	}

	if (moves[DOWN] /*&& !(verticalComponent.height > 505.f)*/) {
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
		this->hit = true;
		break;
	case GAMEOBJECT:
		//    this->hit = true;
		break;
	case IEATABLE: {
					   this->eat = true;
					   IEatable* eatable = dynamic_cast<IEatable*>(&target);
					   if (eatable) {
						   this->setScore(this->getScore() + eatable->getPoints());
						//   this->dynamicScore += eatable->getPoints();

						   float mass = this->getMass() / this->getMassScale();
						   this->setScale((this->getScore() + SCORE_SCALE_RATIO) / SCORE_SCALE_RATIO);
						   this->setMassScale((this->getScore() + SCORE_MASS_RATIO) / SCORE_MASS_RATIO);
						   this->setMass(mass * this->getMassScale());

					   }
					   target.setVisible(false);
					   target.setParticle(true);
	}
		break;
	case NPCOBJ: {
					 this->eat = true;
					 IEatable* eatable = dynamic_cast<IEatable*>(&target);
					 if (eatable) {
						 //   std::cout << this->getId() << " old score: " << this->getScore() << endl; 
						 this->setScore(this->getScore() + eatable->getPoints());
						 //    std::cout << this->getId() << " new score: " << this->getScore() << endl; 
						 float mass = this->getMass() / this->getMassScale();
						 this->setScale((this->getScore() + SCORE_SCALE_RATIO) / SCORE_SCALE_RATIO);
						 this->setMassScale((this->getScore() + SCORE_MASS_RATIO) / SCORE_MASS_RATIO);
						 this->setMass(mass * this->getMassScale());

					 }
					 target.setVisible(false);
					 target.setParticle(true);
	}
		break;
	case POWERUP:
	{
					this->eat = true;
					//  std::cout << "POWER UP" << endl; 
					PowerUpResource * powerUp = dynamic_cast<PowerUpResource *>(&target);
					if (powerUp) {
						float newMass = this->getMass() * powerUp->getMass();
						if (newMass > 7.f){
							this->setMass(newMass);
						/*	this->dynamicScore *= powerUp->getMass();

							float mass = this->getMass() / this->getMassScale();
							this->setScale((this->dynamicScore + SCORE_SCALE_RATIO) / SCORE_SCALE_RATIO);
							this->setMassScale((this->dynamicScore + SCORE_MASS_RATIO) / SCORE_MASS_RATIO);
							this->setMass(mass * this->getMassScale());*/
						}

					}
					target.setVisible(false);
					target.setParticle(true);
	}
		break;

	}

}

GStatus Player::statusFromString(string & string) {

	transform(string.begin(), string.end(), string.begin(), ::tolower);
	if (string == "lose") {
		return GStatus::LOSE;
	}
	else if (string == "win") {
		return GStatus::WIN;
	}
	return GStatus::PENDING;
}

void Player::loadConfiguration(Json::Value config) {
	// orientation not here yet

	Json::Value obj = config["player"];
	for (int i = 0; i < 5; ++i) {
		moves[i] = false;
	}

	this->velocity *= 0.f;
	this->velocity += 0.0001f;
	this->acceleration *= 0.f;
	this->forceAccum *= 0.f;
	this->angle = obj["angle"].asFloat();
	this->height = obj["height"].asFloat();
	this->verticalComponent.height = this->height;
	this->type = this->typeFromString(obj["type"].asString());
	this->modelRadius = obj["modelRadius"].asFloat();
	this->modelHeight = obj["modelHeight"].asFloat();
	this->visible = obj["visible"].asBool();
	this->particle = obj["particle"].asBool();

	this->angleSpeed = obj["angleSpeed"].asFloat();
	this->jumpForce = obj["jumpForce"].asFloat();
	this->moveForce = obj["moveForce"].asFloat();
	this->inverseMass = 1.f / obj["mass"].asFloat();
	this->restitution = obj["restitution"].asFloat();
	this->burp_count = obj["burp count"].asInt();
	this->isJumping = obj["isJumping"].asBool();
	this->score = obj["score"].asInt();
	this->percent = obj["percent"].asInt();
	this->stomach = obj["stomach"].asInt();
	this->status = statusFromString(obj["status"].asString());
	this->scale = obj["scale"].asFloat();
	this->coefficientFriction = obj["coefficientFriction"].asFloat();
	this->jumpSlow = obj["jump slow"].asFloat();
	// Loading of "constants"
	this->SCORE_SCALE_RATIO = obj["score scale ratio"].asFloat();
	this->SCORE_MASS_RATIO = obj["score mass ratio"].asFloat();
	this->MAX_BURP_COUNT = obj["max burp count"].asInt();
	this->MAX_STOMACH_SIZE = obj["max stomach size"].asInt();

}


void Player::serialize(Packet & p) {
	GameObject::serialize(p);

	p.writeInt(this->score);
	p.writeInt(this->percent);
	p.writeInt(this->status);
	p.writeInt(this->min);
	p.writeInt(this->sec);
	//TODO: moves??? no. just no
	p.writeInt(this->MAX_BURP_COUNT);
	p.writeInt(this->MAX_STOMACH_SIZE);

	this->hit = false;
	this->eat = false;
}


void Player::deserialize(Packet & p) {
	int oldscore = this->score;

	GameObject::deserialize(p);

	this->score = p.readInt();
	this->percent = p.readInt();
	this->status = static_cast<GStatus>(p.readInt());
	this->min = p.readInt();
	this->sec = p.readInt();
	this->MAX_BURP_COUNT = p.readInt();
	this->MAX_STOMACH_SIZE = p.readInt();

	int newscore = this->score;

	if (newscore - oldscore != 0) {
		stomach = stomach + newscore - oldscore;
	}

	if (stomach >= MAX_STOMACH_SIZE) {
		burp_count += 1;
		if (burp_count < MAX_BURP_COUNT) {
			GameSound::regburp->play();
		}
		else {
			GameSound::bigburp->play();
			burp_count = 0;
		}
		stomach = 0;
	}


}

int Player::getMin() {
	return this->min;
}

void Player::setMin(int min) {
	this->min = min;
}

int Player::getSec() {
	return this->sec;
}

void Player::setSec(int sec) {
	this->sec = sec;
}
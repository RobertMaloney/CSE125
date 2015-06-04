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

	SCORE_SCALE_RATIO = 250.f;
	SCORE_MASS_RATIO = 750.f;
  	this->type = PLAYER;
	this->isJumping = false;
	this->score = 0;
	this->percent = 0;
	/*this->modelRadius = 7.f;
	this->setMass(10.f);
	this->height = 550.f;
	this->modelRadius = 5.f;
	this->modelHeight = 5.f;*/
	this->stomach = 0;
	this->burp_count = 0;
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
	}

	if (moves[LEFT]) {
		float cosa = glm::cos(glm::radians(angle + 90.f));
		float sina = glm::sin(glm::radians(angle + 90.f));
		this->addForce(cosa * forceConst, sina * forceConst, 0.f);
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
	this->eat = false;
	this->hit = false;

   switch (target.getType()) {
   case PLAYER:
      this->velocity *= -1;
	  this->hit = true;
      break;
   case GAMEOBJECT:
	   if (target.getType() != CLOUD) {
		   this->velocity *= -1;
	   }
      this->hit = true;
      break;
   case IEATABLE:
         {
			this->eat = true;
           // std::cout << "EAT " << endl;
            IEatable* eatable = dynamic_cast<IEatable*>(&target);
            if (eatable) {
             //  std::cout << this->getId() << " old score: " << this->getScore() << endl;
               this->setScore(this->getScore() + eatable->getPoints());
            //   std::cout << this->getId() << " new score: " << this->getScore() << endl;
				float mass = this->getMass() / this->getMassScale();
				this->setScale((this->getScore() + SCORE_SCALE_RATIO) / SCORE_SCALE_RATIO);
				this->setMassScale((this->getScore() + SCORE_MASS_RATIO) / SCORE_MASS_RATIO);
				this->setMass(mass * this->getMassScale());
            }
            else {
              // std::cout << "Error: EATABLE is null: " << typeid(target).name() << endl;
            }
            target.setVisible(false);
            //TODO Render needs to figure out (not) rendering dead/invisible object
         }
			break;
   case NPCOBJ:
      {
         this->eat = true;
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
      }
      break;
   case POWERUP:
         {
			 this->eat = true;
          //  std::cout << "POWER UP" << endl;
            PowerUpResource * powerUp = dynamic_cast<PowerUpResource *>(&target);
            if (powerUp) {
               this->setJumpForce(this->getJumpForce() + powerUp->getJumpForce());
               this->setMoveForce(this->getMoveForce() + powerUp->getMoveForce());

               float newMass = this->getMass() + powerUp->getMass();
               if (newMass > 10.f) this->setMass(newMass);
            }
            target.setVisible(false);
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
	this->angle = obj["angle"].asFloat();
	this->height = obj["height"].asFloat();
	this->verticalComponent.height = this->height;
	this->type = this->typeFromString(obj["type"].asString());
	this->modelRadius = obj["modelRadius"].asFloat();
	this->modelHeight = obj["modelHeight"].asFloat();
	this->visible = obj["visible"].asBool();
	this->angleSpeed = obj["angleSpeed"].asFloat();
	this->jumpForce = obj["jumpForce"].asFloat();
	this->moveForce = obj["moveForce"].asFloat();
	this->inverseMass = 1.f / obj["mass"].asFloat();
	this->restitution = obj["restitution"].asFloat();
	this->burp_count = obj["burp_count"].asInt();
	this->isJumping = obj["isJumping"].asBool();
	this->score = obj["score"].asInt();
	this->percent = obj["percent"].asInt();
	this->stomach = obj["stomach"].asInt();
	this->status = statusFromString(obj["status"].asString());

}


void Player::serialize(Packet & p) {

	GameObject::serialize(p);
	p.writeInt(this->burp_count);
	p.writeInt(this->stomach);
	p.writeInt(this->score);
	p.writeInt(this->percent);
	p.writeInt(this->status);
	//TODO: moves??? no. just no.
}


void Player::deserialize(Packet & p) {
	int oldscore = this->score;

	GameObject::deserialize(p);
	this->burp_count = p.readInt();
	this->stomach = p.readInt();
	this->score = p.readInt();
	this->percent = p.readInt();
	this->status = static_cast<GStatus>(p.readInt());

	int newscore = this->score;
	if (newscore - oldscore != 0) {
		//std::cout << "newscore: " << newscore << "oldscore:" << oldscore << std::endl;
		//std::cout << "stomach:" << stomach << std::endl;
		stomach = stomach + newscore - oldscore;
		std::cout << "stomach:" << stomach << std::endl;
	}
	//check if need to burp
	//stomach = stomach + newscore - oldscore;
	//std::cout << "stomach:" << stomach << std::endl;
	if (stomach >= MAX_STOMACH_SIZE) {
		burp_count += 1;
		std::cout << "should burp" << std::endl;
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
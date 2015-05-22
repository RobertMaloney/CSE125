#include "Player.h"
#include <iostream>
#include <gtx\string_cast.hpp>

//TODO Config file
Player::Player(Model thebm, float radius, float theta, float azimuth, float direction) : MoveableObject() {
	//this->loc = vec4(radius, theta, azimuth, direction);
    this->rm = thebm;

	for (int i = 0; i < 5; ++i)
		this->moves[i] = false;

	this->type = PLAYER;
	this->isJumping = false;
	this->score = 0;
	this->modelRadius = 7.f;
	this->setMass(10.f);
	this->height = 550.f;
	this->modelRadius = 5.f;
	this->modelHeight = 5.f;
	this->status = PENDING;
}

Player::~Player() {

}

int Player::getScore(){
	return this->score;
}

void Player::setScore(int s){
	this->score = s;
}

int Player::getPercent(){
	return this->percent;
}

void Player::setPercent(int p){
	this->percent = p;
}

GStatus Player::getStatus(){
	return this->status;
}

void Player::setStatus(GStatus s){
	this->status = s;
}

bool Player::getMoving(int index) {
	return moves[index];
}

void Player::setMoving(int index, bool b) {
	moves[index] = b;
}



void Player::integrate(float dt) {

	if (moves[RIGHT]) {
		angle -= 1.f;
		this->velocity = this->rotateInXYPlane(this->velocity, glm::radians(-1.f));
	}
	if (moves[LEFT]) {
		angle += 1.f;
		this->velocity = this->rotateInXYPlane(this->velocity, glm::radians(1.f));
	}

	float cosa = glm::cos(glm::radians(angle));
	float sina = glm::sin(glm::radians(angle));
	

	float forceConst = 50.f * (log(this->getMass() == 0) ? 1 : log(this->getMass()));

	if (moves[UP]) {
		this->addForce(cosa * forceConst, sina * forceConst, 0.f);

	}
	
	if (moves[DOWN]) {
		this->addForce(cosa * -forceConst, sina * -forceConst, 0.f);
	}

	if (moves[JUMP] && !this->isJumping) {
		this->verticalComponent.force = 80000.f * (log(this->getMass() == 0) ? 1 : log(this->getMass()));
		this->isJumping = true;
		std::cout << "is jumping " << std::endl;
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
	if (target.getVisible()){
		switch (target.getType()) {
		case PLAYER:
			this->velocity *= -1;
			break;
		case GAMEOBJECT:
			//std::cout << " type " << target.getType() << std::endl;
			//std::cout << " id " << target.getId() << std::endl;
			this->velocity *= -1;
			break;
		case IEATABLE:
			
			std::cout << "EAT " << endl;
			IEatable* eatable = dynamic_cast<IEatable*>(&target);
			if (eatable){
				std::cout << this->getId() << " old score: " << this->getScore() << endl;
				this->setScore(this->getScore() + eatable->getPoints());
				std::cout << this->getId() << " new score: " << this->getScore() << endl;
			}
			else{
				std::cout << "Error: EATABLE is null: " << typeid(target).name() << endl;
			}
			target.setVisible(false);
			//TODO Render needs to figure out (not) rendering dead/invisible object
			break;
		}
			
	}
	
}

void Player::serialize(Packet & p) {

	GameObject::serialize(p);

	p.writeInt(this->score);
	p.writeInt(this->percent);
	p.writeInt(this->status);
	//TODO: moves??? no. just no.
}


void Player::deserialize(Packet & p) {
	GameObject::deserialize(p);
	this->score = p.readInt();
	this->percent = p.readInt();
	this->status = static_cast<GStatus>(p.readInt());
}
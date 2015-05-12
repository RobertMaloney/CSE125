#include "Player.h"


//TODO Config file
Player::Player(Model thebm, float radius, float theta, float azimuth, float direction) : GameObject(radius, theta, azimuth, direction) {

	this->loc = vec4(radius, theta, azimuth, direction);
   this->rm = thebm;
   this->moves[0] = false;
   this->moves[1] = false;
   this->moves[2] = false;
   this->moves[3] = false;

	for (int i = 0; i < 4; ++i)
		this->moves[i] = false;

	this->type = PLAYER;
	this->velocity = 0;
	this->modelRadius = 5.f;
}

Player::~Player() {

}


bool Player::getMoving(int index) {
	return moves[index];
}

void Player::setMoving(int index, bool b) {
	moves[index] = b;
}


void Player::update(float dt) {
	if (moves[UP]) {
		velocity += PLAYER_ACCELERATION;
	}
	if (moves[DOWN]) {
		velocity -= PLAYER_ACCELERATION;
	}
	if (moves[RIGHT]) {
		loc.w -= 1.f;
	}
	if (moves[LEFT]) {
		loc.w += 1.f;
	}
	// if there was no input simulate friction
	if (!moves[UP] && !moves[DOWN]) {
		if (glm::abs(velocity) <= FRICTION && glm::abs(velocity) >= 0) {
			velocity = 0;
		} else { 
			velocity = (velocity > 0) ? velocity - FRICTION : velocity + FRICTION;
		}
	}
	// move the player
	loc.z += glm::cos(glm::radians(loc.w)) * dt * velocity;
	loc.y += glm::sin(glm::radians(loc.w)) * dt * velocity;
}


void Player::collide(float dt, const GameObject & target) {
	switch (target.getType()) {
		case GAMEOBJECT:
			this->velocity *= -1;
			break;
		case PLAYER:
			loc.z -= glm::cos(glm::radians(loc.w)) * dt * velocity;
			loc.y -= glm::sin(glm::radians(loc.w)) * dt * velocity;
			this->velocity *= -1;
			break;
		default:
			break;
}
}
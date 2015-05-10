#include "Player.h"


//TODO Config file
Player::Player(BlobModel thebm, float radius, float theta, float azimuth, float direction) : GameObject(radius, theta, azimuth, direction) {

	this->loc = glm::vec4(radius, theta, azimuth, direction);
	this->bm = thebm;

	for (int i = 0; i < 4; ++i)
		this->moves[i] = false;

	this->type = ObjectType::PLAYER;
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
	if (moves[Player::UP]) {
		velocity += PLAYER_ACCELERATION;
	}

	if (moves[Player::DOWN]) {
		velocity -= PLAYER_ACCELERATION;
	}

	if (moves[Player::RIGHT]) {
		loc.w -= 1.f;
	}

	if (moves[Player::LEFT]) {
		loc.w += 1.f;
	}

	// if there was no input simulate friction
	if (!moves[Player::UP] && !moves[Player::DOWN]) {
		if (glm::abs(velocity) <= FRICTION && glm::abs(velocity) >= 0) {
			velocity = 0;
		} else { 
			velocity = (velocity > 0) ? velocity - FRICTION : velocity + FRICTION;
		}
	}

	loc.z += glm::cos(glm::radians(loc.w)) * dt * velocity;
	loc.y += glm::sin(glm::radians(loc.w)) * dt * velocity;
}


void Player::collide(const GameObject & target) {
	loc.z -= glm::cos(glm::radians(loc.w)) * (1000.f / 60.f) * velocity;
	loc.y -= glm::sin(glm::radians(loc.w)) * (1000.f / 60.f) * velocity;

	switch (target.getType()) {
		case GAMEOBJECT:
			this->velocity *= -1;
			break;
		case PLAYER:
			this->velocity *= -1;
			break;
	}
}
#include "Random.h"
#include <iostream>

std::default_random_engine Random::gen;

glm::vec3 Random::ballRand(float radius) {
	static std::uniform_real_distribution<float> circle(0.f, 360.f);
	glm::vec3 result(radius, 0, 0);
	float angle0 = circle(gen);
	float angle1 = circle(gen);
	return glm::quat(glm::vec3(0, angle0, angle1)) * result;
}

float Random::linearRand(float min, float max) {
	static std::uniform_real_distribution<float> line(0.f, 1.f);
	return line(gen) * (max - min) + min;
}
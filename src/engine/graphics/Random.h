#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <glm.hpp>
#include <gtc\quaternion.hpp>

class Random {
private:
	static std::default_random_engine gen;

public:
	static glm::vec3 ballRand(float);
	static float linearRand(float, float);
};
#endif
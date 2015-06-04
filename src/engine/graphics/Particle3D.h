#ifndef PARTICLE_NODE_H
#define PARTICLE_NODE_H

#include <iostream>
#include <glm.hpp>
#include <vec3.hpp>
#include <vector>
#include <ctime>
#include <string>
#include <random>
#include "Node.h"
#include "HUD.h"
#include "Quad.h"
#include "Random.h"

struct Particle3D {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 color;
	float size;
	float life;
};

class ParticleSystem : public Node {
private:
	std::vector<Particle3D> m_particles;
	Renderable* particleGraphic;
	GLuint texId;

public:
	ParticleSystem(int numParticles, Renderable* geo, GLuint tex) {
		assert(numParticles > 0);
		std::default_random_engine gen;
		std::uniform_real_distribution<float> dist0(0.f, 360.f);
		std::uniform_real_distribution<float> dist1(2.f, 5.f);

		for (int i = 0; i < numParticles; ++i) { // random on unit circle for now
			Particle3D p;
			p.position = glm::vec3(0);
			p.velocity = Random::ballRand(Random::linearRand(2.f, 5.f));
			p.color = glm::vec3(1);
			p.life = Random::linearRand(1.f, 3.f);
			p.size = 1.f;
			m_particles.push_back(p);
		}

		particleGraphic = geo;
		texId = tex;
	}
	virtual MatrixNode* asMatrixNode() {
		return 0;
	}
	virtual Geode* asGeode() {
		return 0;
	}
	virtual CameraNode* asCamera() {
		return 0;
	}
	virtual ParticleSystem* asPSystem() {
		return this;
	}
	virtual std::string getName() {
		return "ParticleSystem";
	}
	void simulate(float timeElapsed) {
		for (auto it = m_particles.begin(); it != m_particles.end(); ) {
			Particle3D& p = *it;
			p.life -= timeElapsed;
			if (p.life < 0) it = m_particles.erase(it);
			else {
				p.position += p.velocity * timeElapsed;
				++it;
			}
		}
	}
	void render(glm::mat4& transform) {
		if (m_particles.size() < 1) return;

		particleGraphic->setTextureId(texId);
		particleGraphic->setIsSkybox(false);

		glm::mat4 tmp;
		for (auto it = m_particles.begin(); it != m_particles.end(); ++it) {
			tmp = glm::translate(transform, it->position);
			particleGraphic->render(&tmp);
		}
	}
};
#endif
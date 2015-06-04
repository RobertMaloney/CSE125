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
#include "..\utility\Config.h"

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

		for (int i = 0; i < numParticles; ++i) { // random on unit circle for now
			Particle3D p;
			p.position = Random::ballRand(Random::linearRand(0.5f, 0.5f));
			p.velocity = Random::ballRand(Random::linearRand(Config::settings["particles"]["velocity"][0].asFloat(), Config::settings["particles"]["velocity"][1].asFloat()));
			p.color = glm::vec3(1);
			p.life = Random::linearRand(Config::settings["particles"]["life"][0].asFloat(), Config::settings["particles"]["life"][1].asFloat());
			p.size = Random::linearRand(Config::settings["particles"]["size"][0].asFloat(), Config::settings["particles"]["size"][1].asFloat());
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
	void render(glm::mat4& transform, GLuint shaderId) {
		if (m_particles.size() < 1) return;

		glUniform1f(glGetUniformLocation(shaderId, "hasTex"), 1);
		glUniform1i(glGetUniformLocation(shaderId, "billboard"), 1);
		glUniform1i(glGetUniformLocation(shaderId, "tex"), 0);

		// Alpha blending (particles)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		particleGraphic->setTextureId(texId);
		particleGraphic->setIsSkybox(false);

		glm::mat4 tmp;
		for (auto it = m_particles.begin(); it != m_particles.end(); ++it) {
			glUniform1f(glGetUniformLocation(shaderId, "billboardScale"), it->size);
			tmp = glm::translate(transform, it->position);
			particleGraphic->render(&tmp);
		}
		glUniform1i(glGetUniformLocation(shaderId, "billboard"), 0);
		glDisable(GL_BLEND);
	}
};
#endif
#ifndef PARTICLE_NODE_H
#define PARTICLE_NODE_H

#include <iostream>
#include <glm.hpp>
#include <vec3.hpp>
#include <vector>
#include <ctime>
#include <algorithm>
#include "Node.h"
//#include "Quad.h"
#include "Random.h"
#include "PType.h"
#include "..\utility\Config.h"




struct Particle3D {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 rotation;
	glm::vec3 color;
	float size;
	float life, totalLife;

};

static int __cdecl particleSort(const void* a, const void* b) {
	return 0;
}

class ParticleSystem : public Node {
private:
	std::vector<Particle3D> m_particles;
	Renderable* particleGraphic;
	GLuint texId;
	PType type;
	//int num;

	// https://github.com/jesusgollonet/ofpennereasing/blob/master/PennerEasing/Expo.cpp
	float easeInOut(float t, float b, float c, float d) {
		if (t == 0) return b;
		if (t == d) return b + c;
		if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
		return c / 2 * (-pow(2, -10 * --t) + 2) + b;
	}



public:

	ParticleSystem(int numParticles, Renderable* geo, GLuint tex, PType t) {
		hasParticle = false;
		assert(numParticles > 0);

		//num = numParticles;

		type = t;

		texId = tex;

		init(numParticles, geo);

	}

	void init(int numP, Renderable* geo){
		particleGraphic = geo;
		int numParticles = numP;
		glm::vec3 colors[] = {
			glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(1.f, 0.5f, 0.f),
			glm::vec3(1.f, 1.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.29f, 0.f, 0.51f),
			glm::vec3(0.56f, 0.f, 1.f), };
			//glm::vec3(0.f, 0.f, 0.f) };
		if (type == P_PLAYER){
			numParticles = 100;
		}
		for (int i = 0; i < numParticles; ++i) { // random on unit circle for now
			Particle3D p;
			p.position = Random::ballRand(Random::linearRand(0.5f, 0.5f));

			if (type == P_PLAYER){
				p.color = colors[i % 7];
				p.velocity = Random::ballRand(Random::linearRand(Config::settings["gas"]["velocity"][0].asFloat(), Config::settings["gas"]["velocity"][1].asFloat()));;
				p.acceleration = glm::vec3(0, 0, Random::linearRand(Config::settings["gas"]["acceleration"][0].asFloat(), Config::settings["gas"]["acceleration"][1].asFloat()));
				p.life = Random::linearRand(Config::settings["gas"]["life"][0].asFloat(), Config::settings["gas"]["life"][1].asFloat());
				p.totalLife = p.life;
				p.size = Random::linearRand(Config::settings["gas"]["size"][0].asFloat(), Config::settings["gas"]["size"][1].asFloat());
			}
			else{//RES
				p.velocity = Random::ballRand(Random::linearRand(Config::settings["particles"]["velocity"][0].asFloat(), Config::settings["particles"]["velocity"][1].asFloat()));
				p.acceleration = glm::vec3(0, 0, Random::linearRand(Config::settings["particles"]["acceleration"][0].asFloat(), Config::settings["particles"]["acceleration"][1].asFloat()));
				p.color = chooseColor(type, i);
				p.life = Random::linearRand(Config::settings["particles"]["life"][0].asFloat(), Config::settings["particles"]["life"][1].asFloat());
				p.totalLife = p.life;
				p.size = Random::linearRand(Config::settings["particles"]["size"][0].asFloat(), Config::settings["particles"]["size"][1].asFloat());
			}

			m_particles.push_back(p);
		}
	}

	glm::vec3 chooseColor(PType t, int i){
		glm::vec3 colors[] = {
			glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(1.f, 0.5f, 0.f),
			glm::vec3(1.f, 1.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.29f, 0.f, 0.51f),
			glm::vec3(0.56f, 0.f, 1.f),
		    glm::vec3(1.0f,1.0f,1.0f),
		    glm::vec3(0.0f,0.0f,0.0f)};

		switch (t){
		case P_FLOWER:
			return colors[6];
		case P_TREE:
			return colors[3];
		case P_STUMP:
			return colors[4];
		case P_ROCK:
			return colors[5];
		case P_TALL:
			return colors[7];
		case P_MUSH:
			return colors[1];
		case P_POWER:
			return colors[i % 7];
		default:
			return colors[8];

		}
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
				//if (type == PType::P_PLAYER){
					//p.position
			//	}
			//	else{
					p.velocity += p.acceleration * timeElapsed;
					p.position += p.velocity * timeElapsed + 0.5f * p.acceleration * timeElapsed * timeElapsed;
			//	}
				++it;
			}
		}
	}
	void render(glm::mat4& transform, glm::mat4& cam, GLuint shaderId) {
		if (m_particles.size() < 1) return;

		glUniform1f(glGetUniformLocation(shaderId, "hasTex"), 1);
		glUniform1i(glGetUniformLocation(shaderId, "billboard"), 1);
		glUniform1i(glGetUniformLocation(shaderId, "tex"), 0);

		// Z sorting for particles
		glm::mat4 modelview = cam * transform;
		std::sort(m_particles.begin(), m_particles.end(), [&modelview](Particle3D a, Particle3D b) -> bool {
			float deptha = glm::translate(modelview, a.position)[3].z;
			float depthb = glm::translate(modelview, b.position)[3].z;
			return deptha < depthb;
		});

		// Alpha blending (particles)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		particleGraphic->setTextureId(texId);
		particleGraphic->setIsSkybox(false);

		glm::mat4 tmp;
		for (auto it = m_particles.begin(); it != m_particles.end(); ++it) {
			glUniform3fv(glGetUniformLocation(shaderId, "colorOverride"), 1, glm::value_ptr(it->color));
			float opacity = easeInOut(it->life, 0.f, 1.0f, it->totalLife);
			glUniform1f(glGetUniformLocation(shaderId, "transparencyOverride"),opacity );
			glUniform1f(glGetUniformLocation(shaderId, "billboardScale"), it->size);
			tmp = glm::translate(transform, it->position);
			particleGraphic->render(&tmp);
		}
		glUniform1i(glGetUniformLocation(shaderId, "billboard"), 0);
		glDisable(GL_BLEND);
	}


};
#endif
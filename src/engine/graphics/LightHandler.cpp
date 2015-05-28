#include "LightHandler.h"
#include <sstream>
#include <iostream>

std::map<int, Light> LightHandler::lights;
int LightHandler::numLights;
bool LightHandler::hasChanged;

void LightHandler::Initialize() {
	numLights = 0;
	hasChanged = false;
}

int LightHandler::addLight(Light& l) {
	hasChanged = true;
	lights[numLights] = l;
	return numLights++;
}

int LightHandler::addLight(int mode, vec3 pos, float sd, vec3 cd, float ss, vec3 cs, float sa, vec3 ca) {
	Light l;
	l.valid = true;
	l.changed = true;
	l.mode = mode;
	l.position = pos;
	l.scalarDiffuse = sd;
	l.colorDiffuse = cd;
	l.scalarSpecular = ss;
	l.colorSpecular = cs;
	l.scalarAmbient = sa;
	l.colorAmbient = ca;

	return addLight(l);
}

Light& LightHandler::getLight(int index) {
	return lights[index];
}

void LightHandler::changePosition(int index, vec3 newpos) {
	lights[index].position = newpos;
	lights[index].changed = true;
}

void LightHandler::updateLighting(GLuint shaderId) {
	int glslid = 0;
	std::stringstream ss;
	for (auto iter = lights.begin(); iter != lights.end() && glslid < MAXLIGHTS; ++iter, ++glslid) {
		ss << "lights[" << glslid << "]";
		Light l = iter->second;
		if (!l.changed) continue;

		GLint lValid = glGetUniformLocation(shaderId, (ss.str() + ".valid").c_str());
		GLint lMode = glGetUniformLocation(shaderId, (ss.str() + ".mode").c_str());
		GLint lPos = glGetUniformLocation(shaderId, (ss.str() + ".position").c_str());
		GLint lSD = glGetUniformLocation(shaderId, (ss.str() + ".scalarDiffuse").c_str());
		GLint lCD = glGetUniformLocation(shaderId, (ss.str() + ".colorDiffuse").c_str());
		GLint lSS = glGetUniformLocation(shaderId, (ss.str() + ".scalarSpecular").c_str());
		GLint lCS = glGetUniformLocation(shaderId, (ss.str() + ".colorSpecular").c_str());
		GLint lSA = glGetUniformLocation(shaderId, (ss.str() + ".scalarAmbient").c_str());
		GLint lCA = glGetUniformLocation(shaderId, (ss.str() + ".colorAmbient").c_str());

		glUniform1i(lValid, l.valid);
		glUniform1i(lMode, l.mode);
		glUniform3fv(lPos, 1, glm::value_ptr(l.position));
		glUniform1f(lSD, l.scalarDiffuse);
		glUniform3fv(lCD, 1, glm::value_ptr(l.colorDiffuse));
		glUniform1f(lSS, l.scalarSpecular);
		glUniform3fv(lCS, 1, glm::value_ptr(l.colorSpecular));
		glUniform1f(lSA, l.scalarAmbient);
		glUniform3fv(lCA, 1, glm::value_ptr(l.colorAmbient));

		iter->second.changed = false;
		ss.str("");
	}

	GLint nl = glGetUniformLocation(shaderId, "numLights");
	glUniform1i(nl, glslid);
}

void LightHandler::removeLightById(int id) {
	lights.erase(id);
}
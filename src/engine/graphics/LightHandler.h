#ifndef LIGHT_HANDLER_H
#define LIGHT_HANDLER_H

#include <glm.hpp>
#define GLEW_STATIC
#include <glew.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <map>
#include <string>

using glm::vec3;

struct Light {
	bool valid;
	int mode;
	vec3 position;

	// DIFFUSE
	float scalarDiffuse;
	vec3 colorDiffuse;

	// SPECULAR
	float scalarSpecular;
	vec3 colorSpecular;

	// AMBIENT
	float scalarAmbient;
	vec3 colorAmbient;
};

class LightHandler {
private:
	static const int MAXLIGHTS = 20;
	static std::map<int, Light> lights;
	static int numLights;
	static bool hasChanged;

public:
	static void Initialize();
	static int addLight(Light&);
	static int addLight(int, vec3, float, vec3, float, vec3, float, vec3);
	static void updateLighting(GLuint);
	static void removeLightById(int);
};
#endif
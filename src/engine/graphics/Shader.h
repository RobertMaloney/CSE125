#ifndef SHADER_H
#define SHADER_H

#include <string>
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

class Shader {
private:
	const std::string version = "#version 330 core\n";
	GLuint m_id;

public:
	Shader(std::string vert, std::string frag);
	void Use();
	GLuint Id();
};
#endif
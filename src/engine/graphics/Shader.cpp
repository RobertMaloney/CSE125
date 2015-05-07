#include "Shader.h"
#include "..\utility\System.h"

Shader::Shader(std::string vert, std::string frag) {
	GLuint m_vertexShader, m_fragmentShader;

	// Load shader files
	std::string vertInfo = System::File2String(vert);
	std::string fragInfo = System::File2String(frag);

	GLchar const* vertFiles[] = { version.c_str(), vertInfo.c_str() };
	GLint vertLengths[] = { version.size(), vertInfo.size() };
	GLchar const* fragFiles[] = { version.c_str(), fragInfo.c_str() };
	GLint fragLengths[] = { version.size(), fragInfo.size() };

	// compile shaders
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_vertexShader, 2, vertFiles, vertLengths);
	glShaderSource(m_fragmentShader, 2, fragFiles, fragLengths);
	glCompileShader(m_vertexShader);
	glCompileShader(m_fragmentShader);

	// If shaders fail, output error
	GLint status;
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		printf("Vertex Shader Failed to Compile\n");
		char buffer[512];
		glGetShaderInfoLog(m_vertexShader, 512, NULL, buffer);
		printf("%s\n", buffer);
		return;
	}
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		printf("Fragment Shader Failed to Compile\n");
		char buffer[512];
		glGetShaderInfoLog(m_fragmentShader, 512, NULL, buffer);
		printf("%s\n", buffer);
		return;
	}

	// Link shaders to program
	m_id = glCreateProgram();
	glAttachShader(m_id, m_vertexShader);
	glAttachShader(m_id, m_fragmentShader);
	glBindFragDataLocation(m_id, 0, "outColor");
	glLinkProgram(m_id);
}

void Shader::Use() {
	glUseProgram(m_id);
}

GLuint Shader::Id() {
	return m_id;
}
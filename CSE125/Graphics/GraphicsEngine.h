#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#define GLEW_STATIC
#include <glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>
#include "Renderable.h"

typedef void (*KeyCallback) (int,int,int);

class GraphicsEngine {
public:
	static void Initialize();
	static bool Closing();
	static void CloseGame();
	static void DrawAndPoll();
	static void Destroy();
	static void SetKeyCallback(KeyCallback);
	static KeyCallback GetKeyCallback();

private:
	static glm::mat4				m_view, m_projection;
	static bool						m_initialized;
	static GLFWwindow				*m_window;
	static GLint					m_uniView, m_uniProjection;
	static std::vector<Renderable*> m_objects;
	static GLuint					m_vertexShader, m_fragmentShader, m_shaderProgram;
	static KeyCallback				m_keyCallback;
};

#endif
#include <Windows.h>

// STL
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "GraphicsEngine.h"
#include "..\graphics\Cube.h"
#include "..\graphics\Geometry.h"
#include "..\utility\System.h"

using namespace std;

// Graphics Engine Static Members
vector<Renderable*> GraphicsEngine::m_objects;
glm::mat4			GraphicsEngine::m_view, GraphicsEngine::m_projection;
bool				GraphicsEngine::m_initialized = false;
GLFWwindow			*GraphicsEngine::m_window;

GLint				GraphicsEngine::m_uniView,
					GraphicsEngine::m_uniProjection;

GLuint				GraphicsEngine::m_vertexShader,
					GraphicsEngine::m_fragmentShader,
					GraphicsEngine::m_shaderProgram;

KeyCallback			GraphicsEngine::m_keyCallback = NULL;

Renderable			*GraphicsEngine::m_player = NULL;

string version = "#version 150\n";

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (GraphicsEngine::GetKeyCallback()) GraphicsEngine::GetKeyCallback()(key, action, mods);
}

/**
 * GraphicsEngine::Initialize()
 * Description: This function initializes the graphics pipeline, compiles the
 * shaders, creates the window, and sets up the view and projection matrices
 */
void GraphicsEngine::Initialize() {
	if (!glfwInit())
		return;

	cout << "Current Dir: " << System::CurrentDirectory() << endl;

	// Load shader files
	string vertInfo = System::File2String("../engine/graphics/Shaders/test.vert");
	string fragInfo = System::File2String("../engine/graphics/Shaders/test.frag");

	GLchar const* vertFiles[] = { version.c_str(), vertInfo.c_str() };
	GLint vertLengths[] = { version.size(), vertInfo.size() };
	GLchar const* fragFiles[] = { version.c_str(), fragInfo.c_str() };
	GLint fragLengths[] = { version.size(), fragInfo.size() };

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_window = glfwCreateWindow(800, 800, "CSE 125", NULL, NULL);
	glfwSetKeyCallback(m_window, key_callback);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	glewInit();

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
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);
	glBindFragDataLocation(m_shaderProgram, 0, "outColor");
	glLinkProgram(m_shaderProgram);
	glUseProgram(m_shaderProgram);

	// Turn on z-buffering
	glEnable(GL_DEPTH_TEST);

	// Testing renderables
	const int CUBE_COUNT = 0;
	for (int i = 0; i < CUBE_COUNT; ++i) {
		glm::vec3 position(-2.f + 0.4f*(i % 10), -2.f + 0.4f*(i / 10), 0.1f);
		m_objects.push_back(new Cube(position, glm::angleAxis(glm::radians((float)i), glm::vec3(0, 0, 1)), glm::vec3(1.f, 1.f, 1.f), 0.02f + 0.08f * (i / (float)100)));
	}
	//m_objects.push_back(new Cube(glm::vec3(0, 0, 0), glm::quat(), glm::vec3(1.f, 1.f, 1.f), 0.5f));
	m_player = new Geometry("../../media/pb.obj");

	// view and projection matrix locations in the shader program
	m_uniView = glGetUniformLocation(m_shaderProgram, "view");
	m_uniProjection = glGetUniformLocation(m_shaderProgram, "projection");

	m_view = glm::lookAt(
		glm::vec3(0.f, 3.f, 2.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 1.f));

	if (glGetError() != 0) printf("Error Code: %d\n", glGetError());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_initialized = true;
}

/**
 * GraphicsEngine::Closing()
 * Description: The bool callback used to tell the main game loop that it
 * should close.
 */
bool GraphicsEngine::Closing() {
	return !m_initialized || glfwWindowShouldClose(m_window);
}

/**
 * GraphicsEngine::CloseGame()
 * Description: This function tells the main loop to shut down
 */
void GraphicsEngine::CloseGame() {
	glfwSetWindowShouldClose(m_window, GL_TRUE);
}

/**
 * GraphicsEngine::DrawAndPoll()
 * Description: This function should be called within in the main game loop to
 * draw the next frame and poll for user input
 */
void GraphicsEngine::DrawAndPoll() {
	int height, width;
	glfwGetWindowSize(m_window, &width, &height);

	m_projection = glm::perspective(
		glm::radians(45.f),
		((float)height) / width,
		1.f, 1000.f);

	glUniformMatrix4fv(m_uniView, 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(m_uniProjection, 1, GL_FALSE, glm::value_ptr(m_projection));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLint light = glGetUniformLocation(m_shaderProgram, "lightPosition");
	const float radius = 2.f;
	float sine = radius*glm::sin(glm::radians(90 * glfwGetTime()));
	float cosine = radius*glm::cos(glm::radians(90 * glfwGetTime()));
	glm::vec3 lightpos(cosine, sine, 1);
	glUniform3fv(light, 1, glm::value_ptr(lightpos));

	// render objects
	int renderableCount = m_objects.size();
	for (int i = 0; i < renderableCount; ++i) {
		//m_objects[i]->getMatrix() = glm::translate(m_objects[i]->getMatrix(), glm::vec3(0, 0, -0.01f));
		m_objects[i]->render();
	}

	m_player->render();

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

/**
 * GraphicsEngine::Destroy()
 * Description: This function cleans up the assets used by the graphics engine
 * before shutting down.
 */
void GraphicsEngine::Destroy() {
	if (m_initialized) {
		int renderableCount = m_objects.size();
		for (int i = 0; i < renderableCount; ++i)
			free(m_objects[i]);

		glDeleteProgram(m_shaderProgram);
		glDeleteShader(m_fragmentShader);
		glDeleteShader(m_vertexShader);

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}

void GraphicsEngine::SetKeyCallback(KeyCallback key_fn) {
	m_keyCallback = key_fn;
}

KeyCallback GraphicsEngine::GetKeyCallback() {
	return m_keyCallback;
}

void GraphicsEngine::MoveUp() {
	if (m_player)
	m_player->getMatrix() = glm::translate(m_player->getMatrix(), glm::vec3(0, -1, 0));
}

void GraphicsEngine::MoveLeft() {
	if (m_player)
	m_player->getMatrix() = glm::translate(m_player->getMatrix(), glm::vec3(1, 0, 0));
}

void GraphicsEngine::MoveDown() {
	if (m_player)
	m_player->getMatrix() = glm::translate(m_player->getMatrix(), glm::vec3(0, 1, 0));
}

void GraphicsEngine::MoveRight() {
	if (m_player)
	m_player->getMatrix() = glm::translate(m_player->getMatrix(), glm::vec3(-1, 0, 0));
}

void GraphicsEngine::UpdatePlayer(deque<Packet> & data) {
	if (data.size() > 0 && data[0].Size() > 0) {
		float * matPointer = glm::value_ptr(m_player->getMatrix());
		float * newData = (float*)&data[0][0];
		for (int i = 0; i < 16; ++i) {
			matPointer[i] = newData[i];
		}
	}
}
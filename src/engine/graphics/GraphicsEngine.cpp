//#include <Windows.h>

#include "GraphicsEngine.h"

//#include "..\utility\ObjectDB.h"
#include "..\graphics\Cube.h"
#include "..\graphics\Geometry.h"
#include "..\utility\System.h"
#include "..\utility\Config.h"
#include <gtc\constants.hpp>
#include "Shader.h"
#include "Skybox.h"
#include "LightHandler.h"
#include "HUD.h"
#include "Ground.h"
#include "..\utility\GameSound.h"

using namespace std;

// Graphics Engine Static Members
vector<MatrixNode*> GraphicsEngine::m_objects;
glm::mat4			GraphicsEngine::m_view, GraphicsEngine::m_projection;
glm::vec2           GraphicsEngine::m_screen_scale;
bool				GraphicsEngine::m_initialized = false;
GLFWwindow			*GraphicsEngine::m_window;

GLint				GraphicsEngine::m_uniView,
GraphicsEngine::m_uniProjection;

KeyCallback			GraphicsEngine::m_keyCallback = NULL;
MatrixNode			*GraphicsEngine::m_player = NULL,

*GraphicsEngine::m_scene = NULL;

CameraNode			*GraphicsEngine::m_mainCamera = NULL;
CameraNode			*GraphicsEngine::m_minimapCamera = NULL;

MenuStatus          GraphicsEngine::ms = START;
GLuint				GraphicsEngine::m_skyboxId = 0;

GLuint				GraphicsEngine::m_HudId1 = 0;
GLuint				GraphicsEngine::m_HudId2 = 0;
GLuint				GraphicsEngine::m_HudId3 = 0;
GLuint				GraphicsEngine::m_HudId4 = 0;
GLuint				GraphicsEngine::m_HudIdN1 = 0;
GLuint				GraphicsEngine::m_HudIdN2 = 0;
GLuint				GraphicsEngine::m_HudIdN3 = 0;
GLuint				GraphicsEngine::m_HudIdN4 = 0;

GLuint				GraphicsEngine::m_groundId = 0;
GLuint				GraphicsEngine::m_borderId = 0;
GLuint				GraphicsEngine::m_plusId = 0;
GLuint				GraphicsEngine::m_minusId = 0;
GLuint				GraphicsEngine::m_timerId = 0;

GLuint				GraphicsEngine::m_menuId1 = 0;
GLuint				GraphicsEngine::m_menuId2 = 0;
GLuint				GraphicsEngine::m_menuId3 = 0;
GLuint				GraphicsEngine::m_menuId4 = 0;
GLuint				GraphicsEngine::m_menuId5 = 0;
GLuint				GraphicsEngine::m_menuId6 = 0;
int					GraphicsEngine::HUDW = 100;
int					GraphicsEngine::HUDH = 100;


Renderable			*GraphicsEngine::m_skybox = NULL;
Renderable			*GraphicsEngine::m_border = NULL;
Renderable			*GraphicsEngine::m_plus = NULL;
Renderable			*GraphicsEngine::m_minus = NULL;
Renderable			*GraphicsEngine::m_timer = NULL;

Renderable			*GraphicsEngine::m_HUD1 = NULL;
Renderable			*GraphicsEngine::m_HUD2 = NULL;
Renderable			*GraphicsEngine::m_HUD3 = NULL;
Renderable			*GraphicsEngine::m_HUD4 = NULL;
Renderable			*GraphicsEngine::m_HUDN1 = NULL;
Renderable			*GraphicsEngine::m_HUDN2 = NULL;
Renderable			*GraphicsEngine::m_HUDN3 = NULL;
Renderable			*GraphicsEngine::m_HUDN4 = NULL;

Renderable			*GraphicsEngine::worldModel = NULL;
Renderable			*GraphicsEngine::m_menu = NULL;

Shader				*GraphicsEngine::m_defaultShader, *GraphicsEngine::m_skyboxShader, *GraphicsEngine::m_textureShader;// , *GraphicsEngine::m_tShader;

int					GraphicsEngine::m_sunLight;

unordered_map<ObjectId, MatrixNode*> GraphicsEngine::objNodeMap;

string version = "#version 330 core\n";

/**
* Description: This function is called when glfwPollEvents() is called.
*/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (GraphicsEngine::GetKeyCallback()) GraphicsEngine::GetKeyCallback()(key, action, mods);
	InputHandler::handleKey(key, action, mods);
}

/**
* Description: This function handles the cursor position
*/
const float lookScale = 0.3f;
static void cursor_callback(GLFWwindow* window, double x, double y) {
	//std::cout << "(x,y): (" << x << ", " << y << ")\n";
	InputHandler::handleMouse(-x * lookScale, y * lookScale);

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) glfwSetCursorPos(window, 0, 0);
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
	m_scene = new MatrixNode();
	
	// Load shader files
	string vertInfo = System::File2String("../engine/graphics/Shaders/test.vert");
	string fragInfo = System::File2String("../engine/graphics/Shaders/test.frag");

	GLchar const* vertFiles[] = { version.c_str(), vertInfo.c_str() };
	GLint vertLengths[] = { version.size(), vertInfo.size() };
	GLchar const* fragFiles[] = { version.c_str(), fragInfo.c_str() };
	GLint fragLengths[] = { version.size(), fragInfo.size() };

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_window = glfwCreateWindow(Config::settings["graphics"]["width"].asInt(), Config::settings["graphics"]["height"].asInt(), "Dusty Planet v0.9", NULL, NULL);
	glfwSetKeyCallback(m_window, key_callback);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	// mouse handling
	setCursor(GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(m_window, cursor_callback);
	glfwSetCursorPos(m_window, 0, 0);

	glewExperimental = GL_TRUE;
	glewInit();

	m_defaultShader = new Shader("../engine/graphics/Shaders/default.vert", "../engine/graphics/Shaders/default.frag");
	m_skyboxShader = new Shader("../engine/graphics/Shaders/skybox.vert", "../engine/graphics/Shaders/skybox.frag");
	m_textureShader = new Shader("../engine/graphics/Shaders/texture2D.vert", "../engine/graphics/Shaders/texture2D.frag");
	//m_tShader = new Shader("../engine/graphics/Shaders/t.vert", "../engine/graphics/Shaders/t.frag");

	// Turn on z-buffering
	glEnable(GL_DEPTH_TEST);


	// SKYBOX
	m_skyboxShader->Use();
	m_skybox = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.f);
	m_skyboxId = Skybox::makeSkybox("../../media/texture/skybox/", 0);
	m_skybox->setIsSkybox(true);
	m_skybox->setTextureId(m_skyboxId);
	
	// HUD
    m_textureShader->Use();
	addHUD();

	// Menu
	m_textureShader->Use();
	m_menu = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.f);
	m_menuId1 = HUD::makeHUD("../../media/texture/bg_start.png", 2);//start start
	m_menuId2 = HUD::makeHUD("../../media/texture/bg_quit.png", 3);//start quit
	m_menuId3 = HUD::makeHUD("../../media/texture/win_replay.png", 4);//win replay
	m_menuId4 = HUD::makeHUD("../../media/texture/win_quit.png", 5);//win quit
	m_menuId5 = HUD::makeHUD("../../media/texture/lose_replay.png", 6);//lose replay
	m_menuId6 = HUD::makeHUD("../../media/texture/lose_quit.png", 7);//lose quit  TODO bug
	m_menu->setTextureId(m_menuId1);

	// WORLD
	//m_textureShader->Use();
	m_defaultShader->Use();
	worldModel = new Geometry("../../media/models/sphere.obj");

	//m_groundId = Ground::makeGround("../../media/texture/ground.png", 4);
	//worldModel->setTextureId(m_groundId);

	Geode* worldGeode = new Geode();
	worldGeode->setRenderable(worldModel);
	//worldGeode->setTex(true);
	m_scene->addChild(worldGeode);


	// CAMERA
	glm::mat4 camview = glm::lookAt(
		glm::vec3(0.f, 24.f, 16.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 1.f));
	m_mainCamera = new CameraNode();
	m_mainCamera->setViewMatrix(camview);
	
	glm::mat4 minimapview = glm::translate(glm::mat4(), glm::vec3(0, 0, -550));
	minimapview = glm::rotate(minimapview, glm::radians(180.f), glm::vec3(0, 0, 1));
	m_minimapCamera = new CameraNode();
	m_minimapCamera->setViewMatrix(minimapview);

	// PLAYER  (Player node is created by default
	/*Renderable * model = GraphicsEngine::selectModel(playerId);
	m_player = GraphicsEngine::addNode(model);
	m_player->addChild(m_mainCamera);
	m_player->addChild(m_minimapCamera);*/
	
	// LIGHTS
	m_sunLight = LightHandler::addLight(0, glm::vec3(-1, -1, -1), 1.f, glm::vec3(1, 1, 1), 0.f, glm::vec3(1, 1, 1), 0.5f, glm::vec3(1, 1, 1)); // direct light
	LightHandler::addLight(1, glm::vec3(0, 0, -505), 1.f, glm::vec3(1, 1, 1), 0.f, glm::vec3(0), 0.f, glm::vec3(0)); // point light

	if (glGetError() != 0) printf("Error Code: %d\n", glGetError());

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	m_initialized = true;

	m_screen_scale = glm::vec2(2.0f, 2.0f);

}

void GraphicsEngine::addHUD(){
	m_HUD1 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId1 = HUD::makeHUD("../../media/texture/HUD1.png", 1);
	m_HUD1->setTextureId(m_HudId1);

	m_HUD2 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId2 = HUD::makeHUD("../../media/texture/HUD2.png", 8);
	m_HUD2->setTextureId(m_HudId2);

	m_HUD3 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId3 = HUD::makeHUD("../../media/texture/HUD3.png", 9);
	m_HUD3->setTextureId(m_HudId3);

	m_HUD4 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId4 = HUD::makeHUD("../../media/texture/HUD4.png", 10);
	m_HUD4->setTextureId(m_HudId4);


	//TODO replace pictures with right ones

	m_HUDN1 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudIdN1 = HUD::makeHUD("../../media/texture/HUD.png", 11);
	m_HUDN1->setTextureId(m_HudIdN1);

	m_HUDN2 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudIdN2 = HUD::makeHUD("../../media/texture/HUD.png", 12);
	m_HUDN2->setTextureId(m_HudIdN2);

	m_HUDN3 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudIdN3 = HUD::makeHUD("../../media/texture/HUD.png", 13);
	m_HUDN3->setTextureId(m_HudIdN3);

	m_HUDN4 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudIdN4 = HUD::makeHUD("../../media/texture/HUD.png", 14);
	m_HUDN4->setTextureId(m_HudIdN4);

	//Mini map border
	m_border = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_borderId = HUD::makeHUD("../../media/texture/border.png", 15);
	m_border->setTextureId(m_borderId);

    // Zoom in and out buttons
	m_plus = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_plusId = HUD::makeHUD("../../media/texture/plus.png", 16);
	m_plus->setTextureId(m_plusId);

	m_minus = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_minusId = HUD::makeHUD("../../media/texture/minus.png", 17);
	m_minus->setTextureId(m_minusId);

	// Timer
	m_timer = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_timerId = HUD::makeHUD("../../media/texture/timer.png", 18);
	m_timer->setTextureId(m_timerId);
}

//TODO it is not working
void GraphicsEngine::ZoomIn() {
	/*CameraNode *a = m_minimapCamera;
	glm::mat4 mat = a->getFlatViewMatrix();
	mat[3][2] += 10;
	a->setViewMatrix(mat);*/

}
//TODO it is not working
void GraphicsEngine::ZoomOut() {
	/*CameraNode *a = m_minimapCamera;
	glm::mat4 mat = a->getFlatViewMatrix();
	mat[3][2] -= 10;
	a->setViewMatrix(mat);*/

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

	glViewport(0, 0, width, height);
	m_projection = glm::perspective(
		45.f,
		((float)width) / height,
		0.1f, 1000.f);

	std::pair<glm::mat4, glm::vec3> cameraData = m_mainCamera->getFlatViewMatrix();
	glm::mat4 view = cameraData.first;
	glm::mat4 skybox_view = glm::mat4(glm::mat3(view));

	//glm::vec4 glPos = m_projection * skybox_view * glm::vec4(1.f, 1.f, 1.f, 1.f);
	//std::cout << glm::to_string(glPos) << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render skybox
	glm::mat4 identity;
	glDepthMask(GL_FALSE);
	m_skyboxShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_skyboxShader->Id(), "view"), 1, GL_FALSE, glm::value_ptr(skybox_view));
	glUniformMatrix4fv(glGetUniformLocation(m_skyboxShader->Id(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(m_skyboxShader->Id(), "skybox"), 0);
	m_skybox->render(&identity);
	glDepthMask(GL_TRUE);

	// render the rest of the scene
	m_defaultShader->Use();

	// SUN AROUND PLANET
	glm::vec3 sunLightDir = LightHandler::getLight(m_sunLight).position;
	sunLightDir = glm::angleAxis(glm::radians(0.03f), glm::vec3(2, -2, 0)) * sunLightDir;
	LightHandler::changePosition(m_sunLight, sunLightDir);

	// Update lights
	LightHandler::updateLighting(m_defaultShader->Id());

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(m_defaultShader->Id(), "camPos"), 1, glm::value_ptr(cameraData.second));

	renderScene(m_scene, &identity);

	glViewport(width -HUDW*2, height-HUDW*2,HUDW*2, HUDH*2);
	glClear(GL_DEPTH_BUFFER_BIT);

	//view = m_minimapCamera->getFlatViewMatrix();

	// change aspect ratio
	m_projection = glm::perspective(
		45.f,
		1.f,
		0.1f, 1000.f);

	// minimap
	cameraData = m_minimapCamera->getFlatViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "view"), 1, GL_FALSE, glm::value_ptr(cameraData.first));
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform3fv(glGetUniformLocation(m_defaultShader->Id(), "camPos"), 1, glm::value_ptr(cameraData.second));

	renderScene(m_scene, &identity);

	// HUD
	glDepthMask(GL_FALSE);
	m_textureShader->Use();
	renderHUD(width, height, identity);
	glEnable(GL_DEPTH_TEST);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void GraphicsEngine::renderHUD(int width, int height, glm::mat4 & identity){
	//HUD1
	glViewport(0, height - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 1);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUD1->render(&identity);

	// HUD2
	glViewport(0, height - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 8);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUD2->render(&identity);


	//HUD3
	glViewport(0, height - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 9);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUD3->render(&identity);

	// HUD4
	glViewport(0, height - HUDH - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 10);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUD4->render(&identity);

	//TODO: replace with numbers...
	//HUDN1
	glViewport(0 + HUDW, height - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 11);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUDN1->render(&identity);

	// HUDN2
	glViewport(0 + HUDW, height - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 12);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUDN2->render(&identity);


	//HUDN3
	glViewport(0 + HUDW, height - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 13);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUDN3->render(&identity);

	// HUDN4
	glViewport(0 + HUDW, height - HUDH - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 14);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_HUDN4->render(&identity);


	//HUD on top of minimap
	glViewport(width - HUDW * 2, height - HUDH * 2, HUDW * 2, HUDH * 2);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 15);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	m_border->render(&identity);
	glDisable(GL_BLEND);

	// Zoom in 
	glViewport(width - HUDW * 2 - HUDW/2, height - HUDH + HUDW/2, HUDW/2, HUDH/2);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 16);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_plus->render(&identity);

	// Zoom out
	glViewport(width - HUDW * 2 - HUDW / 2, height - HUDH , HUDW / 2, HUDH / 2);
	glClear(GL_DEPTH_BUFFER_BIT);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 17);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_minus->render(&identity);

	// Timer
	glViewport(width - HUDW * 2 - HUDW / 2 - HUDW * 5/4, height - HUDH + HUDH / 4, HUDW * 5/4, HUDH * 3 / 4);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 18);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_timer->render(&identity);



}

void GraphicsEngine::setMenuStatus(MenuStatus i){
    ms = i;
}

MenuStatus GraphicsEngine::getMenuStatus(){
	return ms;
}

void GraphicsEngine::DrawAndPollMenu()
{
	int height, width;
	glfwGetWindowSize(m_window, &width, &height);

	glViewport(0, 0, width, height);
	//MenuState
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 identity;
	glDepthMask(GL_FALSE);
	m_textureShader->Use();
	glOrtho(0, 0, 0, 0, 0, 1);
	//renderScene(m_scene, &identity);
	if (ms == START){
		m_menu->setTextureId(m_menuId1);
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 2);
	}
	else if(ms == QUIT){
		m_menu->setTextureId(m_menuId2);
		glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 3);
	}
	else if (ms == MWINREPLAY){
		m_menu->setTextureId(m_menuId3);
		glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 4);
	}
	else if (ms == MWINQUIT){
		m_menu->setTextureId(m_menuId4);
		glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 5);
	}
	else if (ms == MLOSEREPLAY){
		m_menu->setTextureId(m_menuId5);
		glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 6);
	}
	else if (ms == MLOSEQUIT){
		m_menu->setTextureId(m_menuId6);
		glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 7);
	}
	
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	m_menu->render(&identity);
	glDepthMask(GL_TRUE);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}


/**
* GraphicsEngine::renderScene(Node*, glm::mat4*)
* Description: This function renders the scene graph using depth traversal.
*/
void GraphicsEngine::renderScene(Node* node, glm::mat4* matrix) {
	Geode *geode = node->asGeode();
	MatrixNode *mnode = node->asMatrixNode();

	if (geode) {
		// render geode
		//cout << glm::to_string(*matrix) << endl << endl;
		if (geode->getTex()){
			//m_tShader->Use();
			m_textureShader->Use();
			//cout << "ts" << endl;
		}
		else{
			m_defaultShader->Use();
		}
		geode->getRenderable()->render(matrix);
	}
	else if (mnode && mnode->getVisible()) {
		int numChildren = mnode->getNumChildren();
		glm::mat4 newmat = *matrix;
		mnode->postMult(newmat);
		for (int i = 0; i < numChildren; ++i) {
			renderScene(mnode->getChild(i), &newmat);
		}
	}
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
/*
		glDeleteProgram(m_shaderProgram);
		glDeleteShader(m_fragmentShader);
		glDeleteShader(m_vertexShader);*/

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

void GraphicsEngine::ScaleUp()
{
	if (m_player)
		m_player->getMatrix() = glm::scale(m_player->getMatrix(), glm::vec3(1.2, 1.2, 1.2));
}

void GraphicsEngine::ScaleDown()
{
	if (m_player)
		m_player->getMatrix() = glm::scale(m_player->getMatrix(), glm::vec3(0.8, 0.8, 0.8));
}


void GraphicsEngine::bindPlayerNode(GameObject* player) {
   Renderable * model = GraphicsEngine::selectModel(player->getModel());
   m_player = GraphicsEngine::addNode(model, true);
   m_player->addChild(m_mainCamera);
   m_player->addChild(m_minimapCamera);

   GraphicsEngine::insertObject(player->getId(), m_player);// (player->node = m_player;)
}


//Add node into scene graph using a model
MatrixNode* GraphicsEngine::addNode(Renderable* objModel, bool f){
	//Renderable* objModel = new Geometry(modelPath);// "../../media/pb.obj");
	Geode* objGeode = new Geode();
	objGeode->setRenderable(objModel);
	MatrixNode * m_node = new MatrixNode();
	m_node->addChild(objGeode);
	m_scene->addChild(m_node);
	m_node->setVisible(f);
	return m_node;
}

/*void GraphicsEngine::removeNode(MatrixNode * node) {
   node->
}*/

// Select blob model based on playerId, will be changed later
Renderable * GraphicsEngine::selectModel(Model model){
   m_defaultShader->Use();
   Renderable* newModel;
   std::string pathString = "../../media/models/" + ResourceMap::getObjFile(model);
   const char * path = pathString.c_str();
   newModel = new Geometry(path);
   return newModel;
}

// Translate from vec4 postion to matrix in the node of scene graph??
void GraphicsEngine::updateObject(ObjectId objId, glm::quat & q, float angle, float height, bool f) {
	bool old_visible = objNodeMap[objId]->getVisible();
	
	objNodeMap[objId]->getMatrix() = MatrixNode::quatAngle(q, angle, height);
	objNodeMap[objId]->setVisible(f);

	bool new_visible = objNodeMap[objId]->getVisible();

	//check for visible to invisible transition
	if (old_visible != new_visible)
		GameSound::nom->play(); //I play sound here because I want it to be client side only
}

//A mapping from ObjectId to node in scene graph
void GraphicsEngine::insertObject(ObjectId objId, MatrixNode* n) {

	auto found = objNodeMap.find(objId);
	if (found == objNodeMap.end()) {
		objNodeMap.insert(make_pair(objId, n));
	}
	else{
		//TODO exception duplicate node
	}
}

//A mapping from ObjectId to node in scene graph
void GraphicsEngine::removeObject(ObjectId objId) {
  objNodeMap.erase(objId);
}

void GraphicsEngine::setCursor(int state) {
	glfwSetInputMode(m_window, GLFW_CURSOR, state);
}

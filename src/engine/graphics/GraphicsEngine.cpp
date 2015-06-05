
#include "GraphicsEngine.h"

#include "..\graphics\Cube.h"
#include "..\graphics\Geometry.h"
#include "..\utility\System.h"
#include "..\utility\Config.h"
#include <gtc\constants.hpp>
#include "Shader.h"
#include "Skybox.h"
#include "LightHandler.h"
#include "HUD.h"
#include "..\utility\GameSound.h"
#include "Particle3D.h"
#include "Quad.h"

using namespace std;

// Graphics Engine Static Members
vector<MatrixNode*> GraphicsEngine::m_objects;
std::vector<ParticleSystem*> GraphicsEngine::m_psystems;
glm::mat4			GraphicsEngine::m_view, GraphicsEngine::m_projection;
glm::vec2           GraphicsEngine::m_screen_scale;
bool				GraphicsEngine::m_initialized = false;
GLFWwindow			*GraphicsEngine::m_window;

GLint				GraphicsEngine::m_uniView,
GraphicsEngine::m_uniProjection;

KeyCallback			GraphicsEngine::m_keyCallback = NULL;
MatrixNode			*GraphicsEngine::m_player = NULL,

*GraphicsEngine::m_scene = NULL;
Renderable			*GraphicsEngine::m_quad = NULL,
*GraphicsEngine::m_cube = NULL;
GLuint				GraphicsEngine::m_particleTex = 0;

CameraNode			*GraphicsEngine::m_mainCamera = NULL;
CameraNode			*GraphicsEngine::m_minimapCamera = NULL;

MenuStatus          GraphicsEngine::ms = LOADING;
GLuint				GraphicsEngine::m_skyboxId = 0;

GLuint				GraphicsEngine::m_HudId1 = 0;
GLuint				GraphicsEngine::m_HudId2 = 0;
GLuint				GraphicsEngine::m_HudId3 = 0;
GLuint				GraphicsEngine::m_HudId4 = 0;
GLuint				GraphicsEngine::m_HudIdN1 = 0;
GLuint				GraphicsEngine::m_HudIdN2 = 0;
GLuint				GraphicsEngine::m_HudIdN3 = 0;
GLuint				GraphicsEngine::m_HudIdN4 = 0;
GLuint				GraphicsEngine::m_HudIdN5 = 0;
GLuint				GraphicsEngine::m_HudIdN6 = 0;
GLuint				GraphicsEngine::m_HudIdN7 = 0;
GLuint				GraphicsEngine::m_HudIdN8 = 0;
GLuint				GraphicsEngine::m_HudIdN9 = 0;
GLuint				GraphicsEngine::m_HudIdN0 = 0;
GLuint				GraphicsEngine::m_HudIdPer = 0;
GLuint				GraphicsEngine::m_HudIdSpa = 0;
GLuint			    GraphicsEngine::m_HudIdCol = 0;

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
GLuint				GraphicsEngine::m_menuId7 = 0;
GLuint				GraphicsEngine::m_menuId8 = 0;
GLuint				GraphicsEngine::m_menuIdLoading = 0;

int					GraphicsEngine::HUDW = 100;
int					GraphicsEngine::HUDH = 100;
int                 GraphicsEngine::B = 20; //used to change the size of the mini map :)

//Percentage
int					GraphicsEngine::p1p = 0;
int					GraphicsEngine::p2p = 0;
int					GraphicsEngine::p3p = 0;
int					GraphicsEngine::p4p = 0;

Renderable			*GraphicsEngine::m_skybox = NULL;
Renderable			*GraphicsEngine::m_border = NULL;
Renderable			*GraphicsEngine::m_plus = NULL;
Renderable			*GraphicsEngine::m_minus = NULL;
Renderable			*GraphicsEngine::m_timer = NULL;
Renderable			*GraphicsEngine::m_timer1 = NULL;
Renderable			*GraphicsEngine::m_timer2 = NULL;
Renderable			*GraphicsEngine::m_timer3 = NULL;
Renderable			*GraphicsEngine::m_timer4 = NULL;
Renderable			*GraphicsEngine::m_timer5 = NULL;

Renderable			*GraphicsEngine::m_HUD1 = NULL;
Renderable			*GraphicsEngine::m_HUD2 = NULL;
Renderable			*GraphicsEngine::m_HUD3 = NULL;
Renderable			*GraphicsEngine::m_HUD4 = NULL;

Renderable			*GraphicsEngine::m_HUDN10 = NULL;
Renderable			*GraphicsEngine::m_HUDN11 = NULL;
Renderable			*GraphicsEngine::m_HUDN12 = NULL;
Renderable			*GraphicsEngine::m_HUDN13 = NULL;

Renderable			*GraphicsEngine::m_HUDN20 = NULL;
Renderable			*GraphicsEngine::m_HUDN21 = NULL;
Renderable			*GraphicsEngine::m_HUDN22 = NULL;
Renderable			*GraphicsEngine::m_HUDN23 = NULL;

Renderable			*GraphicsEngine::m_HUDN30 = NULL;
Renderable			*GraphicsEngine::m_HUDN31 = NULL;
Renderable			*GraphicsEngine::m_HUDN32 = NULL;
Renderable			*GraphicsEngine::m_HUDN33 = NULL;

Renderable			*GraphicsEngine::m_HUDN40 = NULL;
Renderable			*GraphicsEngine::m_HUDN41 = NULL;
Renderable			*GraphicsEngine::m_HUDN42 = NULL;
Renderable			*GraphicsEngine::m_HUDN43 = NULL;

Renderable			*GraphicsEngine::worldModel = NULL;
Renderable			*GraphicsEngine::m_menu = NULL;

Shader				*GraphicsEngine::m_defaultShader, *GraphicsEngine::m_skyboxShader, *GraphicsEngine::m_textureShader;

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

	glewExperimental = GL_TRUE;
	glewInit();

	m_defaultShader = new Shader("../engine/graphics/Shaders/default.vert", "../engine/graphics/Shaders/default.frag");
	m_skyboxShader = new Shader("../engine/graphics/Shaders/skybox.vert", "../engine/graphics/Shaders/skybox.frag");
	m_textureShader = new Shader("../engine/graphics/Shaders/texture2D.vert", "../engine/graphics/Shaders/texture2D.frag");

	// Turn on z-buffering
	glEnable(GL_DEPTH_TEST);

	// Menu
	m_textureShader->Use();
	m_menu = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.f);
	m_screen_scale = glm::vec2(2.0f, 2.0f);
	m_menuIdLoading = HUD::makeHUD("../../media/texture/start_bg.png");
	DrawAndPollMenu();

	// SKYBOX
	m_skyboxShader->Use();
	m_skybox = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.f);

	m_skyboxId = Skybox::makeSkybox("../../media/texture/skybox/");
	m_skybox->setIsSkybox(true);
	m_skybox->setTextureId(m_skyboxId);
	
	// HUD
    m_textureShader->Use();
	addHUD();//pass in current texture unit

	m_menuId1 = HUD::makeHUD("../../media/texture/bg_start.png");//start start  
	m_menuId2 = HUD::makeHUD("../../media/texture/bg_quit.png");//start quit   
	m_menuId3 = HUD::makeHUD("../../media/texture/win_replay.png");//win replay  
	m_menuId4 = HUD::makeHUD("../../media/texture/win_quit.png");//win quit    
	m_menuId5 = HUD::makeHUD("../../media/texture/lose_replay.png");//lose replay  
	m_menuId6 = HUD::makeHUD("../../media/texture/lose_quit.png");//lose quit  TODO bug  
	m_menuId7 = HUD::makeHUD("../../media/texture/bg_continue.png");//lose replay  
	m_menuId8 = HUD::makeHUD("../../media/texture/bg_pausequit.png");//lose quit  TODO bug 

	// WORLD
	//m_textureShader->Use();
	m_defaultShader->Use();
	worldModel = new Geometry("../../media/models/sphere_t2.obj");

	m_groundId = HUD::makeHUD("../../media/texture/ground12004.png");  
	worldModel->setTextureId(m_groundId);

	Geode* worldGeode = new Geode();
	worldGeode->setRenderable(worldModel);
	worldGeode->setTex(true);
	m_scene->addChild(worldGeode);

	// Particle quad
	m_quad = new Quad(0.5f, glm::vec3(1));
	m_cube = new Cube(glm::vec3(), glm::quat(), glm::vec3(1), 0.5f);
	m_particleTex = HUD::makeHUD("../../media/texture/particle0.png");

	// CAMERA
	glm::mat4 camview = glm::lookAt(
      glm::vec3(0.f, 25.f, 16.f),
		glm::vec3(0.f, 0.f, 7.f),
		glm::vec3(0.f, 0.f, 1.f));
	m_mainCamera = new CameraNode();
	m_mainCamera->setViewMatrix(camview);
	
	glm::mat4 minimapview = glm::translate(glm::mat4(), glm::vec3(0, 0, -550));
	minimapview = glm::rotate(minimapview, glm::radians(180.f), glm::vec3(0, 0, 1));
	m_minimapCamera = new CameraNode();
	m_minimapCamera->setViewMatrix(minimapview);
	m_minimapCamera->setAllowScaling(false);

	// LIGHTS
	m_sunLight = LightHandler::addLight(0, glm::vec3(-1, -1, -1), 1.f, glm::vec3(1, 1, 1), 0.f, glm::vec3(1, 1, 1), 0.5f, glm::vec3(1, 1, 1)); // direct light
	LightHandler::addLight(1, glm::vec3(0, 0, -505), 1.f, glm::vec3(1, 1, 1), 0.f, glm::vec3(0), 0.f, glm::vec3(0)); // point light

	if (glGetError() != 0) printf("Error Code: %d\n", glGetError());

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	ms = START;
	m_initialized = true;

}

void GraphicsEngine::RenderScore(int Player1, int Player2, int Player3, int Player4)
{
	//cout << Player1 << " " << Player2 << " " << Player3 << " " << Player4 << endl;

	int Hud11, Hud12, Hud21, Hud22, Hud31, Hud32, Hud41, Hud42 = 0;
	int Hud10 = 0;
	int Hud20 = 0;
	int Hud30 = 0;
	int Hud40 = 0;

	Hud10 = Player1 / 100;
	Hud11 = Player1 / 10;
	Hud12 = Player1 % 10;

	Hud20 = Player2 / 100;
	Hud21 = Player2 / 10;
	Hud22 = Player2 % 10;

	Hud30 = Player3 / 100;
	Hud31 = Player3 / 10;
	Hud32 = Player3 % 10;

	Hud40 = Player4 / 100;
	Hud41 = Player4 / 10;
	Hud42 = Player4 % 10;
	
	/*cout << "before" << endl;
	cout << Hud10 << Hud11 << Hud12 << endl;
	cout << Hud20 << Hud21 << Hud22 << endl;
	cout << Hud30 << Hud31 << Hud32 << endl;
	cout << Hud40 << Hud41 << Hud42 << endl;*/
	
	if (Hud41 == 0 && Hud40 == 0)
	{ 
		Hud41 = 10;
	}
	else if (Hud41 == 10 && Hud40 == 1){
		Hud41 = 0;
		cout << "HUD41" << Hud41 << endl;
	}

	if (Hud21 == 0 && Hud20 == 0)
	{
		Hud21 = 10;
	}
	else if (Hud21 == 10 && Hud20 == 1){
		Hud21 = 0;
		cout << "HUD21" << Hud21 << endl;
	}

	if (Hud31 == 0 && Hud30 == 0)
	{
		Hud31 = 10;
	}
	else if (Hud31 == 10 && Hud30 == 1){
		Hud31 = 0;
		cout << "HUD31" << Hud31 << endl;
	}

	if (Hud11 == 0 && Hud10 == 0)
	{
		Hud11 = 10;
	}
	else if (Hud11 == 10 && Hud10 == 1){
		Hud11 = 0;
		cout << "HUD11" << Hud11 << endl;
	}

	/*cout << "after" << endl;
	cout << Hud10 << Hud11 << Hud12 << endl;
	cout << Hud20 << Hud21 << Hud22 << endl;
	cout << Hud30 << Hud31 << Hud32 << endl;
	cout << Hud40 << Hud41 << Hud42 << endl;*/

	if (Hud40 == 0)
	{
		Hud40 = 10;
	}
	if (Hud20 == 0)
	{
		Hud20 = 10;
	}
	if (Hud30 == 0)
	{
		Hud30 = 10;
	}
	if (Hud10 == 0)
	{
		Hud10 = 10;
	}
	
	/*cout << "end" << endl;
	cout << Hud10 << Hud11 << Hud12 << endl;
	cout << Hud20 << Hud21 << Hud22 << endl;
	cout << Hud30 << Hud31 << Hud32 << endl;
	cout << Hud40 << Hud41 << Hud42 << endl;*/
	
	m_HUDN10->setTextureId(FindTexuture(Hud10));
	m_HUDN11->setTextureId(FindTexuture(Hud11));
	m_HUDN12->setTextureId(FindTexuture(Hud12));

	m_HUDN20->setTextureId(FindTexuture(Hud20));
	m_HUDN21->setTextureId(FindTexuture(Hud21));
	m_HUDN22->setTextureId(FindTexuture(Hud22));

	m_HUDN30->setTextureId(FindTexuture(Hud30));
	m_HUDN31->setTextureId(FindTexuture(Hud31));
	m_HUDN32->setTextureId(FindTexuture(Hud32));
	
	m_HUDN40->setTextureId(FindTexuture(Hud40));
	m_HUDN41->setTextureId(FindTexuture(Hud41));
	m_HUDN42->setTextureId(FindTexuture(Hud42));
}

//void GraphicsEngine::RenderTimer(){

//}

GLuint GraphicsEngine::FindTexuture(int id){

	switch (id){
	case 1: return m_HudIdN1;
		break;
	case 2: return m_HudIdN2;
		break;
	case 3: return m_HudIdN3;
		break;
	case 4: return m_HudIdN4;
		break;
	case 5: return m_HudIdN5;
		break;
	case 6: return m_HudIdN6;
		break;
	case 7: return m_HudIdN7;
		break;
	case 8: return m_HudIdN8;
		break;
	case 9: return m_HudIdN9;
		break;
	case 0: return m_HudIdN0;
		break;
	case 10: return m_HudIdSpa;
		break;
	case 11: return m_HudIdCol;
		break;

	}
}

void GraphicsEngine::addHUD(){
	m_HUD1 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId1 = HUD::makeHUD("../../media/texture/HUD1.png");
	m_HUD1->setTextureId(m_HudId1);

	m_HUD2 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId2 = HUD::makeHUD("../../media/texture/HUD2.png");
	m_HUD2->setTextureId(m_HudId2);

	m_HUD3 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId3 = HUD::makeHUD("../../media/texture/HUD3.png");
	m_HUD3->setTextureId(m_HudId3);

	m_HUD4 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HudId4 = HUD::makeHUD("../../media/texture/HUD4.png");
	m_HUD4->setTextureId(m_HudId4);

	//TODO replace pictures with right ones
	m_HudIdN1 = HUD::makeHUD("../../media/texture/1.png");
	m_HudIdN2 = HUD::makeHUD("../../media/texture/2.png");
	m_HudIdN3 = HUD::makeHUD("../../media/texture/3.png");
	m_HudIdN4 = HUD::makeHUD("../../media/texture/4.png");
	m_HudIdN5 = HUD::makeHUD("../../media/texture/5.png");
	m_HudIdN6 = HUD::makeHUD("../../media/texture/6.png");
	m_HudIdN7 = HUD::makeHUD("../../media/texture/7.png");
	m_HudIdN8 = HUD::makeHUD("../../media/texture/8.png");
	m_HudIdN9 = HUD::makeHUD("../../media/texture/9.png");
	m_HudIdN0 = HUD::makeHUD("../../media/texture/0.png");
	m_HudIdPer = HUD::makeHUD("../../media/texture/%.png");
	m_HudIdSpa = HUD::makeHUD("../../media/texture/space.png");
	m_HudIdCol = HUD::makeHUD("../../media/texture/col.png");

	m_HUDN10 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN10->setTextureId(m_HudIdSpa);
	m_HUDN11 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN11->setTextureId(m_HudIdSpa);
	m_HUDN12 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN12->setTextureId(m_HudIdN0);
	m_HUDN13 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN13->setTextureId(m_HudIdPer);

	m_HUDN20 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN20->setTextureId(m_HudIdSpa);
	m_HUDN21 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN21->setTextureId(m_HudIdSpa);
	m_HUDN22 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN22->setTextureId(m_HudIdN0);
	m_HUDN23 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN23->setTextureId(m_HudIdPer);

	m_HUDN30 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN30->setTextureId(m_HudIdSpa);
	m_HUDN31 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN31->setTextureId(m_HudIdSpa);
	m_HUDN32 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN32->setTextureId(m_HudIdN0);
	m_HUDN33 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN33->setTextureId(m_HudIdPer);

	m_HUDN40 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN40->setTextureId(m_HudIdSpa);
	m_HUDN41 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN41->setTextureId(m_HudIdSpa);
	m_HUDN42 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN42->setTextureId(m_HudIdN0);
	m_HUDN43 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_HUDN43->setTextureId(m_HudIdPer);

	/*
	//Mini map border
	/*m_border = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_borderId = HUD::makeHUD("../../media/texture/border.png");
	m_border->setTextureId(m_borderId);

    // Zoom in and out buttons
	m_plus = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_plusId = HUD::makeHUD("../../media/texture/plus.png");
	m_plus->setTextureId(m_plusId);

	m_minus = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_minusId = HUD::makeHUD("../../media/texture/minus.png");
	m_minus->setTextureId(m_minusId);
	*/
	// Timer
	/*m_timer = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_timerId = HUD::makeHUD("../../media/texture/timer.png");
	m_timer->setTextureId(m_timerId);*/

	//Timer digits
	m_timer1 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_timer1->setTextureId(m_HudIdN0);

	m_timer2 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_timer2->setTextureId(m_HudIdN0);

	m_timer3 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_timer3->setTextureId(m_HudIdCol);

	m_timer4 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_timer4->setTextureId(m_HudIdN0);

	m_timer5 = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.0f);
	m_timer5->setTextureId(m_HudIdN0);
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

	for (auto it = m_psystems.begin(); it != m_psystems.end(); ++it) {
		(*it)->simulate(glfwGetTime());
	}
	glfwSetTime(0.0); // reset for next frame

	MatrixData cameraData = m_mainCamera->getFlatViewMatrix();
	m_view = cameraData.flattened;
	glm::mat4 skybox_view = glm::scale(glm::mat4(glm::mat3(cameraData.flattened)), glm::vec3(1) / cameraData.scale); // try optimizing later

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

	//Update HUD 
	RenderScore(p1p, p2p, p3p, p4p); //p,,b,g,o
	//RenderTimer();

	// Update lights
	LightHandler::updateLighting(m_defaultShader->Id());
	glUniform1f(glGetUniformLocation(m_defaultShader->Id(), "hasTex"), 0);
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "view"), 1, GL_FALSE, glm::value_ptr(m_view));
	glUniform3fv(glGetUniformLocation(m_defaultShader->Id(), "camPos"), 1, glm::value_ptr(cameraData.translation));

	renderScene(m_scene, &identity);


	//Mini map
	//m_defaultShader->Use();
	glViewport(width - HUDW * 2 + B / 2, height - HUDW * 2 + B / 2, HUDW * 2 - B, HUDH * 2 - B);
	//glViewport(width - HUDW * 2 , height - HUDW * 2 , HUDW * 2, HUDH * 2);
	glClear(GL_DEPTH_BUFFER_BIT);

	//view = m_minimapCamera->getFlatViewMatrix();
	
	// change aspect ratio
	m_projection = glm::perspective(45.f, 1.f, 0.1f, 1000.f);

	// minimap
	cameraData = m_minimapCamera->getFlatViewMatrix();
	glUniform1f(glGetUniformLocation(m_defaultShader->Id(), "hasTex"), 0);
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "view"), 1, GL_FALSE, glm::value_ptr(cameraData.flattened));
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform3fv(glGetUniformLocation(m_defaultShader->Id(), "camPos"), 1, glm::value_ptr(cameraData.translation));

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 0);
	
	//HUD1
	glViewport(0, height - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD1->render(&identity);

	// HUD2
	glViewport(0, height - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD2->render(&identity);

	//HUD3
	glViewport(0, height - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD3->render(&identity);

	// HUD4
	glViewport(0, height - HUDH - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD4->render(&identity);

	//TODO: replace with numbers...
	//HUDN1
	float first = HUDW / 3;
	float second = HUDW / 3 + (HUDW / 3);
	float third = HUDW / 3 + (HUDW / 3) + (HUDW / 3);


	glViewport(0 + HUDW, height - HUDH, HUDW/3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN10->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH, HUDW/3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN11->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH, HUDW/3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN12->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH, HUDW/3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN13->render(&identity);

	// HUDN2
	glViewport(0 + HUDW, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN20->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN21->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN22->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN23->render(&identity);

	//HUDN3
	glViewport(0 + HUDW, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN30->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN31->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN32->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN33->render(&identity);

	// HUDN4
	glViewport(0 + HUDW, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN40->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN41->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN42->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN43->render(&identity);


	//HUD on top of minimap
	/*glViewport(width - HUDW * 2, height - HUDH * 2, HUDW * 2, HUDH * 2);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_border->render(&identity);*/

	// Zoom in 
	/*glViewport(width - HUDW * 2 - HUDW/2, height - HUDH + HUDW/2, HUDW/2, HUDH/2);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_plus->render(&identity);*/

	// Zoom out
	/*glViewport(width - HUDW * 2 - HUDW / 2, height - HUDH , HUDW / 2, HUDH / 2);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_minus->render(&identity);*/

	// Timer
	//glViewport(width - HUDW * 2 - HUDW / 2 - HUDW * 5/4, height - HUDH + HUDH / 4, HUDW * 5/4, HUDH * 3 / 4);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//m_timer->render(&identity);

	glViewport(width - HUDW * 2 - HUDW / 2 - first, height - HUDH , HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_timer1->render(&identity);

	glViewport(width - HUDW * 2 - HUDW / 2 - second, height - HUDH , HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_timer2->render(&identity);

	glViewport(width - HUDW * 2 - HUDW / 2 - third, height - HUDH , HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_timer3->render(&identity);

	glViewport(width - HUDW * 2 - HUDW / 2 - third-first, height - HUDH , HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_timer4->render(&identity);

	glViewport(width - HUDW * 2 - HUDW / 2 - third-second, height - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_timer5->render(&identity);

	glDisable(GL_BLEND);
}


void GraphicsEngine::renderBoard(int width, int height, glm::mat4 & identity){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glOrtho(0, 0, 0, 0, 0, 1);
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 0);

	//HUD1
	glViewport(0, height - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD1->render(&identity);

	// HUD2
	glViewport(0, height - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD2->render(&identity);

	//HUD3
	glViewport(0, height - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD3->render(&identity);

	// HUD4
	glViewport(0, height - HUDH - HUDH - HUDH - HUDH, HUDW, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUD4->render(&identity);

	//TODO: replace with numbers...
	//HUDN1
	float first = HUDW / 3;
	float second = HUDW / 3 + (HUDW / 3);
	float third = HUDW / 3 + (HUDW / 3) + (HUDW / 3);


	glViewport(0 + HUDW, height - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN10->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN11->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN12->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN13->render(&identity);

	// HUDN2
	glViewport(0 + HUDW, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN20->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN21->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN22->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN23->render(&identity);

	//HUDN3
	glViewport(0 + HUDW, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN30->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN31->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN32->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN33->render(&identity);

	// HUDN4
	glViewport(0 + HUDW, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN40->render(&identity);

	glViewport(0 + HUDW + first, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN41->render(&identity);

	glViewport(0 + HUDW + second, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN42->render(&identity);

	glViewport(0 + HUDW + third, height - HUDH - HUDH - HUDH - HUDH, HUDW / 3, HUDH);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_HUDN43->render(&identity);

	glDisable(GL_BLEND);
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
	glUniform2fv(glGetUniformLocation(m_textureShader->Id(), "scale"), 1, glm::value_ptr(m_screen_scale));

    switch (ms){
	case(START):
		m_menu->setTextureId(m_menuId1);
		break;
	case(QUIT):
		m_menu->setTextureId(m_menuId2);
		break;
	case(MWINREPLAY):
		m_menu->setTextureId(m_menuId3);
		break;
	case(MWINQUIT):
		m_menu->setTextureId(m_menuId4);
		break;
	case(MLOSEREPLAY) :
		m_menu->setTextureId(m_menuId5);
		break;
	case(MLOSEQUIT):
		m_menu->setTextureId(m_menuId6);
		break;
	case(MCONTINUE) :
		m_menu->setTextureId(m_menuId7);
		break;
	case(MPAUSEQUIT) :
		m_menu->setTextureId(m_menuId8);
		break;
	case(LOADING) :
		m_menu->setTextureId(m_menuIdLoading);
		break;
	}
	
	glUniform1i(glGetUniformLocation(m_textureShader->Id(), "tex"), 0);// m_menu->getTextureUnit());
	m_menu->render(&identity);

	if (ms == MWINREPLAY || ms == MWINQUIT || ms == MLOSEREPLAY || ms == MLOSEQUIT){
		renderBoard(width, height, identity);
	}


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
	ParticleSystem *psystem = node->asPSystem();

	if (geode) {
		// render geode
		m_defaultShader->Use();
		glUniform1i(glGetUniformLocation(m_defaultShader->Id(), "billboard"), 0);
		if (geode->getTex()){
			glUniform1f(glGetUniformLocation(m_defaultShader->Id(), "hasTex"), 1);
			glUniform1i(glGetUniformLocation(m_defaultShader->Id(), "tex"), 0);
		}
		else{
			glUniform1f(glGetUniformLocation(m_defaultShader->Id(), "hasTex"), 0);

		}
		geode->getRenderable()->render(matrix);
	}
	else if (psystem) {
		// render particles
		m_defaultShader->Use();
		psystem->render(*matrix, m_view, m_defaultShader->Id());
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

void GraphicsEngine::setMenuStatus(MenuStatus i){
	ms = i;
}

MenuStatus GraphicsEngine::getMenuStatus(){
	return ms;
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
void GraphicsEngine::updateObject(ObjectId objId, glm::quat & q, float angle, float height, float scale, bool vf, bool pf) {
	bool old_particle = objNodeMap[objId]->getParticle();
	
	objNodeMap[objId]->getMatrix() = MatrixNode::quatAngle(q, angle, height, scale);
	objNodeMap[objId]->setVisible(vf);
	objNodeMap[objId]->setParticle(pf);


	bool new_particle = objNodeMap[objId]->getParticle();

	//check for particle transition
	if (old_particle != new_particle)
		spawnPSystem(objNodeMap[objId]->getMatrix());

}

void GraphicsEngine::updatePercent(Model m, int p) {
	//cout << "p= " << p << endl;
	switch (m){
	case PB_TYPE:
		p1p = p;
		break;
	case BB_TYPE:
		p2p = p;
		break;
	case GB_TYPE:
		p3p = p;
		break;
	case OB_TYPE:
		p4p = p;
		break;
	}
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

void GraphicsEngine::spawnPSystem(glm::mat4 &matrix) {
	ParticleSystem* ps = new ParticleSystem((int) Random::linearRand(50.f, 100.f), m_quad, m_particleTex);
	MatrixNode* pm = new MatrixNode();
	pm->setMatrix(matrix);
	pm->addChild(ps);
	m_scene->addChild(pm);
	m_psystems.push_back(ps);
}

void GraphicsEngine::reverseCam(bool on) {
	if (on) {
		glm::mat4 camview = glm::lookAt(
			glm::vec3(0.f, -25.f, 16.f),
			glm::vec3(0.f, 0.f, 7.f),
			glm::vec3(0.f, 0.f, 1.f));
		m_mainCamera->setViewMatrix(camview);
	}
	else {
		glm::mat4 camview = glm::lookAt(
			glm::vec3(0.f, 25.f, 16.f),
			glm::vec3(0.f, 0.f, 7.f),
			glm::vec3(0.f, 0.f, 1.f));
		m_mainCamera->setViewMatrix(camview);
	}
}

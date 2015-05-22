//#include <Windows.h>

#include "GraphicsEngine.h"

//#include "..\utility\ObjectDB.h"
#include "..\graphics\Cube.h"
#include "..\graphics\Geometry.h"
#include "..\utility\System.h"
#include <gtc\constants.hpp>
#include "Shader.h"
#include "Skybox.h"

using namespace std;

// Graphics Engine Static Members
vector<MatrixNode*> GraphicsEngine::m_objects;
glm::mat4			GraphicsEngine::m_view, GraphicsEngine::m_projection;
bool				GraphicsEngine::m_initialized = false;
GLFWwindow			*GraphicsEngine::m_window;

GLint				GraphicsEngine::m_uniView,
GraphicsEngine::m_uniProjection;

KeyCallback			GraphicsEngine::m_keyCallback = NULL;
MatrixNode			*GraphicsEngine::m_player = NULL,

*GraphicsEngine::m_scene = NULL;

CameraNode			*GraphicsEngine::m_mainCamera = NULL;
CameraNode			*GraphicsEngine::m_minimapCamera = NULL;

GLuint				GraphicsEngine::m_skyboxId = 0;
Renderable			*GraphicsEngine::m_skybox = NULL;
Shader				*GraphicsEngine::m_defaultShader, *GraphicsEngine::m_skyboxShader;


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
	glfwSetCursorPos(window, 0, 0);
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
	m_window = glfwCreateWindow(800, 800, "CSE 125", NULL, NULL);
	glfwSetKeyCallback(m_window, key_callback);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	// mouse handling
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, cursor_callback);
	glfwSetCursorPos(m_window, 0, 0);

	glewExperimental = GL_TRUE;
	glewInit();

	m_defaultShader = new Shader("../engine/graphics/Shaders/default.vert", "../engine/graphics/Shaders/default.frag");
	m_skyboxShader = new Shader("../engine/graphics/Shaders/skybox.vert", "../engine/graphics/Shaders/skybox.frag");

	// Turn on z-buffering
	glEnable(GL_DEPTH_TEST);


	// SKYBOX
	m_skyboxShader->Use();
	m_skybox = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 0.f, 0.f), 1.f);
	m_skyboxId = Skybox::makeSkybox("../../media/texture/skybox/");
	m_skybox->setTextureId(m_skyboxId);

	// WORLD
	m_defaultShader->Use();
	Renderable* worldModel = new Geometry("../../media/models/sphere.obj");
	Geode* worldGeode = new Geode();
	worldGeode->setRenderable(worldModel);
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

	// PLAYER  (Player node is created by default)

	/*Renderable * model = GraphicsEngine::selectModel(playerId);
	m_player = GraphicsEngine::addNode(model);
	m_player->addChild(m_mainCamera);
	m_player->addChild(m_minimapCamera);*/
	

	if (glGetError() != 0) printf("Error Code: %d\n", glGetError());

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

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

	glViewport(0, 0, width, height);
	m_projection = glm::perspective(
		45.f,
		((float)height) / width,
		0.1f, 1000.f);

	glm::mat4 view = m_mainCamera->getFlatViewMatrix();
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
	GLint light = glGetUniformLocation(m_defaultShader->Id(), "pointLight");
	const float radius = 2.f;
	float sine = radius*glm::sin(glm::radians(90 * glfwGetTime()));
	float cosine = radius*glm::cos(glm::radians(90 * glfwGetTime()));
	glm::vec3 lightpos(cosine, sine, 1);
	glUniform3fv(light, 1, glm::value_ptr(lightpos));

	GLint dirLight = glGetUniformLocation(m_defaultShader->Id(), "dirLight");
	glm::vec3 dirLightVec(-1, -1, -1);
	glUniform3fv(dirLight, 1, glm::value_ptr(dirLightVec));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "view"), 1, GL_FALSE, glm::value_ptr(view));

	renderScene(m_scene, &identity);
	glViewport(width -200, height-200,200, 200);

	glClear( GL_DEPTH_BUFFER_BIT);

	//view = m_minimapCamera->getFlatViewMatrix();

	// minimap
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "view"), 1, GL_FALSE, glm::value_ptr(m_minimapCamera->getFlatViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShader->Id(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

	renderScene(m_scene, &identity);
	glEnable(GL_DEPTH_TEST);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}


void GraphicsEngine::DrawAndPollMenu()
{
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
   Renderable* newModel;
   std::string pathString = "../../media/models/" + ResourceMap::getObjFile(model);
   const char * path = pathString.c_str();
   newModel = new Geometry(path);
	return newModel;
}

// Translate from vec4 postion to matrix in the node of scene graph??
void GraphicsEngine::updateObject(ObjectId objId, glm::quat & q, float angle, float height, bool f) {
	objNodeMap[objId]->getMatrix() = MatrixNode::quatAngle(q, angle, height);
	objNodeMap[objId]->setVisible(f);
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

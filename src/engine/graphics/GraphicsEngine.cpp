//#include <Windows.h>

#include "GraphicsEngine.h"

#include "..\utility\InputHandler.h"

//#include "..\utility\ObjectDB.h"
#include "..\graphics\Cube.h"
#include "..\graphics\Geometry.h"
#include "..\utility\System.h"
#include "..\utility\Model.h"
using namespace std;

// Graphics Engine Static Members
vector<MatrixNode*> GraphicsEngine::m_objects;
glm::mat4			GraphicsEngine::m_view, GraphicsEngine::m_projection;
bool				GraphicsEngine::m_initialized = false;
GLFWwindow			*GraphicsEngine::m_window;

GLint				GraphicsEngine::m_uniView,
GraphicsEngine::m_uniProjection;

GLuint				GraphicsEngine::m_vertexShader,
GraphicsEngine::m_fragmentShader,
GraphicsEngine::m_shaderProgram;

KeyCallback			GraphicsEngine::m_keyCallback = NULL;

MatrixNode			*GraphicsEngine::m_player = NULL,
*GraphicsEngine::m_scene = NULL;

CameraNode			*GraphicsEngine::m_mainCamera = NULL;
unordered_map<ObjectId, MatrixNode*> GraphicsEngine::objNodeMap;

string version = "#version 150\n";

/**
* Description: This function is called when glfwPollEvents() is called.
*/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (GraphicsEngine::GetKeyCallback()) GraphicsEngine::GetKeyCallback()(key, action, mods);
	InputHandler::handleKey(key, action, mods);
}

/**
* GraphicsEngine::Initialize()
* Description: This function initializes the graphics pipeline, compiles the
* shaders, creates the window, and sets up the view and projection matrices
*/
void GraphicsEngine::Initialize(ObjectId playerId) {
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
	Renderable* cube = new Cube(glm::vec3(), glm::quat(), glm::vec3(1.f, 1.f, 1.f), 1.f);
	for (int i = 0; i < CUBE_COUNT; ++i) {
		glm::vec3 position(-2.f + 0.4f*(i % 10), -2.f + 0.4f*(i / 10), 0.1f);
		Geode* cubeGeode = new Geode();
		cubeGeode->setRenderable(cube);
		m_objects.push_back(new MatrixNode());
		m_objects[i]->addChild(cubeGeode);
		glm::mat4 cubeMat;
		cubeMat = glm::translate(cubeMat, position);
		cubeMat = glm::rotate(cubeMat, glm::radians((float)i), glm::vec3(0, 0, 1));
		float scale = 0.02f + 0.08f * (i / (float)100);
		cubeMat = glm::scale(cubeMat, glm::vec3(scale, scale, scale));
		m_objects[i]->setMatrix(cubeMat);
		m_scene->addChild(m_objects[i]);
		//m_objects.push_back(new Cube(position, glm::angleAxis(glm::radians((float)i), glm::vec3(0, 0, 1)), glm::vec3(1.f, 1.f, 1.f), 0.02f + 0.08f * (i / (float)100)));
	}

	// WORLD
	Renderable* worldModel = new Geometry("../../media/sphere.obj");
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

	// PLAYER  (Player node is created by default)
	Renderable * model = GraphicsEngine::selectModel(playerId);
	m_player = GraphicsEngine::addNode(model);
	m_player->addChild(m_mainCamera);

	// view and projection matrix locations in the shader program
	m_uniView = glGetUniformLocation(m_shaderProgram, "view");
	m_uniProjection = glGetUniformLocation(m_shaderProgram, "projection");

	m_view = glm::lookAt(
		glm::vec3(0.f, 6.f, 4.f),
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

	glm::mat4 view = m_mainCamera->getFlatViewMatrix();


	glUniformMatrix4fv(m_uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(m_uniProjection, 1, GL_FALSE, glm::value_ptr(m_projection));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLint light = glGetUniformLocation(m_shaderProgram, "pointLight");
	const float radius = 2.f;
	float sine = radius*glm::sin(glm::radians(90 * glfwGetTime()));
	float cosine = radius*glm::cos(glm::radians(90 * glfwGetTime()));
	glm::vec3 lightpos(cosine, sine, 1);
	glUniform3fv(light, 1, glm::value_ptr(lightpos));

	GLint dirLight = glGetUniformLocation(m_shaderProgram, "dirLight");
	glm::vec3 dirLightVec(-1, -1, -1);
	glUniform3fv(dirLight, 1, glm::value_ptr(dirLightVec));



	glm::mat4 identity;
	renderScene(m_scene, &identity);

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
		geode->getRenderable()->render(matrix);
	}
	else if (mnode) {
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
    GraphicsEngine::insertObject(player->getId(), m_player);// (player->node = m_player;)
}


//Add node into scene graph using a model
MatrixNode* GraphicsEngine::addNode(Renderable* objModel){
	//Renderable* objModel = new Geometry(modelPath);// "../../media/pb.obj");
	Geode* objGeode = new Geode();
	objGeode->setRenderable(objModel);
	MatrixNode * m_node = new MatrixNode();
	m_node->addChild(objGeode);
	m_scene->addChild(m_node);
	return m_node;
		}

// Select blob model based on playerId, will be changed later
Renderable * GraphicsEngine::selectModel(ObjectId playerId){
	Renderable* newModel;

	switch (playerId % 3){
	case 0:
		newModel = new Geometry("../../media/bb.obj");
		break;
	case 1:
		newModel = new Geometry("../../media/gb.obj");
		break;
	case 2:
		newModel = new Geometry("../../media/pb.obj");
		break;
	default:
		newModel = new Geometry("../../media/bb.obj");
		break;
	}
	return newModel;
}

// Select blob model based on playerId, will be changed later
Renderable * GraphicsEngine::selectModel(Model model){
   Renderable* newModel;
   std::string pathString = "../../media/" + ResourceMap::getObjFile(model);
   const char * path = pathString.c_str();
   newModel = new Geometry(path);
   return newModel;
}

// Translate from vec4 postion to matrix in the node of scene graph??
void GraphicsEngine::updateObject(ObjectId objId, glm::vec4 & v) {
	objNodeMap[objId]->getMatrix() = MatrixNode::sphere2xyz(v);
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

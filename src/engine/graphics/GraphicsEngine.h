#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H
// STL
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#define GLEW_STATIC
#include <glew.h>
#include <glm.hpp>
#include <glfw3.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\string_cast.hpp>

#include <vector>
#include <deque>
#include "MatrixNode.h"
#include "Geode.h"
#include "CameraNode.h"
#include "Renderable.h"
#include "Shader.h"

#include "..\network\Packet.h"
#include "..\utility\GameObject.h"
#include "..\utility\GameState.h"


typedef unsigned int ObjectId;

using namespace std;

typedef void (*KeyCallback) (int,int,int);

class GraphicsEngine {
public:
	static void Initialize(ObjectId playerId);
	static bool Closing();
	static void CloseGame();
	static void DrawAndPoll();
	static void Destroy();
	static void SetKeyCallback(KeyCallback);
	static void MoveUp();
	static void MoveLeft();
	static void MoveDown();
	static void MoveRight();
	static void ScaleUp();
	static void ScaleDown();
	static KeyCallback GetKeyCallback();
	static void UpdatePlayer(deque<Packet> &, GameState &);
   static int getKeyState(int);

   static void bindPlayerNode(GameObject* player);
	static void updateObject(ObjectId objId, glm::vec4 & v);
	static void insertObject(ObjectId objId, MatrixNode*);

	static MatrixNode* addNode(Renderable*);
   static Renderable* selectModel(Model model);

private:
	static glm::mat4				m_view, m_projection;
	static bool						m_initialized;
	static GLFWwindow				*m_window;
	static GLint					m_uniView, m_uniProjection;
	static std::vector<MatrixNode*> m_objects;
	static KeyCallback				m_keyCallback;
	static MatrixNode				*m_player;
	static CameraNode				*m_mainCamera;
	static CameraNode				*m_minimapCamera;
	static MatrixNode				*m_scene;

	static GLuint					m_skyboxId;
	static Renderable				*m_skybox;
	static Shader					*m_defaultShader, *m_skyboxShader;


	static unordered_map<ObjectId, MatrixNode*> objNodeMap;
	
	static void renderScene(Node*, glm::mat4*);
};

#endif
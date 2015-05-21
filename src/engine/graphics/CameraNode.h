#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include "Node.h"
#include "MatrixNode.h"
#include "Geode.h"

#include <iostream>
#include <utility>

using namespace std;

class CameraNode : public Node {
private:
	glm::mat4 cam_matrix;
	glm::mat4 cam_revert;

public:
	CameraNode(glm::mat4 & m) : cam_matrix(m) {}
	CameraNode() {}

	virtual MatrixNode* asMatrixNode() {
		return 0;
	}
	virtual Geode* asGeode() {
		return 0;
	}
	virtual CameraNode* asCamera() {
		return this;
	}
	virtual std::string getName() {
		return "CameraNode";
	}
	std::pair<glm::mat4, glm::vec3> getFlatViewMatrix() {
		glm::mat4 flat_view = cam_revert;
		MatrixNode* curr = (m_parent == 0) ? 0 : m_parent->asMatrixNode();
		while (curr != 0) {
			curr->preMult(flat_view);
			curr = (curr->getParent() == 0) ? 0 : curr->getParent()->asMatrixNode();
		}
		glm::vec3 pos(flat_view[3].x, flat_view[3].y, flat_view[3].z);
		flat_view = glm::inverse(flat_view);
		return make_pair(flat_view, pos);
	}
	void setViewMatrix(glm::mat4 & matrix) {
		cam_matrix = matrix;
		cam_revert = glm::inverse(matrix);
	}
};
#endif
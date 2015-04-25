#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include "Node.h"
#include "MatrixNode.h"
#include "Geode.h"

#include <iostream>

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
	glm::mat4 getFlatViewMatrix() {
		glm::mat4 flat_view = cam_revert;
		MatrixNode* curr = (m_parent == 0) ? 0 : m_parent->asMatrixNode();
		while (curr != 0) {
			curr->preMult(flat_view);
			curr = (curr->getParent() == 0) ? 0 : curr->getParent()->asMatrixNode();
		}
		flat_view = glm::inverse(flat_view);
		return flat_view;
	}
	void setViewMatrix(glm::mat4 & matrix) {
		cam_matrix = matrix;
		cam_revert = glm::inverse(matrix);
	}
};
#endif
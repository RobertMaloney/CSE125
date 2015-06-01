#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include "Node.h"
#include "MatrixNode.h"
#include "Geode.h"

#include <iostream>
#include <utility>
#include <gtx\matrix_decompose.hpp>

using namespace std;

class CameraNode : public Node {
private:
	glm::mat4 cam_matrix;
	glm::mat4 cam_revert;
	bool allowScaling;

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
	MatrixData getFlatViewMatrix() {
		MatrixData ret;
		glm::mat4 flat_view = cam_revert;
		MatrixNode* curr = (m_parent == 0) ? 0 : m_parent->asMatrixNode();
		MatrixData md;
		while (curr != 0) {
			if (!allowScaling) {
				md.flattened = curr->getMatrix();
				glm::decompose(md.flattened, md.scale, md.orientation, md.translation, md.skew, md.perspective);
				flat_view = glm::scale(md.flattened, glm::vec3(1) / md.scale) * flat_view;
			}
			else {
				curr->preMult(flat_view);
			}
			curr = (curr->getParent() == 0) ? 0 : curr->getParent()->asMatrixNode();
		}
		ret.flattened = glm::inverse(flat_view);
		glm::decompose(ret.flattened, ret.scale, ret.orientation, ret.translation, ret.skew, ret.perspective);
		return ret;
	}
	void setViewMatrix(glm::mat4 & matrix) {
		cam_matrix = matrix;
		cam_revert = glm::inverse(matrix);
	}
	void setAllowScaling(bool s) {
		this->allowScaling = s;
	}
};
#endif
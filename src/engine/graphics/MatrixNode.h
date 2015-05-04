#ifndef MATRIX_NODE_H
#define MATRIX_NODE_H

#include "Node.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\quaternion.hpp>
#include <vector>
#include <iostream>

class MatrixNode : public Node {
private:
	glm::mat4 m_matrix;
	std::vector<Node*> m_children;

public:
	MatrixNode() {
		m_parent = 0;
	}

	virtual MatrixNode* asMatrixNode() {
		return this;
	}
	virtual Geode* asGeode() {
		return 0;
	}
	virtual CameraNode* asCamera() {
		return 0;
	}
	virtual std::string getName() {
		return "MatrixNode";
	}
	void setMatrix(glm::mat4 & matrix) {
		m_matrix = matrix;
	}
	glm::mat4 & getMatrix() {
		return m_matrix;
	}
	void postMult(glm::mat4 & matrix) {
		matrix = matrix * m_matrix;
	}
	void preMult(glm::mat4 & matrix) {
		matrix = m_matrix * matrix;
	}
	int getNumChildren() {
		return m_children.size();
	}
	Node* getChild(int index) {
		return m_children[index];
	}
	void addChild(Node* child) {
		m_children.push_back(child);
			child->setParent(this);
	}

	static glm::mat4 sphere2xyz(glm::vec4 & spherePos) {
		// get position on world
		glm::vec3 xyz(0, 0, spherePos.x);
		xyz = glm::angleAxis(glm::radians(spherePos.y), glm::vec3(0, 1, 0)) * xyz;
		xyz = glm::angleAxis(glm::radians(spherePos.z), glm::vec3(1, 0, 0)) * xyz;

		// make matrix out of position, add rotations
		glm::mat4 matrix = glm::translate(glm::mat4(), xyz);
		matrix = glm::rotate(matrix, glm::radians(spherePos.y), glm::vec3(0, 1, 0));
		matrix = glm::rotate(matrix, glm::radians(spherePos.z), glm::vec3(1, 0, 0));
		// do normal rotation last
		matrix = glm::rotate(matrix, glm::radians(spherePos.w), glm::vec3(0, 0, 1));

		return matrix;
	}
};
#endif
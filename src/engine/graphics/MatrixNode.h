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
		static glm::quat squat;
		/*
		static glm::mat4 matrix = glm::translate(glm::mat4(), glm::vec3(0, 0, spherePos.x));
		const float angle = 45.f;
		float cos_a = glm::cos(glm::radians(angle));
		float sin_a = glm::sin(glm::radians(angle));
		glm::vec3 direction(sin_a, cos_a, 0);
		glm::vec3 perp(-cos_a, sin_a, 0);
		glm::quat vRot = glm::angleAxis(0.0001f, perp);
		//glm::quat hRot = glm::angleAxis(0.f, direction);
		glm::quat mq = glm::quat_cast(matrix);
		mq = mq * vRot;
		*/
		float cosa = glm::cos(glm::radians(45.f));
		float sina = glm::sin(glm::radians(45.f));
		squat *= glm::angleAxis(0.0001f, glm::vec3(cosa, sina, 0));
		glm::vec3 pos = squat * glm::vec3(0, 0, spherePos.x);

		return glm::translate(glm::mat4(), pos) * glm::toMat4(squat);
		/*
		// get position on world
		glm::vec3 xyz(0, 0, spherePos.x);
		//glm::quat rot = glm::angleAxis(glm::radians(spherePos.y), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(spherePos.z), glm::vec3(1, 0, 0));
		glm::quat rot(glm::vec3(glm::radians(spherePos.z), glm::radians(spherePos.y), 0));
		xyz = rot * xyz;

		// make matrix out of position, add rotations
		glm::mat4 matrix = glm::translate(glm::mat4(), xyz);
		rot = glm::rotation(glm::vec3(0, 0, 1), glm::normalize(xyz));
		matrix = glm::rotate(matrix, glm::angle(rot), glm::axis(rot));
		// do normal rotation last
		matrix = glm::rotate(matrix, glm::radians(spherePos.w), glm::vec3(0, 0, 1));

		return matrix;
		*/
	}

	static glm::mat4 quatAngle(glm::quat & q, float angle) {
		glm::vec3 pos = q * glm::vec3(0, 0, 505.f);
		glm::quat rot = q * glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1));
		return glm::translate(glm::mat4(), pos) * glm::toMat4(rot);
	}
};
#endif
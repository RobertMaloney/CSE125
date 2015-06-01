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
	bool visible;

public:
	MatrixNode(){
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

	bool getVisible(){
		return visible;
	}
	void setVisible(bool f){
		visible = f;
	}

	static glm::mat4 quatAngle(glm::quat & q, float angle, float height, float scale) {
		glm::vec3 pos = q * glm::vec3(0, 0, height);
		glm::quat rot = q * glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1));
		glm::mat4 m = glm::translate(glm::mat4(), pos) * glm::toMat4(rot);
		return glm::scale(m, glm::vec3(scale, scale, scale));
	}


};
#endif
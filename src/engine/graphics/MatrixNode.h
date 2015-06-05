#ifndef MATRIX_NODE_H
#define MATRIX_NODE_H

#include "Node.h"
#include "PType.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\quaternion.hpp>
#include <vector>
#include <iostream>

struct MatrixData {
	glm::mat4 flattened;
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
};

class MatrixNode : public Node {
private:
	glm::mat4 m_matrix;
	std::vector<Node*> m_children;
	bool visible;
	bool particle;
	MatrixNode *mps;
	ParticleSystem *ps;
	PType p;

public:

	MatrixNode(){
		m_parent = 0;
		hasParticle = false;
		p = PType::P_RES;
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
	virtual ParticleSystem* asPSystem() {
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

	bool getParticle(){
		return particle;
	}
	void setParticle(bool f){
		particle = f;
	}

	MatrixNode* getParticleNode(){
		return mps;
	}
	void setParticleNode(MatrixNode *p){
		mps = p;
	}

	ParticleSystem* getParticleSystem(){
		return ps;
	}
	void setParticleSystem(ParticleSystem *p){
		ps = p;
	}

	static glm::mat4 quatAngle(glm::quat & q, float angle, float height, float scale) {
		glm::vec3 pos = q * glm::vec3(0, 0, height);
		glm::quat rot = q * glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1));
		glm::mat4 m = glm::translate(glm::mat4(), pos) * glm::toMat4(rot);
		return glm::scale(m, glm::vec3(scale, scale, scale));
	}

	PType getPType(){
		return p;
	}
	void setPType(PType t){
		p = t;
	}

};
#endif
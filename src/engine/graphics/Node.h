#ifndef NODE_H
#define NODE_H

#include <string>

class MatrixNode;
class Geode;
class CameraNode;
class ParticleSystem;

class Node {
protected:
	Node* m_parent;
	bool withTexture = false;

public:
	bool hasParticle;
	virtual MatrixNode* asMatrixNode() = 0;
	virtual Geode* asGeode() = 0;
	virtual CameraNode* asCamera() = 0;
	virtual ParticleSystem* asPSystem() = 0;
	virtual std::string getName() = 0;
	void setParent(Node* node) {
		m_parent = node;
	}
	Node* getParent() {
		return m_parent;
	}

	bool getTex(){
		return withTexture;
	}

	void setTex(bool t){
		withTexture = t;
	}
};
#endif
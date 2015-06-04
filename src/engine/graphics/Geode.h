#ifndef GEODE_H
#define GEODE_H

#include "Node.h"
#include "Renderable.h"

class Geode : public Node {
private:
	Renderable *m_renderable;

public:

	virtual MatrixNode* asMatrixNode() {
		return 0;
	}
	virtual Geode* asGeode() {
		return this;
	}
	virtual CameraNode* asCamera() {
		return 0;
	}
	virtual ParticleSystem* asPSystem() {
		return 0;
	}
	virtual std::string getName() {
		return "Geode";
	}
	Renderable* getRenderable() {
		return m_renderable;
	}
	void setRenderable(Renderable* render) {
		m_renderable = render;
	}
};
#endif
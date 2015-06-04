#ifndef QUAD_H
#define QUAD_H

#include "Renderable.h"

using namespace std;

class Quad : public Renderable {
public:
	Quad(float halfLength, glm::vec3 color) {
		GLfloat m_vertexData[] = {
			halfLength, halfLength, 0.f, 0.f, 0.f, 1.f, color[0], color[1], color[2], 1.f, 1.f,
			halfLength, -halfLength, 0.f, 0.f, 0.f, 1.f, color[0], color[1], color[2], 1.f, 0.f,
			-halfLength, -halfLength, 0.f, 0.f, 0.f, 1.f, color[0], color[1], color[2], 0.f, 0.f,

			-halfLength, -halfLength, 0.f, 0.f, 0.f, 1.f, color[0], color[1], color[2], 0.f, 0.f,
			-halfLength, halfLength, 0.f, 0.f, 0.f, 1.f, color[0], color[1], color[2], 0.f, 1.f,
			halfLength, halfLength, 0.f, 0.f, 0.f, 1.f, color[0], color[1], color[2], 1.f, 1.f
		};

		bufferObject(GL_TRIANGLES, m_vertexData, sizeof(m_vertexData) / sizeof(GLfloat));
	}
};
#endif
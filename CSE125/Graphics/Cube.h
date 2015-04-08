#ifndef CUBE_H
#define CUBE_H

#include <iostream>

#include "Renderable.h"

using namespace std;

class Cube : public Renderable {
public:
	Cube(glm::vec3 position, glm::quat rotation, glm::vec3 color, GLfloat halfLength) {
		m_matrix = glm::translate(m_matrix, position);
		m_matrix *= glm::mat4_cast(rotation);

		GLfloat m_vertexData[] = {
			halfLength, halfLength, halfLength, color[0], color[1], color[2],
			halfLength, -halfLength, halfLength, color[0], color[1], color[2],
			-halfLength, -halfLength, halfLength, color[0], color[1], color[2],
			-halfLength, halfLength, halfLength, color[0], color[1], color[2],

			halfLength, halfLength, -halfLength, color[0], color[1], color[2],
			halfLength, -halfLength, -halfLength, color[0], color[1], color[2],
			-halfLength, -halfLength, -halfLength, color[0], color[1], color[2],
			-halfLength, halfLength, -halfLength, color[0], color[1], color[2]
		};

		GLuint m_elements[] = {
			// top
			0, 1, 2,
			2, 3, 0,
			// bottom
			4, 5, 6,
			6, 7, 4,
			// left
			2, 3, 7,
			7, 6, 2,
			// right
			0, 1, 5,
			5, 4, 0,
			// back
			0, 3, 7,
			7, 4, 0,
			// front
			1, 2, 6,
			6, 5, 1
		};

		bufferObject(GL_TRIANGLES, m_vertexData, sizeof(m_vertexData) / sizeof(GLfloat), m_elements, sizeof(m_elements) / sizeof(GLuint));
	}
};
#endif
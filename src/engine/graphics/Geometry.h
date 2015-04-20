#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

#include "Renderable.h"
#include "..\Utility\ObjLoader.h"

using namespace std;

class Geometry : public Renderable {
public:
	Geometry(const char * filePath) {
		//m_matrix = glm::scale(m_matrix, glm::vec3(0.01f,0.01f,0.01f));

		vector<GLfloat> gl_data;
		ObjLoader::LoadObj(filePath, gl_data);

		if (gl_data.size() > 0)
			bufferObject(GL_TRIANGLES, &gl_data[0], gl_data.size());
	}
};
#endif
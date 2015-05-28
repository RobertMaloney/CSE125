#ifndef GROUND_H
#define GROUND_H

#include "Renderable.h"
//#include "TextureLoad.h"
#include "Node.h"
#include "MatrixNode.h"
#include <string>
#include <glew.h>
#include <SOIL.h>


class Ground {
public:
	static GLuint makeGround(std::string file, int i) {
		//glActiveTexture(GL_TEXTURE0 + i);
		GLuint image;
		glGenTextures(1, &image);
		glActiveTexture(GL_TEXTURE0 + i);
		image = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, image);

		return image;
	}
};
#endif
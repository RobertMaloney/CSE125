#ifndef HUD_H
#define HUD_H

#include "Renderable.h"
//#include "TextureLoad.h"
#include "Node.h"
#include "MatrixNode.h"
#include <string>
#include <glew.h>
#include <SOIL.h>

//std::string files[] = { "one", "two", "three", "four", "five", "six","seven","eight","nine","zero" };

class HUD {
public:
	static GLuint makeHUD(std::string file) {
		GLuint cubeId;
		//GLuint HUD_NAME = 1;
		//const std::string extension = ".png";

		glGenTextures(1, &cubeId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeId);

		int width, height;
		unsigned char* image;
		image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);


		glTexImage2D(
			GL_TEXTURE_2D,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		return cubeId;
	}
};
#endif
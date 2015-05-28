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
	static GLuint makeHUD(std::string file, int i) {
		//glActiveTexture(GL_TEXTURE0 + i);
		GLuint image = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);



		//glBindTexture(GL_TEXTURE_2D, image);

		return image;
	}
};
#endif
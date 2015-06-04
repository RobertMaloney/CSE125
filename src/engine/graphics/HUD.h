#ifndef HUD_H
#define HUD_H

#include <string>
#include <glew.h>
#include <SOIL.h>

class HUD {
public:  //Texture loader ......
	static GLuint makeHUD(std::string file) {
		GLuint tex_2d = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (0 == tex_2d)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
		return tex_2d;
	}
};
#endif
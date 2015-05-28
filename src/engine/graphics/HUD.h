#ifndef HUD_H
#define HUD_H

#include <string>
#include <glew.h>
#include <SOIL.h>

class HUD {
public:  //Texture loader ......
	static GLuint makeHUD(std::string file) {
	    return SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	}
};
#endif
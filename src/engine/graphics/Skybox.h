#ifndef SKYBOX_H
#define SKYBOX_H

#include "Renderable.h"
//#include "TextureLoad.h"
#include "Node.h"
#include "MatrixNode.h"
#include <string>
#include <glew.h>
#include <SOIL.h>

std::string files[] = { "skybox_right", "skybox_left", "skybox_top", "skybox_bottom", "skybox_back", "skybox_front" };

class Skybox {
public:
	static GLuint makeSkybox(std::string folder, int i) {
		GLuint cubeId;
		//GLuint SKYBOX_NAME = 0;
		const std::string extension = ".png";
		
		glGenTextures(1, &cubeId);
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeId);

		int width, height;
		unsigned char* image;
		for (GLuint i = 0; i < 6; ++i) {
			std::string path = folder + files[i] + extension;
			//std::cout << "Looking for " << path << std::endl;
			image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			//std::cout << "Loaded image (" << width << ", " << height << ")\n";
			//std::cout << SOIL_last_result() << std::endl;

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
				);
			SOIL_free_image_data(image);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		
		return cubeId;
	}
};
#endif
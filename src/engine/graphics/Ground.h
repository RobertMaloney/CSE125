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
	static GLuint makeGround(std::string folder) {
		GLuint cubeId;
		const std::string extension = ".png";

		glGenTextures(1, &cubeId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeId);

		int width, height;
		unsigned char* image;

		std::string path = folder + "ground" + extension;
		//std::cout << "Looking for " << path << std::endl;
		image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		//std::cout << "Loaded image (" << width << ", " << height << ")\n";
		//std::cout << SOIL_last_result() << std::endl;

		glTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
		SOIL_free_image_data(image);
	

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);//cubeId

		return cubeId;
	}
};
#endif
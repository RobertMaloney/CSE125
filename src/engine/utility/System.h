#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <Windows.h>
#include <stdlib.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

using namespace std;

class System {
public:
	static string CurrentDirectory() {
		DWORD pathSize = MAX_PATH;
		WCHAR pathBuffer[MAX_PATH + 1];
		GetCurrentDirectory(pathSize, pathBuffer);
		pathBuffer[MAX_PATH] = 0;

		char smallChar[MAX_PATH + 1];
		wcstombs(smallChar, pathBuffer, MAX_PATH + 1);
		return string(smallChar);
	}

	// Puts a file's contents into a string
	static string File2String(string filename) {
		string result;

		ifstream in(filename, ifstream::in);
		if (in.is_open()) {
			stringstream str_buffer;
			str_buffer << in.rdbuf();
			result = str_buffer.str();
		}

		in.close();
		return result;
	}

	static glm::mat4 sphere2xyz(glm::vec3 spherePos) {
		glm::vec3 xyz(spherePos.x, 0, 0);
		xyz = glm::angleAxis(glm::radians(spherePos.y), glm::vec3(0, 0, 1)) * xyz;
		xyz = glm::angleAxis(glm::radians(spherePos.z), glm::vec3(0, 1, 0)) * xyz;
		return glm::translate(glm::mat4(), xyz);
	}
};
#endif
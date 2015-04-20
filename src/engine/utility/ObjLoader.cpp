#include "ObjLoader.h"
#include <tiny_obj_loader.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <tchar.h>

using namespace std;

bool ObjLoader::LoadObj(const char * filePath, vector<float> & vertexData) {
	const bool debug = false;

	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;

	string err = tinyobj::LoadObj(shapes, materials, filePath);

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

	if (debug) cout << "# shapes: " << shapes.size() << endl;

	for (int i = 0; i < shapes.size(); ++i) {
		tinyobj::mesh_t _mesh = shapes[i].mesh;
		if (debug) {
			cout << "# pos: " << _mesh.positions.size() << endl;
			cout << "# norm: " << _mesh.normals.size() << endl;
			cout << "# ind: " << _mesh.indices.size() << endl;
		}

		bool doNorms = _mesh.normals.size() > 2;
		bool doTex = _mesh.texcoords.size() > 1;

		for (int j = 0; j < _mesh.indices.size(); ++j) {
			int index = (int) _mesh.indices[j];
			// positions
			for (int k = 0; k < 3; ++k) {
				vertexData.push_back(_mesh.positions[3 * index + k]);
			}
			// normals
			for (int k = 0; k < 3; ++k) {
				vertexData.push_back(((doNorms) ? _mesh.normals[3 * index + k] : 0.f));
			}
			// colors
			for (int k = 0; k < 3; ++k) {
				vertexData.push_back(1.f);
			}
		}
	}

	/*
	vector<float> v, vt, vn, index;

	ifstream in(filePath);
	if (!in.is_open()) {
		cout << "File not found." << endl;
		return false;
	}

	const float cMin = -1000000000.f;
	const float cMax = 1000000000.f;
	float x_min = cMax, x_max = cMin, y_min = cMax, y_max = cMin, z_min = cMax, z_max = cMin;

	string line;
	while (getline(in, line)) {
		stringstream iss;
		string token;

		iss << line;
		getline(iss, token, ' ');
		if (token.compare("v") == 0) {
			getline(iss, token, ' ');
			float val = stof(token);
			x_max = max(x_max, val);
			x_min = min(x_min, val);
			v.push_back(val);

			getline(iss, token, ' ');
			val = stof(token);
			y_max = max(y_max, val);
			y_min = min(y_min, val);
			v.push_back(val);

			getline(iss, token, ' ');
			val = stof(token);
			z_max = max(z_max, val);
			z_min = min(z_min, val);
			v.push_back(val);
		}
		else if (token.compare("vt") == 0) {
			while (getline(iss, token, ' ')) vt.push_back(stof(token));
		}
		else if (token.compare("vn") == 0) {
			while (getline(iss, token, ' ')) vn.push_back(stof(token));
		}
		else if (token.compare("f") == 0) {
			while (getline(iss, token, ' ')) index.push_back(stoi(token));
		}
	}

	float x_offset = (x_max + x_min) / 2;
	float y_offset = (y_max + y_min) / 2;
	float z_offset = (z_max + z_min) / 2;
	//cout << "Object center: (" << x_offset << ", " << y_offset << ", " << z_offset << ")\n";

	for (int i = 0; i < index.size(); ++i) {
		int ind = index[i] - 1;
		// position
		vertexData.push_back(v[3 * ind + 0] - x_offset);
		vertexData.push_back(v[3 * ind + 1] - y_offset);
		vertexData.push_back(v[3 * ind + 2] - z_offset);
		// normal
		for (int j = 0; j < 3; ++j) vertexData.push_back(vn[3 * ind + j]);
		// color
		for (int j = 0; j < 3; ++j) vertexData.push_back(1.f);
	}
	*/
}
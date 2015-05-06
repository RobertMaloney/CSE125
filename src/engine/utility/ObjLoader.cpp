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

bool ObjLoader::LoadObj(string filePath, vector<float> & vertexData) {
	const bool debug = false;

	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;

	string dir = filePath.substr(0, filePath.find_last_of('/')+1);

	string err = tinyobj::LoadObj(shapes, materials, filePath.c_str(), dir.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

	if (debug) cout << "# shapes: " << shapes.size() << endl;
	if (debug) cout << "# mats: " << materials.size() << endl;

	for (int i = 0; i < shapes.size(); ++i) {
		tinyobj::mesh_t _mesh = shapes[i].mesh;
		if (debug) {
			cout << "# pos: " << _mesh.positions.size() << endl;
			cout << "# norm: " << _mesh.normals.size() << endl;
			cout << "# ind: " << _mesh.indices.size() << endl;
			cout << "# mat_id: " << _mesh.material_ids.size() << endl;
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
			int mat_id = _mesh.material_ids[j / 3];
			for (int k = 0; k < 3; ++k) {
				vertexData.push_back(materials[mat_id].diffuse[k]);
			}
			// tex
			for (int k = 0; k < 2; ++k) {
				vertexData.push_back(((doTex) ? _mesh.texcoords[2 * index + k] : 0.f));
			}
		}
	}
}
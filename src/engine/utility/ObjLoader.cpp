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

	for (unsigned int i = 0; i < shapes.size(); ++i) {
		tinyobj::mesh_t _mesh = shapes[i].mesh;
		if (debug) {
			cout << "# pos: " << _mesh.positions.size() << endl;
			cout << "# norm: " << _mesh.normals.size() << endl;
			cout << "# ind: " << _mesh.indices.size() << endl;
			cout << "# mat_id: " << _mesh.material_ids.size() << endl;
		}

		bool doNorms = _mesh.normals.size() > 2;
		bool doTex = _mesh.texcoords.size() > 1;

		for (unsigned int j = 0; j < _mesh.indices.size(); ++j) {
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

		/*for (size_t i = 0; i < materials.size(); i++) {
			printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
			printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
			printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
			printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
			printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
			printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
			printf("  material.Ns = %f\n", materials[i].shininess);
			printf("  material.Ni = %f\n", materials[i].ior);
			printf("  material.dissolve = %f\n", materials[i].dissolve);
			printf("  material.illum = %d\n", materials[i].illum);
			printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
			printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
			printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
			printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
			std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
			std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
			for (; it != itEnd; it++) {
				printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
			}
			printf("\n");
		}*/
	}
}
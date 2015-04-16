#include "ObjLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool ObjLoader::LoadObj(const char * filePath, vector<float> & vertexData) {
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
}
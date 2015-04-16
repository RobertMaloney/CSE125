#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <vector>

using namespace std;

class ObjLoader {
public:
	static bool LoadObj(const char*, vector<float>&);
};
#endif
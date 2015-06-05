#include "Model.h"
#include <string>

std::map<Model, std::string> ResourceMap::map = {
      { OB_TYPE, "ob" },
      { PB_TYPE, "pb" },
      { BB_TYPE, "bb" },
      { GB_TYPE, "gb" },
      { TREE, "tree" },
      { STUMP, "stump" },
      { GRASS, "grass" },
      { MUSHROOM, "mushroom" },
      { FLOWER, "flower" },
      { CLOUD, "cloud" },
      { PILL, "pill" },
      { ROCK_1, "boulder1" },
      { ROCK_2, "boulder2" },
      { ROCK_3, "boulder3" },
      { ROCK_4, "rock4" },
      { TALL_ROCK_1, "rock5" },
      { TALL_ROCK_2, "rock6" },
      { TALL_ROCK_3, "rock7" },
      { BUNNY, "bunny" },
      { BIRD, "bird" }
};

std::string ResourceMap::getObjFile(Model rm)
{
   return std::string(map[rm]) + ".obj";
}

std::string ResourceMap::getMatlFile(Model rm)
{
   return std::string(map[rm]) + ".matl";
}


Model ResourceMap::getModelFromString(std::string m) {
	transform(m.begin(), m.end(), m.begin(), ::tolower);
	for (auto it = map.begin(); it != map.end(); ++it) {
		if (m == it->second){
			return it->first;
		}
	}
}
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
      { MUSHROOM_1, "mushroom1" },
      { MUSHROOM_2, "mushroom2" },
      { MUSHROOM_3, "mushroom3" },
      { MUSHROOM_4, "mushroom4" },
      { FLOWER_1, "flower1" },
      { FLOWER_2, "flower2" },
      { FLOWER_3, "flower3" },
      { FLOWER_4, "flower4" },
      { FLOWER_5, "flower5" },
      { FLOWER_6, "flower6" },
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

std::map<Model, PType> ResourceMap::map2 = {
	{ OB_TYPE, PType::P_PLAYER },
	{ PB_TYPE, PType::P_PLAYER },
	{ BB_TYPE, PType::P_PLAYER },
	{ GB_TYPE, PType::P_PLAYER },
	{ TREE, PType::P_TREE },
	{ STUMP, PType::P_STUMP },
	{ GRASS, PType::P_RES },
	{ MUSHROOM, PType::P_MUSH },
	{ MUSHROOM_1, PType::P_MUSH },
	{ MUSHROOM_2, PType::P_MUSH },
	{ MUSHROOM_3, PType::P_MUSH },
	{ MUSHROOM_4, PType::P_MUSH },
	{ FLOWER_1, PType::P_FLOWER },
	{ FLOWER_2, PType::P_FLOWER },
	{ FLOWER_3, PType::P_FLOWER },
	{ FLOWER_4, PType::P_FLOWER },
	{ FLOWER_5, PType::P_FLOWER },
	{ FLOWER_6, PType::P_FLOWER },
	{ CLOUD, PType::P_POWER },
	{ PILL, PType::P_POWER },
	{ ROCK_1, PType::P_ROCK },
	{ ROCK_2, PType::P_ROCK },
	{ ROCK_3, PType::P_ROCK },
	{ ROCK_4, PType::P_ROCK },
	{ TALL_ROCK_1, PType::P_TALL },
	{ TALL_ROCK_2, PType::P_TALL },
	{ TALL_ROCK_3, PType::P_TALL },
	{ BUNNY, PType::P_POWER },
	{ BIRD, PType::P_POWER }
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




PType ResourceMap::getPType(Model rm)
{
	return map2[rm];
}
#ifndef RESOURCE_MODEL_H
#define RESOURCE_MODEL_H

#include <map>
#include <string>
#include <algorithm>

#include "../graphics/PType.h"

using std::transform;

enum Model {
   OB_TYPE,
   PB_TYPE,
   BB_TYPE,
   GB_TYPE,
   TREE,
   STUMP,
   GRASS,
   MUSHROOM,
   MUSHROOM_1,
   MUSHROOM_2,
   MUSHROOM_3,
   MUSHROOM_4,
   FLOWER_1,
   FLOWER_2,
   FLOWER_3,
   FLOWER_4,
   FLOWER_5,
   FLOWER_6,
   CLOUD,
   PILL,
   ROCK_1,
   ROCK_2,
   ROCK_3,
   ROCK_4,
   TALL_ROCK_1,
   TALL_ROCK_2,
   TALL_ROCK_3,
   BUNNY,
   BIRD
};

class ResourceMap {
private:
	static std::map<Model, std::string> map;
	static std::map<Model, PType> map2;

public:

static Model getModelFromString(std::string  m);
   static std::string getObjFile(Model rm);
   static std::string getMatlFile(Model rm);
   static PType getPType(Model rm);
};
/*enum Model {
   RED_TREE_S,
   RED_TREE_L,
   GREEN_TREE_S_1,
   GREEN_TREE_L_2,
   GREEN_TREE_S_1,
   GREEN_TREE_L_2,
   PINE_TREE_1,
   PINE_TREE_2,
   GRASS_1,
   GRASS_2,
   GRASS_3,
   GRASS_4,
   LILY_PAD,
   MUSHROOM_1,
   MUSHROOM_2, 
   MUSHROOM_3,
   RED_FLOWER_1,
   RED_FLOWER_2,
   YELLOW_FLOWER_1,
   YELLOW_FLOWER_2,
   BLUE_FLOWER_1,
   BLUE_FLOWER_2,
   ROCK_S_1,
   ROCK_S_2,
   ROCK_S_3,
   ROCK_S_4,
   ROCK_S_5,
   ROCK_S_6,
   ROCK_L_1,
   ROCK_L_2,
   ROCK_L_3,
   STUMP_1,
   STUMP_2,
   STUMP_3,
   STUMP_4,
   LOG
};*/

#endif
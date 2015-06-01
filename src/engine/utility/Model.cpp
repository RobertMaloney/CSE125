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
      { ROCK_1, "rock1" },
      { ROCK_2, "rock2" },
      { ROCK_3, "rock3" },
      { ROCK_4, "rock4" },
      { TALL_ROCK_1, "rock5" },
      { TALL_ROCK_2, "rock6" },
      { TALL_ROCK_3, "rock7" }
};

std::string ResourceMap::getObjFile(Model rm)
{
   return std::string(map[rm]) + ".obj";
}

std::string ResourceMap::getMatlFile(Model rm)
{
   return std::string(map[rm]) + ".matl";
}
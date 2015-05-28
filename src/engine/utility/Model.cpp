#include "Model.h"
#include <string>

std::map<Model, std::string> ResourceMap::map = {
      { OB_TYPE, "ob" },
      { PB_TYPE, "pb" },
      { BB_TYPE, "bb" },
      { GB_TYPE, "gb" },
      { TREE, "tree" },
      { ROCK, "rock" },
      { STUMP, "stump" },
      { GRASS, "grass" },
      { MUSHROOM, "mushroom" },
      { FLOWER, "flower" },
      { CLOUD, "cloud2" }
};

std::string ResourceMap::getObjFile(Model rm)
{
   return std::string(map[rm]) + ".obj";
}

std::string ResourceMap::getMatlFile(Model rm)
{
   return std::string(map[rm]) + ".matl";
}
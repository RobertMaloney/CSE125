#include "ResourceModel.h"
#include <string>

std::map<ResourceModel, std::string> ResourceMap::map = {
      { OB_TYPE, "ob" },
      { PB_TYPE, "pb" },
      { BB_TYPE, "bb" },
      { GB_TYPE, "gb" },
      { TREE, "tree" },
      { ROCK, "rock" },
      { STUMP, "stump" },
      { GRASS, "grass" },
      { MUSHROOM, "mushroom" }
};

std::string ResourceMap::getObjFile(ResourceModel rm)
{
   return std::string(map[rm]) + ".obj";
}

std::string ResourceMap::getMatlFile(ResourceModel rm)
{
   return std::string(map[rm]) + ".matl";
}
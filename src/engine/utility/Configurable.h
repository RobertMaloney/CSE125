#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <json/json.h>

class Configurable {

public:

	virtual void loadConfiguration(Json::Value config) = 0;

};

#endif
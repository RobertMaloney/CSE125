#ifndef CONFIG_H
#define CONFIG_H

#include "json\json.h"

class Config {
public:
	static Json::Value settings;

	static bool parseJson(std::string);
	static void writeJson(std::string);

private:
	static void validate();
};
#endif
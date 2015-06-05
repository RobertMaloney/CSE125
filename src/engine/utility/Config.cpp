#include "Config.h"
#include "System.h"
#include <iostream>

Json::Value Config::settings;

bool Config::parseJson(std::string path) {
	Json::Reader jr;
	jr.parse(System::File2String(path), settings, false);
	if (!jr.good())
		std::cout << jr.getFormattedErrorMessages() << std::endl;
	else
		validate();

	return jr.good();
}

void Config::writeJson(std::string path) {
	Json::FastWriter jw;
	FILE *f = fopen(path.c_str(), "w");
	std::string json = jw.write(settings);
	fputs(json.c_str(), f);
	fclose(f);
}

// Makes sure that config has everything we need
void Config::validate() {
	if (settings["graphics"].isNull()) {
		settings["graphics"]["width"] = 1600;
		settings["graphics"]["height"] = 1200;
	}
}
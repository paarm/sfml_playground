#pragma once

#include "level.h"
#include "../gamedirector.h"
#include "../../../thirdparty/simplejson/JSONValue.h"
#include <string>
#include <fstream>
#include <streambuf>


class LevelLoader {
private:
	Level mLevel;
	bool parseLevel(const string &rLevelName);
	bool parseRoot(JSONValue *rJSONValueParent);
	bool parseHeader(JSONValue *rJSONValueParent);
public:
	bool loadLevel(const string &rLevelName);
	const string extractString(JSONValue *rJSONValueParent, const wstring &rName);

};

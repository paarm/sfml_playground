#pragma once

#include "level.h"

class LevelLoader {
private:
		Level mLevel;
public:
	bool loadLevel(const string &rLevelName);
};
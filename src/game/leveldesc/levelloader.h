#pragma once

#include "level.h"
#include "../gameconst.h"

class LevelLoader {
private:
		Level mLevel;
public:
	bool loadLevel(const string &rLevelName) {
		bool rv=false;
		mLevel.clearLevel();
		mLevel.setLevelName(rLevelName);

		for (int y=0;y<BLOCK_COUNT_Y;y++) {
			Row &rRow=mLevel.addRow();
			for (int x=0;x<BLOCK_COUNT_Y;x++) {
				rRow.addColumn();
			}
		}
		rv=true;
		return rv;
	}
};
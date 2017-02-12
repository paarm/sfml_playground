#include "levelloader.h"
#include "../gamedirector.h"

bool LevelLoader::loadLevel(const string &rLevelName) {
	bool rv=false;
	mLevel.clearLevel();
	mLevel.setAssetPath("assets/");
	mLevel.setLevelName(rLevelName);
	mLevel.setBackgroundImageName("");

	for (int y=0, countY=GameDirector::getInstance().getBlockCountY();y<countY;y++) {
		Row &rRow=mLevel.addRow();
		for (int x=0, countX=GameDirector::getInstance().getBlockCountX();x<countX;x++) {
			rRow.addColumn();
		}
	}
	rv=true;
	return rv;
}


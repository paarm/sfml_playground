#include "level.h"

Row& Level::addRow() {
	int rRowIndex=mRowList.size();
	Row rRow;
	rRow.setRowIndex(rRowIndex);
	mRowList.push_back(rRow);
	return mRowList[rRowIndex];
}

void Level::clearLevel() {
	mRowList.clear();
	mLevelName="";
	mBackgroundImageName="";
}

void Level::setLevelName(const string &rLevelName) {
	mLevelName=rLevelName;
}

string& Level::getLevelName() {
	return mLevelName;
}

bool Level::isRowAvailable(int rRowIndex) {
	bool rv=false;
	if (rRowIndex>=0 && rRowIndex<mRowList.size()) {
		rv=true;
	}
	return rv;
}

Row& Level::getRow(int rRowIndex) {
	return mRowList[rRowIndex];
}

int Level::getRowCount() {
	return mRowList.size();
}

void Level::setBackgroundImageName(const string &rBackgroundImageName) {
	mBackgroundImageName=rBackgroundImageName;
}
string &Level::getBackgroundImageName() {
	return mBackgroundImageName;
}

void Level::setAssetPath(const string &rAssetPath) {
	mAssetPath=rAssetPath;
}
string &Level::getAssetPath() {
	return mAssetPath;
}

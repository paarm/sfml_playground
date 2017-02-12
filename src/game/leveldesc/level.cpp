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
	mObjectDescMap.clear();
}

void Level::setLevelName(const string &rLevelName) {
	mLevelName=rLevelName;
}

string& Level::getLevelName() {
	return mLevelName;
}

void Level::addObjectDesc(ObjectDesc &rObjectDesc) {
	mObjectDescMap[rObjectDesc.getId()]=rObjectDesc;
}
bool Level::isObjectDescAvailable(const string &rId) {
	bool rv=false;
	if (mObjectDescMap.find(rId)!=mObjectDescMap.end()) {
		rv=true;
	}
	return rv;
}
ObjectDesc& Level::getObjectDesc(const string &rId) {
	return mObjectDescMap[rId];
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

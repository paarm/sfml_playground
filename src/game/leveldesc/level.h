#pragma once

#include "row.h"
#include <vector>

using namespace std;
class Level {
private:
	string 		mAssetPath="assets/";
	string 		mLevelName="";
	vector<Row> mRowList;
	string		mBackgroundImageName="";
public:
	void setAssetPath(const string &rAssetPath);
	string &getAssetPath();
	void setBackgroundImageName(const string &rBackgroundImageName);
	string &getBackgroundImageName();
	Row& addRow();
	void clearLevel();
	void setLevelName(const string &rLevelName);
	string& getLevelName();
	bool isRowAvailable(int rRowIndex);
	Row& getRow(int rRowIndex);
	int getRowCount();
};

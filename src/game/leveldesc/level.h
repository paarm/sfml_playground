#pragma once

#include "row.h"
#include <vector>
#include "objectdesc.h"


using namespace std;
class Level {
private:
	string 		mAssetPath="assets/";
	string 		mLevelName="";
	vector<Row> mRowList;
	string		mBackgroundImageName="";
	map<string, ObjectDesc> mObjectDescMap;
public:
	void setAssetPath(const string &rAssetPath);
	string &getAssetPath();
	void setBackgroundImageName(const string &rBackgroundImageName);
	string &getBackgroundImageName();
	void addObjectDesc(ObjectDesc &rObjectDesc);
	bool isObjectDescAvailable(const string &rId);
	ObjectDesc& getObjectDesc(const string &rId);

	Row& addRow();
	void clearLevel();
	void setLevelName(const string &rLevelName);
	string& getLevelName();
	bool isRowAvailable(int rRowIndex);
	Row& getRow(int rRowIndex);
	int getRowCount();
};

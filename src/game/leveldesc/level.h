#pragma once

#include "row.h"
#include <vector>
#include "objectdesc.h"
#include "layer.h"
#include <iostream>

using namespace std;
class Level {
private:
	string 		mAssetPath="assets/";
	string 		mLevelName="";
	map<string, ObjectDesc> mObjectDescMap;
	vector<Layer> mLayerList;
	Layer		mEmptyLayer;
public:
	void setAssetPath(const string &rAssetPath);
	string &getAssetPath();
	void setBackgroundImageName(const string &rBackgroundImageName);
	string &getBackgroundImageName();
	void addObjectDesc(ObjectDesc &rObjectDesc);
	bool isObjectDescAvailable(const string &rId);
	ObjectDesc& getObjectDesc(const string &rId);

	Layer& getOrAddLayer(const string &rLayerName);
	int getLayerCount();
	Layer& getLayerFromIndex(int rIndex);
	

	void clearLevel();
	void setLevelName(const string &rLevelName);
	string& getLevelName();
};

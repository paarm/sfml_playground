#include "level.h"

void Level::clearLevel() {
	mLevelName="";
	mObjectDescMap.clear();
	mLayerList.clear();
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

void Level::setAssetPath(const string &rAssetPath) {
	mAssetPath=rAssetPath;
}
string &Level::getAssetPath() {
	return mAssetPath;
}

Layer& Level::getOrAddLayer(const string &rLayerName) {
	auto foundIterator=find_if(mLayerList.begin(), mLayerList.end(), [rLayerName] (Layer& r) -> bool {
		//cout <<"Lambda Layer Name: " << r.getLayerName() << "Name: "<< rLayerName <<endl;
		return rLayerName.compare(r.getLayerName())==0;
	});
	if (foundIterator!=mLayerList.end()) { 
		return *foundIterator;
	}
	Layer rLayer;
	rLayer.setLayerName(rLayerName);
	mLayerList.push_back(rLayer);
	return mLayerList[mLayerList.size()-1];
}

int Level::getLayerCount() {
	return mLayerList.size();
}

Layer& Level::getLayerFromIndex(int rIndex) {
	if (rIndex>=0 && rIndex<(int)mLayerList.size()) {
		return mLayerList[rIndex];
	}
	return mEmptyLayer;
}


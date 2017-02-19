#pragma once
#include <vector>
#include <map>
#include "layerobject.h"
using namespace std;

class Layer {
private:
	string  mLayerName;
	int     mZOrder=0;
	vector<LayerObject> mLayerObjectList;
	map<string, LayerObject*> mLayerObjectNameMap;
	LayerObject	mEmptyLayerObject;
public:
	void setLayerName(const string& rLayerName) {
		mLayerName=rLayerName;
	}
	const string& getLayerName() {
		return mLayerName;
	}
	void setZOrder(int rZOrder) {
		mZOrder=rZOrder;
	}
	int getZOrder() {
		return mZOrder;
	}

	void addLayerObject(LayerObject &rLayerObject) {
		mLayerObjectList.push_back(rLayerObject);
		if (rLayerObject.getLayerObjectName().length()>0) {
			mLayerObjectNameMap[rLayerObject.getLayerObjectName()]=&mLayerObjectList[mLayerObjectList.size()-1];
		}
	}
	LayerObject& getLayerObjectByLayerObjectName(string &rLayerObjectName) {
		auto r=mLayerObjectNameMap.find(rLayerObjectName);
		if (r!=mLayerObjectNameMap.end()) {
			return *r->second;
		}
		return mEmptyLayerObject;
	}

	int getLayerObjectCount() {
		return mLayerObjectList.size();
	}
	LayerObject& getLayerObjectByIndex(int rIndex) {
		if ((int)mLayerObjectList.size()>rIndex && rIndex>=0) {
			return mLayerObjectList[rIndex];
		}
		return mEmptyLayerObject;
	}
};


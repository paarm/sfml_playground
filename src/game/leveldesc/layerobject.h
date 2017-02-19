#pragma once
#include <string>
#include "objectdesc.h"

using namespace std;
class LayerObject {
private:
	string 	mLayerObjectName;
	string 	mObjectDescId;
	int		mPosX=0;
	int		mPosY=0;
	int		mAngle=0;
public:
	void setName(const string &rName) {
		mLayerObjectName=rName;
	}
	const string& getLayerObjectName() {
		return mLayerObjectName;
	}
	void setObjectDescId(const string &rObjectDescId) {
		mObjectDescId=rObjectDescId;
	}
	const string& getObjectDescId() {
		return mObjectDescId;
	}
	void setPosX(int rPosX) {
		mPosX=rPosX;
	}
	int getPosX() {
		return mPosX;
	}
	void setPosY(int rPosY) {
		mPosY=rPosY;
	}
	int getPosY() {
		return mPosY;
	}
};
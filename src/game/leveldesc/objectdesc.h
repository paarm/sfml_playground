#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;
enum class ObjectType {
	Nothing=0,
	Fixed,
	Gravity,
	Spike,
	Player,
	Enemy
};

enum class Direction {
	Default=0,
	Left,
	Right,
	Bottom,
	Top
};

class ObjectSequence {
private:
	string 		mObjectSequenceName;
	string		mSequenceName;
	int			mFrameDelayInMs;
public:
	string& getObjectSequenceName() {
		return mObjectSequenceName;
	}
	void setObjectSequenceName(string &rObjectSequenceName) {
		mObjectSequenceName=rObjectSequenceName;
	}
	string& getSequenceName() {
		return mSequenceName;
	}
	void setSequenceName(string &rSequenceName) {
		mSequenceName=rSequenceName;
	}
	int getFrameDelayInMs() {
		return mFrameDelayInMs;
	}
	void setFrameDelayInMs(int rFrameDelayInMs) {
		mFrameDelayInMs=rFrameDelayInMs;
	}
};

class ObjectDesc {
private:
	string 		mId;
	ObjectType	mObjectType;
	string		mDefaultFrame;
	string 		mDefaultSequence;
	map<string, ObjectSequence> mObjectSequenceMap;
	vector<string> mObjectSequenceNamesVector;
public:
	void setId(string &rId) {
		mId=rId;
	}
	void setObjectType(ObjectType rObjectType) {
		mObjectType=rObjectType;
	}
	void setDefaultFrame(string &rDefaultFrame) {
		mDefaultFrame=rDefaultFrame;
	}
	void setDefaultSequence(string &rDefaultSequence) {
		mDefaultSequence=rDefaultSequence;
	}
	void addObjectSequence(ObjectSequence &rObjectSequence) {
		const string &key=rObjectSequence.getObjectSequenceName();
		mObjectSequenceMap[key]=ObjectSequence(rObjectSequence);
		mObjectSequenceNamesVector.push_back(key);
	}

	bool isObjectSequenceAvailable(const string &rObjectSequenceName) {
		bool rv=false;
		if (mObjectSequenceMap.find(rObjectSequenceName)!=mObjectSequenceMap.end()) {
			rv=true;
		}
		return rv;
	}
	ObjectSequence &getObjectSequence(const string &rObjectSequenceName) {
		return mObjectSequenceMap[rObjectSequenceName];
	}

	int getObjectSequenceCount() {
		return mObjectSequenceNamesVector.size();
	}

	const string& getObjectSequenceName(int rIndex) {
		return mObjectSequenceNamesVector[rIndex];
	}

	string& getId() {
		return mId;
	}
	ObjectType& getObjectType() {
		return mObjectType;
	}
	string& getDefaultFrame() {
		return mDefaultFrame;
	}
	string& getDefaultSequence() {
		return mDefaultSequence;
	}
};
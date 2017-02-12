#pragma once
#include "objectdesc.h"
#include "../../engine/node.h"

class Column {
private:
	int             mRowIndex;
	int             mColumnIndex;
	ObjectDesc 		*mObjectDesc;
	Direction  		mDirection;
	Node            *mCurrentNode=nullptr;
public:
	Column() : mRowIndex(-1), mColumnIndex(-1), mObjectDesc(nullptr), mCurrentNode(nullptr), mDirection(Direction::Default) {}
	void setRowIndex(int rRowIndex) {
		mRowIndex=rRowIndex;
	}
	void setColumnIndex(int rColumnIndex) {
		mColumnIndex=rColumnIndex;
	}
	int getRowIndex() {
		return mRowIndex;
	}
	int getColumnIndex() {
		return mColumnIndex;
	}
	void setDirection(Direction &rDirection) {
		mDirection=rDirection;
	}
	Direction getDirection() {
		return mDirection;
	}
	ObjectDesc*getObjectDesc() {
		return mObjectDesc;
	}
	void setObjectDesc(ObjectDesc *rObjectDesc) {
		mObjectDesc=rObjectDesc;
	}
	void setCurrentNode(Node *rNode) {
		mCurrentNode=rNode;
	}
	Node* getCurrentNode() {
		return mCurrentNode;
	}
};
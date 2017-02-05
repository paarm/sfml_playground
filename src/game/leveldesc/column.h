#pragma once
#include "block.h"

class Column {
private:
	int             mRowIndex;
	int             mColumnIndex;
	Block 			mBlock;
public:
	Column() : mRowIndex(-1), mColumnIndex(-1) {}
	Block &getBlock() {
		return mBlock;
	}
	void setBlock(Block rBlock) {
		mBlock=rBlock;
	}
	void clearBlock() {
		mBlock=Block();
	}
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
	
};
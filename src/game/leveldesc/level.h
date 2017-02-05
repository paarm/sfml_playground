#pragma once

#include "row.h"
#include <vector>

using namespace std;
class Level {
private:
	string 		mLevelName="";
	vector<Row> mRowList;
public:
	Row& addRow() {
		int rRowIndex=mRowList.size();
		Row rRow;
		rRow.setRowIndex(rRowIndex);
		mRowList.push_back(rRow);
		return mRowList[rRowIndex];
	}
	void clearLevel() {
		mRowList.clear();
		mLevelName="";
	}

	void setLevelName(const string &rLevelName) {
		mLevelName=rLevelName;
	}
	string& getLevelName() {
		return mLevelName;
	}
	bool isRowAvailable(int rRowIndex) {
		bool rv=false;
		if (rRowIndex>=0 && rRowIndex<mRowList.size()) {
			rv=true;
		}
		return rv;
	}
	Row& getRow(int rRowIndex) {
		return mRowList[rRowIndex];
	}
	int getRowCount() {
		return mRowList.size();
	}
};

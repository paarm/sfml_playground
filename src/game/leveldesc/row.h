#pragma once
#include <vector>
#include "column.h"

using namespace std;
class Row {
private:
	vector<Column> mColumnList;
	int mRowIndex;
public:
	Row() : mRowIndex(-1) {}
	void setRowIndex(int rRowIndex) {
		mRowIndex=rRowIndex;
	}
	int  getRowIndex() {
		return mRowIndex;
	}
	Column& addColumn() {
		int rColumnIndex=mColumnList.size();
		Column rColumn;
		rColumn.setColumnIndex(rColumnIndex);
		rColumn.setRowIndex(mRowIndex);
		mColumnList.push_back(rColumn);
		return mColumnList[rColumnIndex];
	}
	bool isColumnAvailable(int rColumnIndex) {
		bool rv=false;
		if (rColumnIndex>=0 && rColumnIndex<mColumnList.size()) {
			rv=true;
		}
		return rv;
	}
	
	Column& getColumn(int rColumnIndex) {
		return mColumnList[rColumnIndex];
	}
	int getColumnCount() {
		return mColumnList.size();
	}
};
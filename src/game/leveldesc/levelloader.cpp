#include "levelloader.h"

using namespace std;
bool LevelLoader::loadLevel(const string &rLevelName) {
	bool rv=false;

	parseLevel(rLevelName);
#if 0
	mLevel.clearLevel();
	mLevel.setAssetPath("assets/");
	mLevel.setLevelName(rLevelName);
	mLevel.setBackgroundImageName("");
#endif
	for (int y=0, countY=GameDirector::getInstance().getBlockCountY();y<countY;y++) {
		Row &rRow=mLevel.addRow();
		for (int x=0, countX=GameDirector::getInstance().getBlockCountX();x<countX;x++) {
			rRow.addColumn();
		}
	}
	rv=true;
	return rv;
}

bool LevelLoader::parseLevel(const string &rLevelName) {
	bool rv=true;
	cout << "Loading Level " << rLevelName << endl;

	ifstream rFileStream(rLevelName);
	string rFileString((istreambuf_iterator<char>(rFileStream)), istreambuf_iterator<char>());
	cout << rFileString <<endl;

	JSONValue *rJSONValue=JSON::Parse(rFileString.c_str());
	if (rJSONValue->IsObject()) {
		parseRoot(rJSONValue);
	} 

	return rv;
}

bool LevelLoader::parseRoot(JSONValue *rJSONValueParent) {
	bool rv=false;
	do {
		if (!rJSONValueParent->IsObject()) {
			cout << "Level root is no object" << endl;
			break;
		}
		JSONValue *rHeader=rJSONValueParent->Child(L"Header");
		if (!rHeader) {
			cout << "Level Header not found" << endl;
			break;
		} 
		if (!parseHeader(rHeader)) {
			break;
		}
		rv=true;
	} while(false);
	return rv;
}


bool LevelLoader::parseHeader(JSONValue *rJSONValueParent) {
	bool rv=false;
	do {
		if (!rJSONValueParent->IsObject()) {
			cout << "Level Header is no object" << endl;
			break;
		}

		string t=extractString(rJSONValueParent, L"LevelName");
		if (t.empty()) {
			cout << "LevelName not found or empty" << endl;
			break;
		}
		mLevel.setLevelName(t);

		t=extractString(rJSONValueParent, L"AssetPath");
		if (t.empty()) {
			cout << "AssetPath not found or empty" << endl;
			break;
		}
		mLevel.setAssetPath(t);

		t=extractString(rJSONValueParent, L"BackgroundImageName");
		if (t.empty()) {
			cout << "BackgroundImageName not found or empty" << endl;
			break;
		}
		mLevel.setBackgroundImageName(t);
		rv=true;
	} while(false);
	return rv;
}

const string LevelLoader::extractString(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsString()) {
		return string(v->AsStringPtr()->begin(), v->AsStringPtr()->end());
	}
	return "";
}


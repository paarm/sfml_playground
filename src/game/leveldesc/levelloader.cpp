#include "levelloader.h"
#include "../../engine/texturemanager.h"

using namespace std;
bool LevelLoader::loadLevel(const string &rLevelName) {
	bool rv=false;

	rv=parseLevel(rLevelName);
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
		rv=parseRoot(rJSONValue);
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
		// Header
		JSONValue *rHeader=rJSONValueParent->Child(L"Header");
		if (!rHeader) {
			cout << "Level Header not found" << endl;
			break;
		} 
		if (!parseHeader(rHeader)) {
			break;
		}
		// Textures
		JSONValue *rTextures=rJSONValueParent->Child(L"Textures");
		if (!rTextures) {
			cout << "Level Textures definition not found" << endl;
			break;
		} 
		if (!parseTextures(rTextures)) {
			break;
		}
		// Textures AutoFramesAndSequences		
		JSONValue *rAutoFramesAndSequences=rJSONValueParent->Child(L"AutoFramesAndSequences");
		if (!rAutoFramesAndSequences) {
			cout << "Level AutoFramesAndSequences definition not found" << endl;
			break;
		} 
		if (!parseAutoFramesAndSequences(rAutoFramesAndSequences)) {
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

bool LevelLoader::parseTextures(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level Textures is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rTextureName=extractString(rJSONValue, L"TextureName");
			if (rTextureName.empty()) {
				cout << "Textures.TextureName not found or empty" << endl;
				continue;
			}
			string rTextureFile=extractString(rJSONValue, L"TextureFile");
			if (rTextureName.empty()) {
				cout << "Textures.TextureFile not found or empty" << endl;
				continue;
			}
			TextureManager::getInstance().loadTexture(mLevel.getAssetPath()+rTextureFile, &rTextureName);
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseAutoFramesAndSequences(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level AutoFramesAndSequences is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rTextureName=extractString(rJSONValue, L"TextureName");
			if (rTextureName.empty()) {
				cout << "Textures.TextureName not found or empty" << endl;
				continue;
			}
			string rSequenceName=extractString(rJSONValue, L"SequenceName");
			if (rSequenceName.empty()) {
				cout << "AutoFramesAndSequences.SequenceName not found or empty" << endl;
				continue;
			}
			int rFrameCountX=(int)extractNumber(rJSONValue, L"FrameCountX");
			int rFrameCountY=(int)extractNumber(rJSONValue, L"FrameCountY");

			TextureManager::getInstance().createAutomaticFramesAndSequence(rTextureName, rSequenceName, rFrameCountX, rFrameCountY);
		}
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

double LevelLoader::extractNumber(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsNumber()) {
		return v->AsNumber();
	}
	return 0.0;
}


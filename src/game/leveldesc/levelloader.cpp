#include "levelloader.h"
#include "../../engine/texturemanager.h"

using namespace std;
bool LevelLoader::loadLevel(const string &rLevelName) {
	bool rv=false;

	mLevel.clearLevel();
	for (int y=0, countY=GameDirector::getInstance().getBlockCountY();y<countY;y++) {
		Row &rRow=mLevel.addRow();
		for (int x=0, countX=GameDirector::getInstance().getBlockCountX();x<countX;x++) {
			rRow.addColumn();
		}
	}
	rv=parse(rLevelName);
#if 0
	mLevel.clearLevel();
	mLevel.setAssetPath("assets/");
	mLevel.setLevelName(rLevelName);
	mLevel.setBackgroundImageName("");
#endif
	return rv;
}

Level& LevelLoader::getLevel() {
	return mLevel;
}


bool LevelLoader::parse(const string &rLevelName) {
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
		if (rAutoFramesAndSequences) {
			if (!parseAutoFramesAndSequences(rAutoFramesAndSequences)) {
				break;
			}
		}
		// Frames for Textures FramesForTexture		
		JSONValue *rFramesForTexture=rJSONValueParent->Child(L"FramesForTextures");
		if (rFramesForTexture) {
			if (!parseFramesForTextures(rFramesForTexture)) {
				break;
			}
		}
		// Frames for FramesForSequences		
		JSONValue *rFramesForSequences=rJSONValueParent->Child(L"FramesForSequences");
		if (rFramesForSequences) {
			if (!parseFramesForSequences(rFramesForSequences)) {
				break;
			}
		}
		// ObjectDescriptors
		JSONValue *rObjectDescriptors=rJSONValueParent->Child(L"ObjectDescriptors");
		if (!rObjectDescriptors) {
			cout << "Level ObjectDescriptors definition not found" << endl;
			break;
		} 
		if (!parseObjectDescriptors(rObjectDescriptors)) {
			break;
		}
		// Level
		JSONValue *rLevel=rJSONValueParent->Child(L"Level");
		if (!rLevel) {
			cout << "Level definition not found" << endl;
			break;
		} 
		if (!parseLevel(rLevel)) {
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

bool LevelLoader::parseFramesForTextures(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level FramesForTextures is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rTextureName=extractString(rJSONValue, L"TextureName");
			if (rTextureName.empty()) {
				cout << "FramesForTextures.TextureName not found or empty" << endl;
				continue;
			}
			string rFrameName=extractString(rJSONValue, L"FrameName");
			if (rFrameName.empty()) {
				cout << "FramesForTextures.rFrameName not found or empty" << endl;
				continue;
			}
			int rSourceX=(int)extractNumber(rJSONValue, L"SourceX");
			int rSourceY=(int)extractNumber(rJSONValue, L"SourceY");
			int rWidth=(int)extractNumber(rJSONValue, L"Width");
			int rHeight=(int)extractNumber(rJSONValue, L"Height");

			TextureManager::getInstance().addFrameToTexture(rTextureName, rFrameName, rSourceX, rSourceY, rWidth, rHeight);
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseFramesForSequences(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level FramesForSequences is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rSequenceName=extractString(rJSONValue, L"SequenceName");
			if (rSequenceName.empty()) {
				cout << "FramesForSequences.SequenceName not found or empty" << endl;
				continue;
			}
			JSONValue *rFrames=rJSONValue->Child(L"Frames");
			if (rFrames && rFrames->IsArray()) {
				const JSONArray &rJSONArrayFrames=rFrames->AsArray();
				for (auto *rJSONValueFrame : rJSONArrayFrames) {
					if (!rJSONValueFrame->IsObject()) {
						cout << "FramesForSequences.Frames.Frame not an object" << endl;
						continue;
					}
					string rFrameName=extractString(rJSONValueFrame, L"FrameName");
					if (rFrameName.empty()) {
						cout << "FramesForSequences.Frames.Frame.FrameName not found or empty" << endl;
						continue;
					}
					TextureFrame *rTextureFrame=TextureManager::getInstance().getTextureFrame(rFrameName);
					if (!rTextureFrame) {
						cout << "FramesForSequences.Frames.Frame.FrameName not exists in TextureManager. FrameName: "<< rFrameName << endl;
						continue;
					}
					TextureManager::getInstance().addFrameSequence(rSequenceName)->addTextureFrame(rTextureFrame);
				}
			
			}
		}
		rv=true;
	} while(false);
	return rv;
}


bool LevelLoader::parseObjectDescriptors(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level ObjectDescriptors is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rId=extractString(rJSONValue, L"Id");
			if (rId.empty()) {
				cout << "ObjectDescriptors.Id not found or empty" << endl;
				continue;
			}
			string rObjectType=extractString(rJSONValue, L"ObjectType");
			if (rObjectType.empty()) {
				cout << "ObjectDescriptors.ObjectType not found or empty" << endl;
				continue;
			}
			ObjectType rObjectTypeX;
			if (rObjectType=="Fixed") {
				rObjectTypeX=ObjectType::Fixed;
			} else if (rObjectType=="Gravity") {
				rObjectTypeX=ObjectType::Gravity;
			} else if (rObjectType=="Spike") {
				rObjectTypeX=ObjectType::Spike;
			} else if (rObjectType=="Player") {
				rObjectTypeX=ObjectType::Player;
			} else if (rObjectType=="Enemy") {
				rObjectTypeX=ObjectType::Enemy;
			} else {
				rObjectTypeX=ObjectType::Nothing;
			}

			string rDefaultFrame=extractString(rJSONValue, L"DefaultFrame");
			//if (rDefaultFrame.empty()) {
			//	cout << "ObjectDescriptors.DefaultFrame not found or empty" << endl;
			//	continue;
			//}
			string rDefaultSequence=extractString(rJSONValue, L"DefaultSequence");
			//if (rDefaultSequence.empty()) {
			//	cout << "ObjectDescriptors.DefaultSequence not found or empty" << endl;
			//	continue;
			//}
			ObjectDesc rObjectDesc;
			rObjectDesc.setId(rId);
			rObjectDesc.setObjectType(rObjectTypeX);
			rObjectDesc.setDefaultFrame(rDefaultFrame);
			rObjectDesc.setDefaultSequence(rDefaultSequence);
		
			JSONValue *rObjectSequences=rJSONValue->Child(L"ObjectSequences");
			if (rObjectSequences && rObjectSequences->IsArray()) {
				const JSONArray &rJSONArrayS=rObjectSequences->AsArray();
				for (auto *rJSONValueS : rJSONArrayS) {
					string rObjectSequenceName=extractString(rJSONValueS, L"ObjectSequencesName");
					if (rObjectSequenceName.empty()) {
						cout << "ObjectDescriptors.ObjectSequences.ObjectSequenceName not found or empty" << endl;
						continue;
					}
					string rSequenceName=extractString(rJSONValueS, L"SequenceName");
					if (rSequenceName.empty()) {
						cout << "ObjectDescriptors.ObjectSequences.SequenceName not found or empty" << endl;
						continue;
					}
					
					int rFrameDelayInMs=(int)extractNumber(rJSONValueS, L"FrameDelayInMs");
					ObjectSequence rObjectSequence;

					rObjectSequence.setObjectSequenceName(rObjectSequenceName);
					rObjectSequence.setSequenceName(rSequenceName);
					rObjectSequence.setFrameDelayInMs(rFrameDelayInMs);
					rObjectDesc.addObjectSequence(rObjectSequence);
				}
			}
			mLevel.addObjectDesc(rObjectDesc);
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseLevel(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsObject()) {
			cout << "Level is no Object" << endl;
			break;
		}
		//const JSONObject &rJSONObject=rJSONValueParent->AsObject();
		string rBackgroundTextureName=extractString(rJSONValueParent, L"BackgroundTextureName");
		if (rBackgroundTextureName.empty()) {
			cout << "Level.BackgroundTextureName not found or empty" << endl;
			break;;
		}
		mLevel.setBackgroundImageName(rBackgroundTextureName);

		JSONValue *rBlocks=rJSONValueParent->Child(L"Blocks");
		if (!rBlocks || !rBlocks->IsArray()) {
			cout << "Level.Blocks not found or not an Array" << endl;
			break;;
		}

		const JSONArray &rJSONArray=rBlocks->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rId=extractString(rJSONValue, L"Id");
			if (rId.empty()) {
				cout << "Lebel.Blocks.Id not found or empty" << endl;
				continue;
			}
			int rRowIndex=(int)extractNumber(rJSONValue, L"RowIndex");
			int rColumnIndex=(int)extractNumber(rJSONValue, L"ColumnIndex");
			string rDirection=extractString(rJSONValue, L"Direction");


			Direction rDirectionX;
			if (rDirection=="Left") {
				rDirectionX=Direction::Left;
			} else if (rDirection=="Right") {
				rDirectionX=Direction::Right;
			} else if (rDirection=="Top") {
				rDirectionX=Direction::Top;
			} else if (rDirection=="Bottom") {
				rDirectionX=Direction::Bottom;
			} else {
				rDirectionX=Direction::Default;
			}
			if (!mLevel.isRowAvailable(rRowIndex)) {
				cout << "Lebel.Blocks.RowIndex "<<rRowIndex<< " is out of range" << endl;
				continue;
			}
			Row &rRow=mLevel.getRow(rRowIndex);
			if (!rRow.isColumnAvailable(rColumnIndex)) {
				cout << "Lebel.Blocks.ColumnIndex "<<rColumnIndex<< " is out of range" << endl;
				continue;
			}
			Column &rColumn=rRow.getColumn(rColumnIndex);
			rColumn.setDirection(rDirectionX);
			if (!mLevel.isObjectDescAvailable(rId)) {
				cout << "Lebel.Blocks.Id "<<rId<< " is not available in section ObjectDescriptors.Id" << endl;
				continue;
			}
			ObjectDesc& rObjectDesc=mLevel.getObjectDesc(rId);
			rColumn.setObjectDesc(&rObjectDesc);
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


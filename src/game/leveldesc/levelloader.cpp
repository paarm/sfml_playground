#include "levelloader.h"
#include "../../engine/texturemanager.h"

using namespace std;
bool LevelLoader::loadLevel(const string &rLevelName) {
	bool rv=false;

	mLevel.clearLevel();
	rv=parse(rLevelName);
	
#if 0
	for (int y=0, countY=GameDirector::getInstance().getBlockCountY();y<countY;y++) {
		Row &rRow=mLevel.addRow();
		for (int x=0, countX=GameDirector::getInstance().getBlockCountX();x<countX;x++) {
			rRow.addColumn();
		}
	}
	rv=parse(rLevelName);
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
			string rObjectDescId=extractString(rJSONValue, L"ObjectDescId");
			if (rObjectDescId.empty()) {
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
			rObjectDesc.setId(rObjectDescId);
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
		JSONValue *rLayers=rJSONValueParent->Child(L"Layers");
		if (!rLayers || !rLayers->IsArray()) {
			cout << "Level.Layers not found or not an Array" << endl;
			break;;
		}
		const JSONArray &rJSONArray=rLayers->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rLayerName=extractString(rJSONValue, L"LayerName");
			if (rLayerName.empty()) {
				cout << "Lebel.Layers.LayerName not found or empty" << endl;
				continue;
			}
			int rZOrder=(int)extractNumber(rJSONValue, L"ZOrder");
			int rGridX=(int)extractNumber(rJSONValue, L"GridX");
			int rGridY=(int)extractNumber(rJSONValue, L"GridY");
			if (rGridX<1) {
				rGridX=1;
			}
			if (rGridY<1) {
				rGridY=1;
			}
			Layer &rLayer=mLevel.getOrAddLayer(rLayerName);
			JSONValue *rLayerObjects=rJSONValue->Child(L"LayerObjects");
			if (!rLayerObjects || !rLayerObjects->IsArray()) {
				cout << "Level.Layer.LayerObjects not found or not an Array" << endl;
				break;;
			}
			const JSONArray &rLayerObjectsArray=rLayerObjects->AsArray();
			for (auto *rJSONValueLayerObject : rLayerObjectsArray) {
				string rObjectName=extractString(rJSONValueLayerObject, L"ObjectName");
				if (rObjectName.empty()) {
					cout << "Level.Layer.LayerObjects.ObjectName not found or empty" << endl;
					continue;
				}
				string rObjectDescId=extractString(rJSONValueLayerObject, L"ObjectDescId");
				if (rObjectDescId.empty()) {
					cout << "Level.Layer.LayerObjects.ObjectDescId not found or empty" << endl;
					continue;
				}
				if (!mLevel.isObjectDescAvailable(rObjectDescId)) {
					cout << "Level.Layer.LayerObjects.ObjectDescId " << rObjectDescId <<" is not available" << endl;
					continue;
				}

				int rGridPosX=(int)extractNumberNegIfNotFound(rJSONValueLayerObject, L"GridPosX");
				int rGridPosY=(int)extractNumberNegIfNotFound(rJSONValueLayerObject, L"GridPosY");
				int rPosX=(int)extractNumberNegIfNotFound(rJSONValueLayerObject, L"PosX");
				int rPosY=(int)extractNumberNegIfNotFound(rJSONValueLayerObject, L"PosY");
				if (rPosX==-1 && rGridPosX==-1) {
					cout << "Level.Layer.LayerObjects.PosX or GridPosX not found" << endl;
					continue;
				}
				if (rPosY==-1.0 && rGridPosY==-1.0) {
					cout << "Level.Layer.LayerObjects.PosY or GridPosY not found" << endl;
					continue;
				}
				int realPosX=0;
				if (rPosX!=-1) {
					realPosX=rPosX;	
				} else {
					realPosX=rGridPosX*rGridX;
				}
				int realPosY=0;
				if (rPosY!=-1) {
					realPosY=rPosY;	
				} else {
					realPosY=rGridPosY*rGridY;
				}
				LayerObject rLayerObject;
				rLayerObject.setName(rObjectName);
				rLayerObject.setObjectDescId(rObjectDescId);
				rLayerObject.setPosX(realPosX);
				rLayerObject.setPosY(realPosY);
				rLayer.addLayerObject(rLayerObject);
			}
		}
		rv=true;
	} while(0);
	return rv;
}


const string LevelLoader::extractString(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsString()) {
		return string(v->AsStringPtr()->begin(), v->AsStringPtr()->end());
	}
	return "";
}

float LevelLoader::extractNumber(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsNumber()) {
		return (float)v->AsNumber();
	}
	return 0.0;
}

float LevelLoader::extractNumberNegIfNotFound(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsNumber()) {
		return (float)v->AsNumber();
	}
	return -1.0;
}


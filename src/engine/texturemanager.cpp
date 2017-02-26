#include "texturemanager.h"

TextureManager::TextureManager() {
	//mTextureMap = new map<string, SDL_Texture*>();
}

TextureManager::~TextureManager() {
	reset();
}

void TextureManager::reset() {
	mAliasMap.clear();
	mFrameMap.clear();
	mFrameSequenceMap.clear();
	for(const auto &it : mTextureMap) {
		std::cout << "Texture destroyed: " << it.first << std::endl;
		delete it.second;
	}
	mTextureMap.clear();
}
#if 0
bool TextureManager::proofSDLRenderer() {
	if (!mSDL_Renderer) {
		std::cout << "SDL_Renderer not available. Set a SDL renderer with setSDLRenderer" << std::endl;
		return false;
	}
	return true;
}
#endif
#if 0
void TextureManager::queryTextureSizeAndFormat(SDL_Texture *rSDL_Texture, int *dx, int *dy, Uint32 *format) {
	int 	access;
	if (rSDL_Texture) {
		SDL_QueryTexture(rSDL_Texture, format, &access, dx, dy);
	} else {
		(*dx)=0;
		(*dy)=0;
	}
}
void TextureManager::queryTextureSize(sf::Texture *rSDL_Texture, int *dx, int *dy) {
	if (rSDL_Texture) {
		(*dx)=rSDL_Texture->getSize().x;
		(*dy)=rSDL_Texture->getSize().y;
	}
}
#endif


sf::Texture *TextureManager::loadTexture(const string &pathName, const string& alias) {
	sf::Texture *rv=nullptr;
	rv=TextureManager::getTexture(pathName);
	if (!rv) {
		rv=new sf::Texture();
		if (rv->loadFromFile(pathName.c_str())) {
			std::cout << "Load Texture: " << pathName;
			if (!alias.empty()) {
				std::cout << " as alias: " <<alias << std::endl;
			} else {
				std::cout << std::endl;
			}
			mTextureMap[pathName]=rv;
			if (!alias.empty()) {
				mAliasMap[alias]=pathName;
			}
			// add the default frame with the full texture for the texture path or if present for the alias name 
			addFrameToTexture(pathName, !alias.empty()?alias: pathName, 0, 0, (int)rv->getSize().x, (int)rv->getSize().y);
		}
	} else if (!alias.empty()) {
		// texture already exists. Add only the alias if not exist anymore
		std::cout << "Reuse already loaded Texture: " << pathName << " as alias: " << alias << std::endl;
		if (mAliasMap.find(alias)!=mAliasMap.end()) {
			mAliasMap[alias]=pathName;
			// add the default frame with the full texture for the texture path or if present for the alias name 
			addFrameToTexture(pathName, alias, 0, 0, (int)rv->getSize().x, (int)rv->getSize().y);
		}
	}
	return rv;
}

string* TextureManager::getPathNameFromAlias(const string *alias) {
	string *rv=nullptr;
	if (alias) {
		const auto& itAlias=mAliasMap.find(*alias);
		if (itAlias!=mAliasMap.end()) {
			rv=&itAlias->second;
		}
	}
	return rv;
}

sf::Texture* TextureManager::getTexture(const string &name) {
	sf::Texture *rv=nullptr;
	string *pathName=getPathNameFromAlias(&name);
	if (pathName) {
		const auto &it=mTextureMap.find(*pathName);
		if (it!=mTextureMap.end()) {
			rv=it->second;
		}
	} else {
		const auto &it=mTextureMap.find(name);
		if (it!=mTextureMap.end()) {
			rv=it->second;
		}
	}
	return rv;
}

TextureFrame *TextureManager::getTextureFrame(const string &frameName) {
	const auto &it=mFrameMap.find(frameName);
	if (it==mFrameMap.end()) {
		string* pathName=getPathNameFromAlias(&frameName);
		if (pathName) {
			const auto &it2=mFrameMap.find(frameName);
			if (it2!=mFrameMap.end()) {
				return &it2->second;
			}	
		}
	} else {
		return &it->second;
	}
	return nullptr;
}

FrameSequence *TextureManager::createAutomaticFramesAndSequence(const string &textureName, const string &rSequenceName, int xFrameCount, int yFrameCount) {
	if (isFrameSequenceExist(rSequenceName)) {
		return getFrameSequence(rSequenceName);
	}
	int frameCount=addFramesAutomaticallyToTexture(textureName, rSequenceName, xFrameCount, yFrameCount);
	return addAutomaticFramesToFrameSequence(rSequenceName, frameCount);
}

int TextureManager::addFramesAutomaticallyToTexture(const string &textureName, const string &rSequenceName, int xFrameCount, int yFrameCount) {
	sf::Texture* rSDL_Texture=getTexture(textureName);
	int frameCount=0;
	if (rSDL_Texture && xFrameCount>0 && yFrameCount>0) {
		int textX=0;
		int textY=0;
		textX=(int)rSDL_Texture->getSize().x;
		textY=(int)rSDL_Texture->getSize().y;

		int xFrameSize=textX/xFrameCount;
		int yFrameSize=textY/yFrameCount;

		for (int y=0;y<yFrameCount;y++) {
			for (int x=0;x<xFrameCount;x++) {
				string frameName=rSequenceName;
				frameName+=to_string(frameCount);
				addFrameToTexture(textureName, frameName, x*xFrameSize, y*yFrameSize, xFrameSize, yFrameSize);
				frameCount++;
			}
		}
	}
	return frameCount;		
}
FrameSequence *TextureManager::addAutomaticFramesToFrameSequence(const string& rSequenceName, int count) {
	FrameSequence *rv=addFrameSequence(rSequenceName);
	if (rv) {
		for (int i=0;i<count;i++) {
			string frameName=rSequenceName;
			frameName+=to_string(i);
			TextureFrame *textureFrame=getTextureFrame(frameName);
			if (textureFrame) {
				rv->addTextureFrame(textureFrame);			
			} else {
				break;
			}
		}
	}
	return rv;
}


TextureFrame *TextureManager::addFrameToTexture(const string &textureName, const string &frameName, int x, int y, int dx, int dy) {
	TextureFrame*rv=nullptr;
	sf::Texture* rSDL_Texture=getTexture(textureName);
	if (rSDL_Texture) {
		rv=getTextureFrame(frameName);
		if (!rv) {
			int textX=0;
			int textY=0;
			textX=(int)rSDL_Texture->getSize().x;
			textY=(int)rSDL_Texture->getSize().y;
			
			Frame rFrame;
			rFrame.x=x;
			rFrame.y=y;
			rFrame.dx=dx;
			rFrame.dy=dy;
			keepSourceInTextureRange(rFrame, textX, textY);
			TextureFrame rTextureFrame;
			rTextureFrame.mFrameName=frameName;
			rTextureFrame.mFrame=rFrame;
			rTextureFrame.mSDL_Texture=rSDL_Texture;
			mFrameMap[frameName]=rTextureFrame;
			rv=&mFrameMap[frameName];
		}
	}
	return rv;
}

FrameSequence *TextureManager::addFrameSequence(const string& rSequenceName) {
	const auto &it=mFrameSequenceMap.find(rSequenceName);
	if (it!=mFrameSequenceMap.end()) {
		return &it->second;
	}
	FrameSequence rFrameSequence;
	mFrameSequenceMap[rSequenceName]=rFrameSequence;
	return &mFrameSequenceMap[rSequenceName];
}
bool TextureManager::isFrameSequenceExist(const string &rSequenceName) {
	bool rv=false;
	const auto &it=mFrameSequenceMap.find(rSequenceName);
	if (it!=mFrameSequenceMap.end()) {
		rv=true;
	}
	return rv;
}
FrameSequence *TextureManager::getFrameSequence(const string& rSequenceName) {
	const auto &it=mFrameSequenceMap.find(rSequenceName);
	if (it!=mFrameSequenceMap.end()) {
		return &it->second;
	}
	return nullptr;
}

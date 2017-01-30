#pragma once
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;


struct Frame {
    int x;
    int y;
    int dx;
    int dy;
};

struct TextureFrame {
	Frame 		mFrame;
	sf::Texture *mSDL_Texture;
};

class FrameSequence {
private:
    vector<TextureFrame*> mTextureFrame;

public:
    FrameSequence() {}
    FrameSequence* addTextureFrame(TextureFrame *f) {
      mTextureFrame.push_back(f);
	  return this;
	}
    TextureFrame* getTextureFrame(int rIndex) {
        return mTextureFrame[rIndex];
    }
	int countFrames () {
        return mTextureFrame.size();
    }
};


class TextureManager {
private:
	map<string, sf::Texture*> mTextureMap;
	map<string, string> mAliasMap;
	map<string, TextureFrame>mFrameMap;
    map<string,	FrameSequence> mFrameSequenceMap;
	
	sf::Texture *mSDL_Renderer;
	TextureManager();
	bool proofSDLRenderer();
	string* getPathNameFromAlias(const string *alias);
	void addDefaultFrame(const string &pathName, const string *&alias, sf::Texture* rSDL_Texture);
	void keepSourceInTextureRange(Frame &rFrame, int textX, int textY) {
		rFrame.x=max(0,rFrame.x);
		rFrame.x=min(textX,rFrame.x);
		rFrame.y=max(0,rFrame.y);
		rFrame.y=min(textY,rFrame.y);
		// todo: proof dx, dy
	}	
public:
	//void queryTextureSizeAndFormat(sf::Texture *rSDL_Texture, int *dx, int *dy, Uint32 *format);
	static TextureManager & getInstance() {
		static TextureManager instance;
		return instance;
	}
	~TextureManager();
	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;
	sf::Texture* loadTexture(const string &pathName, const char*alias);
	sf::Texture* loadTexture(const string &pathName, const string* alias=nullptr);
	sf::Texture* getTexture(const string &name);

	TextureFrame *getTextureFrame(const string &frameName) {
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

	FrameSequence *createAutomaticFramesAndSequence(const string &textureName, const string &sequenceName, int xFrameCount, int yFrameCount=1) {
		int frameCount=addFramesAutomaticallyToTexture(textureName, sequenceName, xFrameCount, yFrameCount);
		return addAutomaticFramesToFrameSequence(sequenceName, frameCount);
	}

	int addFramesAutomaticallyToTexture(const string &textureName, const string &sequenceName, int xFrameCount, int yFrameCount=1) {
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
					string frameName=sequenceName;
					frameName+=to_string(frameCount);
					addFrameToTexture(textureName, frameName, x*xFrameSize, y*yFrameSize, xFrameSize, yFrameSize);
					frameCount++;
				}
			}
		}
		return frameCount;		
	}
    FrameSequence *addAutomaticFramesToFrameSequence(const string& rSequenceName, int count) {
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
	

	TextureFrame *addFrameToTexture(const string &textureName, const string &frameName, int x, int y, int dx, int dy) {
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
				rTextureFrame.mFrame=rFrame;
				rTextureFrame.mSDL_Texture=rSDL_Texture;
				mFrameMap[frameName]=rTextureFrame;
				rv=&mFrameMap[frameName];
			}
		}
		return rv;
	}

	

    FrameSequence *addFrameSequence(const string& rSequenceName) {
        const auto &it=mFrameSequenceMap.find(rSequenceName);
        if (it!=mFrameSequenceMap.end()) {
            return &it->second;
        }
        FrameSequence rFrameSequence;
        mFrameSequenceMap[rSequenceName]=rFrameSequence;
        return &mFrameSequenceMap[rSequenceName];
    }
    bool isFrameSequenceExist(const string &rSequenceName) {
        bool rv=false;
        const auto &it=mFrameSequenceMap.find(rSequenceName);
        if (it!=mFrameSequenceMap.end()) {
            rv=true;
        }
        return rv;
    }
    FrameSequence *getFrameSequence(const string& rSequenceName) {
        const auto &it=mFrameSequenceMap.find(rSequenceName);
        if (it!=mFrameSequenceMap.end()) {
            return &it->second;
        }
        return nullptr;
    }
};
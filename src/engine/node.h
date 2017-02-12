#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "texturemanager.h"
#include "fontmanager.h"

using namespace std;

enum class NodeType{
	Node=0,
	Sprite,
	Text,
	Player,
	Root
};

class NodeBase {
public:
	virtual NodeType getNodeType()=0;
	virtual void setTextureFrame(TextureFrame *rTextureFrame)=0;
protected:
};

class Node : NodeBase {
private:
	Node 	*mParentNode=nullptr;
	string 	mName;
	bool 	mScheduledUpdate=false;
	void initChildNodeList();
protected:
	vector<Node*> *mChildNodes=nullptr;
	vector<Node*> *mChildToDelete=nullptr;

	virtual void	drawInternal(sf::RenderTarget& target, const sf::Transform& parentTransform);
	virtual void 	updateInternal(double deltaTime);

	virtual void doUpdate(double deltaTime);
	virtual void doDestroy();
public:
	Node(const string &name);
	Node(const char *name);
	Node();
	virtual ~Node();
	Node* addNode(Node *n);
	void deleteMeScheduled();
	void scheduleChildToDelete(Node *rToDeleteNode);
	void deleteNode(Node *n);
	void deleteChilds();
	Node* searchNode(const string&name, bool searchInSub=false);
	void debugPrint();
	void scheduleUpdate(bool rScheduleUpdate) {
		mScheduledUpdate=rScheduleUpdate;
	}
	virtual NodeType getNodeType() override { return NodeType::Node;}
	virtual void setTextureFrame(TextureFrame *rTextureFrame) override {}
};

enum class PlayerType {
	None=0,
	ForwardLoop,
	BackwardLoop,
	Forward,
	Backward
};

class FramePlayer;


class NodeRoot : public Node {
private:
public:
	virtual NodeType getNodeType() override { return NodeType::Root;}
	void	executeDraw(sf::RenderTarget& target, const sf::Transform& parentTransform) {
		drawInternal(target, parentTransform);
	}
	void 	executeUpdate(double deltaTime) {
		updateInternal(deltaTime);
	}
};

class NodeText : public Node {
private:
	sf::Text		mText;
	FontContext		*mFontContext=nullptr;
protected:
	//virtual void updateInternal(double deltaTime) override;
	virtual void drawInternal(sf::RenderTarget& target, const sf::Transform& parentTransform) override;
public:
	NodeText(const string &name, FontContext *rFontContext, int x, int y);
	void setText(const string &rText) {
		mText.setString(rText);
	}

	double getX();
	double getY();
	//int	getWidth();
	//int	getHeight();
	void setPosition(double, double);
	void setPositionRelative(double distanceX, double distanceY);
	void setScale(double, double);
	void scaleRelative(double, double);
	void setRotation(double);
	void setRotationRelative(double rAddAngle);
	double getAngle();


	virtual NodeType getNodeType() override { return NodeType::Text;}
};

class Node2d : public Node {
private:
	sf::Sprite		mSprite;
	TextureFrame *mTextureFrame=nullptr;
	FramePlayer  *mFramePlayer=nullptr;
	//vector<TextureFrame*> mTextureFrames;	
protected:
	virtual void updateInternal(double deltaTime) override;
	virtual void drawInternal(sf::RenderTarget& target, const sf::Transform& parentTransform) override;
public:
	Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y);
	Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y, int w, int h);
	double getX();
	double getY();
	int	getWidth();
	int	getHeight();
	void setPosition(double, double);
	void setPositionRelative(double distanceX, double distanceY);
	void setScale(double, double);
	void scaleRelative(double, double);
	void setRotation(double);
	void setRotationRelative(double rAddAngle);
	double getAngle();

	virtual NodeType getNodeType() override { return NodeType::Sprite;}
	void setTextureFrame(TextureFrame*) override;
	void activateFramePlayer(const string &name, double rDelayInMs=-1.0, PlayerType playerType=PlayerType::None);
	bool isFramePlayerFinish();
	void deactivateFramePlayer();
};


class FramePlayer : public Node {
private:
	PlayerType		mPlayerType;
	int				mCurrentFrame=0;
	double			mDelayInMs;
	FrameSequence 	*mFrameSequence;
	TextureFrame	*mCurrentTextureFrame=nullptr;
	double 			mCurrentDelayTime=0.0;
	bool			mIsFinish=false;
public:
	FramePlayer(const string& name, FrameSequence *rFrameSequence, double rDelayInMs, PlayerType playerType=PlayerType::ForwardLoop) : Node(name) {
		mPlayerType=playerType;
		mFrameSequence=rFrameSequence;
		mDelayInMs=rDelayInMs;
		mCurrentTextureFrame=mFrameSequence->getTextureFrame(mCurrentFrame);
		resetPlayer();
	}	
	void resetPlayer() {
		if (mPlayerType==PlayerType::Backward) {
			mCurrentFrame=getLastFrameIndex();
		} else {
			mCurrentFrame=0;
		}
		mIsFinish=false;
		mCurrentDelayTime=0.0;
	}
	void setPlayerType(PlayerType rPlayerType) {
		mPlayerType=rPlayerType;
	}
	void setDelayInMs(double rDelayInMs) {
		mDelayInMs=rDelayInMs;
	}

	void addFrame() {
		if (mCurrentFrame<mFrameSequence->countFrames()-1) {
			mCurrentFrame++;
		} else {
			mIsFinish=true;
		}
	}
	void subFrame() {
		if (mCurrentFrame>0) {
			mCurrentFrame--;
		} else {
			mIsFinish=true;
		}
	}
	void addFrameLoop() {
		mCurrentFrame++;
		if (mCurrentFrame>=mFrameSequence->countFrames()) {
			mCurrentFrame=0;
		}
	}
	void subFrameLoop() {
		mCurrentFrame--;
		if (mCurrentFrame<0) {
			mCurrentFrame=mFrameSequence->countFrames()-1;
		}
	}
	int getLastFrameIndex() {
		return mFrameSequence->countFrames()-1;
	}
	void updateFramePlayer(double deltaTime) {
		if (!mIsFinish) {
			mCurrentDelayTime+=deltaTime;
			if (mCurrentDelayTime>mDelayInMs) {
				mCurrentDelayTime=0.0;
				if (mPlayerType==PlayerType::ForwardLoop) {
					addFrameLoop();
				} else if (mPlayerType==PlayerType::BackwardLoop) {
					subFrameLoop();
				} else if (mPlayerType==PlayerType::Forward) {
					addFrame();
				} else if (mPlayerType==PlayerType::Backward) {
					subFrame();
				}
			}
		} 
	}
	bool isFinish() {
		return mIsFinish;
	}
	TextureFrame* getCurrentTextureFrame() {
		return mFrameSequence->getTextureFrame(mCurrentFrame);
	}
	virtual NodeType getNodeType() override { return NodeType::Player;}
	
};
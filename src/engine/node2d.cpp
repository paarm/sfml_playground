#include "node.h"

Node2d::Node2d(const string &name, int x, int y) : Node(name) {
	setPosition((float)x,(float)y);
	setScale(1.0,1.0);
	//setTextureFrame(rTextureFrame);
}  

Node2d::Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y) : Node(name) {
	setPosition((float)x,(float)y);
	setScale(1.0,1.0);
	setTextureFrame(rTextureFrame);
}  

Node2d::Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y, int w, int h) : Node2d(name, rTextureFrame, x, y) {
	setScale((float)w/(float)mTextureFrame->mFrame.dx,(float)h/(float)mTextureFrame->mFrame.dy);
}

/*
void Node2d::keepSourceInTextureRange() {
	mSourceRect.x=max(0,mSourceRect.x);
	mSourceRect.x=min(mTextureSize.x,mSourceRect.x);
	mSourceRect.y=max(0,mSourceRect.y);
	mSourceRect.y=min(mTextureSize.y,mSourceRect.y);
}

void Node2d::queryTextureSize() {
	Uint32 	format;
	int 	access;
	if (mSDL_Texture) {
		SDL_QueryTexture(mSDL_Texture, &format, &access, &mTextureSize.x, &mTextureSize.y);
	} else {
		mTextureSize.x=0;
		mTextureSize.y=0;
	}
}
*/
void Node2d::setTextureFrame(TextureFrame *rTextureFrame) {
	if (rTextureFrame!=nullptr) {
		if (mTextureFrame==nullptr || mTextureFrame->mSDL_Texture==nullptr || rTextureFrame->mSDL_Texture!=mTextureFrame->mSDL_Texture) {
			mSprite.setTexture(*rTextureFrame->mSDL_Texture);
		}
	}
	mTextureFrame=rTextureFrame;
}

void Node2d::drawInternal(sf::RenderTarget& target, const sf::Transform& parentTransform) {
	//mSprite.setOrigin(0, 0);//mSprite.getPosition().x+mTextureFrame->mFrame.dx/2.0, mSprite.getPosition().y+mTextureFrame->mFrame.dy/2.0);
 	//if (mTextureFrame) {
		if (mTextureFrame) {
			mSprite.setTextureRect(sf::IntRect(mTextureFrame->mFrame.x, mTextureFrame->mFrame.y, mTextureFrame->mFrame.dx, mTextureFrame->mFrame.dy));
		}
		sf::Transform combinedTransform = parentTransform * mSprite.getTransform();
#if 0
		mDestRect.x=mX;
		mDestRect.y=mY;
		mDestRect.h=mH;
		mDestRect.w=mW;
#endif		

/*		mSourceRect.x=mTextureFrame->mFrame.x;
		mSourceRect.y=mTextureFrame->mFrame.y;
		mSourceRect.h=mTextureFrame->mFrame.dx;
		mSourceRect.w=mTextureFrame->mFrame.dy;
*/
		target.draw(mSprite, parentTransform);
#if 0
		if (mAngle==0.0) {
			SDL_RenderCopy(rSDL_Renderer, mTextureFrame->mSDL_Texture, &mSourceRect, &mDestRect);
		} else {
			SDL_RenderCopyEx(rSDL_Renderer, mTextureFrame->mSDL_Texture, &mSourceRect, &mDestRect, mAngle, nullptr, SDL_FLIP_NONE);
		}
#endif		
	//}
	// draw the childs
	Node::drawInternal(target, combinedTransform);
}

void Node2d::updateInternal(float deltaTime) {
	if (mFramePlayer) {
		mFramePlayer->updateFramePlayer(deltaTime);
		setTextureFrame(mFramePlayer->getCurrentTextureFrame());
	}
	Node::updateInternal(deltaTime);
}

void Node2d::activateFramePlayer(const string &name, float rDelayInMs, PlayerType playerType) {
	Node *n=searchNode(name);
	if (n && n->getNodeType()==NodeType::Player) {
		mFramePlayer=(FramePlayer*)n;
		if (rDelayInMs!=-1.0) {
			mFramePlayer->setDelayInMs(rDelayInMs);
		}
		if (playerType!=PlayerType::None) {
			mFramePlayer->setPlayerType(playerType);
		}
		mFramePlayer->resetPlayer();
	}
}

bool Node2d::isFramePlayerFinish() {
	bool rv=true;
	if (mFramePlayer) {
		rv=mFramePlayer->isFinish();
	}
	return rv;
}
void Node2d::deactivateFramePlayer() {
	mFramePlayer=nullptr;
}

void Node2d::setPositionRelative(float distanceX, float distanceY) {
	mSprite.move(sf::Vector2f(distanceX,distanceY));
}


void Node2d::setPosition(float rX, float rY) {
	mSprite.setPosition(sf::Vector2f(rX,rY));
}

float Node2d::getX() {
	return mSprite.getPosition().x;
}

float Node2d::getY() {
	return mSprite.getPosition().y;
}

int Node2d::getHeight() {
	if (mTextureFrame) {
		return (int)(mTextureFrame->mFrame.dx*mSprite.getScale().x);
	}
	return 0;
}

int Node2d::getWidth() {
	if (mTextureFrame) {
		return (int)(mTextureFrame->mFrame.dy*mSprite.getScale().y);
	}
	return 0;
}

void Node2d::setScale(float rScaleX, float rScaleY) {
	mSprite.setScale(sf::Vector2f(rScaleX, rScaleY));
}

void Node2d::scaleRelative(float rScaleX, float rScaleY) {
	mSprite.scale(sf::Vector2f(rScaleX, rScaleY));
}

void Node2d::setRotation(float rAngle) {
	mSprite.setRotation(rAngle);
}

float Node2d::getAngle() {
	return mSprite.getRotation();
}

void Node2d::setRotationRelative(float rAngle) {
	mSprite.rotate(rAngle);
}

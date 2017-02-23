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
	if (mTextureFrame) {
		int x=mTextureFrame->mFrame.x;
		int y=mTextureFrame->mFrame.y;
		int dx=mTextureFrame->mFrame.dx;
		int dy=mTextureFrame->mFrame.dy;
		if (mFlipX) {
			x=x+dx;
			dx=-dx;
		}
		if (mFlipY) {
			y=y+dy;
			dy=-dy;
		}
		mSprite.setTextureRect(sf::IntRect(x, y, dx, dy));
	}
	sf::Transform combinedTransform = parentTransform * mSprite.getTransform();
	target.draw(mSprite, parentTransform);
	// draw the childs with the combinedTransform matrix as parent matrix
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

void Node2d::setOriginMiddle() {
	mSprite.setOrigin(sf::Vector2f(getWidth()/2, getHeight()/2));
}

void Node2d::setOriginFactor(float rOriginFactorX, float rOriginFactorY) {
	mSprite.setOrigin(sf::Vector2f(getWidth()*rOriginFactorX, getHeight()*rOriginFactorY));
}
float Node2d::getAngle() {
	return mSprite.getRotation();
}

void Node2d::setRotationRelative(float rAngle) {
	mSprite.rotate(rAngle);
}

void Node2d::flipX(bool rFlipX) {
	mFlipX=rFlipX;
}

void Node2d::flipY(bool rFlipY) {
	mFlipY=rFlipY;
}
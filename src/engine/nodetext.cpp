#include "node.h"

NodeText::NodeText(const string &name, FontContext *rFontContext, int x, int y) : Node(name) {
	setPosition(x,y);
	setScale(1.0,1.0);
    mText.setString("");
    mText.setColor(sf::Color::Red);
    mText.setCharacterSize(24); // in pixels, not points!
	mText.setFont(rFontContext->mFont);
    mFontContext=rFontContext;
}

void NodeText::drawInternal(sf::RenderTarget& target, const sf::Transform& parentTransform) {
	//mText.setTextureRect(sf::IntRect(mTextureFrame->mFrame.x, mTextureFrame->mFrame.y, mTextureFrame->mFrame.dx, mTextureFrame->mFrame.dy));
	sf::Transform combinedTransform = parentTransform * mText.getTransform();
	target.draw(mText, parentTransform);
	Node::drawInternal(target, combinedTransform);
}


void NodeText::setPositionRelative(double distanceX, double distanceY) {
	mText.move(sf::Vector2f(distanceX,distanceY));
}


void NodeText::setPosition(double rX, double rY) {
	mText.setPosition(sf::Vector2f(rX,rY));
}

double NodeText::getX() {
	return mText.getPosition().x;
}

double NodeText::getY() {
	return mText.getPosition().y;
}
#if 0
int NodeText::getHeight() {
	return mTextureFrame->mFrame.dx*mText.getScale().x;
}

int NodeText::getWidth() {
	return mTextureFrame->mFrame.dy*mText.getScale().y;
}
#endif

void NodeText::setScale(double rScaleX, double rScaleY) {
	mText.setScale(sf::Vector2f(rScaleX, rScaleY));
}

void NodeText::scaleRelative(double rScaleX, double rScaleY) {
	mText.scale(sf::Vector2f(rScaleX, rScaleY));
}

void NodeText::setRotation(double rAngle) {
	mText.setRotation(rAngle);
}

double NodeText::getAngle() {
	return mText.getRotation();
}

void NodeText::setRotationRelative(double rAngle) {
	mText.rotate(rAngle);
}

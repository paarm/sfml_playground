#pragma once
#include <vector>
#include "../engine/node.h"
#include "gamedirector.h"

using namespace std;

class ObjectBase {
private:
protected:
	Node2d		*mNode2d;
	sf::FloatRect mLocation;
public:
	sf::FloatRect getLocalBounds() {
		if (mNode2d) {
			return mNode2d->getLocalBounds();
		}
		return sf::FloatRect();
	}
	sf::FloatRect& getLocation() {
		if (mNode2d) {
			mLocation.left=mNode2d->getX();
			mLocation.top=mNode2d->getY();
			mLocation.width=mNode2d->getWidth();
			mLocation.height=mNode2d->getHeight();
		} else {
			mLocation.left=0.0;
			mLocation.top=0.0;
			mLocation.width=0.0;
			mLocation.height=0.0;
		}
		return mLocation;
	}
};

class FixedObject : public ObjectBase {
private:
public:
	FixedObject(Node2d *rNode2d) {
		mNode2d=rNode2d;
	}
	void setNode2d(Node2d* rNode2d) {
		mNode2d=rNode2d;
	}
};

enum class Gravity {
	Bottom,
	Top
};

enum class Direction {
	Right,
	Left
};

class ObjectMoveable : public ObjectBase {
private:
	Gravity			mGravity;
	Direction		mDirection;
	bool 			mTouchGround;
	float			mLastDeltaX=0.0;
	float			mLastDeltaY=0.0;
public:	
	void setNode2d(Node2d* rNode2d) {
		mNode2d=rNode2d;
	}
	void moveLeftRight(float rDeltaTime, float rSpeedPerSecond) {
		mLastDeltaX=0.0;
		if (mNode2d) {
			if (mTouchGround) {
				float rDeltaX=(float)rSpeedPerSecond*rDeltaTime/(float)1000.0;
				if (mDirection==Direction::Left) {
					mNode2d->setFlipX(true);
					rDeltaX=-rDeltaX;
				} else {
					mNode2d->setFlipX(false);
				}
				if (rDeltaX!=0.0) {
					mLastDeltaX=rDeltaX;
					//mNode2d->setPositionRelative(rDeltaX, 0.0);
				}
			}
		}
	}
	void revertLeftRightMove() {
		if (mNode2d) {
			mNode2d->setPositionRelative(-mLastDeltaX, 0.0);
		}	
	}
	void commitLeftRightMove(float correctDeltaX) {
		if (mNode2d) {
			if (mLastDeltaX>0.0) {
				mNode2d->setPositionRelative(mLastDeltaX-correctDeltaX, 0.0);
			} else {
				mNode2d->setPositionRelative(mLastDeltaX+correctDeltaX, 0.0);
			}
		}	
	}
	void moveTopBottom(float rDeltaTime, float rSpeedPerSecond) {
		if (mNode2d) {
			//if (!mTouchGround) {
				float rDeltaY=(float)rSpeedPerSecond*rDeltaTime/(float)1000.0;
				if (mGravity==Gravity::Top) {
					rDeltaY=-rDeltaY;
				}
				if (rDeltaY!=0.0) {
					mLastDeltaY=rDeltaY;
					//mNode2d->setPositionRelative(0.0, rDeltaY);
				}
			//}
		}
	}

	void revertTopBottomMove() {
		if (mNode2d) {
			mNode2d->setPositionRelative(0.0, -mLastDeltaY);
		}	
	}
	void commitTopBottomMove(float correctDeltaY) {
		if (mNode2d) {
			if (mLastDeltaY>0.0) {
				mNode2d->setPositionRelative(0.0, mLastDeltaY-correctDeltaY);
			} else if (mLastDeltaY<0.0) {
				mNode2d->setPositionRelative(0.0, mLastDeltaY+correctDeltaY);
			}
		}	
	}

	bool changeGravityToTop() {
		bool rGravityChanged=false;
		if (mNode2d && mTouchGround && mGravity==Gravity::Bottom) {
			mNode2d->setFlipY(true);
			mGravity=Gravity::Top;
			mTouchGround=false;
			rGravityChanged=true;
		}
		return rGravityChanged;
	}
	bool changeGravityToBottom() {
		bool rGravityChanged=false;
		if (mNode2d && mTouchGround && mGravity==Gravity::Top) {
			mNode2d->setFlipY(false);
			mGravity=Gravity::Bottom;
			mTouchGround=false;
			rGravityChanged=true;
		}
		return rGravityChanged;
	}
	void setGroundTouched(bool rGroundTouched) {
		mTouchGround=rGroundTouched;
	}
	void setDirection(Direction rDirection) {
		mDirection=rDirection;
	}
	float getLastDeltaX() {
		return mLastDeltaX;
	}
	float getLastDeltaY() {
		return mLastDeltaY;
	}
	void resetMoveDeltas() {
		mLastDeltaX=0.0;
		mLastDeltaY=0.0;
	}

};

class EnemyObject : public ObjectMoveable {
private:
public:
};

class PlayerObject : public ObjectMoveable {
private:
public:
};

class WorldState {
private:
	vector<FixedObject> mFixedObjectList;
	vector<EnemyObject> mEnemyObjectList;
	PlayerObject 		mPlayerObject;
	sf::FloatRect		mIntersectionX;
	sf::FloatRect		mIntersectionY;
public:
	void setPlayerObject(Node2d *rNode2d) {
		mPlayerObject.setNode2d(rNode2d);
	}
	void addFixedObject(Node2d *rNode2d) {
		mFixedObjectList.emplace_back(rNode2d);
	}
	void update(float deltaTime, bool keyLeft, bool keyRight, bool keyUp, bool keyDown, bool keyPick) {
		if (keyLeft || keyRight) {
			if (keyLeft) {
				mPlayerObject.setDirection(Direction::Left);
			}
			if (keyRight) {
				mPlayerObject.setDirection(Direction::Right);
			}
			mPlayerObject.moveLeftRight(deltaTime, GameDirector::getInstance().getPlayerSpeedLeftRightPerSecond());
		}

		if (keyUp) {
			mPlayerObject.changeGravityToTop();
		} else if (keyDown) {
			mPlayerObject.changeGravityToBottom();
		}
		mPlayerObject.moveTopBottom(deltaTime, GameDirector::getInstance().getPlayerSpeedTopBottomPerSecond());
		correctGroundIntersectForPlayer();
		mPlayerObject.resetMoveDeltas();
	}

	void correctGroundIntersectForPlayer() {
		sf::FloatRect& oldPos=mPlayerObject.getLocation();
		bool rIntersectsY=false;
		if (mPlayerObject.getLastDeltaY()!=0.0) {
			sf::FloatRect newPos=oldPos;
			newPos.top=(newPos.top+mPlayerObject.getLastDeltaY());
			for(FixedObject& rFixedObject: mFixedObjectList) {
				if (rFixedObject.getLocation().intersects(newPos,mIntersectionY)) {
					rIntersectsY=true;
					break;
				}
			}
		}

		bool rIntersectsX=false;
		if (mPlayerObject.getLastDeltaX()!=0.0) {
			sf::FloatRect newPos=oldPos;
			newPos.left=(newPos.left+mPlayerObject.getLastDeltaX());
			for(FixedObject& rFixedObject: mFixedObjectList) {
				if (rFixedObject.getLocation().intersects(newPos,mIntersectionX)) {
					rIntersectsX=true;
					break;
				}
			}
		}

		if (!rIntersectsY) {
			mPlayerObject.commitTopBottomMove(0.0);
			mPlayerObject.setGroundTouched(false);
		} else {
			mPlayerObject.commitTopBottomMove(mIntersectionY.height);
			mPlayerObject.setGroundTouched(true);
		}
		if (!rIntersectsX) {
			mPlayerObject.commitLeftRightMove(0.0);
		} else {
			mPlayerObject.commitLeftRightMove(mIntersectionX.width);
		}
	}
};



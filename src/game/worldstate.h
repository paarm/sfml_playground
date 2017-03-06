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

	bool moveLeftRight(float rDeltaTime, float rSpeedPerSecond, vector<FixedObject>& rFixedObjectList) {
		bool intersected=false;
		mLastDeltaX=0.0;
		if (mNode2d) {
			if (mTouchGround) {
				float rDeltaX=(float)rSpeedPerSecond*rDeltaTime/(float)1000.0;
				if (rDeltaX>GameDirector::getInstance().getBlockSizeX()-2.0) {
					// limit move to something lower thatn 32 so that it cannot breat trought a full block
					rDeltaX=GameDirector::getInstance().getBlockSizeX()-2.0;
				}
				if (mDirection==Direction::Left) {
					mNode2d->setFlipX(true);
					rDeltaX=-rDeltaX;
				} else {
					mNode2d->setFlipX(false);
				}
				if (rDeltaX!=0.0) {
					mLastDeltaX=rDeltaX;

					sf::FloatRect& oldPos=getLocation();
					sf::FloatRect newPos=oldPos;
					sf::FloatRect intersectionRect;
					newPos.left=(newPos.left+getLastDeltaX());
					for(FixedObject& rFixedObject: rFixedObjectList) {
						if (rFixedObject.getLocation().intersects(newPos,intersectionRect)) {
							intersected=true;
							if (getDirection()==Direction::Left) {
								mLastDeltaX+=intersectionRect.width;
							} else {
								mLastDeltaX-=intersectionRect.width;
							}
							newPos.left=oldPos.left;
							newPos.left=(newPos.left+getLastDeltaX());
						}
						if (mLastDeltaX==0.0) {
							// ok nothing more to correct - dont need to check other static objects
							break;
						}
					}
					mNode2d->setPositionRelative(mLastDeltaX, 0.0);
					if (mNode2d->getX()<0.0) {
						mNode2d->setPosition(0.0, mNode2d->getY());
					} else if (mNode2d->getX()+mNode2d->getWidth()>GameDirector::getInstance().getVirtualScreenSizeX()) {
						mNode2d->setPosition(0.0, GameDirector::getInstance().getVirtualScreenSizeX()-mNode2d->getWidth());
					}
				}
			}
		}
		return intersected;
	}

	bool moveTopBottom(float rDeltaTime, float rSpeedPerSecond, vector<FixedObject>& rFixedObjectList) {
		bool intersected=false;
		if (mNode2d) {
			//if (!mTouchGround) {
				float rDeltaY=(float)rSpeedPerSecond*rDeltaTime/(float)1000.0;
				if (rDeltaY>GameDirector::getInstance().getBlockSizeY()-2.0) {
					// limit move to something lower thatn 32 so that it cannot breat trought a full block
					rDeltaY=GameDirector::getInstance().getBlockSizeY()-2.0;
				}
				// move to top direction?
				if (mGravity==Gravity::Top) {
					rDeltaY=-rDeltaY;
				}
				// anyone moved to top or bottom?
				if (rDeltaY!=0.0) {
					mLastDeltaY=rDeltaY;

					sf::FloatRect& oldPos=getLocation();
					sf::FloatRect newPos=oldPos;
					sf::FloatRect intersectionRect;
					newPos.top=(newPos.top+getLastDeltaY());
					// check all static objects for collision now
					for(FixedObject& rFixedObject: rFixedObjectList) {
						if (rFixedObject.getLocation().intersects(newPos,intersectionRect)) {
							// ok have collision - remove the overlap from the move delta and continue with the oder static objects
							intersected=true;
							if (getGravity()==Gravity::Top) {
								mLastDeltaY+=intersectionRect.height;
							} else {
								mLastDeltaY-=intersectionRect.height;
							}
							// continue with a fresh position from the corrected move delta
							newPos.top=oldPos.top;
							newPos.top=(newPos.top+getLastDeltaY());
						}
						if (mLastDeltaY==0.0) {
							// ok nothing more to correct - dont need to check other static objects
							break;
						}
					}
					mNode2d->setPositionRelative(0.0, mLastDeltaY);

					if (!intersected) {
						// rollover to top or bottom of the screen if required
						if (mGravity==Gravity::Top && mNode2d->getY()<0.0-mNode2d->getHeight()) {
							mNode2d->setPosition(mNode2d->getX(), GameDirector::getInstance().getVirtualScreenSizeY()+mNode2d->getHeight());
						} else if (mGravity==Gravity::Bottom && mNode2d->getY()>GameDirector::getInstance().getVirtualScreenSizeY()) {
							mNode2d->setPosition(mNode2d->getX(), 0.0-mNode2d->getHeight());
						}
					}
					if (!intersected) {
						setGroundTouched(false);
					} else {
						// touched, so can move to right or left from now on
						setGroundTouched(true);
					}
				}
			//}
		}
		return intersected;
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
	Direction getDirection() {
		return mDirection;
	}
	Gravity getGravity() {
		return mGravity;
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
	sf::FloatRect		mIntersectionRect;
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
			mPlayerObject.moveLeftRight(deltaTime, GameDirector::getInstance().getPlayerSpeedLeftRightPerSecond(), mFixedObjectList);
		}

		if (keyUp) {
			mPlayerObject.changeGravityToTop();
		} else if (keyDown) {
			mPlayerObject.changeGravityToBottom();
		}
		mPlayerObject.moveTopBottom(deltaTime, GameDirector::getInstance().getPlayerSpeedTopBottomPerSecond(), mFixedObjectList);
		mPlayerObject.resetMoveDeltas();
	}
};



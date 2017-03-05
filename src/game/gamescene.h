#pragma once
#include "../engine/node.h"
#include "leveldesc/levelloader.h"
#include "worldstate.h"

class GameScene : public Node {
private:
	LevelLoader mLevelLoader;
	WorldState  mWorldState;
	Node2d *background=nullptr;
protected:
	void init(const string& rLevelName);
	//virtual void doDestroy() override;
	virtual void doUpdate(float) override; 
public:
	GameScene(const string &rLevelName);
	using Node::Node;
};

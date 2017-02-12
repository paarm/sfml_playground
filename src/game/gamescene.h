#pragma once
#include "../engine/node.h"
#include "leveldesc/levelloader.h"


class GameScene : public Node {
private:
	LevelLoader mLevelLoader;
	Node2d *background=nullptr;
protected:
	void init();
	//virtual void doDestroy() override;
	//virtual void doUpdate(double) override; 
public:
	GameScene();
	using Node::Node;
};

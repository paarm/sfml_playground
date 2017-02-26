#include "gamedirector.h"
#include "../engine/director.h"
#include "gamescene.h"

#define VIRTUAL_SCREEN_SIZE_X 1280
#define VIRTUAL_SCREEN_SIZE_Y 768
#define BLOCK_SIZE_X 32
#define BLOCK_SIZE_Y 32
#define BLOCK_COUNT_X 40
#define BLOCK_COUNT_Y 24

int GameDirector::getVirtualScreenSizeX() {
	return VIRTUAL_SCREEN_SIZE_X;
}
int GameDirector::getVirtualScreenSizeY() {
	return VIRTUAL_SCREEN_SIZE_Y;
}
int GameDirector::getBlockSizeX() {
	return BLOCK_SIZE_X;
}
int GameDirector::getBlockSizeY() {
	return BLOCK_SIZE_Y;
}
int GameDirector::getBlockCountX() {
	return BLOCK_COUNT_X;
}
int GameDirector::getBlockCountY() {
	return BLOCK_COUNT_Y;
}
void GameDirector::run() {
	Director::getInstance().initialize(VIRTUAL_SCREEN_SIZE_X, VIRTUAL_SCREEN_SIZE_Y);
	//TextureManager::getInstance().loadTexture("assets/cursor.png", "cursor");
	//Director::getInstance()->setMousePointer(TextureManager::getInstance().getTexture("cursor"), MousePointerAlignment::Middle);
	GameScene *rGameScene=new GameScene("assets/levels/testlevel.json");
	Director::getInstance().runWithNode(rGameScene);
}

void GameDirector::runWithLevel(const string &rLevelName) {

}

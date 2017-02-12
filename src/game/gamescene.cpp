#include "gamescene.h"


GameScene::GameScene() : Node("Game Scene") {
	init();
} 

void GameScene::init() {
	mLevelLoader.loadLevel("Dummy");
	FontContext *rFontContext=FontManager::getInstance().addFont("assets/sansation.ttf","sansation");
	NodeText *s_text=(NodeText*)addNode(new NodeText(string ("Hello Text"), rFontContext, 10, 10));
	s_text->setText("Hallo Welt");

	TextureManager::getInstance().loadTexture("assets/testschwein.png", "testschwein");
	TextureManager::getInstance().createAutomaticFramesAndSequence("testschwein", "testschwein.Walk", 11, 1);

	Node2d *rNodeSchweinchen=(Node2d*)addNode(new Node2d(string("Sprite 2"), TextureManager::getInstance().getTextureFrame(string("testschwein.Walk0")), 150, 0));
	//s_sub->setScale(2.0,2.0);
	// add two animation player to the second sprite and activate animation player 1 for now
	rNodeSchweinchen->addNode(new FramePlayer("testschwein.Walk", TextureManager::getInstance().getFrameSequence("testschwein.Walk"),50));
	rNodeSchweinchen->activateFramePlayer("testschwein.Walk");
	//s_sub->addNode(new FramePlayer("Test.Walk2", textureManager.getFrameSequence("Test.Walk2"),1000, PlayerType::Forward));

	scheduleUpdate(true);
}

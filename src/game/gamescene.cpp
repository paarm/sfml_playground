#include "gamescene.h"
#include "../engine/director.h"

GameScene::GameScene(const string& rLevelName) : Node(rLevelName) {
	init(rLevelName);
}

void GameScene::init(const string &rLevelName) {
	mLevelLoader.loadLevel(rLevelName);
	mLevelLoader.addLevelToNode(*this);


	Node* rNodePlayfieldLayer=(Node*)this->searchNode("PlayfieldLayer", true);
	if (rNodePlayfieldLayer) {
		const vector<Node*>& rNodeList=rNodePlayfieldLayer->getChildNodeList();
		for (Node *rNode : rNodeList) {
			if (rNode->getNodeType()==NodeType::Sprite) {
				Node2d* rNode2dBlock=(Node2d*)rNode;
				if (rNode->getHint()=="Gravity") {
					mWorldState.addJumpObject(rNode2dBlock);
				} else {
					mWorldState.addFixedObject(rNode2dBlock);
				}
			}
		}
	}
	Node* rNodeEnemiesLayer=(Node*)this->searchNode("EnemiesLayer", true);
	if (rNodeEnemiesLayer) {
		const vector<Node*>& rNodeList=rNodeEnemiesLayer->getChildNodeList();
		for (Node *rNode : rNodeList) {
			if (rNode->getNodeType()==NodeType::Sprite && rNode->getHint()=="Enemy") {
				Node2d* rNode2dEnemy=(Node2d*)rNode;
				mWorldState.addEnemyObject(rNode2dEnemy);
			}
		}
	}
	Node* rNodePlayerLayer=(Node*)this->searchNode("PlayerLayer", true);
	if (rNodePlayerLayer) {
		const vector<Node*>& rNodeList=rNodePlayerLayer->getChildNodeList();
		for (Node *rNode : rNodeList) {
			if (rNode->getNodeType()==NodeType::Sprite && rNode->getHint()=="Player") {
				Node2d* rNode2dPlayer=(Node2d*)rNode;
				mWorldState.setPlayerObject(rNode2dPlayer);
			}
		}
	}

	//TextureManager::getInstance().loadTexture("assets/testschwein.png", "testschwein");
	//TextureManager::getInstance().createAutomaticFramesAndSequence("testschwein", "testschwein.Walk", 11, 1);

	//s_sub->addNode(new FramePlayer("Test.Walk2", textureManager.getFrameSequence("Test.Walk2"),1000, PlayerType::Forward));

#if 0

	Node2d *rNodeSchweinchen=(Node2d*)addNode(new Node2d("Sprite 2", TextureManager::getInstance().getTextureFrame("Testschwein.Walk0"), 150, 0));
	//s_sub->setScale(2.0,2.0);
	// add two animation player to the second sprite and activate animation player 1 for now
	rNodeSchweinchen->addNode(new FramePlayer("Testschwein.Walk", TextureManager::getInstance().getFrameSequence("Testschwein.Walk"),50));
	rNodeSchweinchen->activateFramePlayer("Testschwein.Walk");
	rNodeSchweinchen->setFlipX(true);
	rNodeSchweinchen->setFlipY(true);
#endif
	FontContext *rFontContext=FontManager::getInstance().addFont("assets/sansation.ttf","sansation");
	NodeText *s_text=(NodeText*)addNode(new NodeText("FramesPerSecond", rFontContext, 10, 10));
	s_text->setText("0");

#if 0
	Level &rLevel=mLevelLoader.getLevel();
	for (int i=0,count=rLevel.getRowCount();i<count;i++) {
		Row &rRow=rLevel.getRow(i);
		for (int c=0,ccount=rRow.getColumnCount();c<ccount;c++) {
			Column& rColumn=rRow.getColumn(c);
			ObjectDesc *rObjectDesc=rColumn.getObjectDesc();
			if (rObjectDesc) {
				TextureFrame *rTextureFrame=TextureManager::getInstance().getTextureFrame(rObjectDesc->getDefaultFrame());
				Node2d *rNode2d=nullptr;
				if (rTextureFrame) {
					string name=""+to_string(i)+"/"+to_string(c);
					rNode2d=(Node2d*)addNode(new Node2d(name, rTextureFrame, c*GameDirector::getInstance().getBlockSizeX(), i*GameDirector::getInstance().getBlockSizeY()));
					rColumn.setCurrentNode(rNode2d);
				}
				int acount=rObjectDesc->getObjectSequenceCount();
				for (int a=0;a<acount;a++) {
					const string &rName=rObjectDesc->getObjectSequenceName(a);
					ObjectSequence &rObjectSequence=rObjectDesc->getObjectSequence(rName);
					FrameSequence *rFrameSequence=TextureManager::getInstance().getFrameSequence(rObjectSequence.getSequenceName());
					if (rFrameSequence) {
						rNode2d->addNode(new FramePlayer(rObjectSequence.getObjectSequenceName(), rFrameSequence,(float)rObjectSequence.getFrameDelayInMs()));
						if (rObjectDesc->getDefaultSequence()==rObjectSequence.getObjectSequenceName()) {
							rNode2d->activateFramePlayer(rObjectDesc->getDefaultSequence());
						}						
					}
				}
			}
		}
	}
#endif
	scheduleUpdate(true);
}

void GameScene::doUpdate(float rDelta) {
	bool keyLeft=false;
	bool keyRight=false;
	bool keyUp=false;
	bool keyDown=false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		keyLeft=true;
		cout << "Key Left" << endl;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		keyRight=true;
		cout << "Key Right" << endl;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		keyUp=true;
		cout << "Key Up" << endl;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		keyDown=true;
		cout << "Key Down" << endl;
	}
	mWorldState.update(rDelta, keyLeft, keyRight, keyUp, keyDown, false);
#if 0
	Node2d* rBackgroundLayer=(Node2d*)searchNode("DuMusstDichDrehen", true);
	if (rBackgroundLayer) {
		rBackgroundLayer->setOriginMiddle();
		rBackgroundLayer->setFlipX(true);
		rBackgroundLayer->setRotationRelative((float)500.0*rDelta/(float)1000.0);
	}
	rBackgroundLayer=(Node2d*)searchNode("DuMusstVerkehrtDrehen", true);
	if (rBackgroundLayer) {
		rBackgroundLayer->setRotationRelative((float)-500.0*rDelta/(float)1000.0);
		//rBackgroundLayer->setPositionRelative((float)-20.0*rDelta/(float)1000.0, 0.0);
	}
#endif	
	NodeText *s_text=(NodeText*)searchNode("FramesPerSecond", true);
	if (s_text) {
		s_text->setText(std::to_string((int)((1000.0/rDelta)))+" FPS");
		//s_text->setText(std::to_string(rDelta)+" FPS");
	}
	//GameScene *rGameScene=new GameScene("assets/levels/testlevel.json");
	//Director::getInstance()->switchScene(rGameScene);
}

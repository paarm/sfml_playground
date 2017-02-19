#include "gamescene.h"


GameScene::GameScene() : Node("Game Scene") {
	init();
} 

void GameScene::init() {
	mLevelLoader.loadLevel("assets/levels/testlevel.json");
	//TextureManager::getInstance().loadTexture("assets/testschwein.png", "testschwein");
	//TextureManager::getInstance().createAutomaticFramesAndSequence("testschwein", "testschwein.Walk", 11, 1);

	Node2d *rNodeSchweinchen=(Node2d*)addNode(new Node2d(string("Sprite 2"), TextureManager::getInstance().getTextureFrame(string("Testschwein.Walk0")), 150, 0));
	//s_sub->setScale(2.0,2.0);
	// add two animation player to the second sprite and activate animation player 1 for now
	rNodeSchweinchen->addNode(new FramePlayer("Testschwein.Walk", TextureManager::getInstance().getFrameSequence("Testschwein.Walk"),50));
	rNodeSchweinchen->activateFramePlayer("Testschwein.Walk");
	//s_sub->addNode(new FramePlayer("Test.Walk2", textureManager.getFrameSequence("Test.Walk2"),1000, PlayerType::Forward));

	Level &rLevel=mLevelLoader.getLevel();
	for (int i=0, count=rLevel.getLayerCount();i<count;i++) {
		Layer &rLayer=rLevel.getLayerFromIndex(i);
		if (rLayer.getLayerName().length()>0) {
			Node2d *rLayerNode=(Node2d*)addNode(new Node2d(rLayer.getLayerName(), 0,0));
			for (int o=0, counto=rLayer.getLayerObjectCount();o<counto;o++) {
				LayerObject& rLayerObject=rLayer.getLayerObjectByIndex(o);
				ObjectDesc &rObjectDesc=rLevel.getObjectDesc(rLayerObject.getObjectDescId());
				TextureFrame *rTextureFrame=TextureManager::getInstance().getTextureFrame(rObjectDesc.getDefaultFrame());
				Node2d *rNode2d=nullptr;
				if (rTextureFrame) {
					string name=rLayerObject.getLayerObjectName();
					rNode2d=(Node2d*)rLayerNode->addNode(new Node2d(name, rTextureFrame, rLayerObject.getPosX(), rLayerObject.getPosY()));
					int acount=rObjectDesc.getObjectSequenceCount();
					for (int a=0;a<acount;a++) {
						const string &rName=rObjectDesc.getObjectSequenceName(a);
						ObjectSequence &rObjectSequence=rObjectDesc.getObjectSequence(rName);
						FrameSequence *rFrameSequence=TextureManager::getInstance().getFrameSequence(rObjectSequence.getSequenceName());
						if (rFrameSequence) {
							rNode2d->addNode(new FramePlayer(rObjectSequence.getObjectSequenceName(), rFrameSequence,(float)rObjectSequence.getFrameDelayInMs()));
							if (rObjectDesc.getDefaultSequence()==rObjectSequence.getObjectSequenceName()) {
								rNode2d->activateFramePlayer(rObjectDesc.getDefaultSequence());
							}						
						}
					}
				}
			}
		}
	}
	FontContext *rFontContext=FontManager::getInstance().addFont("assets/sansation.ttf","sansation");
	NodeText *s_text=(NodeText*)addNode(new NodeText(string ("Hello Text"), rFontContext, 10, 10));
	s_text->setText("Hallo Welt");

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
	Node2d* rBackgroundLayer=(Node2d*)searchNode("DuMusstDichDrehen", true);
	if (rBackgroundLayer) {
		rBackgroundLayer->setRotationRelative((float)500.0*rDelta/(float)1000.0);
	}
	rBackgroundLayer=(Node2d*)searchNode("DuMusstVerkehrtDrehen", true);
	if (rBackgroundLayer) {
		rBackgroundLayer->setRotationRelative((float)-500.0*rDelta/(float)1000.0);
		rBackgroundLayer->setPositionRelative((float)-20.0*rDelta/(float)1000.0, 0.0);
	}
}

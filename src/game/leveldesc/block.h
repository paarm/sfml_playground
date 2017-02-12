#pragma once
#include "blocktype.h"
#include "../../engine/node.h"

class Block {
private:
	BlockType       mBlockType;
	SpikeDirection  mSpikeDirection;
	Node            *mCurrentNode=nullptr;
public:
	Block() : mBlockType(BlockType::Nothing), mSpikeDirection(SpikeDirection::None), mCurrentNode(nullptr){}
	void setBlockType(BlockType rBlockType) {
		mBlockType=rBlockType;
	}
	void setSpikeDirection(SpikeDirection &rSpikeDirection) {
		mSpikeDirection=rSpikeDirection;
	}
	BlockType &getBlockType() {
		return mBlockType;
	}
	SpikeDirection &getSpikeDirection() {
		return mSpikeDirection;
	}
	void setCurrentNode(Node *rNode) {
		mCurrentNode=rNode;
	}
	Node* getCurrentNode() {
		return mCurrentNode;
	}
};
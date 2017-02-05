#pragma once
#include "blocktype.h"
#include "../../node.h"

class Block {
private:
	BlockType       mBlockType;
	SpikeDirection  mSpikeDirection;
	Node            *mNode=nullptr;
public:
	Block() : mBlockType(BlockType::Nothing), mSpikeDirection(SpikeDirection::None), mNode(nullptr){}
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
	Node* getNode() {
		return mNode;
	}
};
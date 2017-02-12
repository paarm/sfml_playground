#pragma once
#include "blocktype.h"
#include "../../engine/node.h"

class Block {
private:
	BlockType       mBlockType;
	Direction  mDirection;
	Node            *mCurrentNode=nullptr;
public:
	Block() : mBlockType(BlockType::Nothing), mDirection(Direction::Default), mCurrentNode(nullptr){}
	void setBlockType(BlockType rBlockType) {
		mBlockType=rBlockType;
	}
	void setSpikeDirection(Direction &rSpikeDirection) {
		mDirection=rSpikeDirection;
	}
	BlockType &getBlockType() {
		return mBlockType;
	}
	Direction &getDirection() {
		return mDirection;
	}
	void setCurrentNode(Node *rNode) {
		mCurrentNode=rNode;
	}
	Node* getCurrentNode() {
		return mCurrentNode;
	}
};
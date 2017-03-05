#pragma once
#include <string>

using namespace std;
class GameDirector {
private:
	GameDirector() = default;
	GameDirector(const GameDirector&)=delete;
	GameDirector& operator=(const GameDirector&)=delete;
public:
	static GameDirector& getInstance() {
		static GameDirector rGameDirector;
		return rGameDirector;
	}
	void runWithLevel(const string &rLevelName);
	
	int getVirtualScreenSizeX();
	int getVirtualScreenSizeY();
	int getBlockSizeX();
	int getBlockSizeY();
	int getBlockCountX();
	int getBlockCountY();
	float getEnemyLeftRightSpeedPerSecond();
	float getEnemyTopBottomSpeedPerSecond();
	float getPlayerSpeedLeftRightPerSecond();
	float getPlayerSpeedTopBottomPerSecond();
	void run();
};
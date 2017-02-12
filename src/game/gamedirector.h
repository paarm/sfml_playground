#pragma once

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
	int getVirtualScreenSizeX();
	int getVirtualScreenSizeY();
	int getBlockSizeX();
	int getBlockSizeY();
	int getBlockCountX();
	int getBlockCountY();

	void run();
};
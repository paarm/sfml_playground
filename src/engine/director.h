#pragma once
#include <SFML/Window.hpp>
#include "clock.h"
#include "texturemanager.h"
#include "node.h"
#if 0
enum class MousePointerAlignment {
	LeftTop=0,
	Middle,
};
#endif
#if 0
struct CustomMouseIcon {
	MousePointerAlignment mMousePointerAlignment;
	SDL_Texture *mSDL_Texture;
	SDL_Rect 	mSDL_DestRect;
	int 		mOffsetX;
	int 		mOffsetY;
};
#endif
class RenderWindowHandle {
public:
	sf::RenderWindow *mSFL_Window=nullptr;
};

class Director {
private:
	//SDL_Renderer 	*mSDL_Renderer=nullptr;
	RenderWindowHandle	mWindowHandle;
	bool 			mIsInitialized=false;
	bool			mIsRunning=false;
	NodeRoot		mRootScene {"Root scene"};
	Node			*mStageScene=nullptr;
	Node			*mCurrentScene=nullptr;
	Clock 			clock;
	//CustomMouseIcon mCustomMouseIcon;
	Director();
	Director(const Director&) = delete;
	Director& operator=(const Director&)=delete;
public:
	static Director& getInstance() {
		static Director instance; 
		return instance;
	}
	~Director();
	void initialize(int rVirtualScreenResolutionX, int rVirtualScreenResolutionY);
	void switchScene(Node *n);
	void runWithNode(Node *n);
	//void setMousePointer(SDL_Texture *rSDL_Texture, MousePointerAlignment rMousePointerAlignment=MousePointerAlignment::LeftTop);
};
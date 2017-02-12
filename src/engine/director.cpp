#include "director.h"

Director::Director() {
	mRootScene.scheduleUpdate(true);
}

Director::~Director() {
	if (mWindowHandle.mSFL_Window) {
		delete mWindowHandle.mSFL_Window;
		mWindowHandle.mSFL_Window=nullptr;
	}
	mRootScene.deleteChilds();
}

void Director::initialize(int rVirtualScreenResolutionX, int rVirtualScreenResolutionY) {
	//init SDL2
	//if (!SDL_Init(SDL_INIT_VIDEO)) {
		// open window
	mWindowHandle.mSFL_Window = new sf::RenderWindow(sf::VideoMode(rVirtualScreenResolutionX, rVirtualScreenResolutionY), "Hello World!");
	if (mWindowHandle.mSFL_Window) {
		mWindowHandle.mSFL_Window->setVerticalSyncEnabled(true);
		mIsInitialized=true;
	} else {
		std::cout << "Cannot create Window" << std::endl;
	}
	//}
		//mSDL_Window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
#if 0
		if (mSDL_Window) {
			mSDL_Renderer = SDL_CreateRenderer(mSDL_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mSDL_Renderer) {
				TextureManager::getInstance().setSDLRenderer(mSDL_Renderer);
				mIsInitialized=true;
			} else {
				std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			}
		} else {
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		}
	} else {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
	}
#endif	
}

void Director::switchScene(Node *n) {
	mStageScene=n;
}

#if 0
void Director::setMousePointer(SDL_Texture *rSDL_Texture, MousePointerAlignment rMousePointerAlignment) {
	mCustomMouseIcon.mSDL_Texture=rSDL_Texture;
	mCustomMouseIcon.mMousePointerAlignment=rMousePointerAlignment;
	if (mCustomMouseIcon.mSDL_Texture) {
		SDL_ShowCursor( SDL_DISABLE );
		mCustomMouseIcon.mOffsetX=0;
		mCustomMouseIcon.mOffsetY=0;
		mCustomMouseIcon.mSDL_DestRect.x=0;
		mCustomMouseIcon.mSDL_DestRect.y=0;

		TextureManager::getInstance().queryTextureSize(mCustomMouseIcon.mSDL_Texture, &mCustomMouseIcon.mSDL_DestRect.w, &mCustomMouseIcon.mSDL_DestRect.h);

/*
		Uint32 format;
		TextureManager::getInstance().queryTextureSizeAndFormat(mCustomMouseIcon.mSDL_Texture, &mCustomMouseIcon.mSDL_DestRect.w, &mCustomMouseIcon.mSDL_DestRect.h, &format);
		SDL_PixelFormat *pixelFormat=SDL_AllocFormat(format);

		SDL_SetColorKey(mCustomMouseIcon.mSDL_Texture,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(pixelFormat,0x255,0x255,0x255)); 		
		SDL_FreeFormat(pixelFormat);
*/
		if (mCustomMouseIcon.mMousePointerAlignment==MousePointerAlignment::Middle) {
			mCustomMouseIcon.mOffsetX=-mCustomMouseIcon.mSDL_DestRect.w/2;
			mCustomMouseIcon.mOffsetY=-mCustomMouseIcon.mSDL_DestRect.h/2;
		}
	} else {
		SDL_ShowCursor( SDL_ENABLE );
	}
}
#endif	

void Director::runWithNode(Node *n) {
	if (mIsInitialized) {
		bool quit=false;
		switchScene(n);
		while (!quit) {
			clock.tick();
			sf::Event e;
			while (mWindowHandle.mSFL_Window->pollEvent(e)) {
				if (e.type == sf::Event::Closed) {
					mWindowHandle.mSFL_Window->close();
				}
			}
			if (mStageScene) {
				mRootScene.deleteChilds();
				mRootScene.addNode(mStageScene);
				mStageScene=nullptr;
			}
			mRootScene.executeUpdate(clock.getDelta());
			mWindowHandle.mSFL_Window->clear();
			mRootScene.executeDraw(*mWindowHandle.mSFL_Window, sf::Transform::Identity);
			mWindowHandle.mSFL_Window->display();
			
#if 0
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT) {
				quit = true;
				std::cout << "SDL_QUIT" << endl;
			}
			if (e.type == SDL_KEYDOWN) {
				quit = true;
				std::cout << "SDL_KEYDOWN" << endl;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				quit = true;
				std::cout << "SDL_MOUSEBUTTONDOWN" << endl;
			}
			/*if (e.type==SDL_MOUSEMOTION) {
				mouseRect.x = e.motion.x;
				mouseRect.y = e.motion.y;
			}*/
			mRootScene.updateInternal(clock.getDelta());
			//First clear the renderer
			SDL_RenderClear(mSDL_Renderer);
			//SDL_SetRenderDrawBlendMode(mSDL_Renderer, SDL_BLENDMODE_BLEND);
			// draw the hole node tree
			mRootScene.draw(mSDL_Renderer,0,0);

			if (mCustomMouseIcon.mSDL_Texture) {
				SDL_GetMouseState(&mCustomMouseIcon.mSDL_DestRect.x, &mCustomMouseIcon.mSDL_DestRect.y);
				mCustomMouseIcon.mSDL_DestRect.x+=mCustomMouseIcon.mOffsetX;
				mCustomMouseIcon.mSDL_DestRect.y+=mCustomMouseIcon.mOffsetY;
				SDL_RenderCopy(mSDL_Renderer, mCustomMouseIcon.mSDL_Texture, nullptr, &mCustomMouseIcon.mSDL_DestRect);
			}
			//Update the screen
			SDL_RenderPresent(mSDL_Renderer);
			if (mStageScene) {
				mRootScene.deleteChilds();
				mRootScene.addNode(mStageScene);
				mStageScene=nullptr;
			}
#endif			
			if (quit==true) {
				break;
			} else {
				quit=(!mWindowHandle.mSFL_Window->isOpen());
			}
		}
	}
}



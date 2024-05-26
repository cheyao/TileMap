#ifndef GAME_HPP
#define GAME_HPP
#pragma once

#include <SDL3/SDL.h>

#include <vector>

class Game {
       public:
	Game();
	~Game();

	int init();
	int iterate();
	static int event(const SDL_Event& event);

	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);

	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);

	// No copy
	Game(Game const&) = delete;
	void operator=(Game const& x) = delete;

       private:
	void input();
	void update();
	void draw();

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint64 mTicks;

	bool mUpdatingActors;

#ifdef __EMSCRIPTEN__
	int mWidth, mHeight;
#endif
};

#endif	// GAME_HPP

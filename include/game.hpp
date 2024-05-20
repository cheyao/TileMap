#ifndef GAME_H
#define GAME_H
#pragma once

#include <SDL3/SDL.h>

#include <vector>

#include "actor.hpp"
#include "spriteComponent.hpp"

class Game {
       public:
	Game();
	~Game();
	int init();
	int iterate();
	int event(SDL_Event event);

	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);

	void addSprite(SpriteComponent* sprite);
	void removeSprite(SpriteComponent* sprite);

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
};

#endif	// GAME_H

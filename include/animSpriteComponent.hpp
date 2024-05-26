#ifndef ANIMATED_SPRITE_COMPONENT_HPP
#define ANIMATED_SPRITE_COMPONENT_HPP
#pragma once

#include <SDL3/SDL.h>

#include "spriteComponent.hpp"

class AnimSpriteComponent : public SpriteComponent {
       public:
	explicit AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void update(float delta) override;
	void draw(SDL_Renderer* renderer) override;

	float getFPS() const { return mFPS; }
	void setFPS(float fps) { mFPS = fps; }

	int getFrames() const { return mFrames; }
	void setFrames(int frames) { mFrames = frames; }

       private:
	int mFrames;

	float mFPS;
	float mCurrentFrame;
};

#endif	// ANIMATED_SPRITE_COMPONENT_HPP

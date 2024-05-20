#ifndef ANIMATED_SPRITE_COMPONENT_H
#define ANIMATED_SPRITE_COMPONENT_H
#pragma once

#include "spriteComponent.hpp"

class AnimSpriteComponent : public SpriteComponent {
       public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void update(float delta) override;
	void draw(SDL_Renderer* renderer) override;

	float getFPS() { return mFPS; }
	void setFPS(float fps) { mFPS = fps; }
	int getFrames() { return mFrames; }
	void setFrames(int frames) { mFrames = frames; }

       private:
	int mFrames;

	float mFPS;
	float mCurrentFrame;
};

#endif	// ANIMATED_SPRITE_COMPONENT_H

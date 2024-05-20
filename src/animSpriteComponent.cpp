#include "animSpriteComponent.hpp"

#include <SDL3/SDL.h>

#include "actor.hpp"
#include "common.hpp"
#include "spriteComponent.hpp"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
    : SpriteComponent(owner, drawOrder),
      mFrames(24),
      mFPS(24.f),
      mCurrentFrame(0.f) {}

void AnimSpriteComponent::update(float delta) {
	SpriteComponent::update(delta);

	if (!mTexture) {
		// Return if no texture
		return;
	}

	mCurrentFrame += mFPS * delta;

	if (mCurrentFrame >= mFrames) {
		mCurrentFrame -= mFrames;
	}
}

void AnimSpriteComponent::draw(SDL_Renderer* renderer) {
	if (mTexture == nullptr) {
		return;
	}

	SDL_FRect src;
	src.w = 64;
	src.h = mTextureHeight;
	src.x = 64 * static_cast<int>(mCurrentFrame);
	src.y = 0;

	SDL_FRect dst;
	dst.w = src.w * mOwner->getScale();
	dst.h = src.h * mOwner->getScale();
	dst.x = mOwner->getPosition().x - dst.w / 2;
	dst.y = mOwner->getPosition().y - dst.h / 2;

	SDL_FPoint center(mOwner->getPosition());

	SDL_RenderTextureRotated(renderer, mTexture, &src, &dst,
				 -(maths::toDegrees(mOwner->getRotation())),
				 &center, SDL_FLIP_NONE);
}

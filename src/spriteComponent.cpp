#include "spriteComponent.hpp"

#include <SDL3/SDL.h>

#include "actor.hpp"
#include "common.hpp"
#include "component.hpp"
#include "game.hpp"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
    : Component(owner), mDrawOrder(drawOrder) {
	mOwner->getGame()->addSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->getGame()->removeSprite(this);
	SDL_DestroyTexture(mTexture);
}

void SpriteComponent::draw(SDL_Renderer* renderer) {
	if (mTexture == nullptr) {
		// Return if no texture
		return;
	}

	SDL_FRect dst;

	dst.w = mTextureWidth * mOwner->getScale();
	dst.h = mTextureHeight * mOwner->getScale();
	dst.x = mOwner->getPosition().x - dst.w / 2;
	dst.y = mOwner->getPosition().y - dst.h / 2;

	SDL_FPoint center(mOwner->getPosition());

	SDL_RenderTextureRotated(renderer, mTexture, nullptr, &dst,
				 -(maths::toDegrees(mOwner->getRotation())),
				 &center, SDL_FLIP_NONE);
}

void SpriteComponent::setTexture(SDL_Texture* texture) {
	mTexture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth,
			 &mTextureHeight);
}


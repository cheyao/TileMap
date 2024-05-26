#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP
#pragma once

#include <SDL3/SDL.h>

#include "component.hpp"

class SpriteComponent : public Component {
       public:
	// (Lower draw order corresponds with further back)
	explicit SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void draw(SDL_Renderer* renderer);
	virtual void setTexture(SDL_Texture* texture);

	int getDrawOrder() const { return mDrawOrder; }
	int getTextureHeight() const { return mTextureHeight; }
	int getTextureWidth() const { return mTextureWidth; }

       protected:
	SDL_Texture* mTexture;
	int mDrawOrder;
	int mTextureWidth;
	int mTextureHeight;
};

#endif	// sprite_COMPONENT_HPP

#ifndef TILE_MAP_COMPONENT_HPP
#define TILE_MAP_COMPONENT_HPP
#pragma once

#include <SDL3/SDL.h>

#include <vector>

#include "common.hpp"
#include "spriteComponent.hpp"

class TileMapComponent : public SpriteComponent {
       public:
	explicit TileMapComponent(class Actor* owner, int drawOrder = 10);

	void setScreenSize(Vector2 size) { mScreenSize = size; }
	void setScrollSpeed(float speed) { mScrollSpeed = speed; }
	void setDictionary(const char* dict);

	void update(float delta) override;
	void draw(SDL_Renderer* renderer) override;

       private:
	Vector2 mOffset;
	Vector2 mScreenSize;
	std::vector<std::vector<int>> mDictionary;
	float mScrollSpeed;
};

#endif	// TILE_MAP_COMPONENT_HPP

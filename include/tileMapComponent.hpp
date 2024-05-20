#ifndef TILE_MAP_COMPONENT_H
#define TILE_MAP_COMPONENT_H
#pragma once

#include "spriteComponent.hpp"
#include "common.hpp"

#include <vector>
#include <string>

class TileMapComponent : public SpriteComponent {
       public:
	TileMapComponent(class Actor* owner, int drawOrder = 10);

	void setScreenSize(Vector2 size) { mScreenSize = size; }
	void setScrollSpeed(float speed) { mScrollSpeed = speed; }
	void setDictionary(std::string dict);

	void update(float delta) override;
	void draw(SDL_Renderer* renderer) override;

       private:
	Vector2 mOffset;
	Vector2 mScreenSize;
	std::vector<std::vector<int>> mDictionary;
	float mScrollSpeed;
};

#endif	// TILE_MAP_COMPONENT_H

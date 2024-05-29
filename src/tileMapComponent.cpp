#include "tileMapComponent.hpp"

#include <SDL3/SDL.h>
#include <stddef.h>

#include <cctype>
#include <string>

#include "actor.hpp"
#include "spriteComponent.hpp"

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
    : SpriteComponent(owner, drawOrder), mScrollSpeed(0.f) {}

void TileMapComponent::setDictionary(const char* dict) {
	char* basepath = SDL_GetBasePath();
	std::string base;
	if (basepath == NULL) {
		base = "";
	} else {
		base = static_cast<std::string>(basepath);
		SDL_free(basepath);
	}

	SDL_IOStream* input = SDL_IOFromFile(
	    (base + dict).c_str(), "r");

	// C in C++ lol #BlameSDL
	mDictionary.push_back(std::vector<int>());
	char c;
	int i = 0;
	int sign = 1;
	while (SDL_ReadIO(input, &c, 1)) {
		if (c == '-') {
			sign = -1;
		}

		if (std::isdigit(c)) {
			i *= 10;
			i += c - '0';
			continue;
		}

		if (c == ',') {
			mDictionary[mDictionary.size() - 1].push_back(i * sign);
			i = 0;
			sign = 1;
			continue;
		}

		if (c == '\n') {
			mDictionary[mDictionary.size() - 1].push_back(i * sign);
			mDictionary.push_back(std::vector<int>());
			i = 0;
			sign = 1;
			continue;
		}
	}

	if (mDictionary[mDictionary.size() - 1].empty()) {
		mDictionary.pop_back();
	}

	SDL_CloseIO(input);
}

void TileMapComponent::update(float delta) {
	SpriteComponent::update(delta);

	mOffset.x += mScrollSpeed * delta;

	// Make sure the float doesn't overflow
	if (mOffset.x > 1060) {
		mOffset.x -= 1024;
	}
}

void TileMapComponent::draw(SDL_Renderer* renderer) {
	// If no texture return
	if (mTexture == nullptr) {
		return;
	}

	// Time to paint!
	for (size_t x = 0; x < mDictionary[0].size(); ++x) {
		for (size_t y = 0; y < mDictionary.size(); ++y) {
			if (mDictionary[y][x] == -1) {
				continue;
			}

			SDL_FRect src;
			src.w = 32;
			src.h = 32;
			src.x = 32 * static_cast<int>(mDictionary[y][x] % 8);
			src.y = 32 * static_cast<int>(mDictionary[y][x] / 8);

			SDL_FRect dst;
			dst.w = src.w * mOwner->getScale();
			dst.h = src.h * mOwner->getScale();
			dst.x = x * dst.w - mOffset.x;
			dst.y = y * dst.h - mOffset.y;

			// Get the offset back on screen
			while (dst.x < -32) {
				dst.x += 1024;
			}
			while (dst.y < -32) {
				dst.y += 768;
			}

			SDL_RenderTexture(renderer, mTexture, &src, &dst);

			if (dst.x < 0) {
				dst.x += 1024;
				SDL_RenderTexture(renderer, mTexture, &src,
						  &dst);
			}
		}
	}
}


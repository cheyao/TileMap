#ifndef ACTOR_H
#define ACTOR_H
#pragma once

#include <vector>

#include "common.hpp"

class Actor {
       public:
	enum State { STATE_ALIVE, STATE_PAUSED, STATE_DEAD };

	Actor(class Game* game);
	virtual ~Actor();

	void update(float delta);
	void updateComponents(float delta);
	virtual void updateActor(float delta);

	void input(const Uint8* keystate);
	virtual void actorInput(const Uint8* keystate);

	class Game* getGame() { return mGame; }

	const Vector2& getPosition() const { return mPosition; }
	void setPosition(const Vector2& pos) { mPosition = pos; }
	float getScale() const { return mScale; }
	void setScale(float scale) { mScale = scale; }
	float getRotation() { return mRotation; }
	void setRotation(float rotation) { mRotation = rotation; }

	State getState() { return mState; }
	void setState(State state) { mState = state; }

	void addComponent(class Component* component);
	void removeComponent(class Component* component);

       private:
	State mState;

	Vector2 mPosition;
	float mScale;
	float mRotation;  // Radians

	std::vector<class Component*> mComponents;
	class Game* mGame;
};

#endif	// ACTOR_H

#include "actor.hpp"

#include <algorithm>

#include "component.hpp"
#include "game.hpp"

Actor::Actor(Game* game) : mState(STATE_ALIVE), mScale(1.0f), mRotation(0), mGame(game) {
	mGame->addActor(this);
}

Actor::~Actor() {
	mGame->removeActor(this);

	while (!mComponents.empty()) {
		delete mComponents.back();
	}
}

void Actor::update(float delta) {
	if (mState == STATE_ALIVE) {
		updateComponents(delta);
		updateActor(delta);
	}
}

void Actor::updateComponents(float delta) {
	for (auto component : mComponents) {
		component->update(delta);
	}
}

void Actor::updateActor(float delta) { (void) delta; }

void Actor::input(const Uint8* keystate) {
	if (mState != STATE_ALIVE) {
		return;
	}

	for (auto component : mComponents) {
		component->input(keystate);
	}
	actorInput(keystate);
}

void Actor::actorInput(const Uint8* keystate) { (void) keystate; }

void Actor::addComponent(Component* component) {
	int order = component->getUpdateOrder();
	auto iter = mComponents.begin();

	for (; iter != mComponents.end(); ++iter) {
		if (order < (*iter)->getUpdateOrder()) {
			break;
		}
	}

	mComponents.insert(iter, component);
}

void Actor::removeComponent(Component* component) {
	auto iter =
	    std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}


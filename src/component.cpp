#include "component.hpp"

#include "actor.hpp"

Component::Component(Actor* owner, int updateOrder) {
	mUpdateOrder = updateOrder;
	mOwner = owner;

	mOwner->addComponent(this);
}

Component::~Component() { mOwner->removeComponent(this); }

void Component::update(float delta) { (void) delta; }

void Component::input(const Uint8* keystate) { (void) keystate; }


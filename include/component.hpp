#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#pragma once

#include <SDL3/SDL.h>

class Component {
       public:
	// (the lower the update order, the earlier the component updates)
	explicit Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void update(float delta);
	virtual void input(const Uint8* keystate);

	int getUpdateOrder() const { return mUpdateOrder; }

       protected:
	// Owner
	class Actor* mOwner;
	int mUpdateOrder;
};

#endif	// COMPONENT_HPP

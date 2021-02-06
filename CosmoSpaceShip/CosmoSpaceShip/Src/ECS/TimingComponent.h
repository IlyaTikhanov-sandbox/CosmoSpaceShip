#pragma once

#include "Components.h"
#include "../Stack.h"


class TimingComponent : public Component
{
	Stack <Vector2D> *storage;
	int size_;

public:

	TimingComponent():size_(0) 
	{
		storage = new Stack<Vector2D>(5);
	}
	inline int size() const { return size_; }

	void init() override
	{
		size_ = 0;
	}
	void update() override
	{
	}

	void AddTimingState(Vector2D const & state)
	{
		storage->push(state);
		size_ = storage->GetCurrElem();
	}
	void GetLastTimingState(TransformComponent * transform)
	{
		if (!storage->empty())
		{
			transform->position = storage->pop();
			size_ = storage->GetCurrElem();
		}			
	}

	int size()
	{
		return size_;
	}


};
#pragma once
#include "Globals.h"

class MoveToPosition :public virtual Task
{
	MoveToPosition(XMFLOAT3* currentPosition, XMFLOAT3 targetPosition)
	{
		currentPosition = &targetPosition;
		delete(this);
	}

};

class FollowObject : public Task
{

	XMFLOAT3* currentPosition;
	GameObject* target;

	FollowObject(XMFLOAT3* currentPosition, GameObject* target)
	{
		this->currentPosition = currentPosition;
		this->target = target;
	}

	void Update(float totalTime, float dTime)
	{
		if (this->currentPosition == nullptr || target == nullptr)
			delete(this);
		else
			this->currentPosition = &this->target->body->Position;

	}

};
#pragma once
#include "CollideableEntity.h"

class Ball :
    public CollideableEntity
{
    Ball();

    ~Ball();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* thisBody, PhysBody* otherBody, b2Contact* contactInfo) override;

};


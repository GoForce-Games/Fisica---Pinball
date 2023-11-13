#pragma once
#include "Entity.h"
class Bumper : public Entity
{
public:

	Bumper();

	~Bumper();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* thisBody, PhysBody* otherBody, b2Contact* contactInfo) override;

};


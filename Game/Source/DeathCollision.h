#pragma once
#include "Entity.h"
class DeathCollision : public Entity
{
public:
	DeathCollision(pugi::xml_node objectData = pugi::xml_node());

	~DeathCollision();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* thisBody, PhysBody* otherBody, b2Contact* contactInfo) override;

};


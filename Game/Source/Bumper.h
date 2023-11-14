#pragma once
#include "Entity.h"
#include "SDL/include/SDL_rect.h"
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

public:

	int pointValue;

	int radius;
	float launchPower;
	SString texturePath;

	SDL_Texture* texture = nullptr;

};


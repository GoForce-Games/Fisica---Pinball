#pragma once
#include "Entity.h"

class Ball : public Entity
{
public:

    Ball();

    ~Ball();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* thisBody, PhysBody* otherBody, b2Contact* contactInfo) override;

public:
	SString texturePath;
	SDL_Texture* ballTex = nullptr;

	int radius;

	float accel = 0.2f;
	

	bool godMode = false;

};


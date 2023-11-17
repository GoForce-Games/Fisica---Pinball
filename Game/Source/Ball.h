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

	void SetPosition(const iPoint& newPos) override;

public:
	const char* texturepath;
	SDL_Texture* ballTex = nullptr;

};


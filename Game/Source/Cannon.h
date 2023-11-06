#ifndef __CANNON_H__
#define __CANNON_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
class Ball;

class Cannon : public Entity
{
public:

	Cannon();
	
	virtual ~Cannon();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void LoadBall();

	void LaunchBall();

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;

	Ball* ball;
	bool launched;

	float launchAngle = 0.0f;
	float launchPower = 0.0f;
	float launchPowerIncrease = 0.0f;

};

#endif // __CANNON_H__
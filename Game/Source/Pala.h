#ifndef __PALA_H__
#define __PALA_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -10.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

struct SDL_Texture;
class Pala : public Entity
{
public:

	Pala();
	virtual ~Pala();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	virtual bool CleanUp();

public:

	

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	float angle1 = 0.0f;
	float angle2 = 0.0f;
	b2World* world;
	b2Body* ball;
	b2Body* arm;
	b2RevoluteJoint* joint;
	b2Body* ball2;
	b2Body* arm2;
	b2RevoluteJoint* joint2;
};

#endif // __PALA_H__
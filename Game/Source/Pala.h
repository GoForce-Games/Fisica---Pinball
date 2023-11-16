#ifndef __PALA_H__
#define __PALA_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"


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
	SDL_Texture* texture2;
	const char* texturePath;
	PhysBody* pbody;

	float angle1 = 0.0f;
	float angle2 = 0.0f;
	
	b2Body* ball1;
	b2Body* arm1;
	b2RevoluteJointDef jointDef1;
	b2RevoluteJoint* joint1;
	b2Body* ball2;
	b2Body* arm2;
	b2RevoluteJointDef jointDef2;
	b2RevoluteJoint* joint2;

};

#endif // __PALA_H__
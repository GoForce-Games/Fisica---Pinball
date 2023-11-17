#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "PugiXml/src/pugixml.hpp"

class PhysBody;
class b2Contact;

enum class EntityType
{
	CANNON,
	BALL,
	ITEM,
	PALA,
	BUMPER,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contactInfo) {

	};

	virtual void SetPosition(const iPoint& newPos) {
		position.Create(newPos.x, newPos.y);
	}

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;
	iPoint position2;
	bool renderable = true;

	PhysBody* pbody = nullptr;

	bool setToDestroy = false;
};

#endif // __ENTITY_H__
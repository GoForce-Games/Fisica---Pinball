#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"

#include "Entity.h"

#include "List.h"


class Cannon;

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	Entity* CreateEntityByName(SString name);

	void DestroyEntity(Entity* entity);

	void RemoveEntity(Entity* entity);

	void AddEntity(Entity* entity);

public:

	List<Entity*> entities;

	pugi::xml_node entityPresets;

};

#endif // __ENTITYMANAGER_H__

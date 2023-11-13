#include "EntityManager.h"

#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Cannon.h"
#include "Item.h"
#include "Ball.h"
#include "Bumper.h"
#include "Pala.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
	needsAwaking = true;
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	Module::Awake(config);
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	entityPresets = config.child("entitypresets");

	return ret;

}

bool EntityManager::Start() {
	Module::Start();
	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		delete item->data;
		entities.Del(item);
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::CANNON:	entity = new Cannon(); break;
	case EntityType::ITEM:		entity = new Item(); break;
	case EntityType::BALL:		entity = new Ball(); break;
	case EntityType::BUMPER:	entity = new Bumper(); break;
	case EntityType::PALA:       entity = new Pala(); break;
	default:
		break;
	}

	if (entity != nullptr) {
		entity->parameters = entityPresets.child(entity->name.GetString());
	}
	// Si ya ha pasado la fase de inicializacion (Awake() y Start()), ejecuta manualmente la función correspondiente para esta entidad
	if (awoken) entity->Awake();
	if (started) entity->Start();

	entities.Add(entity);

	return entity;
}

// Removes the entity from the list and destroys it. Make sure not to have any pointers to it after running this
void EntityManager::DestroyEntity(Entity* entity)
{
	RemoveEntity(entity);
	entity->CleanUp();
	delete entity;
}

// Removes the entity from the list
void EntityManager::RemoveEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) 
		{
			delete item->data;
			entities.Del(item);
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;
		if (pEntity->setToDestroy) { DestroyEntity(pEntity); continue; }
		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}
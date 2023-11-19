#include "DeathCollision.h"
#include "App.h"
#include "Log.h"
#include "Reload.h"
#include "IntroScreen.h"
#include "LoseScreen.h"

DeathCollision::DeathCollision(pugi::xml_node objectData) : Entity(EntityType::LOSE_CONDITION)
{
	dataFromMap = objectData;
	name.Create("defeatdetector");
}

DeathCollision::~DeathCollision()
{
}

bool DeathCollision::Awake()
{
	if (dataFromMap) {
		//Build from map data
		position.x = dataFromMap.attribute("x").as_int();
		position.y = dataFromMap.attribute("y").as_int();

		//Get polygon vertices
		SString pStr = dataFromMap.child("polygon").attribute("points").as_string();
		std::vector<SString> coords = pStr.GetWords(' ');
		std::vector<int> intPoints;

		for (SString item : coords)
		{
			std::vector<SString> coord = item.GetWords(',');
			if (coord.size() >= 2) {
				intPoints.push_back(std::stof(coord[0].GetString()));
				intPoints.push_back(std::stof(coord[1].GetString()));
			}
		}

		int* pointsArray = intPoints.data();
		int numPoints = intPoints.size();

		//Build PhysBody
		pbody = app->physics->CreateChain(position.x, position.y, pointsArray, numPoints, STATIC);

		pbody->listener = this;
	}
	else {
		//Build from presets
	}
	return true;
}

bool DeathCollision::Start()
{
	return true;
}

bool DeathCollision::Update(float dt)
{
	return true;
}

bool DeathCollision::CleanUp()
{
	pbody->setToDestroy = true;
	pbody->boundEntity = nullptr;
	setToDestroy = true; //Asegura la eliminacion por parte del gestor de entidades

	return true;
}

void DeathCollision::OnCollision(PhysBody* thisBody, PhysBody* otherBody, b2Contact* contactInfo)
{
	//Prevent calling the function repeatedly
	if (app->reloader->activePreset == nullptr && !app->intro->active && !app->lose->active) {
		LOG("Lose condition achieved, transitioning to Game Over screen...");
		app->reloader->StartReload("gameToGameOver");
	}
}

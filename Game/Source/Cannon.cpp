#include "Cannon.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Cannon::Cannon() : Entity(EntityType::CANNON)
{
	name.Create("cannon");
}

Cannon::~Cannon() {

}

bool Cannon::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	launchAngle = parameters.attribute("launchAngle").as_float();
	launchPowerIncrease = parameters.attribute("launchPowerIncrease").as_float();
	texturePath = parameters.attribute("texturepath").as_string();
	launched = false;

	return true;
}

bool Cannon::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	LoadBall();

	return true;
}

bool Cannon::Update(float dt)
{
	if (ball != nullptr) {
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
			launchPower += launchPowerIncrease * dt;
		}
		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
			LaunchBall();
		}
	}

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Cannon::CleanUp()
{

	return true;
}

void Cannon::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Cannon::LoadBall()
{

}

void Cannon::LaunchBall()
{


	launchPower = 0.0f;
}

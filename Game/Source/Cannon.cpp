#include "Cannon.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Physics.h"

#include "Ball.h"

#include "Log.h"
#include "Point.h"

#include "Box2D/Box2D/Box2D.h"

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
	launchPowerCap = parameters.attribute("launchPowerCap").as_float();
	texturePath = parameters.attribute("texturepath").as_string();

	canonFx = app->audio->LoadFx("Assets/Audio/Fx/canon.wav");

	return true;
}

bool Cannon::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	int size = 16;
	pbody = app->physics->CreateRectangleSensor(position.x, position.y+size+5, size, size, bodyType::STATIC);
	pbody->listener = this;

	return true;
}

bool Cannon::Update(float dt)
{
	if (ball != nullptr) {
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && launchPower < launchPowerCap) {
			launchPower += launchPowerIncrease * dt;
		}
		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
			LaunchBall();
			app->audio->PlayFx(canonFx);
		}
	}
	else {
		if (canLaunch && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
			LoadBall();
		}

		// PELIGRO: CAUSA CRASH
		//if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		//	canLaunch = true;
	}

	//app->render->DrawTexture(texture, position.x, position.y);

	//if (app->debug)
	//	app->render->DrawCircle(position.x, position.y, 160, 255, 0, 0);

	return true;
}

bool Cannon::CleanUp()
{

	return true;
}

void Cannon::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contactInfo) {
	if (ball == nullptr) {
		Ball* tempBall = dynamic_cast<Ball*>(physB->boundEntity);
		if (tempBall) {
			ball = tempBall;
			canLaunch = true;
		}
	}
}

void Cannon::LoadBall()
{
	if (canLaunch && ball == nullptr) {
		ball = (Ball*)app->entityManager->CreateEntity(EntityType::BALL);
		ball->position.x = position.x;
		ball->position.y = position.y;
		if (ball->pbody == nullptr) ball->Awake();
	}
}

void Cannon::LaunchBall()
{
	ball->SetPosition(position);
	ball->pbody->body->SetLinearVelocity({ 0.0f,-launchPower });
	ball = nullptr;
	canLaunch = false;
	launchPower = 0.0f;
}

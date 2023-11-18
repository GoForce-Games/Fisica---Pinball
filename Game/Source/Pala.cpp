#include "Pala.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "Box2D/Box2D/Box2D.h"




Pala::Pala() : Entity(EntityType::PALA) {
	name.Create("palas");
}

Pala::~Pala() {}

bool Pala::Awake() {
	position.x = parameters.attribute("x1").as_int();
	position.y = parameters.attribute("y1").as_int();
	position2.x = parameters.attribute("x2").as_int();
	position2.y = parameters.attribute("y2").as_int();
	texturePath1 = parameters.attribute("texturepath1").as_string();
	texturePath2 = parameters.attribute("texturepath2").as_string();
	angle1 = parameters.attribute("angle1").as_float();
	angle2 = parameters.attribute("angle2").as_float();

	palaFx = app->audio->LoadFx("Assets/Audio/Fx/pala.wav");

	return true;
}


bool Pala::Start() {
	

	// Set this module as a listener for contacts
	
	texture = app->tex->Load(texturePath1);
	texture2 = app->tex->Load(texturePath2);

	//initilize textures
	//texture = app->tex->Load(texturePath);
	

	PhysBody* pbodyCircle1 = app->physics->CreateCircle(115, 525, 5, bodyType::STATIC);
	PhysBody* pbodyCircle2 = app->physics->CreateCircle(220, 525, 5, bodyType::STATIC);
	PhysBody* pbodyRect1 = app->physics->CreateRectangle(135, 535, 45, 10, bodyType::DYNAMIC);
	PhysBody* pbodyRect2 = app->physics->CreateRectangle(200, 535, 45, 10, bodyType::DYNAMIC);

	ball1 = pbodyCircle1->body;
	ball2 = pbodyCircle2->body;
	arm1 = pbodyRect1->body;
	arm2 = pbodyRect2->body;

	jointDef1.Initialize(ball1, arm1, ball1->GetWorldCenter());
	jointDef1.bodyA = ball1;
	jointDef1.bodyB = arm1;
	jointDef1.localAnchorA.Set(0, 0);
	jointDef1.localAnchorB.Set(-0.5, -0.1);
	jointDef1.enableLimit = true; 
	jointDef1.lowerAngle = -0.25f * b2_pi; 
	jointDef1.upperAngle = 0.1f * b2_pi; 
	jointDef1.enableMotor = true;
	jointDef1.motorSpeed = 0.1;
	jointDef1.maxMotorTorque = 500.0f;
	joint1 = (b2RevoluteJoint*) app->physics->world->CreateJoint(&jointDef1);

	jointDef2.Initialize(ball2, arm2, ball2->GetWorldCenter());
	jointDef2.localAnchorA.Set(0, 0);
	jointDef2.localAnchorB.Set(0.5, -0.1);
	jointDef2.enableLimit = true;
	jointDef2.lowerAngle = -0.1f * b2_pi;
	jointDef2.upperAngle = 0.25f * b2_pi;
	jointDef2.enableMotor = true;
	jointDef2.motorSpeed = 0.1;
	jointDef2.maxMotorTorque = 500.0f;
	joint2 = (b2RevoluteJoint*) app->physics->world->CreateJoint(&jointDef2);
	
	return true;
}

bool Pala::Update(float dt) {

	angle1 = joint1->GetJointAngle() * RADTODEG-17.0f;
	angle2 = joint2->GetJointAngle() * RADTODEG+17.0f;

	/*if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		app->audio->PlayFx(palaFx);
	}*/

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (angle1 > 55) {
		}
		else {
		joint1->SetMotorSpeed(-12.5f);
	    }
	}
	else  
	{
		if (angle1 > 0) {
		}
		else {
			joint1->SetMotorSpeed(10.9f);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (angle2 > 55) {
		}
		else {
			joint2->SetMotorSpeed(12.5f);
		}
	}
	else 
	{
		if (angle2 < 0) {
		}
		else {
			joint2->SetMotorSpeed(-10.9f);
		}

	}
	//  Iterate all objects in the world and draw the bodies

	app->render->DrawTexture(texture, position.x, position.y, NULL, 1.0F, angle1);
	app->render->DrawTexture(texture2, position2.x, position2.y, NULL, 1.0F, angle2);
	
	return true;
}



bool Pala::CleanUp() {
	if (texture != nullptr) {
		app->tex->UnLoad(texture);
		texture = nullptr;
	}
	return true;
}
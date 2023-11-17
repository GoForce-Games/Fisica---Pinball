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
	
}

Pala::~Pala() {}

bool Pala::Awake() {

	return true;
}


bool Pala::Start() {
	

	// Set this module as a listener for contacts
	


	//initilize textures
	//texture = app->tex->Load(texturePath);
	texture = app->tex->Load("Assets/Textures/Pala derecha.png");
	texture2 = app->tex->Load("Assets/Textures/Pala izquierda.png");

	PhysBody* pbodyCircle1 = app->physics->CreateCircle(115, 525, 5, bodyType::STATIC);
	PhysBody* pbodyCircle2 = app->physics->CreateCircle(220, 525, 5, bodyType::STATIC);
	PhysBody* pbodyRect1 = app->physics->CreateRectangle(135, 505, 45, 10, bodyType::DYNAMIC);
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

	app->render->Blit(texture, 115, 525, NULL, 1.0F, -(18*M_PI* angle1));
	app->render->Blit(texture2, 135, 525, NULL, 1.0F, -(18 * M_PI * angle2));

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (angle1 > 90) {
		}
		else {
		angle1 += 19.5f;
		joint1->SetMotorSpeed(-19.5f);
	    }
	}
	else  
	{
		if (angle1 < 0) {
		}
		else {
			angle1 -= 10.9f;
			joint1->SetMotorSpeed(10.9f);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (angle2> 90) {
		}
		else {
			angle2 += 19.5f;
			joint2->SetMotorSpeed(19.5f);
		}
	}
	else if (joint2->GetJointAngle() > 0.0f * DEGTORAD)
	{
		if (angle2 < 0) {
		}
		else {
			angle2 -= 10.9f;
			joint2->SetMotorSpeed(-10.9f);
		}

	}
	//  Iterate all objects in the world and draw the bodies
	
	return true;
}



bool Pala::CleanUp() {
	if (texture != nullptr) {
		app->tex->UnLoad(texture);
		texture = nullptr;
	}
	return true;
}
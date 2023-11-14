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


	
	
	PhysBody* pbodyCircle1 = app->physics->CreateCircle(115, 525, 5, bodyType::STATIC);
	PhysBody* pbodyCircle2 = app->physics->CreateCircle(220, 525, 5, bodyType::STATIC);
	PhysBody* pbodyRect1 = app->physics->CreateRectangle(135, 505, 50, 15, bodyType::DYNAMIC);
	PhysBody* pbodyRect2 = app->physics->CreateRectangle(200, 535, 50, 15, bodyType::DYNAMIC);

	
	ball1 = pbodyCircle1->body;
	ball2 = pbodyCircle2->body;
	arm1 = pbodyRect1->body;
	arm2 = pbodyRect2->body;


	
	jointDef1.Initialize(ball1, arm1, ball1->GetWorldCenter());
	jointDef1.bodyA = ball1;
	jointDef1.bodyB = arm1;
	jointDef1.localAnchorA.Set(0, 0);
	jointDef1.localAnchorB.Set(-0.5, -0.1);
	jointDef1.enableMotor = true;
	jointDef1.motorSpeed = 0.1;
	jointDef1.maxMotorTorque = 500.0f;
	joint1 = (b2RevoluteJoint*) app->physics->world->CreateJoint(&jointDef1);


	
	jointDef2.Initialize(ball2, arm2, ball2->GetWorldCenter());
	jointDef1.localAnchorA.Set(0, 0);
	jointDef1.localAnchorB.Set(0.5, 0.1);
	jointDef2.enableMotor = true;
	jointDef2.motorSpeed = 0.1;
	jointDef2.maxMotorTorque = 500.0f;
	joint2 = (b2RevoluteJoint*) app->physics->world->CreateJoint(&jointDef2);
	/*
	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_staticBody;
	ballBodyDef.position.Set(PIXEL_TO_METERS(xball1), PIXEL_TO_METERS(yball1));
	ball = world->CreateBody(&ballBodyDef);
	b2CircleShape ballShape;
	ballShape.m_radius = PIXEL_TO_METERS(10);
	b2FixtureDef ballFixture;
	ballFixture.shape = &ballShape;
	ball->CreateFixture(&ballFixture);

	b2BodyDef armBodyDef;
	armBodyDef.type = b2_dynamicBody;
	armBodyDef.position.Set(PIXEL_TO_METERS(150), PIXEL_TO_METERS(530));
	arm = world->CreateBody(&armBodyDef);
	b2PolygonShape armShape;
	armShape.SetAsBox(PIXEL_TO_METERS(50) * 0.5f, PIXEL_TO_METERS(15) * 0.5f);
	b2FixtureDef armFixture;
	armFixture.shape = &armShape;
	armFixture.density = 1.0f;
	arm->CreateFixture(&armFixture);

	b2RevoluteJointDef jointDef;
	jointDef.bodyA = ball;
	jointDef.bodyB = arm;
	jointDef.localAnchorA.Set(PIXEL_TO_METERS(15), 0);
	jointDef.localAnchorB.Set(0, 0);
	jointDef.collideConnected = false;
	joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);




	int xball2 = 215;
	int yball2 = 640 / 1.205f;
	b2BodyDef ballBodyDef2;
	ballBodyDef2.type = b2_staticBody;
	ballBodyDef2.position.Set(PIXEL_TO_METERS(xball2), PIXEL_TO_METERS(yball2));
	ball2 = world->CreateBody(&ballBodyDef2);
	b2CircleShape ballShape2;
	ballShape2.m_radius = PIXEL_TO_METERS(10);
	b2FixtureDef ballFixture2;
	ballFixture2.shape = &ballShape2;
	ball2->CreateFixture(&ballFixture2);

	b2BodyDef armBodyDef2;
	armBodyDef2.type = b2_dynamicBody;
	armBodyDef2.position.Set(PIXEL_TO_METERS(170), PIXEL_TO_METERS(530));
	armBodyDef2.angle = b2_pi;
	arm2 = world->CreateBody(&armBodyDef);
	b2PolygonShape armShape2;
	armShape2.SetAsBox(PIXEL_TO_METERS(50) * 0.5f, PIXEL_TO_METERS(15) * 0.5f);
	b2FixtureDef armFixture2;
	armFixture2.shape = &armShape2;
	armFixture2.density = 1.0f;
	arm2->CreateFixture(&armFixture2);

	b2RevoluteJointDef jointDef2;
	jointDef2.bodyA = ball2;
	jointDef2.bodyB = arm2;
	jointDef2.localAnchorA.Set(PIXEL_TO_METERS(-15), 0);
	jointDef2.localAnchorB.Set(0, 0);
	jointDef2.collideConnected = false;
	joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef2);*/

	

	return true;
}

bool Pala::Update(float dt) {

	

	

	//for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	//{
	//	// For each contact detected by Box2D, see if the first one colliding is a sensor
	//	if (c->IsTouching() && c->GetFixtureA()->IsSensor())
	//	{
	//		// If so, we call the OnCollision listener function (only of the sensor), passing as inputs our custom PhysBody classes
	//		PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
	//		PhysBody* pb2 = (PhysBody*)c->GetFixtureB()->GetBody()->GetUserData();

	//		if (pb1 && pb2 && pb1->listener)
	//			pb1->listener->OnCollision(pb1, pb2, c);
	//	}
	//}

    

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT && joint1->GetJointAngle() < 45.0f * DEGTORAD)
	{
		/*float rotationSpeed = 0.5f;
		joint1->SetMotorSpeed(rotationSpeed);*/
		joint1->SetMotorSpeed(-4.5f);
	}
	else  
	{
		joint1->SetMotorSpeed(3.9f);
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && joint2->GetJointAngle() < 45.0f * DEGTORAD)
	{
		joint2->SetMotorSpeed(04.5f);
	}
	else if (joint2->GetJointAngle() > 0.0f * DEGTORAD)
	{
		joint2->SetMotorSpeed(-3.9f);
	}

	
	//  Iterate all objects in the world and draw the bodies
	
	return true;
}



bool Pala::CleanUp()
{
	if (texture != nullptr) {
		app->tex->UnLoad(texture);
		texture = nullptr;
	}
	return true;
}
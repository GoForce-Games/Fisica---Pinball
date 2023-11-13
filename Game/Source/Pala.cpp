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


#ifdef _DEBUG
#pragma comment( lib, "../Game/Source/External/Box2D/libx86/DebugLib/Box2D.lib" )
#else
#pragma comment( lib, "../Game/Source/External/Box2D/libx86/ReleaseLib/Box2D.lib" )
#endif

Pala::Pala() : Entity(EntityType::PALA) {
	world = NULL;
}

Pala::~Pala() {}

bool Pala::Awake() {

	return true;
}


bool Pala::Start() {
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// Set this module as a listener for contacts
	


	//initilize textures
	//texture = app->tex->Load(texturePath);


	
	
	pbody = app->physics->CreateCircle(115, 530, 5, bodyType::STATIC);
	pbody = app->physics->CreateCircle(220, 530, 5, bodyType::STATIC);
	pbody = app->physics->CreateRectangle(135, 535, 50, 15, bodyType::STATIC);
	pbody = app->physics->CreateRectangle(200, 535, 50, 15, bodyType::STATIC);
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

	world->Step(1.0f / 60.0f, 6, 2);

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

    

	/*if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT && joint->GetJointAngle() < 45.0f * DEGTORAD)
	{
		float rotationSpeed = 0.5f;
		joint->SetMotorSpeed(rotationSpeed);
	}
	else if (joint->GetJointAngle() > 0.0f * DEGTORAD)
	{
		joint->SetMotorSpeed(-0.5f);
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && joint->GetJointAngle() < 45.0f * DEGTORAD)
	{
		float rotationSpeed = 0.5f;
		joint->SetMotorSpeed(rotationSpeed);
	}
	else if (joint->GetJointAngle() > 0.0f * DEGTORAD)
	{
		joint->SetMotorSpeed(-0.5f);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->debug = !app->debug;*/

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
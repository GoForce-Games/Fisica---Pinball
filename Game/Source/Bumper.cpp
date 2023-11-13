#include "Bumper.h"
#include "Ball.h"

#include "Physics.h"

Bumper::Bumper() : Entity(EntityType::BUMPER)
{
	name.Create("Bumper");
}

Bumper::~Bumper()
{
}

bool Bumper::Awake()
{
	return false;
}

bool Bumper::Start()
{
	return true;
}

bool Bumper::Update(float dt)
{
	return true;
}

bool Bumper::CleanUp()
{
	return true;
}

void Bumper::OnCollision(PhysBody* thisBody, PhysBody* otherBody, b2Contact* contactInfo)
{
	Ball* ball = dynamic_cast<Ball*>(otherBody->boundEntity);
	if (ball) {
		b2Vec2 impulse = otherBody->body->GetPosition();
		impulse -= pbody->body->GetPosition();
		impulse.Normalize();
		otherBody->body->ApplyLinearImpulse(impulse, otherBody->body->GetPosition(), true);
	}
}

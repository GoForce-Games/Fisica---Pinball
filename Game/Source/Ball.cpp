#include "Ball.h"

#include "Box2D/Box2D/Box2D.h"

Ball::Ball() : CollideableEntity(EntityType::BALL)
{
}

Ball::~Ball()
{
}

bool Ball::Awake()
{
	return false;
}

bool Ball::Start()
{
	return false;
}

bool Ball::Update(float dt)
{
	return false;
}

bool Ball::CleanUp()
{
	return false;
}

void Ball::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contactInfo)
{
}

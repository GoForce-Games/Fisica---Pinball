#include "Ball.h"

#include "App.h"
#include "Physics.h"

#include "Box2D/Box2D/Box2D.h"

Ball::Ball() : Entity(EntityType::BALL)
{
	name.Create("ball");
}

Ball::~Ball()
{
}

bool Ball::Awake()
{
	int radius = parameters.attribute("radius").as_int();

	pbody = app->physics->CreateCircle(position.x,position.y, radius, bodyType::DYNAMIC);
	pbody->boundEntity = this;

	return true;
}

bool Ball::Start()
{
	return true;
}

bool Ball::Update(float dt)
{
	// Actualiza la posicion de la entidad bola con la posicion de su cuerpo fisico
	pbody->GetPosition(position.x, position.y);



	return true;
}

bool Ball::CleanUp()
{
	pbody->setToDestroy = true;
	pbody->boundEntity = nullptr;
	setToDestroy = true; //Asegura la eliminacion por parte del gestor de entidades

	return true;
}

void Ball::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contactInfo)
{
}

void Ball::SetPosition(const iPoint& newPos)
{
	b2Transform t = pbody->body->GetTransform();
	t.p.Set(PIXEL_TO_METERS(newPos.x), PIXEL_TO_METERS(newPos.y));
	pbody->body->SetTransform(t.p,t.q.GetAngle());
}

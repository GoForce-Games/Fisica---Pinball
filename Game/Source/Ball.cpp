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
	radius = parameters.attribute("radius").as_int();

	pbody = app->physics->CreateCircle(position.x,position.y, radius, bodyType::DYNAMIC);
	pbody->boundEntity = this;
	pbody->body->SetBullet(true);

	//position - radius en x y en y
	//0.0169333333
	return true;
}

bool Ball::Start()
{
	return true;
}

bool Ball::Update(float dt)
{
	//PELIGRO: CAUSA CRASH
	//if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	//	setToDestroy = true;

	if (pbody == nullptr) return true;

	// Actualiza la posicion de la entidad bola con la posicion de su cuerpo fisico
	pbody->GetPosition(position.x, position.y);

	//app->render->DrawCircle(position.x, position.y, radius, 255U, 255U, 255U);
	app->render->DrawTexture(texture, position.x - radius, position.y - radius);

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

#include "Bumper.h"
#include "Ball.h"

#include "App.h"
#include "Physics.h"
#include "Textures.h"

Bumper::Bumper() : Entity(EntityType::BUMPER)
{
	name.Create("Bumper");
}

Bumper::~Bumper()
{
}

bool Bumper::Awake()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	radius = parameters.attribute("radius").as_int();
	launchPower = parameters.attribute("launchPower").as_float();
	pointValue = parameters.attribute("pointValue").as_int();
	// TODO Unificar el uso de texturas para componentes repetidos
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Bumper::Start()
{
	pbody = app->physics->CreateCircle(position.x, position.y, radius, bodyType::STATIC);
	pbody->listener = this;

	texture = app->tex->Load(texturePath.GetString());
	return true;
}

bool Bumper::Update(float dt)
{

	//app->render->DrawTexture(texture, position.x - radius, position.y - radius);

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

		//TODO implementar suma de puntuación
	}
}

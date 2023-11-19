#include "Ball.h"

#include "App.h"
#include "Physics.h"
#include "Textures.h"
#include "Defs.h"

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

	texturePath = parameters.attribute("texturepath").as_string();
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	pbody = app->physics->CreateCircle(position.x,position.y, radius, bodyType::DYNAMIC);
	pbody->boundEntity = this;
	pbody->body->SetBullet(true);
	pbody->listener = this;

	//position - radius en x y en y
	//0.0169333333
	return true;
}

bool Ball::Start()
{
	ballTex = app->tex->Load(texturePath.GetString());
	return true;
}

bool Ball::Update(float dt)
{
	//PELIGRO: CAUSA CRASH
	//if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	//	setToDestroy = true;

	if (pbody == nullptr) return true;

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		godMode = !godMode;
	}
	b2Vec2 impulse = b2Vec2_zero;
	if (godMode) {
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			impulse = b2Vec2(0.0f, -0.01f);
			pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			impulse = b2Vec2(-0.01f, 0.0f);
			pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			impulse = b2Vec2(0.01f, 0.0f);
			pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		impulse = b2Vec2(0.0f, -0.05f);
		pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
	}
	
	pbody->GetPosition(position.x, position.y);
	app->render->DrawTexture(ballTex, position.x, position.y);
	// Actualiza la posicion de la entidad bola con la posicion de su cuerpo fisico


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

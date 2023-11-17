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
	int radius = parameters.attribute("radius").as_int();

	texturepath = parameters.attribute("texturepath").as_string();
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	pbody = app->physics->CreateCircle(position.x,position.y, radius, bodyType::DYNAMIC);
	pbody->boundEntity = this;
	pbody->body->SetBullet(true);

	//position - radius en x y en y
	//0.0169333333
	return true;
}

bool Ball::Start()
{
	ballTex = app->tex->Load(texturepath);
	return true;
}

bool Ball::Update(float dt)
{
	
	app->render->DrawTexture(ballTex, position.x, position.y);
	// Actualiza la posicion de la entidad bola con la posicion de su cuerpo fisico
	pbody->GetPosition(position.x, position.y);

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		godMode = !godMode;
	}
	b2Vec2 impulse = b2Vec2_zero;
	if (godMode) {
		float valor;
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			impulse = b2Vec2(0.0f, -0.01f);
			pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			impulse = b2Vec2(-0.01f, 0.0f);
			pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			impulse = b2Vec2(0.01f, 0.0f);
			pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
		}
		else {
		}
	}

	

	
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

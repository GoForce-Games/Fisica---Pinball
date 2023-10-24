#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"

#include <math.h>
#include <cmath>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	cabeza = App->textures->Load("Assets/Cabezacanon.png");
	soporte = App->textures->Load("Assets/Soportecanon.png");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	PhysBall& ball = App->physics->balls[0];

	if (ball.exists) {
		if (!ball.physics_enabled) {

			if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT && launch_angle < 90.0f) {
				launch_angle += 0.05f;
				if (launch_angle > M_PI / 2)
					launch_angle = M_PI / 2;
			}
			else if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT && launch_angle > 0.0f) {
				launch_angle -= 0.05f;
				if (launch_angle < 0.0f)
					launch_angle = 0.0f;
			}


			if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) {
				launch_power += 0.5f;
			}
			if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) {
				launch_power -= 0.5f;
			}
			else if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_SPACE) == KEY_STATE::KEY_REPEAT) {
				LaunchBall();
			}


		}
		else {
			if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_C) == KEY_STATE::KEY_REPEAT) {
				ball.physics_enabled = false;
			}
		}
	}




	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	PhysBall& ball = App->physics->balls[0];
	int x0 = METERS_TO_PIXELS(ball.startX),
		y0 = SCREEN_HEIGHT - METERS_TO_PIXELS(ball.startY);

	int x1 = x0 + cos(launch_angle) * 50.0f;
	int y1 = y0 - sin(launch_angle) * 50.0f;

	int x2 = x0 + cos(launch_angle) * launch_power;
	int y2 = y0 - sin(launch_angle) * launch_power;

	App->renderer->Blit(cabeza, -35, 540, NULL, 1.0F, -(18*M_PI*launch_angle));
	App->renderer->Blit(soporte, -33, 550, NULL);
	App->renderer->DrawLine(x0, y0, x1, y1, 255, 0, 0);
	App->renderer->DrawLine(x0, y0, x2, y2, 0, 255, 0);

	return UPDATE_CONTINUE;
}

void ModulePlayer::LaunchBall()
{
	ModulePhysics* mp = App->physics;
	PhysBall& ball = mp->balls[0];

	ball.x = ball.startX;
	ball.y = ball.startY;

	ball.vx = cos(launch_angle) * launch_power;
	ball.vy = sin(launch_angle) * launch_power;

	ball.physics_enabled = true;
}




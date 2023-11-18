#include "LoseScreen.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Reload.h"
#include "Scene.h"
#include "Map.h"
#include "Audio.h"


LoseScreen::LoseScreen() {
	name.Create("LoseScreen");
}

LoseScreen::~LoseScreen()
{

}

bool LoseScreen::Start()
{
	LOG("Loading intro assets");

	bool ret = true;

	LoseTexture = app->tex->Load("Assets/Textures/Wasted.png");

	return ret;
}

bool LoseScreen::Update(float dt)
{
	app->render->DrawTexture(LoseTexture, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
	{
		app->reloader->StartReload("lose");
	}

	return true;
}

bool LoseScreen::CleanUp()
{
	if (LoseTexture != nullptr)
	{
		app->tex->UnLoad(LoseTexture);
		LoseTexture = nullptr;

	}

	return true;
}

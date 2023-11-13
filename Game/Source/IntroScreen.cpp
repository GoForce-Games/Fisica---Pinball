#include "IntroScreen.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Reload.h"
#include "Scene.h"
#include "Map.h"


IntroScreen::IntroScreen() {

}

IntroScreen::~IntroScreen()
{
}

bool IntroScreen::Start()
{	
	LOG("Loading intro assets");

	bool ret = true;

	IntroJuegoTexture = app->tex->Load("Assets/Textures/GoForceGames.png");

	return ret;
}

bool IntroScreen::Update(float dt)
{
  	app->render->DrawTexture(IntroJuegoTexture, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_SPACE))
	{		
		app->reloader->StartReload("map");
	}

	return true;
}

bool IntroScreen::CleanUp()
{
	if (IntroJuegoTexture != nullptr)
	{
		SDL_DestroyTexture(IntroJuegoTexture);
		IntroJuegoTexture = nullptr;	

	}

	return true;
}

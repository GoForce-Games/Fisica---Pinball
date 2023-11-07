#include "IntroScreen.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"


IntroScreen::IntroScreen() {

}

IntroScreen::~IntroScreen()
{
}

bool IntroScreen::Start()
{
	LOG("Loading intro assets");


	bool ret = true;

	IntroJuegoTexture = app->tex->Load("Assets/Textures/Presentacio1.png");

	return ret;
}

bool IntroScreen::Update()
{
	
	app->render->DrawTexture(IntroJuegoTexture, 0, 0, NULL);

	return true;
}

bool IntroScreen::CleanUp()
{
	

	return true;
}

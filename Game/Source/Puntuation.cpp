#include "Puntuation.h"
#include "App.h"
#include "Textures.h"
#include "ModuleFonts.h"
#include <stdio.h>
#include "SDL_image/include/SDL_image.h"
#include <sstream>
#include <string.h>
#include "Log.h"

using namespace std;

Puntuation::Puntuation() : Module()
{
}

Puntuation::~Puntuation()
{
}

bool Puntuation::Init()
{
	return true;
}

bool Puntuation::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	char lookupTable[] = { "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[ç]^_`abcdefghijklmnopqrstuvwxyz{|}¨+" };
	textFont = app->fonts->Load("Assets/Fonts/font2.png", lookupTable, 5);
	

	return ret;

}

bool Puntuation::Update(float dt)	
{
	sprintf_s(scoreText, 10, "%d", score);  
	app->fonts->BlitText(228, 40, textFont, scoreText);

	return true;
}

bool Puntuation::PostUpdate()
{
	return true;
}

bool Puntuation::CleanUp()
{
	
	return true;
}



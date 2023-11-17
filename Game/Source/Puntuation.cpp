#include "Puntuation.h"
#include "App.h"
#include "Textures.h"
#include "ModuleFonts.h"
#include <stdio.h>
#include "SDL_image/include/SDL_image.h"
#include <sstream>
#include <string.h>
#include "Log.h"

#include "Box2D/Box2D/Box2D.h"

Puntuation::Puntuation() : Module()
{
}

Puntuation::~Puntuation()
{
}

bool Puntuation::Awake(pugi::xml_node& config)
{
	SString docPath = config.child("filePath").attribute("path").as_string(); // TODO sin terminar
	scoreDoc.load_file(docPath.GetString());
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

	return true;
}

bool Puntuation::PostUpdate()
{
	scoreText = std::to_string(score).c_str();
	app->fonts->BlitText(225, 40, textFont, scoreText.GetString());
	return true;
}

bool Puntuation::CleanUp()
{
	return true;
}

void Puntuation::AddScore(int s)
{
	score = s;
	b2Clamp(score, 0, 99999);
}

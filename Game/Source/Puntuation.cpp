#include "Puntuation.h"
#include "App.h"
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

	scoreText = new char[MAX_SCORE_LENGTH];

	score = 000;
	numeracion = 0;
	
	
	


	return ret;

}

bool Puntuation::Update()
{
	return true;
}

bool Puntuation::PostUpdate()
{
	sprintf_s(scoreText, MAX_SCORE_LENGTH, "%7d", score);
	//App->fonts->BlitText(0, 16, *textFont, scoreText);
	return true;
}

bool Puntuation::CleanUp()
{
	if (scoreText != nullptr) {
		delete scoreText;
		scoreText = nullptr;
	}
	return true;
}



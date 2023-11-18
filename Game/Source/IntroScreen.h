#pragma once

#include "Module.h"


#include "SDL/include/SDL.h"

class IntroScreen : public Module
{
public:
	IntroScreen(bool startEnabled = true);
	~IntroScreen();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	SDL_Texture* IntroJuegoTexture;


};

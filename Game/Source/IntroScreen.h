#pragma once

#include "Module.h"


#include "SDL/include/SDL.h"

class IntroScreen : public Module
{
public:
	IntroScreen();
	~IntroScreen();

	bool Start() override;
	bool Update();
	bool CleanUp();

	SDL_Texture* IntroJuegoTexture;


};

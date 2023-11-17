#pragma once

#include "Module.h"


#include "SDL/include/SDL.h"

class LoseScreen : public Module
{
public:
	LoseScreen();
	~LoseScreen();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	SDL_Texture* LoseTexture;

};

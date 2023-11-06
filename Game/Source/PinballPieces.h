#pragma once
#include "Module.h"
#include "List.h"
#include "Point.h"

struct SDL_Texture;

class PinballPieces : public Module
{
public:

	PinballPieces();

	virtual ~PinballPieces();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();


private:
	SDL_Texture* palaI;
	SDL_Texture* palaD;
	SDL_Texture* muelle;
	SDL_Texture* impulso;
	SDL_Texture* p10;
	SDL_Texture* p20;
	SDL_Texture* p30;
	SDL_Texture* trianguloD;
	SDL_Texture* trianguloI;
	SDL_Texture* cuadrado_webon;
	SDL_Texture* recta;
	//SDL_Texture* trianguloD; // redefinido
	SDL_Texture* estrella;
	SDL_Texture* u_feliz;
	SDL_Texture* x2;
	SDL_Texture* x3;
	SDL_Texture* x5;
	SDL_Texture* paredI;
	SDL_Texture* paredD;
};




#ifndef __PUNTUATION_H__
#define __PUNTUATION_H__

#include "Module.h"
#include "Defs.h"
#include "SDL/include/SDL.h"
#include "Input.h"

#define MAX_SCORE_LENGTH 10

class Puntuation : public Module
{
public:
	// Constructor
	Puntuation();    

	// Destructor
	~Puntuation();   
	
	bool Init();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	int score = 10000;
	int numeracion;

	int scoreFont2;
	int textFont = -1;
	char scoreText[10] = { "\0" };
	
  
};

#endif //__PUNTUATION_H__
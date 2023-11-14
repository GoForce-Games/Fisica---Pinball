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



	/*int score;
	int numeracion;

	int *scoreFont;
	int *textFont;
	char* scoreText = nullptr;*/
	
  
};

#endif //__PUNTUATION_H__
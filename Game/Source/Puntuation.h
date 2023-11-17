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
	
	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void AddScore(int score);

private:

	int score = 00000;
	int topScore = 00000;
	int highScore = 00000;

	

	int textFont = -1;
	SString scoreText;
	
	pugi::xml_document scoreDoc;
  
};

#endif //__PUNTUATION_H__
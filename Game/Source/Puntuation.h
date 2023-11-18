#ifndef __PUNTUATION_H__
#define __PUNTUATION_H__

#include "Module.h"
#include "Defs.h"
#include "SDL/include/SDL.h"
#include "Input.h"
#include "List.h"
#include "ScoreStruct.h"

#define MAX_SCORE_LENGTH 10

class Puntuation : public Module
{
public:
	// Constructor
	Puntuation(bool startEnabled = true);    

	// Destructor
	~Puntuation();   
	
	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void AddScore(int score);

	void SortScore();

	void SaveScore();

private:

	ScoreStruct* score = nullptr;
	ScoreStruct* topScore = nullptr;
	ScoreStruct* highScore = nullptr;

	List<ScoreStruct*> scoreList;

	int textFont = -1;
	SString scoreText;
	int scoreFx;
	
	SString filePath;
	pugi::xml_document scoreDoc;
	pugi::xml_node scoreRootNode;
  
};

#endif //__PUNTUATION_H__
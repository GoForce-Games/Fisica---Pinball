#include "Puntuation.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "ModuleFonts.h"
#include <stdio.h>
#include "SDL_image/include/SDL_image.h"
#include <sstream>
#include <string.h>
#include "Log.h"

#include "Box2D/Box2D/Box2D.h"

Puntuation::Puntuation(bool startEnabled) : Module(startEnabled)
{
	name.Create("scoreModule");
}

Puntuation::~Puntuation()
{
}

bool Puntuation::Awake(pugi::xml_node& config)
{
	LOG("Loading score file...");

	filePath = config.attribute("path").as_string(); // TODO sin terminar
	scoreDoc.load_file(filePath.GetString());
	scoreRootNode = scoreDoc.child("scores");
	if (scoreRootNode && scoreRootNode.first_child()) {
		for(pugi::xml_node scoreNode = scoreRootNode.child("score"); scoreNode != NULL; scoreNode = scoreNode.next_sibling("score"))
		{
			scoreList.Add(new ScoreStruct(scoreNode.attribute("name").as_string(), scoreNode.attribute("value").as_int()));
			scoreNode.parent().remove_child(scoreNode);
		}
		scoreList.BubbleSort();
		scoreRootNode.remove_children();
	}

	scoreFx = app->audio->LoadFx("Assets/Audio/Fx/score.wav");

	score = new ScoreStruct();

	return true;
}

bool Puntuation::Start()
{

	bool ret = true;

	//TODO cargar y guardar puntuaciones

	char lookupTable[] = { "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[ç]^_`abcdefghijklmnopqrstuvwxyz{|}¨+" };
	textFont = app->fonts->Load("Assets/Fonts/font2.png", lookupTable, 5);
	

	return ret;

}

bool Puntuation::Update(float dt)	
{
	scoreText = std::to_string(score->score).c_str();
	app->fonts->BlitText(225, 40, textFont, scoreText.GetString());
	return true;
}

bool Puntuation::PostUpdate()
{
	return true;
}

bool Puntuation::CleanUp()
{
	SaveScore();

	for (ListItem<ScoreStruct*>* item = scoreList.start; item; item = item->next)
	{
		RELEASE(item->data);
	}

	scoreList.Clear();

	return true;
}

void Puntuation::AddScore(int s)
{
	score->score += s;
	b2Clamp(score->score, 0, 99999);
	app->audio->PlayFx(scoreFx);
}

//Sort by score using bubblesort
void Puntuation::SortScore()
{
	bool swapped = true;

	while (swapped)
	{
		swapped = false;
		ListItem<ScoreStruct*>* tmp = scoreList.start;

		while (tmp != NULL && tmp->next != NULL)
		{
			if (tmp->data->score > tmp->next->data->score)
			{
				SWAP(tmp->data, tmp->next->data);
				swapped = true;
			}

			tmp = tmp->next;
		}
	}
}

void Puntuation::SaveScore()
{
	scoreList.Add(score);
	SortScore();


	for (ListItem<ScoreStruct*>* item = scoreList.end; item && item->data; item = item->prev)
	{
		pugi::xml_node scoreNode = scoreRootNode.append_child("score");
		scoreNode.append_attribute("name").set_value(item->data->name.GetString());
		scoreNode.append_attribute("value").set_value(item->data->score);
	}
	scoreDoc.save_file(filePath.GetString());
}

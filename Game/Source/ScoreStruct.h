#pragma once
#include "SString.h"

struct ScoreStruct
{
	SString name;
	int score;

	ScoreStruct(SString _name = "", int _score = 0) : name(_name), score(_score) {}
};
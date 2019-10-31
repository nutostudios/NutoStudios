#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModuleUi : public Module
{
public:
	ModuleUi(Application* app, bool start_enabled = true);
	virtual ~ModuleUi();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	int cont = 0;
	int font = 0;
	int Score = 0;
	int LastScore = 0;
	int HighScore = 0;
	int Aux[100];
	char score[10];
	char lastscore[10];
	char highscore[10];
};

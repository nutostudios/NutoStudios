#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = false);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	void StartBall();
	void SpawnNextBall();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
public:

	
	
	PhysBody * ball;
	iPoint BallPosition;
	bool DesappearBall = false;
	bool Hole1 = false;
	bool Hole2 = false;
	bool Hole3 = false;
	bool Hole4 = false;
	int bonus = 1;
	int lives = 3;
	bool jointOn = true;
	uint bouncer;
	uint NewBall;
	uint enthole;
	uint exhole;
	uint triangle;
	uint lightfx;
};
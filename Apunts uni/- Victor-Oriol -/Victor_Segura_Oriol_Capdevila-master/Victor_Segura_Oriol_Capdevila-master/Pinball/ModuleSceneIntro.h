#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = false);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void ContinueAfterHole();
	void ControlTime();
	void BlitAll();
	void ControlBall();
	void LightsOff();

public:
	
	PhysBody* pivote;
	PhysBody* pivote2;
	PhysBody* pivote3;
	PhysBody* pivote4;
	PhysBody* BarraL;
	PhysBody* BarraR;
	PhysBody* Background;
	PhysBody* BackgroundU;
	PhysBody* BackgroundL;
	PhysBody* BackgroundR;
	PhysBody* BackgroundLine;
	PhysBody* ChainL;
	PhysBody* ChainR;
	PhysBody* BouncyL;
	PhysBody* BouncyR;
	PhysBody* RightCircle;
	PhysBody* RightFlipper;
	PhysBody* LeftCircle;
	PhysBody* LeftFlipper; 
	PhysBody* UpCircle;
	PhysBody* UpFlipper;
	PhysBody* Hole1;
	PhysBody* Hole2;
	PhysBody* Hole3;
	PhysBody* Hole4;
	PhysBody* Barrier = nullptr;
	PhysBody* Barrier2 = nullptr;
	PhysBody* Light1;
	PhysBody* Light2;
	PhysBody* Light3;
	PhysBody* Light4;
	PhysBody* Light5;
	PhysBody* Light6;
	PhysBody* Light7;
	PhysBody* Light8;
	PhysBody* Light9;
	PhysBody* Light10;

	//Sensors
	PhysBody* CircleUp;
	PhysBody* CircleUp2;
	PhysBody* CircleUp3;
	PhysBody* SensorUp;
	PhysBody* SensorUp2;
	PhysBody* Space;
	PhysBody* Space2;
	PhysBody* barrier;
	PhysBody* Joint;

	SDL_Texture* up;
	SDL_Texture* right;
	SDL_Texture* left;
	SDL_Texture* circle;
	SDL_Texture* background;
	SDL_Texture* flipper;
	SDL_Texture* flipper2;
	SDL_Texture* lightUp;
	SDL_Texture* lightLeft;
	SDL_Texture* lightU;
	SDL_Texture* lightRight;
	SDL_Texture* bouncerHit = nullptr;
	SDL_Texture* Num0 = nullptr;
	SDL_Texture* Num1 = nullptr;
	SDL_Texture* Num2 = nullptr;
	SDL_Texture* Num3 = nullptr;
	SDL_Texture* CircleLight = nullptr;
	SDL_Texture* k5 = nullptr;
	SDL_Texture* k10 = nullptr;
	SDL_Texture* k20 = nullptr;
	SDL_Texture* bonusx2 = nullptr;
	SDL_Texture* bonusx4 = nullptr;
	SDL_Texture* bonusx6 = nullptr;
	SDL_Texture* bonusx8 = nullptr;
	SDL_Texture* bonusx10 = nullptr;
	SDL_Texture* bonusHeld= nullptr;
	SDL_Texture* P = nullptr;
	SDL_Texture* PI = nullptr;
	SDL_Texture* PIN = nullptr;
	SDL_Texture* PINB = nullptr;
	SDL_Texture* PINBA = nullptr;
	SDL_Texture* PINBAL = nullptr;
	SDL_Texture* PINBALL = nullptr;
	SDL_Texture* PINBALLD = nullptr;
	SDL_Texture* PINBALLDR = nullptr;
	SDL_Texture* PINBALLDROI = nullptr;
	SDL_Texture* PINBALLDROID = nullptr;
	SDL_Texture* BARRIER2 = nullptr;

	bool OnLight1 = false;
	bool OnLight2 = false;
	bool OnLight3 = false;
	bool OnLight4 = false;
	bool OnLight5 = false;
	bool OnLight6 = false;
	bool OnLight7 = false;
	bool OnLight8 = false;
	bool OnLight9 = false;
	bool OnLight10 = false;

	bool BArrier = false;

	bool Circleup1 = false;
	bool Circleup2 = false;
	bool Circleup3 = false;
	bool ActiveBonus = false;

	bool Barrier2Created = false;
	bool BarrierBonus = false;
	bool BallInHole = false;

	bool ActiveHole1 = false;
	bool ActiveHole2 = false;
	bool ActiveHole3 = false;
	bool ActiveHole4 = false;

	bool space = true;

	bool Right = false;
	bool Left = false;
	bool Up = false;


	float velocity = 5.0f;
	bool StartVelocity = false;
	bool BallIsStopped = false;

	
	uint bonus_fx;
	uint flipperfx;
	uint song;
	uint time = 0;
	uint TimeUp = 0;
	uint TimeRight = 0;
	uint TimeLeft = 0;
	uint TimeLight1 = 0;
	uint StopTime = 2000;
	uint pinball = 0;
	uint contbonus = 0;
	uint timepivote = 0;
	uint timepivote2 = 0;
	uint timepivote3 = 0;
	uint timepivote4 = 0;

	bool printp = false;
	bool printp2 = false;
	bool printp3 = false;
	bool printp4 = false;

};

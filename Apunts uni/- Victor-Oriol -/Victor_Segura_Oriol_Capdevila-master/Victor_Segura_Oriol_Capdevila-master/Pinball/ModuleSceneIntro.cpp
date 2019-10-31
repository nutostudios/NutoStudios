#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "ModuleUi.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	background = App->textures->Load("pinball/Background.png");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	lightUp = App->textures->Load("pinball/Up_Light.png");
	up = App->textures->Load("pinball/Up.png");
	right = App->textures->Load("pinball/left.png");
	left = App->textures->Load("pinball/right.png");
	lightU = App->textures->Load("pinball/U_light.png");
	lightLeft = App->textures->Load("pinball/L_Light.png");
	lightRight = App->textures->Load("pinball/R_light.png");
	circle = App->textures->Load("pinball/Bola.png"); 
	flipper = App->textures->Load("pinball/Resorte.png");
	flipper2 = App->textures->Load("pinball/Resorte2.png");
	Num0 = App->textures->Load("pinball/Numbers0.png");
	Num1 = App->textures->Load("pinball/Numbers1.png");
	Num2 = App->textures->Load("pinball/Numbers2.png");
	Num3 = App->textures->Load("pinball/Numbers3.png");
	CircleLight = App->textures->Load("pinball/Active_Point.png");
	k5 = App->textures->Load("pinball/5k.png");
	k10 = App->textures->Load("pinball/10k.png");
	k20 = App->textures->Load("pinball/20k.png");
	P = App->textures->Load("pinball/P.png");
	PI = App->textures->Load("pinball/PI.png");
	PIN = App->textures->Load("pinball/PIN.png");
	PINB = App->textures->Load("pinball/PINB.png");
	PINBA = App->textures->Load("pinball/PINBA.png");
	PINBAL = App->textures->Load("pinball/PINBAL.png");
	PINBALL = App->textures->Load("pinball/PINBALL.png");
	PINBALLD = App->textures->Load("pinball/PINBALLD.png");
	PINBALLDR = App->textures->Load("pinball/PINBALLDR.png");
	PINBALLDROI = App->textures->Load("pinball/PINBALLDOI.png");
	PINBALLDROID = App->textures->Load("pinball/PINBALLDOID.png");
	bouncerHit = App->textures->Load("pinball/Bouncer Hit.png");
	bonusx2 = App->textures->Load("pinball/x2 Bonus.png");
	bonusx4 = App->textures->Load("pinball/x4 Bonus.png");
	bonusx6 = App->textures->Load("pinball/x6 Bonus.png");
	bonusx8 = App->textures->Load("pinball/x8 Bonus.png");
	bonusx10 = App->textures->Load("pinball/x10 Bonus.png");
	bonusHeld = App->textures->Load("pinball/Held Bonus.png");
	BARRIER2 = App->textures->Load("pinball/Barrier2.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	song = App->audio->PlayMusic("pinball/song.ogg");
	flipperfx = App->audio->LoadFx("pinball/flipper.wav");
	App->physics->CreatePinballWalls();
	App->physics->CreateSensors();
	RightCircle = new PhysBody;
	RightFlipper = new PhysBody;
	App->physics->CreateFlipper(RightFlipper, RightCircle, true, false);
	LeftCircle = new PhysBody;
	LeftFlipper = new PhysBody;
	App->physics->CreateFlipper(LeftFlipper, LeftCircle, false, false);
	UpCircle = new PhysBody;
	UpFlipper = new PhysBody;
	App->physics->CreateFlipper(UpFlipper, UpCircle, false, true);
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background);
	App->textures->Unload(circle);
	App->textures->Unload(flipper);
	App->textures->Unload(flipper2);
	App->textures->Unload(lightUp);
	App->textures->Unload(lightLeft);
	App->textures->Unload(lightU);
	App->textures->Unload(lightRight);
	App->textures->Unload(Num0);
	App->textures->Unload(Num1);
	App->textures->Unload(Num2);
	App->textures->Unload(Num3);
	App->textures->Unload(CircleLight);
	App->textures->Unload(left);
	App->textures->Unload(right);
	App->textures->Unload(k5);
	App->textures->Unload(k10);
	App->textures->Unload(k20);
	App->textures->Unload(up);
	App->textures->Unload(bouncerHit);
	App->textures->Unload(P);
	App->textures->Unload(PI);
	App->textures->Unload(PIN);
	App->textures->Unload(PINB);
	App->textures->Unload(PINBA);
	App->textures->Unload(PINBAL);
	App->textures->Unload(PINBALL);
	App->textures->Unload(PINBALLD);
	App->textures->Unload(PINBALLDR);
	App->textures->Unload(PINBALLDROI);
	App->textures->Unload(PINBALLDROID);
	App->textures->Unload(bonusx2);
	App->textures->Unload(bonusx4);
	App->textures->Unload(bonusx6);
	App->textures->Unload(bonusx8);
	App->textures->Unload(bonusx10);
	App->textures->Unload(bonusHeld);
	App->textures->Unload(BARRIER2);

	return true;
}


update_status ModuleSceneIntro::Update()
{
	
	if (OnLight2 && OnLight3 && OnLight4) {
		OnLight2 = false;
		OnLight3 = false;
		OnLight4 = false;
		pinball++;
		App->ui->Score += 1500 * App->player->bonus;
	}
	if (OnLight5 && OnLight6 && OnLight7) {
		OnLight5 = false;
		OnLight6 = false;
		OnLight7 = false;
		pinball++;
		App->ui->Score += 1500 * App->player->bonus;
	}
	if (OnLight8 && OnLight9 && OnLight10) {
		OnLight8 = false;
		OnLight9 = false;
		OnLight10 = false;
		pinball++;
		App->ui->Score += 1500 * App->player->bonus;
	}
	if (!OnLight1 && Barrier == nullptr && !BarrierBonus && !BallInHole) {
		int barrier[8] = {
			5, 672,
			36, 694,
			33, 697,
			5, 677
		};
		Barrier = App->physics->CreateChain(0, 0, barrier, 8, b2BodyType::b2_staticBody);
		BarrierBonus = true;
	}
	if (OnLight1 && BarrierBonus) {
		App->physics->DestroyBody(Barrier);
		BarrierBonus = false;
		Barrier = nullptr;
	}

	
	if (Circleup1 && Circleup2 && Circleup3) {
		contbonus++;
		ActiveBonus = true;
		Circleup1 = false;
		Circleup2 = false;
		Circleup3 = false;
		App->audio->PlayFx(bonus_fx);
	}
	
	
	fVector normal(0.0f, 0.0f);

	if (BallIsStopped)
	{
		ContinueAfterHole();
	}

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	BlitAll();
	ControlTime();
	ControlBall();
	if (BArrier && Barrier2 == nullptr && !Barrier2Created) {
		Barrier2 = App->physics->CreateRectangle(388, 18, 5, 28);
		Barrier2Created = true;
	}
	return UPDATE_CONTINUE;
}

// TODO 8: Now just define collision callback for the circle and play bonus_fx audio
void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
}

void ModuleSceneIntro::ContinueAfterHole()
{
	if (SDL_GetTicks() - time >= StopTime && (App->player->Hole1 || App->player->Hole2 || App->player->Hole3 || App->player->Hole4)) {
		BallIsStopped = false;
		App->player->StartBall();
		}
}

void ModuleSceneIntro::ControlTime()
{
	if (TimeUp >= 2000 && TimeUp > 0) {
		TimeUp = 0;
		Up = false;
	}
	if (TimeLeft >= 2000 && TimeLeft > 0) {
		TimeLeft = 0;
		Left = false;
	}
	if (TimeRight >= 2000 && TimeRight > 0) {
		TimeRight = 0;
		Right = false;
	}
	if (TimeLight1 >= 2000 && TimeLight1 > 0) {
		TimeLight1 = 0;
		OnLight1 = false;
	}
	if (timepivote >= 10) {
		timepivote = 0;
		printp = false;
	}
	if (timepivote2 >= 10) {
		timepivote2 = 0;
		printp2 = false;
	}
	if (timepivote3 >= 10) {
		timepivote3 = 0;
		printp3 = false;
	}
	if (timepivote4 >= 10) {
		timepivote4 = 0;
		printp4 = false;
	}

}

void ModuleSceneIntro::BlitAll()
{
	//RENDERER
	int x, y;
	if (pinball == 1) 
		App->renderer->Blit(P, -1, 0, NULL);
	else if (pinball == 2) 
		App->renderer->Blit(PI, -1, 0, NULL);
	else if (pinball == 3) 
		App->renderer->Blit(PIN, -1, 0, NULL);
	else if (pinball == 4) 
		App->renderer->Blit(PINB, -1, 0, NULL);
	else if (pinball == 5) 
		App->renderer->Blit(PINBA, -1, 0, NULL);
	else if (pinball == 6) 
		App->renderer->Blit(PINBAL, -1, 0, NULL);
	else if (pinball == 7) 
		App->renderer->Blit(PINBALL, -1, 0, NULL);
	else if (pinball == 8) 
		App->renderer->Blit(PINBALLD, -1, 0, NULL);
	else if (pinball == 9) 
		App->renderer->Blit(PINBALLDR, -1, 0, NULL);
	else if (pinball == 10) 
		App->renderer->Blit(PINBALLDROI, -1, 0, NULL);
	else if (pinball >= 11) 
		App->renderer->Blit(PINBALLDROID, -1, 0, NULL);
	else App->renderer->Blit(background, -1, 0, NULL);
		
	if (Barrier2Created) 
		App->renderer->Blit(BARRIER2, 385, 4, NULL, 1.0f);

	LeftFlipper->GetPosition(x, y);
	App->renderer->Blit(flipper2, x - 43, y - 20, NULL, 1.0f, LeftFlipper->GetRotation());
	RightFlipper->GetPosition(x, y);
	App->renderer->Blit(flipper, x - 35, y - 20, NULL, 1.0f, RightFlipper->GetRotation());
	UpFlipper->GetPosition(x, y);
	App->renderer->Blit(flipper2, x - 43, y - 20, NULL, 1.0f, UpFlipper->GetRotation());
	if (OnLight1) {
		App->renderer->Blit(lightUp, 169, 61, NULL, 1.0f);
		App->renderer->Blit(k20, 2, 614, NULL, 1.0f);
		TimeLight1++;
	}
	if (OnLight2)
		App->renderer->Blit(lightLeft, 70, 343, NULL, 1.0f);
	if (OnLight3)
		App->renderer->Blit(lightLeft, 80, 326, NULL, 1.0f);
	if (OnLight4)
		App->renderer->Blit(lightLeft, 90, 307, NULL, 1.0f);
	if (OnLight5)
		App->renderer->Blit(lightU, 266, 225, NULL, 1.0f);
	if (OnLight6)
		App->renderer->Blit(lightU, 281, 240, NULL, 1.0f);
	if (OnLight7)
		App->renderer->Blit(lightU, 295, 255, NULL, 1.0f);
	if (OnLight8)
		App->renderer->Blit(lightRight, 393, 395, NULL, 1.0f);
	if (OnLight9)
		App->renderer->Blit(lightRight, 403, 413, NULL, 1.0f);
	if (OnLight10)
		App->renderer->Blit(lightRight, 412, 431, NULL, 1.0f);
	if (Circleup1)
		App->renderer->Blit(CircleLight, 264, 65, NULL, 1.0f);
	if (Circleup2)
		App->renderer->Blit(CircleLight, 304, 65, NULL, 1.0f);
	if (Circleup3)
		App->renderer->Blit(CircleLight, 344, 65, NULL, 1.0f);
#ifdef _DEBUG //We don't know why in release this isn't needed and in debug it is.
	if (ActiveHole1)
		App->renderer->Blit(circle, 189, 15, NULL, 1.0f);
	if (ActiveHole2)
		App->renderer->Blit(circle, 384, 55, NULL, 1.0f);
	if (ActiveHole3)
		App->renderer->Blit(circle, 399, 244, NULL, 1.0f);
	if (ActiveHole4)
		App->renderer->Blit(circle, 5, 704, NULL, 1.0f);

#endif 
	
	if (Right) {
		App->renderer->Blit(right, 96, 21, NULL, 1.0f);
		App->renderer->Blit(k5, 388, 91, NULL, 1.0f);
		TimeRight++;
	}
	if (Left) {
		App->renderer->Blit(left, 4, 21, NULL, 1.0f);
		App->renderer->Blit(k5, 192, 46, NULL, 1.0f);
		TimeLeft++;
	}
	if (Up) {
		App->renderer->Blit(up, 10, 232, NULL, 1.0f);
		App->renderer->Blit(k10, 381, 266, NULL, 1.0f);
		TimeUp++;
	}
	if (ActiveBonus) {
		if (contbonus == 1) {
			App->renderer->Blit(bonusx2, 80, 626, NULL, 1.0f);
			App->player->bonus = 2;
		}
		else if (contbonus == 2) {
			App->renderer->Blit(bonusx2, 80, 626, NULL, 1.0f);
			App->renderer->Blit(bonusx4, 109, 649, NULL, 1.0f);
			App->player->bonus = 4;
		}
		else if (contbonus == 3) {
			App->renderer->Blit(bonusx2, 80, 626, NULL, 1.0f);
			App->renderer->Blit(bonusx4, 109, 649, NULL, 1.0f);
			App->renderer->Blit(bonusx6, 139, 670, NULL, 1.0f);
			App->player->bonus = 6;
		}
		else if (contbonus == 4) {
			App->renderer->Blit(bonusx2, 80, 626, NULL, 1.0f);
			App->renderer->Blit(bonusx4, 109, 649, NULL, 1.0f);
			App->renderer->Blit(bonusx6, 139, 670, NULL, 1.0f);
			App->renderer->Blit(bonusx8, 314, 670, NULL, 1.0f);
			App->player->bonus = 8;
		}
		else if (contbonus == 5) {
			App->renderer->Blit(bonusx2, 80, 626, NULL, 1.0f);
			App->renderer->Blit(bonusx4, 109, 649, NULL, 1.0f);
			App->renderer->Blit(bonusx6, 139, 670, NULL, 1.0f);
			App->renderer->Blit(bonusx8, 314, 670, NULL, 1.0f);
			App->renderer->Blit(bonusx10, 340, 650, NULL, 1.0f);
			App->player->bonus = 10;
		}
		else  {
			App->renderer->Blit(bonusx2, 80, 626, NULL, 1.0f);
			App->renderer->Blit(bonusx4, 109, 649, NULL, 1.0f);
			App->renderer->Blit(bonusx6, 139, 670, NULL, 1.0f);
			App->renderer->Blit(bonusx8, 314, 670, NULL, 1.0f);
			App->renderer->Blit(bonusx10, 340, 650, NULL, 1.0f);
			App->renderer->Blit(bonusHeld, 370, 630, NULL, 1.0f);
			App->player->bonus = 20;
		}
	}
	if (printp) {
		App->renderer->Blit(bouncerHit, 255, 141, NULL, 1.0f);
		timepivote++;
	}
	if (printp2) {
		App->renderer->Blit(bouncerHit, 330, 141, NULL, 1.0f);
		timepivote2++;
	}
	if (printp3) {
		App->renderer->Blit(bouncerHit, 52, 71, NULL, 1.0f);
		timepivote3++;
	}
	if (printp4) {
		App->renderer->Blit(bouncerHit, 284, 210, NULL, 1.0f);
		timepivote4++;
	}

	
	
	
	if (App->ui->Score >= 10 && App->ui->Score <100) {
		App->fonts->BlitText(205, 773, App->ui->font, App->ui->score);
	}
	else if (App->ui->Score >= 100 && App->ui->Score < 1000) {
		App->fonts->BlitText(195, 773, App->ui->font, App->ui->score);
	}
	else if (App->ui->Score >= 1000 && App->ui->Score < 10000) {
		App->fonts->BlitText(185, 773, App->ui->font, App->ui->score);
	}
	else if (App->ui->Score >= 10000) {
		App->fonts->BlitText(175, 773, App->ui->font, App->ui->score);
	}
	else App->fonts->BlitText(215, 773, App->ui->font, App->ui->score);

	if (App->player->ball)
		App->renderer->Blit(circle, App->player->BallPosition.x, App->player->BallPosition.y, NULL, 1.0f, App->player->ball->GetRotation());
}

void ModuleSceneIntro::ControlBall()
{
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		RightFlipper->body->ApplyTorque(500, true);
		App->audio->PlayFx(flipperfx);
	}
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		LeftFlipper->body->ApplyTorque(-500, true);
		UpFlipper->body->ApplyTorque(-500, true);
		App->audio->PlayFx(flipperfx);
	}
	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) && space)
		App->player->ball->body->ApplyLinearImpulse({ 0, -0.8f }, { 0,0 }, true, false);

}

void ModuleSceneIntro::LightsOff()
{
	OnLight1 = false;
	OnLight2 = false;
	OnLight3 = false;
	OnLight4 = false;
	OnLight5 = false;
	OnLight6 = false;
	OnLight7 = false;
	OnLight8 = false;
	OnLight9 = false;
	OnLight10 = false;

	Circleup1 = false;
	Circleup2 = false;
	Circleup3 = false;
}

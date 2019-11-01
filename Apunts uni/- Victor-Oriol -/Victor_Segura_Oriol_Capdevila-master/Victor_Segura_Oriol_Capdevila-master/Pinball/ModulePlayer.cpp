#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFinish.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleUi.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}


// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	BallPosition.x = 454;
	BallPosition.y = 421;
	lives = 3;
	DesappearBall = false;
	Hole1 = false;
	
	bouncer = App->audio->LoadFx("pinball/Bouncers.wav");
	triangle = App->audio->LoadFx("pinball/Triangle.wav");
	enthole = App->audio->LoadFx("pinball/Enter Hole.wav");
	exhole = App->audio->LoadFx("pinball/Exit Hole.wav");
	NewBall = App->audio->LoadFx("pinball/New ball.wav");
	lightfx = App->audio->LoadFx("pinball/Light.wav");

	return true;
}

update_status ModulePlayer::PreUpdate()
{

	ball->GetPosition(BallPosition.x, BallPosition.y);
	
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	BallPosition.x = 454;
	BallPosition.y = 421;
	lives = 3;
	DesappearBall = false;
	Hole1 = false;

	return true;
}

void ModulePlayer::StartBall()
{
	App->scene_intro->ActiveHole1 = false;
	App->scene_intro->ActiveHole2 = false;
	App->scene_intro->ActiveHole3 = false;
	App->scene_intro->ActiveHole4 = false;

	if (Hole1) {
		ball = App->physics->CreateCircle(210, 40, 15, b2BodyType::b2_dynamicBody);
		ball->listener = this;
		ball->body->ApplyLinearImpulse({ 0,5 }, { 0,0 }, true, false);
		ball->body->SetBullet(false);
		Hole1 = false;
		App->audio->PlayFx(exhole);
	}
	else if (Hole2) {
		ball = App->physics->CreateCircle(395, 80, 15, b2BodyType::b2_dynamicBody);
		ball->listener = this;
		ball->body->ApplyLinearImpulse({ 2,6 }, { 0,0 }, true, false);
		ball->body->SetBullet(false);
		Hole2 = false;
		App->audio->PlayFx(exhole);
	}
	else if (Hole3) {
		ball = App->physics->CreateCircle(400, 270, 15, b2BodyType::b2_dynamicBody);
		ball->listener = this;
		ball->body->ApplyLinearImpulse({ -3,5 }, { 0,0 }, true, false);
		ball->body->SetBullet(false);
		Hole3 = false;
		App->audio->PlayFx(exhole);
	}
	else if (Hole4) {
		ball = App->physics->CreateCircle(23, 700, 15, b2BodyType::b2_dynamicBody);
		ball->listener = this;
		ball->body->ApplyLinearImpulse({ 13,0 }, { 0,0 }, true, true);
		ball->body->SetBullet(false);
		Hole4 = false;
		App->scene_intro->BallInHole = false;
		App->audio->PlayFx(exhole);
	}
	else {
		ball = App->physics->CreateCircle(454, 421, 15, b2BodyType::b2_dynamicBody);
		ball->listener = this;
		ball->body->SetBullet(false);
		App->audio->PlayFx(NewBall);
	}
}

void ModulePlayer::SpawnNextBall()
{
	lives--;
	App->physics->DestroyBody(App->scene_intro->Barrier2);
	App->scene_intro->Barrier2Created = false;
	App->scene_intro->Barrier2 = nullptr;
	App->scene_intro->LightsOff();
	App->scene_intro->space = true;
	App->scene_intro->contbonus = 0;
	bonus = 1;
	App->scene_intro->BArrier = false;
	App->physics->DestroyBody(ball);
	App->scene_intro->ActiveBonus = false;
	if (lives >= 0) {
		StartBall();
	}
	else {
		App->scene_intro->pinball = 0;
		App->finish->Enable();
		App->fade->FadeToBlack(this, (Module*)App->finish, 0.0f);
	}


}




// Update: draw background
update_status ModulePlayer::Update()
{
	if (lives == 3) {
		App->renderer->Blit(App->scene_intro->Num3, 455, 710,NULL);
	}
	else if (lives == 2) {
		App->renderer->Blit(App->scene_intro->Num2, 457, 710, NULL);
	}
	else if (lives == 1) {
		App->renderer->Blit(App->scene_intro->Num1, 457, 710, NULL);
	}
	else if (lives == 0) {
		App->renderer->Blit(App->scene_intro->Num0, 458, 710, NULL);
	}
	if (DesappearBall) {
		App->physics->DestroyBody(ball);
		DesappearBall = false;
	}
		
	if (BallPosition.x >= 169 && BallPosition.x <= 308 && BallPosition.y >= 766) {
		if (App->physics->MouseJoint != nullptr) {
			jointOn = false;
		}

		SpawnNextBall();
	}

	//App->physics->SpeedBall(ball);
	return UPDATE_CONTINUE;
}


void ModulePlayer::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyB == App->scene_intro->Hole1) {
		Hole1 = true;
		DesappearBall = true;
		App->scene_intro->BallIsStopped = true;
		App->scene_intro->time = SDL_GetTicks();
		App->scene_intro->ActiveHole1 = true;
		App->audio->PlayFx(enthole);
		if (App->scene_intro->Left)
			App->ui->Score += 5000 * bonus;
		else App->ui->Score += 1000 * bonus;
		jointOn = false;
	}

	if (bodyB == App->scene_intro->Hole2) {
		Hole2 = true;
		DesappearBall = true;
		App->scene_intro->BallIsStopped = true;
		App->scene_intro->time = SDL_GetTicks();
		App->scene_intro->ActiveHole2 = true;
		App->audio->PlayFx(enthole);
		if (App->scene_intro->Right)
			App->ui->Score += 2500 * bonus;
		else App->ui->Score += 500 * bonus;
		jointOn = false;
	}
	/*if (bodyB == App->scene_intro->Joint) {
		jointOn = true;
	}*/
	if (bodyB == App->scene_intro->Hole3) {
		Hole3 = true;
		DesappearBall = true;
		App->scene_intro->BallIsStopped = true;
		App->scene_intro->time = SDL_GetTicks();
		App->scene_intro->ActiveHole3 = true;
		App->audio->PlayFx(enthole);
		if (App->scene_intro->Up)
			App->ui->Score += 10000 * bonus;
		else App->ui->Score += 1000 * bonus;
		jointOn = false;
	}
	if (bodyB == App->scene_intro->Hole4) {
		Hole4 = true;
		DesappearBall = true;
		App->scene_intro->BallIsStopped = true;
		App->scene_intro->time = SDL_GetTicks();
		App->scene_intro->ActiveHole4 = true;
		App->ui->Score += 20000 * bonus;
		App->scene_intro->BallInHole = true;
		App->audio->PlayFx(enthole);
		jointOn = false;
	}
	if (bodyB == App->scene_intro->BouncyL) {
		ball->body->ApplyLinearImpulse({ 2,-2.5f }, { 0,0 }, false, false);
		App->ui->Score += 200 * bonus;
		App->renderer->Blit(App->scene_intro->bouncerHit, 0, 0, NULL,1.0f);
		App->audio->PlayFx(triangle);
	}
	if (bodyB == App->scene_intro->BouncyR) {
		ball->body->ApplyLinearImpulse({ -2,-2.5f }, { 0,0 }, false, false);
		App->ui->Score += 200 * bonus;
		App->audio->PlayFx(triangle);
	}
	if (bodyB == App->scene_intro->Light1) {
 		App->scene_intro->OnLight1 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light2) {
		App->scene_intro->OnLight2 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light3) {
		App->scene_intro->OnLight3 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light4) {
		App->scene_intro->OnLight4 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light5) {
		App->scene_intro->OnLight5 = true;
		App->ui->Score += 500 * bonus; 
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light6) {
		App->scene_intro->OnLight6 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light7) {
		App->scene_intro->OnLight7 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light8) {
		App->scene_intro->OnLight8 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light9) {
		App->scene_intro->OnLight9 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->Light10) {
		App->scene_intro->OnLight10 = true;
		App->ui->Score += 500 * bonus;
		App->audio->PlayFx(lightfx);
	}
	if (bodyB == App->scene_intro->CircleUp) {
		App->scene_intro->Circleup1 = true;
		App->ui->Score += 500 * bonus;
	}
	if (bodyB == App->scene_intro->CircleUp2) {
		App->scene_intro->Circleup2 = true;
		App->ui->Score += 500 * bonus;
	}
	if (bodyB == App->scene_intro->CircleUp3) {
		App->scene_intro->Circleup3 = true;
		App->ui->Score += 500 * bonus;
	}
	if (bodyB == App->scene_intro->SensorUp) {
		int x, y;
		App->scene_intro->SensorUp->GetPosition(x, y);
		if (x > BallPosition.x) {
			App->scene_intro->Left = true;
			App->scene_intro->TimeLeft = 1;
			App->ui->Score += 500 * bonus;
		}
		if (x < BallPosition.x) {
			App->scene_intro->Right = true;
			App->scene_intro->TimeRight = 1;
			App->ui->Score += 500 * bonus;
		}
	}
	if (bodyB == App->scene_intro->SensorUp2) {
		int x, y;
		App->scene_intro->SensorUp2->GetPosition(x, y);
		if (y < BallPosition.y) {
			App->scene_intro->TimeUp = 1;
			App->scene_intro->Up = true;
			App->ui->Score += 500 * bonus;
		}
	}
	if (bodyB == App->scene_intro->pivote2) {
		b2Vec2 vec;
		App->ui->Score += 200 * bonus;
		App->scene_intro->printp2 = true;
		vec = ball->body->GetLinearVelocity();
		ball->body->ApplyLinearImpulse({2.0f,-0.5f}, { 0,0 }, true, false);
		App->audio->PlayFx(bouncer);
	}
	if (bodyB == App->scene_intro->pivote) {
		b2Vec2 vec;
		App->ui->Score += 200 * bonus;
		App->scene_intro->printp = true;
		vec = ball->body->GetLinearVelocity();
		ball->body->ApplyLinearImpulse({ -2.0f,-0.5f }, { 0,0 }, true, false);
		App->audio->PlayFx(bouncer);
	}
	if (bodyB == App->scene_intro->pivote3) {
		b2Vec2 vec;
		App->ui->Score += 200 * bonus;
		App->scene_intro->printp3 = true;
		vec = ball->body->GetLinearVelocity();
		ball->body->ApplyLinearImpulse({ 0.0f,-0.5f }, { 0,0 }, true, false);
		App->audio->PlayFx(bouncer);
	}
	if (bodyB == App->scene_intro->pivote4) {
		b2Vec2 vec;
		App->scene_intro->printp4 = true;
		App->ui->Score += 200 * bonus;
		vec = ball->body->GetLinearVelocity();
		ball->body->ApplyLinearImpulse({ 0.5f, 0.0f }, { 0,0 }, true, false);
		App->audio->PlayFx(bouncer);
	}
	if (bodyB == App->scene_intro->Space) {
		App->scene_intro->space = true;
	}
	if (bodyB == App->scene_intro->Space2) {
		App->scene_intro->space = false;
	}
	if (bodyB == App->scene_intro->barrier) {
		App->scene_intro->BArrier = true;
	}
}

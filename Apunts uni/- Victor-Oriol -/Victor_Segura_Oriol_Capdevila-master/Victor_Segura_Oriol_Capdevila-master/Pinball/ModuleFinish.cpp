#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTutorial.h"
#include "SDL\include\SDL.h"
#include "ModulePhysics.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleFinish.h"
#include "ModuleFonts.h"
#include "ModuleUi.h"
#include "ModuleSceneIntro.h"



ModuleFinish::ModuleFinish(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Finish.PushBack({ 0, 0, 477, 798 });
	Finish.loop = false;
}

ModuleFinish::~ModuleFinish()
{}


bool ModuleFinish::Start()
{
	LOG("Loading space intro");
	finish = App->textures->Load("pinball/Score Screen.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;
	Finish.Reset();




	return true;
}

// UnLoad assets
bool ModuleFinish::CleanUp()
{
	LOG("Unloading space scene");

	App->textures->Unload(finish);
	Finish.Reset();



	return true;
}


update_status ModuleFinish::Update()
{
	//App->ui->LastScore = App->ui->Aux[App->ui->cont];
	SDL_RenderClear(App->renderer->renderer);
	anim = &Finish;
	App->renderer->Blit(finish, 0, 0, &(anim->GetCurrentFrame()));
	
	if (App->ui->Score >= 10 && App->ui->Score <100) {
		App->fonts->BlitText(195, 240, App->ui->font, App->ui->highscore);
	}
	else if (App->ui->Score >= 100 && App->ui->Score < 1000) {
		App->fonts->BlitText(200, 240, App->ui->font, App->ui->highscore);
	}
	else if (App->ui->Score >= 1000 && App->ui->Score < 10000) {
		App->fonts->BlitText(180, 240, App->ui->font, App->ui->highscore);
	}
	else if (App->ui->Score >= 10000) {
		App->fonts->BlitText(170, 240, App->ui->font, App->ui->highscore);
	}
	else App->fonts->BlitText(210, 240, App->ui->font, App->ui->highscore);

	
	if (App->ui->Score >= 10 && App->ui->Score <100) {
		App->fonts->BlitText(200, 450, App->ui->font, App->ui->score);
	}
	else if (App->ui->Score >= 100 && App->ui->Score < 1000) {
		App->fonts->BlitText(190, 450, App->ui->font, App->ui->score);
	}
	else if (App->ui->Score >= 1000 && App->ui->Score < 10000) {
		App->fonts->BlitText(180, 450, App->ui->font, App->ui->score);
	}
	else if (App->ui->Score >= 10000) {
		App->fonts->BlitText(170, 450, App->ui->font, App->ui->score);
	}
	else App->fonts->BlitText(210, 450, App->ui->font, App->ui->score);

	if (App->ui->cont > 1) {
		if (App->ui->LastScore >= 10 && App->ui->LastScore <100) {
			App->fonts->BlitText(200, 350, App->ui->font, App->ui->lastscore);
		}
		else if (App->ui->LastScore >= 100 && App->ui->LastScore < 1000) {
			App->fonts->BlitText(190, 350, App->ui->font, App->ui->lastscore);
		}
		else if (App->ui->LastScore >= 1000 && App->ui->LastScore < 10000) {
			App->fonts->BlitText(180, 350, App->ui->font, App->ui->lastscore);
		}
		else if (App->ui->LastScore >= 10000) {
			App->fonts->BlitText(170, 350, App->ui->font, App->ui->lastscore);
		}
		else App->fonts->BlitText(210, 350, App->ui->font, App->ui->lastscore);
	}
	else App->fonts->BlitText(210, 350, App->ui->font, App->ui->lastscore);



	App->physics->debug = false;
	if (((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) && App->fade->IsFading() == false))
	{
		App->ui->Score = 0;
		App->finish->Disable();
		App->fade->FadeToBlack(this, (Module*)App->tutorial, 0.0f);
	}
	return UPDATE_CONTINUE;
}
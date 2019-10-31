#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Flecha3.h"
#include "ModuleInput.h"
#include <stdio.h>

Flecha3::Flecha3()
{
	idle.PushBack({ 0,0,80,70 });

	hit.PushBack({ 0,0,80,70 });
	hit.PushBack({ 0,70,80,70 });
	hit.PushBack({ 0, 140, 80,70 });
	hit.PushBack({ 0, 210, 80,70 });
	hit.PushBack({ 0, 280, 80,70 });
	hit.PushBack({ 0, 350, 80,70 });
	hit.PushBack({ 0, 420, 80,70 });
	hit.speed = 0.7f;
	hit.loop = false;

	rep.PushBack({ 0, 350, 80,70 });
	rep.PushBack({ 0, 280, 80,70 });
	rep.PushBack({ 0, 210, 80,70 });
	rep.PushBack({ 0, 140, 80,70 });
	rep.PushBack({ 0,70,80,70 });
	rep.speed = 0.7f;
	rep.loop = false;
}

Flecha3::~Flecha3()
{
}

bool Flecha3::Start()
{
	texture = App->textures->Load("Assets/Sprites/Nuevo Resorte_I.png");
	current_animation = &idle;
	position.x = 39;
	position.y = 209;
	return true;
}

bool Flecha3::CleanUp()
{
	App->textures->Unload(texture);

	return true;
}

update_status Flecha3::Update()
{
	bool active = App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_Z] == KEY_STATE::KEY_DOWN;
	bool noactive = App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_UP || App->input->keyboard[SDL_SCANCODE_Z] == KEY_STATE::KEY_UP;
	if (active)
	{
		hit.Reset();
		current_animation = &hit;

	}
	if (noactive) {
		current_animation = &rep;
		rep.Reset();
	}

	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}
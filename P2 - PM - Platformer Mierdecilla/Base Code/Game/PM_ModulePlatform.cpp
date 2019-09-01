#include "Application.h"
#include "PM_ModulePlatform.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"

Platform::Platform(int x, int y) : position(x, y)
{}

Platform::~Platform()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Platform::GetCollider() const
{
	return collider;
}

void Platform::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void Platform::OnCollision(Collider* collider)
{
	App->particles->AddParticle(App->particles->explosion, position.x, position.y);
}
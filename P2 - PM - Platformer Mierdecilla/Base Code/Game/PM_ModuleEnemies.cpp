/*

#include "Application.h"
#include "PM_ModuleEnemies.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"


// Crear 2 collisions en los enemigos y un bool: si el player choca con las dos muere, si choca con la de arriba (saltando) lo mata)


Enemy::Enemy(int x, int y) : position(x, y)
{}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void Enemy::OnCollision(Collider* collider)
{
	App->particles->AddParticle(App->particles->explosion, position.x, position.y);
}

*/
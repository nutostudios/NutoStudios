#include "j1App.h"
#include "Entity.h"
#include "j1Collision.h"
#include "EntityManager.h"
#include "j1Render.h"
#include "j1Scene.h"

Entity::Entity(int x, int y) : position(x, y)
{
}

Entity::~Entity()
{
	if (collider != nullptr) {
		collider = nullptr;
	}
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites)
{


	//todo: this should be done setting to 0 the dt in draw's update ... no time to do it the right way!
}

void Entity::OnCollision(Collider* collider)
{

}

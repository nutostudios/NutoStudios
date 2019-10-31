#include "j1App.h"
#include "j1Render.h"
#include "EntityManager.h"
#include "j1Textures.h"
#include "Entity.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"


j1Entities::j1Entities()
{
	name.create("entities");
}

// Destructor
j1Entities::~j1Entities()
{

}

bool j1Entities::Awake(pugi::xml_node& config)
{
	LOG("Loading Entities from config file");
	bool ret = true;

	spritesheetName.create(config.child("spritesheetSource").attribute("name").as_string());

	return ret;
}

bool j1Entities::Start()
{
	bool ret = true;

	entity_sprites = App->tex->Load(spritesheetName.GetString());

	if (entity_sprites == NULL) {
		LOG("Error loading entities spritesheet!!");
		ret = false;
	}


	if (!ret)
		return false;

	return ret;
}

bool j1Entities::PreUpdate()
{
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i]->to_destroy) {
			delete(entities[i]);
			entities[i] = nullptr;
			if (!entities.RemoveAt(i)) {
				LOG("Error removing entity");
				return false;
			}
		}
	}
	return true;
}

// Called before render is available
bool j1Entities::Update(float dt)
{

	for (int i = 0; i < entities.Count(); i++)
		if (entities[i] != nullptr) entities[i]->Draw(entity_sprites);

	return true;
}

// Called before quitting
bool j1Entities::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(entity_sprites);

	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr) {
			delete(entities[i]);
			entities[i] = nullptr;
			if (!entities.RemoveAt(i)) {
				LOG("Error removing entity");
				return false;
			}
		}
	}

	entities.Clear();

	return true;
}


void j1Entities::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->GetCollider() == c1)
			entities[i]->OnCollision(c2);
}


bool j1Entities::Load(pugi::xml_node& load)
{
	bool ret = true;

	//delete all enemies but the player
	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr) {
			if (entities[i]->type != PLAYER) {
				entities[i]->to_destroy = true;
			}
		}
	}

	if (!load.child("player").empty())
	{
		pugi::xml_node& player_load = load.child("player");
	
	}

	for (pugi::xml_node bat = load.child("bat"); bat && ret; bat = bat.next_sibling("bat"))
		SpawnEntity(bat.child("position").attribute("x").as_float(), bat.child("position").attribute("y").as_float(), BAT);

	for (pugi::xml_node slime = load.child("slime"); slime && ret; slime = slime.next_sibling("slime"))
		SpawnEntity(slime.child("position").attribute("x").as_float(), slime.child("position").attribute("y").as_float(), SLIME);

	for (pugi::xml_node coin = load.child("coin"); coin && ret; coin = coin.next_sibling("coin"))
		SpawnEntity(coin.child("position").attribute("x").as_float(), coin.child("position").attribute("y").as_float(), COIN);

	return ret;
}

bool j1Entities::Save(pugi::xml_node& save) const
{
	bool ret = true;

	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == PLAYER) {
				pugi::xml_node& player_save = save.append_child("player");
				entities[i]->Save(player_save);
			}
		}
	}

	return ret;
}

Player* j1Entities::GetPlayer() const {

	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == PLAYER)
				return (Player*)entities[i];
		}
	}

	return nullptr;

}
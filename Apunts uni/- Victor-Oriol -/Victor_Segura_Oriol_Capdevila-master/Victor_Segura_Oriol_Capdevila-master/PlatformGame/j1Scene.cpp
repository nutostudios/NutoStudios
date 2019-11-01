#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"
#include "Entity.h"
#include "EntityManager.h"
#include "j1Collision.h"

j1Scene::j1Scene() : j1Module()
{
	//App->scene->Disable();
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	
	for (pugi::xml_node map = config.child("mapname"); map; map = map.next_sibling("mapname")) {
		p2SString * data = new p2SString;
		data->create(map.attribute("name").as_string());
		map_name.add(data);
	}
	Song = config.child("song").text().as_string();
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	App->map->Load(map_name.start->data->GetString());
	App->audio->PlayMusic(Song.GetString());
	//active = false;
	//player = App->entitymanager->AddEntity(Entity::EntityTypes::PLAYER);
	
	//player->Awake(App->LoadConfig(App->entitymanager->name.GetString));
	
	//player->Start();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	/*
	if (App->player->NextMap) {
		App->player->NextMap = false;
		KnowMap = 1;
		App->map->ChangeMap(map_name[KnowMap]);
		App->player->Spawn();
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT) {
		App->map->ChangeMap(map_name[KnowMap]);
		App->player->Spawn();
		KnowMap = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		if (KnowMap == 0) {
			App->map->ChangeMap(map_name[KnowMap]);
			App->player->Spawn();
		}
		else if (KnowMap == 1) {
			App->map->ChangeMap(map_name[KnowMap]);
			App->player->Spawn();
		}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT) {
			App->map->ChangeMap(map_name[KnowMap]);
			App->player->Spawn();
			KnowMap = 1;
	}
	*/
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->collision->Disable();
	LOG("Freeing scene");
	return true;
}

bool j1Scene::Load(pugi::xml_node & scene)
{
	KnowMap = scene.child("KnowMap").attribute("value").as_int();

	return true;
}

bool j1Scene::Save(pugi::xml_node & scene) const
{
	scene.append_child("KnowMap").append_attribute("value") = KnowMap;


	return true;
}

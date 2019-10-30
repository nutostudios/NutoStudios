#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)
	//Hardcoded RECT I just want to go to sleep
	SDL_Rect default_Rect;
	default_Rect.x = 0;
	default_Rect.y = 0;
	default_Rect.w = 265;
	default_Rect.h = 199;
	App->render->Blit(tileset.Tex,0,0,&default_Rect);
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	App->tex->UnLoad(tileset.Tex);

	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		ret = LoadMap();
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	

	if(ret == true)
	{
		for (map_node = map_file.child("map").child("tileset"); map_node; map_node = map_node.next_sibling("tileset"))
		{
			LoadTileSet();
		}
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMap()
{
	Map.width = map_file.first_child().attribute("width").as_uint();
	Map.height = map_file.first_child().attribute("height").as_uint();
	Map.tilewidth = map_file.first_child().attribute("tilewidth").as_uint();
	Map.tileheight = map_file.first_child().attribute("tileheight").as_uint();

	if(Map.width == 0 && Map.height == 0 && Map.tilewidth == 0 && Map.tileheight == 0)
	return false;

	return true;
}

bool j1Map::LoadTileSet()
{
	
	//map_node = map_file.first_child().child("tileset");
	
	tileset.firstgid = map_node.attribute("firstgid").as_uint();
	tileset.tilewidth = map_node.attribute("tilewidth").as_uint();
	tileset.tileheight = map_node.attribute("tileheight").as_uint();
	tileset.spacing = map_node.attribute("spacing").as_uint();
	tileset.margin = map_node.attribute("margin").as_uint();

	tileset.Tex = App->tex->Load(map_node.child("image").attribute("source").as_string());

	return false;
}
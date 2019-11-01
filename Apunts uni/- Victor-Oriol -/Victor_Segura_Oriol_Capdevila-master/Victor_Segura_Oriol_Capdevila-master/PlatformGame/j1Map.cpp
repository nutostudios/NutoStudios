#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
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

	for (layer = data.layers.start; layer != nullptr; layer = layer->next) {
		for (tileset = data.tilesets.start; tileset != nullptr; tileset = tileset->next) {
			for (uint i = 0; i < layer->data->width; ++i) {
				for (uint j = 0; j < layer->data->height; ++j) {
					uint gid = layer->data->Get(i, j);
					iPoint pos = MapToWorld(i, j);
					SDL_Rect rect = tileset->data->GetTileRect(gid);
					App->render->Blit(tileset->data->texture, pos.x, pos.y, &rect, SDL_FLIP_NONE, layer->data->ParallaxSpeed);
				}
			}
		}
	}
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

bool j1Map::ChangeMap(const p2SString * map)
{
	CleanUp();
	Load(map->GetString());
	return true;
}



SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		SDL_DestroyTexture(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();
	App->collision->ColliderCleanUp();
	//Remove all layers
	p2List_item<map_layer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();
	//Remove objects
	p2List_item<ObjectGroup*>* item3;
	item3 = data.object_layers.start;

	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.object_layers.clear();



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

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}


	// Load layer info ----------------------------------------------
	pugi::xml_node layers;
	for (layers = map_file.child("map").child("layer"); layers && ret; layers = layers.next_sibling("layer"))
	{
		map_layer* set = new map_layer();

		if (ret == true)
		{
			ret = LoadLayer(layers, set);
		}
		data.layers.add(set);
	}

	pugi::xml_node ObjGrouop;
	for (ObjGrouop = map_file.child("map").child("objectgroup"); ObjGrouop && ret; ObjGrouop = ObjGrouop.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjects(ObjGrouop, set);
		}
		data.object_layers.add(set);
	}
	//LoadPlayerProperties();
	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<map_layer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			map_layer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<ObjectGroup*>* obj_layer = data.object_layers.start;
		while (obj_layer != NULL)
		{
			ObjectGroup* o = obj_layer->data;
			LOG("Group ----");
			LOG("Gname: %s", o->name.GetString());

			obj_layer = obj_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}



// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, map_layer* layer) {
	
	bool ret = true;
	layer->name.create(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node prop = node.child("properties");
	layer->ParallaxSpeed = prop.child("property").attribute("value").as_float();
	pugi::xml_node data = node.child("data");
	layer->data = new uint[layer->width*layer->height];
	memset(layer->data, 0, sizeof(uint)*layer->height*layer->width);

	int i = 0;
	pugi::xml_node tile = node.child("data");

	for (tile = tile.child("tile"); tile; tile = tile.next_sibling("tile")) {
		layer->data[i] = tile.attribute("gid").as_int(0);
		i++;
	}

	return ret;
}

bool j1Map::LoadObjects(pugi::xml_node & node, ObjectGroup* obj)
{
	bool ret = true;

	obj->name = node.attribute("name").as_string();

		for (pugi::xml_node& object = node.child("object"); object && ret; object = object.next_sibling("object"))
		{
			if (obj->name == "Collider") {
				SDL_Rect rect;
				rect.x = object.attribute("x").as_int();
				rect.y = object.attribute("y").as_int();
				rect.w = object.attribute("width").as_int();
				rect.h = object.attribute("height").as_int();
				pugi::xml_node prop = object.child("properties").child("property");
				int i;
				i = prop.attribute("value").as_int();

				if (i == 1) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_WALL_LEFT);
				}
				else if (i == 2) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_GROUND);
				}
				else if (i == 3) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_SPIKES);
				}
				else if (i == 4) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_FALL);
				}
				else if (i == 5) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_WIN);
				}
				else if (i == 6) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_WATER);
				}
				else if (i == 7) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_CLIMB);
				}
				else if (i == 8) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_ROPE);
				}
				else if (i == 10) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_NONE);
				}
				else if (i == 11) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_WALL_UP);
				}

				else if (i == 12) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_PLATFORM);
				}
				else if (i == 13) {
					App->collision->AddCollider(rect, COLLIDER_TYPE::COLLIDER_WALL_RIGHT);
				}
			}
			else {
				ObjectData* data = new ObjectData;

				data->height = object.attribute("height").as_uint();
				data->width = object.attribute("width").as_uint();
				data->x = object.attribute("x").as_uint();
				data->y = object.attribute("y").as_uint();
			
				pugi::xml_node prop = object.child("properties").child("property");
				data->name = prop.attribute("value").as_string();
				obj->objects.add(data);
			}
			
		}

	//}
	
	return ret;
}

bool j1Map::LoadPlayerProperties()
{
	/*
	p2List_item<ObjectGroup*>* item = data.object_layers.start;
	p2List_item<ObjectData*>* item2;

	while (item != NULL) {
		if (item->data->name == "Player") {
			item2 = item->data->objects.start;
			while (item2 != NULL) {
				if (item2->data->name == "Position_Player_Start_Map1") {
					App->player->initialmap1.x = item2->data->x;
					App->player->initialmap1.y = item2->data->y;
				}
				if (item2->data->name == "Position_Player_Start_Map2") {
					App->player->initialmap2.x = item2->data->x;
					App->player->initialmap2.y = item2->data->y;
				}
				item2 = item2->next;
			}
		}
		item = item->next;
	}
	*/
	return true;
}

ObjectGroup::~ObjectGroup()
{
	objects.clear();

}

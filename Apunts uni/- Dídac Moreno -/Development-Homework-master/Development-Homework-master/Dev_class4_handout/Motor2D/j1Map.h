#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct TileSet
{

	unsigned int firstgid, tilewidth, tileheight, spacing, margin;
	SDL_Texture *Tex;
};

// TODO 1: Create a struct needed to hold the information to Map node
struct mapInfo
{
	enum orientation
	{
		Orientation_Error = -1,orthogonal, isometric,hexagonal
	};

	enum render_order
	{
		Render_Order_Error = -1, right_down, right_up, left_down,left_up
	};
	
	unsigned int width, height, tilewidth, tileheight;
	//unsigned int nextobjectid;

};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:
	//TODO 3 (MYSELF)
	bool LoadMap();
	//TODO 4 (MYSELF)
	bool LoadTileSet();

public:

	// TODO 1: Add your struct for map info as public for now
	mapInfo Map;
	TileSet tileset;

private:

	pugi::xml_document	map_file;
	pugi::xml_node map_node;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__
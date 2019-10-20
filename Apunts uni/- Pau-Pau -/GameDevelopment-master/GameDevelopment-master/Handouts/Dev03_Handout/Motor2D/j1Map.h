#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------

struct Tileset {
	unsigned int firstgid	= 0u;
	const char* name		= "";
	unsigned int width		= 0u;
	unsigned int height		= 0u;
	unsigned int spacing	= 0u;
	unsigned int margin		= 0u;
	const char* img_source	= "";
};


// TODO 1: Create a struct needed to hold the information to Map node
enum class RenderOrder {
	RIGHT_DOWN,
	RIGHT_UP,
	LEFT_DOWN,
	LEFT_UP,
	UNKNOWN
};

enum class Orientation {
	ORTHOGONAL,
	ISOMETRIC,
	STAGGERED,
	HEXAGONAL,
	UNKNOWN
};

struct Map {
	float version				= 0.0f;
	Orientation orientation		= Orientation::UNKNOWN;
	RenderOrder renderorder		= RenderOrder::UNKNOWN;
	unsigned int width			= 0u;
	unsigned int height			= 0u;
	unsigned int tilewidth		= 0u;
	unsigned int tileheight		= 0u;
	int nextobject				= 0;
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

	bool LoadMap(pugi::xml_node& node);
	bool LoadTileset(pugi::xml_node& node);

private:


public:

	// TODO 1: Add your struct for map info as public for now
	Map map;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__
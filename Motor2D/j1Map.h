#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2DynArray.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct tmx_image_info {
	p2SString	source;
	uint		width,
		height;
};

struct tmx_tileset_info {
	uint		firstgid;
	p2SString	name;
	uint		tilewidth,
				tileheight,
				spacing,
				margin,
				tilecount,
				columns;

	tmx_image_info image;
};

// TODO 1: Create a struct needed to hold the information to Map node

struct tmx_map_info {
	enum map_orientation {
		ORIENTATION_ERROR = -1,
		ORIENTATION_ORTHOGONAL,
		ORIENTATION_ISOMETRIC
	};

	enum map_renderorder {
		RENDER_ERROR = -1,
		RENDER_RIGHT_DOWN,
		RENDER_LEFT_DOWN,
		RENDER_RIGHT_UP,
		RENDER_LEFT_UP
	};

	p2SString	version;
	map_orientation orientation;
	map_renderorder renderorder;
	uint		width,
				height,
				tilewidth,
				tileheight,
				nextobjectid;

	tmx_tileset_info tileset;

	p2DynArray<uint> tile_id_array;
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

	tmx_map_info FillMapInfo(const pugi::xml_document& map_doc);

public:

	// TODO 1: Add your struct for map info as public for now
	tmx_map_info map_info;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__
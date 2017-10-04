#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
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

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map

	// Remove all tilesets


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	char* buf;
	int size = App->fs->Load(tmp.GetString(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		map_info = FillMapInfo(map_file); //Preguntar tema de pas per referencia / pas per copia!!!
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	

	// TODO 5: LOG all the data loaded
	// iterate all tilesets and LOG everything

	map_loaded = ret;

	return ret;
}

tmx_map_info j1Map::FillMapInfo(const pugi::xml_document& map_doc) {
	tmx_map_info map;
	pugi::xml_node node = map_doc.child("map");
	pugi::xml_node tileset = node.child("tileset");
	pugi::xml_node image = tileset.child("image");
	pugi::xml_node layer = tileset.next_sibling();
	pugi::xml_node layer_data = layer.first_child();

	map.version = node.attribute("version").as_string();
	p2SString orientation = node.attribute("orientation").as_string();
	if (orientation == "orthogonal") {
		map.orientation = tmx_map_info::map_orientation::ORIENTATION_ORTHOGONAL;
	}
	else if (orientation == "isometric") {
		map.orientation = tmx_map_info::map_orientation::ORIENTATION_ISOMETRIC;
	}

	p2SString render_order = node.attribute("renderorder").as_string();
	if (render_order == "right-down") {
		map.renderorder = tmx_map_info::map_renderorder::RENDER_RIGHT_DOWN;
	}
	else if (render_order == "right-up") {
		map.renderorder = tmx_map_info::map_renderorder::RENDER_RIGHT_UP;
	}
	else if (render_order == "left-down") {
		map.renderorder = tmx_map_info::map_renderorder::RENDER_LEFT_DOWN;
	}
	else if (render_order == "left-up") {
		map.renderorder = tmx_map_info::map_renderorder::RENDER_LEFT_UP;
	}

	map.width = node.attribute("width").as_uint();
	map.height = node.attribute("height").as_uint();
	map.tilewidth = node.attribute("tilewidth").as_uint();
	map.tileheight = node.attribute("tileheight").as_uint();
	map.nextobjectid = node.attribute("nextobjectid").as_uint();

	map.tileset.firstgid = tileset.attribute("firstgid").as_uint();
	map.tileset.name = tileset.attribute("name").as_string();
	map.tileset.tilewidth = tileset.attribute("tilewidth").as_uint();
	map.tileset.tileheight = tileset.attribute("tileheight").as_uint();
	map.tileset.spacing = tileset.attribute("spacing").as_uint();
	map.tileset.margin = tileset.attribute("margin").as_uint();
	map.tileset.tilecount = tileset.attribute("tilecount").as_uint();
	map.tileset.columns = tileset.attribute("columns").as_uint();

	map.tileset.image.source = image.attribute("source").as_string();
	map.tileset.image.width = image.attribute("width").as_int();
	map.tileset.image.height = image.attribute("height").as_int();

	for (pugi::xml_node tile : layer_data.children()) {
		map.tile_id_array.PushBack(tile.first_attribute().as_uint());
	}

	return map;
}
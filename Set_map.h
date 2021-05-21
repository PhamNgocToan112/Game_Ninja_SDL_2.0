#pragma once

#include"Image_manager.h"

class TileMap : public Image_manager
{
public:
	TileMap() { ; }
	~TileMap() { ; }

};

class GameMap
{
public:
	GameMap();
	~GameMap() { ; }

	void LoadMap(const  char* name, SDL_Renderer* screen);
	void RenderMap(SDL_Renderer* screen);
	Map getMap()const { return game_map_; };
	void SetMap(Map& map_data) { game_map_ = map_data; };
private:
	Map game_map_;
	TileMap tile_map[TILE_NUM];
	// bảng các tile 
};




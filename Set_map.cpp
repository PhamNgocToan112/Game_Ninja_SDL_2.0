#include "Set_map.h"
#include<fstream>

GameMap::GameMap() {
	game_map_.max_x_ = 0;
	game_map_.max_y_ = 0;

	game_map_.start_x_ = 0;
	game_map_.start_y_ = 0;
}

void GameMap::LoadMap(const char* name, SDL_Renderer* screen) {
	std::ifstream file;
	file.open(name, std::ios::binary);

	for (int i = 0; i < MAX_MAP_Y; i++) {
		for (int j = 0; j < MAX_MAP_X; j++) {
			file >> game_map_.tile[i][j];
			int val = game_map_.tile[i][j];

			if (val > 0) {
				if (j > game_map_.max_x_) {
					game_map_.max_x_ = j;
				}
				if (i > game_map_.max_y_) {
					game_map_.max_y_ = i;
				}
			}
		}
	}
	game_map_.max_x_ = (game_map_.max_x_ + 1) * TILE_SIZE;
	game_map_.max_y_ = (game_map_.max_y_ + 1) * TILE_SIZE;
	// đọc file .dat

	bool check;
	std::string file_map = "";
	for (int i = 0; i < TILE_NUM; i++) {
		file_map = "map/";
		file_map += std::to_string(i);
		file_map += ".png";

		check = tile_map[i].loadImg(file_map, screen);
		if (check == false) {
			std::cout << "tile_map" << i << "empty";
			continue;
		}
	}
	//load tile_map
}

void GameMap::RenderMap(SDL_Renderer* screen) {
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;
	// tọa độ để render ảnh lên

	int	index_x = 0;
	int index_y = 0;
	// ô render trong tile

	index_x = game_map_.start_x_ / TILE_SIZE;
	x1 = (game_map_.start_x_ % TILE_SIZE) * -1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);
	// render map từ vị trí x1 đến x2 theo chiều ngang

	index_y = game_map_.start_y_ / TILE_SIZE;
	y1 = 0;
	y2 = SCREEN_HEIGHT;
	// render map từ vị trí y1 đến y2 theo chiều dọc

	for (int i = y1; i < y2; i += TILE_SIZE)
	{
		index_x = game_map_.start_x_ / TILE_SIZE;
		for (int j = x1; j < x2; j += TILE_SIZE) {
			int val = game_map_.tile[index_y][index_x];

			if (val > 0) {
				tile_map[val].SetRect(j, i);
				tile_map[val].Render(screen, NULL);
				// render vị trí (j, i) ra màn hình
			}
			// render tile 
			index_x++;
		}
		index_y++;
	}

}
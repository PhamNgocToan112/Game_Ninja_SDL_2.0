#pragma once

#include<ctime>
#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include<Windows.h>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>// text


static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int FRAME_PER_SECOND = 15; // số frame / s

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 230;
const int COLOR_KEY_G = 230;
const int COLOR_KEY_B = 230;
const int RENDER_DRAW_COLOR = 0xff;
//screen

#define BLANK_TILE 0
#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define TILE_NUM 30
#define STATE_MONEY 4
// map

#define MONSTER_FRAME_NUM 8
#define MONSTER_GRAVITY_SPEED 0.8
#define MONSTER_MAX_FALL_SPEED 10
#define MONSTER_NUM 50
#define MAX_MONSTER_BULLET_DISTANCE 600
// monster

#define CHARACTER_FRAME_NUM 8
#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 18
#define TILE_BACK 5
#define BULLET_SPEED 20
#define POSITON_Y_BULLET 64 * 0.5
#define HEARTS 3

struct Input {
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
};
//character

struct Map {
	int start_x_;
	int start_y_;
	//vị trí bắt đầu màn hình,
	//thay đổi khi nhân vật di chuyển trong hàm CenterEntityOnMap (trong class character)

	int max_x_;
	int max_y_;
	//tọa độ cuối của map

	int tile[MAX_MAP_Y][MAX_MAP_X];
	//bảng lưu trạng thái map

};


namespace SDLCommonFunc
{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}

// check va chạm


static Mix_Chunk* g_sound_bullet;
static Mix_Chunk* g_sound_dead;
// âm thanh

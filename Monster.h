#pragma once

#include"Image_manager.h"
#include"Bullet.h"

#define MONSTER_FRAME_NUM 8
#define MONSTER_GRAVITY_SPEED 0.8
#define MONSTER_MAX_FALL_SPEED 10
#define MONSTER_SPEED 3

class Monster : public Image_manager
{
public:
	Monster();
	~Monster();

	enum TypeMove {
		STATIC_MONSTER = 0,
		MOVE_IN_SPACE_MONSTER = 1,
	};
	// 2 kiểu quái đứng yên và di chuyển => phát triển thêm...

	void set_x_walk(const int& xWalk) { x_walk_ = xWalk; }
	void set_y_walk(const int& yWalk) { y_walk_ = yWalk; }

	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }

	float get_x_pos()const { return x_pos_; }
	float get_y_pos()const { return y_pos_; }

	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }
	void set_clips();
	bool LoadImg(std :: string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame()const { return width_frame_; }
	int get_height_frame()const { return height_frame_; }
	void DoPlayer(Map& gMap);
	void CheckToMap(Map& gMap);

	void InitsMonsters();

	void set_type_move(const int& typeMove) {	type_move_ = typeMove;}
	void set_input_left(const int& ipLeft) { input_type_.left_ = ipLeft;}
	void ImpMoveType(SDL_Renderer* screen);

	std::vector<BulletObject*> get_bullet_list() const { return bullet_list_; }\
	// đạn 
	void set_bullet_list(const std::vector<BulletObject*>& bl_list) { bullet_list_ = bl_list; }

	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
	void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
	void RemoveBullet(const int& idx);



private:
	int map_x_;
	int map_y_;
	// check ra ngoài bản đồ

	float y_walk_;
	float x_walk_;
	// lượng di chuyển
	float x_pos_;
	float y_pos_;
	// vị trí
	bool on_ground_;
	int come_back_time_;
	
	
	SDL_Rect frame_clip_[MONSTER_FRAME_NUM];
	
	int width_frame_;
	int height_frame_;
	// chiều dài rộng 1 frame
	int frame_index;
	//frame

	int type_move_;
	// kiểu di chuyển 

	Input input_type_;
	std :: vector<BulletObject*> bullet_list_;
	// đạn của monster
};

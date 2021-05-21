#pragma once

#include"Image_manager.h"
#include"Bullet.h"
#include "Lib_and_const.h"

class Character : public Image_manager {
public:
	Character();
	~Character() { ; }

	enum WalkType {
		WALK_NONE = 0,
		WALK_RIGHT = 1,
		WALK_LEFT = 2,
	};


	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandelInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound);
	void set_clips();
	// lấy vị trí và kích thước các frame từ tấm ảnh 8 frame
	void DoPlayer(Map& map_data);
	// xử lý di chuyển cho nhân vật
	void CheckPlayer(Map& map_data);
	// kiểm tra nhân vật khi va chạm vs map
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; };
	// set tọa độ của map start x, start y
	void CenterEntityOnMap(Map& map_data);
	// thông số của map khi di chuyển nhân vật
	void UpdateImagePlayer(SDL_Renderer* des);
	// thay đổi tấm ảnh khi bấm nút khác
	SDL_Rect GetRectFrame();
	// lấy tọa độ của duy nhất 1 frame 

	void set_bullet_list(std::vector<BulletObject*> bullet_list)
	{
		p_bullet_list_ = bullet_list;
	}

	std::vector<BulletObject*> get_bullet_list() const { return p_bullet_list_; }

	void HandleBullet(SDL_Renderer* des);
	void RemoveBullet(const int& idx);
	// xóa đạn 
	void IncreaseMoney();
	int get_frame_width() const { return width_frame_; }
	int get_frame_height() const { return height_frame_; }
	void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time; }
	int GetMoneyCount() const { return money_count; }
	void set_falling(const bool& f) { falling_ = f; }
	bool get_falling()const { return falling_; }


private:

	int money_count;
	std::vector<BulletObject*> p_bullet_list_;
	// tập hợp kiểm soát các viên đạn(băng đạn), phi tiêu...

	float x_walk_;
	float y_walk_;
	// di chuyển

	float x_pos_;
	float y_pos_;
	//vị trí nhân vật hiện tại => để show hình ảnh

	int width_frame_;
	int height_frame_;
	// lưu kích thước 1 frame 

	SDL_Rect frame_clip_[CHARACTER_FRAME_NUM];
	// bảng lưu các frame
	Input input_type_;
	// => check tăng frame, thay đổi vị trí nhân vật
	int frame_index_;
	int status_;
	// kiểm tra trạng thái đi sang trái hay phải => xem load tấm ảnh nào

	bool on_ground_;

	int map_x_;
	int map_y_;
	//vị trí của màn hình khi di chuyển => để check cho màn hình di chuyển theo nhân vật

	bool falling_;
	int come_back_time_; 
};





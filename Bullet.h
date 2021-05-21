#pragma once

#include "Lib_and_const.h"
#include "Image_manager.h"


class BulletObject : public Image_manager {
public:
	BulletObject();
	~BulletObject();
	
	enum BulletDir {
		DIR_RIGHT = 20,
		DIR_LEFT = 21,
		DIR_UP = 22,
		DIR_UP_RIGHT = 23,
		DIR_UP_LEFT = 24,
		DIR_DOWN_LEFT = 25,
		DIR_DOWN_RIGHT = 26,
		DIR_DOWN = 27,
	};
	// các hằng số hướng bắn

	enum BulletType
	{
		SHURIKEN = 50,
		KUNAI = 51,
		SPHERE_BULLET = 52,
	};
	// các hằng số loại đạn


	void set_x_val(const int& xVal) { x_val_ = xVal; }
	void set_y_val(const int& yVal) { y_val_ = yVal; }
	int get_x_val() const { return x_val_; }
	int get_y_val()const { return y_val_; }

	void set_is_move(const bool& isMove) { is_move_ = isMove; }
	bool get_is_move()const { return is_move_; }
	
	void set_bullet_dir(const unsigned int& bulletDir) { bullet_dir_ = bulletDir; }
	unsigned int get_bullet_dir() const { return bullet_dir_; }

	void set_bullet_type(const unsigned int& bulletType) { bullet_type_ = bulletType; }
	unsigned int get_bullet_type() const { return bullet_type_; }

	void HandleMove(const int& x_border, const int& y_border);
	//xử lý di chuyển cho đạn
	bool loadImgBullet(SDL_Renderer* des);
	

private:
	int x_val_;
	int y_val_;
	// lượng giá trị di chuyển của đạn
	bool is_move_;
	//check trong hay ngoài màn hình => để xóa khi ra ngoài màn hình

	unsigned int bullet_dir_;
	// hướng của viên đạn

	unsigned int bullet_type_;
	//loại vũ khí phóng ra(đạn, phi tiêu ...)

};










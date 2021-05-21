#include "Monster.h"

Monster::Monster() {
	width_frame_ = 0;
	height_frame_ = 0;

	x_walk_ = 0.0;
	y_walk_ = 0.0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;

	on_ground_ = 0;
	come_back_time_ = 0;

	frame_index = 0;

	input_type_.left_ = 0;
	type_move_ = STATIC_MONSTER;
}

Monster :: ~Monster() {

}

bool Monster::LoadImg(std::string path, SDL_Renderer* screen) {
	bool ret = Image_manager::loadImg(path, screen);
	if (ret) {
		width_frame_ = rect_.w / MONSTER_FRAME_NUM;
		height_frame_ = rect_.h;

	}
	return ret;
}


void Monster::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i <= 7; i++) {
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void Monster::Show(SDL_Renderer* des) {
	if (come_back_time_ == 0) {
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		frame_index++;
		// cho quái thay đổi liên tục

		if (frame_index >= 8) {
			frame_index = 0;
		}

		SDL_Rect* currentClip = &frame_clip_[frame_index];
		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
		SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
	}
}


void Monster::DoPlayer(Map& gMap) {
	if (come_back_time_ == 0) {
		x_walk_ = 0;
		y_walk_ += MONSTER_GRAVITY_SPEED;

		if (y_walk_ >= MONSTER_MAX_FALL_SPEED) {
			y_walk_ = MONSTER_MAX_FALL_SPEED;
		}

		CheckToMap(gMap);

	}
	else if (come_back_time_ > 0) {
		come_back_time_--;
		if (come_back_time_ == 0) {
			InitsMonsters();
		}
	}
}

void Monster::RemoveBullet(const int& idx)
{
	int size = bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = bullet_list_.at(idx);
		bullet_list_.erase(bullet_list_.begin() + idx);
		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}

}

void Monster::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move())
			{
				int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
				// khoảng cách quái và viên đạn
				if (bullet_distance < MAX_MONSTER_BULLET_DISTANCE && bullet_distance >0)
				{
					p_bullet->HandleMove(x_limit, y_limit);
					p_bullet->Render(screen, NULL);
					// nhỏ hơn ms đc bắn ra
				}
				else
				{
					p_bullet->set_is_move(false);
					// lớn hơn cho đạn biến mất 
				}
			}
			else
			{
				p_bullet->set_is_move(true);
				// cho quái bắn đạn liên tục
				//=> cho về true, sau đó đưa về vị trí ban đầu
				p_bullet->SetRect(rect_.x + 3, rect_.y + 20);
				//vị trí đạn bắn ra
			}
		}
	}
}



void Monster::InitsMonsters() {
	x_walk_ = 0;
	y_walk_ = 0;
	

	y_pos_ = 0;
	come_back_time_ = 0;
	input_type_.left_ = 1;
}

void Monster::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
	if (p_bullet != NULL)
	{
		p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
		bool ret = p_bullet->loadImgBullet(screen);

		if (ret)
		{
			p_bullet->set_is_move(true);
			p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
			// hướng bắn
			p_bullet->SetRect(x_pos_ + 5, rect_.y + 10);
			// vị trí của đạn dựa trên vị trí quái
			p_bullet->set_x_val(15);
			// tốc độ đạn
			bullet_list_.push_back(p_bullet);
			// cho vào list
		}

	}
}

void Monster::CheckToMap(Map& map_data) {
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;


	//ktra theo chiều ngang
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

	x1 = (x_pos_ + x_walk_) / TILE_SIZE; //ô thứ bn
	x2 = (x_pos_ + x_walk_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y1 = (y_pos_ + height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (x_walk_ > 0) // đang di chuyển vể bên phải
		{
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];


			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_walk_ = 0;
			}
		}

		else if (x_walk_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];

			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_walk_ = 0;
			}

		}
	}



	//ktra theo chiều dọc

	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_walk_) / TILE_SIZE;
	y2 = (y_pos_ + y_walk_ + height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_walk_ > 0)
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];

			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
			{
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				y_walk_ = 0;
				on_ground_ = true;
			}


		}
		else if (y_walk_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];

			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_walk_ = 0;
			}


		}
	}

	x_pos_ += x_walk_;
	y_pos_ += y_walk_;

	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}

	if (y_pos_ > map_data.max_y_)
	{
		come_back_time_ = 60;
	}

}


void Monster::ImpMoveType(SDL_Renderer* screen) {
	if (type_move_ == STATIC_MONSTER) {
		;
	}
}
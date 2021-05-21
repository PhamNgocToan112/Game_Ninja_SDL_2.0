#include "Character.h"

Character::Character()
{
	frame_index_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;

	x_walk_ = 0;
	y_walk_ = 0;

	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;

	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	on_ground_ = false;

	map_x_ = 0;
	map_y_ = 0;

	come_back_time_ = 0;
	money_count = 0;

}

bool Character::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = Image_manager::loadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w / 8;   //vì có 8 frame
		height_frame_ = rect_.h;
	}
	return ret;
}

SDL_Rect Character::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;

}

void Character::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		for (int i = 0; i < CHARACTER_FRAME_NUM; i++) {
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void Character::Show(SDL_Renderer* des)
{
	UpdateImagePlayer(des);

	if (input_type_.left_ == 1 || input_type_.right_ == 1) frame_index_++;
	else frame_index_ = 0;

	if (frame_index_ >= 8) frame_index_ = 0;

	if (come_back_time_ == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect* current_clip = &frame_clip_[frame_index_];
		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
	}
}

void Character::HandelInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImagePlayer(screen);
		}
		break;

		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImagePlayer(screen);

		}
		break;
		case SDLK_UP:
			input_type_.jump_ = 1;
			input_type_.right_ = 0;
			input_type_.left_ = 0;

			UpdateImagePlayer(screen);
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym) {

		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;
		}
		break;

		case SDLK_LEFT:
		{
			input_type_.left_ = 0;

		}
		break;
		}
	}

	if (events.type == SDL_MOUSEBUTTONDOWN) {
		if (events.button.button == SDL_BUTTON_LEFT) {
			//input_type_.jump_ = 1;
		}
		else if (events.button.button == SDL_BUTTON_RIGHT)
		{
			BulletObject* p_bullet = new BulletObject();
			p_bullet->set_bullet_type(BulletObject::SHURIKEN);
			// chọn loại đạn 
			p_bullet->loadImgBullet(screen);
			// load ảnh vs đạn tương ứng
			p_bullet->set_x_val(BULLET_SPEED);
			// chọn vận tốc bay
			p_bullet->set_is_move(true);
			// set trạng thái bay
			Mix_PlayChannel(-1, bullet_sound, 0);
			// thêm tiếng

			//=> tạo đạn và cho vào list
			if (status_ == WALK_LEFT) {
				p_bullet->set_bullet_dir(BulletObject::DIR_LEFT); 
				// chọn hướng bắn
				p_bullet->SetRect(this->rect_.x, rect_.y + POSITON_Y_BULLET);
				// chọn vị trí bắn ra
			}
			else {
				p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
				p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + POSITON_Y_BULLET);
			}
			/*p_bullet->set_x_val(BULLET_SPEED);
			p_bullet->set_y_val(BULLET_SPEED);
			p_bullet->set_is_move(true);*/

			p_bullet_list_.push_back(p_bullet);
			// cho đạn vào list
		}
	}
}

void Character::HandleBullet(SDL_Renderer* des)
{
	for (int i = 0; i < p_bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = p_bullet_list_.at(i);
		// lấy viên đạn trong list ra
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move() == true)
			{
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				// cho viên đạn bay theo hướng đã có => nếu ra khỏi màn hình 
				//=> (is_move == false) => xóa ở else bên dưới

				p_bullet->Render(des, NULL);
				//render ra màn hình vs tọa độ đã có
			}
			else
			{
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
				if (p_bullet != NULL)
				{
					delete p_bullet;
					p_bullet = NULL;
					// check lại xem đã xóa chưa( có thể ms chỉ loại khỏi danh sách)
				}
			}
		}
	}

}

void Character::RemoveBullet(const int& idx)
{
	int size = p_bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = p_bullet_list_.at(idx);
		p_bullet_list_.erase(p_bullet_list_.begin() + idx);
		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}

	// xóa vũ khí phóng ra khỏi list
}

void Character::DoPlayer(Map& map_data)
{
	if (come_back_time_ == 0)
	{
		x_walk_ = 0;
		y_walk_ += GRAVITY_SPEED;

		if (y_walk_ >= MAX_FALL_SPEED) y_walk_ = MAX_FALL_SPEED;

		if (input_type_.left_ == 1) x_walk_ -= PLAYER_SPEED;
		else if (input_type_.right_ == 1) x_walk_ += PLAYER_SPEED;
		// set cho tốc độ rơi, tốc độ di chuyển tăng dần đều

		if (input_type_.jump_ == 1)
		{
			if (on_ground_ == true)
			{
				y_walk_ = -PLAYER_JUMP_VAL;
			}
			on_ground_ = false;
			input_type_.jump_ = 0;
		}

		CheckPlayer(map_data); // check sau đó thay đổi vị trí
		CenterEntityOnMap(map_data); 
	}
	// dựa vào 
	if (come_back_time_ > 0)
	{
		come_back_time_--;
		if (come_back_time_ == 0)
		{
			on_ground_ = false;
			if (x_pos_ > TILE_BACK * TILE_SIZE) {
				x_pos_ -= (TILE_BACK * TILE_SIZE);  //5 ô tile map
			}
			else x_pos_ = 0;

			y_pos_ = 0;
			x_walk_ = 0;
			y_walk_ = 0;
		}
	}
	// nhân vật xuất hiện lại sau khi chết, rơi xuống vực
}

void Character::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);
	// vị trí bắt đầu của map
	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;
		// nhân vật chưa đến giữa màn hình
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}

	// map di chuyển theo chiều ngang

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y_ < 0)
	{
		map_data.start_y_ = 0;
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}

	// map di chuyển theo chiều dọc 
}

void Character::CheckPlayer(Map& map_data)
{
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

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseMoney();
				// xóa đồng tiền khi va chạm phải, và tăng money_count
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_walk_ = 0;
				}
				// va chạm vào các ô khác tile 0 hoặc 4 thì ko cho đi
			}
		}
		else if (x_walk_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				IncreaseMoney();
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos_ = (x1 + 1) * TILE_SIZE;
					x_walk_ = 0;
				}
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

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseMoney();

			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= (height_frame_ + 1);
					y_walk_ = 0;
					on_ground_ = true;
					if (status_ == WALK_NONE) {
						status_ = WALK_RIGHT;
					}
				}
			}
		}
		else if (y_walk_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];
			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				IncreaseMoney();
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					y_pos_ = (y1 + 1) * TILE_SIZE;
					y_walk_ = 0;
				}
			}
		}
	}
	// dựa vào vị trí nhân vật từ ô thứ [x1][y1] đến ô thứ [x2][y2] để đối chiếu với bảng 
	// tile của map => xác định nhân vật đi vào ô tile nào => check va chạm

	x_pos_ += x_walk_;
	y_pos_ += y_walk_;
	// thay đổi vị trí nhân vật

	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}
	// check lại vị trí khi nhân vật đi về phía đầu hoặc cuối bản đồ
	if (y_pos_ > map_data.max_y_)
	{
		come_back_time_ = 60;
	}
	// nếu nhân vật rơi xuống vực 
}

void Character::IncreaseMoney()
{
	money_count++;
}

void Character::UpdateImagePlayer(SDL_Renderer* des)
{
	if (on_ground_ == true)
	{
		if (status_ == WALK_LEFT)
		{
			loadImg("img//player_left.png", des);
		}
		else
		{
			loadImg("img//player_right.png", des);
		}
	}
	else
	{
		if (status_ == WALK_LEFT)
		{
			loadImg("img//jum_left.png", des);
		}
		else
		{
			loadImg("img//jum_right.png", des);
		}
	}
}
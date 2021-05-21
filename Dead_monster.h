#pragma once

#include "Image_manager.h"

#define NUM_FRAME_DEAD 8

class Dead_monster : public Image_manager
{
public:
	Dead_monster();
	~Dead_monster();

	void set_clip();
	void set_frame(const unsigned int& fr) { frame_ = fr; }
	// chọn xem ở frame nào
	virtual bool loadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);

	int get_frame_width() const { return frame_width_; }
	int get_frame_height() const { return frame_height_; }



private:
	int frame_width_;
	int frame_height_;
	// kích thước 1 frame

	unsigned int frame_;
	SDL_Rect frame_clip_[8];
};
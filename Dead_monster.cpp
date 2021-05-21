#include "Dead_monster.h"

Dead_monster::Dead_monster()
{

	frame_height_ = 0;
	frame_width_ = 0;
	frame_ = 0;
}

Dead_monster::~Dead_monster()
{

}

bool Dead_monster::loadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = Image_manager::loadImg(path, screen);

	if (ret)
	{
		frame_width_ = rect_.w / NUM_FRAME_DEAD;
		frame_height_ = rect_.h;
	}

	return ret;
}

void Dead_monster::set_clip()
{
	if (frame_width_ > 0 && frame_height_ > 0)
	{
		for (int i = 0; i < NUM_FRAME_DEAD; i++) {
			frame_clip_[i].x = frame_width_ * i;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = frame_width_;
			frame_clip_[i].h = frame_height_;
		}
	}
}


void Dead_monster::Show(SDL_Renderer* screen)
{
	SDL_Rect* current_clip = &frame_clip_[frame_];
	SDL_Rect render_quad = { rect_.x, rect_.y, frame_width_, frame_height_ };
	if (current_clip != NULL)
	{
		render_quad.w = current_clip->w;
		render_quad.h = current_clip->h;
	}

	SDL_RenderCopy(screen, p_object_, current_clip, &render_quad);

}

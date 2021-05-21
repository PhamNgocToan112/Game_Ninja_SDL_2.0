#pragma once
#include"Lib_and_const.h"

class Image_manager {
protected:
	SDL_Texture* p_object_;
	SDL_Rect rect_;
public:
	Image_manager();
	~Image_manager();

	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; }
	SDL_Rect GetRect() const { return rect_; }
	SDL_Texture* GetObject() const { return p_object_; }

	virtual bool loadImg(std::string path, SDL_Renderer* screen); //load ảnh đồng thời lấy kích thước và tọa độ của tấm ảnh 
	void Render(SDL_Renderer* targetScreen, SDL_Rect* clip);//render lên màn hình vs kích thước lấy được
	void Free();
};




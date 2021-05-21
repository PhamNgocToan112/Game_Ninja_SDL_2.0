#include "Image_manager.h"

Image_manager::Image_manager() {
	p_object_ = NULL;

	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;

}

Image_manager :: ~Image_manager() {
	Free();
};


bool Image_manager::loadImg(std::string path, SDL_Renderer* screen) {
	SDL_Texture* new_texture = NULL;

	SDL_Surface* load_surface = IMG_Load(path.c_str());

	if (load_surface != NULL) {
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		// Cho ảnh nhân vật trùng vào background

		new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
		if (new_texture != NULL) {
			rect_.w = load_surface->w;
			rect_.h = load_surface->h;
		}

		SDL_FreeSurface(load_surface);
	}

	p_object_ = new_texture;

	return p_object_;
	// p_object khac NULL => return 1
	// p_object = NULL => return 0

}

void Image_manager::Render(SDL_Renderer* targetScreen, SDL_Rect* clip) {
	SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h };

	SDL_RenderCopy(targetScreen, p_object_, clip, &renderquad);
}

void Image_manager::Free() {
	if (p_object_ != NULL) {
		SDL_DestroyTexture(p_object_);
		p_object_ = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}

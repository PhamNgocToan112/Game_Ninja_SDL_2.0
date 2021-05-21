#include "Text.h"

Text::Text()
{
	text_color_.r = 255;
	text_color_.b = 255;
	text_color_.g = 255;
	// màu trắng
	texture_ = NULL;
}

Text::~Text()
{

}

bool Text::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, text_content_.c_str(), text_color_);
	if (text_surface)
	{
		texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
		width_text_ = text_surface->w;
		height_text_ = text_surface->h;

		SDL_FreeSurface(text_surface);
	}

	return texture_ != NULL;

}


void Text::Free()
{
	if (texture_ != NULL)
	{
		SDL_DestroyTexture(texture_);
		texture_ = NULL;
	}
}

void Text::SetColor(int type)
{
	if (type == RED_TEXT)
	{
		SDL_Color color = { 255, 0, 0 };
		text_color_ = color;
	}
	else if (type == WHITE_TEXT)
	{
		SDL_Color color = { 255, 255, 255 };
		text_color_ = color;

	}
	else if (type == BLACK_TEXT)
	{
		SDL_Color color = { 0, 0, 0 };
		text_color_ = color;
	}
}
void Text::RenderText(SDL_Renderer* screen,
	int xp, int yp,
	SDL_Rect* clip,
	double angle,
	SDL_Point* center,
	SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { xp, yp, width_text_, height_text_ };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;

	}
	SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle, center, flip);

}
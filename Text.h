#pragma once

#include "Lib_and_const.h"

class Text
{
public:
    Text();
    ~Text();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,

    };


    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    // load text => surface
    void Free();
    
    void SetColor(int type);
        // set màu
    void RenderText(
        SDL_Renderer* screen,
        int x_pos, int y_pos,
        SDL_Rect* clip = NULL,
        double angle = 0.0, // góc ngiêng
        SDL_Point* center = NULL,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    );
    // render text

    int GetWidth() const { return width_text_; }
    int GetHeight() const { return height_text_; }

    void SetText(const std::string& content_) { text_content_ = content_; }
    // tạo nội dung cho text
    std::string GetText() const { return text_content_; }
    // lấy text
private:
    std::string text_content_;
    // lưu nội dung text
    SDL_Color text_color_;
    // màu sắc của text
    SDL_Texture* texture_;

    int width_text_;
    int height_text_;
    // kích thước

};
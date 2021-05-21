#pragma once

#include "Image_manager.h"

class Player : public Image_manager
{
public:
    Player();
    ~Player();

    void SetNum(const int& num) { number_ = num; }
    
    void AddPos(const int& xPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);
    // load ảnh mạng nhân vậtban đầu

   // void InitCrease();
    // load ảnh giảm xuống
    void Decrease();

private:
    int number_;
    // số lượng mạng tối đa của nhân vật
    std::vector<int> pos_list_;
    // list tấm ảnh

};


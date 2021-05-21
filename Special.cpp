#include "Special.h"

Player::Player()
{
	number_ = 0;
}

Player::~Player()
{

}

void Player::AddPos(const int& xp)
{
	pos_list_.push_back(xp);
}

void Player::Init(SDL_Renderer* screen)
{
	loadImg("img//player.png", screen);
	number_ = HEARTS;

	for (int i = 0; i < number_; i++) {
		AddPos(20 + 60 * i);
		// mặc định cách nhau 60 
	}
}

void Player::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < pos_list_.size(); i++)
	{
		rect_.x = pos_list_.at(i);
		rect_.y = 0;
		Render(screen, NULL);
	}
}

void Player::Decrease()
{
	number_--;
	pos_list_.pop_back();
	// xóa khỏi list
}








































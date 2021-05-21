#include "Imp_timer.h"
#include"Lib_and_const.h"

ImpTimer::ImpTimer() {
	start_tick_ = 0;
	paused_tick_ = 0;
	is_paused_ = false;
	is_started_ = false;
}


ImpTimer :: ~ImpTimer() {

}

void ImpTimer::start() {
	is_started_ = true;
	is_paused_ = false;
	start_tick_ = SDL_GetTicks();
	// liên tục lấy tg bắt đầu 1 vòng while
}

int ImpTimer::get_ticks() {
	if (is_started_ == true) {	
		return SDL_GetTicks() - start_tick_;
	}
}


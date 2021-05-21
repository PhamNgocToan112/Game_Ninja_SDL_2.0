#pragma once

class ImpTimer {
public:
	ImpTimer();
	~ImpTimer();

	void start();
	// tính thời điểm bắt đầu
	int get_ticks(); // lấy thời gian chạy 1 frame
	
	/*void stop();
	void paused();
	void unpaused();
	bool is_started();
	bool is_paused();*/
	// xử lý khi muốn pause chương trình

private:
	int start_tick_;
	int paused_tick_;
	// thời điểm bắt đầu và thời điểm dừng 1 frame

	bool is_paused_;
	bool is_started_;




};






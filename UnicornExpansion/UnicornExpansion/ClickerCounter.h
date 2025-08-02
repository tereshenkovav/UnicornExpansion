#pragma once

class ClickerCounter
{
private:
	int soundcountall;
	int counter_click = 0;
	int last_clicked_uid = -1;
	int last_sound_idx = -1;
public:
	// Передается размер массива звуков, где последний массив - секретная реплик после заданного числа нажатий
	ClickerCounter(int soundcountall);
	int getNextSoundIdx(int selected_uid);
};

const int COUNT_CLICK_FOR_SECRET = 10;

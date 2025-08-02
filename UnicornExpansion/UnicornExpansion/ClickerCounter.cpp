#include "ClickerCounter.h"
#include <stdlib.h>

ClickerCounter::ClickerCounter(int soundcountall)
{
	this->soundcountall = soundcountall;
}

int ClickerCounter::getNextSoundIdx(int selected_uid)
{
    // «апуск дл€ единорога новой реплики, не совпадающей со старой
    int new_sound_idx = last_sound_idx;
    while (new_sound_idx == last_sound_idx)
        new_sound_idx = rand() % (soundcountall - 1);
    last_sound_idx = new_sound_idx;

    // —четчик дл€ пасхалки - возвращаем последний индекс в массиве
    if (selected_uid == last_clicked_uid) {
        counter_click++;
        if (counter_click == COUNT_CLICK_FOR_SECRET) {
            new_sound_idx = soundcountall - 1;
            counter_click = 0;
        }
    }
    else {
        last_clicked_uid = selected_uid;
        counter_click = 0;
    }
    return new_sound_idx;
}

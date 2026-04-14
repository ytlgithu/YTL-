#include "custom.h"


static screen_08_sound_t *scr = NULL;


//点击声音开关按钮
void screen_08_sound_switch_1_sound_custom_clicked(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    yk_settings.sound = lv_obj_has_state(target, LV_STATE_CHECKED);
    yk_delay_save_data_timer_start();
}


//创建执行一次
void screen_08_sound_custom_created(void)
{ 
    scr = screen_08_sound_get(&ui_manager);
    // 设置声音开关的初始状态
    if (yk_settings.sound == val0_sound_on)
    {
        lv_obj_add_state(scr->switch_1_sound, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(scr->switch_1_sound, LV_STATE_CHECKED);
    }
}
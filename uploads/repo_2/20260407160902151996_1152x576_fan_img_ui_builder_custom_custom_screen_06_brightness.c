#include "custom.h"


static screen_06_brightness_t *scr = NULL;


// //点击返回保存数据
// void screen_06_brightness_button_1_back_custom_clicked(void)
// {
//     yk_printf("screen_06_brightness_button_1_back_custom_clicked\n");
//     yk_delay_save_data_timer_start();
// }

//设置亮度
void screen_06_brightness_slider_1_custom_value_changed(void)
{
    yk_printf("screen_06_brightness_slider_1_custom_value_changed\n");
	yk_settings.screen_brightness = lv_slider_get_value(scr->slider_1);
    lv_label_set_text_fmt(scr->label_2_percent, "%d", yk_settings.screen_brightness);
    yk_f_set_pwm_brightness(yk_settings.screen_brightness);
    yk_delay_save_data_timer_start();
}


//创建执行一次
void screen_06_brightness_custom_created(void)
{ 
    scr = screen_06_brightness_get(&ui_manager);
    lv_slider_set_value(scr->slider_1, yk_settings.screen_brightness, LV_ANIM_OFF);
    lv_label_set_text_fmt(scr->label_2_percent, "%d", yk_settings.screen_brightness);
}

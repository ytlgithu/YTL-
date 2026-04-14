#include "custom.h"


static screen_12_idle_t *scr = NULL;


//每次进入
void screen_12_idle_custom_loaded()
{
    yk_printf("screen_5_shutdown_custom_loaded\n");
    yk_v_screen_id = SCREEN_12_ID;
    //进入关屏页：将屏幕亮度置为0
    #if YK_USE_UI_BUILDER
    yk_f_set_pwm_brightness(0);
    #endif
}

//每次退出
void screen_12_idle_custom_unload_start()
{
    yk_printf("screen_5_shutdown_custom_unload_start\n");
    scr = screen_12_idle_get(&ui_manager);
    //退出关屏页：恢复到设置的屏幕亮度
    #if YK_USE_UI_BUILDER
    yk_f_set_pwm_brightness(yk_settings.screen_brightness);
    #endif
}

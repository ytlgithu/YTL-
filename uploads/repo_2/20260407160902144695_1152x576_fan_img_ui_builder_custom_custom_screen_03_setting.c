#include "custom.h"


static screen_03_setting_t *scr = NULL;


//创建执行一次
// 设置界面首次创建：缓存 screen 指针
void screen_03_setting_custom_created(void)
{ 
    scr = screen_03_setting_get(&ui_manager);
}

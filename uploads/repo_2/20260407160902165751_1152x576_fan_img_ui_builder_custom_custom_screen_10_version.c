#include "custom.h"


//版本号
#define YK_VERSION 103  //版本号V_1.0.3


static screen_10_version_t *scr = NULL;


//创建执行一次
void screen_10_version_custom_created(void)
{ 
    scr = screen_10_version_get(&ui_manager);
    lv_label_set_text_fmt(scr->label_2_version_V, "V_%d.%d.%d",YK_VERSION/100,YK_VERSION/10%10,YK_VERSION%10);
}
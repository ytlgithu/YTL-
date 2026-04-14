#include "custom.h"
#include "time.h"


uint8_t yk_v_hor_value = 13;
uint8_t yk_v_min_value = 20;
uint8_t yk_v_sec_value = 1;


//自定义设置日期函数
void yk_f_set_rtc_date(uint16_t year, uint8_t month, uint8_t day)
{
	#if YK_USE_UI_BUILDER
    // 设置日期
    rt_err_t ret = set_date(year, month, day);
    if (ret != RT_EOK)
    {
        printf("set RTC date failed");
        return;
    }
	#endif
}

//自定义设置时间函数
void yk_f_set_rtc_time(uint8_t hour, uint8_t minute, uint8_t second)
{
    yk_printf("yk set RTC time: %02d:%02d:%02d \n", hour, minute, second);
	#if YK_USE_UI_BUILDER
    // 设置时间
    rt_err_t ret = set_time(hour, minute, second);
    if (ret != RT_EOK)
    {
        printf("set RTC time failed");
        return;
    }
	#endif
}

//自定义获取时间函数
void yk_f_get_rtc_time(void){
	#if 1
	// 获取并打印当前时间
    time_t now;
    struct tm *local_time;
    now = time(NULL);
    local_time = localtime(&now);
    
    // printf("date time: %04d-%02d-%02d %02d:%02d:%02d\n", local_time->tm_year+1900, local_time->tm_mon+1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
	yk_v_hor_value = local_time->tm_hour;
	yk_v_min_value = local_time->tm_min;
	yk_v_sec_value = local_time->tm_sec;
    // lv_label_set_text_fmt(screen_2_main_get(&ui_manager)->label_1_time, "%02d:%02d", local_time->tm_hour, local_time->tm_min);
    // lv_label_set_text_fmt(screen_3_down_paddle_get(&ui_manager)->label_2_time, "%02d:%02d", local_time->tm_hour, local_time->tm_min);
	#endif
}
#include "apps.h"


//自定义打印信息函数
void yk_printf(char *fmt, ...)
{
    #if YK_USE_PRINTF
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    #endif
}


//自定义驱动初始化
void yk_driver_init()
{
    printf("yk_driver_init() \n");
#if YK_USE_UI_BUILDER


#if yk_stkdriver_h
    STK_init_ALS();
#endif

#if yk_pwm_h
    yk_f_pwm_init();
#endif

#if yk_gpio_h
    yk_f_gpio_init();
#endif

#if yk_psadc_h
    yk_f_psadc_init();
#endif

#if yk_sound_play_h
    yk_f_sound_play_init();
#endif

#if yk_touch_h
    yk_f_touch_init();
#endif

#if yk_wdt_h
    yk_wdt_inti();
#endif
// yk_hrtimer_open(YK_HRTIMER_HEATING_CH);  // 启动发热丝控制定时器
// yk_hrtimer_open(YK_HRTIMER_FAN_CH);      // 启动风扇控制定时器


#endif  //YK_USE_UI_BUILDER
}
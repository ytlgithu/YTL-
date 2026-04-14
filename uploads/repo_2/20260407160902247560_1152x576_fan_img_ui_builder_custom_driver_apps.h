#ifndef __apps_H__
#define __apps_H__

#include <stdio.h>
#include <stdlib.h>
#include "../custom.h"


#ifdef YK_USE_UI_BUILDER
#include "lv_port_indev.h"
#endif

#define yk_stkdriver_h 0
#include "./inc/STKdriver.h"
#define yk_test_uart_flowctrl_h 0
#include "./inc/test_uart_flowctrl.h"
#include "./inc/yk_uart_receive_data_handle.h"
#define yk_file_rm_h 1
#include "./inc/yk_file_rm.h"
#define yk_pwm_h 1
#include "./inc/yk_pwm.h"
#define yk_gpio_h 1
#include "./inc/yk_gpio.h"
#define yk_hrtimer_h 1
#include "./inc/yk_hrtimer.h"
#include "./inc/yk_obj.h"
#define yk_psadc_h 1
#include "./inc/yk_psadc.h"
#define yk_sound_play_h 0
#include "./inc/yk_sound_play.h"
#define yk_touch_h 0
#include "./inc/yk_touch.h"
#define yk_wdt_h 1
#include "./inc/yk_wdt.h"



#define YK_USE_PRINTF 1  //是否打开自定义log信息1打开 0关闭
void yk_printf(char *fmt, ...);


void yk_driver_init();


#endif
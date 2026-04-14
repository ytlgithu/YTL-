#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_hrtimer_h


#ifndef __YK_HRTIMER_H__
#define __YK_HRTIMER_H__


// 定义使用的定时器通道数量
#define YK_HRTIMER_CH_NUM 2

// 定义发热丝控制定时器通道
#define YK_HRTIMER_HEATING_CH 0

// 定义风扇控制定时器通道
#define YK_HRTIMER_FAN_CH 1


void yk_hrtimer_open(uint8_t ch);
void yk_hrtimer_close(uint8_t ch);


#endif


#endif


#endif  //YK_USE_UI_BUILDER
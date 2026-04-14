#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_touch_h


#ifndef __YK_TOUCH_H__
#define __YK_TOUCH_H__

void yk_f_touch_init();
void yk_f_touch_shake();

#define SCREENSAVER_COUNTDOWN_TIME 150  //屏保倒计时时间单位0.1s
extern uint16_t screensaver_time;

#endif


#endif


#endif  //YK_USE_UI_BUILDER
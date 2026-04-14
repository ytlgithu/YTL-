#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_pwm_h


#ifndef __YK_PWM_H__
#define __YK_PWM_H__


void yk_f_pwm_init(void);
void yk_fd_set_pwm_brightness(uint8_t pulse);


#endif


#endif


#endif  //YK_USE_UI_BUILDER
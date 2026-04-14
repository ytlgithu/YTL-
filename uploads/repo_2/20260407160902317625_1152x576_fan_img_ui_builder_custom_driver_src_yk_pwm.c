#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_pwm_h


#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/rt_drv_pwm.h>

#define PWM_DEV_CHANNEL 2


struct rt_device_pwm *pwm_dev = RT_NULL;


static uint32_t period  = 100000;  //周期
//static uint8_t pulse = 10;  //占空比


void yk_fd_set_pwm_brightness(uint8_t pulse)
{
    printf("ytl-test05-yk_f_set_pwm_brightness() \n");
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, period*pulse*0.01);
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);

    // rt_pwm_set_output(pwm_dev, 2, 10000, 5000, 0);
    // rt_pwm_set_output(pwm_dev, 2, 10000, 2000, 1);
    // rt_pwm_enable(pwm_dev, 2);
}


// static void yk_set_pwm_brightness_cb()
// {
    
//     pulse++;
//     if (pulse == 100)
//     {
//         pulse = 10;
//     }
//     printf(" pulse == %d\n",pulse);
//     yk_func_set_pwm_brightness(pulse);
// }


void yk_f_pwm_init(void)
{
    printf("yk_f_pwm_init() \n");
    pwm_dev = (struct rt_device_pwm *)rt_device_find("pwm");
    if (pwm_dev == NULL) {
        rt_kprintf("can't find pwm device!\n");
        //return -RT_ERROR;
    }

    //lv_timer_create(yk_set_pwm_brightness_cb, 100, NULL); // 每100毫秒调用一次
}
//INIT_APP_EXPORT(yk_func_pwm);


#endif


#endif  //YK_USE_UI_BUILDER
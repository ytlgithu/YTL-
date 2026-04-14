#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_touch_h


#include <stdio.h>
#include "lvgl.h"
#include <aic_common.h>
#include <drivers/pin.h>


static lv_timer_t *touch_state_task = NULL;  //触摸状态任务
static lv_timer_t *touch_shake_task = NULL;  //触摸震动任务



uint16_t screensaver_time = SCREENSAVER_COUNTDOWN_TIME;  //屏保时间


static void touch_state_task_cb()  //触摸任务
{

	if (yk_v_operating_mode)
	{
		// if (yk_func_get_touch_state())
		// {
		// 	printf("touch_state() == %d \n",yk_func_get_touch_state());
		// 	printf("touch_x() == %d \n",yk_func_get_touch_x());
		// 	printf("touch_y() == %d \n",yk_func_get_touch_y());
		// 	screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
		// }
		if (yk_v_tap_position == 0 || yk_v_operating_mode == 2)
		{
			if (screensaver_time)
			{
				screensaver_time--;
				//printf("screensaver_time1 == %d \n",screensaver_time);
				if (yk_v_operating_mode == 1)
				{
					if (screensaver_time == SCREENSAVER_COUNTDOWN_TIME - 100) {
						yk_view_set_dis_view(SCREEN_1_A_ID);
					}
				}
				if (screensaver_time == 0)
				{
					//screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
					yk_view_set_dis_view(SCREEN_3_SCREENSAVER_ID);
				}
			}
		}
		else if (yk_v_tap_position > 1)
		{
			if (screensaver_time > SCREENSAVER_COUNTDOWN_TIME - 100)
			{
				screensaver_time--;
				printf("screensaver_time2 == %d \n",screensaver_time);
				if (screensaver_time == SCREENSAVER_COUNTDOWN_TIME - 100) {
					a_mode_img_switchover_display[0] = 1;
					if (yk_v_screen_id == SCREEN_1_A_1_ID)
					{
						yk_f_toggle_background(&ui_manager);
					}
				}
			}
		}
	}
}


//触摸重新倒计时进入待机屏保模式
void yk_f_touch_init()
{
	printf("ytl-test04-yk_f_touch_init() \n");
	//创建触摸任务
	if(touch_state_task==NULL)touch_state_task = lv_timer_create(touch_state_task_cb,100,NULL);
	lv_timer_ready(touch_state_task);
}


static void touch_shake_cb()
{
	yk_func_set_io(0);
}

//触摸震动
void yk_f_touch_shake()
{
    yk_func_set_io(1);
    //rt_thread_mdelay(100);
	if (touch_shake_task == NULL)
	{
		touch_shake_task = lv_timer_create(touch_shake_cb,150,NULL);
	}else
	{
		lv_timer_del(touch_shake_task);
		touch_shake_task = NULL;
		if (touch_shake_task == NULL)
		{
			touch_shake_task = lv_timer_create(touch_shake_cb,150,NULL);
		}
	}
	
	
	
    
}
//INIT_APP_EXPORT(yk_f_touch_init);


#endif


#endif  //YK_USE_UI_BUILDER
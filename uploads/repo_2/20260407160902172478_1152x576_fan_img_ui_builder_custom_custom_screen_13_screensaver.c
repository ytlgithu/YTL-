#include "custom.h"


#define SCREENSAVER_COUNTDOWN_TIME 10*60*2 //屏保倒计时时间单位0.1s ,默认倒计时2分钟
uint16_t screensaver_time = SCREENSAVER_COUNTDOWN_TIME;  //屏保时间


#if YK_USE_PM
//image0_task_cb 里那段低功耗倒计时是在“秒值变化时”才执行一次，等效每秒触发，因此倒计时常量应该用秒为单位。
#define PM_COUNTDOWN (60*16)
uint16_t pm_countdown = PM_COUNTDOWN;  //进入低功耗模式倒计时变量
//设置一个直接设置低功耗倒计时时间的函数,单位为秒
void screensaver_set_pm_countdown(uint16_t time)
{
	pm_countdown = time;
}
#endif


static screen_13_screensaver_t *scr = NULL;


//屏保倒计时到0后，进入低功耗模式（减低CPU频率）
static void screensaver_schedule_lowpower(void)
{
	//进入低功耗模式
	#if YK_USE_UI_BUILDER
	rt_pm_run_enter(PM_RUN_MODE_LOW_SPEED);
	#endif
}

//退出屏保时，恢复屏幕亮度和CPU频率
static void screensaver_restore_cpu_freq(void)
{
	//恢复屏幕亮度
	#if YK_USE_UI_BUILDER
	rt_pm_run_enter(PM_RUN_MODE_NORMAL_SPEED);
	#endif
}


//自定义获取触摸点击状态
uint8_t is_touch_pressed(void) {
	lv_indev_t *indev = lv_indev_get_next(NULL);
	bool touch_pressed = lv_indev_get_state(indev);
    if (touch_pressed) {
        lv_point_t point;
        lv_indev_get_point(indev, &point);  // 获取当前触摸点坐标
        int16_t touch_x = point.x;
        int16_t touch_y = point.y;
		// yk_printf("touch_x:%d,touch_y:%d\n",touch_x,touch_y);
    }
	return touch_pressed;
}


//重新计时
void screensaver_custom_re_countdown()
{

	//运行中延长进入屏保时间，避免影响烹饪过程的交互
	if (!lv_obj_has_flag(screen_02_main_get(&ui_manager)->container_2_3_run, LV_OBJ_FLAG_HIDDEN))
	{
		screensaver_time = 600*30;  //30分钟进入屏保时钟界面
	}
	else
	{
		screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
	}
	//yk_printf("last_state == %d x%d y%d\n",yk_touch_read_data.last_state,yk_touch_read_data.last_x,yk_touch_read_data.last_y);
}


//获取rtc时间任务
static void rtc_task_cb(lv_timer_t *timer)
{
	//触摸到屏幕则重新计时，避免误进入屏保
	if (is_touch_pressed())
	{
		screensaver_custom_re_countdown();
	}

	#if 1
	//屏保倒计时
	if (!yk_v_stop_run_state &&
		yk_v_screen_id != SCREEN_12_ID && 
		yk_v_screen_id != SCREEN_13_ID
		)
	{
		screensaver_time--;
		// yk_printf("screensaver_time == %d \n",screensaver_time);
		if (screensaver_time == 0) {
			screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
			if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_off) {
				yk_view_set_dis_view(SCREEN_12_ID);
				yk_printf("Shutdown screen \n");
			}else {
				yk_view_set_dis_view(SCREEN_13_ID);
				yk_printf("Screensaver screen \n");
			}
		}
	}
	#endif
	
	//获取时间函数
	yk_f_get_rtc_time();
}


//创建执行一次
void screen_13_screensaver_custom_created(void)
{ 
    scr = screen_13_screensaver_get(&ui_manager);

    //获取rtc时间任务
	lv_timer_t *rtc_task = lv_timer_create(rtc_task_cb,100,NULL);
	lv_timer_ready(rtc_task);
}

static lv_timer_t *clock_image0_task = NULL;  //创建时钟任务名

static lv_obj_t *container1 = NULL;  //创建数字容器控件名
static lv_obj_t *container2 = NULL;  //创建时钟容器控件名

static uint16_t h_angel = 222;
static uint16_t m_angel = 150;
static uint16_t s_angel = 60;

#if 1
//每次进入设置对应的时间和显示模式
static void first_display()
{
	//根据设置选择显示数字时钟或指针时钟，并处理24小时制的AM/PM显示
	if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_clock)
	{
		//显示时钟表盘显示
		lv_obj_clear_flag(container2, LV_OBJ_FLAG_HIDDEN);
	}
	else if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_digital)
	{
		//隐藏时钟表盘显示
		lv_obj_add_flag(container2, LV_OBJ_FLAG_HIDDEN);

		if (yk_settings.clock_hour_system_24)
		{
			//隐藏label:AM/PM
			lv_obj_add_flag(scr->label_2_am_pm, LV_OBJ_FLAG_HIDDEN);
		}
		else
		{
			//显示label:AM/PM
			lv_obj_clear_flag(scr->label_2_am_pm, LV_OBJ_FLAG_HIDDEN);
		}
	}
}


static uint8_t s_val = 0;  //控制秒钟值改变时,才去执行时钟表盘显示代码
static void image0_task_cb()
{
	//仅在秒值变化时刷新UI，降低刷新开销
	if (s_val != yk_v_sec_value)
	{
		s_val = yk_v_sec_value;
		if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_clock)
		{
			//屏保时钟显示
			if (yk_v_hor_value > 12) {
				h_angel = (yk_v_hor_value-12) * 30*10 + yk_v_min_value * 0.5*10;
			}else {
				h_angel = yk_v_hor_value * 30*10 + yk_v_min_value * 0.5*10;
			}
			m_angel = yk_v_min_value * 6*10 + yk_v_sec_value * 0.1*10;
			s_angel = yk_v_sec_value * 6*10;

			//yk_printf("yk_v_sec_value=%d\n",yk_v_sec_value);

			lv_img_set_angle(scr->image_1_h,h_angel);
			lv_img_set_angle(scr->image_2_m,m_angel);
			lv_img_set_angle(scr->image_3_s,s_angel);
		}
		else if(yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_digital)
		{
			if (yk_settings.clock_hour_system_24)
			{
				lv_label_set_text_fmt(scr->label_1_time,"%02d:%02d",yk_v_hor_value,yk_v_min_value);
			}else
			{
				if (yk_v_hor_value >= 12)
				{
					lv_label_set_text_fmt(scr->label_1_time,"%02d:%02d",yk_v_hor_value-12,yk_v_min_value);

					lv_label_set_text(scr->label_2_am_pm, yk_f_get_translation("pm"));
				}else
				{
					lv_label_set_text_fmt(scr->label_1_time,"%02d:%02d",yk_v_hor_value,yk_v_min_value);

					lv_label_set_text(scr->label_2_am_pm, yk_f_get_translation("am"));
				}
			}
		}
		#if YK_USE_PM
		//低功耗倒计时
		if (pm_countdown > 0) {
			pm_countdown--;
			yk_printf("pm_countdown == %d \n",pm_countdown);
			if (pm_countdown == 0) {
				yk_f_set_pwm_brightness(2);
				//屏保倒计时到0后，进入低功耗模式（减低CPU频率）
				screensaver_schedule_lowpower();
			}
		}
		#endif
	}
}

//每次进入
void screen_13_screensaver_custom_loaded()
{
	yk_printf("screen_13_screensaver_custom_loaded\n");
	yk_v_screen_id = SCREEN_13_ID;
	
	// 屏保时间为关时，就直接进入关机界面
	if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_off) {
		yk_printf("Shutdown screen \n");
		// yk_view_set_dis_view(SCREEN_17_SHUTDOWN_ID);
	}else {
		container1 = scr->container_1;
		container2 = scr->container_2;

		first_display();

		//创建屏保刷新任务：驱动指针角度/数字时间更新
		if(clock_image0_task == NULL){
			clock_image0_task = lv_timer_create(image0_task_cb,100,NULL);
			lv_timer_ready(clock_image0_task);
		}
	}
}
#endif
//每次退出
void screen_13_screensaver_custom_unloaded()
{	
	yk_printf("screen_13_screensaver_custom_unloaded\n");
	#if YK_USE_PM
	pm_countdown = PM_COUNTDOWN;
	#endif
	//退出屏保：释放定时任务并恢复亮度
	if (clock_image0_task != NULL)
	{
		lv_timer_del(clock_image0_task);
		clock_image0_task = NULL;
	}

	//恢复亮度
	yk_f_set_pwm_brightness(yk_settings.screen_brightness);
}

//点击按钮退出
void screen_13_screensaver_button_1_back_custom_pressed(void)
{
	yk_printf("screen_13_screensaver_button_1_back_custom_pressed\n");
	//退出屏保时，恢复CPU频率
	screensaver_restore_cpu_freq();
	yk_view_set_dis_view(SCREEN_02_MAIN_ID);
}

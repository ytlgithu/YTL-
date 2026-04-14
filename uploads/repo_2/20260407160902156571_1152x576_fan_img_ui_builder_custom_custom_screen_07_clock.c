#include "custom.h"


static screen_07_clock_t *scr = NULL;

#if 1
static uint8_t clock_hour_system_246 = 0;
static uint8_t clock_screensaver_time_display7 = 0;
static bool am_pm = 0;  //定义一个选中AM还是PM的变量
static uint8_t yk_set_hor_value = 12;
static uint8_t yk_set_min_value = 12;

//时间设置：切换为AM显示
void screen_07_clock_container_7_am_custom_clicked()
{
	yk_printf("screen_07_clock_container_7_am_custom_clicked\n");
	am_pm = false;
	lv_img_set_src(scr->image_7_am, LVGL_IMAGE_PATH(set_time1.png));
	lv_img_set_src(scr->image_8_pm, LVGL_IMAGE_PATH(set_time0.png));
}

//时间设置：切换为PM显示
void screen_07_clock_container_8_pm_custom_clicked()
{
	yk_printf("screen_07_clock_container_8_pm_custom_clicked\n");
	am_pm = true;
	lv_img_set_src(scr->image_7_am, LVGL_IMAGE_PATH(set_time0.png));
	lv_img_set_src(scr->image_8_pm, LVGL_IMAGE_PATH(set_time1.png));
}


//时间格式：24小时制开关点击（同步滚轮与AM/PM显示）
void screen_07_clock_switch_1_24_hour_custom_clicked()
{
	yk_printf("screen_07_clock_switch_1_24_hour_custom_clicked\n");
	bool sw_get_state = lv_obj_has_state(scr->switch_1_24_hour,LV_STATE_CHECKED);
	yk_printf("switch_1_24_hour_clicked_event_handler sw_get_state = %d\n",sw_get_state);
	

	uint8_t yk_hor_value_set = 0;
	//根据当前显示模式，将滚轮值换算为24小时制小时值
	if (yk_settings.clock_hour_system_24) {
		yk_hor_value_set = lv_roller_get_selected(scr->roller_2_24h);
	}else {
		if (am_pm) {
			yk_hor_value_set = lv_roller_get_selected(scr->roller_1_12h)+12+1;
			if (yk_hor_value_set == 24) {
				yk_hor_value_set = 12;
			}
		}else {
			yk_hor_value_set = lv_roller_get_selected(scr->roller_1_12h)+1;
			if (yk_hor_value_set == 12)
			{
				yk_hor_value_set = 0;
			}
			
		}
	}

	//根据开关状态切换控件可见性：24小时滚轮 vs 12小时滚轮+AM/PM
	if (sw_get_state) {
		//隐藏AM PM 容器控件
		lv_obj_add_flag(scr->container_4_am_pm, LV_OBJ_FLAG_HIDDEN);
		//显示24 小时h 滚轮控件
		lv_obj_clear_flag(scr->roller_2_24h, LV_OBJ_FLAG_HIDDEN);
		//隐藏12 小时h 滚轮控件
		lv_obj_add_flag(scr->roller_1_12h, LV_OBJ_FLAG_HIDDEN);
	}else {
		//显示AM PM 容器控件
		lv_obj_clear_flag(scr->container_4_am_pm, LV_OBJ_FLAG_HIDDEN);
		//隐藏24 小时h 滚轮控件
		lv_obj_add_flag(scr->roller_2_24h, LV_OBJ_FLAG_HIDDEN);
		//显示12 小时模式滚轮控件
		lv_obj_clear_flag(scr->roller_1_12h, LV_OBJ_FLAG_HIDDEN);
	}
	yk_settings.clock_hour_system_24 = sw_get_state;

	//切换显示模式后，滚轮定位到同一实际小时值
	if (yk_settings.clock_hour_system_24) {
		//获取时间，滚轮显示对应时间
		lv_roller_set_selected(scr->roller_2_24h,yk_hor_value_set,LV_ANIM_OFF);
	}else {
		//获取时间，滚轮显示对应时间
		if (yk_hor_value_set > 12) {
			lv_roller_set_selected(scr->roller_1_12h,yk_hor_value_set-12-1,LV_ANIM_OFF);
		}else {
			lv_roller_set_selected(scr->roller_1_12h,yk_hor_value_set-1,LV_ANIM_OFF);
		}
	}

	// 根据当前时间选择AM或PM
	if (yk_hor_value_set >= 12) {
		screen_07_clock_container_8_pm_custom_clicked();
	} else {
		screen_07_clock_container_7_am_custom_clicked();
	}

    //延迟保存数据
    yk_delay_save_data_timer_start();
}


//点击ok键修改时间
//时间设置：确认保存并写入RTC，然后返回设置页
void screen_07_clock_button_1_ok_custom_clicked()
{
	yk_printf("screen_07_clock_button_1_ok_custom_clicked\n");
	//将当前滚轮选择换算为24小时制小时值
	if (yk_settings.clock_hour_system_24) {
		yk_set_hor_value = lv_roller_get_selected(scr->roller_2_24h);
	}else {
		if (am_pm) {
			yk_set_hor_value = lv_roller_get_selected(scr->roller_1_12h)+12+1;
			if (yk_set_hor_value == 24) {
				yk_set_hor_value = 12;
			}
		}else {
			yk_set_hor_value = lv_roller_get_selected(scr->roller_1_12h)+1;
			if (yk_set_hor_value == 12)
			{
				yk_set_hor_value = 0;
			}
		}
	}
	yk_set_min_value = lv_roller_get_selected(scr->roller_3_m);

	//保存时间到核心模块RTC里面
	yk_f_set_rtc_date(2025, 2, 14);
	// 延迟 1 毫秒用于 RTC 同步
	#if YK_USE_UI_BUILDER
    rt_thread_mdelay(1);
	#endif
	yk_f_set_rtc_time(yk_set_hor_value, yk_set_min_value, 1);

	//按确认键,时间设置完了,返回到设置界面
	yk_view_set_dis_view(SCREEN_03_ID);
}

//屏保时间显示：选择数字时钟样式
void screen_07_clock_container_3_display_custom_clicked()
{
	yk_printf("screen_07_clock_container_3_display_custom_clicked\n");
	lv_img_set_src(scr->image_3_digital, LVGL_IMAGE_PATH(set_time1.png));
	lv_img_set_src(scr->image_4_clock, LVGL_IMAGE_PATH(set_time0.png));
	// lv_img_set_src(scr->image_5_on_off, LVGL_IMAGE_PATH(set_time0.png));

	yk_settings.clock_screensaver_time_display = val7_clock_screensaver_time_display_digital;

    //延迟保存数据
    yk_delay_save_data_timer_start();
}

//屏保时间显示：选择指针时钟样式
void screen_07_clock_container_4_clock_custom_clicked()
{
	yk_printf("screen_07_clock_container_4_clock_custom_clicked\n");
	lv_img_set_src(scr->image_3_digital, LVGL_IMAGE_PATH(set_time0.png));
	lv_img_set_src(scr->image_4_clock, LVGL_IMAGE_PATH(set_time1.png));
	// lv_img_set_src(scr->image_5_on_off, LVGL_IMAGE_PATH(set_time0.png));

	yk_settings.clock_screensaver_time_display = val7_clock_screensaver_time_display_clock;

    //延迟保存数据
    yk_delay_save_data_timer_start();
}

// void screen_13_clock_settings_container_5_on_off_custom_clicked()
// {
// 	yk_printf("screen_13_clock_settings_container_5_on_off_custom_clicked\n");
// 	lv_img_set_src(scr->image_3_digital, LVGL_IMAGE_PATH(set_time0.png));
// 	lv_img_set_src(scr->image_4_clock, LVGL_IMAGE_PATH(set_time0.png));
// 	// lv_img_set_src(scr->image_5_on_off, LVGL_IMAGE_PATH(set_time1.png));

// 	yk_settings.clock_screensaver_time_display = val7_clock_screensaver_time_display_off;
// }


//进入
//页面进入：加载当前配置并刷新时间显示/滚轮状态
void screen_07_clock_custom_loaded(void) {
	yk_printf("screen_07_clock_custom_loaded\n");

	clock_hour_system_246 = yk_settings.clock_hour_system_24;
	clock_screensaver_time_display7 = yk_settings.clock_screensaver_time_display;


	//屏保显示对应按钮
	if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_digital) {
		lv_img_set_src(scr->image_3_digital, LVGL_IMAGE_PATH(set_time1.png));
		lv_img_set_src(scr->image_4_clock, LVGL_IMAGE_PATH(set_time0.png));
		// lv_img_set_src(scr->image_5_on_off, LVGL_IMAGE_PATH(set_time0.png));
	}else if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_clock) {
		lv_img_set_src(scr->image_3_digital, LVGL_IMAGE_PATH(set_time0.png));
		lv_img_set_src(scr->image_4_clock, LVGL_IMAGE_PATH(set_time1.png));
		// lv_img_set_src(scr->image_5_on_off, LVGL_IMAGE_PATH(set_time0.png));
	}else if (yk_settings.clock_screensaver_time_display == val7_clock_screensaver_time_display_off) {
		lv_img_set_src(scr->image_3_digital, LVGL_IMAGE_PATH(set_time0.png));
		lv_img_set_src(scr->image_4_clock, LVGL_IMAGE_PATH(set_time0.png));
		// lv_img_set_src(scr->image_5_on_off, LVGL_IMAGE_PATH(set_time1.png));
	}

	//根据24小时制设置切换显示控件，并设置滚轮对应时间
	if (clock_hour_system_246) {
		lv_obj_set_state(scr->switch_1_24_hour, LV_ANIM_ON,true);
		//隐藏AM PM 容器控件
		lv_obj_add_flag(scr->container_4_am_pm, LV_OBJ_FLAG_HIDDEN);
		//显示24 小时h 滚轮控件
		lv_obj_clear_flag(scr->roller_2_24h, LV_OBJ_FLAG_HIDDEN);
		//隐藏12 小时h 滚轮控件
		lv_obj_add_flag(scr->roller_1_12h, LV_OBJ_FLAG_HIDDEN);

		//设置滚轮列表显示对应的时间
		lv_roller_set_selected(scr->roller_2_24h,yk_v_hor_value,LV_ANIM_OFF);
	}else {
		lv_obj_set_state(scr->switch_1_24_hour, LV_ANIM_OFF,true);
		//显示AM PM 容器控件
		lv_obj_clear_flag(scr->container_4_am_pm, LV_OBJ_FLAG_HIDDEN);
		//隐藏24 小时h 滚轮控件
		lv_obj_add_flag(scr->roller_2_24h, LV_OBJ_FLAG_HIDDEN);
		//显示12 小时模式滚轮控件
		lv_obj_clear_flag(scr->roller_1_12h, LV_OBJ_FLAG_HIDDEN);
	}

	//设置滚轮列表h显示对应的时间
	if (yk_v_hor_value > 12) {
		lv_roller_set_selected(scr->roller_1_12h,yk_v_hor_value-12-1,LV_ANIM_OFF);
		screen_07_clock_container_8_pm_custom_clicked();
	}else {
		lv_roller_set_selected(scr->roller_1_12h,yk_v_hor_value-1,LV_ANIM_OFF);
		screen_07_clock_container_7_am_custom_clicked();
	}
	//设置滚轮列表m显示对应的时间
	lv_roller_set_selected(scr->roller_3_m,yk_v_min_value,LV_ANIM_OFF);
}
#endif


//创建执行一次
//首次创建：获取屏幕对象
void screen_07_clock_custom_created(void)
{ 
    scr = screen_07_clock_get(&ui_manager);
}

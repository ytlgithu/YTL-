#include "custom.h"


static screen_01_logo_t *scr = NULL;


//加载视图屏幕资源,但是不显示
static void yk_screen_load()
{
	#if 1
	screen_02_main_create(&ui_manager);
	screen_03_setting_create(&ui_manager);
	screen_05_language_create(&ui_manager);
	screen_06_brightness_create(&ui_manager);
	screen_07_clock_create(&ui_manager);
	screen_08_sound_create(&ui_manager);
	screen_09_F_to_C_create(&ui_manager);
	screen_10_version_create(&ui_manager);
	screen_11_show_all_create(&ui_manager);
	screen_12_idle_create(&ui_manager);
	screen_13_screensaver_create(&ui_manager);
	screen_14_love_favorites_create(&ui_manager);
	screen_15_restore_factory_settings_create(&ui_manager);
	screen_04_display_create(&ui_manager);

	#endif
	//根据已保存语言设置初始化当前语言环境
	if (yk_settings.language == val3_language_en) {
		yk_f_set_language("en");
	}else if (yk_settings.language == val3_language_zh) {
		yk_f_set_language("zh");
	}
	else if (yk_settings.language == val3_language_fr) {
		yk_f_set_language("fr");
	}
	else if (yk_settings.language == val3_language_sp) {
		yk_f_set_language("sp");
	}
	else if (yk_settings.language == val3_language_ja) {
		yk_f_set_language("ja");
	}
	else if (yk_settings.language == val3_language_de) {
		yk_f_set_language("de");
	}
	else if (yk_settings.language == val3_language_it) {
		yk_f_set_language("it");
	}
}


//logo界面图渐变显示
static uint8_t logo_task_i = 0;
static void logo_task_cb(lv_timer_t *timer)
{
    logo_task_i++;
    if (logo_task_i == 2) {
		lv_obj_set_style_bg_img_src(scr->obj, LVGL_IMAGE_PATH(logo100_welcome.png), LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else if (logo_task_i == 3)
	{
		yk_screen_load();
	}
	else if (logo_task_i >= 5) {
		//PWM设置屏幕亮度测试
		yk_f_set_pwm_brightness(yk_settings.screen_brightness);
		
		yk_view_set_dis_view(SCREEN_02_MAIN_ID);
        lv_timer_del(timer);
		#if P_AGING_TEST  //跳转烘烤界面老化测试用
		food_tf_time(200,60);
		yk_view_set_dis_view(SCREEN_3_BAKE_ID);
		#endif
	}
	yk_printf("logo_task_i == %d\n",logo_task_i);
}



//首次创建：启动logo渐变定时器并延迟进入主界面
void screen_01_logo_custom_created()
{
    scr = screen_01_logo_get(&ui_manager);
    //logo界面图渐变显示
    lv_timer_create(logo_task_cb,900,NULL);
}

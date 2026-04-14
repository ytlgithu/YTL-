#include "custom.h"

static screen_15_restore_factory_settings_t *scr = NULL;


//恢复出厂设置：清空收藏与自定义文案等用户数据
static void factory_reset_apply_defaults(void)
{
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        yk_settings.user_favorites[i].is_favorite = 0;
        snprintf(yk_settings.user_favorites[i].name, sizeof(yk_settings.user_favorites[i].name), "name%02d", i + 1);
        yk_settings.user_favorites[i].customize[0] = '\0';
        yk_settings.user_favorites[i].steps_custom[0] = '\0';
        yk_settings.user_favorites[i].flag = 0;
        yk_settings.user_favorites[i].temperature = 0;
        yk_settings.user_favorites[i].time = 0;
    }

    for (int i = 0; i < YK_MODE_COUNT; i++) {
        yk_settings.food_customize[i][0] = '\0';
        yk_settings.food_steps_custom[i][0] = '\0';
    }
}

//恢复出厂设置确认：执行清空并提示结果
static void factory_reset_confirm_ok_clicked(lv_event_t *e)
{
    (void)e;
    //重置设置结构体为默认值
    yk_settings_init(&yk_settings);
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
    //应用默认主题
    screen_04_display_custom_created();
    screen_05_language_custom_created();
    screen_06_brightness_custom_created();
    yk_f_set_pwm_brightness(yk_settings.screen_brightness);
    screen_08_sound_custom_created();
    screen_09_F_to_C_custom_created();
    food_tf_time(yk_v_food_t_f,yk_v_food_min);

    //应用默认设置到文件存储中
    factory_reset_apply_defaults();
    //保存设置到文件
    yk_delay_save_data_timer_start();
    //重新初始化文件系统模块
    yk_favorites_reload_from_settings();
    //刷新主界面收藏图标与文本
    yk_main_refresh_love_icon();
    //更新当前界面文本显示
    yk_main_update_ui_text();
    yk_ui_center_toast_show(yk_ui_tr("factory_reset_success", "恢复出厂设置成功"), 3000);
}

//恢复出厂设置取消：关闭提示
static void factory_reset_confirm_cancel_clicked(lv_event_t *e)
{
    (void)e;
    yk_ui_center_toast_dismiss();
}

//点击出现提示弹框，提示是否确认恢复出厂设置
void screen_15_restore_factory_settings_container_1_restore_factory_settings_custom_clicked(void)
{
    yk_printf("screen_15_restore_factory_settings_container_1_restore_factory_settings_custom_clicked\n");

    //界面对象未就绪时不弹窗，避免空指针访问
    if (!scr || !scr->obj) return;
    yk_ui_confirm_show(yk_ui_tr("factory_reset_warning", "恢复出厂设置后，所有数据将被删除，请谨慎操作。"),
                       720,
                       260,
                       yk_ui_tr("cancel", "取消"),
                       yk_ui_tr("confirm", "确认"),
                       factory_reset_confirm_cancel_clicked,
                       factory_reset_confirm_ok_clicked);
}


//创建执行一次
void screen_15_restore_factory_settings_custom_created(void)
{ 
    scr = screen_15_restore_factory_settings_get(&ui_manager);
}

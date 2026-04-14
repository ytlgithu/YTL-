#include "custom.h"

//切换语言==============================================================================================================
// 语言切换后刷新全局 UI 文本（含主界面信息区与各设置界面）
void yk_f_update_ui_text() {
#define YK_SET_LBL(lbl, key) do { if ((lbl) != NULL) lv_label_set_text((lbl), yk_f_get_translation((key))); } while (0)
	screen_02_main_t *screen_02_main_scr = screen_02_main_get(&ui_manager);
	if (screen_02_main_scr) {
		YK_SET_LBL(screen_02_main_scr->label_1_airfry, "airfry");
		YK_SET_LBL(screen_02_main_scr->label_2_bake, "bake");
		YK_SET_LBL(screen_02_main_scr->label_3_preheat, "preheat");
		YK_SET_LBL(screen_02_main_scr->label_4_dehydrate, "dehydrate");
		YK_SET_LBL(screen_02_main_scr->label_7_ingredients, "ingredients");
		YK_SET_LBL(screen_02_main_scr->label_8_steps, "steps");
		YK_SET_LBL(screen_02_main_scr->label_9_customize, "customize");
		YK_SET_LBL(screen_02_main_scr->label_run_shake, "shake");
		YK_SET_LBL(screen_02_main_scr->label_run_cancel, "cancel");
		YK_SET_LBL(screen_02_main_scr->label_6_pop_up_1_insert_basket, "insert_basket");
	}

	//设置界面
	screen_03_setting_t *screen_03_setting_scr = screen_03_setting_get(&ui_manager);
	if (screen_03_setting_scr) {
		YK_SET_LBL(screen_03_setting_scr->label_0_settings, "setting");
		YK_SET_LBL(screen_03_setting_scr->label_1_display, "display");
		YK_SET_LBL(screen_03_setting_scr->label_2_language, "language");
		YK_SET_LBL(screen_03_setting_scr->label_3_brightness, "brightness");
		YK_SET_LBL(screen_03_setting_scr->label_4_clock, "clock");
		YK_SET_LBL(screen_03_setting_scr->label_5_sound, "sound");
		YK_SET_LBL(screen_03_setting_scr->label_6_f_to_c, "f_to_c");
		YK_SET_LBL(screen_03_setting_scr->label_7_version, "version");
		YK_SET_LBL(screen_03_setting_scr->label_8_restore_factory_settings, "restore_factory_settings");
	}

	//主题界面
	screen_04_display_t *screen_04_display_scr = screen_04_display_get(&ui_manager);
	if (screen_04_display_scr) {
		YK_SET_LBL(screen_04_display_scr->label_0_display, "display");
		YK_SET_LBL(screen_04_display_scr->label_1_ligth, "light");
		YK_SET_LBL(screen_04_display_scr->label_2_dark, "dark");
	}

	//语言界面
	screen_05_language_t *screen_05_language_scr = screen_05_language_get(&ui_manager);
	if (screen_05_language_scr) {
		YK_SET_LBL(screen_05_language_scr->label_0_language, "language");
		YK_SET_LBL(screen_05_language_scr->label_1_english, "english");
		YK_SET_LBL(screen_05_language_scr->label_2_chinese, "chinese");
		YK_SET_LBL(screen_05_language_scr->label_3_french, "french");
		YK_SET_LBL(screen_05_language_scr->label_4_spanish, "spanish");
		YK_SET_LBL(screen_05_language_scr->label_5_japan, "japan");
		YK_SET_LBL(screen_05_language_scr->label_6_german, "german");
		YK_SET_LBL(screen_05_language_scr->label_7_italian, "italian");
	}

	//亮度界面
	screen_06_brightness_t *screen_06_brightness_scr = screen_06_brightness_get(&ui_manager);
	if (screen_06_brightness_scr) {
		YK_SET_LBL(screen_06_brightness_scr->label_0_brightness, "brightness");
		YK_SET_LBL(screen_06_brightness_scr->label_1_brightness, "brightness");
	}

	//时钟界面
	screen_07_clock_t *screen_07_clock_scr = screen_07_clock_get(&ui_manager);
	if (screen_07_clock_scr) {
		YK_SET_LBL(screen_07_clock_scr->label_0_clock, "clock");
		YK_SET_LBL(screen_07_clock_scr->label_1_24_hour, "hour_system_24");
		YK_SET_LBL(screen_07_clock_scr->label_2_clock, "clock");
		YK_SET_LBL(screen_07_clock_scr->label_3_am, "am");
		YK_SET_LBL(screen_07_clock_scr->label_4_pm, "pm");
		YK_SET_LBL(screen_07_clock_scr->label_5_time, "time");
		YK_SET_LBL(screen_07_clock_scr->label_6_screensaver_time_display, "screensaver_time_display");
		YK_SET_LBL(screen_07_clock_scr->label_7_digital, "digital");
		YK_SET_LBL(screen_07_clock_scr->label_8_clock, "clock");
	}

	//声音界面
	screen_08_sound_t *screen_08_sound_scr = screen_08_sound_get(&ui_manager);
	if (screen_08_sound_scr) {
		YK_SET_LBL(screen_08_sound_scr->label_0_sound, "sound");
		YK_SET_LBL(screen_08_sound_scr->label_1_sound, "sound");
	}

	//f_c界面
	screen_09_F_to_C_t *screen_09_F_to_C_scr = screen_09_F_to_C_get(&ui_manager);
	if (screen_09_F_to_C_scr) {
		YK_SET_LBL(screen_09_F_to_C_scr->label_0_f_to_c, "f_to_c");
		YK_SET_LBL(screen_09_F_to_C_scr->label_1_f_to_c, "f_to_c");
	}

	//版本界面
	screen_10_version_t *screen_10_version_scr = screen_10_version_get(&ui_manager);
	if (screen_10_version_scr) {
		YK_SET_LBL(screen_10_version_scr->label_0_version, "version");
		YK_SET_LBL(screen_10_version_scr->label_1_version, "version");
	}

	screen_11_show_all_t *screen_11_show_all_scr = screen_11_show_all_get(&ui_manager);
	if (screen_11_show_all_scr) {
		YK_SET_LBL(screen_11_show_all_scr->label_1_choose_mode, "choose_mode");
	}

	screen_15_restore_factory_settings_t *screen_15_restore_factory_settings_scr = screen_15_restore_factory_settings_get(&ui_manager);
	if (screen_15_restore_factory_settings_scr) {
		YK_SET_LBL(screen_15_restore_factory_settings_scr->label_0_restore_factory_settings, "restore_factory_settings");
		YK_SET_LBL(screen_15_restore_factory_settings_scr->label_1_restore_factory_settings, "restore_factory_settings");
	}

	yk_main_update_ui_text();
	yk_favorites_update_ui_text();
#undef YK_SET_LBL
}

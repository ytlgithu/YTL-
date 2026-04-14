#include "custom.h"

yk_view_id_e yk_v_screen_id = 0;  //当前视图ID


//自定义切换界面函数
void yk_view_set_dis_view(yk_view_id_e v_id)
{
    lv_obj_t *act_scr = lv_scr_act();
	switch(v_id)
	{
		case SCREEN_01_LOGO_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_01_logo_create(&ui_manager);
				lv_scr_load(screen_01_logo_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_02_MAIN_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_02_main_create(&ui_manager);
				lv_scr_load(screen_02_main_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_03_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_03_setting_create(&ui_manager);
				lv_scr_load(screen_03_setting_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_04_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_04_display_create(&ui_manager);
				lv_scr_load(screen_04_display_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_05_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_05_language_create(&ui_manager);
				lv_scr_load(screen_05_language_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_06_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_06_brightness_create(&ui_manager);
				lv_scr_load(screen_06_brightness_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_07_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_07_clock_create(&ui_manager);
				lv_scr_load(screen_07_clock_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_08_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_08_sound_create(&ui_manager);
				lv_scr_load(screen_08_sound_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_09_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_09_F_to_C_create(&ui_manager);
				lv_scr_load(screen_09_F_to_C_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_10_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_10_version_create(&ui_manager);
				lv_scr_load(screen_10_version_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_11_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_11_show_all_create(&ui_manager);
				lv_scr_load(screen_11_show_all_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_12_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_12_idle_create(&ui_manager);
				lv_scr_load(screen_12_idle_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_13_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_13_screensaver_create(&ui_manager);
				lv_scr_load(screen_13_screensaver_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_14_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_14_love_favorites_create(&ui_manager);
				lv_scr_load(screen_14_love_favorites_get(&ui_manager)->obj);
			}
			break;
		case SCREEN_15_ID:
			if (!screen_is_loading(act_scr))
			{
				//yk_v_screen_id = v_id;
				screen_15_restore_factory_settings_create(&ui_manager);
				lv_scr_load(screen_15_restore_factory_settings_get(&ui_manager)->obj);
			}
			break;
		default:
            // 处理未定义的枚举值，包括 SCREEN_NULL
            yk_printf("Warning: Unknown screen ID %d\n", v_id);
            break;
    }
	yk_printf("yk_v_screen_id =!!!!= %d \n",yk_v_screen_id);
}

#if 0
void yk_view_set_dis_view_SCREEN_2_MAIN_ID()
{
	yk_view_set_dis_view(SCREEN_2_MAIN_ID);
}
#endif
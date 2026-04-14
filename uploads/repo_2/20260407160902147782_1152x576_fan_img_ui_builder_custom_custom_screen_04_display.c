#include "custom.h"


static screen_04_display_t *scr = NULL;


//点击亮色
// 切换为亮色主题：同步全局设置、更新各界面颜色/图标
void screen_04_display_container_1_ligth_custom_clicked(void)
{ 
    lv_obj_clear_flag(scr->image_1_ligth, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_2_dark, LV_OBJ_FLAG_HIDDEN);
    yk_settings.display_mode = val1_display_mode_light;
    //在当前页面点击保存数据
    if(yk_v_screen_id == SCREEN_04_ID)yk_delay_save_data_timer_start();
    //修改背景颜色
    lv_obj_set_style_bg_color(scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改容器颜色
    lv_obj_set_style_bg_color(scr->container_1_ligth, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(scr->container_2_dark, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改文字颜色
    lv_obj_set_style_text_color(scr->label_1_ligth, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr->label_2_dark, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改返回按钮背景图片
    lv_obj_set_style_bg_img_src(scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);

    screen_02_main_t *screen_02_main_scr = screen_02_main_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_02_main_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改容器颜色
    lv_obj_set_style_bg_color(screen_02_main_scr->container_6_pop_up_1, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改文字颜色
    lv_obj_set_style_text_color(screen_02_main_scr->label_1_airfry, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_2_bake, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_3_preheat, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_4_dehydrate, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_6_pop_up_1_insert_basket, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);

    //修改图标
    lv_img_set_src(screen_02_main_scr->image_4_t, LVGL_IMAGE_PATH(t01.png));
    lv_img_set_src(screen_02_main_scr->image_5_time, LVGL_IMAGE_PATH(time01.png));
    lv_obj_set_style_bg_img_src(screen_02_main_scr->button_2_x, LVGL_IMAGE_PATH(icon_x.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //灯
    if (yk_v_lamp_state)
    {
        lv_img_set_src(screen_02_main_scr->image_6_lamp, LVGL_IMAGE_PATH(lamp_on.png));
    }
    else
    {
        lv_img_set_src(screen_02_main_scr->image_6_lamp, LVGL_IMAGE_PATH(lamp_off.png));
    }
    //停止运行图标
    if(yk_v_stop_run_state)
    {
        lv_img_set_src(screen_02_main_scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_1.png));
    }
    else
    {
        lv_img_set_src(screen_02_main_scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_0.png));
    }

    //设置界面
    screen_03_setting_t *screen_03_setting_scr = screen_03_setting_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_03_setting_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_03_setting_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_1_display, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_2_language, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_3_brightness, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_4_clock, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_5_sound, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_6_f_to_c, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_7_version, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_8_restore_factory_settings, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变图标
    lv_img_set_src(screen_03_setting_scr->image_1_display, LVGL_IMAGE_PATH(set_icon1_display.png));
    lv_img_set_src(screen_03_setting_scr->image_2_language, LVGL_IMAGE_PATH(set_icon2_language.png));
    lv_img_set_src(screen_03_setting_scr->image_3_brightness, LVGL_IMAGE_PATH(set_icon3_brightness.png));
    lv_img_set_src(screen_03_setting_scr->image_4_clock, LVGL_IMAGE_PATH(set_icon4_clock.png));
    lv_img_set_src(screen_03_setting_scr->image_5_sound, LVGL_IMAGE_PATH(set_icon5_sound.png));
    lv_img_set_src(screen_03_setting_scr->image_6_f_to_c, LVGL_IMAGE_PATH(set_icon6_f_c.png));
    lv_img_set_src(screen_03_setting_scr->image_7_version, LVGL_IMAGE_PATH(set_icon7_version.png));
    lv_img_set_src(screen_03_setting_scr->image_8_restore_factory_settings, LVGL_IMAGE_PATH(set_icon8_restore.png));
    //改变文本颜色
    lv_obj_set_style_text_color(screen_03_setting_scr->label_1_display, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_2_language, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_3_brightness, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_4_clock, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_5_sound, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_6_f_to_c, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_7_version, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_8_restore_factory_settings, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);

    //语言界面
    screen_05_language_t *screen_05_language_scr = screen_05_language_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_05_language_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_05_language_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_05_language_scr->container_1_english, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_2_chinese, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_3_french, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_4_spanish, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_5_japan, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_6_german, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_7_italian, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_05_language_scr->label_1_english, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_2_chinese, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_3_french, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_4_spanish, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_5_japan, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_6_german, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_7_italian, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //背光亮度界面
    screen_06_brightness_t *screen_06_brightness_scr = screen_06_brightness_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_06_brightness_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_06_brightness_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_06_brightness_scr->container_1, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_06_brightness_scr->label_1_brightness, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);

    //时钟界面
    screen_07_clock_t *screen_07_clock_scr = screen_07_clock_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_07_clock_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_07_clock_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_07_clock_scr->container_1, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_07_clock_scr->container_2, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_07_clock_scr->container_3, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_07_clock_scr->label_1_24_hour, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_2_clock, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_3_am, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_4_pm, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_5_time, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_6_screensaver_time_display, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_7_digital, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_8_clock, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变滑块颜色
    lv_obj_set_style_text_color(screen_07_clock_scr->roller_1_12h, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->roller_2_24h, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->roller_3_m, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);

    //声音界面
    screen_08_sound_t *screen_08_sound_scr = screen_08_sound_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_08_sound_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_08_sound_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_08_sound_scr->container_1_sound, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_08_sound_scr->label_1_sound, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);

    //F_to_C界面
    screen_09_F_to_C_t *screen_09_F_to_C_scr = screen_09_F_to_C_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_09_F_to_C_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_09_F_to_C_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_09_F_to_C_scr->container_1_F_to_C, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_09_F_to_C_scr->label_1_f_to_c, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_09_F_to_C_scr->label_2_f, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_09_F_to_C_scr->label_3_c, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);

    //版本界面
    screen_10_version_t *screen_10_version_scr = screen_10_version_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_10_version_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_10_version_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_10_version_scr->container_1, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_10_version_scr->label_1_version, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_10_version_scr->label_2_version_V, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);

    //展示全部食物界面
    screen_11_show_all_t *screen_11_show_all_scr = screen_11_show_all_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_11_show_all_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器背景颜色
    lv_obj_set_style_bg_color(screen_11_show_all_scr->container_2_show_all, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    // //修改文字颜色
    // lv_obj_set_style_text_color(screen_11_show_all_scr->label_1_choose_mode, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //按钮x图片
    lv_obj_set_style_bg_img_src(screen_11_show_all_scr->button_1_x, LVGL_IMAGE_PATH(icon_x.png), LV_PART_MAIN | LV_STATE_DEFAULT);

    //收藏界面
    screen_14_love_favorites_t *screen_14_love_favorites_scr = screen_14_love_favorites_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_14_love_favorites_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器背景颜色
    lv_obj_set_style_bg_color(screen_14_love_favorites_scr->container_2_favorites, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    // //修改文字颜色
    // lv_obj_set_style_text_color(screen_11_show_all_scr->label_1_choose_mode, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //按钮x图片
    lv_obj_set_style_bg_img_src(screen_14_love_favorites_scr->button_1_x, LVGL_IMAGE_PATH(icon_x.png), LV_PART_MAIN | LV_STATE_DEFAULT);

    //恢复出厂设置界面
    screen_15_restore_factory_settings_t *screen_15_restore_factory_settings_scr = screen_15_restore_factory_settings_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_15_restore_factory_settings_scr->obj, lv_color_hex(YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //返回按钮图片
    lv_obj_set_style_bg_img_src(screen_15_restore_factory_settings_scr->button_1_back, LVGL_IMAGE_PATH(back.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器背景颜色
    lv_obj_set_style_bg_color(screen_15_restore_factory_settings_scr->container_1_restore_factory_settings, lv_color_hex(YK_COLOR_COUNT_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改文字颜色
    lv_obj_set_style_text_color(screen_15_restore_factory_settings_scr->label_1_restore_factory_settings, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
}

//点击暗色
// 切换为暗色主题：同步全局设置、更新各界面颜色/图标
void screen_04_display_container_2_dark_custom_clicked(void)
{ 
    lv_obj_add_flag(scr->image_1_ligth, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scr->image_2_dark, LV_OBJ_FLAG_HIDDEN);
    yk_settings.display_mode = val1_display_mode_dark;
    //在当前页面点击保存数据
    if(yk_v_screen_id == SCREEN_04_ID)yk_delay_save_data_timer_start();
    //修改背景颜色
    lv_obj_set_style_bg_color(scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改容器颜色
    lv_obj_set_style_bg_color(scr->container_1_ligth, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(scr->container_2_dark, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改文字颜色
    lv_obj_set_style_text_color(scr->label_1_ligth, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr->label_2_dark, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改返回按钮背景图片
    lv_obj_set_style_bg_img_src(scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);

    screen_02_main_t *screen_02_main_scr = screen_02_main_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_02_main_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改容器颜色
    lv_obj_set_style_bg_color(screen_02_main_scr->container_6_pop_up_1, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改文字颜色
    lv_obj_set_style_text_color(screen_02_main_scr->label_1_airfry, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_2_bake, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_3_preheat, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_4_dehydrate, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_02_main_scr->label_6_pop_up_1_insert_basket, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);

    //修改图标
    lv_img_set_src(screen_02_main_scr->image_4_t, LVGL_IMAGE_PATH(t01_dark.png));
    lv_img_set_src(screen_02_main_scr->image_5_time, LVGL_IMAGE_PATH(time01_dark.png));
    lv_obj_set_style_bg_img_src(screen_02_main_scr->button_2_x, LVGL_IMAGE_PATH(icon_x_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //灯
    if (yk_v_lamp_state)
    {
        lv_img_set_src(screen_02_main_scr->image_6_lamp, LVGL_IMAGE_PATH(lamp_on.png));
    }
    else
    {
        lv_img_set_src(screen_02_main_scr->image_6_lamp, LVGL_IMAGE_PATH(lamp_off_dark.png));
    }
    //停止运行图标
    if(yk_v_stop_run_state)
    {
        lv_img_set_src(screen_02_main_scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_1.png));
    }
    else
    {
        lv_img_set_src(screen_02_main_scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_0_dark.png));
    }

    //设置界面
    screen_03_setting_t *screen_03_setting_scr = screen_03_setting_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_03_setting_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_03_setting_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_1_display, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_2_language, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_3_brightness, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_4_clock, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_5_sound, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_6_f_to_c, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_7_version, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_03_setting_scr->container_8_restore_factory_settings, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变图标
    lv_img_set_src(screen_03_setting_scr->image_1_display, LVGL_IMAGE_PATH(set_icon1_display_dark.png));
    lv_img_set_src(screen_03_setting_scr->image_2_language, LVGL_IMAGE_PATH(set_icon2_language_dark.png));
    lv_img_set_src(screen_03_setting_scr->image_3_brightness, LVGL_IMAGE_PATH(set_icon3_brightness_dark.png));
    lv_img_set_src(screen_03_setting_scr->image_4_clock, LVGL_IMAGE_PATH(set_icon4_clock_dark.png));
    lv_img_set_src(screen_03_setting_scr->image_5_sound, LVGL_IMAGE_PATH(set_icon5_sound_dark.png));
    lv_img_set_src(screen_03_setting_scr->image_6_f_to_c, LVGL_IMAGE_PATH(set_icon6_f_c_dark.png));
    lv_img_set_src(screen_03_setting_scr->image_7_version, LVGL_IMAGE_PATH(set_icon7_version_dark.png));
    lv_img_set_src(screen_03_setting_scr->image_8_restore_factory_settings, LVGL_IMAGE_PATH(set_icon8_restore_dark.png));
    //改变文本颜色
    lv_obj_set_style_text_color(screen_03_setting_scr->label_1_display, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_2_language, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_3_brightness, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_4_clock, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_5_sound, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_6_f_to_c, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_7_version, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_03_setting_scr->label_8_restore_factory_settings, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);

    //语言界面
    screen_05_language_t *screen_05_language_scr = screen_05_language_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_05_language_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_05_language_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_05_language_scr->container_1_english, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_2_chinese, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_3_french, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_4_spanish, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_5_japan, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_6_german, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_05_language_scr->container_7_italian, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_05_language_scr->label_1_english, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_2_chinese, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_3_french, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_4_spanish, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_5_japan, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_6_german, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_05_language_scr->label_7_italian, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //背光亮度界面
    screen_06_brightness_t *screen_06_brightness_scr = screen_06_brightness_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_06_brightness_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_06_brightness_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_06_brightness_scr->container_1, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_06_brightness_scr->label_1_brightness, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);

    //时钟界面
    screen_07_clock_t *screen_07_clock_scr = screen_07_clock_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_07_clock_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_07_clock_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_07_clock_scr->container_1, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_07_clock_scr->container_2, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_07_clock_scr->container_3, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_07_clock_scr->label_1_24_hour, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_2_clock, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_3_am, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_4_pm, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_5_time, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_6_screensaver_time_display, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_7_digital, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->label_8_clock, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变滑块颜色
    lv_obj_set_style_text_color(screen_07_clock_scr->roller_1_12h, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->roller_2_24h, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_07_clock_scr->roller_3_m, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);

    //声音界面
    screen_08_sound_t *screen_08_sound_scr = screen_08_sound_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_08_sound_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_08_sound_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_08_sound_scr->container_1_sound, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_08_sound_scr->label_1_sound, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);

    //F_to_C界面
    screen_09_F_to_C_t *screen_09_F_to_C_scr = screen_09_F_to_C_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_09_F_to_C_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_09_F_to_C_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_09_F_to_C_scr->container_1_F_to_C, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_09_F_to_C_scr->label_1_f_to_c, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_09_F_to_C_scr->label_2_f, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_09_F_to_C_scr->label_3_c, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);

    //版本界面
    screen_10_version_t *screen_10_version_scr = screen_10_version_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_10_version_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(screen_10_version_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器颜色
    lv_obj_set_style_bg_color(screen_10_version_scr->container_1, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变文本颜色
    lv_obj_set_style_text_color(screen_10_version_scr->label_1_version, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_10_version_scr->label_2_version_V, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);

    screen_11_show_all_t *screen_11_show_all_scr = screen_11_show_all_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_11_show_all_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器背景颜色
    lv_obj_set_style_bg_color(screen_11_show_all_scr->container_2_show_all, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    // //修改文字颜色
    // lv_obj_set_style_text_color(screen_11_show_all_scr->label_1_choose_mode, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //按钮x图片
    lv_obj_set_style_bg_img_src(screen_11_show_all_scr->button_1_x, LVGL_IMAGE_PATH(icon_x_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);

    //收藏界面
    screen_14_love_favorites_t *screen_14_love_favorites_scr = screen_14_love_favorites_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_14_love_favorites_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器背景颜色
    lv_obj_set_style_bg_color(screen_14_love_favorites_scr->container_2_favorites, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    // //修改文字颜色
    // lv_obj_set_style_text_color(screen_11_show_all_scr->label_1_choose_mode, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //按钮x图片
    lv_obj_set_style_bg_img_src(screen_14_love_favorites_scr->button_1_x, LVGL_IMAGE_PATH(icon_x_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);

    //恢复出厂设置界面
    screen_15_restore_factory_settings_t *screen_15_restore_factory_settings_scr = screen_15_restore_factory_settings_get(&ui_manager);
    //改变背景颜色
    lv_obj_set_style_bg_color(screen_15_restore_factory_settings_scr->obj, lv_color_hex(YK_COLOR_BG_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //返回按钮图片
    lv_obj_set_style_bg_img_src(screen_15_restore_factory_settings_scr->button_1_back, LVGL_IMAGE_PATH(back_dark.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    //改变容器背景颜色
    lv_obj_set_style_bg_color(screen_15_restore_factory_settings_scr->container_1_restore_factory_settings, lv_color_hex(YK_COLOR_COUNT_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    //修改文字颜色
    lv_obj_set_style_text_color(screen_15_restore_factory_settings_scr->label_1_restore_factory_settings, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
}



//创建执行一次
void screen_04_display_custom_created(void)
{ 
    yk_printf("screen_04_display_custom_created\n");
    scr = screen_04_display_get(&ui_manager);

    // 设置亮色还是暗色的初始状态
    if (yk_settings.display_mode == val1_display_mode_light)
    {
        screen_04_display_container_1_ligth_custom_clicked();
    }
    else
    {
        screen_04_display_container_2_dark_custom_clicked();
    }
}


//每次进入
void screen_04_display_custom_loaded()
{
    yk_printf("screen_04_display_custom_loaded\n");
	yk_v_screen_id = SCREEN_04_ID;
}

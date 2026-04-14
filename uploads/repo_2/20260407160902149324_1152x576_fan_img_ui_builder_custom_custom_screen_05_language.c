#include "custom.h"


static screen_05_language_t *scr = NULL;


//语言切换：选中英文并同步全局语言设置
void screen_05_language_container_1_english_custom_clicked(void)
{
    yk_printf("screen_05_language_container_1_english_custom_clicked\n");
	yk_settings.language = val3_language_en;

    lv_obj_clear_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);

    yk_f_set_language("en");
    yk_delay_save_data_timer_start();
}

//语言切换：选中中文并同步全局语言设置
void screen_05_language_container_2_chinese_custom_clicked(void)
{
    yk_printf("screen_05_language_container_2_chinese_custom_clicked\n");
    yk_settings.language = val3_language_zh;
    
    lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);

	yk_f_set_language("zh");
    yk_delay_save_data_timer_start();
}

//语言切换：选中法语并同步全局语言设置
void screen_05_language_container_3_french_custom_clicked(void)
{
    yk_printf("screen_05_language_container_3_french_custom_clicked\n");
    yk_settings.language = val3_language_fr;
    
    lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);

	yk_f_set_language("fr");
    yk_delay_save_data_timer_start();
}

//语言切换：选中西班牙语并同步全局语言设置
void screen_05_language_container_4_spanish_custom_clicked(void)
{
    yk_printf("screen_05_language_container_4_spanish_custom_clicked\n");
    yk_settings.language = val3_language_sp;
    
    lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);

	yk_f_set_language("sp");
    yk_delay_save_data_timer_start();
}

//语言切换：选中日语并同步全局语言设置
void screen_05_language_container_5_japan_custom_clicked(void)
{
    yk_printf("screen_05_language_container_5_japan_custom_clicked\n");
    yk_settings.language = val3_language_ja;
    
    lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);

	yk_f_set_language("ja");
    yk_delay_save_data_timer_start();
}

//语言切换：选中德语并同步全局语言设置
void screen_05_language_container_6_german_custom_clicked(void)
{
    yk_printf("screen_05_language_container_6_german_custom_clicked\n");
    yk_settings.language = val3_language_de;
    
    lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);

	yk_f_set_language("de");
    yk_delay_save_data_timer_start();
}

//语言切换：选中意大利语并同步全局语言设置
void screen_05_language_container_7_italian_custom_clicked(void)
{
    yk_printf("screen_05_language_container_7_italian_custom_clicked\n");
    yk_settings.language = val3_language_it;
    
    lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);

	yk_f_set_language("it");
    yk_delay_save_data_timer_start();
}



//首次创建只执行一次
void screen_05_language_custom_created(void)
{
    yk_printf("screen_05_language_custom_created\n");
    scr = screen_05_language_get(&ui_manager);

    //根据已保存的语言设置，刷新当前选中态勾选图标
    if (yk_settings.language == val3_language_en)
    {
        lv_obj_clear_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);
    }
    else if (yk_settings.language == val3_language_zh)
    { 
        lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);
    }
    else if (yk_settings.language == val3_language_fr)
    { 
        lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);
    }
    else if (yk_settings.language == val3_language_sp)
    { 
        lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);
    }
    else if (yk_settings.language == val3_language_ja)
    { 
        lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);
    }
    else if (yk_settings.language == val3_language_de)
    { 
        lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);
    }
    else if (yk_settings.language == val3_language_it)
    { 
        lv_obj_add_flag(scr->image_1_english, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_2_chinese, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_3_french, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_4_spanish, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_5_japan, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_6_german, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr->image_7_italian, LV_OBJ_FLAG_HIDDEN);
    }
    
}

#include "custom.h"


static screen_09_F_to_C_t *scr = NULL;

//温度单位切换：更新选中态图标、全局设置并同步相关数据
static void apply_temp_unit(val2_f_to_c unit)
{
    if (unit == val2_t_f) {
        lv_img_set_src(scr->image_1_f, LVGL_IMAGE_PATH(set_time1.png));
        lv_img_set_src(scr->image_2_c, LVGL_IMAGE_PATH(set_time0.png));
    } else {
        lv_img_set_src(scr->image_1_f, LVGL_IMAGE_PATH(set_time0.png));
        lv_img_set_src(scr->image_2_c, LVGL_IMAGE_PATH(set_time1.png));
    }

    yk_settings.f_to_c = unit;
    food_tf_time(yk_v_food_t_f, yk_v_food_min);
    yk_favorites_reload_from_settings();
    yk_delay_save_data_timer_start();
}


//点击华摄氏度
void screen_09_F_to_C_container_2_f_custom_clicked(lv_event_t *e)
{
    apply_temp_unit(val2_t_f);
}

//点击摄氏度
void screen_09_F_to_C_container_3_c_custom_clicked(lv_event_t *e)
{
    apply_temp_unit(val2_t_c);
}



//创建执行一次
void screen_09_F_to_C_custom_created(void)
{ 
    scr = screen_09_F_to_C_get(&ui_manager);

    //首次进入根据已保存的单位，刷新当前选中态
    if (yk_settings.f_to_c == val2_t_f)
    {
        lv_img_set_src(scr->image_1_f, LVGL_IMAGE_PATH(set_time1.png));
        lv_img_set_src(scr->image_2_c, LVGL_IMAGE_PATH(set_time0.png));

        // lv_obj_set_style_text_color(scr->label_2_f, lv_color_hex(0xc4300e), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_text_color(scr->label_3_c, lv_color_hex(0x615f5f), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (yk_settings.f_to_c == val2_t_c)
    {
        lv_img_set_src(scr->image_1_f, LVGL_IMAGE_PATH(set_time0.png));
        lv_img_set_src(scr->image_2_c, LVGL_IMAGE_PATH(set_time1.png));

        // lv_obj_set_style_text_color(scr->label_2_f, lv_color_hex(0x615f5f), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_text_color(scr->label_3_c, lv_color_hex(0xc4300e), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

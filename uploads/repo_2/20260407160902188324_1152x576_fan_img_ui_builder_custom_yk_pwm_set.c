#include "custom.h"


//自定义设置屏幕亮度
void yk_f_set_pwm_brightness(uint8_t pulse)
{
#if defined(_WIN32)
    static lv_obj_t *brightness_mask = NULL;

    if (pulse > 100) pulse = 100;

    if (!brightness_mask) {
        brightness_mask = lv_obj_create(lv_layer_top());
        lv_obj_set_pos(brightness_mask, 0, 0);
        lv_obj_set_style_bg_color(brightness_mask, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(brightness_mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(brightness_mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(brightness_mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(brightness_mask, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_clear_flag(brightness_mask, LV_OBJ_FLAG_CLICKABLE);
    }

    lv_display_t *disp = lv_display_get_default();
    if (disp) {
        lv_obj_set_size(
            brightness_mask,
            lv_display_get_horizontal_resolution(disp),
            lv_display_get_vertical_resolution(disp)
        );
    } else {
        // f:\AiUlBuilder\LVGL\1152x576_fan_img\ui_builder\custom - 避免硬编码分辨率，按当前显示分辨率兜底
        lv_obj_set_size(brightness_mask, yk_ui_screen_w(), yk_ui_screen_h());
    }

    uint8_t opa = (uint8_t)(((100 - pulse) * 220) / 100);
    lv_obj_set_style_bg_opa(brightness_mask, opa, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (opa == 0) lv_obj_add_flag(brightness_mask, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_clear_flag(brightness_mask, LV_OBJ_FLAG_HIDDEN);
#elif YK_USE_UI_BUILDER
    yk_fd_set_pwm_brightness(pulse);
#else
    (void)pulse;
#endif
}

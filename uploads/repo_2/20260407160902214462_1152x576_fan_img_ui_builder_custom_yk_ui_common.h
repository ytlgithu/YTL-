#ifndef YK_UI_COMMON_H
#define YK_UI_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui_objects.h"

// f:\AiUlBuilder\LVGL\1152x576_fan_img\ui_builder\custom - 统一获取当前显示分辨率
static inline lv_coord_t yk_ui_screen_w(void)
{
#if LVGL_VERSION_MAJOR == 8
    return lv_disp_get_hor_res(NULL);
#else
    return (lv_coord_t)lv_display_get_horizontal_resolution(NULL);
#endif
}

// f:\AiUlBuilder\LVGL\1152x576_fan_img\ui_builder\custom - 统一获取当前显示分辨率
static inline lv_coord_t yk_ui_screen_h(void)
{
#if LVGL_VERSION_MAJOR == 8
    return lv_disp_get_ver_res(NULL);
#else
    return (lv_coord_t)lv_display_get_vertical_resolution(NULL);
#endif
}

// 关闭遮罩对象（若对象存在则删除，并将指针置空）
void yk_ui_mask_close(lv_obj_t **mask_obj);

// 多语言取词：优先查 key 对应翻译；为空时返回 fallback
const char *yk_ui_tr(const char *key, const char *fallback);

// 关闭居中 Toast（若存在）
void yk_ui_center_toast_dismiss(void);

// 显示居中 Toast：txt 为提示文本，ms 为显示时长（毫秒）
void yk_ui_center_toast_show(const char *txt, uint32_t ms);

// 关闭确认弹窗（若存在）
void yk_ui_confirm_dismiss(void);

// 显示确认弹窗：message 为内容；panel_w/panel_h 为面板尺寸；
// cancel_text/ok_text 为按钮文案；cancel_cb/ok_cb 为对应点击回调
void yk_ui_confirm_show(const char *message,
                        int panel_w,
                        int panel_h,
                        const char *cancel_text,
                        const char *ok_text,
                        lv_event_cb_t cancel_cb,
                        lv_event_cb_t ok_cb);

// f:\AiUlBuilder\LVGL\1152x576_fan_img\ui_builder\custom - 弹框按钮统一去阴影（默认/按下/聚焦）
void yk_ui_btn_disable_shadow(lv_obj_t *btn);

void yk_ui_keyboard_apply_theme(lv_obj_t *kb, bool dark);

lv_obj_t *yk_ui_keyboard_create(lv_obj_t *parent,
                                lv_obj_t *ta,
                                lv_coord_t w,
                                lv_coord_t h,
                                bool dark);

void yk_ui_textarea_apply_cursor_theme(lv_obj_t *ta, bool dark);

typedef bool (*yk_ui_text_input_ok_cb_t)(lv_obj_t *ta, void *user_data);
typedef void (*yk_ui_text_input_cancel_cb_t)(void *user_data);
typedef void (*yk_ui_text_input_close_cb_t)(void *user_data);

// 通用“文本输入弹窗”（遮罩 + 面板 + 标题 + 输入框 + 取消/确定 + 可选键盘）
//
// 使用方式：
// 1) 直接调用 yk_ui_text_input_show(...) 弹出
// 2) ok_cb 返回 true 表示允许关闭弹窗；返回 false 表示保留弹窗（用于业务校验失败提示）
// 3) cancel_cb 在点击“取消”时触发；close_cb 在遮罩对象被删除时触发（无论取消/确定/外部删除）
//
// 参数说明：
// - parent: 弹窗父对象（通常传当前 screen 的根对象）
// - title_text/cancel_text/ok_text: 标题与按钮文字
// - panel_w/panel_h/panel_y: 面板尺寸与 Y 偏移（水平居中，顶对齐到 panel_y）
// - ta_w/ta_h: 输入框尺寸（相对面板）
// - btn_w/btn_h/btn_y/btn_x: 两个按钮的尺寸与位置（以面板底部为参考，左右用 +/-btn_x 偏移）
// - one_line: true=单行输入；false=多行输入
// - max_len: 最大输入长度（0 表示不限制）
// - initial_text: 初始文本（NULL 视为 ""）
// - kb_h: 软键盘高度（<=0 表示不创建键盘）
// - dark: true=暗色模式；false=亮色模式
// - cancel_cb/ok_cb/close_cb/user_data: 业务回调与用户数据
//
// 输出对象指针（可传 NULL）：
// - out_ta/out_kb/out_title/out_lbl_cancel/out_lbl_ok: 返回内部创建出来的对象指针，便于外部进一步设置
lv_obj_t *yk_ui_text_input_show(lv_obj_t *parent,
                               const char *title_text,
                               const char *cancel_text,
                               const char *ok_text,
                               lv_coord_t panel_w,
                               lv_coord_t panel_h,
                               lv_coord_t panel_y,
                               lv_coord_t ta_w,
                               lv_coord_t ta_h,
                               lv_coord_t btn_w,
                               lv_coord_t btn_h,
                               lv_coord_t btn_y,
                               lv_coord_t btn_x,
                               bool one_line,
                               uint32_t max_len,
                               const char *initial_text,
                               lv_coord_t kb_h,
                               bool dark,
                               yk_ui_text_input_cancel_cb_t cancel_cb,
                               yk_ui_text_input_ok_cb_t ok_cb,
                               yk_ui_text_input_close_cb_t close_cb,
                               void *user_data,
                               lv_obj_t **out_ta,
                               lv_obj_t **out_kb,
                               lv_obj_t **out_title,
                               lv_obj_t **out_lbl_cancel,
                               lv_obj_t **out_lbl_ok);

#ifdef __cplusplus
}
#endif

#endif

#include "custom.h"

const char *yk_ui_tr(const char *key, const char *fallback)
{
    const char *t = yk_f_get_translation(key);
    if (t && t[0] != '\0') return t;
    return fallback;
}

// f:\AiUlBuilder\LVGL\1152x576_fan_img\ui_builder\custom - 弹框按钮统一去阴影（默认/按下/聚焦）
void yk_ui_btn_disable_shadow(lv_obj_t *btn)
{
    if (!btn) return;
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
}

void yk_ui_keyboard_apply_theme(lv_obj_t *kb, bool dark)
{
    if (!kb) return;

    lv_obj_set_style_bg_color(kb, lv_color_hex(dark ? YK_COLOR_BG_DARK : YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(kb, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(kb, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(kb, lv_color_hex(dark ? 0x3F3F3F : 0xFFFFFF), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(kb, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(kb, &lv_font_montserrat_26, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(kb, lv_color_hex(dark ? 0xFFFFFF : 0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(kb, dark ? 0 : 1, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(kb, lv_color_hex(0xC8C8C8), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(kb, 8, LV_PART_ITEMS | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(kb, lv_color_hex(dark ? 0x505050 : 0xE0E0E0), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(kb, &lv_font_montserrat_26, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(kb, lv_color_hex(dark ? 0xFFFFFF : 0x000000), LV_PART_ITEMS | LV_STATE_PRESSED);
}

lv_obj_t *yk_ui_keyboard_create(lv_obj_t *parent,
                                lv_obj_t *ta,
                                lv_coord_t w,
                                lv_coord_t h,
                                bool dark)
{
    if (!parent) return NULL;
    lv_obj_t *kb = lv_keyboard_create(parent);
    if (!kb) return NULL;
    if (ta) lv_keyboard_set_textarea(kb, ta);
    lv_obj_set_size(kb, w, h);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    yk_ui_keyboard_apply_theme(kb, dark);
    return kb;
}

typedef struct {
    lv_obj_t *mask;
    lv_obj_t *ta;
    yk_ui_text_input_cancel_cb_t cancel_cb;
    yk_ui_text_input_ok_cb_t ok_cb;
    yk_ui_text_input_close_cb_t close_cb;
    void *user_data;
} yk_ui_text_input_wrap_t;

static void yk_ui_text_input_on_delete(lv_event_t *e)
{
    // 1) 取出包装结构体（里面保存了回调与 user_data）
    yk_ui_text_input_wrap_t *w = (yk_ui_text_input_wrap_t *)lv_event_get_user_data(e);
    if (!w) return;
    // 2) 通知外部弹窗已关闭（无论是取消/确定/外部删除，都会走到这里）
    if (w->close_cb) {
        w->close_cb(w->user_data);
    }
    // 3) 释放包装结构体
    lv_mem_free(w);
}

static void yk_ui_text_input_cancel_clicked(lv_event_t *e)
{
    // 1) 取出包装结构体
    yk_ui_text_input_wrap_t *w = (yk_ui_text_input_wrap_t *)lv_event_get_user_data(e);
    if (!w) return;
    // 2) 播放触摸音
    yk_f_touch_sound();
    // 3) 转发取消回调（业务层可做状态复位等）
    if (w->cancel_cb) {
        w->cancel_cb(w->user_data);
    }
    // 4) 删除遮罩（删除后会触发遮罩的 LV_EVENT_DELETE，从而走 yk_ui_text_input_on_delete）
    if (w->mask && lv_obj_is_valid(w->mask)) {
        lv_obj_del(w->mask);
    }
}

static void yk_ui_text_input_ok_clicked(lv_event_t *e)
{
    // 1) 取出包装结构体
    yk_ui_text_input_wrap_t *w = (yk_ui_text_input_wrap_t *)lv_event_get_user_data(e);
    if (!w) return;
    // 2) 播放触摸音
    yk_f_touch_sound();
    // 3) 先默认允许关闭；若业务回调返回 false，则保留弹窗（用于提示输入不合法等）
    bool should_close = true;
    if (w->ok_cb) {
        should_close = w->ok_cb(w->ta, w->user_data);
    }
    // 4) 允许关闭时删除遮罩（删除后会触发 yk_ui_text_input_on_delete）
    if (should_close && w->mask && lv_obj_is_valid(w->mask)) {
        lv_obj_del(w->mask);
    }
}

void yk_ui_textarea_apply_cursor_theme(lv_obj_t *ta, bool dark)
{
    if (!ta) return;
    lv_color_t cursor_color = lv_color_hex(dark ? 0xFFFFFF : 0x000000);

    const lv_state_t states[] = {
        LV_STATE_DEFAULT,
        LV_STATE_FOCUSED,
        LV_STATE_EDITED,
        LV_STATE_FOCUSED | LV_STATE_EDITED,
        LV_STATE_FOCUS_KEY,
        LV_STATE_FOCUS_KEY | LV_STATE_FOCUSED,
        LV_STATE_FOCUS_KEY | LV_STATE_EDITED,
        LV_STATE_FOCUS_KEY | LV_STATE_FOCUSED | LV_STATE_EDITED,
        LV_STATE_PRESSED,
        LV_STATE_PRESSED | LV_STATE_FOCUSED,
        LV_STATE_PRESSED | LV_STATE_EDITED,
        LV_STATE_PRESSED | LV_STATE_FOCUSED | LV_STATE_EDITED,
        LV_STATE_PRESSED | LV_STATE_FOCUS_KEY,
        LV_STATE_PRESSED | LV_STATE_FOCUS_KEY | LV_STATE_FOCUSED,
        LV_STATE_PRESSED | LV_STATE_FOCUS_KEY | LV_STATE_EDITED,
        LV_STATE_PRESSED | LV_STATE_FOCUS_KEY | LV_STATE_FOCUSED | LV_STATE_EDITED,
    };

    for (uint32_t i = 0; i < sizeof(states) / sizeof(states[0]); i++) {
        lv_state_t st = states[i];
        lv_obj_set_style_bg_opa(ta, LV_OPA_TRANSP, LV_PART_CURSOR | st);
        lv_obj_set_style_border_width(ta, 2, LV_PART_CURSOR | st);
        lv_obj_set_style_border_color(ta, cursor_color, LV_PART_CURSOR | st);
        lv_obj_set_style_border_opa(ta, LV_OPA_COVER, LV_PART_CURSOR | st);
        lv_obj_set_style_border_side(ta, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR | st);
        lv_obj_set_style_radius(ta, 0, LV_PART_CURSOR | st);
    }
}

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
                               lv_obj_t **out_lbl_ok)
{
    // 0) 基础保护：没有 parent 就无法创建弹窗
    if (!parent) return NULL;

    // 1) 根据主题准备本弹窗配色（只影响弹窗自身）
    uint32_t panel_bg = dark ? YK_COLOR_COUNT_DARK : YK_COLOR_COUNT_LIGHT;
    uint32_t txt_color = dark ? YK_COLOR_LBL_DARK : YK_COLOR_LBL_LIGHT;
    uint32_t cancel_bg = dark ? YK_COLOR_BG_DARK : 0xE0E0E0;

    // 2) 创建全屏遮罩：阻止点击穿透 + 给出暗底效果
    lv_obj_t *mask = lv_obj_create(parent);
    lv_obj_set_pos(mask, 0, 0);
    lv_obj_set_size(mask, yk_ui_screen_w(), yk_ui_screen_h());
    lv_obj_set_style_bg_color(mask, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(mask, dark ? 160 : 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(mask, LV_OBJ_FLAG_SCROLLABLE);

    // 3) 创建弹窗面板：承载标题、输入框、按钮等
    lv_obj_t *panel = lv_obj_create(mask);
    lv_obj_set_size(panel, panel_w, panel_h);
    lv_obj_set_style_bg_color(panel, lv_color_hex(panel_bg), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(panel, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(panel, 18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(panel, LV_ALIGN_TOP_MID, 0, panel_y);

    // 4) 标题：居中显示
    lv_obj_t *title = lv_label_create(panel);
    lv_label_set_text(title, title_text ? title_text : "");
    lv_obj_set_style_text_color(title, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 6);

    // 5) 输入框：设置尺寸/单多行/长度限制/初始文本/主题样式/光标样式
    lv_obj_t *ta = lv_textarea_create(panel);
    lv_obj_set_size(ta, ta_w, ta_h);
    lv_textarea_set_one_line(ta, one_line);
    if (max_len > 0) lv_textarea_set_max_length(ta, max_len);
    lv_textarea_set_text(ta, initial_text ? initial_text : "");
    lv_obj_set_style_text_font(ta, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ta, lv_color_hex(dark ? 0x1F1F1F : YK_COLOR_BG_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ta, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ta, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    yk_ui_textarea_apply_cursor_theme(ta, dark);
    lv_obj_set_style_radius(ta, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ta, dark ? 0 : 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ta, lv_color_hex(0xC8C8C8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ta, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ta, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_coord_t pad_tb = one_line ? 8 : 12;
    lv_obj_set_style_pad_top(ta, pad_tb, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ta, pad_tb, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 56);

    // 6) 取消按钮：绑定文字、样式与位置（事件在最后统一绑定）
    lv_obj_t *btn_cancel = lv_btn_create(panel);
    lv_obj_set_size(btn_cancel, btn_w, btn_h);
    lv_obj_set_style_radius(btn_cancel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn_cancel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn_cancel, lv_color_hex(cancel_bg), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn_cancel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    yk_ui_btn_disable_shadow(btn_cancel);
    lv_obj_align(btn_cancel, LV_ALIGN_BOTTOM_MID, -btn_x, btn_y);

    lv_obj_t *lbl_cancel = lv_label_create(btn_cancel);
    lv_label_set_text(lbl_cancel, cancel_text ? cancel_text : "");
    lv_obj_set_style_text_color(lbl_cancel, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl_cancel, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(lbl_cancel);

    // 7) 确定按钮：固定红底白字（事件在最后统一绑定）
    lv_obj_t *btn_ok = lv_btn_create(panel);
    lv_obj_set_size(btn_ok, btn_w, btn_h);
    lv_obj_set_style_radius(btn_ok, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn_ok, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn_ok, lv_color_hex(0xC4300E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn_ok, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    yk_ui_btn_disable_shadow(btn_ok);
    lv_obj_align(btn_ok, LV_ALIGN_BOTTOM_MID, btn_x, btn_y);

    lv_obj_t *lbl_ok = lv_label_create(btn_ok);
    lv_label_set_text(lbl_ok, ok_text ? ok_text : "");
    lv_obj_set_style_text_color(lbl_ok, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl_ok, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(lbl_ok);

    // 8) 可选创建软键盘：需要 kb_h > 0 才创建，并与输入框绑定
    lv_obj_t *kb = NULL;
    if (kb_h > 0) {
        kb = yk_ui_keyboard_create(mask, ta, yk_ui_screen_w(), kb_h, dark);
        if (kb) {
            lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_TEXT_LOWER);
        }
    }

    // 9) 创建包装结构体：用于把按钮点击与“关闭/取消/确定回调”统一管理
    yk_ui_text_input_wrap_t *w = (yk_ui_text_input_wrap_t *)lv_mem_alloc(sizeof(yk_ui_text_input_wrap_t));
    if (w) {
        w->mask = mask;
        w->ta = ta;
        w->cancel_cb = cancel_cb;
        w->ok_cb = ok_cb;
        w->close_cb = close_cb;
        w->user_data = user_data;
        // 10) 绑定事件：
        // - 遮罩删除 -> close_cb + 释放包装结构体
        // - 取消按钮 -> cancel_cb + 删除遮罩
        // - 确定按钮 -> ok_cb(决定是否关弹窗) + 按需删除遮罩
        lv_obj_add_event_cb(mask, yk_ui_text_input_on_delete, LV_EVENT_DELETE, w);
        lv_obj_add_event_cb(btn_cancel, yk_ui_text_input_cancel_clicked, LV_EVENT_CLICKED, w);
        lv_obj_add_event_cb(btn_ok, yk_ui_text_input_ok_clicked, LV_EVENT_CLICKED, w);
    }

    // 11) 回传关键对象指针（外部可按需继续设置）
    if (out_ta) *out_ta = ta;
    if (out_kb) *out_kb = kb;
    if (out_title) *out_title = title;
    if (out_lbl_cancel) *out_lbl_cancel = lbl_cancel;
    if (out_lbl_ok) *out_lbl_ok = lbl_ok;
    // 12) 返回遮罩对象句柄（外部也可通过 yk_ui_mask_close 关闭）
    return mask;
}

// 关闭遮罩层：删除对象并清空句柄
void yk_ui_mask_close(lv_obj_t **mask_obj)
{
    if (!mask_obj || !*mask_obj) return;
    lv_obj_del(*mask_obj);
    *mask_obj = NULL;
}

static lv_timer_t *yk_center_toast_timer = NULL;
static lv_obj_t *yk_center_toast_obj = NULL;
static lv_obj_t *yk_center_toast_label = NULL;

static void yk_ui_center_toast_on_delete(lv_event_t *e)
{
    (void)e;
    yk_center_toast_obj = NULL;
    yk_center_toast_label = NULL;
    if (yk_center_toast_timer) {
        lv_timer_del(yk_center_toast_timer);
        yk_center_toast_timer = NULL;
    }
}

static void yk_ui_center_toast_timer_cb(lv_timer_t *timer)
{
    if (yk_center_toast_obj) {
        lv_obj_add_flag(yk_center_toast_obj, LV_OBJ_FLAG_HIDDEN);
    }
    if (yk_center_toast_timer == timer) {
        yk_center_toast_timer = NULL;
    }
    lv_timer_del(timer);
}

void yk_ui_center_toast_dismiss(void)
{
    if (yk_center_toast_timer) {
        lv_timer_del(yk_center_toast_timer);
        yk_center_toast_timer = NULL;
    }
    if (yk_center_toast_obj && lv_obj_is_valid(yk_center_toast_obj)) {
        lv_obj_del(yk_center_toast_obj);
    }
    yk_center_toast_obj = NULL;
    yk_center_toast_label = NULL;
}

void yk_ui_center_toast_show(const char *txt, uint32_t ms)
{
    lv_obj_t *parent = lv_scr_act();
    if (!parent) return;

    if (yk_center_toast_obj && !lv_obj_is_valid(yk_center_toast_obj)) {
        yk_center_toast_obj = NULL;
        yk_center_toast_label = NULL;
    }
    if (yk_center_toast_label && !lv_obj_is_valid(yk_center_toast_label)) {
        yk_center_toast_label = NULL;
    }

    if (yk_center_toast_obj && lv_obj_get_parent(yk_center_toast_obj) != parent) {
        yk_ui_center_toast_dismiss();
    }

    if (!yk_center_toast_obj) {
        yk_center_toast_obj = lv_obj_create(parent);
        lv_obj_add_event_cb(yk_center_toast_obj, yk_ui_center_toast_on_delete, LV_EVENT_DELETE, NULL);
        lv_obj_set_width(yk_center_toast_obj, 420);
        lv_obj_set_height(yk_center_toast_obj, LV_SIZE_CONTENT);
        lv_obj_set_style_radius(yk_center_toast_obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(yk_center_toast_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(yk_center_toast_obj, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(yk_center_toast_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(yk_center_toast_obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(yk_center_toast_obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(yk_center_toast_obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(yk_center_toast_obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(yk_center_toast_obj, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(yk_center_toast_obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_center(yk_center_toast_obj);

        yk_center_toast_label = lv_label_create(yk_center_toast_obj);
        lv_obj_set_width(yk_center_toast_label, 420 - 24);
        lv_label_set_long_mode(yk_center_toast_label, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_color(yk_center_toast_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(yk_center_toast_label, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(yk_center_toast_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_center(yk_center_toast_label);
    }

    if (txt && yk_center_toast_label) {
        lv_label_set_text(yk_center_toast_label, txt);
    }
    if (yk_center_toast_label) {
        lv_obj_center(yk_center_toast_label);
    }
    lv_obj_center(yk_center_toast_obj);
    lv_obj_clear_flag(yk_center_toast_obj, LV_OBJ_FLAG_HIDDEN);

    if (yk_center_toast_timer) {
        lv_timer_del(yk_center_toast_timer);
        yk_center_toast_timer = NULL;
    }
    yk_center_toast_timer = lv_timer_create(yk_ui_center_toast_timer_cb, ms, NULL);
}

typedef struct {
    lv_event_cb_t cb;
    void *user_data;
} yk_ui_event_wrap_t;

static lv_obj_t *yk_confirm_mask = NULL;

void yk_ui_confirm_dismiss(void)
{
    if (yk_confirm_mask && lv_obj_is_valid(yk_confirm_mask)) {
        lv_obj_del(yk_confirm_mask);
    }
    yk_confirm_mask = NULL;
}

 

// 确认弹窗事件代理：
// 1) 先关闭弹窗（避免回调里再次触发弹窗时叠加）
// 2) 再把事件转发给业务回调
// 3) 最后释放包装结构体，避免内存泄漏
static void yk_ui_confirm_event_proxy(lv_event_t *e)
{
    yk_ui_event_wrap_t *w = (yk_ui_event_wrap_t *)lv_event_get_user_data(e);
    if (!w) return;
    (void)w->user_data;
    yk_f_touch_sound();
    yk_ui_confirm_dismiss();
    if (w->cb) {
        w->cb(e);
    }
    lv_mem_free(w);
}

// 显示确认弹窗：在当前页面创建遮罩+面板，并绑定取消/确认回调
void yk_ui_confirm_show(const char *message,
                        int panel_w,
                        int panel_h,
                        const char *cancel_text,
                        const char *ok_text,
                        lv_event_cb_t cancel_cb,
                        lv_event_cb_t ok_cb)
{
    // 以当前活动页面作为父对象，弹窗会跟随页面切换销毁
    lv_obj_t *parent = lv_scr_act();
    if (!parent) return;

    // 若已有确认弹窗，先关闭，保证同一时刻只有一个确认弹窗
    if (yk_confirm_mask) {
        yk_ui_confirm_dismiss();
    }

    // 根据深色/浅色模式准备配色（只影响本弹窗，不改全局主题）
    bool dark = (yk_settings.display_mode == val1_display_mode_dark);
    uint32_t panel_bg = dark ? YK_COLOR_COUNT_DARK : YK_COLOR_COUNT_LIGHT;
    uint32_t txt_color = dark ? YK_COLOR_LBL_DARK : YK_COLOR_LBL_LIGHT;
    uint32_t cancel_bg = dark ? YK_COLOR_BG_DARK : 0xD0D0D0;

    // 1) 创建全屏遮罩：阻止点击穿透，并给出暗底效果
    yk_confirm_mask = lv_obj_create(parent);
    lv_obj_set_pos(yk_confirm_mask, 0, 0);
    // f:\AiUlBuilder\LVGL\1152x576_fan_img\ui_builder\custom - 避免硬编码分辨率，按当前显示分辨率创建遮罩
    lv_obj_set_size(yk_confirm_mask, yk_ui_screen_w(), yk_ui_screen_h());
    lv_obj_set_style_bg_color(yk_confirm_mask, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(yk_confirm_mask, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(yk_confirm_mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(yk_confirm_mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(yk_confirm_mask, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(yk_confirm_mask, LV_OBJ_FLAG_SCROLLABLE);

    // 2) 创建面板：承载文本与按钮
    lv_obj_t *panel = lv_obj_create(yk_confirm_mask);
    lv_obj_set_size(panel, panel_w, panel_h);
    lv_obj_set_style_bg_color(panel, lv_color_hex(panel_bg), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(panel, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(panel, 18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_center(panel);

    // 3) 文本：长文本换行，水平居中；垂直位置在按钮上方区域居中（按实际按钮位置计算）
    lv_obj_t *lbl = lv_label_create(panel);
    lv_label_set_text(lbl, message ? message : "");
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(lbl, panel_w - 60);
    lv_obj_set_style_text_color(lbl, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl, fs_montserratmedium_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 4) 取消按钮：绑定事件代理（自动关弹窗）+ 业务回调
    lv_obj_t *btn_cancel = lv_btn_create(panel);
    lv_obj_set_size(btn_cancel, 180, 60);
    lv_obj_set_style_radius(btn_cancel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn_cancel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn_cancel, lv_color_hex(cancel_bg), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn_cancel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    yk_ui_btn_disable_shadow(btn_cancel);
    lv_obj_align(btn_cancel, LV_ALIGN_BOTTOM_MID, -120, -30);

    yk_ui_event_wrap_t *w_cancel = (yk_ui_event_wrap_t *)lv_mem_alloc(sizeof(yk_ui_event_wrap_t));
    if (w_cancel) {
        w_cancel->cb = cancel_cb;
        w_cancel->user_data = NULL;
        lv_obj_add_event_cb(btn_cancel, yk_ui_confirm_event_proxy, LV_EVENT_CLICKED, w_cancel);
    }

    lv_obj_t *lbl_cancel = lv_label_create(btn_cancel);
    lv_label_set_text(lbl_cancel, cancel_text ? cancel_text : "");
    lv_obj_set_style_text_color(lbl_cancel, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl_cancel, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(lbl_cancel);

    // 5) 确认按钮：同样通过事件代理先关闭弹窗，再执行回调
    lv_obj_t *btn_ok = lv_btn_create(panel);
    lv_obj_set_size(btn_ok, 180, 60);
    lv_obj_set_style_radius(btn_ok, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn_ok, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn_ok, lv_color_hex(0xC4300E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn_ok, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    yk_ui_btn_disable_shadow(btn_ok);
    lv_obj_align(btn_ok, LV_ALIGN_BOTTOM_MID, 120, -30);

    yk_ui_event_wrap_t *w_ok = (yk_ui_event_wrap_t *)lv_mem_alloc(sizeof(yk_ui_event_wrap_t));
    if (w_ok) {
        w_ok->cb = ok_cb;
        w_ok->user_data = NULL;
        lv_obj_add_event_cb(btn_ok, yk_ui_confirm_event_proxy, LV_EVENT_CLICKED, w_ok);
    }

    lv_obj_t *lbl_ok = lv_label_create(btn_ok);
    lv_label_set_text(lbl_ok, ok_text ? ok_text : "");
    lv_obj_set_style_text_color(lbl_ok, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl_ok, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(lbl_ok);

    lv_obj_update_layout(panel);
    lv_obj_update_layout(lbl);

    lv_coord_t region_top = 0;
    lv_coord_t region_bottom = lv_obj_get_y(btn_cancel);
    lv_coord_t region_h = region_bottom - region_top;
    lv_coord_t lbl_h = lv_obj_get_height(lbl);
    lv_coord_t y = region_top;
    if (region_h > lbl_h) {
        y = (lv_coord_t)(region_top + (region_h - lbl_h) / 2);
    }
    lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, y);
}

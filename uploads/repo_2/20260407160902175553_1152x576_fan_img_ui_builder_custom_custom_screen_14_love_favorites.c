#include "custom.h"

#include <string.h>

extern const char* yk_get_image_path_by_flag(uint8_t flag);
extern const char* yk_get_favorite_image_path_by_flag(uint8_t flag);

static screen_14_love_favorites_t *scr = NULL;

void yk_favorites_reload_from_settings(void);
uint16_t adjust_to_nearest_0_or_5(uint16_t value);

#define FAVORITE_ROW_UD_MAGIC 0x30564146u
#define FAVORITES_LEFT_GROUP_SHIFT 20

// 收藏列表管理（在收藏界面实现）
#define MAX_FAVORITES 32
static lv_obj_t* favorite_items[MAX_FAVORITES];
static int favorite_count = 0;

static uint32_t favorites_new_flags[MAX_FAVORITES];
static int favorites_new_flag_cnt = 0;

// 收藏行用户数据结构：图片源 + 标记
typedef struct {
    uint32_t magic;
    const void *img_src;
    uint32_t flag;
    lv_obj_t *img_arrow_l;
    lv_obj_t *img_arrow_r;
    lv_obj_t *click_area;
    lv_obj_t *row_click;
    lv_obj_t *lbl_name;
    lv_obj_t *btn_del;
    lv_obj_t *img_del;
    lv_obj_t *sld_temp;
    lv_obj_t *sld_time;
    lv_obj_t *dot_new;
} favorite_row_data_t;


static bool favorites_edit_mode = false;
static lv_obj_t *favorites_bound_screen_obj = NULL;

static lv_obj_t *favorites_rename_mask = NULL;
static lv_obj_t *favorites_rename_ta = NULL;
static lv_obj_t *favorites_rename_kb = NULL;
static lv_obj_t *favorites_rename_title = NULL;
static lv_obj_t *favorites_rename_lbl_cancel = NULL;
static lv_obj_t *favorites_rename_lbl_ok = NULL;
static lv_obj_t *favorites_rename_target_label = NULL;
static uint32_t favorites_rename_target_flag = 0;
static bool favorites_ignore_next_thumb_click = false;
static lv_obj_t *favorites_pending_delete_row = NULL;
static uint32_t favorites_pending_delete_flag = 0;

static void favorites_apply_edit_mode(bool edit_mode);
static void favorites_open_row(lv_obj_t *row);
static void favorites_settings_insert_front(const yk_user_favorite *item);

static void favorites_new_flags_clear(void)
{
    favorites_new_flag_cnt = 0;
    memset(favorites_new_flags, 0, sizeof(favorites_new_flags));
}

static bool favorites_new_flag_has(uint32_t flag)
{
    for (int i = 0; i < favorites_new_flag_cnt; i++) {
        if (favorites_new_flags[i] == flag) return true;
    }
    return false;
}

static void favorites_new_flag_add(uint32_t flag)
{
    if (favorites_new_flag_has(flag)) return;
    if (favorites_new_flag_cnt >= MAX_FAVORITES) return;
    favorites_new_flags[favorites_new_flag_cnt++] = flag;
}

static void favorites_new_flag_remove(uint32_t flag)
{
    for (int i = 0; i < favorites_new_flag_cnt; i++) {
        if (favorites_new_flags[i] == flag) {
            for (int j = i; j < favorites_new_flag_cnt - 1; j++) {
                favorites_new_flags[j] = favorites_new_flags[j + 1];
            }
            favorites_new_flag_cnt--;
            favorites_new_flags[favorites_new_flag_cnt] = 0;
            return;
        }
    }
}

static lv_obj_t *favorites_create_new_dot(lv_obj_t *parent)
{
    lv_obj_t *dot = lv_obj_create(parent);
    lv_obj_set_size(dot, 14, 14);
    lv_obj_set_style_radius(dot, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dot, lv_color_hex(0xC4300E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dot, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(dot, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(dot, LV_OBJ_FLAG_HIDDEN);
    return dot;
}

static inline bool favorites_is_dark(void)
{
    return (yk_settings.display_mode == val1_display_mode_dark);
}

static inline uint32_t favorites_text_color_hex(void)
{
    return favorites_is_dark() ? YK_COLOR_LBL_DARK : YK_COLOR_LBL_LIGHT;
}

static inline uint32_t favorites_slider_track_color_hex(void)
{
    return favorites_is_dark() ? 0x4a4a4a : 0xE7E7EE;
}

static inline uint32_t favorites_slider_track_disabled_color_hex(void)
{
    return favorites_is_dark() ? 0x3a3a3a : 0xE7E7EE;
}

static inline uint32_t favorites_slider_indicator_disabled_color_hex(void)
{
    return favorites_is_dark() ? 0x7a7a7a : 0xB6B6B6;
}

static inline lv_coord_t favorites_slider_radius(void)
{
    return 10;
}

static inline const char *favorites_icon_x_path(void)
{
    return favorites_is_dark() ? LVGL_IMAGE_PATH(icon_x_dark.png) : LVGL_IMAGE_PATH(icon_x.png);
}

static inline const char *favorites_icon_add_path(void)
{
    return favorites_is_dark() ? LVGL_IMAGE_PATH(icon_add_dark.png) : LVGL_IMAGE_PATH(icon_add.png);
}

static inline const char *favorites_icon_edit01_path(void)
{
    return favorites_is_dark() ? LVGL_IMAGE_PATH(icon_edit_dark01.png) : LVGL_IMAGE_PATH(icon_edit01.png);
}

static inline const char *favorites_icon_edit02_path(void)
{
    return favorites_is_dark() ? LVGL_IMAGE_PATH(icon_edit_dark02.png) : LVGL_IMAGE_PATH(icon_edit02.png);
}

static inline const char *favorites_icon_del_path(void)
{
    return favorites_is_dark() ? LVGL_IMAGE_PATH(icon_del_dark.png) : LVGL_IMAGE_PATH(icon_del.png);
}

static void favorites_show_toast(const char *txt, uint32_t ms)
{
    if (!scr) scr = screen_14_love_favorites_get(&ui_manager);
    if (!scr || !scr->obj) return;
    yk_ui_center_toast_show(txt, ms);
}

static int favorites_find_favorite_slot_by_flag(uint32_t flag)
{
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        if (yk_settings.user_favorites[i].is_favorite &&
            yk_settings.user_favorites[i].flag == flag) {
            return i;
        }
    }
    return -1;
}

void yk_favorites_update_ui_text(void)
{
    //收藏
    if (scr && scr->label_0_favorites) {
        lv_label_set_text(scr->label_0_favorites, yk_ui_tr("favorites", "收藏"));
    }
    if (favorites_rename_title) {
        lv_label_set_text(favorites_rename_title, yk_ui_tr("favorites_rename_title", "编辑名称"));
    }
    if (favorites_rename_lbl_cancel) {
        lv_label_set_text(favorites_rename_lbl_cancel, yk_ui_tr("cancel", "取消"));
    }
    if (favorites_rename_lbl_ok) {
        lv_label_set_text(favorites_rename_lbl_ok, yk_ui_tr("save", "保存"));
    }
}

static void favorites_rename_close(void)
{
    favorites_rename_mask = NULL;
    favorites_ignore_next_thumb_click = false;
    favorites_rename_ta = NULL;
    favorites_rename_kb = NULL;
    favorites_rename_title = NULL;
    favorites_rename_lbl_cancel = NULL;
    favorites_rename_lbl_ok = NULL;
    favorites_rename_target_label = NULL;
    favorites_rename_target_flag = 0;
}

static void favorites_rename_close_cb(void *user_data)
{
    (void)user_data;
    favorites_rename_close();
}

static bool favorites_rename_ok_cb(lv_obj_t *ta, void *user_data)
{
    (void)user_data;
    if (!ta) return true;

    const char *txt = lv_textarea_get_text(ta);
    if (!txt || txt[0] == '\0') {
        favorites_show_toast(yk_ui_tr("favorites_name_empty", "名称不能为空"), 2000);
        return false;
    }

    int slot = favorites_find_favorite_slot_by_flag(favorites_rename_target_flag);
    if (slot < 0) {
        return true;
    }

    strncpy(yk_settings.user_favorites[slot].name, txt, sizeof(yk_settings.user_favorites[slot].name) - 1);
    yk_settings.user_favorites[slot].name[sizeof(yk_settings.user_favorites[slot].name) - 1] = '\0';
    yk_delay_save_data_timer_start();

    if (favorites_rename_target_label) {
        lv_label_set_text(favorites_rename_target_label, yk_settings.user_favorites[slot].name);
    }
    return true;
}

static void favorites_rename_name_clicked(lv_event_t *e)
{
    // 1) 获取点击对象与目标 label（label 通过 user_data 传入；为空时兜底用 click_obj）
    lv_obj_t *click_obj = lv_event_get_target(e);
    lv_obj_t *lbl = (lv_obj_t *)lv_event_get_user_data(e);
    if (!click_obj) return;

    // 2) 向上查找“行容器”的用户数据（避免直接假设点击对象就是 row）
    lv_obj_t *row = NULL;
    favorite_row_data_t *ud = NULL;
    lv_obj_t *cur = click_obj;
    for (int i = 0; i < 6 && cur; i++) {
        favorite_row_data_t *maybe = (favorite_row_data_t *)lv_obj_get_user_data(cur);
        if (maybe && maybe->magic == FAVORITE_ROW_UD_MAGIC) {
            row = cur;
            ud = maybe;
            break;
        }
        cur = lv_obj_get_parent(cur);
    }
    if (!row || !ud) return;

    // 3) 非编辑模式：不弹窗，并阻止冒泡，避免触发“点击缩略图跳转”等行为
    if (!favorites_edit_mode) {
        lv_event_stop_bubbling(e);
        lv_event_stop_processing(e);
        return;
    }

    // 4) 兜底 label，并做“重复打开/对象未就绪”的保护
    if (!lbl) lbl = click_obj;
    if (favorites_rename_mask) return;
    if (!scr) scr = screen_14_love_favorites_get(&ui_manager);
    if (!scr || !scr->obj) return;

    // 5) 找到当前条目对应的收藏槽位（拿到初始名称）
    int slot = favorites_find_favorite_slot_by_flag(ud->flag);
    if (slot < 0) return;

    // 6) 记录弹窗目标：确定时写回 settings，并把 label 文本刷新成新名称
    favorites_rename_target_label = lbl;
    favorites_rename_target_flag = ud->flag;
    favorites_ignore_next_thumb_click = true;

    // 7) 创建通用“文本输入弹窗”：遮罩+面板+标题+输入框+取消/保存+键盘
    bool dark = favorites_is_dark();
    favorites_rename_mask = yk_ui_text_input_show(
        scr->obj,
        yk_ui_tr("favorites_rename_title", "编辑名称"),
        yk_ui_tr("cancel", "取消"),
        yk_ui_tr("save", "保存"),
        720,
        230,
        100,
        660,
        60,
        220,
        60,
        -10,
        140,
        true,
        INPUT_BUFFER_SIZE - 1,
        yk_settings.user_favorites[slot].name,
        210,
        dark,
        NULL,
        favorites_rename_ok_cb,
        favorites_rename_close_cb,
        NULL,
        &favorites_rename_ta,
        &favorites_rename_kb,
        &favorites_rename_title,
        &favorites_rename_lbl_cancel,
        &favorites_rename_lbl_ok
    );
}

static void favorites_commit_edits_to_settings(void)
{
    for (int i = 0; i < favorite_count; i++) {
        lv_obj_t *row = favorite_items[i];
        if (!row) continue;
        favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(row);
        if (!ud || ud->magic != FAVORITE_ROW_UD_MAGIC) continue;
        if (!ud->sld_temp || !ud->sld_time) continue;

        uint16_t temp_c = (uint16_t)(lv_slider_get_value(ud->sld_temp) * 5);
        uint16_t time_min = (uint16_t)lv_slider_get_value(ud->sld_time);
        if (temp_c < 45) temp_c = 45;
        if (temp_c > 200) temp_c = 200;
        if (time_min < 1) time_min = 1;
        if (time_min > 360) time_min = 360;

        for (int k = 0; k < FAVORITE_ITEM_COUNT; k++) {
            if (!yk_settings.user_favorites[k].is_favorite) continue;
            if (yk_settings.user_favorites[k].flag != ud->flag) continue;
            yk_settings.user_favorites[k].temperature = temp_c;
            yk_settings.user_favorites[k].time = time_min;
            break;
        }
    }
    yk_delay_save_data_timer_start();
}

static void favorites_confirm_close(void)
{
    yk_ui_confirm_dismiss();
}

static void favorites_confirm_ok_clicked(lv_event_t *e)
{
    (void)e;
    favorites_confirm_close();
    favorites_commit_edits_to_settings();
    favorites_new_flags_clear();
    favorites_ignore_next_thumb_click = false;
    favorites_edit_mode = false;
    favorites_apply_edit_mode(false);
}

static void favorites_confirm_cancel_clicked(lv_event_t *e)
{
    (void)e;
    favorites_confirm_close();
}

static void favorites_delete_confirm_close(void)
{
    yk_ui_confirm_dismiss();
    favorites_pending_delete_row = NULL;
    favorites_pending_delete_flag = 0;
}

static void favorites_delete_by_row(lv_obj_t *row, uint32_t flag_to_remove)
{
    if (!row) return;

    int index_to_remove = -1;
    for (int i = 0; i < favorite_count; i++) {
        if (favorite_items[i] == row) {
            index_to_remove = i;
            break;
        }
    }
    if (index_to_remove == -1) return;

    for (int k = 0; k < FAVORITE_ITEM_COUNT; k++) {
        if (yk_settings.user_favorites[k].is_favorite &&
            yk_settings.user_favorites[k].flag == flag_to_remove) {
            yk_settings.user_favorites[k].is_favorite = 0;
            memset(yk_settings.user_favorites[k].name, 0, sizeof(yk_settings.user_favorites[k].name));
            memset(yk_settings.user_favorites[k].customize, 0, sizeof(yk_settings.user_favorites[k].customize));
            memset(yk_settings.user_favorites[k].steps_custom, 0, sizeof(yk_settings.user_favorites[k].steps_custom));
            yk_settings.user_favorites[k].flag = yk_food_flag;
            yk_settings.user_favorites[k].temperature = 0;
            yk_settings.user_favorites[k].time = 0;
            break;
        }
    }
    favorites_new_flag_remove(flag_to_remove);
    yk_main_refresh_love_icon();

    favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(row);
    if (ud && ud->magic == FAVORITE_ROW_UD_MAGIC) lv_mem_free(ud);

    lv_obj_del(row);

    for (int j = index_to_remove; j < favorite_count - 1; j++) {
        favorite_items[j] = favorite_items[j + 1];
    }
    favorite_items[favorite_count - 1] = NULL;
    favorite_count--;

    yk_delay_save_data_timer_start();
}

static void favorites_delete_confirm_ok_clicked(lv_event_t *e)
{
    (void)e;
    lv_obj_t *row = favorites_pending_delete_row;
    uint32_t flag_to_remove = favorites_pending_delete_flag;
    favorites_delete_confirm_close();
    favorites_delete_by_row(row, flag_to_remove);
}

static void favorites_delete_confirm_cancel_clicked(lv_event_t *e)
{
    (void)e;
    favorites_delete_confirm_close();
}

static void favorites_show_delete_confirm(lv_obj_t *row, uint32_t flag_to_remove)
{
    if (!scr) scr = screen_14_love_favorites_get(&ui_manager);
    if (!scr || !scr->obj) return;

    favorites_pending_delete_row = row;
    favorites_pending_delete_flag = flag_to_remove;
    yk_ui_confirm_show(yk_ui_tr("favorites_delete_confirm", "确定删除？"),
                       520,
                       240,
                       yk_ui_tr("cancel", "取消"),
                       yk_ui_tr("ok", "确定"),
                       favorites_delete_confirm_cancel_clicked,
                       favorites_delete_confirm_ok_clicked);
}

static void favorites_show_save_confirm(void)
{
    if (!scr) scr = screen_14_love_favorites_get(&ui_manager);
    if (!scr || !scr->obj) return;
    yk_ui_confirm_show(yk_ui_tr("favorites_save_confirm", "保存修改？"),
                       520,
                       240,
                       yk_ui_tr("cancel", "取消"),
                       yk_ui_tr("ok", "确定"),
                       favorites_confirm_cancel_clicked,
                       favorites_confirm_ok_clicked);
}

// 判断某个 flag 是否存在于给定数组中
static bool favorites_flag_in_list(const uint32_t *flags, int cnt, uint32_t flag)
{
    for (int i = 0; i < cnt; i++) {
        if (flags[i] == flag) return true;
    }
    return false;
}

// 对单条收藏行应用编辑/非编辑模式（箭头、点击区、删除键、滑块可用性）
static void favorites_apply_row_mode(lv_obj_t *row, bool edit_mode)
{
    if (!row) return;

    favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(row);
    if (!ud || ud->magic != FAVORITE_ROW_UD_MAGIC) return;

    if (ud->dot_new) {
        if (edit_mode && favorites_new_flag_has(ud->flag)) lv_obj_clear_flag(ud->dot_new, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(ud->dot_new, LV_OBJ_FLAG_HIDDEN);
    }

    if (ud->img_arrow_l) {
        if (edit_mode) lv_obj_clear_flag(ud->img_arrow_l, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(ud->img_arrow_l, LV_OBJ_FLAG_HIDDEN);
    }

    if (ud->img_arrow_r) {
        if (edit_mode) lv_obj_clear_flag(ud->img_arrow_r, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(ud->img_arrow_r, LV_OBJ_FLAG_HIDDEN);
    }

    if (ud->click_area) {
        if (edit_mode) lv_obj_add_state(ud->click_area, LV_STATE_DISABLED);
        else lv_obj_clear_state(ud->click_area, LV_STATE_DISABLED);
    }

    if (ud->row_click) {
        if (edit_mode) lv_obj_add_flag(ud->row_click, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_clear_flag(ud->row_click, LV_OBJ_FLAG_HIDDEN);
    }

    if (ud->btn_del) {
        if (edit_mode) lv_obj_clear_state(ud->btn_del, LV_STATE_DISABLED);
        else lv_obj_add_state(ud->btn_del, LV_STATE_DISABLED);
    }

    if (ud->img_del) {
        if (edit_mode) lv_obj_clear_flag(ud->img_del, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(ud->img_del, LV_OBJ_FLAG_HIDDEN);
    }

    if (ud->sld_temp) {
        if (edit_mode) lv_obj_clear_state(ud->sld_temp, LV_STATE_DISABLED);
        else lv_obj_add_state(ud->sld_temp, LV_STATE_DISABLED);
    }

    if (ud->sld_time) {
        if (edit_mode) lv_obj_clear_state(ud->sld_time, LV_STATE_DISABLED);
        else lv_obj_add_state(ud->sld_time, LV_STATE_DISABLED);
    }
}

// 对整个收藏界面应用编辑/非编辑模式（编辑按钮图标、添加按钮、每行状态）
static void favorites_apply_edit_mode(bool edit_mode)
{
    if (!scr) return;

    if (scr->button_2_edit) {
        lv_obj_set_style_bg_img_src(
            scr->button_2_edit,
            edit_mode ? favorites_icon_edit02_path() : favorites_icon_edit01_path(),
            LV_PART_MAIN | LV_STATE_DEFAULT
        );
    }

    if (scr->button_1_add) {
        lv_obj_set_style_bg_img_src(
            scr->button_1_add,
            favorites_icon_add_path(),
            LV_PART_MAIN | LV_STATE_DEFAULT
        );
    }

    if (scr->button_1_add) {
        if (edit_mode) lv_obj_clear_flag(scr->button_1_add, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(scr->button_1_add, LV_OBJ_FLAG_HIDDEN);
    }

    for (int i = 0; i < favorite_count; i++) {
        favorites_apply_row_mode(favorite_items[i], edit_mode);
    }
}

// 添加按钮的业务逻辑：创建一个新的 DIY 收藏条目并持久化
static void favorites_add_btn_action(void)
{
    if (!scr) scr = screen_14_love_favorites_get(&ui_manager);
    if (!scr) return;

    if (yk_favorites_get_count() >= FAVORITE_ITEM_COUNT) {
        favorites_show_toast(yk_ui_tr("favorites_max_10", "最多收藏20个"), 3000);
        return;
    }

    int diy_count = 0;
    for (int k = 0; k < FAVORITE_ITEM_COUNT; k++) {
        if (!yk_settings.user_favorites[k].is_favorite) continue;
        diy_count++;
    }

    bool found_slot = false;
    uint8_t flag = 0;
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        uint8_t candidate = (uint8_t)(FAVORITE_FLAG_DIY_BASE + (uint8_t)i);
        bool used = false;
        for (int k = 0; k < FAVORITE_ITEM_COUNT; k++) {
            if (!yk_settings.user_favorites[k].is_favorite) continue;
            if (yk_settings.user_favorites[k].flag == candidate) {
                used = true;
                break;
            }
        }
        if (!used) {
            flag = candidate;
            found_slot = true;
            break;
        }
    }
    if (!found_slot) return;

    yk_user_favorite item;
    memset(&item, 0, sizeof(item));
    item.is_favorite = 1;
    snprintf(item.name, sizeof(item.name), "DIY%02d", diy_count + 1);
    item.customize[0] = '\0';
    item.steps_custom[0] = '\0';
    item.flag = flag;
    item.temperature = 200;
    item.time = 30;
    favorites_settings_insert_front(&item);

    yk_delay_save_data_timer_start();
    favorites_new_flag_add(flag);
    yk_favorites_reload_from_settings();
    if (scr->container_3_favorites) {
        lv_obj_update_layout(scr->container_3_favorites);
        lv_obj_scroll_to_y(scr->container_3_favorites, 0, LV_ANIM_OFF);
        if (favorite_items[0]) {
            lv_obj_scroll_to_view(favorite_items[0], LV_ANIM_OFF);
        }
    }
}

// 收藏界面加载完成回调：绑定 scr、重置编辑态并刷新列表
static void favorites_screen_loaded(lv_event_t *e)
{
    (void)e;
    scr = screen_14_love_favorites_get(&ui_manager);
    favorites_edit_mode = false;
    yk_favorites_reload_from_settings();
}

// 摄氏温度转华氏显示值（含特殊值与个位 0/5 规则）
static uint16_t favorite_temp_c_to_f_display(uint16_t temp_c)
{
    uint16_t sel_val_t_f = (uint16_t)(1.8 * temp_c + 32 + 0.5);
    if (sel_val_t_f <= 115) {
        sel_val_t_f = 110;
    } else if (sel_val_t_f >= 392) {
        sel_val_t_f = 400;
    }
    sel_val_t_f = adjust_to_nearest_0_or_5(sel_val_t_f);
    return sel_val_t_f;
}

// 根据单位设置温度标签文本（℃/℉）
static void favorite_set_temp_label(lv_obj_t *lbl, uint16_t temp_c)
{
    if (!lbl) return;

    if (yk_settings.f_to_c == val2_t_f) {
        lv_label_set_text_fmt(lbl, "%d℉", favorite_temp_c_to_f_display(temp_c));
    } else {
        lv_label_set_text_fmt(lbl, "%d℃", temp_c);
    }
}


// 收藏界面首次创建：绑定屏幕加载事件并刷新列表
void screen_14_love_favorites_custom_created(void)
{ 
    yk_printf("screen_14_love_favorites_custom_created\n");
    yk_v_screen_id = SCREEN_14_ID;
    scr = screen_14_love_favorites_get(&ui_manager);
    if (scr) {
        if (scr->button_1_x) {
            lv_obj_set_style_bg_img_src(
                scr->button_1_x,
                favorites_icon_x_path(),
                LV_PART_MAIN | LV_STATE_DEFAULT
            );
        }
        if (scr->obj && scr->obj != favorites_bound_screen_obj) {
            lv_obj_add_event_cb(scr->obj, favorites_screen_loaded, LV_EVENT_SCREEN_LOADED, NULL);
            favorites_bound_screen_obj = scr->obj;
        }
    }
    favorites_edit_mode = false;
    yk_favorites_reload_from_settings();
}

// 点击 X：是否保存修改并返回主界面 
void screen_14_love_favorites_button_1_x_custom_clicked(void)
{
    if (favorites_edit_mode) {
        favorites_show_save_confirm();
        return;
    }
    yk_main_update_ui_text();
    yk_view_set_dis_view(SCREEN_02_MAIN_ID);
}

// 点击添加：新增一个 DIY 收藏项
void screen_14_love_favorites_button_1_add_custom_clicked(void)
{
    yk_f_touch_sound();
    favorites_add_btn_action();
}

// 点击编辑：切换编辑模式，并在退出编辑时回写滑块数值到设置
void screen_14_love_favorites_button_2_edit_custom_clicked(void)
{
    yk_f_touch_sound();
    bool next_mode = !favorites_edit_mode;
    if (!favorites_edit_mode && next_mode) {
        favorites_new_flags_clear();
    } else if (favorites_edit_mode && !next_mode) {
        favorites_commit_edits_to_settings();
        favorites_ignore_next_thumb_click = false;
    }

    favorites_edit_mode = next_mode;
    favorites_apply_edit_mode(favorites_edit_mode);
}

// 回调：收藏项内温度滑块变化
static void favorite_slider_temp_changed(lv_event_t *e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    lv_obj_t * lbl = (lv_obj_t *)lv_event_get_user_data(e);
    if (!lbl) return;
    uint16_t val = lv_slider_get_value(slider) * 5;
    favorite_set_temp_label(lbl, val);
}

// 回调：收藏项内时间滑块变化
static void favorite_slider_time_changed(lv_event_t *e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    lv_obj_t * lbl = (lv_obj_t *)lv_event_get_user_data(e);
    if (!lbl) return;
    int val = lv_slider_get_value(slider);
    lv_label_set_text_fmt(lbl, "%d MIN", val);
}

static void favorites_open_row(lv_obj_t *row)
{
    // 第一步：基础保护（空指针不处理）
    if (!row) return;
    // 第二步：如果上一轮事件标记为“忽略下一次点击”，则消费掉该标记并直接返回
    if (favorites_ignore_next_thumb_click) {
        favorites_ignore_next_thumb_click = false;
        return;
    }
    // 第三步：编辑模式下禁止跳转主界面（避免误触导致退出编辑流程）
    if (favorites_edit_mode) return;

    // 第四步：取出当前行的 user_data（里面包含 flag、滑块等关键信息）
    favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(row);
    // 第五步：校验 user_data（防止被覆盖或不是收藏行）
    if (!ud || ud->magic != FAVORITE_ROW_UD_MAGIC) return;

    // 第六步：获取主界面对象（用于同步主界面上的 UI 控件）
    screen_02_main_t *main_scr = screen_02_main_get(&ui_manager);
    if (!main_scr) return;

    // 第七步：主界面运行中时，不允许跳转/切换（只提示并保持当前界面）
    if (!lv_obj_has_flag(main_scr->container_2_3_run, LV_OBJ_FLAG_HIDDEN)) {
        yk_view_set_dis_view(SCREEN_02_MAIN_ID);
        // yk_show_toast(main_scr->container_7_toast_notification, yk_ui_tr("busy_cannot_switch", "运行中无法切换"), 3000);
        yk_ui_center_toast_show(yk_ui_tr("busy_cannot_switch", "运行中无法切换"), 3000);
        return;
    }

    // 第八步：清掉主界面 4 个 DIY 标签的选中态（进入主界面后保持一致的“未选中”起点）
    lv_obj_remove_state(main_scr->label_1_airfry, LV_STATE_CHECKED);
    lv_obj_remove_state(main_scr->label_2_bake, LV_STATE_CHECKED);
    lv_obj_remove_state(main_scr->label_3_preheat, LV_STATE_CHECKED);
    lv_obj_remove_state(main_scr->label_4_dehydrate, LV_STATE_CHECKED);

    // 第九步：把收藏 flag 映射为主界面 food_flag
    // - DIY 收藏（flag>=FAVORITE_FLAG_DIY_BASE）：主界面统一落在 food0（自定义）
    // - 普通收藏：food_flag 直接等于 flag
    uint8_t mapped_flag = (ud->flag >= FAVORITE_FLAG_DIY_BASE) ? 0 : (uint8_t)ud->flag;
    // 第十步：更新全局状态（active_favorite_flag 用于信息区与爱心；food_flag 用于主界面选中项）
    yk_active_favorite_flag = ud->flag;
    yk_food_flag = mapped_flag;
    // 第十一步：标记“当前标题显示来自收藏列表”（主界面标题优先显示收藏名称）
    yk_active_from_favorites = true;
    // 第十二步：刷新主界面选中态/主图/爱心图标等 UI
    update_all_containers_border_opa(yk_food_flag);
    // 第十三步：如果是 DIY 收藏，把主界面大图切到 DIY 图（与普通菜谱区分）
    if (ud->flag >= FAVORITE_FLAG_DIY_BASE) {
        lv_obj_set_style_bg_img_src(main_scr->container_2_1, LVGL_IMAGE_PATH(diy_00_01.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // 第十四步：把收藏行上的温度/时间同步到主界面（用于立即显示并驱动默认参数）
    if (ud->sld_temp && ud->sld_time) {
        uint16_t temp_c = (uint16_t)(lv_slider_get_value(ud->sld_temp) * 5);
        uint16_t time_min = (uint16_t)lv_slider_get_value(ud->sld_time);
        food_tf_time(temp_c, time_min);
    }

    // 第十五步：同步主界面爱心图标（收藏一定是 love1，但这里仍用 has_flag 做一致性判断）
    if (main_scr->image_2_2_1_love) {
        const char *love0 = LVGL_IMAGE_PATH(icon_love_0.png);
        const char *love1 = LVGL_IMAGE_PATH(icon_love_1.png);
        lv_img_set_src(main_scr->image_2_2_1_love, yk_favorites_has_flag(ud->flag) ? love1 : love0);
    }

    // 第十六步：刷新主界面文案（标题 + 信息区），确保标题显示收藏名/默认名符合规则
    yk_main_update_ui_text();
    // 第十七步：切换到主界面
    yk_view_set_dis_view(SCREEN_02_MAIN_ID);
}

// 点击收藏缩略图：同步参数到主界面并切换回主界面
static void favorite_thumb_clicked(lv_event_t *e)
{
    lv_obj_t *row = (lv_obj_t *)lv_event_get_user_data(e);
    favorites_open_row(row);
}

static lv_obj_t *favorites_find_row_from_obj(lv_obj_t *obj)
{
    lv_obj_t *cur = obj;
    while (cur) {
        favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(cur);
        if (ud && ud->magic == FAVORITE_ROW_UD_MAGIC) return cur;
        cur = lv_obj_get_parent(cur);
    }
    return NULL;
}

static void favorites_settings_insert_front(const yk_user_favorite *item)
{
    if (!item) return;

    yk_user_favorite tmp[FAVORITE_ITEM_COUNT];
    memset(tmp, 0, sizeof(tmp));

    int out = 0;
    tmp[out++] = *item;

    for (int i = 0; i < FAVORITE_ITEM_COUNT && out < FAVORITE_ITEM_COUNT; i++) {
        const yk_user_favorite *fav = &yk_settings.user_favorites[i];
        if (!fav->is_favorite) continue;
        if (fav->flag == item->flag) continue;
        tmp[out++] = *fav;
    }

    memcpy(yk_settings.user_favorites, tmp, sizeof(tmp));
}

// 删除收藏项回调（点击删除按钮）- 修改为基于flag的方式删除
static void favorite_delete_cb(lv_event_t *e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * row = favorites_find_row_from_obj(btn);
    if (!row) return;

    favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(row);
    uint32_t flag_to_remove = (ud && ud->magic == FAVORITE_ROW_UD_MAGIC) ? ud->flag : yk_food_flag;
    favorites_show_delete_confirm(row, flag_to_remove);
}

static lv_obj_t *favorites_create_row(
    lv_obj_t *parent,
    const void *img_src,
    const char *name,
    uint16_t temp,
    uint16_t time_min,
    uint32_t flag,
    uint32_t txt_color,
    uint32_t slider_track,
    uint32_t slider_track_disabled,
    uint32_t slider_indicator_disabled
)
{
    if (!parent) return NULL;

    if (temp < 45) temp = 45;
    if (temp > 200) temp = 200;
    if (time_min < 1) time_min = 1;
    if (time_min > 360) time_min = 360;

    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, lv_pct(100), 96);
    lv_obj_set_style_pad_left(row, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(row, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(row, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(row, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_bottom(row, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(row, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(row, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(row, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *top_line = lv_obj_create(row);
    lv_obj_set_size(top_line, lv_pct(100), 60);
    lv_obj_set_style_border_width(top_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(top_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(top_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(top_line, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top_line, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(top_line, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *left_top = lv_obj_create(top_line);
    lv_obj_set_size(left_top, 100, 60);
    lv_obj_set_style_border_width(left_top, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(left_top, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(left_top, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_left(left_top, FAVORITES_LEFT_GROUP_SHIFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_right(left_top, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(left_top, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(left_top, LV_OBJ_FLAG_OVERFLOW_VISIBLE);

    lv_obj_t *img_arrow_l = lv_img_create(left_top);
    lv_img_set_src(img_arrow_l, LVGL_IMAGE_PATH(icon_arrows_left.png));
    lv_obj_align(img_arrow_l, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *img = lv_img_create(left_top);
    lv_img_set_src(img, img_src);
    lv_obj_set_size(img, 60, 60);
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 20, 0);
    lv_obj_set_style_margin_bottom(left_top, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *dot_new = favorites_create_new_dot(left_top);
    lv_obj_set_pos(dot_new, 74, 0);
    lv_obj_move_foreground(dot_new);

    lv_obj_t *img_arrow_r = lv_img_create(left_top);
    lv_img_set_src(img_arrow_r, LVGL_IMAGE_PATH(icon_arrows_right.png));
    lv_obj_align(img_arrow_r, LV_ALIGN_LEFT_MID, 84, 0);

    lv_obj_t *lbl_temp = lv_label_create(top_line);
    favorite_set_temp_label(lbl_temp, temp);
    lv_label_set_long_mode(lbl_temp, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(lbl_temp, 80);
    lv_obj_set_height(lbl_temp, 24);
    lv_obj_set_style_text_font(lbl_temp, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl_temp, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_right(lbl_temp, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *sld_temp = lv_slider_create(top_line);
    lv_obj_set_size(sld_temp, 220, 20);
    lv_obj_set_style_radius(sld_temp, favorites_slider_radius(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(sld_temp, favorites_slider_radius(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sld_temp, lv_color_hex(slider_track), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sld_temp, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_COVER, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sld_temp, lv_color_hex(slider_track_disabled), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(sld_temp, lv_color_hex(slider_indicator_disabled), LV_PART_INDICATOR | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_COVER, LV_PART_INDICATOR | LV_STATE_DISABLED);
    lv_obj_set_style_margin_right(sld_temp, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_slider_set_range(sld_temp, 45/5, 200/5);
    lv_slider_set_value(sld_temp, temp/5, LV_ANIM_OFF);
    lv_obj_add_event_cb(sld_temp, favorite_slider_temp_changed, LV_EVENT_VALUE_CHANGED, lbl_temp);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_width(sld_temp, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_height(sld_temp, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_border_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_outline_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_width(sld_temp, 0, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_height(sld_temp, 0, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_outline_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_width(sld_temp, 0, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_height(sld_temp, 0, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_opa(sld_temp, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_width(sld_temp, 0, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_height(sld_temp, 0, LV_PART_KNOB | LV_STATE_FOCUSED);

    lv_obj_t *spacer = lv_obj_create(top_line);
    lv_obj_set_size(spacer, 0, 0);
    lv_obj_set_style_bg_opa(spacer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(spacer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(spacer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(spacer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_grow(spacer, 1);

    lv_obj_t *lbl_time = lv_label_create(top_line);
    lv_label_set_text_fmt(lbl_time, "%d MIN", time_min);
    lv_label_set_long_mode(lbl_time, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(lbl_time, 80);
    lv_obj_set_height(lbl_time, 24);
    lv_obj_set_style_text_font(lbl_time, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl_time, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_right(lbl_time, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *sld_time = lv_slider_create(top_line);
    lv_obj_set_size(sld_time, 220, 20);
    lv_obj_set_style_radius(sld_time, favorites_slider_radius(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(sld_time, favorites_slider_radius(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sld_time, lv_color_hex(slider_track), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sld_time, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_COVER, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sld_time, lv_color_hex(slider_track_disabled), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(sld_time, lv_color_hex(slider_indicator_disabled), LV_PART_INDICATOR | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_COVER, LV_PART_INDICATOR | LV_STATE_DISABLED);
    lv_obj_set_style_margin_right(sld_time, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    int time_max = (time_min > 60) ? 360 : 60;
    lv_slider_set_range(sld_time, 1, time_max);
    lv_slider_set_value(sld_time, time_min, LV_ANIM_OFF);
    lv_obj_add_event_cb(sld_time, favorite_slider_time_changed, LV_EVENT_VALUE_CHANGED, lbl_time);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_width(sld_time, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_height(sld_time, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_border_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_outline_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_width(sld_time, 0, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_height(sld_time, 0, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_outline_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_width(sld_time, 0, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_height(sld_time, 0, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_opa(sld_time, LV_OPA_TRANSP, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_width(sld_time, 0, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_style_height(sld_time, 0, LV_PART_KNOB | LV_STATE_FOCUSED);
    lv_obj_set_flex_grow(sld_time, 0);

    lv_obj_t *btn_del = lv_btn_create(top_line);
    lv_obj_set_size(btn_del, 80, 50);
    lv_obj_clear_flag(btn_del, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_clear_flag(btn_del, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_border_width(btn_del, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // f:\AiUlBuilder\LVGL\1152x576_fan_img\ui_builder\custom - 统一按钮去阴影
    yk_ui_btn_disable_shadow(btn_del);
    lv_obj_set_style_outline_width(btn_del, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(btn_del, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(btn_del, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn_del, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn_del, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(btn_del, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(btn_del, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(btn_del, 0, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_margin_left(btn_del, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_right(btn_del, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(btn_del, favorite_delete_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *img_del = lv_img_create(btn_del);
    lv_img_set_src(img_del, favorites_icon_del_path());
    lv_obj_align(img_del, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *name_hit = lv_obj_create(row);
    lv_obj_set_size(name_hit, lv_pct(100), 34);
    lv_obj_set_style_border_width(name_hit, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(name_hit, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(name_hit, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_left(name_hit, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(name_hit, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *lbl_name = lv_label_create(name_hit);
    lv_label_set_text(lbl_name, name ? name : "");
    lv_label_set_long_mode(lbl_name, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(lbl_name, LV_SIZE_CONTENT);
    lv_obj_set_height(lbl_name, 34);
    lv_obj_set_style_text_align(lbl_name, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl_name, fs_montserratmedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl_name, lv_color_hex(txt_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(lbl_name, LV_ALIGN_LEFT_MID, 43, 0);
    lv_obj_add_flag(lbl_name, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(lbl_name, favorites_rename_name_clicked, LV_EVENT_CLICKED, lbl_name);

    lv_obj_t *row_click = lv_obj_create(row);
    lv_obj_add_flag(row_click, LV_OBJ_FLAG_FLOATING);
    lv_obj_add_flag(row_click, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(row_click, 0, 0);
    lv_obj_set_size(row_click, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(row_click, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(row_click, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(row_click, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(row_click, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(row_click, favorite_thumb_clicked, LV_EVENT_CLICKED, row);
    lv_obj_move_foreground(row_click);

    favorite_row_data_t *ud = (favorite_row_data_t *)lv_mem_alloc(sizeof(favorite_row_data_t));
    if (ud) {
        ud->magic = FAVORITE_ROW_UD_MAGIC;
        ud->img_src = img_src;
        ud->flag = flag;
        ud->img_arrow_l = img_arrow_l;
        ud->img_arrow_r = img_arrow_r;
        ud->click_area = left_top;
        ud->row_click = row_click;
        ud->lbl_name = lbl_name;
        ud->btn_del = btn_del;
        ud->img_del = img_del;
        ud->sld_temp = sld_temp;
        ud->sld_time = sld_time;
        ud->dot_new = dot_new;
        lv_obj_set_user_data(row, (void*)ud);
    } else {
        lv_obj_set_user_data(row, (void*)img_src);
    }

    return row;
}

// 创建一条收藏项并添加到收藏界面容器，同时写入设置并触发保存
void yk_favorites_add(lv_obj_t *parent, const void *img_src, const char *name, uint16_t temp, uint16_t time_min)
{
    // 1) 基本保护：界面未初始化/数量超限/父容器为空则不处理
    if (!scr) return;
    if (favorite_count >= MAX_FAVORITES) return;

    if (!parent) return;

    // 2) 生成收藏 flag：
    //    - 普通菜谱：flag 直接用 yk_food_flag
    //    - DIY（yk_food_flag==0）：从 FAVORITE_FLAG_DIY_BASE 开始找一个未占用的 flag
    uint32_t flag = yk_food_flag;
    if (flag == 0) {
        bool found_slot = false;
        for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
            uint8_t candidate = (uint8_t)(FAVORITE_FLAG_DIY_BASE + (uint8_t)i);
            bool used = false;
            for (int k = 0; k < FAVORITE_ITEM_COUNT; k++) {
                if (!yk_settings.user_favorites[k].is_favorite) continue;
                if (yk_settings.user_favorites[k].flag == candidate) {
                    used = true;
                    break;
                }
            }
            if (!used) {
                flag = candidate;
                found_slot = true;
                break;
            }
        }
        if (!found_slot) return;
    }
    uint32_t txt_color = favorites_text_color_hex();
    uint32_t slider_track = favorites_slider_track_color_hex();
    uint32_t slider_track_disabled = favorites_slider_track_disabled_color_hex();
    uint32_t slider_indicator_disabled = favorites_slider_indicator_disabled_color_hex();

    // 3) 确保父容器使用竖向布局（避免子项重叠）
    // 确保父容器使用竖向布局（避免子项重叠）
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(parent, 44, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 4) 创建 UI 行（收藏列表里的一个条目）
    lv_obj_t *row = favorites_create_row(
        parent,
        img_src,
        name,
        temp,
        time_min,
        flag,
        txt_color,
        slider_track,
        slider_track_disabled,
        slider_indicator_disabled
    );
    if (!row) return;
    
    // 5) 插入到列表头部并更新运行时数组
    for (int i = favorite_count; i > 0; i--) {
        favorite_items[i] = favorite_items[i - 1];
    }
    favorite_items[0] = row;
    favorite_count++;
    favorites_new_flag_add(flag);
    favorites_apply_row_mode(row, favorites_edit_mode);
    lv_obj_move_to_index(row, 0);
    lv_obj_update_layout(parent);
    lv_obj_scroll_to_y(parent, 0, LV_ANIM_OFF);

    // 6) 写入持久化设置（仅保存收藏基本信息：名称/温度/时间/flag）
    //    注意：普通菜谱自定义内容已统一存储在 food_customize / food_steps_custom 中，这里不再保存自定义内容
    //          DIY 收藏（yk_food_flag==0）需要保存自己的一份自定义内容（避免多个 DIY 收藏共用一份）
    yk_user_favorite item;
    memset(&item, 0, sizeof(item));
    item.is_favorite = 1;
    strncpy(item.name, name ? name : "", sizeof(item.name) - 1);
    item.name[sizeof(item.name) - 1] = '\0';
    if (yk_food_flag == 0) {
        strncpy(item.customize, yk_settings.food_customize[0], sizeof(item.customize) - 1);
        item.customize[sizeof(item.customize) - 1] = '\0';
    } else {
        item.customize[0] = '\0';
    }
    item.steps_custom[0] = '\0';
    item.flag = (uint8_t)flag;
    item.temperature = temp;
    item.time = time_min;
    favorites_settings_insert_front(&item);
    
    // 7) 触发保存，并把当前 active flag 切换到新收藏（用于主界面爱心/选中态刷新）
    // 触发数据保存
    yk_delay_save_data_timer_start();
    yk_active_favorite_flag = flag;
}

// 按标志移除收藏项，并刷新列表与主界面爱心图标
void yk_favorites_remove_by_flag(uint32_t food_flag)
{
    // 1) 在设置里找到对应的收藏槽位
    int removed = 0;
    for (int k = 0; k < FAVORITE_ITEM_COUNT; k++) {
        if (yk_settings.user_favorites[k].is_favorite &&
            yk_settings.user_favorites[k].flag == food_flag) {
            // 2) 清空该收藏项（自定义内容不在这里维护，统一存储在 food_customize / food_steps_custom）
            yk_settings.user_favorites[k].is_favorite = 0;
            memset(yk_settings.user_favorites[k].name, 0, sizeof(yk_settings.user_favorites[k].name));
            memset(yk_settings.user_favorites[k].customize, 0, sizeof(yk_settings.user_favorites[k].customize));
            memset(yk_settings.user_favorites[k].steps_custom, 0, sizeof(yk_settings.user_favorites[k].steps_custom));
            yk_settings.user_favorites[k].flag = yk_food_flag;
            yk_settings.user_favorites[k].temperature = 0;
            yk_settings.user_favorites[k].time = 0;
            removed = 1;
            break;
        }
    }

    // 3) 没找到则直接返回
    if (!removed) return;
    // 4) 触发保存，并刷新收藏列表与主界面爱心图标
    yk_delay_save_data_timer_start();
    yk_favorites_reload_from_settings();
    yk_main_refresh_love_icon();
}

// 检查图片源是否已在收藏中 - 优化版本（兼容旧数据）
int yk_favorites_has_img(const void *img_src)
{
    if (!img_src) return 0;
    
    for (int i = 0; i < favorite_count; i++) {
        lv_obj_t *row = favorite_items[i];
        if (!row) continue;
        
        // 从用户数据中获取图片源
        void *ud_ptr = lv_obj_get_user_data(row);
        const void *stored_img_src = NULL;
        if (ud_ptr) {
            favorite_row_data_t *ud = (favorite_row_data_t *)ud_ptr;
            stored_img_src = (ud->magic == FAVORITE_ROW_UD_MAGIC) ? ud->img_src : (const void *)ud_ptr;
        }
        if (stored_img_src && strcmp((const char*)stored_img_src, (const char*)img_src) == 0) {
            return 1;
        }
    }
    return 0;
}

// 检查指定标志是否已在收藏中
int yk_favorites_has_flag(uint32_t food_flag)
{
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        if (yk_settings.user_favorites[i].is_favorite && 
            yk_settings.user_favorites[i].flag == food_flag) {
            return 1;
        }
    }
    return 0;
}

// 返回当前收藏数量
int yk_favorites_get_count(void)
{
    // 优化：返回设置结构体中的真实收藏数量
    int count = 0;
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        if (yk_settings.user_favorites[i].is_favorite) {
            count++;
        }
    }
    return count;
}

// 从设置中重建收藏列表：清空旧UI、按顺序生成行并应用当前编辑态
void yk_favorites_reload_from_settings(void)
{
    if (!scr) scr = screen_14_love_favorites_get(&ui_manager);
    if (!scr) return;
    lv_obj_t *parent = scr->container_3_favorites;
    if (!parent) return;

    if (scr->button_1_x) {
        lv_obj_set_style_bg_img_src(
            scr->button_1_x,
            favorites_icon_x_path(),
            LV_PART_MAIN | LV_STATE_DEFAULT
        );
    }

    uint32_t txt_color = favorites_text_color_hex();
    uint32_t slider_track = favorites_slider_track_color_hex();
    uint32_t slider_track_disabled = favorites_slider_track_disabled_color_hex();
    uint32_t slider_indicator_disabled = favorites_slider_indicator_disabled_color_hex();

    uint32_t prev_flags[MAX_FAVORITES];
    int prev_flag_cnt = 0;
    for (int i = 0; i < favorite_count && prev_flag_cnt < MAX_FAVORITES; i++) {
        lv_obj_t *row = favorite_items[i];
        if (!row) continue;
        favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(row);
        if (ud && ud->magic == FAVORITE_ROW_UD_MAGIC) {
            prev_flags[prev_flag_cnt++] = ud->flag;
        }
    }

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(parent, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(parent, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(parent, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(parent, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_style_width(parent, 6, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(parent, lv_color_hex(0xC8C8C8), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(parent, 200, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(parent, 3, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    int child_cnt = lv_obj_get_child_cnt(parent);
    for (int i = 0; i < child_cnt; i++) {
        lv_obj_t *child = lv_obj_get_child(parent, 0);
        if (child) {
            favorite_row_data_t *ud = (favorite_row_data_t *)lv_obj_get_user_data(child);
            if (ud && ud->magic == FAVORITE_ROW_UD_MAGIC) lv_mem_free(ud);
            lv_obj_del(child);
        }
    }
    memset(favorite_items, 0, sizeof(favorite_items));
    favorite_count = 0;

    int order[FAVORITE_ITEM_COUNT];
    int order_cnt = 0;
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        if (!yk_settings.user_favorites[i].is_favorite) continue;
        if (!favorites_flag_in_list(prev_flags, prev_flag_cnt, yk_settings.user_favorites[i].flag)) {
            order[order_cnt++] = i;
        }
    }
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        if (!yk_settings.user_favorites[i].is_favorite) continue;
        if (favorites_flag_in_list(prev_flags, prev_flag_cnt, yk_settings.user_favorites[i].flag)) {
            order[order_cnt++] = i;
        }
    }

    for (int oi = 0; oi < order_cnt; oi++) {
        int i = order[oi];
        if (!yk_settings.user_favorites[i].is_favorite) continue;
        uint8_t flag = yk_settings.user_favorites[i].flag;
        uint16_t temp = yk_settings.user_favorites[i].temperature;
        uint16_t time_min = yk_settings.user_favorites[i].time;
        const char *name = yk_settings.user_favorites[i].name[0] ? yk_settings.user_favorites[i].name : NULL;
        const void *img_src = (flag >= FAVORITE_FLAG_DIY_BASE) ? LVGL_IMAGE_PATH(food_00_03.png) : yk_get_favorite_image_path_by_flag(flag);

        lv_obj_t *row = favorites_create_row(
            parent,
            img_src,
            name,
            temp,
            time_min,
            (uint32_t)flag,
            txt_color,
            slider_track,
            slider_track_disabled,
            slider_indicator_disabled
        );
        if (!row) continue;

        favorite_items[favorite_count++] = row;
    }
    lv_obj_update_layout(parent);
    if (lv_obj_get_scroll_bottom(parent) > 0) {
        lv_obj_set_style_pad_right(parent, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_pad_right(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    favorites_apply_edit_mode(favorites_edit_mode);
}

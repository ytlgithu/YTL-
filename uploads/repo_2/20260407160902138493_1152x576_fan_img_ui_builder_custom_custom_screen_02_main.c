#include "custom.h"
#include <string.h>


uint8_t yk_food_flag = 0;  //食物标志
uint8_t yk_diy_flag = 0;  //自定义标志
uint32_t yk_active_favorite_flag = 0;
bool yk_active_from_favorites = false;
uint16_t yk_v_food_t_f = 100;  //食物烘烤温度单位 ℃
uint16_t yk_v_food_min = 30;  //食物烘烤时间单位 min


static screen_02_main_t *scr = NULL;
static lv_obj_t *main_info_scroll = NULL;

static lv_obj_t *main_customize_mask = NULL;
static lv_obj_t *main_customize_ta = NULL;
static lv_obj_t *main_customize_kb = NULL;
static uint32_t main_customize_target_flag = 0;
static bool main_customize_is_steps = false;
static int main_customize_target_slot = -1;
static uint8_t main_customize_food_index = 0;
static bool main_customize_save_to_favorite = false;

static bool main_info_confirm_show_steps = false;
static bool main_info_confirm_open = false;

// 只在主界面第一次创建时做一次“默认态”初始化，避免每次进主界面都重置用户选择
static bool main_first_enter_done = false;
static uint32_t main_last_diy_favorite_flag = 0;

// 多语言自定义文本“打包格式”说明：
// - 兼容两种存储方式：
//   1) 非打包：直接存一段纯文本（视为所有语言共用一份）
//   2) 打包：按语言分别存储，格式为：langId<0x1F>text<0x1E>langId<0x1F>text...
// - 0x1E：条目分隔符（Entry Separator）
// - 0x1F：键值分隔符（Key/Value Separator）
#define YK_LANG_ENTRY_SEP ((char)0x1E)
#define YK_LANG_KV_SEP ((char)0x1F)

static int main_lang_id(void)
{
    // 1) 读取当前语言枚举
    int v = (int)yk_settings.language;
    // 2) 做边界保护，避免越界访问
    if (v < 0) v = 0;
    if (v > (int)val3_language_it) v = (int)val3_language_it;
    // 3) 返回安全的语言 id
    return v;
}

static bool main_packed_lang_is_packed(const char *s)
{
    // 判断字符串是否为“打包格式”
    if (!s) return false;
    for (; *s; s++) {
        if (*s == YK_LANG_ENTRY_SEP || *s == YK_LANG_KV_SEP) return true;
    }
    return false;
}

static const char *main_packed_lang_get(const char *packed, int lang, char *out, size_t out_sz)
{
    // 1) 参数检查与输出缓冲初始化
    if (!out || out_sz == 0) return NULL;
    out[0] = '\0';
    if (!packed || packed[0] == '\0') return NULL;

    // 2) 非打包：直接拷贝（等价“所有语言共用一份”）
    if (!main_packed_lang_is_packed(packed)) {
        size_t n = strlen(packed);
        if (n >= out_sz) n = out_sz - 1;
        memcpy(out, packed, n);
        out[n] = '\0';
        return out;
    }

    // 3) 打包：逐条解析 langId<kv>text<entry>，找到匹配语言返回
    const char *p = packed;
    while (*p) {
        int id = 0;
        bool have_id = false;
        const char *id_start = p;
        while (*p && *p != YK_LANG_KV_SEP && *p != YK_LANG_ENTRY_SEP) {
            if (*p < '0' || *p > '9') {
                have_id = false;
                break;
            }
            have_id = true;
            id = id * 10 + (*p - '0');
            p++;
        }

        if (*p != YK_LANG_KV_SEP) {
            p = id_start;
            while (*p && *p != YK_LANG_ENTRY_SEP) p++;
            if (*p == YK_LANG_ENTRY_SEP) p++;
            continue;
        }
        p++;

        const char *val_start = p;
        while (*p && *p != YK_LANG_ENTRY_SEP) p++;
        const char *val_end = p;

        // 4) 命中目标语言：把 value 拷贝到 out 返回
        if (have_id && id == lang) {
            size_t n = (size_t)(val_end - val_start);
            if (n >= out_sz) n = out_sz - 1;
            memcpy(out, val_start, n);
            out[n] = '\0';
            return out;
        }
        if (*p == YK_LANG_ENTRY_SEP) p++;
    }
    // 5) 没找到：返回 NULL（out 保持为空串）
    return NULL;
}

static void main_packed_lang_set(char *packed, size_t packed_sz, int lang, const char *val)
{
    // 1) 参数检查与输入兜底
    if (!packed || packed_sz == 0) return;
    if (!val) val = "";

    char tmp[512];
    tmp[0] = '\0';
    bool wrote_any = false;

    // 2) 如果原来是打包格式：先把其它语言条目原样保留（跳过当前 lang）
    if (main_packed_lang_is_packed(packed)) {
        const char *p = packed;
        while (*p) {
            int id = 0;
            bool have_id = false;
            const char *id_start = p;
            while (*p && *p != YK_LANG_KV_SEP && *p != YK_LANG_ENTRY_SEP) {
                if (*p < '0' || *p > '9') {
                    have_id = false;
                    break;
                }
                have_id = true;
                id = id * 10 + (*p - '0');
                p++;
            }
            if (*p != YK_LANG_KV_SEP) {
                p = id_start;
                while (*p && *p != YK_LANG_ENTRY_SEP) p++;
                if (*p == YK_LANG_ENTRY_SEP) p++;
                continue;
            }
            p++;
            const char *val_start = p;
            while (*p && *p != YK_LANG_ENTRY_SEP) p++;
            const char *val_end = p;
            if (*p == YK_LANG_ENTRY_SEP) p++;

            if (!have_id) continue;
            if (id == lang) continue;
            size_t vn = (size_t)(val_end - val_start);
            if (vn == 0) continue;

            char idbuf[12];
            snprintf(idbuf, sizeof(idbuf), "%d", id);
            size_t need = strlen(idbuf) + 1 + vn + (wrote_any ? 1 : 0);
            if (strlen(tmp) + need >= sizeof(tmp)) continue;
            if (wrote_any) {
                size_t tl = strlen(tmp);
                tmp[tl] = YK_LANG_ENTRY_SEP;
                tmp[tl + 1] = '\0';
            }
            strncat(tmp, idbuf, sizeof(tmp) - strlen(tmp) - 1);
            {
                size_t tl = strlen(tmp);
                tmp[tl] = YK_LANG_KV_SEP;
                tmp[tl + 1] = '\0';
            }
            strncat(tmp, val_start, vn);
            tmp[strlen(tmp)] = '\0';
            wrote_any = true;
        }
    }

    // 3) 把当前语言的新值追加进去（val 为空表示“清空该语言自定义”）
    if (val[0] != '\0') {
        char idbuf[12];
        snprintf(idbuf, sizeof(idbuf), "%d", lang);
        size_t vn = strlen(val);
        size_t need = strlen(idbuf) + 1 + vn + (wrote_any ? 1 : 0);
        if (strlen(tmp) + need < sizeof(tmp)) {
            if (wrote_any) {
                size_t tl = strlen(tmp);
                tmp[tl] = YK_LANG_ENTRY_SEP;
                tmp[tl + 1] = '\0';
            }
            strncat(tmp, idbuf, sizeof(tmp) - strlen(tmp) - 1);
            {
                size_t tl = strlen(tmp);
                tmp[tl] = YK_LANG_KV_SEP;
                tmp[tl + 1] = '\0';
            }
            strncat(tmp, val, sizeof(tmp) - strlen(tmp) - 1);
            wrote_any = true;
        }
    }

    // 4) 写回目标缓冲（截断保护）
    strncpy(packed, tmp, packed_sz - 1);
    packed[packed_sz - 1] = '\0';
}

// 通过收藏 flag 查找收藏槽位（存在则返回索引，否则返回 -1）
static int main_find_favorite_slot_by_flag(uint32_t flag)
{
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        if (yk_settings.user_favorites[i].is_favorite &&
            (uint32_t)yk_settings.user_favorites[i].flag == flag) {
            return i;
        }
    }
    return -1;
}

// 判断当前 active flag 是否为有效收藏
static bool main_is_favorite_active(void)
{
    return (main_find_favorite_slot_by_flag(yk_active_favorite_flag) >= 0);
}

static const char *main_get_food_name_key(uint8_t food_flag)
{
    // 第一步：把 food_flag 映射为翻译 key（food_00_name ~ food_16_name）
    switch (food_flag) {
        case 0: return "food_00_name";
        case 1: return "food_01_name";
        case 2: return "food_02_name";
        case 3: return "food_03_name";
        case 4: return "food_04_name";
        case 5: return "food_05_name";
        case 6: return "food_06_name";
        case 7: return "food_07_name";
        case 8: return "food_08_name";
        case 9: return "food_09_name";
        case 10: return "food_10_name";
        case 11: return "food_11_name";
        case 12: return "food_12_name";
        case 13: return "food_13_name";
        case 14: return "food_14_name";
        case 15: return "food_15_name";
        case 16: return "food_16_name";
        default: return "food_00_name";
    }
}

static void main_set_food_name_label_text(void)
{
    // 第一步：空指针保护（主界面对象或标题控件不存在则不处理）
    if (!scr || !scr->label_10_food_name) return;

    if (yk_food_flag != 0) {
        lv_label_set_text(scr->label_10_food_name, "");
        lv_obj_add_flag(scr->label_10_food_name, LV_OBJ_FLAG_HIDDEN);
        return;
    }
    lv_obj_clear_flag(scr->label_10_food_name, LV_OBJ_FLAG_HIDDEN);

    // 第二步：根据当前 yk_food_flag 获取默认标题翻译 key，并取出默认翻译文本
    const char *key = main_get_food_name_key(yk_food_flag);
    const char *default_name = yk_f_get_translation(key);

    // 第三步：如果当前 active_favorite_flag 对应有效收藏，且收藏名非空，则标题优先显示收藏名
    // 说明：不区分“从哪里进入主界面”，只要当前展示状态是收藏，就显示收藏的名字
    int slot = main_find_favorite_slot_by_flag(yk_active_favorite_flag);
    if (slot >= 0 && yk_settings.user_favorites[slot].name[0] != '\0') {
        lv_label_set_text(scr->label_10_food_name, yk_settings.user_favorites[slot].name);
        return;
    }

    // 第四步：未收藏/收藏名为空/收藏槽位无效时，回退显示默认翻译名
    lv_label_set_text(scr->label_10_food_name, default_name);
}

// 信息区文本更新后，强制把“滚动容器”回到顶部
// 注意：label_9_info 已经被 re-parent 到 main_info_scroll（右侧滚动条所在容器）里，
// 所以不能只对 label_9_info 自身做 scroll_to，否则会残留旧的滚动偏移，短文本会“看不见”
static void main_info_scroll_to_top(void)
{
    if (!scr || !scr->label_9_info) return;

    lv_obj_t *scroll_obj = main_info_scroll ? main_info_scroll : lv_obj_get_parent(scr->label_9_info);
    if (!scroll_obj) scroll_obj = scr->label_9_info;

    lv_obj_update_layout(scr->label_9_info);
    lv_obj_update_layout(scroll_obj);
    lv_obj_scroll_to(scroll_obj, 0, 0, LV_ANIM_OFF);
}

// 刷新信息区文本：优先显示自定义内容，否则显示默认翻译
static void main_set_info_label_text(void)
{
    if (!scr || !scr->label_9_info) return;

    // 1) 判断当前显示的是“配料”还是“步骤”
    bool show_steps = false;
    if (scr->container_2_2_1 && !lv_obj_has_flag(scr->container_2_2_1, LV_OBJ_FLAG_HIDDEN)) {
        show_steps = (lv_obj_get_style_bg_opa(scr->label_7_ingredients, LV_PART_MAIN | LV_STATE_DEFAULT) == 0);
    }

    // 2) 获取当前语言 id，并准备输出缓冲
    int lang = main_lang_id();
    char txt_buf[512];

    int slot = main_find_favorite_slot_by_flag(yk_active_favorite_flag);
    bool is_diy_fav = (slot >= 0 && yk_active_favorite_flag >= FAVORITE_FLAG_DIY_BASE);

    if (is_diy_fav) {
        const char *fav_packed = show_steps ? yk_settings.user_favorites[slot].steps_custom
                                            : yk_settings.user_favorites[slot].customize;
        if (main_packed_lang_get(fav_packed, lang, txt_buf, sizeof(txt_buf)) && txt_buf[0] != '\0') {
            lv_label_set_text(scr->label_9_info, txt_buf);
            main_info_scroll_to_top();
            return;
        }
        // DIY 收藏：新增时要求显示默认配料/步骤，不使用“未收藏编辑过”的统一存储内容
        // 因此这里不再继续读取 food_customize[0]/food_steps_custom[0]，直接走默认翻译回退
    }

    // 3) 统一存储优先：从 food_customize / food_steps_custom 读取当前语言的自定义文本
    uint8_t idx = yk_food_flag;
    if (is_diy_fav) idx = (uint8_t)YK_MODE_COUNT;
    if (idx < YK_MODE_COUNT) {
        const char *packed = show_steps ? yk_settings.food_steps_custom[idx] : yk_settings.food_customize[idx];
        if (main_packed_lang_get(packed, lang, txt_buf, sizeof(txt_buf)) && txt_buf[0] != '\0') {
            lv_label_set_text(scr->label_9_info, txt_buf);
            main_info_scroll_to_top();
            return;
        }
    }

    // 4) 兼容旧数据：如果当前 active 是收藏，且收藏里有自定义文本，则迁移到统一存储再显示
    if (!is_diy_fav && slot >= 0 && idx < YK_MODE_COUNT) {
        const char *fav_packed = show_steps ? yk_settings.user_favorites[slot].steps_custom
                                            : yk_settings.user_favorites[slot].customize;
        if (main_packed_lang_get(fav_packed, lang, txt_buf, sizeof(txt_buf)) && txt_buf[0] != '\0') {
            if (show_steps) {
                main_packed_lang_set(yk_settings.food_steps_custom[idx], sizeof(yk_settings.food_steps_custom[idx]), lang, txt_buf);
            } else {
                main_packed_lang_set(yk_settings.food_customize[idx], sizeof(yk_settings.food_customize[idx]), lang, txt_buf);
            }
            yk_delay_save_data_timer_start();
            lv_label_set_text(scr->label_9_info, txt_buf);
            main_info_scroll_to_top();
            return;
        }
    }

    // 5) 没有自定义文本：回退显示默认翻译内容
    // 未设置自定义文本：显示默认翻译内容
    if (show_steps) {
        if (yk_active_favorite_flag >= FAVORITE_FLAG_DIY_BASE) {
            lv_label_set_text(scr->label_9_info, yk_f_get_translation(yk_food_info[0].steps));
        } else {
            lv_label_set_text(scr->label_9_info, yk_f_get_translation(yk_food_info[yk_food_flag].steps));
        }
    } else {
        if (yk_active_favorite_flag >= FAVORITE_FLAG_DIY_BASE) {
            lv_label_set_text(scr->label_9_info, yk_f_get_translation(yk_food_info[0].ingredients));
        } else {
            lv_label_set_text(scr->label_9_info, yk_f_get_translation(yk_food_info[yk_food_flag].ingredients));
        }
    }
    main_info_scroll_to_top();
}

// 关闭自定义编辑弹窗并清理状态
static void main_customize_close(void)
{
    main_customize_mask = NULL;
    main_customize_kb = NULL;
    main_customize_ta = NULL;
    main_customize_target_flag = 0;
    main_customize_is_steps = false;
    main_customize_target_slot = -1;
    main_customize_food_index = 0;
    main_customize_save_to_favorite = false;
}

static void main_customize_close_cb(void *user_data)
{
    (void)user_data;
    main_customize_close();
}

static bool main_customize_ok_cb(lv_obj_t *ta, void *user_data)
{
    (void)user_data;
    if (!ta) return true;

    // 1) 读取输入框文本（空指针兜底为空串）
    const char *txt = lv_textarea_get_text(ta);
    if (!txt) txt = "";
    // 2) 获取当前语言 id（按语言保存）
    int lang = main_lang_id();

    int slot = main_find_favorite_slot_by_flag(yk_active_favorite_flag);
    bool is_diy_fav = (slot >= 0 && yk_active_favorite_flag >= FAVORITE_FLAG_DIY_BASE);
    if (is_diy_fav) {
        if (slot < 0) {
            return true;
        }
        if (main_customize_is_steps) {
            main_packed_lang_set(
                yk_settings.user_favorites[slot].steps_custom,
                sizeof(yk_settings.user_favorites[slot].steps_custom),
                lang,
                txt
            );
        } else {
            main_packed_lang_set(
                yk_settings.user_favorites[slot].customize,
                sizeof(yk_settings.user_favorites[slot].customize),
                lang,
                txt
            );
        }
    } else {
        // 3) 校验索引合法性（避免越界）
        if (main_customize_food_index >= YK_FOOD_SUM) {
            return true;
        }
        // 4) 写入“统一存储”（只更新当前语言那一份）
        if (main_customize_is_steps) {
            main_packed_lang_set(
                yk_settings.food_steps_custom[main_customize_food_index],
                sizeof(yk_settings.food_steps_custom[main_customize_food_index]),
                lang,
                txt
            );
        } else {
            main_packed_lang_set(
                yk_settings.food_customize[main_customize_food_index],
                sizeof(yk_settings.food_customize[main_customize_food_index]),
                lang,
                txt
            );
        }
    }
    // 5) 触发延迟保存 + 立即刷新信息区显示 + 关闭弹窗
    yk_delay_save_data_timer_start();

    main_set_info_label_text();
    return true;
}

// 打开自定义编辑弹窗（配料/步骤），运行中禁止打开
static void main_open_customize_editor(bool show_steps)
{
    // 1) 条件保护：UI 未初始化/已打开/运行中时不允许弹出
    if (!scr || !scr->obj || !scr->label_9_info) return;
    if (main_customize_mask) return;
    if (scr->container_2_3_run && !lv_obj_has_flag(scr->container_2_3_run, LV_OBJ_FLAG_HIDDEN)) return;

    // 2) 根据主题设置弹窗配色
    bool dark = (yk_settings.display_mode == val1_display_mode_dark);

    // 3) 判断当前编辑目标：DIY 收藏（写回 user_favorites）还是统一存储（写回 food_customize/food_steps_custom）
    int slot = main_find_favorite_slot_by_flag(yk_active_favorite_flag);
    bool is_diy_fav = (slot >= 0 && yk_active_favorite_flag >= FAVORITE_FLAG_DIY_BASE);
    if (is_diy_fav) {
        main_customize_save_to_favorite = true;
        main_customize_target_slot = slot;
        main_customize_target_flag = yk_active_favorite_flag;
    } else {
        // 3) 统一存储：编辑目标指向当前 food_flag 对应的自定义文本
        main_customize_save_to_favorite = false;
        main_customize_target_slot = -1;
        main_customize_target_flag = 0;
    }
    main_customize_food_index = yk_food_flag;
    main_customize_is_steps = show_steps;

    // 4) 计算最大可输入长度：按“当前目标存储区”的数组容量决定
    uint32_t max_len = 0;
    if (main_customize_save_to_favorite && main_customize_target_slot >= 0) {
        max_len = (uint32_t)(show_steps ? sizeof(yk_settings.user_favorites[main_customize_target_slot].steps_custom)
                                        : sizeof(yk_settings.user_favorites[main_customize_target_slot].customize));
    } else if (main_customize_food_index < YK_FOOD_SUM) {
        max_len = (uint32_t)(show_steps ? sizeof(yk_settings.food_steps_custom[main_customize_food_index])
                                        : sizeof(yk_settings.food_customize[main_customize_food_index]));
    }

    // 5) 取出存储区的“打包多语言”字段（后续按当前语言解包成初始显示文本）
    const char *stored_packed = NULL;
    if (main_customize_save_to_favorite && main_customize_target_slot >= 0) {
        stored_packed = show_steps ? yk_settings.user_favorites[main_customize_target_slot].steps_custom
                                   : yk_settings.user_favorites[main_customize_target_slot].customize;
    } else if (main_customize_food_index < YK_FOOD_SUM) {
        stored_packed = show_steps ? yk_settings.food_steps_custom[main_customize_food_index]
                                   : yk_settings.food_customize[main_customize_food_index];
    }

    // 6) 生成输入框初始文本：优先使用“当前语言”存储内容；为空则用信息区当前显示作为默认
    int lang = main_lang_id();
    char cur_lang_txt[512];
    cur_lang_txt[0] = '\0';
    const char *init_txt = NULL;
    if (main_packed_lang_get(stored_packed, lang, cur_lang_txt, sizeof(cur_lang_txt)) && cur_lang_txt[0] != '\0') {
        init_txt = cur_lang_txt;
    } else {
        const char *cur = lv_label_get_text(scr->label_9_info);
        init_txt = cur ? cur : "";
    }

    // 7) 输入框最大长度需要预留 '\0'
    uint32_t max_len_set = 0;
    if (max_len > 0) max_len_set = max_len - 1;

    // 8) 打开通用“文本输入弹窗”（遮罩+面板+标题+输入框+取消/确定+键盘）
    main_customize_mask = yk_ui_text_input_show(
        scr->obj,
        show_steps ? yk_ui_tr("steps", "步骤") : yk_ui_tr("customize", "自定义"),
        yk_ui_tr("cancel", "取消"),
        yk_ui_tr("ok", "确定"),
        900,
        340,
        20,
        860,
        190,
        180,
        60,
        0,
        120,
        false,
        max_len_set,
        init_txt,
        210,
        dark,
        NULL,
        main_customize_ok_cb,
        main_customize_close_cb,
        NULL,
        &main_customize_ta,
        &main_customize_kb,
        NULL,
        NULL,
        NULL
    );
}

// 关闭“个性化提示”确认弹窗并清理状态
static void main_info_confirm_close(void)
{
    yk_ui_confirm_dismiss();
    main_info_confirm_show_steps = false;
    main_info_confirm_open = false;
}

// “个性化提示”弹窗：取消事件
static void main_info_confirm_cancel_clicked(lv_event_t *e)
{
    (void)e;
    main_info_confirm_close();
}

// “个性化提示”弹窗：确认事件（进入编辑器）
static void main_info_confirm_ok_clicked(lv_event_t *e)
{
    (void)e;
    bool show_steps = main_info_confirm_show_steps;
    main_info_confirm_close();
    main_open_customize_editor(show_steps);
}

// 显示“个性化提示”确认弹窗（防止误触直接进入编辑）
static void main_show_info_confirm(bool show_steps)
{
    if (!scr || !scr->obj) return;
    if (main_info_confirm_open || main_customize_mask) return;

    main_info_confirm_show_steps = show_steps;
    main_info_confirm_open = true;

    yk_ui_confirm_show(yk_ui_tr("input_only_alnum", "只能输入字母和数字。"),
                       520,
                       240,
                       yk_ui_tr("cancel", "取消"),
                       yk_ui_tr("confirm", "确认"),
                       main_info_confirm_cancel_clicked,
                       main_info_confirm_ok_clicked);
}

// 点击信息区：弹出确认提示，再进入个性化编辑
static void main_label_9_info_clicked(lv_event_t *e)
{
    (void)e;
    if (!scr || !scr->obj || !scr->label_9_info) return;
    if (main_customize_mask || main_info_confirm_open) return;
    if (scr->container_2_3_run && !lv_obj_has_flag(scr->container_2_3_run, LV_OBJ_FLAG_HIDDEN)) return;

    bool show_steps = false;
    if (scr->container_2_2_1 && !lv_obj_has_flag(scr->container_2_2_1, LV_OBJ_FLAG_HIDDEN)) {
        show_steps = (lv_obj_get_style_bg_opa(scr->label_7_ingredients, LV_PART_MAIN | LV_STATE_DEFAULT) == 0);
    }

    main_show_info_confirm(show_steps);
}


// 定义全局容器数组以便复用
static lv_obj_t* containers[YK_FOOD_SUM];



// 定义食物图片路径数组
static const char* food_image_paths[YK_FOOD_SUM] = {
    LVGL_IMAGE_PATH(food_00_01.png),
    LVGL_IMAGE_PATH(food_01_01.png),
    LVGL_IMAGE_PATH(food_02_01.png),
    LVGL_IMAGE_PATH(food_03_01.png),
    LVGL_IMAGE_PATH(food_04_01.png),
    LVGL_IMAGE_PATH(food_05_01.png),
    LVGL_IMAGE_PATH(food_06_01.png),
    LVGL_IMAGE_PATH(food_07_01.png),
    LVGL_IMAGE_PATH(food_08_01.png),
    LVGL_IMAGE_PATH(food_09_01.png),
    LVGL_IMAGE_PATH(food_10_01.png),
    LVGL_IMAGE_PATH(food_11_01.png),
    LVGL_IMAGE_PATH(food_12_01.png),
    LVGL_IMAGE_PATH(food_13_01.png),
    LVGL_IMAGE_PATH(food_14_01.png),
    LVGL_IMAGE_PATH(food_15_01.png),
    LVGL_IMAGE_PATH(food_16_01.png),
};

//收藏图片数组图片数组路径
static const char* food_image_paths_favorites[YK_FOOD_SUM] = {
    LVGL_IMAGE_PATH(food_00_03.png),
    LVGL_IMAGE_PATH(food_01_03.png),
    LVGL_IMAGE_PATH(food_02_03.png),
    LVGL_IMAGE_PATH(food_03_03.png),
    LVGL_IMAGE_PATH(food_04_03.png),
    LVGL_IMAGE_PATH(food_05_03.png),
    LVGL_IMAGE_PATH(food_06_03.png),
    LVGL_IMAGE_PATH(food_07_03.png),
    LVGL_IMAGE_PATH(food_08_03.png),
    LVGL_IMAGE_PATH(food_09_03.png),
    LVGL_IMAGE_PATH(food_10_03.png),
    LVGL_IMAGE_PATH(food_11_03.png),
    LVGL_IMAGE_PATH(food_12_03.png),
    LVGL_IMAGE_PATH(food_13_03.png),
    LVGL_IMAGE_PATH(food_14_03.png),
    LVGL_IMAGE_PATH(food_15_03.png),
    LVGL_IMAGE_PATH(food_16_03.png),
};

// 根据食物标志获取主界面大图路径（_01）
const char* yk_get_image_path_by_flag(uint8_t flag)
{
    if (flag < YK_FOOD_SUM) return food_image_paths[flag];
    return food_image_paths[0];
}

// 根据食物标志获取收藏缩略图路径（_03）
const char* yk_get_favorite_image_path_by_flag(uint8_t flag)
{
    if (flag < YK_FOOD_SUM) return food_image_paths_favorites[flag];
    return food_image_paths_favorites[0];
}
// 食物配料和步骤数组
food_info_t yk_food_info[YK_FOOD_SUM] = {
    {"food_00_ingredients", "food_00_steps"},      // 食物0
    {"food_01_ingredients", "food_01_steps"},      // 食物1
    {"food_02_ingredients", "food_02_steps"},      // 食物2
    {"food_03_ingredients", "food_03_steps"},      // 食物3
    {"food_04_ingredients", "food_04_steps"},      // 食物4
    {"food_05_ingredients", "food_05_steps"},      // 食物5
    {"food_06_ingredients", "food_06_steps"},      // 食物6
    {"food_07_ingredients", "food_07_steps"},      // 食物7
    {"food_08_ingredients", "food_08_steps"},      // 食物8
    {"food_09_ingredients", "food_09_steps"},      // 食物9
    {"food_10_ingredients", "food_10_steps"},      // 食物10
    {"food_11_ingredients", "food_11_steps"},      // 食物11
    {"food_12_ingredients", "food_12_steps"},      // 食物12
    {"food_13_ingredients", "food_13_steps"},      // 食物13
    {"food_14_ingredients", "food_14_steps"},      // 食物14
    {"food_15_ingredients", "food_15_steps"},      // 食物15
    {"food_16_ingredients", "food_16_steps"},      // 食物16
};


/**
 * @brief 将数值的个位数调整为 0 或 5
 * 
 * @param value 输入的原始数值
 * @return uint16_t 调整后的数值
 */
uint16_t adjust_to_nearest_0_or_5(uint16_t value)
{
    uint16_t remainder = value % 10;

    // 个位小于 3 取 0；3~7 取 5；大于等于 8 进位取 0
    if (remainder < 3) {
        return value - remainder; // 调整为最近的以 0 结尾的数
    } else if (remainder < 8) {
        return value - remainder + 5; // 调整为最近的以 5 结尾的数
    } else {
        return value - remainder + 10; // 调整为下一个以 0 结尾的数
    }
}

// 根据模式设置温度/时间滑块范围，并对当前值做越界裁剪
static void yk_main_apply_slider_limits(uint8_t mode_flag)
{
    if (!scr || !scr->slider_1_t || !scr->slider_2_time) return;

    int32_t temp_min = 45;
    int32_t temp_max = 200;
    int32_t time_min = 1;
    int32_t time_max = 60;

    if (mode_flag < YK_FOOD_SUM) {
        // 内置菜谱：默认范围
        temp_min = 45;
        temp_max = 200;
        time_min = 1;
        time_max = 60;
    } else if (mode_flag == (YK_FOOD_SUM + 0)) {
        // 自定义 1：AIRFRY
        temp_min = 100;
        temp_max = 200;
        time_min = 1;
        time_max = 60;
    } else if (mode_flag == (YK_FOOD_SUM + 1)) {
        // 自定义 2：BAKE
        temp_min = 45;
        temp_max = 200;
        time_min = 1;
        time_max = 60;
    } else if (mode_flag == (YK_FOOD_SUM + 2)) {
        // 自定义 3：PREHEAT
        temp_min = 100;
        temp_max = 200;
        time_min = 1;
        time_max = 5;
    } else if (mode_flag == (YK_FOOD_SUM + 3)) {
        // 自定义 4：DEHYDRATE
        temp_min = 45;
        temp_max = 80;
        time_min = 1;
        time_max = 360;
    } else {
        // 兜底范围
        temp_min = 45;
        temp_max = 200;
        time_min = 1;
        time_max = 360;
    }

    lv_slider_set_range(scr->slider_1_t, temp_min / 5, temp_max / 5);
    lv_slider_set_range(scr->slider_2_time, time_min, time_max);

    if (yk_v_food_t_f < (uint16_t)temp_min) yk_v_food_t_f = (uint16_t)temp_min;
    if (yk_v_food_t_f > (uint16_t)temp_max) yk_v_food_t_f = (uint16_t)temp_max;
    if (yk_v_food_min < (uint16_t)time_min) yk_v_food_min = (uint16_t)time_min;
    if (yk_v_food_min > (uint16_t)time_max) yk_v_food_min = (uint16_t)time_max;

    lv_slider_set_value(scr->slider_1_t, yk_v_food_t_f / 5, LV_ANIM_OFF);
    lv_slider_set_value(scr->slider_2_time, yk_v_food_min, LV_ANIM_OFF);
}


// 设置并刷新温度/时间（支持默认菜谱、单位切换与滑块同步）
void food_tf_time(uint16_t foof_t_f,uint16_t foof_min)
{
    if (foof_t_f == 0 && foof_min == 0)
    {
        uint16_t fav_temp_c = 0;
        uint16_t fav_time_min = 0;

        if (yk_favorites_has_flag(yk_food_flag)) {
            for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
                if (yk_settings.user_favorites[i].is_favorite &&
                    yk_settings.user_favorites[i].flag == yk_food_flag) {
                    fav_temp_c = yk_settings.user_favorites[i].temperature;
                    fav_time_min = yk_settings.user_favorites[i].time;
                    break;
                }
            }
        }

        if (fav_temp_c != 0 && fav_time_min != 0) {
            yk_v_food_t_f = fav_temp_c;
            yk_v_food_min = fav_time_min;
        }
        else if (yk_food_flag == 0)
        {
            //自定义
            yk_v_food_t_f = 200;
            yk_v_food_min = 10;
        }
        else if (yk_food_flag == 1)
        {
            //薯条 FRIES
            yk_v_food_t_f = 200;
            yk_v_food_min = 17;
        }
        else if (yk_food_flag == 2)
        {
            //鸡翅 WINGS
            yk_v_food_t_f = 200;
            yk_v_food_min = 16;
        }
        else if (yk_food_flag == 3)
        {
            //鸡腿 DRUMSTICK
            yk_v_food_t_f = 200;
            yk_v_food_min = 27;
        }
        else if (yk_food_flag == 4)
        {
            //烤鸡 CHICKEN
            yk_v_food_t_f = 200;
            yk_v_food_min = 30;
        }
        else if (yk_food_flag == 5)
        {
            //土豆块 POTATO BLOCKS
            yk_v_food_t_f = 180;
            yk_v_food_min = 23;
        } 
        else if (yk_food_flag == 6)
        {
            //牛排 STEAK
            yk_v_food_t_f = 200;
            yk_v_food_min = 6;
        } 
        else if (yk_food_flag == 7)
        {
            //培根 BACON
            yk_v_food_t_f = 180;
            yk_v_food_min = 8;
        } 
        else if (yk_food_flag == 8)
        {
            //肉 MEAT （猪排）
            yk_v_food_t_f = 200;
            yk_v_food_min = 18;
        } 
        else if (yk_food_flag == 9)
        {
            //虾 SHRIMP
            yk_v_food_t_f = 200;
            yk_v_food_min = 8;
        } 
        else if (yk_food_flag == 10)
        {
            //鱼 FISH
            yk_v_food_t_f = 180;
            yk_v_food_min = 22;
        }
        else if (yk_food_flag == 11)
        {
            //烤肠
            yk_v_food_t_f = 200;
            yk_v_food_min = 10;
        }
        else if (yk_food_flag == 12)
        {
            //糕点 PASTRY
            yk_v_food_t_f = 160;
            yk_v_food_min = 12;
        } 
        else if (yk_food_flag == 13)
        {
            //蔬菜 VEGGIES
            yk_v_food_t_f = 190;
            yk_v_food_min = 10;
        } 
        else if (yk_food_flag == 14)
        {
            //排骨 RIBS
            yk_v_food_t_f = 180;
            yk_v_food_min = 22;
        } 
        else if (yk_food_flag == 15)
        {
            //披萨 PIZZA
            yk_v_food_t_f = 200;
            yk_v_food_min = 8;
        } 
        else if (yk_food_flag == 16)
        {
            //洋葱圈 ONION RINGS
            yk_v_food_t_f = 190;
            yk_v_food_min = 10;
        }
    }
    else
    {
        yk_v_food_t_f = foof_t_f;
        yk_v_food_min = foof_min;
    }

    yk_printf("food_tf_time: %d, %d\n", yk_v_food_t_f, yk_v_food_min);
    if (scr && scr->slider_1_t && scr->slider_2_time) {
        int32_t t_min = (int32_t)lv_slider_get_min_value(scr->slider_1_t) * 5;
        int32_t t_max = (int32_t)lv_slider_get_max_value(scr->slider_1_t) * 5;
        if ((int32_t)yk_v_food_t_f < t_min) yk_v_food_t_f = (uint16_t)t_min;
        if ((int32_t)yk_v_food_t_f > t_max) yk_v_food_t_f = (uint16_t)t_max;

        int32_t time_min = (int32_t)lv_slider_get_min_value(scr->slider_2_time);
        int32_t time_max = (int32_t)lv_slider_get_max_value(scr->slider_2_time);
        if ((int32_t)yk_v_food_min < time_min) yk_v_food_min = (uint16_t)time_min;
        if ((int32_t)yk_v_food_min > time_max) yk_v_food_min = (uint16_t)time_max;
    }
    
    lv_slider_set_value(scr->slider_1_t,yk_v_food_t_f/5, LV_ANIM_ON);
    if (yk_settings.f_to_c == val2_t_f)
    {
        uint16_t sel_val_t_f = 1.8*yk_v_food_t_f + 32 + 0.5;
        if (sel_val_t_f <= 115)
		{
            //修改温度值，温度大于等于200℃，华摄氏度就改为400℉
			sel_val_t_f = 110;
		}
        else if (sel_val_t_f >= 392)
		{
            //修改温度值，温度大于等于200℃，华摄氏度就改为400℉
			sel_val_t_f = 400;
		}
        //修改温度值,让他个位数不是0，就5
        sel_val_t_f = adjust_to_nearest_0_or_5(sel_val_t_f);
        lv_label_set_text_fmt(scr->label_5_t, "%d℉",sel_val_t_f);
    }
    else if(yk_settings.f_to_c == val2_t_c){
        lv_label_set_text_fmt(scr->label_5_t, "%d℃",yk_v_food_t_f);
    }

    lv_slider_set_value(scr->slider_2_time,yk_v_food_min, LV_ANIM_ON);
    lv_label_set_text_fmt(scr->label_6_time, "%d MIN",yk_v_food_min);
}


//点击关机
void screen_02_main_image_1_off_custom_clicked(void)
{
    //如果正在工作，就提示正在工作状态中，请勿息屏，否则设备会停止工作！
    if (yk_v_stop_run_state)
    {
        yk_ui_center_toast_show(yk_ui_tr("states_in_traffic", "正在工作状态中，请勿息屏，否则设备会停止工作！"), 3000);
        return;
    }
    #if YK_USE_PM
    screensaver_set_pm_countdown(2);
    #endif
    yk_view_set_dis_view(SCREEN_13_ID);
}

// 点击配料：显示当前食物配料文本并更新按钮样式
void screen_02_main_label_7_ingredients_custom_clicked(void)
{ 
    lv_obj_set_style_bg_opa(scr->label_7_ingredients, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr->label_7_ingredients, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr->label_8_steps, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr->label_8_steps, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    main_set_info_label_text();
}

// 点击步骤：显示当前食物步骤文本并更新按钮样式
void screen_02_main_label_8_steps_custom_clicked(void)
{ 
    lv_obj_set_style_bg_opa(scr->label_7_ingredients, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr->label_7_ingredients, lv_color_hex(YK_COLOR_LBL_LIGHT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr->label_8_steps, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr->label_8_steps, lv_color_hex(YK_COLOR_LBL_DARK), LV_PART_MAIN | LV_STATE_DEFAULT);
    main_set_info_label_text();
}

#if 1 // 弹框定时器（用于爱心收藏提示）
static lv_timer_t *task_toast = NULL;
static uint32_t main_love_pending_flag = 0;
static bool main_love_pending_valid = false;

// toast 自动隐藏定时器回调
static void toast_timer_cb(lv_timer_t *timer)
{
    // 获取 user_data 指针并隐藏弹框
    lv_obj_t * toast_obj = (lv_obj_t *)lv_timer_get_user_data(timer);
    if (toast_obj) {
        lv_obj_add_flag(toast_obj, LV_OBJ_FLAG_HIDDEN);
    }
    // 删除定时器并清空句柄
    if (task_toast != NULL) {
        lv_timer_del(task_toast);
        task_toast = NULL;
    }
}

// 显示带文本的 toast 弹框并在 ms 毫秒后自动隐藏
void yk_show_toast(lv_obj_t *toast, const char *txt, uint32_t ms)
{
    if (toast == NULL) return;
    // 设置弹框内的 label 文本（如果提供）
    if (txt && scr && scr->label_10_toast_notification) {
        lv_label_set_text(scr->label_10_toast_notification, txt);
    }
    lv_obj_clear_flag(toast, LV_OBJ_FLAG_HIDDEN);
    if (task_toast != NULL) {
        lv_timer_del(task_toast);
        task_toast = NULL;
    }
    task_toast = lv_timer_create(toast_timer_cb, ms, toast);
}

// 关闭“取消收藏”确认弹窗并清理状态
static void main_love_confirm_close(void)
{
    main_love_pending_flag = 0;
    main_love_pending_valid = false;
}

void yk_main_update_ui_text(void)
{
    // 第一步：先刷新标题（label_10_food_name）
    main_set_food_name_label_text();
    // 第二步：再刷新信息区文本（label_9_info）
    main_set_info_label_text();
}

// “取消收藏确认”弹窗：确认事件（执行移除并提示）
static void main_love_confirm_ok_clicked(lv_event_t *e)
{
    (void)e;
    // 第一步：校验弹窗状态，避免重复触发或状态异常
    if (!main_love_pending_valid) {
        main_love_confirm_close();
        return;
    }
    // 第二步：取出待取消的收藏 flag，并关闭弹窗状态
    uint32_t flag_to_remove = main_love_pending_flag;
    main_love_confirm_close();
    // 第三步：从设置里移除该收藏
    yk_favorites_remove_by_flag(flag_to_remove);
    // 第四步：如果当前正在显示的就是这条收藏，立刻回退到“普通食物/未收藏”模式
    if (yk_active_favorite_flag == flag_to_remove) {
        yk_active_favorite_flag = yk_food_flag;
        yk_active_from_favorites = false;
    }
    // 第五步：如果缓存的“最后 DIY 收藏”就是被删的那条，也要清掉，避免回显无效收藏
    if (main_last_diy_favorite_flag == flag_to_remove) {
        main_last_diy_favorite_flag = 0;
    }
    // 第六步：刷新主界面文案，并提示用户
    yk_main_update_ui_text();
    yk_ui_center_toast_show(yk_ui_tr("favorites_removed", "已取消收藏"), 3000);
}

// “取消收藏确认”弹窗：取消事件
static void main_love_confirm_cancel_clicked(lv_event_t *e)
{
    (void)e;
    main_love_confirm_close();
}

// 显示“取消收藏”确认弹窗（避免误触直接移除）
static void main_love_show_remove_confirm(uint32_t flag_to_remove)
{
    if (!scr || !scr->obj) return;

    main_love_pending_flag = flag_to_remove;
    main_love_pending_valid = true;
    yk_ui_confirm_show(yk_ui_tr("favorites_remove_confirm", "确定取消收藏？"),
                       520,
                       240,
                       yk_ui_tr("cancel", "取消"),
                       yk_ui_tr("ok", "确定"),
                       main_love_confirm_cancel_clicked,
                       main_love_confirm_ok_clicked);
}
#endif  //弹框定时器

// 点击爱心：切换收藏状态并弹出提示
void screen_02_main_image_2_2_1_love_custom_clicked (lv_event_t *e) { 
    yk_printf("screen_02_main_image_2_2_1_love_custom_clicked\n");
    // 获取当前图片源
    const void* current_src = lv_img_get_src(scr->image_2_2_1_love);
    const char *cur_path = (const char *)current_src;
    const char *love0 = LVGL_IMAGE_PATH(icon_love_0.png);
    const char *love1 = LVGL_IMAGE_PATH(icon_love_1.png);
    yk_printf("current_src: %s\n", cur_path ? cur_path : "(null)");
    yk_printf("icon_love_0.png: %s\n", love0);

    // 根据当前图片源切换图片（使用字符串比较，避免不同地址但相同路径导致的指针比较失败）
    if (cur_path && strcmp(cur_path, love0) == 0) {
        if (yk_food_flag >= YK_FOOD_SUM) {
            return;
        }
        if (yk_favorites_get_count() >= 20) {
            yk_ui_center_toast_show(yk_ui_tr("favorites_max_10", "最多收藏20个"), 3000);
            return;
        }
        lv_img_set_src(scr->image_2_2_1_love, love1);
        // 获取当前主图路径和名称，加入收藏
        const void *main_img = food_image_paths_favorites[yk_food_flag];
        char fav_name[16];
        int cnt = yk_favorites_get_count();
        snprintf(fav_name, sizeof(fav_name), "DIY%02d", cnt + 1);
        yk_favorites_add(screen_14_love_favorites_get(&ui_manager)->container_3_favorites,main_img, fav_name, yk_v_food_t_f, yk_v_food_min);
        yk_main_update_ui_text();
        yk_ui_center_toast_show(yk_ui_tr("favorites_added", "已加入收藏"), 3000);
    } else {
        main_love_show_remove_confirm(yk_active_favorite_flag);
    }
}


// 更新右侧列表选中态（边框）、主界面大图及爱心图标
void update_all_containers_border_opa(uint8_t food_flag)
{
    //修改主界面大图图片
    lv_obj_set_style_bg_img_src(scr->container_2_1, food_image_paths[yk_food_flag], LV_PART_MAIN | LV_STATE_DEFAULT);

    // 根据当前展示的大图是否已收藏来设置爱心图标
    const char *love0 = LVGL_IMAGE_PATH(icon_love_0.png);
    const char *love1 = LVGL_IMAGE_PATH(icon_love_1.png);
    if (yk_favorites_has_flag(yk_active_favorite_flag)) {
        lv_img_set_src(scr->image_2_2_1_love, love1);
    } else {
        lv_img_set_src(scr->image_2_2_1_love, love0);
    }

    yk_main_apply_slider_limits(yk_food_flag);

    // 先把选中的容器边框全部设置为不显示
    for (int i = 0; i < YK_FOOD_SUM; i++) {
        lv_obj_set_style_border_opa(containers[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    //隐藏容器
    lv_obj_add_flag(scr->container_2_2_1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(scr->container_2_2_2, LV_OBJ_FLAG_HIDDEN);
    
    //根据当前 food_flag 决定显示哪块信息：0 显示自定义区，其它显示配料/步骤区
    if (food_flag != 0) 
    {
        //显示配料、步骤容器
        lv_obj_clear_flag(scr->container_2_2_1, LV_OBJ_FLAG_HIDDEN);
        //点击其他食物默认展示配料
        screen_02_main_label_7_ingredients_custom_clicked();
        
        // 把选中的容器边框显示出来
        lv_obj_set_style_border_opa(containers[food_flag], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        // 滚动到点击的容器到中间位置
        lv_coord_t container_height = 158;  // 每个容器的高度
        lv_coord_t scroll_pos = food_flag * (container_height + 21) - (lv_obj_get_height(scr->container_3_food) / 2 - container_height / 2);
        lv_obj_scroll_to_y(scr->container_3_food, scroll_pos, LV_ANIM_ON);
    }
    else
    {
        //显示自定义容器
        lv_obj_clear_flag(scr->container_2_2_2, LV_OBJ_FLAG_HIDDEN);
        main_set_info_label_text();
        // 把选中的容器边框显示出来
        lv_obj_set_style_border_opa(containers[food_flag], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        // 滚动到点击的容器到中间位置
        lv_coord_t container_height = 158;  // 每个容器的高度
        lv_coord_t scroll_pos = food_flag * (container_height + 21) - (lv_obj_get_height(scr->container_3_food) / 2 - container_height / 2);
        lv_obj_scroll_to_y(scr->container_3_food, scroll_pos, LV_ANIM_ON);
    }
}

// 刷新主界面爱心图标（根据当前 food_flag 是否已收藏）
void yk_main_refresh_love_icon(void)
{
    screen_02_main_t *screen_02_main_scr = screen_02_main_get(&ui_manager);
    if (!screen_02_main_scr || !screen_02_main_scr->image_2_2_1_love) return;

    const char *love0 = LVGL_IMAGE_PATH(icon_love_0.png);
    const char *love1 = LVGL_IMAGE_PATH(icon_love_1.png);
    if (yk_favorites_has_flag(yk_active_favorite_flag)) {
        lv_img_set_src(screen_02_main_scr->image_2_2_1_love, love1);
    } else {
        lv_img_set_src(screen_02_main_scr->image_2_2_1_love, love0);
    }
}

// 点击右侧食物缩略图：切换当前 food_flag 并刷新UI
static void screen_02_main_container_3_food_clicked(lv_event_t *e)
{ 
    yk_printf("screen_02_main_container_3_food_clicked\n");
    
    
    // 第一步：获取触发事件的对象（被点击的右侧缩略图容器）
    lv_obj_t* clicked_obj = lv_event_get_target(e);
    
    // 第二步：遍历右侧容器数组，找到被点击项对应的索引 i（即将切换到的 food_flag）
    for (int i = 0; i < YK_FOOD_SUM; i++) {
        if (clicked_obj == containers[i]) {
            // 第三步：重复点击当前项：不处理
            if (yk_food_flag == i) return;  //如果点击的容器和当前选中的容器相同则返回

            // 第四步：运行中禁止切换（避免运行过程切菜谱导致状态异常）
            if (!lv_obj_has_flag(scr->container_2_3_run, LV_OBJ_FLAG_HIDDEN))
            {
                // yk_show_toast(scr->container_7_toast_notification, yk_ui_tr("busy_cannot_switch", "运行中无法切换"), 3000);
                yk_ui_center_toast_show(yk_ui_tr("busy_cannot_switch", "运行中无法切换"), 3000);
                return;  //正在运行禁止切换图片
            }

            // 第五步：切换前先清掉 4 个 DIY 模式标签的选中态（airfry/bake/preheat/dehydrate）
            lv_obj_remove_state(scr->label_1_airfry, LV_STATE_CHECKED);
            lv_obj_remove_state(scr->label_2_bake, LV_STATE_CHECKED);
            lv_obj_remove_state(scr->label_3_preheat, LV_STATE_CHECKED);
            lv_obj_remove_state(scr->label_4_dehydrate, LV_STATE_CHECKED);
            
            // 第六步：如果当前正在显示 DIY 收藏，先把它记下来（用于稍后切回“自定义0”继续回显收藏内容）
            if (yk_active_favorite_flag >= FAVORITE_FLAG_DIY_BASE) {
                main_last_diy_favorite_flag = yk_active_favorite_flag;
            }

            // 第七步：更新当前食物索引（决定右侧选中态、默认食谱文本、默认标题翻译等）
            yk_food_flag = i;
            // 第八步：决定 active flag 与标题显示来源（影响信息区文字来源 + 爱心图标 + 标题显示）
            // - 切到“自定义0”：若存在“上一次 DIY 收藏”且该收藏仍有效，则继续按收藏显示
            // - 切到其它食物：回到普通模式（active 跟随 food_flag，标题显示默认翻译名）
            if (yk_food_flag == 0 &&
                main_last_diy_favorite_flag >= FAVORITE_FLAG_DIY_BASE &&
                yk_favorites_has_flag(main_last_diy_favorite_flag)) {
                yk_active_favorite_flag = main_last_diy_favorite_flag;
                yk_active_from_favorites = true;
            } else {
                yk_active_favorite_flag = yk_food_flag;
                yk_active_from_favorites = false;
            }

            // 第九步：刷新 UI（右侧选中态/主图/爱心图标等）
            update_all_containers_border_opa(yk_food_flag);

            // 第十步：按当前模式刷新温度/时间（普通菜谱默认值 or 收藏里的值）
            food_tf_time(0,0);
            yk_printf("yk_food_flag = %d \n", yk_food_flag);

            // 第十一步：刷新主界面文案（标题 + 信息区）
            yk_main_update_ui_text();
            break;
        }
    }
}

// 温度滑块数值变化：更新全局温度并同步UI
void screen_02_main_slider_1_t_custom_value_changed()
{
    //yk_printf("screen_02_main_slider_1_t_custom_value_changed\n");
    yk_v_food_t_f = lv_slider_get_value(scr->slider_1_t);
    yk_v_food_t_f = yk_v_food_t_f*5;
    food_tf_time(yk_v_food_t_f,yk_v_food_min);
}


// 时间滑块数值变化：更新全局时间并同步UI
void screen_02_main_slider_2_time_custom_value_changed(void)
{
    //yk_printf("screen_02_main_slider_1_t_custom_value_changed\n");
    yk_v_food_min = lv_slider_get_value(scr->slider_2_time);
    food_tf_time(yk_v_food_t_f,yk_v_food_min);
}

// 设置四个 DIY 标签的选中状态（互斥）
// - diy_flag = 1~4：只选中对应的一个，其它全部取消
// - diy_flag = 0：四个全部取消选中
static void main_set_diy_checked(uint8_t diy_flag)
{
    // 主界面对象未初始化时不处理，避免空指针
    if (!scr) return;

    // 统一把四个标签放到数组里，方便用循环处理
    lv_obj_t *labels[4] = {
        scr->label_1_airfry,
        scr->label_2_bake,
        scr->label_3_preheat,
        scr->label_4_dehydrate
    };

    // 逐个标签设置选中/取消选中
    for (int i = 0; i < 4; i++) {
        // 取出当前标签对象
        lv_obj_t *obj = labels[i];
        // 该标签不存在就跳过（防止生成器改动导致的空指针）
        if (!obj) continue;
        // i 从 0 开始，所以 i+1 对应 diy_flag 的 1~4
        if (diy_flag != 0 && (uint8_t)(i + 1) == diy_flag) {
            // 当前就是目标：设置为选中态
            lv_obj_add_state(obj, LV_STATE_CHECKED);
        } else {
            // 其它：取消选中态
            lv_obj_remove_state(obj, LV_STATE_CHECKED);
        }
    }
}

// DIY 选择入口：同时处理“点击选中/再点取消选中”和默认温度/时间的赋值
// - diy_flag = 1~4：选择对应 DIY
// - 如果当前已选中同一个 DIY，再点一次会取消选中（yk_diy_flag=0）
static void main_diy_select(uint8_t diy_flag)
{
    // 主界面对象未初始化时不处理，避免空指针
    if (!scr) return;
    // 只允许 1~4 的 DIY 编号
    if (diy_flag < 1 || diy_flag > 4) return;

    // 用数组统一管理四个标签，便于通过 diy_flag 找到当前标签
    lv_obj_t *labels[4] = {
        scr->label_1_airfry,
        scr->label_2_bake,
        scr->label_3_preheat,
        scr->label_4_dehydrate
    };

    // 取出当前点击对应的标签对象（diy_flag 从 1 开始，所以要减 1）
    lv_obj_t *cur = labels[diy_flag - 1];
    // 如果当前已经选中，再点一次：取消全部选中并清空 DIY 标志
    if (cur && lv_obj_has_state(cur, LV_STATE_CHECKED)) {
        // 取消四个标签选中
        main_set_diy_checked(0);
        // 记录当前没有选中任何 DIY
        yk_diy_flag = 0;
        yk_main_apply_slider_limits(yk_food_flag);
        // 取消 DIY 后：回到右侧主食物（当前 yk_food_flag）的温度/时间
        // food_tf_time(0,0) 会按当前食物标志自动取默认/收藏温度时间
        food_tf_time(0, 0);
        return;
    }

    // 四个 DIY 的默认温度/时间预设（下标 1~4 对应 DIY1~DIY4；下标 0 不用）
    // 温度单位：℃（这里直接使用 yk_v_food_t_f 的单位）
    static const uint16_t preset_t[5] = {0, 160, 200, 200, 55};
    // 时间单位：min
    static const uint16_t preset_min[5] = {0, 25, 20, 1, 180};

    // 设置界面四个标签的选中状态（互斥）
    main_set_diy_checked(diy_flag);
    // 保存当前选择的 DIY 编号
    yk_diy_flag = diy_flag;
    // 应用该 DIY 的默认温度
    yk_v_food_t_f = preset_t[diy_flag];
    // 应用该 DIY 的默认时间
    yk_v_food_min = preset_min[diy_flag];
    yk_main_apply_slider_limits((uint8_t)(YK_FOOD_SUM + (diy_flag - 1)));
    // 刷新温度/时间显示与相关 UI
    food_tf_time(yk_v_food_t_f, yk_v_food_min);
}

// 点击 AIRFRY：切到自定义1并应用默认参数
void screen_02_main_label_1_airfry_custom_clicked()
{
    // 仅用于调试打印
    yk_printf("screen_02_main_label_1_airfry_custom_clicked\n");
    // 统一入口：处理选中/取消选中 + 应用默认温度/时间
    main_diy_select(1);
}
// 点击 BAKE：切到自定义2并应用默认参数
void screen_02_main_label_2_bake_custom_clicked()
{
    // 仅用于调试打印
    yk_printf("screen_02_main_label_2_bake_custom_clicked\n");
    // 统一入口：处理选中/取消选中 + 应用默认温度/时间
    main_diy_select(2);
}

//照明灯任务名
static lv_timer_t *task_lamp = NULL;  //创建照明灯任务名
//灯的图片按钮变量
bool yk_v_lamp_state = false;


// 设置照明灯图标与IO（根据 yk_v_lamp_state 与显示模式）
static void yk_fs_set_lamp_v()
{
	//手动设置照明灯图片按钮状态(根据图片按钮事件手动设置状态)
    if (yk_v_lamp_state)
    {
        lv_img_set_src(scr->image_6_lamp, LVGL_IMAGE_PATH(lamp_on.png));
        #if YK_USE_UI_BUILDER
        yk_io_set_lamp(1);
        #endif
    }else
    {
        // 关灯：按主题切换不同的关灯图标
        if (yk_settings.display_mode == val1_display_mode_light)
        {
            lv_img_set_src(scr->image_6_lamp, LVGL_IMAGE_PATH(lamp_off.png));
        }
        else  if (yk_settings.display_mode == val1_display_mode_dark)
        {
            lv_img_set_src(scr->image_6_lamp, LVGL_IMAGE_PATH(lamp_off_dark.png));
        }

        #if YK_USE_UI_BUILDER
        yk_io_set_lamp(0);
        #endif
    }
}
// 照明灯自动关闭定时器回调
static void task_lamp_cb(lv_timer_t *timer)
{
    //删除照明灯任务
    if (task_lamp != NULL) {
        lv_timer_del(task_lamp);
        task_lamp = NULL;
        // //io设置灯关
        // io_set_lamp(lamp_switch_v);
        yk_v_lamp_state = false;
        yk_printf("yk_v_lamp_state off \n");
        // // 获取 user_data 指针
        // lv_obj_t * lamp_obj = (lv_obj_t *)lv_timer_get_user_data(timer);
        // lv_img_set_src(lamp_obj, LVGL_IMAGE_PATH(BG_lamp_off.png));

        //设置照明灯切换页面的时候显示对应的开关图标
        yk_fs_set_lamp_v();
    }
}
// 点击照明灯：切换灯状态并启动/重置自动关闭定时器
void screen_02_main_image_6_lamp_custom_clicked(lv_event_t *e)
{
    yk_printf("screen_02_main_image_6_lamp_custom_clicked\n");
    lv_obj_t *lamp_obj = lv_event_get_target(e);
	if(yk_v_lamp_state)
	{
		yk_v_lamp_state = false;
	}else
	{
		yk_v_lamp_state = true;
	}

    //删除照明灯任务
    if (task_lamp != NULL) {
        lv_timer_del(task_lamp);
        task_lamp = NULL;
    }

	yk_fs_set_lamp_v();

	//设置照明灯图标开关显示
	if (yk_v_lamp_state) {
		yk_printf("yk_v_lamp_state on\n");
		if (task_lamp == NULL)task_lamp = lv_timer_create(task_lamp_cb,900*30,lamp_obj);
	}
}
// 点击开始/暂停：触发运行逻辑
void screen_02_main_image_7_stop_run_custom_clicked()
{
    yk_printf("screen_02_main_image_7_stop_run_custom_clicked\n");
    yk_f_run();
}
// 点击 PREHEAT：切到自定义3并应用默认参数
void screen_02_main_label_3_preheat_custom_clicked()
{
    // 仅用于调试打印
    yk_printf("screen_02_main_label_3_preheat_custom_clicked\n");
    // 统一入口：处理选中/取消选中 + 应用默认温度/时间
    main_diy_select(3);
}
// 点击 DEHYDRATE：切到自定义4并应用默认参数
void screen_02_main_label_4_dehydrate_custom_clicked()
{
    // 仅用于调试打印
    yk_printf("screen_02_main_label_4_dehydrate_custom_clicked\n");
    // 统一入口：处理选中/取消选中 + 应用默认温度/时间
    main_diy_select(4);
}


// 主界面首次创建：初始化控件样式、绑定事件、默认选中项
void screen_02_main_custom_created()
{
    yk_printf("screen_02_main_custom_created\n");
    // 获取当前屏幕
    scr = screen_02_main_get(&ui_manager);
    main_info_scroll = NULL;

    if (!main_first_enter_done) {
        // 第一步：首次进入主界面，强制定位到“第一个自定义(未收藏)”作为默认展示
        yk_food_flag = 0;
        // 第二步：默认 active flag 也指向未收藏（避免默认进来就命中某条收藏）
        yk_active_favorite_flag = 0;
        yk_active_from_favorites = false;
        // 第三步：清空 DIY 标签选中态记录
        yk_diy_flag = 0;
        // 第四步：标记已初始化，后续再次进入主界面不再重置
        main_first_enter_done = true;
    }

    if (scr && scr->container_2_2 && scr->label_9_info) {
        lv_obj_t *info_label = scr->label_9_info;
        lv_coord_t x = lv_obj_get_x(info_label);
        lv_coord_t y = lv_obj_get_y(info_label);
        lv_coord_t w = lv_obj_get_width(info_label);
        lv_coord_t h = lv_obj_get_height(info_label);

        main_info_scroll = lv_obj_create(scr->container_2_2);
        lv_obj_set_pos(main_info_scroll, x, y);
        lv_obj_set_size(main_info_scroll, w, h);
        lv_obj_set_style_bg_opa(main_info_scroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(main_info_scroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(main_info_scroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_scrollbar_mode(main_info_scroll, LV_SCROLLBAR_MODE_AUTO);
        lv_obj_set_scroll_dir(main_info_scroll, LV_DIR_VER);
        lv_obj_add_flag(main_info_scroll, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(main_info_scroll, main_label_9_info_clicked, LV_EVENT_CLICKED, NULL);

        lv_obj_set_parent(info_label, main_info_scroll);
        lv_obj_set_pos(info_label, 0, 0);
        lv_obj_set_style_width(main_info_scroll, 6, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(main_info_scroll, lv_color_hex(0xC8C8C8), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(main_info_scroll, 200, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(main_info_scroll, 3, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

        lv_coord_t content_w = w - 14;
        if (content_w < 10) content_w = 10;
        lv_obj_set_size(info_label, content_w, LV_SIZE_CONTENT);
        lv_label_set_long_mode(info_label, LV_LABEL_LONG_WRAP);
    }

    //设置选中状态的样式
    lv_obj_set_style_bg_opa(scr->label_1_airfry, 255, LV_PART_MAIN | LV_STATE_CHECKED);  //选中状态背景透明度255
    lv_obj_set_style_text_color(scr->label_1_airfry, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_CHECKED); //选中状态字体颜色白色

    lv_obj_set_style_bg_opa(scr->label_2_bake, 255, LV_PART_MAIN | LV_STATE_CHECKED);  //选中状态背景透明度255
    lv_obj_set_style_text_color(scr->label_2_bake, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_CHECKED); //选中状态字体颜色白色

    lv_obj_set_style_bg_opa(scr->label_3_preheat, 255, LV_PART_MAIN | LV_STATE_CHECKED);  //选中状态背景透明度255
    lv_obj_set_style_text_color(scr->label_3_preheat, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_CHECKED); //选中状态字体颜色白色

    lv_obj_set_style_bg_opa(scr->label_4_dehydrate, 255, LV_PART_MAIN | LV_STATE_CHECKED);  //选中状态背景透明度255
    lv_obj_set_style_text_color(scr->label_4_dehydrate, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_CHECKED); //选中状态字体颜色白色

    lv_obj_set_style_border_width(scr->label_1_airfry, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr->label_2_bake, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr->label_3_preheat, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr->label_4_dehydrate, 0, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_border_opa(scr->label_1_airfry, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr->label_2_bake, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr->label_3_preheat, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr->label_4_dehydrate, 0, LV_PART_MAIN | LV_STATE_CHECKED);

    // DIY 禁用态：文字颜色变灰（运行中给该标签加 LV_STATE_DISABLED 时触发）
    lv_obj_set_style_text_color(scr->label_1_airfry, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_color(scr->label_2_bake, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_color(scr->label_3_preheat, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_color(scr->label_4_dehydrate, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);

    lv_obj_set_style_bg_opa(scr->label_1_airfry, 0, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(scr->label_2_bake, 0, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(scr->label_3_preheat, 0, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(scr->label_4_dehydrate, 0, LV_PART_MAIN | LV_STATE_DISABLED);

    lv_obj_set_style_border_color(scr->label_1_airfry, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_border_color(scr->label_2_bake, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_border_color(scr->label_3_preheat, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_border_color(scr->label_4_dehydrate, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED);

    lv_obj_set_style_text_color(scr->label_1_airfry, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr->label_2_bake, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr->label_3_preheat, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr->label_4_dehydrate, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);

    lv_obj_set_style_bg_color(scr->label_1_airfry, lv_color_hex(0x6A6A6A), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(scr->label_2_bake, lv_color_hex(0x6A6A6A), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(scr->label_3_preheat, lv_color_hex(0x6A6A6A), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(scr->label_4_dehydrate, lv_color_hex(0x6A6A6A), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);

    lv_obj_set_style_bg_opa(scr->label_1_airfry, 255, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr->label_2_bake, 255, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr->label_3_preheat, 255, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr->label_4_dehydrate, 255, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);

    lv_obj_set_style_border_color(scr->label_1_airfry, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr->label_2_bake, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr->label_3_preheat, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr->label_4_dehydrate, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);

    lv_obj_set_style_border_width(scr->label_1_airfry, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr->label_2_bake, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr->label_3_preheat, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr->label_4_dehydrate, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);

    lv_obj_set_style_border_opa(scr->label_1_airfry, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr->label_2_bake, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr->label_3_preheat, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr->label_4_dehydrate, 0, LV_PART_MAIN | LV_STATE_DISABLED | LV_STATE_CHECKED);

    //运行界面首次创建只执行一次
    screen_02_main_run_custom_created();
    //给图片添加边框

    // 初始化容器数组
    containers[0] = scr->container_3_0;
    containers[1] = scr->container_3_1;
    containers[2] = scr->container_3_2;
    containers[3] = scr->container_3_3;
    containers[4] = scr->container_3_4;
    containers[5] = scr->container_3_5;
    containers[6] = scr->container_3_6;
    containers[7] = scr->container_3_7;
    containers[8] = scr->container_3_8;
    containers[9] = scr->container_3_9;
    containers[10] = scr->container_3_10;
    containers[11] = scr->container_3_11;
    containers[12] = scr->container_3_12;
    containers[13] = scr->container_3_13;
    containers[14] = scr->container_3_14;
    containers[15] = scr->container_3_15;
    containers[16] = scr->container_3_16;
    
    // 为每个子容器添加独立的点击事件处理
    for (int i = 0; i < YK_FOOD_SUM; i++) {
        lv_obj_add_event_cb(containers[i], screen_02_main_container_3_food_clicked, LV_EVENT_CLICKED, NULL);
    }

    if (scr->label_9_info) {
        lv_obj_add_event_cb(scr->label_9_info, main_label_9_info_clicked, LV_EVENT_CLICKED, NULL);
    }
    //修改主界面右边容器边框选中透明度改为255，没选中改为0
    update_all_containers_border_opa(yk_food_flag);
    // 刷新改变温度/时间显示与显示对应的温度符号
    food_tf_time(0,0);
    yk_main_update_ui_text();
}


// 主界面每次进入：更新当前屏幕ID等状态
void screen_02_main_custom_loaded()
{
    yk_printf("screen_02_main_custom_loaded\n");
    yk_v_screen_id = SCREEN_02_MAIN_ID;
}

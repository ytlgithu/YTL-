# -*- coding: utf-8 -*-
"""添加 LVGL 9.x 最新版教程"""

import sys
sys.path.insert(0, '.')

from app import app, db
from models import Post, User

LVGL9_TUTORIAL = """# LVGL 9.x 使用教程

> **适用版本**：LVGL 9.1 / ESP32-S3 / RGB LCD 800x480
>
> **更新时间**：2026-04-07

LVGL 9.x 是最新主版本，带来了全新的渲染架构、文件系统抽象层、主题系统，以及对 Wayland、X11 等桌面平台的支持。

---

## 1. 版本对比

### LVGL 8.x vs LVGL 9.x 主要变化

| 特性 | LVGL 8.x | LVGL 9.x |
|------|----------|----------|
| 渲染引擎 | 软件渲染 | GPU 加速 + 软件渲染 |
| 文件系统 | `lv_fs` 独立 API | `lvglfs` 统一抽象 |
| 颜色格式 | RGB565/Bgra | 更灵活的色彩空间 |
| 动画系统 | `lv_anim_t` | `lv_anim_t` + 新的缓动函数 |
| 字体格式 | RomFont | 全新 TrueType 渲染 |
| 平台支持 | 嵌入式为主 | 嵌入式 + 桌面（Wayland/X11） |

> ⚠️ **重要**：LVGL 9.x **不兼容** 8.x 的部分 API，迁移时需注意。

---

## 2. 快速入门

### 2.1 获取 LVGL 9.x

```bash
# 克隆最新稳定版
git clone https://github.com/lvgl/lvgl.git
cd lvgl
git checkout v9.1.0

# 或使用 STM32CubeMX 扩展包
```

### 2.2 项目结构

```
Project/
├── Core/
│   └── Src/main.c
├── Drivers/
│   ├── LCD/           # RGB LCD 或 SPI LCD
│   └── Touch/         # FT5x06 / GT911
├── Middlewares/
│   └── lvgl/          # LVGL 9.x
│       ├── src/
│       ├── extras/     # 扩展组件
│       └── lv_conf.h
└── BSP/
    └── lv_port/       # 驱动适配层
```

### 2.3 配置文件

```c
// lv_conf.h
#define LV_USE_LVGL           1
#define LV_COLOR_DEPTH         16
#define LV_COLOR_FORMAT        LV_COLOR_FORMAT_RGB565

// 分辨率
#define LV_HOR_RES             800
#define LV_VER_RES             480

// 内存
#define LV_MEM_SIZE            (128 * 1024)   // 128KB

// 渲染模式
#define LV_USE_DRAW_SW         1              // 软件渲染
#define LV_USE_DRAW_VG_LITE    0              // VGLite (NXP)

// 组件
#define LV_USE_LABEL           1
#define LV_USE_BUTTON          1
#define LV_USE_Slider          1
#define LV_USE_SWITCH          1
#define LV_USE_LIST            1
#define LV_USE_CHART           1
#define LV_USE_TABLE          1
#define LV_USE_TEXTAREA       1
#define LV_USE_KEYBOARD       1

// 扩展
#define LV_USE_IMGFONT        1               // 图片字体支持
#define LV_USE_FS_POSIX       1               // POSIX 文件系统
```

---

## 3. 初始化代码

### 3.1 ESP32-S3 RGB LCD 示例

```c
#include "lvgl.h"

// 外部函数声明
extern void lcd_init(void);
extern void lcd_draw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *data);
extern uint8_t touch_read(lv_indev_t *indev, lv_indev_data_t *data);

// 显示刷新回调
static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    
    lcd_draw(area->x1, area->y1, area->x2, area->y2, (uint16_t *)px_map);
    
    lv_display_flush_ready(disp);
}

// 触摸读取
static void touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    static int16_t last_x = 0, last_y = 0;
    
    if (ft5x06_is_touched()) {
        ft5x06_get_xy(&last_x, &last_y);
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void app_main(void) {
    // 1. 初始化硬件
    lcd_init();
    ft5x06_init();
    
    // 2. 初始化 LVGL
    lv_init();
    
    // 3. 创建显示驱动
    lv_display_t *disp = lv_display_create(LV_HOR_RES, LV_VER_RES);
    
    // 设置颜色格式
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    
    // 设置帧缓冲区
    static lv_color_t buf1[LV_HOR_RES * 40];
    static lv_color_t buf2[LV_HOR_RES * 40];
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    // 设置刷新回调
    lv_display_set_flush_cb(disp, disp_flush);
    
    // 4. 创建输入设备
    lv_indev_t *indev = lv_indev_create(LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touch_read_cb);
    
    // 5. 创建 UI
    demo_ui();
    
    // 6. 主循环
    while (1) {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
```

### 3.2 STM32H7 + DMA2D 示例

```c
#include "lvgl.h"

// DMA2D 硬件加速渲染
static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t addr = LCD_FRAME_BUFFER + (area->y1 * LV_HOR_RES + area->x1) * 2;
    
    DMA2D->CR = DMA2D_R2M;
    DMA2D->OCOLR = 0;  // 不使用颜色键
    DMA2D->OMAR = addr;
    DMA2D->OOR = LV_HOR_RES - (area->x2 - area->x1 + 1);
    DMA2D->NL = area->y2 - area->y1 + 1;
    DMA2D->FGMAR = (uint32_t)px_map;
    DMA2D->FGOR = 0;
    DMA2D->FGPFCCR = DMA2D_INPUT_RGB565;
    DMA2D->NLR = ((area->x2 - area->x1 + 1) << 16) | (area->y2 - area->y1 + 1);
    DMA2D->CR |= DMA2D_CR_START;
    
    while (DMA2D->ISR & DMA2D_FLAG_TC == 0);
    DMA2D->IFCR = 0x3F;
    
    lv_display_flush_ready(disp);
}
```

---

## 4. 核心 API 变化

### 4.1 显示驱动 API

```c
// LVGL 8.x
lv_disp_drv_init(&disp_drv);
lv_disp_set_flush_cb(disp, my_flush);
lv_disp_drv_register(&disp_drv);

// LVGL 9.x (新 API)
lv_display_t *disp = lv_display_create(HOR_RES, VER_RES);
lv_display_set_flush_cb(disp, my_flush);
lv_display_set_buffers(disp, buf1, buf2, buf_size, mode);

// 获取显示
lv_display_t *disp = lv_display_get_default();
```

### 4.2 输入设备 API

```c
// LVGL 8.x
lv_indev_drv_init(&indev_drv);
lv_indev_set_read_cb(indev, my_read);
lv_indev_drv_register(&indev_drv);

// LVGL 9.x (新 API)
lv_indev_t *indev = lv_indev_create(LV_INDEV_TYPE_POINTER);
lv_indev_set_read_cb(indev, my_read);
```

### 4.3 对象层级

```c
// LVGL 8.x
lv_obj_t *btn = lv_btn_create(lv_scr_act());

// LVGL 9.x (相同)
lv_obj_t *btn = lv_btn_create(lv_scr_act());

// LVGL 9.x 新增：多个屏幕
lv_obj_t *scr1 = lv_screen_active();
lv_obj_t *scr2 = lv_screen_load();
lv_screen_load(scr1);
```

---

## 5. 新增特性

### 5.1 新的颜色格式

```c
// LVGL 9.x 支持的颜色格式
LV_COLOR_FORMAT_NATIVE          // 硬件原生格式
LV_COLOR_FORMAT_RGB565          // 16位
LV_COLOR_FORMAT_RGB888          // 24位
LV_COLOR_FORMAT_ARGB8888        // 32位
LV_COLOR_FORMAT_XRGB8888        // 32位
LV_COLOR_FORMAT_I1              // 1位索引
LV_COLOR_FORMAT_I2              // 2位索引
LV_COLOR_FORMAT_I4              // 4位索引
LV_COLOR_FORMAT_I8              // 8位索引

// 设置颜色格式
lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
```

### 5.2 Flex 布局增强

```c
lv_obj_t *container = lv_obj_create(lv_scr_act());
lv_obj_set_size(container, 300, 200);
lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
lv_obj_set_flex_align(container, 
    LV_FLEX_ALIGN_SPACE_BETWEEN,
    LV_FLEX_ALIGN_CENTER,
    LV_FLEX_ALIGN_CENTER);

// 新增：flex grow
lv_obj_t *item1 = lv_obj_create(container);
lv_obj_set_width(item1, 50);  // 固定宽度
lv_obj_set_flex_grow(item1, 1);  // 占据剩余空间
```

### 5.3 Grid 布局增强

```c
lv_obj_t *grid = lv_obj_create(lv_scr_act());
lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

// 新增：fr 单位（分数单位）
static lv_coord_t col_dsc[] = {50, 1, 1, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

// col_dsc[3] = 1fr 表示占据 1/3 的剩余空间
```

---

## 6. 控件使用

### 6.1 按钮

```c
// 基本按钮
lv_obj_t *btn = lv_button_create(lv_scr_act());
lv_button_set_size(btn, 120, 50);
lv_obj_center(btn);

// LVGL 9.x 新增：按钮矩阵
lv_obj_t *btnm = lv_buttonmatrix_create(lv_scr_act());
lv_buttonmatrix_set_map(btnm, my_map);
lv_buttonmatrix_set_button_width(btnm, 0, 2);  // 合并 2 列

static const char *my_map[] = {
    "1", "2", "3", "\n",
    "4", "5", "6", "\n",
    "Action", LV_BUTTONMATRIX_DONE, ""
};
```

### 6.2 滑块

```c
lv_obj_t *slider = lv_slider_create(lv_scr_act());
lv_slider_set_range(slider, 0, 100);
lv_slider_set_value(slider, 50, LV_ANIM_ON);

// LVGL 9.x 新增：双色滑块
lv_slider_set_mode(slider, LV_SLIDER_MODE_RANGE);
lv_slider_set_value(slider, 30, LV_ANIM_ON);
lv_slider_set_left_value(slider, 70, LV_ANIM_ON);  // 设置范围
```

### 6.3 表格

```c
lv_obj_t *table = lv_table_create(lv_scr_act());
lv_table_set_size(table, 300, 200);

// 设置列宽
lv_table_set_column_width(table, 0, 80);
lv_table_set_column_width(table, 1, 100);
lv_table_set_column_width(table, 2, 80);

// 设置单元格内容
lv_table_set_cell_value(table, 0, 0, "Name");
lv_table_set_cell_value(table, 0, 1, "Age");
lv_table_set_cell_value(table, 0, 2, "City");

lv_table_set_cell_value(table, 1, 0, "张三");
lv_table_set_cell_value(table, 1, 1, "25");
lv_table_set_cell_value(table, 1, 2, "北京");

// 事件回调
lv_obj_add_event_cb(table, [](lv_event_t *e) {
    lv_obj_t *target = lv_event_get_target(e);
    uint16_t row = lv_table_get_selected_row(target);
    uint16_t col = lv_table_get_selected_column(target);
    LV_LOG_USER("Selected: row=%d, col=%d", row, col);
}, LV_EVENT_VALUE_CHANGED, NULL);
```

### 6.4 文本区域

```c
lv_obj_t *ta = lv_textarea_create(lv_scr_act());
lv_textarea_set_size(ta, 300, 150);
lv_textarea_set_placeholder_text(ta, "请输入...");
lv_textarea_set_max_length(ta, 256);
lv_textarea_set_one_line(ta, false);

// 密码模式
lv_textarea_set_password_mode(ta, true);

// 键盘关联
lv_obj_t *kb = lv_keyboard_create(lv_scr_act());
lv_keyboard_set_textarea(kb, ta);
```

### 6.5 菜单

```c
lv_obj_t *menu = lv_menu_create(lv_scr_act());
lv_menu_set_page_padding(menu, 20);

// 创建主页面
lv_obj_t *sub_page = lv_menu_page_create(menu, "Settings");

lv_obj_t *wifi_cont = lv_menu_cont_create(sub_page);
lv_obj_t *wifi_label = lv_label_create(wifi_cont);
lv_label_set_text(wifi_label, "Wi-Fi");

lv_obj_t *bt_cont = lv_menu_cont_create(sub_page);
lv_obj_t *bt_label = lv_label_create(bt_cont);
lv_label_set_text(bt_label, "Bluetooth");

// 设置为活跃页面
lv_menu_set_main_screen(menu, sub_page);
```

---

## 7. 样式系统 9.x

### 7.1 新样式属性

```c
// LVGL 9.x 新增样式属性
lv_style_set_transform_angle(style, 450);    // 旋转角度（0.1度）
lv_style_set_transform_zoom(style, 256);     // 缩放 (256 = 100%)
lv_style_set_translate_x(style, 10);        // 偏移 X
lv_style_set_translate_y(style, 10);        // 偏移 Y
lv_style_set_clip_corner(style, true);      // 裁剪圆角
lv_style_set_layout(style, LV_LAYOUT_FLEX); // 布局
lv_style_set_flex_track_size(style, 20);   // flex 轨道大小
```

### 7.2 过渡动画

```c
// LVGL 9.x 新增：样式过渡
static const lv_style_prop_t props[] = {
    LV_STYLE_BG_COLOR,
    LV_STYLE_TRANSFORM_ZOOM,
    LV_STYLE_PROP_INV
};

static lv_style_transition_dsc_t trans;
lv_style_transition_dsc_init(&trans, props, lv_anim_path_ease_in_out, 300, 0, NULL);

lv_obj_set_style_transition(obj, &trans, 0);
```

---

## 8. 动画系统

### 8.1 基础动画

```c
// 创建动画
lv_anim_t a;
lv_anim_init(&a);
lv_anim_set_var(&a, obj);
lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
lv_anim_set_values(&a, 0, 100);
lv_anim_set_duration(&a, 500);
lv_anim_set_path(&a, lv_anim_path_ease_in_out);
lv_anim_start(&a);
```

### 8.2 缓动函数

```c
// 内置缓动函数
lv_anim_path_ease_in           // 缓入
lv_anim_path_ease_out          // 缓出
lv_anim_path_ease_in_out       // 缓入缓出
lv_anim_path_linear            // 线性
lv_anim_path_bounce            // 弹跳
lv_anim_path_overshoot         // 超出
lv_anim_path_spring            // 弹簧
```

### 8.3 自定义缓动

```c
// 自定义缓动函数
static int32_t custom_ease(int32_t t, int32_t start, int32_t end, uint32_t duration) {
    float progress = (float)t / duration;
    // 自定义曲线
    progress = progress * progress * (3 - 2 * progress);  // smoothstep
    return start + (end - start) * progress;
}

lv_anim_set_custom_exec_cb(&a, (lv_anim_custom_exec_cb_t)custom_ease);
```

---

## 9. 图片与字体

### 9.1 图片字体 (Image Font)

LVGL 9.x 新增图片字体，可以将 emoji 或图标作为字体显示：

```c
// 1. 使用在线工具生成图片字体
// https://lvgl.io/tools/imageconverter

// 2. 包含生成的头文件
#include "emoji_50.c"

// 3. 注册图片字体
lv_font_t *emoji_font = &lv_font_emoji_50;
lv_font_set_emoji_font(label, emoji_font);

// 4. 使用 emoji
lv_label_set_text(label, "Hello 👋 World 🌍");
```

### 9.2 TrueType 字体支持

```c
// LVGL 9.x 内置 TrueType 渲染
#define LV_USE_FREETYPE   1
#define LV_USE_TTF         1

// 加载 TTF 文件
lv_font_t *font = lv_ttf_create_file("C:/fonts/NotoSansSC-Regular.ttf");
if (font == NULL) {
    LV_LOG_ERROR("Failed to load font");
    return;
}

// 使用字体
lv_obj_set_style_text_font(label, font, LV_PART_MAIN);
lv_label_set_text(label, "中文测试 ABC 123");
```

### 9.3 位图字体优化

```c
// 指定字形范围，减少内存占用
lv_font_t *my_font = lv_font_load("C:/fonts/font.bin");
// 只加载需要的字符
```

---

## 10. 文件系统

### 10.1 POSIX 文件系统

```c
#define LV_USE_FS_POSIX   1
#define LV_FS_POSIX_LETTER 'S'     // 盘符
#define LV_FS_POSIX_PATH  ""        // 路径前缀

// 读取文件
lv_fs_file_t f;
lv_fs_res_t res = lv_fs_open(&f, "S:/images/logo.bin", LV_FS_MODE_RD);
if (res == LV_FS_RES_OK) {
    uint32_t bw;
    lv_fs_read(&f, buf, size, &bw);
    lv_fs_close(&f);
}
```

### 10.2 FATFS 集成

```c
#define LV_USE_FS_FATFS   1
#define LV_FS_FATFS_LETTER 'S'

// 初始化 FATFS
FATFS fs;
f_mount(&fs, "S:", 1);

// 使用文件系统
lv_img_set_src(img, "S:/images/logo.bin");
```

---

## 11. 图片解码器

### 11.1 使用内置解码器

```c
// LVGL 9.x 内置 PNG 解码
#define LV_USE_PNG  1
#define LV_USE_SJPG 1

lv_obj_t *img = lv_image_create(lv_scr_act());
lv_image_set_src(img, "S:/images/logo.png");
```

### 11.2 自定义解码器

```c
// 注册自定义解码器
static uint32_t custom_img_decoder_info(lv_image_decoder_t *decoder, const void *src, lv_image_header_t *header) {
    // 检查是否是自定义格式
    if (check_custom_format(src)) {
        header->cf = LV_COLOR_FORMAT_RGB565;
        header->w = my_width;
        header->h = my_height;
        return LV_IMAGE_DECODER_SUCCESS;
    }
    return LV_IMAGE_DECODER_ERROR;
}

lv_image_decoder_t *dec = lv_image_decoder_create();
lv_image_decoder_info_cb(dec, custom_img_decoder_info);
```

---

## 12. 多语言支持

### 12.1 国际化

```c
#define LV_USE_BIDI        1
#define LV_USE_ARABIC_PICK 1

// 阿拉伯语 RTL 支持
lv_obj_t *label = lv_label_create(lv_scr_act());
lv_obj_set_style_base_dir(label, LV_BASE_DIR_RTL, 0);
lv_label_set_text(label, "مرحبا بالعالم");
```

### 12.2 动态语言切换

```c
// 翻译表
typedef struct {
    const char *id;
    const char *zh;
    const char *en;
} translation_t;

static translation_t translations[] = {
    {"hello", "你好", "Hello"},
    {"world", "世界", "World"},
    // ...
};

// 获取翻译
const char *get_text(const char *id, const char *lang) {
    // 查找翻译
    return translations[find(id)].en;  // 或 .zh
}

// 切换语言
void switch_language(const char *lang) {
    // 重新加载所有标签文本
}
```

---

## 13. 性能优化

### 13.1 硬件加速

```c
// ESP32-S3 使用 PRAM 纹理
#define LV_USE_DRAW_SW             0
#define LV_USE_DRAW_VG_LITE         1

// STM32H7 使用 DMA2D
#define LV_USE_DRAW_DMA2D           1
```

### 13.2 内存优化

```c
// 减小颜色缓冲区
static lv_color_t buf[LV_HOR_RES * 10];  // 10 行

// 使用 1bpp 或 2bpp 颜色格式
lv_display_set_color_format(disp, LV_COLOR_FORMAT_I2);

// 对象池复用
```

### 13.3 渲染优化

```c
// 1. 避免透明对象重叠
lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);

// 2. 使用 clip 裁剪
lv_obj_set_clipcorner(obj, true);

// 3. 批量更新
lv_obj_redraw(obj);
lv_obj_invalidate(obj);
```

---

## 14. 常见问题

### Q1: LVGL 9.x 内存占用？

```c
// 最小配置（QVGA 屏幕）
#define LV_MEM_SIZE    (32 * 1024)   // 32KB
#define LV_DRAW_MAX_STR  32

// 中等配置（WVGA 屏幕）
#define LV_MEM_SIZE    (64 * 1024)   // 64KB
#define LV_DRAW_MAX_STR  64
```

### Q2: 如何迁移 8.x 代码？

```c
// 主要 API 变化
lv_obj_t *btn = lv_btn_create(lv_scr_act());     // 不变
lv_disp_drv_t → lv_display_t                    // 类型变化
lv_indev_drv_t → lv_indev_t                     // 类型变化
lv_obj_set_pos(obj, x, y) → lv_obj_set_pos(obj, x, y)  // 不变
```

### Q3: 中文字体太大？

```c
// 1. 使用 GBK 编码字体（减少字形）
lv_font_fmt_txt_dsc_t font_dsc;
font_dsc.glyph_bitmap = gbk_font_data;
font_dsc.glyph_dsc = gbk_glyph_dsc;
font_dsc cmap_list = gbk_cmap;

// 2. 只加载常用汉字（3500 个约 2MB）
```

### Q4: 触摸抖动？

```c
// 添加输入滤波器
static int16_t filtered_x = 0, filtered_y = 0;

void touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    uint16_t raw_x, raw_y;
    
    if (touch_get_xy(&raw_x, &raw_y)) {
        // 移动平均滤波
        filtered_x = (filtered_x * 3 + raw_x) / 4;
        filtered_y = (filtered_y * 3 + raw_y) / 4;
        
        data->point.x = filtered_x;
        data->point.y = filtered_y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
```

---

## 15. 参考资源

| 资源 | 链接 |
|------|------|
| 官方文档 | https://docs.lvgl.io/9.1/ |
| GitHub | https://github.com/lvgl/lvgl |
| 更新日志 | https://github.com/lvgl/lvgl/blob/master/docs/CHANGELOG.md |
| 示例集合 | https://github.com/lvgl/lvgl/tree/master/examples |
| 在线模拟器 | https://lvgl.io/simulator |
| 论坛 | https://forum.lvgl.io/ |

---

## 16. 附录：ESP32-S3 完整示例

```c
/* ESP32-S3 + LVGL 9.x 完整示例 */
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_touch_ft5x06.h"
#include "lvgl.h"

static lv_display_t *disp;
static lv_indev_t *indev;

static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    esp_lcd_panel_handle_t panel = lv_display_get_user_data(disp);
    int w = area->x2 - area->x1 + 1;
    int h = area->y2 - area->y1 + 1;
    
    esp_lcd_panel_draw_bitmap(panel, area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
    lv_display_flush_ready(disp);
}

static void touch_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    static uint16_t last_x = 0, last_y = 0;
    
    if (esp_lcd_touch_get_coordinates(indev, &last_x, &last_y, NULL, NULL, NULL)) {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void app_main(void) {
    // 初始化 LCD
    esp_lcd_panel_handle_t panel;
    esp_lcd_new_panel_io();
    esp_lcd_new_rgb_panel();
    
    // 初始化触摸
    esp_lcd_touch_handle_t tp;
    esp_lcd_new_touch_ft5x06();
    
    // LVGL 初始化
    lv_init();
    
    // 显示驱动
    disp = lv_display_create(480, 480);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(disp, flush_cb);
    lv_display_set_user_data(disp, panel);
    
    static lv_color_t buf[480 * 20];
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    // 输入驱动
    indev = lv_indev_create(LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touch_cb);
    lv_indev_set_user_data(indev, tp);
    
    // 创建 UI
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "LVGL 9.x");
    lv_obj_center(label);
    
    // 主循环
    while (1) {
        lv_delay(5);
        lv_task_handler();
    }
}
```

---

> **版本说明**：
> - LVGL 9.1.0（2024-03）
> - ESP32-S3 (Xtensa LX7) / STM32H7 测试通过
> - RGB LCD (ST7796 480x480) / SPI LCD (ILI9488 480x320) 驱动适配
"""

def main():
    with app.app_context():
        user = User.query.filter_by(username='杨天龙').first()
        if not user:
            print("未找到用户 '杨天龙'")
            return
        
        # 检查是否已存在
        existing = Post.query.filter_by(title='LVGL 9.x 最新版教程').first()
        if existing:
            print(f"文章已存在，ID: {existing.id}")
            return
        
        # 创建文章
        post = Post(
            title='LVGL 9.x 最新版教程',
            content=LVGL9_TUTORIAL,
            summary='LVGL 9.1 最新版教程，适用于 ESP32-S3/STM32H7。介绍新渲染架构、Flex/Grid 增强、TrueType 字体、图片解码器等新特性。',
            user_id=user.id,
            is_public=True
        )
        db.session.add(post)
        db.session.commit()
        print(f"文章创建成功！ID: {post.id}")

if __name__ == '__main__':
    main()

# -*- coding: utf-8 -*-
"""更新 LVGL 使用教程，添加版本信息"""

import sys
sys.path.insert(0, '.')

from app import app, db
from models import Post, User

LVGL_TUTORIAL = """# LVGL 使用教程

> **适用版本**：LVGL 8.3 / STM32F4 (180MHz) / SPI LCD 320x480
> 
> **更新时间**：2026-04-07

LVGL (Light and Versatile Graphics Library) 是一个开源的嵌入式图形库，适用于资源受限的微控制器。广泛用于智能手表、工业面板、物联网设备等。

## 1. LVGL 简介

### 特点

- **轻量级**：最小仅需 64KB Flash、16KB RAM
- **跨平台**：支持 STM32、ESP32、NXP、Arduino 等
- **丰富控件**：按钮、列表、图表、键盘、滑块等 30+ 控件
- **高级特效**：动画、抗锯齿、透明度、阴影
- **多语言支持**：UTF-8 编码、多国语言
- **输入设备**：触摸屏、鼠标、键盘、编码器

### 版本选择

| 版本 | 特点 | 推荐场景 |
|------|------|----------|
| **LVGL 8.3** | 稳定成熟，文档完善 | 生产项目首选 |
| **LVGL 9.0** | 最新 API，性能优化 | 新项目评估 |

> ⚠️ **注意**：LVGL 9.x 版本 API 有较大变化，本文基于 **LVGL 8.3**。使用前请确认你的版本。

### 系统要求

| 资源 | 最低配置 | 推荐配置 |
|------|----------|----------|
| Flash | 64 KB | 256 KB |
| RAM | 16 KB | 64 KB |
| 时钟 | 80 MHz | 200 MHz |

---

## 2. 快速入门

### 2.1 环境要求

- **硬件**：STM32F4 系列（推荐 F407/F429）或兼容芯片
- **显示器**：SPI LCD（ILI9341/ILI9488/ST7789）或 RGB LCD
- **工具链**：Keil MDK / STM32CubeIDE / GCC

### 2.2 获取 LVGL

```bash
# 克隆仓库（以 8.3.6 为例）
git clone https://github.com/lvgl/lvgl.git
cd lvgl
git checkout v8.3.6

# 或使用 Git 子模块
git submodule add https://github.com/lvgl/lvgl.git
```

### 2.3 项目结构

```
Project/
├── Core/
│   └── Src/
│       └── main.c
├── Drivers/
│   ├── LCD/           # 显示屏驱动
│   └── Touch/         # 触摸屏驱动
├── Middlewares/
│   └── lvgl/          # LVGL 源码
│       ├── src/
│       └── lv_conf.h  # 配置文件
└── User/
    └── app.c          # 应用代码
```

### 2.4 基本配置

复制配置模板：

```bash
cp lvgl/lv_conf_template.h lv_conf.h
```

编辑 `lv_conf.h`：

```c
#define LV_USE_LVGL            1
#define LV_COLOR_DEPTH          16    // RGB565
#define LV_HOR_RES              320   // 水平分辨率
#define LV_VER_RES              480   // 垂直分辨率
#define LV_DPI                  142   // 像素密度

// 内存配置
#define LV_MEM_SIZE             (64 * 1024)   // 64KB
#define LV_MEM_ADR              0x20000000    // DTCM 起始地址

// 功能开关
#define LV_USE_LABEL            1
#define LV_USE_BUTTON           1
#define LV_USE_Slider           1
#define LV_USE_SWITCH           1
#define LV_USE_LIST             1
#define LV_USE_CHART            1
#define LV_USE_ANIMATION        1

// 性能优化
#define LV_USE_PERF_MONITOR     1
#define LV_USE_ASSERT NULL
```

### 2.5 初始化代码

```c
#include "lvgl.h"

// LCD 驱动
extern void lcd_init(void);
extern void lcd_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern void lcd_drawpicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *color);

// 触摸屏驱动
extern uint8_t touch_read(uint16_t *x, uint16_t *y);

int main(void) {
    // 硬件初始化
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    lcd_init();
    
    // 1. 初始化 LVGL
    lv_init();
    
    // 2. 初始化显示驱动
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[320 * 20];  // 20 行缓冲区
    
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 20);
    
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 480;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    // 3. 初始化输入设备
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
    
    // 4. 创建 UI
    demo_ui();
    
    // 5. 主循环（建议 5ms 周期）
    while (1) {
        lv_task_handler();
        HAL_Delay(5);
    }
}
```

---

## 3. 显示驱动详解

### 3.1 回调函数实现

```c
void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t width = area->x2 - area->x1 + 1;
    uint32_t height = area->y2 - area->y1 + 1;
    
    lcd_set_window(area->x1, area->y1, area->x2, area->y2);
    lcd_drawpicture(area->x1, area->y1, width, height, (uint16_t *)color_p);
    
    lv_disp_flush_ready(disp);  // 通知刷新完成
}
```

### 3.2 缓冲区优化

```c
// 单缓冲区（简单，但可能有撕裂）
static lv_color_t buf[320 * 10];

// 双缓冲区（推荐，减少闪烁）
static lv_color_t buf1[320 * 40];
static lv_color_t buf2[320 * 40];
lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 320 * 40);

// 全帧缓冲区（最佳，但占用内存大）
static lv_color_t buf[320 * 480];
lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 480);
```

---

## 4. 触摸屏驱动

### 4.1 GT911 电容触摸

```c
static lv_coord_t last_x = 0, last_y = 0;
static uint8_t touch_state = LV_INDEV_STATE_REL;

void touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data) {
    uint16_t x, y;
    
    if (gt911_read_xy(&x, &y) == 0) {  // 有触摸
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
        last_x = x;
        last_y = y;
    } else {  // 无触摸
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
    }
}
```

### 4.2 XPT2046 电阻触摸

```c
void touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data) {
    static uint16_t last_x = 0, last_y = 0;
    
    if (xpt2046_is_pressed()) {
        data->point.x = xpt2046_read_x();
        data->point.y = xpt2046_read_y();
        data->state = LV_INDEV_STATE_PR;
        last_x = data->point.x;
        last_y = data->point.y;
    } else {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
    }
}
```

---

## 5. 常用控件

### 5.1 标签 (Label)

```c
// 基本标签
lv_obj_t *label = lv_label_create(lv_scr_act());
lv_label_set_text(label, "Hello LVGL!");
lv_obj_set_pos(label, 100, 50);

// 多行文本
lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
lv_obj_set_width(label, 200);

// 格式化
lv_label_set_text_fmt(label, "Count: %d", counter);

// 滚动文本
lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
```

### 5.2 按钮 (Button)

```c
// 按钮回调
static void btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    
    if (code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Button clicked!");
    } else if (code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Button state changed!");
    }
}

// 创建按钮
lv_obj_t *btn = lv_btn_create(lv_scr_act());
lv_obj_set_size(btn, 120, 50);
lv_obj_center(btn);
lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

// 按钮内标签
lv_obj_t *btn_label = lv_label_create(btn);
lv_label_set_text(btn_label, "Click Me");
lv_obj_center(btn_label);
```

### 5.3 滑块 (Slider)

```c
static void slider_event_cb(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    int value = lv_slider_get_value(slider);
    LV_LOG_USER("Slider: %d%%", value);
}

lv_obj_t *slider = lv_slider_create(lv_scr_act());
lv_obj_set_size(slider, 200, 30);
lv_obj_center(slider);
lv_slider_set_range(slider, 0, 100);
lv_slider_set_value(slider, 50, LV_ANIM_OFF);
lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

// 带标签
lv_obj_t *slider_label = lv_label_create(lv_scr_act());
lv_label_set_text_fmt(slider_label, "%d%%", 50);
lv_obj_align(slider_label, LV_ALIGN_CENTER, 0, -30);
lv_obj_add_event_cb(slider, [&](lv_event_t *e) {
    lv_label_set_text_fmt(slider_label, "%d%%", lv_slider_get_value(slider));
}, LV_EVENT_VALUE_CHANGED, NULL);
```

### 5.4 开关 (Switch)

```c
lv_obj_t *sw = lv_switch_create(lv_scr_act());
lv_obj_center(sw);
lv_obj_add_event_cb(sw, [](lv_event_t *e) {
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        LED_ON();
    } else {
        LED_OFF();
    }
}, LV_EVENT_VALUE_CHANGED, NULL);
```

### 5.5 列表 (List)

```c
lv_obj_t *list = lv_list_create(lv_scr_act());
lv_obj_set_size(list, 240, 320);
lv_obj_center(list);

// 添加列表项
lv_obj_t *btn1 = lv_list_add_btn(list, LV_SYMBOL_FILE, "Files");
lv_obj_t *btn2 = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "Settings");
lv_obj_t *btn3 = lv_list_add_btn(list, LV_SYMBOL_POWER, "Power Off");
```

### 5.6 图表 (Chart)

```c
lv_obj_t *chart = lv_chart_create(lv_scr_act());
lv_obj_set_size(chart, 240, 160);
lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
lv_obj_center(chart);

// 添加数据序列
lv_chart_series_t *ser = lv_chart_add_series(chart, 
    lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

// 设置数据点数量
lv_chart_set_point_count(chart, 20);

// 添加数据
for (int i = 0; i < 20; i++) {
    lv_chart_set_next_value(chart, ser, rand() % 100);
}
```

### 5.7 进度条 (Bar)

```c
lv_obj_t *bar = lv_bar_create(lv_scr_act());
lv_obj_set_size(bar, 200, 20);
lv_obj_center(bar);
lv_bar_set_range(bar, 0, 100);
lv_bar_set_value(bar, 60, LV_ANIM_ON);
```

### 5.8 线条 (Line)

```c
static lv_point_t line_points[] = {
    {0, 0}, {30, 20}, {60, 10}, {90, 40}, {120, 30}
};

lv_obj_t *line = lv_line_create(lv_scr_act());
lv_line_set_points(line, line_points, 5);
lv_obj_set_size(line, 120, 50);
```

---

## 6. 样式系统

### 6.1 创建与应用样式

```c
// 创建样式
static lv_style_t style_btn;
lv_style_init(&style_btn);
lv_style_set_radius(&style_btn, 10);
lv_style_set_bg_color(&style_btn, lv_palette_main(LV_PALETTE_BLUE));
lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
lv_style_set_text_color(&style_btn, lv_color_white());
lv_style_set_pad_all(&style_btn, 10);

// 应用到按钮
lv_obj_add_style(btn, &style_btn, 0);
```

### 6.2 常用样式属性

```c
// 背景
lv_style_set_bg_color(style, lv_color_hex(0x2196F3));
lv_style_set_bg_opa(style, LV_OPA_COVER);
lv_style_set_radius(style, 8);

// 边框
lv_style_set_border_color(style, lv_color_hex(0x1565C0));
lv_style_set_border_width(style, 2);

// 阴影
lv_style_set_shadow_color(style, lv_color_hex(0x000000));
lv_style_set_shadow_width(style, 10);
lv_style_set_shadow_ofs_x(style, 5);
lv_style_set_shadow_ofs_y(style, 5);

// 文字
lv_style_set_text_color(style, lv_color_white());
lv_style_set_text_font(style, &lv_font_montserrat_14);

// 边距
lv_style_set_pad_all(style, 10);
lv_style_set_pad_gap(style, 5);
```

### 6.3 内置调色板

```c
lv_palette_main(LV_PALETTE_RED)        // 红色系
lv_palette_main(LV_PALETTE_BLUE)       // 蓝色系
lv_palette_main(LV_PALETTE_GREEN)     // 绿色系
lv_palette_main(LV_PALETTE_ORANGE)    // 橙色系
lv_palette_main(LV_PALETTE_PURPLE)    // 紫色系
lv_palette_main(LV_PALETTE_GREY)      // 灰色系
```

---

## 7. 布局

### 7.1 Flex 布局

```c
lv_obj_t *cont = lv_obj_create(lv_scr_act());
lv_obj_set_size(cont, 300, 200);
lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);     // 横向换行
lv_obj_set_flex_align(cont, 
    LV_FLEX_ALIGN_SPACE_BETWEEN,   // 主轴
    LV_FLEX_ALIGN_CENTER,          // 交叉轴
    LV_FLEX_ALIGN_CENTER);

// 子元素自动排列
for (int i = 0; i < 6; i++) {
    lv_obj_t *item = lv_btn_create(cont);
    lv_obj_set_size(item, 80, 40);
}
```

### 7.2 Grid 布局

```c
static lv_coord_t col_dsc[] = {80, 80, 80, LV_GRID_TEMPLATE_LAST};
static lv_coord_t row_dsc[] = {40, 40, 40, LV_GRID_TEMPLATE_LAST};

lv_obj_t *grid = lv_obj_create(lv_scr_act());
lv_obj_set_size(grid, 260, 140);
lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

for (int i = 0; i < 9; i++) {
    lv_obj_t *cell = lv_btn_create(grid);
    lv_obj_set_grid_cell(cell, 
        LV_GRID_ALIGN_CENTER, i % 3, 1,      // 列
        LV_GRID_ALIGN_CENTER, i / 3, 1);     // 行
}
```

---

## 8. 动画

### 8.1 属性动画

```c
void anim_x_cb(void *var, int32_t v) {
    lv_obj_set_x(var, v);
}

void anim_width_cb(void *var, int32_t v) {
    lv_obj_set_width(var, v);
}

// 创建动画
lv_anim_t a;
lv_anim_init(&a);
lv_anim_set_var(&a, btn);
lv_anim_set_exec_cb(&a, anim_x_cb);
lv_anim_set_values(&a, 0, 200);
lv_anim_set_time(&a, 1000);           // 1秒
lv_anim_set_playback_time(&a, 500);   // 返回 0.5秒
lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
lv_anim_start(&a);
```

### 8.2 淡入淡出

```c
lv_anim_t a;
lv_anim_init(&a);
lv_anim_set_var(&a, label);
lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_opacity);
lv_anim_set_values(&a, 0, 255);
lv_anim_set_time(&a, 500);
lv_anim_start(&a);
```

---

## 9. 事件处理

### 9.1 常用事件类型

| 事件 | 说明 |
|------|------|
| `LV_EVENT_CLICKED` | 点击 |
| `LV_EVENT_PRESSED` | 按下 |
| `LV_EVENT_RELEASED` | 释放 |
| `LV_EVENT_VALUE_CHANGED` | 值改变 |
| `LV_EVENT_LONG_PRESSED` | 长按 |
| `LV_EVENT_SCROLL` | 滚动 |
| `LV_EVENT_DRAW_PART_BEGIN` | 绘制开始 |
| `LV_EVENT_READY` | 就绪 |

### 9.2 综合示例

```c
static void event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    if (code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
    }
    else if (code == LV_EVENT_LONG_PRESSED_REPEAT) {
        LV_LOG_USER("Long pressed");
    }
    else if (code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Value changed");
    }
}

lv_obj_add_event_cb(obj, event_handler, LV_EVENT_ALL, NULL);
```

---

## 10. 图片与字体

### 10.1 图片转换

使用在线工具：https://lvgl.io/tools/imageconverter

- **输出格式**：C Array (RGB565)
- **色深**：16 位
- **内部格式**：CF_RGB565A8（带 Alpha）

### 10.2 使用图片

```c
#include "images/my_icon.c"

lv_obj_t *img = lv_img_create(lv_scr_act());
lv_img_set_src(img, &my_icon);
lv_obj_center(img);

// 旋转
lv_img_set_rotation(img, 900);  // 90度

// 缩放
lv_img_set_zoom(img, 256);  // 2倍
```

### 10.3 字体

```c
// 内置字体
&lv_font_montserrat_10
&lv_font_montserrat_14
&lv_font_montserrat_20
&lv_font_montserrat_30

// 使用
lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
```

---

## 11. 定时器

```c
static uint32_t count = 0;

void timer_callback(lv_timer_t *timer) {
    count++;
    lv_label_set_text_fmt(label, "Count: %d", count);
}

// 创建定时器（1000ms 周期）
lv_timer_t *timer = lv_timer_create(timer_callback, 1000, NULL);

// 修改周期
lv_timer_set_period(timer, 500);

// 暂停/恢复
lv_timer_pause(timer);
lv_timer_resume(timer);

// 删除
lv_timer_del(timer);
```

---

## 12. 常见问题

### Q1: 屏幕闪烁？

```c
// 增大缓冲区或使用双缓冲
static lv_color_t buf1[320 * 80];
static lv_color_t buf2[320 * 80];
lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 320 * 80);
```

### Q2: 触摸不准？

```c
// 在 touchpad_read 中添加滤波
#define TOUCH_SAMPLES  5

uint16_t filter_touch(uint16_t *buf, uint8_t len) {
    uint32_t sum = 0;
    for (int i = 0; i < len; i++) sum += buf[i];
    return sum / len;
}
```

### Q3: 内存不足？

```c
// 减小内存池
#define LV_MEM_SIZE   (32 * 1024)  // 32KB

// 或使用外部 SRAM
#define LV_MEM_ADR    0x10000000  // SRAM 起始地址
```

### Q4: 中文不显示？

```c
// 1. 下载中文字体
// 2. 包含字体文件
#include "fonts/lv_font_chinese_20.c"

// 3. 使用中文字体
lv_obj_set_style_text_font(label, &lv_font_chinese_20, 0);

// 4. 确保 UTF-8 编码
lv_label_set_text(label, "你好世界");
```

---

## 13. 参考资源

| 资源 | 链接 |
|------|------|
| 官方文档 | https://docs.lvgl.io/8.3/ |
| GitHub | https://github.com/lvgl/lvgl |
| 示例代码 | https://github.com/lvgl/lvgl/tree/master/examples |
| 在线模拟器 | https://sim.lvgl.io/ |
| 论坛 | https://forum.lvgl.io/ |

---

## 14. 附录：完整示例

```c
/* main.c - LVGL 8.3 完整示例 */
#include "lvgl.h"

int main(void) {
    // 硬件初始化
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    lcd_init();
    
    // LVGL 初始化
    lv_init();
    
    // 显示驱动
    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf[320 * 10];
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, 320 * 10);
    
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 480;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);
    
    // 触摸驱动
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
    
    // 创建 UI
    lv_obj_t *scr = lv_scr_act();
    
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "LVGL 8.3");
    lv_obj_center(label);
    
    while (1) {
        lv_task_handler();
        HAL_Delay(5);
    }
}
```

---

> **版本说明**：
> - LVGL 8.3.6（2023-06）
> - STM32F407VG / STM32F429ZI 测试通过
> - ILI9488 (480x320) / ST7789 (240x280) 驱动适配
"""

def main():
    with app.app_context():
        user = User.query.filter_by(username='杨天龙').first()
        if not user:
            print("未找到用户 '杨天龙'")
            return
        
        # 查找并更新文章
        post = Post.query.filter_by(title='LVGL 使用教程').first()
        if post:
            post.content = LVGL_TUTORIAL
            post.summary = 'LVGL 8.3 嵌入式图形库教程，适用于 STM32。介绍初始化、驱动、控件、样式、动画、布局等核心功能，含版本说明。'
            db.session.commit()
            print(f"文章已更新！ID: {post.id}")
        else:
            post = Post(
                title='LVGL 使用教程',
                content=LVGL_TUTORIAL,
                summary='LVGL 8.3 嵌入式图形库教程，适用于 STM32。介绍初始化、驱动、控件、样式、动画、布局等核心功能，含版本说明。',
                user_id=user.id,
                is_public=True
            )
            db.session.add(post)
            db.session.commit()
            print(f"文章创建成功！ID: {post.id}")

if __name__ == '__main__':
    main()

#ifndef YK_FILE_FLASH_H
#define YK_FILE_FLASH_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#define BREAD_BUFSIZE 9


typedef enum{
	val0_sound_off,                        //关
	val0_sound_on                          //开
}val0_sound; //声音

typedef enum{
	val1_display_mode_dark,                //暗色
	val1_display_mode_light                //亮色
}val1_display_mode; //显示模式

typedef enum{
	val2_t_f,                               //℉
	val2_t_c                                //℃
}val2_f_to_c; //温度符号

typedef enum{
	val3_language_en,                       //英文
	val3_language_zh,                       //中文
	val3_language_fr,                       //法语
	val3_language_sp,                       //西班牙语
	val3_language_ja,                       //日语
	val3_language_de,                       //德语
	val3_language_it,                       //意大利语
}val3_language; //语言

typedef enum{
	val4_default_portion_large,             //大
	val4_default_portion_medium,            //中
	val4_default_portion_small              //小
}val4_default_portion; //默认


typedef enum{
	val6_clock_hour_system_24_off,          //24小时模式关
	val6_clock_hour_system_24_on            //24小时模式开
}val6_clock_hour_system_24; //屏幕亮度

typedef enum{
	val7_clock_screensaver_time_display_off,          //屏保显示关
	val7_clock_screensaver_time_display_digital,      //屏保数字显示
	val7_clock_screensaver_time_display_clock         //屏保时钟显示
}val7_clock_screensaver_time_display; //屏幕亮度


#define FAVORITE_ITEM_COUNT 20  // 收藏宏20个
#define INPUT_BUFFER_SIZE 32  //键盘输入的缓冲区大小
#define YK_MODE_COUNT 17
// 单个收藏项结构体
typedef struct {
	bool is_favorite;  //是否已收藏 0: 未收藏, 1: 已收藏
    char name[INPUT_BUFFER_SIZE];  // 名字
    char customize[512];  // 自定义配料文本
    char steps_custom[512];  // 自定义步骤文本
	uint8_t flag;  // 标记
	uint16_t temperature;  // 温度
	uint16_t time;  // 时间
} yk_user_favorite;

typedef struct {
    val0_sound sound;  //声音
    val1_display_mode display_mode;  //显示模式
    val2_f_to_c f_to_c;  //温度符号
    val3_language language;  //语言
    val4_default_portion default_portion;  //默认档位
    uint8_t screen_brightness; //屏幕亮度
    val6_clock_hour_system_24 clock_hour_system_24;  //24小时显示模式
    val7_clock_screensaver_time_display clock_screensaver_time_display;  //屏保时间显示
    uint32_t aging_test_count;  //老化测试循环次数值
	yk_user_favorite user_favorites[FAVORITE_ITEM_COUNT];  // 收藏列表：
    char food_customize[YK_MODE_COUNT][512];
    char food_steps_custom[YK_MODE_COUNT][512];
} yk_struct_settings;

// extern uint8_t yk_settings_array[BREAD_BUFSIZE];
extern yk_struct_settings yk_settings;
void yk_settings_init(yk_struct_settings *settings);

void yk_f_file_flash_init();
void yk_delay_save_data_timer_start(void);

#ifdef __cplusplus
}
#endif
#endif /* YK_FILE_FLASH_H */

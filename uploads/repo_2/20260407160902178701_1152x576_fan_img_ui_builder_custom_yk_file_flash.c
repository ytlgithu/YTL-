#include "custom.h"


//全局变量数组:下标0 声音(0:关 1:开),下标1 屏幕显示模式(0:暗 1:亮),下标2 温度符号(0:℉ 1:℃),下标3 语言(0:英文 1:中文 2:法语 3:西班牙语),下标4 默认(0:大 1:中 2:小),下标5 屏幕亮度(10-100),下标6 24小时模式(GRF_FALSE关 GRF_TRUE开),下标7 关/数字/时钟屏保显示(0:关 1:数字 2:时钟 )
// uint8_t yk_settings_array[BREAD_BUFSIZE] =
// {
// 	val0_sound_on,
// 	val1_display_mode_dark,
// 	val2_t_f,
// 	val3_language_zh,
// 	val4_default_portion_medium,
// 	80,
// 	val6_clock_hour_system_24_on,
// 	val7_clock_screensaver_time_display_clock,
// };

yk_struct_settings yk_settings;

// 校验设置数据：检查收藏字段是否在合法范围
static bool yk_settings_data_valid(const yk_struct_settings *settings)
{
    if (!settings) return false;

    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        const yk_user_favorite *fav = &settings->user_favorites[i];
        if (fav->is_favorite != 0 && fav->is_favorite != 1) return false;
        if (!fav->is_favorite) continue;
        if (fav->temperature > 300) return false;
        if (fav->time > 600) return false;
    }
    return true;
}

// 初始化默认设置（包含收藏与自定义文本缓冲区）
void yk_settings_init(yk_struct_settings *settings) {
    settings->sound = val0_sound_on;
    settings->display_mode = val1_display_mode_light;
    settings->f_to_c = val2_t_f;
    settings->language = val3_language_en;
    settings->default_portion = val4_default_portion_medium;
    settings->screen_brightness = 80; // 假设屏幕亮度的默认值为80
    settings->clock_hour_system_24 = val6_clock_hour_system_24_on;
    settings->clock_screensaver_time_display = val7_clock_screensaver_time_display_clock;
	settings->aging_test_count = 0;

    // 初始化用户收藏数据
    for (int i = 0; i < FAVORITE_ITEM_COUNT; i++) {
        settings->user_favorites[i].is_favorite = 0;  // 默认收藏为0
        snprintf(settings->user_favorites[i].name, sizeof(settings->user_favorites[i].name), "name%02d", i + 1);
        settings->user_favorites[i].customize[0] = '\0';
        settings->user_favorites[i].steps_custom[0] = '\0';
        settings->user_favorites[i].flag = 0;  // 默认收藏为0
        settings->user_favorites[i].temperature = 0;  /// 默认温度设为0
        settings->user_favorites[i].time = 0;  // 默认时间设为0
    }

    for (int i = 0; i < YK_MODE_COUNT; i++) {
        settings->food_customize[i][0] = '\0';
        settings->food_steps_custom[i][0] = '\0';
    }
}


// 文件读取初始化：读取持久化设置，不合法则恢复默认并写回
void yk_f_file_flash_init()  //文件读取初始化函数
{
	yk_settings_init(&yk_settings);
    yk_f_file_rw_init((uint8_t*)&yk_settings,sizeof(yk_struct_settings));
    if (!yk_settings_data_valid(&yk_settings)) {
        // 数据不合法：恢复默认并写回
        yk_settings_init(&yk_settings);
        yk_f_file_w((uint8_t*)&yk_settings, sizeof(yk_struct_settings));
    }

	yk_printf("yk_settings.sound == %d \n",yk_settings.sound);
	yk_printf("yk_settings.display_mode == %d \n",yk_settings.display_mode);
	yk_printf("yk_settings.f_to_c == %d \n",yk_settings.f_to_c);
	yk_printf("yk_settings.language == %d \n",yk_settings.language);
	yk_printf("yk_settings.default_portion == %d \n",yk_settings.default_portion);
	yk_printf("yk_settings.screen_brightness == %d \n",yk_settings.screen_brightness);
	yk_printf("yk_settings.clock_hour_system_24 == %d \n",yk_settings.clock_hour_system_24);
}



//创建一个延迟保存数据定时器的名字
static lv_timer_t *delay_save_data_timer = NULL;
// 延迟保存回调：写入设置并销毁定时器
static void delay_save_data_timer_cb(lv_timer_t *timer)
{
    yk_f_file_w((uint8_t*)&yk_settings,sizeof(yk_struct_settings));
    //删除定时器
    if (delay_save_data_timer)
    {
        lv_timer_del(delay_save_data_timer);
        delay_save_data_timer = NULL;
    }
}
// 启动延迟保存：合并短时间内的多次设置变更
void yk_delay_save_data_timer_start(void)
{
    //如果有定时器先删除定时器
    if (delay_save_data_timer)
    {
        lv_timer_del(delay_save_data_timer);
        delay_save_data_timer = NULL;
    }

    if (delay_save_data_timer == NULL)
    {
        delay_save_data_timer = lv_timer_create(delay_save_data_timer_cb, 2000, NULL);
    }
}

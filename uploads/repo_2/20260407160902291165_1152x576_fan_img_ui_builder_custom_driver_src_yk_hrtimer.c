#include "../apps.h"
#ifdef YK_USE_UI_BUILDER
/*
 * Copyright (c) 2022-2023, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: matteo <duanmt@artinchip.com>
 */
#if yk_hrtimer_h


// 控制发热丝开关频率的布尔值，用于切换加热状态
static bool yk_io_set_heating_on_value = false;
// 发热丝PWM参数
#define G_HEATING_PERIOD_US 84                       // 发热丝PWM周期(微秒)
#define G_HEATING_DUTY_US   (G_HEATING_PERIOD_US * 0.5)  // 发热丝PWM占空比70%(微秒)
static bool g_heating_pwm_state = false; // 发热丝PWM当前状态

// 控制风扇开关频率的布尔值，用于切换风扇状态
static bool yk_io_set_fan_on_value = false;
// 风扇PWM常量参数
#define G_FAN_PERIOD_US  84                         // 风扇PWM周期(微秒)
#define G_FAN_DUTY_US    (G_FAN_PERIOD_US*0.5)        // 风扇PWM占空比(微秒)
static bool g_fan_pwm_state = false;     // 风扇PWM当前状态

#if YK_DC_AC == 1
static char *argv0[] = {"test_hrtimer", "-m", "period", "-c", "0", "-s", "0", "-u", "50"};
static char *argv1[] = {"test_hrtimer", "-m", "period", "-c", "1", "-s", "0", "-u", "50"};
#else
static char *argv0[] = {"test_hrtimer", "-m", "period", "-c", "0", "-s", "0", "-u", "50"};
static char *argv1[] = {"test_hrtimer", "-m", "period", "-c", "1", "-s", "0", "-u", "25"};
#endif



#include <stdlib.h>
#include <posix/string.h>
#include <getopt.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "aic_core.h"
#include "drv_hrtimer.h"
#include "boot_param.h"

#ifndef USEC_PER_SEC
#define USEC_PER_SEC            1000000
#endif

// 定义定时器最大运行时间（60秒）
#define HRTIMER_MAX_ELAPSE      (60 * USEC_PER_SEC)

// 存储定时器设备指针的数组，用于管理多个定时器通道
static rt_device_t g_hrtimer_dev[AIC_HRTIMER_CH_NUM] = {RT_NULL};

// 调试标志，用于控制是否输出调试信息
static u32 g_debug = 0;

// 存储每个通道的最大循环次数
static u32 g_loop_max[AIC_HRTIMER_CH_NUM] = {0};

// 存储每个通道的当前循环计数
static u32 g_loop_cnt[AIC_HRTIMER_CH_NUM] = {0};

// 记录开始时间，用于计算时间间隔
static ulong g_start_us = 0;

// 命令行选项短格式定义
static const char sopts[] = "m:c:s:u:dh";

// 命令行选项长格式定义
static const struct option lopts[] = {
    {"mode",        required_argument, NULL, 'm'},
    {"channel",     required_argument, NULL, 'c'},
    {"sec",         required_argument, NULL, 's'},
    {"microsecond", required_argument, NULL, 'u'},
    {"debug",             no_argument, NULL, 'v'},
    {"usage",             no_argument, NULL, 'h'},
    {0, 0, 0, 0}
};

/**
 * @brief 打印使用说明
 * @param program 程序名称
 */
static void usage(char *program)
{
    yk_printf("Usage: %s [options]: \n", program);
    yk_printf("\t -m, --mode\t\t定时器模式，可选: oneshot(单次)/period(周期)\n");
    yk_printf("\t -c, --channel\t\t定时器通道号 [0, %d] \n", AIC_HRTIMER_CH_NUM - 1);
    yk_printf("\t -s, --second\t\t定时器秒数 (必须 > 0) \n");
    yk_printf("\t -u, --microsecond\t定时器微秒数 (必须 > 0) \n");
    yk_printf("\t -d, --debug\t\t显示超时日志\n");
    yk_printf("\t -h, --usage \n");
    yk_printf("\n");
    yk_printf("示例: %s -m oneshot -c 0 -s 2 -u 3 \n", program);
}

/**
 * @brief 定时器超时回调函数
 * @param dev 定时器设备指针
 * @param size 数据大小（未使用）
 * @return 错误码
 */
static rt_err_t hrtimer_cb(rt_device_t dev, rt_size_t size)
{
    // 获取定时器设备对应的通道信息
    struct hrtimer_info *info = (struct hrtimer_info *)dev->user_data;
    rt_hwtimerval_t next_timeout = {0};

#ifdef ULOG_USING_ISR_LOG
    if (g_debug)
        yk_printf("%d/%d hrtimer%d timeout callback! Elapsed %ld us\n",
               g_loop_cnt[info->id], g_loop_max[info->id],
               info->id, aic_timer_get_us() - g_start_us);
#endif
    
    // 根据通道号分别控制发热丝或风扇
    if (info->id == YK_HRTIMER_HEATING_CH) {
        #if 0
        // 控制发热丝开关
        yk_io_set_heating_on_value = !yk_io_set_heating_on_value;
        yk_io_set_heating(yk_io_set_heating_on_value);
        // yk_printf("发热丝通道 %d 触发，当前状态: %s\n", 
        //        info->id, yk_io_set_heating_on_value ? "开启" : "关闭");
        #if YK_DC_AC == 1
        #else
        // 对于发热丝，继续使用固定周期
        next_timeout.sec = 0;
        next_timeout.usec = 42;  // 固定50微秒周期
        #endif
        #endif

        // 控制发热丝PWM (50%占空比)
        if (g_heating_pwm_state) {
            // 当前为高电平，切换到低电平
            yk_io_set_heating(0);
            g_heating_pwm_state = false;
            // 设置下一个低电平持续时间 = 周期 - 占空比
            uint32_t low_time = G_HEATING_PERIOD_US - G_HEATING_DUTY_US;
            next_timeout.sec = 0;
            next_timeout.usec = low_time;
        } else {
            // 当前为低电平，切换到高电平
            yk_io_set_heating(1);
            g_heating_pwm_state = true;
            // 设置下一个高电平持续时间 = 占空比
            next_timeout.sec = 0;
            next_timeout.usec = G_HEATING_DUTY_US;
        }
    } else if (info->id == YK_HRTIMER_FAN_CH) {
        #if YK_DC_AC == 1
        yk_io_set_fan_on_value = !yk_io_set_fan_on_value;
        yk_io_set_fan(yk_io_set_fan_on_value);
        // 对于风扇，继续使用固定周期
        next_timeout.sec = 0;
        next_timeout.usec = 42;  // 固定50微秒周期
        #else
        // 控制风扇PWM
        if (g_fan_pwm_state) {
            // 当前为高电平，切换到低电平
            yk_io_set_fan(0);
            g_fan_pwm_state = false;
            // 设置下一个低电平持续时间 = 周期 - 占空比
            uint32_t low_time = G_FAN_PERIOD_US - G_FAN_DUTY_US;
            next_timeout.sec = 0;
            next_timeout.usec = low_time;
            // yk_printf("风扇PWM: 切换到低电平，持续%d微秒\n", low_time);
        } else {
            // 当前为低电平，切换到高电平
            yk_io_set_fan(1);
            g_fan_pwm_state = true;
            // 设置下一个高电平持续时间 = 占空比
            next_timeout.sec = 0;
            next_timeout.usec = G_FAN_DUTY_US;
            // yk_printf("风扇PWM: 切换到高电平，持续%d微秒\n", G_FAN_DUTY_US);
        }
        #endif
    }

    
    // 设置下一次超时时间以实现PWM
    if (!rt_device_write(dev, 0, &next_timeout, sizeof(next_timeout))) {
        pr_err("设置下一次超时值失败\n");
    }
    
    #if 0
    // 更新计数器和时间戳
    g_start_us = aic_timer_get_us();
    g_loop_cnt[info->id]++;
    yk_printf("定时器通道 %d 触发，当前循环计数 == %d\n", info->id, g_loop_cnt[info->id]);

    // // 如果达到最大循环次数，则停止定时器
    if ((g_loop_max[info->id] > 1) && (g_loop_cnt[info->id] > g_loop_max[info->id])) {
        rt_device_control(g_hrtimer_dev[info->id], HWTIMER_CTRL_STOP, NULL);
    }
    #endif

    return RT_EOK;
}

/**
 * @brief 配置并启动定时器
 * @param argc 参数个数
 * @param argv 参数数组
 */
static void cmd_test_hrtimer(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    u32 c, ch = 0;
    rt_hwtimerval_t tm = {0};
    rt_hwtimer_mode_t mode = HWTIMER_MODE_ONESHOT;

    optind = 0;
    while ((c = getopt_long(argc, argv, sopts, lopts, NULL)) != -1) {
        switch (c) {
        case 'm':
            // 设置定时器模式（单次或周期）
            if (strncasecmp("period", optarg, strlen(optarg)) == 0)
                mode = HWTIMER_MODE_PERIOD;
            continue;

        case 'c':
            // 设置定时器通道号
            ch = atoi(optarg);
            if (ch >= AIC_HRTIMER_CH_NUM) {
                pr_err("通道号 %s 无效\n", optarg);
                return;
            }
            continue;

        case 's':
            // 设置秒数
            tm.sec = atoi(optarg);
            continue;

        case 'u':
            // 设置微秒数
            tm.usec = atoi(optarg);
            continue;

        case 'd':
            // 开启调试模式
            g_debug = 1;
            continue;

        case 'h':
            // 显示帮助信息
            usage(argv[0]);
            return;

        default:
            pr_err("无效参数\n");
            usage(argv[0]);
            return;
        }
    }

    // 检查定时器时间设置是否有效
    if ((tm.sec == 0) && (tm.usec == 0)) {
        pr_err("无效参数\n");
        usage(argv[0]);
        return;
    }

    // 查找并初始化定时器设备
    if (!g_hrtimer_dev[ch]) {
        char name[10] = "";

        snprintf(name, 10, "hrtimer%d", ch);
        /* 查找定时器设备 */
        g_hrtimer_dev[ch] = rt_device_find(name);
        if (g_hrtimer_dev[ch] == RT_NULL) {
            pr_err("找不到设备 %s!\n", name);
            return;
        }

        /* 以读写模式打开设备 */
        ret = rt_device_open(g_hrtimer_dev[ch], RT_DEVICE_OFLAG_RDWR);
        if (ret != RT_EOK) {
            pr_err("打开设备 %s 失败!\n", name);
            return;
        }
    }

    /* 设置超时回调函数 */
    rt_device_set_rx_indicate(g_hrtimer_dev[ch], hrtimer_cb);

    // 设置定时器工作模式
    ret = rt_device_control(g_hrtimer_dev[ch], HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != RT_EOK) {
        pr_err("设置模式失败! 错误码: %d\n", ret);
        return;
    }

    yk_printf("定时器通道%d: 创建定时器，周期 %d.%06d 秒, 工作模式: %s\n",
           ch, (u32)tm.sec, (u32)tm.usec,
           mode == HWTIMER_MODE_ONESHOT ? "单次触发" : "周期触发");
           
    // 如果是周期模式，计算最大循环次数
    if (mode != HWTIMER_MODE_ONESHOT) {
        g_loop_max[ch] = HRTIMER_MAX_ELAPSE / (tm.sec * USEC_PER_SEC + tm.usec);
        yk_printf("\t将循环 %d 次\n", g_loop_max[ch]);
    }
    
    // 初始化计数器和时间戳
    g_loop_cnt[ch] = 0;
    g_start_us = aic_timer_get_us();
    
    // 启动定时器
    if (!rt_device_write(g_hrtimer_dev[ch], 0, &tm, sizeof(tm))) {
        pr_err("设置超时值失败\n");
        return;
    }
}

/**
 * @brief 启动发热丝和风扇控制定时器（分别控制两个设备）
 */
void yk_hrtimer_open(uint8_t ch)
{
    yk_printf("yk_hrtimer_open - 分别启动发热丝和风扇控制定时器\n");

    switch (ch) {
        case YK_HRTIMER_HEATING_CH:
            // 启动通道0定时器（主加热控制通道）
            cmd_test_hrtimer(9, argv0);
            break;
        case YK_HRTIMER_FAN_CH:
            if(YK_DC_AC)return;  //如果是直流电机不需要风扇定时器
            if (G_FAN_PERIOD_US - G_FAN_DUTY_US == 0)return;  // 如果风扇周期和占空比相等，则不需要定时器
            // 启动通道1定时器（辅助控制通道）
            cmd_test_hrtimer(9, argv1);
            break;
        default:
            yk_printf("无效的定时器通道号: %d\n", ch);
        }
}

/**
 * @brief 关闭发热丝和风扇控制定时器（分别关闭两个设备）
 */
void yk_hrtimer_close(uint8_t ch)
{
    yk_printf("yk_hrtimer_close - 分别关闭发热丝和风扇控制定时器\n");
    
    switch (ch) {
    case YK_HRTIMER_HEATING_CH:
        // 关闭发热丝控制定时器
        if (g_hrtimer_dev[YK_HRTIMER_HEATING_CH]) {
            yk_printf("关闭发热丝控制定时器（通道%d）\n", YK_HRTIMER_HEATING_CH);
            rt_device_control(g_hrtimer_dev[YK_HRTIMER_HEATING_CH], HWTIMER_CTRL_STOP, NULL);
            yk_io_set_heating(0);  // 确保发热丝关闭
            yk_printf("发热丝已关闭\n");
        }
        break;
    case YK_HRTIMER_FAN_CH:
        if(YK_DC_AC)return;
        if (G_FAN_PERIOD_US - G_FAN_DUTY_US == 0)return;  // 如果风扇周期和占空比相等，则不需要定时器
        
        // 关闭风扇控制定时器
        if (g_hrtimer_dev[YK_HRTIMER_FAN_CH]) {
            yk_printf("关闭风扇控制定时器（通道%d）\n", YK_HRTIMER_FAN_CH);
            rt_device_control(g_hrtimer_dev[YK_HRTIMER_FAN_CH], HWTIMER_CTRL_STOP, NULL);
            yk_io_set_fan(0);  // 确保风扇关闭
            yk_printf("风扇已关闭\n");
        }
        break;
    default:
        yk_printf("无效的定时器通道号: %d\n", ch);
    }
}

// 在应用程序初始化时自动导出并执行yk_hrtimer_open函数
//INIT_APP_EXPORT(yk_hrtimer_open);
#endif


#endif  //YK_USE_UI_BUILDER
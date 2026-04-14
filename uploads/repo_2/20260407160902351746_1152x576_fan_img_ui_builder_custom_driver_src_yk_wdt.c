#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#include <rtthread.h>
#include <aic_core.h>
#include <drivers/watchdog.h>
#include <aic_drv_wdt.h>
#include <hal_wdt.h>
#include <getopt.h>

static void yk_wdt_feed_thread_entry(void *parameter)
{
    rt_device_t wdt_dev = RT_NULL;
    do {
        wdt_dev = rt_device_find("wdt");
        //printf("wdt_dev:%p\n", wdt_dev);
        rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
        rt_thread_mdelay(2000);  //每隔2000毫秒喂一次狗
    } while (wdt_dev);
}


//初始化超时时间
void yk_wdt_inti()
{
    int timeout = 5;  //设置超时时间5秒
    rt_device_t wdt_dev = RT_NULL;
    rt_thread_t wdt_thread = RT_NULL;

    wdt_dev = rt_device_find("wdt");
    rt_device_init(wdt_dev);

    rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    rt_kprintf("set timeout:%d\n", timeout);

    wdt_thread = rt_thread_create("wdt_feed_thread", yk_wdt_feed_thread_entry,RT_NULL, 1024, 10, 10);
    if (wdt_thread != RT_NULL) {
        rt_thread_startup(wdt_thread);
        rt_kprintf("keep feeding the dog!\n");
    } else {
        rt_kprintf("wdt thread create fail!\n");
    }
}


#endif  //YK_USE_UI_BUILDER
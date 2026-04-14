#include "../apps.h"
#ifdef YK_USE_UI_BUILDER
/*
 * Copyright (c) 2023-2024, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#if yk_test_uart_flowctrl_h


#include <rtthread.h>
#include <rtdevice.h>
#include <aic_core.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>

#define SAMPLE_UART_NAME       "uart7"
#define MAX_BUFFER_LEN       1200

struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};

static char uart_name[RT_NAME_MAX] = SAMPLE_UART_NAME;
rt_device_t fc_serial;
static struct rt_semaphore rx_fc_sem;
static int fc_g_exit = 0;
static uint32_t baud = 0;


rt_err_t uart_input_set(rt_device_t dev, rt_size_t size)
{
    if (size > 0)
        rt_sem_release(&rx_fc_sem);

    return RT_EOK;
}


void serial_normal_test_entry(void *parameter)
{
    #if 0
    u8 databuf[MAX_BUFFER_LEN] = {0};

    char ch;
    int ret = 0;
    char str_receive[MAX_BUFFER_LEN] = {0};
    int index = 0;

    while (1) {
        ret = rt_device_read(fc_serial, 0, &ch, 1);
        if (ret) {
            str_receive[index] = ch;
            index ++;
            //uart_receive_input(ch);
        } else {
            //rt_kprintf("waiting for uart input index== %d\n",index);
            if (index)
            {
                uart_receive_buff_input(&str_receive, index);
                //receive_data_handle(&str_receive,index);
                index = 0;
            }
            rt_sem_take(&rx_fc_sem, RT_WAITING_FOREVER);
        }
    }

    rt_sem_detach(&rx_fc_sem);
    fc_g_exit = 1;
    #endif

    #if 1
    // 增大缓冲区以适应大数据传输
    char str_receive[MAX_BUFFER_LEN] = {0};
    int index = 0;
    int read_len = 0;
    
    // 设置每次读取的最大长度，留一些余量防止溢出
    int max_read_len = MAX_BUFFER_LEN - 100;

    while (1) {
        // 批量读取数据，提高效率
        read_len = rt_device_read(fc_serial, 0, &str_receive[index], 
                                 max_read_len - index);
        
        if (read_len > 0) {
            index += read_len;
            // 如果缓冲区接近满载，立即处理数据
            if (index >= (max_read_len)) {
                uart_receive_buff_input((unsigned char*)str_receive, index);
                //receive_data_handle(&str_receive,index);
                index = 0;
            }
        } else {
            // 没有更多数据可读时，处理已接收的数据
            if (index > 0) {
                uart_receive_buff_input((unsigned char*)str_receive, index);
                //receive_data_handle(&str_receive,index);
                index = 0;
            }
            // 等待新的数据到达
            rt_sem_take(&rx_fc_sem, RT_WAITING_FOREVER);
        }
    }

    rt_sem_detach(&rx_fc_sem);
    fc_g_exit = 1;
    #endif
}


void uart_normal_test (void)
{
    printf("ytl-test00_01-uart_normal_test (void) \n");
    fc_serial = rt_device_find(uart_name);
    if (!fc_serial) {
        rt_kprintf("find %s failed!\n", uart_name);
        return;
    }

    rt_sem_init(&rx_fc_sem, "fc_rx_sem", 0, RT_IPC_FLAG_FIFO);

    if (rt_device_open(fc_serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX) != RT_EOK) {
        rt_kprintf("open failed!\n");
        return;
    }

    if (rt_device_set_rx_indicate(fc_serial, uart_input_set)!= RT_EOK) {
        rt_kprintf("indicate failed!\n");
        return;
    }

    rt_thread_t thread = rt_thread_create("fc_normal_serial", serial_normal_test_entry,
                                            RT_NULL, 1024 * 8, 18, 10);
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    } else {
        rt_kprintf("thread create fail!\n");
    }
}


static int test_uart_flowctrl(int argc, char *argv[])
{
    printf("ytl-test00-test_uart_flowctrl(int argc, char *argv[]) \n");
    uart_normal_test();
    return 0;
}
INIT_APP_EXPORT(test_uart_flowctrl);


#endif


#endif  //YK_USE_UI_BUILDER
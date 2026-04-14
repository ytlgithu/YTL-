#include "../apps.h"
#ifdef YK_USE_UI_BUILDER
/*
 * Copyright (c) 2022-2024, ShuangYu Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: ALS <1251525679@qq.com>
 */
#if yk_stkdriver_h


#include <stdio.h>
#include <stdint.h>
#include <drivers/i2c.h>
#include <drivers/pin.h>

#include "lvgl.h"

#include "../inc/STKdriver.h"
#include "aic_core.h"


static lv_timer_t *iic_als_task_change = NULL;  //执行屏幕亮度慢慢加大或减小


struct rt_i2c_bus_device *i2c_yk = NULL;


uint8_t set_brightness_value = 100;  //设置亮度值
static uint8_t set_brightness_value_change = 100;  //设置亮度值改变才去设置屏幕亮度
static uint8_t set_brightness_value_change_i = 0;  //设置亮度值改变才去设置屏幕亮度 渐变亮度设置
static s8 v1 = 0;  //取亮度的差值变量
static uint8_t v2 = 0;  //设置渐变亮度变量


//设置屏幕亮度慢慢亮暗删除
static void yk_f_set_brightness_del()
{
	if (iic_als_task_change) {
		lv_timer_del(iic_als_task_change);
		iic_als_task_change = NULL;
		if (v1 > 0) {
			set_brightness_value_change -= v2;
			printf("set_brightness_value->set_brightness_value_change - == %d \n",set_brightness_value_change);
		}
		else if (v1 == 0)
		{
			/* code */
		}
		else {
			set_brightness_value_change += v2;
			printf("set_brightness_value->set_brightness_value_change + == %d \n",set_brightness_value_change);
		}
		v2 = 0;
	}
}

static void iic_als_task_change_cb()
{
	v2++;
	printf("v2 == %d \n",v2);
	if (v1 > 0) {
		yk_func_set_pwm_brightness(set_brightness_value_change - v2);
		if (v2 == v1) {
			yk_f_set_brightness_del();
			set_brightness_value_change_i = 0;
		}
	}
	else if (v1 == 0)
	{
		yk_f_set_brightness_del();
		set_brightness_value_change_i = 0;
	}
	else {
		yk_func_set_pwm_brightness(set_brightness_value_change + v2);
		if (v2 == -v1) {
			yk_f_set_brightness_del();
			set_brightness_value_change_i = 0;
		}
	}
}

//设置屏幕亮度慢慢亮暗创建
void yk_f_set_brightness_create(uint8_t set_brightness_value_01)
{

	//先判断是否有任务在执行，有任务执行就先删除任务
	if(iic_als_task_change){
		yk_f_set_brightness_del();
	}
	//然后再创建任务
	if(iic_als_task_change == NULL)
	{
		set_brightness_value = set_brightness_value_01;
		v1 = set_brightness_value_change - set_brightness_value;

		printf("set_brightness_value_change =00= %d \n",set_brightness_value_change);
		printf("set_brightness_value =00= %d \n",set_brightness_value);
		printf("v1 =00= %d \n",v1);

		printf("iic_als_task_change =00= %d \n",iic_als_task_change);
		iic_als_task_change = lv_timer_create(iic_als_task_change_cb,40,NULL);
	}
		
}

//根据获取到的感光强度，设置屏幕亮度
static void set_brightness(u16 als_data)
{
//读取光感强度值
	//u16 als_data;
	u16 als_data_max_value = 65535*0.16;  //设置光感强度最高值  感应最大值是：65535
	//printf("als_data_max_value == %d \n",als_data_max_value);
	//获取光感强度值
	//als_data = ((rbuff[0x13]<<8)|rbuff[0x14]);  //ALS_DATA1+ALS_DATA2

	//判断光感强度值范围,确定亮度值
	u8 als_data_v = 1;
	for (int i = 0; i < 10*als_data_v; ++i)
	{
		if (i < 10*als_data_v-1)
		{
			if (als_data > als_data_max_value*0.1/als_data_v*i && als_data <= als_data_max_value*0.1/als_data_v*(i+1))
			{
				set_brightness_value = 10/als_data_v*(i+1);
			}
			if (als_data > als_data_max_value*0.1/als_data_v*i && als_data <= als_data_max_value*0.1/als_data_v*(i+1))
			{

				if (als_data <= 1834) {
					set_brightness_value = 10;
				}else {
					set_brightness_value = 10/als_data_v*(i+1);
				}
				u16 set_brightness_value_0;
				u16 set_brightness_value_1;
				set_brightness_value_0 = als_data_max_value*0.1/als_data_v*i;
				set_brightness_value_1 = als_data_max_value*0.1/als_data_v*(i+1);
				//printf("亮度：%d 范围%d < %d <= %d \n",set_brightness_value,set_brightness_value_0,als_data,set_brightness_value_1);
			}
		}
		else
		{
			if (als_data > als_data_max_value*0.1/als_data_v*(als_data_v*10-1))
			{
				set_brightness_value = 100;
				u16 set_brightness_value_0;
				set_brightness_value_0 = als_data_max_value*0.1/als_data_v*(als_data_v*10-1);
				//printf("亮度：%d 范围%d < %d \n",set_brightness_value,set_brightness_value_0,als_data);
			}
		}
	}

	//判断当前光感强度平稳超过3秒时执行亮度设置
	if (set_brightness_value_change != set_brightness_value)
	{
		printf("set_brightness_value_change_i == %d \n",set_brightness_value_change_i);
		if (set_brightness_value_change_i < 2) {
			set_brightness_value_change_i++;

		}
		else if(set_brightness_value_change_i == 2){
			set_brightness_value_change_i = 3;
			//设置屏幕亮度慢慢亮暗
			yk_f_set_brightness_create(set_brightness_value);
		}
	}
	else {
		set_brightness_value_change_i = 0;
	}
}

static u16  read_als_data(struct rt_i2c_bus_device *i2c);
static void my_timer_callback_cb(lv_timer_t * timer) {
    // 你的代码逻辑
    //printf("Your code logic =================2024/11/18\n");
	if (yk_v_screen_id != SCREEN_3_SCREENSAVER_ID)
	{
		u16 als_data = read_als_data(i2c_yk);
    	set_brightness(als_data);
	}
}


//reg:读取的寄存器
//val:读取的值
static int stk3311_read_reg(struct rt_i2c_bus_device *i2c, u8 reg, u8 *val)
{
    if (rt_i2c_read_reg(i2c, STK3311_DEFAULT_SLAVE_ID, reg, val, 1) != 1)
    {
        LOG_E("%s: error: reg = 0x%x, val = 0x%x", __func__, reg, *val);
        return -1;
    }

    return 0;
}

//reg:写入的寄存器
//val：写入寄存器的值
static int stk3311_write_reg(struct rt_i2c_bus_device *i2c, u8 reg, u8 val)
{
    if (rt_i2c_write_reg(i2c, STK3311_DEFAULT_SLAVE_ID, reg, &val, 1) != 1) {
        LOG_E("%s: error: reg = 0x%x, val = 0x%x", __func__, reg, val);
        return -1;
    }

    return 0;
}

void STK_cheek_product_id(struct rt_i2c_bus_device *i2c)
{   
    u8 id = 0;
    stk3311_read_reg(i2c, STK3311_PRODUCT_ID, &id);
    LOG_E("Invalid chip ID: %02x\n", id);
}

void STK_enable_ALS(struct rt_i2c_bus_device *i2c)
{
   
   //读产品的id值
   STK_cheek_product_id(i2c);
   //开启光线传感器
   stk3311_write_reg(i2c,STK3311_STATE_REG,0x02);
    return;
}

void STK_DisEnable_ALS(struct rt_i2c_bus_device *i2c)
{
    //关闭传感器
    stk3311_write_reg(i2c,STK3311_STATE_REG,(~0x02));
}

u16 STK_READ_ALS(struct rt_i2c_bus_device *i2c)
{
    u16 als_data = 0;
    u8 als_data1 = 0;
    u8 als_data2 = 0;

    stk3311_read_reg(i2c,STK3311_ASL_DATA1_REG,&als_data1);
    stk3311_read_reg(i2c,STK3311_ASL_DATA2_REG,&als_data2);
    als_data = (als_data1 << 8) | (als_data2);
    return als_data;
}

static u16  read_als_data(struct rt_i2c_bus_device *i2c)
{
    u8 flag = 0;
    u16 als_data = 0;

    //读取标准位
    stk3311_read_reg(i2c,STK3311_FLAG_REG,&flag);
    //LOG_E("Invalid chip FLAG: 0x%x\n",flag);
    //读取环境光数值
    als_data = STK_READ_ALS(i2c);
    //LOG_E("read_als_data  = %d\n",als_data);
    
    return als_data;
}

void STK_init_ALS(void)
{
	printf("STK_init_ALS() \n");
    i2c_yk = rt_i2c_bus_device_find(STK3311_I2C_NAME);
    STK_enable_ALS(i2c_yk);
    // 创建定时器
    lv_timer_t * timer = lv_timer_create(my_timer_callback_cb, 1000, NULL); // 每1000毫秒调用一次
    

    return;
}
//INIT_APP_EXPORT(STK_init_ALS);


#endif


#endif  //YK_USE_UI_BUILDER
#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#include "../apps.h"
#if yk_gpio_h


#include <drivers/pin.h>
#include <aic_common.h>


static rt_base_t pin_fryer = 0;  //获取锅是否插入高低电平 POWER-DEF:39 PA.2
static rt_base_t pin_heating = 0;  //控制发热丝高低电平 T1:40 PA.3
//static rt_base_t pin_NTC = 0;  //获取NTC即ADC的值 T2:41 PA.4
static rt_base_t pin_fan = 0;  //控制风扇高低电平 T3:42 PA.5
static rt_base_t pin_lamp = 0;  //控制灯高低电平 lamp:39 PC.0
static rt_base_t pin_lamp_pe8 = 0;  //控制灯高低电平 lamp:39 PE.8
static rt_base_t pin_beep = 0;  //控制蜂鸣器高低电平 BEEP:43 PE11


static void POWER_BOARD_TEST_MODE_gpio_init();


//蜂鸣器IO：设置高低电平
void yk_io_set_beep(uint8_t state_value)
{
	//蜂鸣器
	rt_pin_write(pin_beep, state_value);
	printf("蜂鸣器高低电平: set pin %d\n",state_value);
}

//风扇IO：设置高低电平
void yk_io_set_fan(uint8_t state_value)
{
    rt_pin_write(pin_fan, state_value);
	//printf("设置风扇高低电平: set pin %d\n",state_value);
}

// void yk_io_set_NTC(uint8_t state_value)
// {
// 	rt_pin_write(pin_NTC, state_value);
// 	printf("设置NTC光高低电平: set pin %d\n",state_value);
// }

void yk_io_set_heating(uint8_t state_value)
{
	rt_pin_write(pin_heating, state_value);
	//printf("设置发热丝高低电平: set pin %d\n",state_value);
}

//照明灯IO：设置高低电平（同步两个引脚）
void yk_io_set_lamp(uint8_t state_value)
{
	rt_pin_write(pin_lamp, state_value);
	rt_pin_write(pin_lamp_pe8, state_value);
	printf("设置灯高低电平: set pin %d\n",state_value);
}

//锅在位检测：读取锅插入引脚电平
uint8_t yk_io_get_fryer()
{
	// uint8_t pinvalue=grf_gpio_readInputPin(GRF_GPIO_GROUP_A,4);
	//printf("获取锅高低电平：Get pin pinvalue=%d\n",pinvalue);
    uint8_t pinvalue=rt_pin_read(pin_fryer);
    //printf("获取锅高低电平：Get pin pinvalue=%d\n",pinvalue);
	return pinvalue;
}


//初始化IO口
void yk_f_gpio_init()
{
    printf("yk_f_gpio_init() \n");

	//输出/输入引脚初始化：锅在位输入、发热/风扇/灯/蜂鸣器输出
    pin_fryer = rt_pin_get("PA.2");
    rt_pin_mode(pin_fryer, PIN_MODE_INPUT);

    pin_heating = rt_pin_get("PA.3");
    rt_pin_mode(pin_heating, PIN_MODE_OUTPUT);

    // pin_NTC = rt_pin_get("PA.4");
    // rt_pin_mode(pin_NTC, PIN_MODE_OUTPUT);

    pin_fan = rt_pin_get("PA.5");
    rt_pin_mode(pin_fan, PIN_MODE_OUTPUT);

    pin_lamp = rt_pin_get("PC.0");
    rt_pin_mode(pin_lamp, PIN_MODE_OUTPUT);

    pin_lamp_pe8 = rt_pin_get("PE.8");
    rt_pin_mode(pin_lamp_pe8, PIN_MODE_OUTPUT);
    
    pin_beep = rt_pin_get("PE.11");
    rt_pin_mode(pin_beep, PIN_MODE_OUTPUT);

    #if POWER_BOARD_TEST_MODE
    POWER_BOARD_TEST_MODE_gpio_init();
    #endif
}
//INIT_APP_EXPORT(yk_func_gpio_init);

//USB HID 初始化：这里复用为灯引脚初始化（历史遗留调试入口）
void yk_usbd_hid_keyboard_init()
{
    rt_kprintf("115 yk_usbd_hid_keyboard_init\n");
    #if 1
    //控制灯高低电平 lamp:39 PC.0
    rt_base_t pin_lamp = 0;
    pin_lamp = rt_pin_get("PC.0");
    printf("pin_lamp == %ld\n",pin_lamp);
    rt_pin_mode(pin_lamp, PIN_MODE_OUTPUT);
    rt_pin_write(pin_lamp, 0);
    #endif
}
INIT_DEVICE_EXPORT(yk_usbd_hid_keyboard_init);

#if 1
static void yk_gpio_init1()
{
    printf("yk_gpio_init1() \n");
}

static void yk_gpio_init2()
{
    printf("yk_gpio_init2() \n");
}

static void yk_gpio_init3()
{
    printf("yk_gpio_init3() \n");
}

static void yk_gpio_init4()
{
    printf("yk_gpio_init4() \n");
}

static void yk_gpio_init5()
{
    printf("yk_gpio_init5() \n");
}

static void yk_gpio_init6()
{
    printf("yk_gpio_init6() \n");
}

static void yk_gpio_init7()
{
    printf("yk_gpio_init7() \n");
}


// INIT_BOARD_EXPORT(yk_gpio_init1);  //1
// INIT_PREV_EXPORT(yk_gpio_init2);  //2
// INIT_DEVICE_EXPORT(yk_gpio_init3);  //3
// INIT_COMPONENT_EXPORT(yk_gpio_init4);  //4
// INIT_ENV_EXPORT(yk_gpio_init5);  //5
// INIT_APP_EXPORT(yk_gpio_init6);  //6
// INIT_LATE_APP_EXPORT(yk_gpio_init7);  //7
#endif

#endif


//测试模式PCBA

//获取风扇引脚高低电平
uint8_t yk_io_get_fan_test_mode()
{
    uint8_t pinvalue=rt_pin_read(pin_fan);
	return pinvalue;
}

//获取灯引脚高低电平
uint8_t yk_io_get_lamp_pe8_test_mode()
{
    uint8_t pinvalue=rt_pin_read(pin_lamp_pe8);
	return pinvalue;
}

//测试模式把风扇和灯改为输入模式
void yk_f_gpio_init_test_mode()
{
    printf("yk_f_gpio_init_test_mode() \n");
    pin_fan = rt_pin_get("PA.5");
    rt_pin_mode(pin_fan, PIN_MODE_INPUT);

    pin_lamp_pe8 = rt_pin_get("PE.8");
    rt_pin_mode(pin_lamp_pe8, PIN_MODE_INPUT);
}


//电源板测试模式
#if POWER_BOARD_TEST_MODE
static rt_base_t pin_pc_1 = 0;  //PC.1
static rt_base_t pin_pc_2 = 0;  //PC.2
static rt_base_t pin_pc_3 = 0;  //PC.3
static rt_base_t pin_pc_4 = 0;  //PC.4

//电源板测试模式初始化IO口
static void POWER_BOARD_TEST_MODE_gpio_init()
{
    printf("POWER_BOARD_TEST_MODE_gpio_init() \n");

    pin_pc_1 = rt_pin_get("PC.1");
    rt_pin_mode(pin_pc_1, PIN_MODE_INPUT);
    pin_pc_2 = rt_pin_get("PC.2");
    rt_pin_mode(pin_pc_2, PIN_MODE_INPUT);
    pin_pc_3 = rt_pin_get("PC.3");
    rt_pin_mode(pin_pc_3, PIN_MODE_INPUT);
    pin_pc_4 = rt_pin_get("PC.4");
    rt_pin_mode(pin_pc_4, PIN_MODE_INPUT);
}

//电源板测试模式获取PC.1引脚高低电平
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_1_lamp()
{
    uint8_t pinvalue=rt_pin_read(pin_pc_1);
	return pinvalue;
}

//电源板测试模式获取PC.2引脚高低电平
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_2_fan()
{
    uint8_t pinvalue=rt_pin_read(pin_pc_2);
	return pinvalue;
}

//电源板测试模式获取PC.3引脚高低电平
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_3_heating()
{
    uint8_t pinvalue=rt_pin_read(pin_pc_3);
	return pinvalue;
}

//电源板测试模式获取PC.4引脚高低电平
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_4_start()
{
    uint8_t pinvalue=rt_pin_read(pin_pc_4);
	return pinvalue;
}
#endif


#endif  //YK_USE_UI_BUILDER

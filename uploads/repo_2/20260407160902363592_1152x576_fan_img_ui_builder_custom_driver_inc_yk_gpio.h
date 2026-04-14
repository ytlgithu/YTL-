#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_gpio_h


#ifndef __YK_GPIO_H__
#define __YK_GPIO_H__


#include <rtdef.h>

void yk_f_gpio_init();
void yk_io_set_beep(uint8_t state_value);
void yk_io_set_fan(uint8_t state_value);
void yk_io_set_heating(uint8_t state_value);
void yk_io_set_lamp(uint8_t state_value);
uint8_t yk_io_get_fryer();


/*下面是测试模式自定义函数*/
uint8_t yk_io_get_fan_test_mode();
uint8_t yk_io_get_lamp_pe8_test_mode();
void yk_f_gpio_init_test_mode();


#include "custom.h"
#if  POWER_BOARD_TEST_MODE
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_1_lamp();
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_2_fan();
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_3_heating();
uint8_t POWER_BOARD_TEST_MODE_get_pin_pc_4_start();
#endif


#endif


#endif


#endif  //YK_USE_UI_BUILDER
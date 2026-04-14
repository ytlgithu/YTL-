#include "../apps.h"


#if yk_test_uart_flowctrl_h


#ifndef __YK_UART_RECEIVE_DATA_HANDLE_H__
#define __YK_UART_RECEIVE_DATA_HANDLE_H__


#ifdef YK_USE_UI_BUILDER
#include <rtthread.h>
#include <rtdevice.h>
#include <aic_core.h>
#else
extern uint8_t fc_serial;  //自定义串口设备句柄
#endif  //YK_USE_UI_BUILDER

#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>

#if 0
typedef enum {
	SCREEN_NULL,
	SCREEN_ID = 1,
	SCREEN_1_A_ID = 2,
	SCREEN_2_M_ID = 3,
	SCREEN_1_A_1_ID = 4,
	SCREEN_2_M_1_ID = 5,
	SCREEN_3_SCREENSAVER_ID = 6,
} yk_view_id_e;


extern uint8_t yk_v_screen_id;
extern uint8_t yk_v_tap_position;
extern uint8_t yk_v_operating_mode;

extern u8 a_mode_img_switchover_display[];

extern u8* a_array01[];
extern u8* a_array03[];
extern u8* a_array04[];
extern u8* a_array05[];
extern u8* a_array06[];
extern u8* a_array07[];
extern u8* a_array08[];


void yk_f_uart_send(u8 cmd,u8 data);
void yk_f_uart_send_query(u8 cmd,u8 data);

//自定义切换界面函数
void yk_view_set_dis_view(uint8_t v_id);

#endif

void yk_f_uart_send(unsigned char value);
void receive_data_handle(uint8_t* databuf,uint32_t datalen);

#endif


#endif
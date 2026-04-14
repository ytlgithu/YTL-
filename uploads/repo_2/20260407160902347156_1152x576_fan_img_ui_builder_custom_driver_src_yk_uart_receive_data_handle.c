#include "../apps.h"


#if yk_test_uart_flowctrl_h



//yk-ytl-uart-2024/11/29
#define HEAD_FH 0x55
#define HEAD_FL 0xAA
#define DATA_LEN 0x04
#define DATA_SEND 0x04  //串口发送
#define DATA_SEND_QUERY 0x06  //串口发送查询
#define RX_BUF_LEN   1024
static uint8_t RX_HAND_BUF[RX_BUF_LEN];

#define EXTERNAL_BUFFER_SIZE 100

#define GRF_OK    (0)
#define GRF_FAIL  (-1)

#if 0
//自定义全局变量
uint8_t yk_v_screen_id = 0;  //当前视图ID
uint8_t yk_v_tap_position = 0;//档位0 1 2 3 四个档位 默认0档位
uint8_t yk_v_operating_mode = 3;  //工作模式 0手动 1自动 2空气进化模式 3禁止工作
uint8_t a_mode_img_switchover_display[] = {1,2,1,1,1,1,1,1,1};//自动模式图片切换显示数组,0自动模式按左右按钮的标准位置，1档位，2滤芯, 3空气净化模式，4空气质量，5PM2.5, 6PM10, 7NO2, 8PETO AIR BEST

#if YK_V_O
uint8_t* a_array01[] = {"","",LVGL_IMAGE_PATH(A_01_0002.jpg),LVGL_IMAGE_PATH(A_01_0003_V.jpg)};//只有2档和3档 数组
#else
uint8_t* a_array01[] = {"","",LVGL_IMAGE_PATH(A_01_0002.jpg),LVGL_IMAGE_PATH(A_01_0003_O.jpg)};//只有2档和3档 数组
#endif

uint8_t* a_array03[] = {LVGL_IMAGE_PATH(A_03_0000.jpg),LVGL_IMAGE_PATH(A_03_0001.jpg),LVGL_IMAGE_PATH(A_03_0002.jpg),LVGL_IMAGE_PATH(A_03_0003.jpg)};//滤芯数组

uint8_t* a_array04[] = {"",LVGL_IMAGE_PATH(A_04_0001.jpg),LVGL_IMAGE_PATH(A_04_0002.jpg),LVGL_IMAGE_PATH(A_04_0003.jpg)};//PM2.5数组

uint8_t* a_array05[] = {"",LVGL_IMAGE_PATH(A_05_0001.jpg),LVGL_IMAGE_PATH(A_05_0002.jpg),LVGL_IMAGE_PATH(A_05_0003.jpg)};//PM10数组

uint8_t* a_array06[] = {"",LVGL_IMAGE_PATH(A_06_0001.jpg),LVGL_IMAGE_PATH(A_06_0002.jpg),LVGL_IMAGE_PATH(A_06_0003.jpg)};//NO2数组

uint8_t* a_array07[] = {"",LVGL_IMAGE_PATH(A_07_0001.jpg),LVGL_IMAGE_PATH(A_07_0002.jpg),LVGL_IMAGE_PATH(A_07_0003.jpg)};//空气指标 AIR QUALITY数组

uint8_t* a_array08[] = {"",LVGL_IMAGE_PATH(A_08_0001.jpg),LVGL_IMAGE_PATH(A_08_0002.jpg),LVGL_IMAGE_PATH(A_08_0003.jpg)};//PETO AIR数组

//自定义静态变量
static uint8_t cmdBuf[EXTERNAL_BUFFER_SIZE];  //获取指定需要的数据

//自定义切换界面函数
void yk_view_set_dis_view(uint8_t v_id)
{
	printf("yk_view_set_dis_view \n");
	lv_obj_t *act_scr = lv_scr_act();
	switch(v_id)
	{
		case SCREEN_ID:
			if (yk_v_screen_id != SCREEN_ID)
			{
				if (!screen_is_loading(act_scr))
				{
					yk_v_screen_id = v_id;
					screen_create(&ui_manager);
					lv_scr_load_anim(screen_get(&ui_manager)->obj, LV_SCR_LOAD_ANIM_FADE_ON, 0, 10, false);
				}
			}
			break;
		case SCREEN_1_A_ID:
			if (yk_v_screen_id != SCREEN_1_A_ID)
			{
				if (!screen_is_loading(act_scr))
				{
					//如果是从手动模式过来，就删除手动模式的动图任务
					if (yk_v_screen_id == SCREEN_2_M_1_ID)
					{
						yk_func_screen_2_M_1_task_del();
					}
					yk_v_screen_id = v_id;
					screen_1_A_create(&ui_manager);
					lv_scr_load_anim(screen_1_A_get(&ui_manager)->obj, LV_SCR_LOAD_ANIM_FADE_ON, 0, 10, false);
					if (YK_V_O)
					{
						/* code */
					}else
					{
						screen_3_screensaver_task_del();
					}
				}
			}
			break;
		case SCREEN_2_M_ID:
			if (yk_v_screen_id != SCREEN_2_M_ID)
			{
				if (!screen_is_loading(act_scr))
				{
					yk_v_screen_id = v_id;
					screen_2_M_create(&ui_manager);
					lv_scr_load_anim(screen_2_M_get(&ui_manager)->obj, LV_SCR_LOAD_ANIM_FADE_ON, 0, 10, false);
				}
			}
			break;
		case SCREEN_1_A_1_ID:
			if (yk_v_screen_id != SCREEN_1_A_1_ID)
			{
				if (!screen_is_loading(act_scr))
				{
					yk_v_screen_id = v_id;
					screen_1_A_1_create(&ui_manager);
					lv_scr_load_anim(screen_1_A_1_get(&ui_manager)->obj, LV_SCR_LOAD_ANIM_FADE_ON, 0, 10, false);
					yk_f_toggle_background(&ui_manager);
					if (YK_V_O)
					{
						/* code */
					}else
					{
						screen_3_screensaver_task_del();
					}
				}
			}else
			{
				yk_f_toggle_background(&ui_manager);
			}
			break;
		case SCREEN_2_M_1_ID:
			if (yk_v_screen_id != SCREEN_2_M_1_ID)
			{
				if (!screen_is_loading(act_scr))
				{
					yk_v_screen_id = v_id;
					screen_2_M_1_create(&ui_manager);
					lv_scr_load_anim(screen_2_M_1_get(&ui_manager)->obj, LV_SCR_LOAD_ANIM_FADE_ON, 0,10, false);
					//tap_position_switchover();
					if (yk_v_tap_position == 0)  //如果当前档位位0时，就把档位改为1
					{
						yk_v_tap_position = 1;
					}
					yk_func_screen_2_M_1_task_create();
					if (YK_V_O)
					{
						/* code */
					}else
					{
						screen_3_screensaver_task_del();
					}
					printf("yk_v_operating_mode == %d \n",yk_v_operating_mode);
				}
			}
			break;
		case SCREEN_3_SCREENSAVER_ID:
			if (yk_v_screen_id != SCREEN_3_SCREENSAVER_ID)
			{
				if (!screen_is_loading(act_scr))
				{
					//如果是从手动模式过来，就删除手动模式的动图任务
					if (yk_v_screen_id == SCREEN_2_M_1_ID)
					{
						yk_func_screen_2_M_1_task_del();
					}
					yk_v_screen_id = v_id;
					screen_3_screensaver_create(&ui_manager);
					lv_scr_load_anim(screen_3_screensaver_get(&ui_manager)->obj, LV_SCR_LOAD_ANIM_FADE_ON, 0, 10, false);
					if (YK_V_O)
					{
						/* code */
					}else
					{
						screen_3_screensaver_task_create();
					}
					//设置屏幕亮度慢慢亮暗
					yk_f_set_brightness_create(3);
					printf("yk_v_operating_mode == %d \n",yk_v_operating_mode);
				}
			}
			break;
	}
	
	printf("yk_v_screen_id == %d \n",yk_v_screen_id);
}


//CRC 校验
static uint16_t crc_cal_value(uint8_t* data_value,uint8_t data_length)
{
	int i;
	uint16_t crc_value = 0xffff;
	while(data_length--)
	{
		crc_value ^=*data_value++;
		for(i=0;i<8;i++)
		{
			if(crc_value&0x0001)
			crc_value=(crc_value>>1)^0xA001;
			else
			crc_value =crc_value >>1;
		}
	}
	printf("crc_value == %04x \n",crc_value);
	return crc_value;
}

static lv_timer_t *uart_send_task = NULL;
static uint8_t txdata[257] = {0};
static uint8_t uart_send_task_i = 0;
static void uart_send_task_cb()
{
	uart_send_task_i++;
	printf("uart_send_task_i == %d \n",uart_send_task_i);
	rt_device_write(fc_serial, 0, txdata, txdata[2]+5);
	if (uart_send_task_i >=2)
	{
		lv_timer_del(uart_send_task);
		uart_send_task = NULL;
	}
	
}
//自定义串口发送
void yk_f_uart_send(u8 cmd,u8 data)
{
    uint8_t i=0;
    txdata[i++] = HEAD_FH;
    txdata[i++] = HEAD_FL;
    txdata[i++] = DATA_LEN;
    txdata[i++] = DATA_SEND;
    txdata[i++] = cmd;
    txdata[i++] = (data >> 8) & 0xFF;
    txdata[i++] = data & 0xFF;
    uint16_t crc =crc_cal_value(&txdata[3],txdata[2]);
    txdata[i++] = (crc >> 8) & 0xFF;
    txdata[i++] = crc & 0xFF;
	rt_device_write(fc_serial, 0, txdata, txdata[2]+5);
	if (uart_send_task)
	{
		lv_timer_del(uart_send_task);
		uart_send_task = NULL;
	}
	if (uart_send_task == NULL)
	{
		uart_send_task_i = 0;
		uart_send_task = lv_timer_create(uart_send_task_cb,200,&txdata);
		//lv_timer_ready(uart_send_task);
	}
	
	

    // for (int var = 0; var < 3; ++var) {
	// 	rt_device_write(fc_serial, 0, txdata, txdata[2]+5);
	// }
}
//自定义串口发送查询
void yk_f_uart_send_query(u8 cmd,u8 data)
{
    uint8_t txdata[257] = {0};
    uint8_t i=0;
    txdata[i++] = HEAD_FH;
    txdata[i++] = HEAD_FL;
    txdata[i++] = DATA_LEN;
    txdata[i++] = DATA_SEND_QUERY;
    txdata[i++] = cmd;
    txdata[i++] = (data >> 8) & 0xFF;
    txdata[i++] = data & 0xFF;
    uint16_t crc =crc_cal_value(&txdata[3],txdata[2]);
    txdata[i++] = (crc >> 8) & 0xFF;
    txdata[i++] = crc & 0xFF;
    //for (int var = 0; var < 3; ++var) {
		rt_device_write(fc_serial, 0, txdata, txdata[2]+5);
	//}
}

//串口接收
static void USARTy_IRQHandler(uint8_t* databuf)
{
	printf("USARTy_IRQHandler(uint8_t* databuf) \n");

	cmdBuf[0] = databuf[3];
	cmdBuf[1] = databuf[4];
	cmdBuf[2] = databuf[6];

	if (cmdBuf[0] == 0x03)
	{
		switch(cmdBuf[1])
		{
			case 0x01:
				//主->屏 档位1 2 3,只有在手动模式才接收档位
				//printf("sizeof(a_array01) == %d \n",sizeof(a_array01)/sizeof(uint8_t*));
				if (cmdBuf[2] < sizeof(a_array01)/sizeof(uint8_t*))
				{
					yk_v_tap_position = cmdBuf[2];
					if (yk_v_operating_mode != 3)  //不是禁用状态就可以启动风机
					{
						if (yk_v_operating_mode == 0) {
							printf("yk_v_tap_position == %d \n",yk_v_tap_position);
						}else if (yk_v_operating_mode == 1) {
							if (yk_v_tap_position > 1) {
								a_mode_img_switchover_display[1] = yk_v_tap_position;
								a_mode_img_switchover_display[0] = 1;
								yk_view_set_dis_view(SCREEN_1_A_1_ID);
								printf("yk_v_tap_position == %d \n",yk_v_tap_position);
							}
						}
					}
					screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
				}
				break;
			case 0x02:
				//待机状态
				if (cmdBuf[2] == 0x00) {
					yk_view_set_dis_view(SCREEN_3_SCREENSAVER_ID);
				}else if (cmdBuf[2] == 0x02) {
					printf("主->屏 自动模式 \n");
					yk_v_operating_mode = 1;
					yk_view_set_dis_view(SCREEN_1_A_ID);
					screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
					//删除三角形动图任务
					yk_func_screen_2_M_1_task_del();
				}else if (cmdBuf[2] == 0x03) {
					printf("主->屏 手动模式 \n");
					yk_v_operating_mode = 0;
					yk_view_set_dis_view(SCREEN_2_M_1_ID);
					screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
				}
				else if (cmdBuf[2] == 0x04) {
					printf("主->屏 空气净化模式 \n");
					//自动检测指令
					if (yk_v_operating_mode != 3)
					{
						yk_v_operating_mode = 2;
						if (cmdBuf[2] == 0x04)
						{
							printf("cmdBuf[2] == 0x04 \n");
							a_mode_img_switchover_display[0] = 3;
							yk_view_set_dis_view(SCREEN_1_A_1_ID);
							screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
						}
					}
				}
				break;
			case 0x03:
				//滤芯
				if (cmdBuf[2] < sizeof(a_array03)/sizeof(uint8_t*))
				{
					a_mode_img_switchover_display[2] = cmdBuf[2];
					//强制提醒需要换滤芯红色图显示
					if (yk_v_operating_mode)
					{
						if (a_mode_img_switchover_display[2] == 0x03) {
							a_mode_img_switchover_display[0] = 2;
							yk_view_set_dis_view(SCREEN_1_A_1_ID);
							screensaver_time = SCREENSAVER_COUNTDOWN_TIME;
						}
					}
				}
				break;
			case 0x04:
				//PM2.5
				if (cmdBuf[2] < sizeof(a_array04)/sizeof(uint8_t*))
				{
					a_mode_img_switchover_display[4] = cmdBuf[2];
				}
				break;
			case 0x05:
				//PM10
				if (cmdBuf[2] < sizeof(a_array05)/sizeof(uint8_t*))
				{
					a_mode_img_switchover_display[5] = cmdBuf[2];
				}
				break;
			case 0x06:
				//NO2
				if (cmdBuf[2] < sizeof(a_array06)/sizeof(uint8_t*))
				{
					a_mode_img_switchover_display[6] = cmdBuf[2];
				}
				break;
			case 0x07:
				//空气指标 AIR QUALITY
				if (cmdBuf[2] < sizeof(a_array07)/sizeof(uint8_t*))
				{
					a_mode_img_switchover_display[7] = cmdBuf[2];
				}
				break;
			case 0x08:
				//PETO AIR数组
				if (cmdBuf[2] < sizeof(a_array08)/sizeof(uint8_t*))
				{
					a_mode_img_switchover_display[8] = cmdBuf[2];
				}
				break;
			}
			//串口发送
    		rt_device_write(fc_serial, 0, databuf, databuf[2]+5);
		}
}


static s8 yk_comm_handle(uint8_t* data)
{
	//求校验
	uint16_t crc = 0xffff;
	crc = (data[data[2]+3]<<8) | data[data[2]+4];

//	for(uint8_t i=0;i<data[2]+5;i++)
//	{
//		printf(" %02X",data[i]);
//	}
//
//	printf("crc == 0x%04X \n",crc);
//	printf("crc_cal_value(data[3],4) == 0x%04X \n",crc_cal_value(&data[3],4));

	if (crc_cal_value(&data[3],data[2]) == crc)
	{
		USARTy_IRQHandler(data);
		return GRF_OK;
	}
	else {
		return GRF_FAIL;
	}
}
#endif



//自定义串口发送函数
void yk_f_uart_send(unsigned char value)
{
	#ifdef  YK_USE_UI_BUILDER
	rt_device_write(fc_serial, 0, &value, 1);
	#else
	//在PC端测试时使用
	send_data(&value,1);
	#endif
}


//串口接收处理
void receive_data_handle(uint8_t* databuf,uint32_t datalen)
{
	// 打印databuf的十六进制内容和datalen的值
    printf("databuf (Hex): %02X datalen: %u\n", databuf[0],datalen);

	for(uint16_t i=0;i<datalen;i++)
	{
		printf("databuf[%d]=0x%02x \n",i,databuf[i]);
	}
	
	if (databuf[0] == HEAD_FH && databuf[1] == HEAD_FL && databuf[2] == 0x00 && databuf[3] == 0x06 && 
		databuf[4] == 0x00 && databuf[5] == 0x05 && 
		databuf[6] == 0x01 && databuf[7] == 0x01 && databuf[8] == 0x00 && databuf[9] == 0x01 && databuf[10] == 0x00 && 
		databuf[11] == 0x0d)
	{
		yk_printf("send shutdown button click event1\r\n");
		screen_2_main_t *scr = screen_2_main_get(&ui_manager);
		yk_printf("send shutdown button click event2\r\n");
		if (scr != NULL && scr->button_3_shutdown != NULL) {
			yk_printf("send shutdown button click event3\r\n");
			// lv_obj_event_send(scr->button_3_shutdown, LV_EVENT_CLICKED, NULL);
			// screen_2_main_button_3_shutdown_custom_clicked();
			// screen_2_main_label_6_air_fry_custom_clicked();
		}
		yk_printf("send shutdown button click event2222\r\n");
	}
	
	
	
	#if 0
	uint16_t i=0;
	static uint16_t last_data_num=0;

	if(last_data_num + datalen > RX_BUF_LEN){
		datalen = datalen-last_data_num;
	}
	if(last_data_num){
		memcpy(RX_HAND_BUF+last_data_num,databuf,datalen);  //把接收到的指令添加到RX_HAND_BUF后面
		databuf=RX_HAND_BUF;  //再把添加完的全部指令赋值给databuf去执行下面的通讯协议
		datalen+=last_data_num;
		last_data_num = 0;
	}
	else {
		memcpy(RX_HAND_BUF,databuf,datalen);  //拷贝接收到的指令给RX_HAND_BUF，这样就不会把多余的错误的指令去执行
		databuf=RX_HAND_BUF;  //再把需要执行的数组赋值给databuf去执行下面的通讯协议
	}




//  打印接收串口信息
	//printf("datalen =dddddddddddddddddddddddddddddd= %d\n",datalen);
	// for(uint16_t i=0;i<15;i++)
	// {
	// 	printf("databuf[%d]=0x%02x RX_HAND_BUF[%d]=0x%02x\n\n",i,databuf[i],i,RX_HAND_BUF[i]);
	// }

	for(i=0;datalen-i>=9;i++)  //最短的指令为7个
	{
		if(databuf[i]==HEAD_FH && databuf[i+1]==HEAD_FL && yk_comm_handle(databuf+i)==GRF_OK)
		{
			i += databuf[i+2]+4;
		}
		else {
			break;
		}
	}
	last_data_num = datalen-i;
    //printf("last_data_num =dddddddddddddddddddddddddddddd= %d\n",last_data_num);



	if(last_data_num<=100 && last_data_num!=0)
	{
		uint8_t  last_data_buf[256] = {0};
		memcpy(last_data_buf,databuf+i,last_data_num);
		memcpy(RX_HAND_BUF,last_data_buf,last_data_num);
	}else{
		last_data_num=0;
		memset(RX_HAND_BUF, 0, sizeof(RX_HAND_BUF)/sizeof(uint8_t));  //利用memset函数清空数组里面的所有元素把值都设为0
	}
	#endif
}

#endif
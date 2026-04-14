#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_stkdriver_h


/*
 * Copyright (c) 2023, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: xuan.wen <xuan.wen@artinchip.com>
 */

#ifndef __STKDRIVER_H__
#define __STKDRIVER_H__

#define STK3311_I2C_NAME "i2c0"

#define STK3311_DEFAULT_SLAVE_ID 0x48

#define STK3311_PRODUCT_ID       0x3E
#define STK3311_STATE_REG        0x00
#define STK3311_FLAG_REG         0x10
#define STK3311_ASL_DATA1_REG    0x13
#define STK3311_ASL_DATA2_REG    0x14

void yk_f_set_brightness_create(uint8_t set_brightness_value_01);
extern void STK_init_ALS(void);

extern uint8_t set_brightness_value;

#endif


#endif


#endif  //YK_USE_UI_BUILDER
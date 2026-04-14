#include "../apps.h"


#if yk_file_rm_h


#ifndef __YK_FILE_RM_H__
#define __YK_FILE_RM_H__

// 原始接口保持不变（确保兼容性）
uint32_t yk_f_file_r(uint8_t* databuf,uint32_t len);
uint32_t yk_f_file_w(uint8_t* databuf,uint32_t len);
void yk_f_file_rw_init(uint8_t* databuf,uint32_t len);

// 新增宏定义（提高安全性 & 可读性）
#define yk_file_write(obj)   yk_f_file_w((uint8_t *)&(obj), sizeof(obj))
#define yk_file_read(obj)    yk_f_file_r((uint8_t *)&(obj), sizeof(obj))
#define yk_file_init(obj)    yk_f_file_rw_init((uint8_t *)&(obj), sizeof(obj))

#endif


#endif
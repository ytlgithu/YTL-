#ifndef YK_UI_H
#define YK_UI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ui_objects.h"
#include "aic_ui.h"
#include "ui_util.h"

//自定义视图ID
typedef enum {
	SCREEN_NULL ,
	SCREEN_01_LOGO_ID,
	SCREEN_02_MAIN_ID,
	SCREEN_03_ID,
	SCREEN_04_ID,
	SCREEN_05_ID,
	SCREEN_06_ID,
	SCREEN_07_ID,
	SCREEN_08_ID,
	SCREEN_09_ID,
	SCREEN_10_ID,
	SCREEN_11_ID,
	SCREEN_12_ID,
	SCREEN_13_ID,
	SCREEN_14_ID,
	SCREEN_15_ID,
} yk_view_id_e;

extern yk_view_id_e yk_v_screen_id;

void yk_view_set_dis_view(yk_view_id_e v_id);


#ifdef __cplusplus
}
#endif
#endif
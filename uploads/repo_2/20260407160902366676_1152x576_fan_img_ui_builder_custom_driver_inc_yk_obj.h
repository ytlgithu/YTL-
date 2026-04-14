#include "../apps.h"


#ifndef __YK_OBJ_H__
#define __YK_OBJ_H__


typedef struct {
    lv_obj_t * obj;
    lv_event_code_t event_code;
} event_params_t;
void yklv_async_send_event(lv_obj_t * obj, lv_event_code_t event_code);

void yklv_delayed_execute(void (*func)(void), uint32_t delay_ms);


void food_tf_time_wrapper(void);
void yk_all_data_update(void);


#endif
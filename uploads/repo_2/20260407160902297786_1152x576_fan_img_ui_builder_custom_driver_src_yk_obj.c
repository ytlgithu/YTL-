#include "../apps.h"


#if 1  // 异步处理函数（发送事件函数）
// 添加异步执行的回调函数
static void async_event_handler(void * user_data)
{
    event_params_t * params = (event_params_t *)user_data;
    if (params->obj) {
        lv_obj_send_event(params->obj, params->event_code, NULL);
    }
    // 释放参数内存
    lv_free(params);
}
// 创建一个通用函数来异步发送事件
void yklv_async_send_event(lv_obj_t * obj, lv_event_code_t event_code)
{
    event_params_t * params = lv_malloc(sizeof(event_params_t));
    if (params) {
        params->obj = obj;
        params->event_code = event_code;
        lv_async_call(async_event_handler, params);
    }
}
#endif

#if 1  // 创建一个通用函数来异步执行其他线程的函数
// 创建一个通用的函数指针回调，可以从用户数据中获取要调用的函数
static void generic_func_cb(lv_timer_t *timer)
{
    // 从定时器的用户数据中获取函数指针
    void (*func)(void) = (void (*)(void))timer->user_data;
    if (func) {
        func();  // 调用传入的函数
    }
    lv_timer_del(timer); // 删除定时器
}

// 封装延迟执行任意函数的接口
void yklv_delayed_execute(void (*func)(void), uint32_t delay_ms)
{
    if (delay_ms == 0) {
        // 立即执行 - 使用lv_async_call确保在LVGL主线程中执行
        lv_async_call((void (*)(void *))func, NULL);
    } else {
        // 延迟执行
        lv_timer_create(generic_func_cb, delay_ms, (void *)func);
    }
}

#if 1  // 创建包装函数
void food_tf_time_wrapper(void) {
    yk_printf("food_tf_time_wrapper\n");
    food_tf_time(yk_v_food_t_f, yk_v_food_min);
}
#endif


#if 0  // 数据上报
void yk_all_data_update(void) {
    yk_printf("yk_all_data_update\n");
    mcu_dp_bool_update(DPID_SWITCH,1); //BOOL型数据上报;
}
#endif


#endif
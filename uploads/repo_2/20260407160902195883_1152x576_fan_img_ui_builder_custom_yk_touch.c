#include "custom.h"


static lv_timer_t *touch_sound_task = NULL;  //触摸声音任务


//触摸音回调：关闭蜂鸣器并释放定时器
static void touch_sound_cb()
{
	if (touch_sound_task){
		lv_timer_del(touch_sound_task);
		touch_sound_task = NULL;
		#ifdef YK_USE_UI_BUILDER
		yk_io_set_beep(0);
		#endif
	}
}

//触摸声音
void yk_f_touch_sound()
{
	//屏幕未就绪时不响，避免启动阶段误触发
	if (yk_v_screen_id)
	{
		if (yk_settings.sound)
		{
			yk_printf("yk_io_set_beep(1);=====================================================================\n");
			#ifdef YK_USE_UI_BUILDER
				yk_io_set_beep(1);
			#endif
			//rt_thread_mdelay(100);
			if (touch_sound_task == NULL)
			{
				touch_sound_task = lv_timer_create(touch_sound_cb,150,NULL);
			}else
			{
				lv_timer_del(touch_sound_task);
				touch_sound_task = NULL;
				if (touch_sound_task == NULL)
				{
					touch_sound_task = lv_timer_create(touch_sound_cb,150,NULL);
				}
			}
		}
	}
}

static void yk_touch_sound_indev_event_cb(lv_event_t * e) // 输入设备点击事件回调
{
	lv_indev_t * indev = lv_event_get_current_target(e); // 获取当前事件的输入设备
	if (!indev) indev = lv_event_get_indev(e); // 回退获取触发事件的输入设备
	if (indev && lv_indev_get_type(indev) != LV_INDEV_TYPE_POINTER) return; // 只处理触摸/指针设备
	lv_obj_t * target = (lv_obj_t *)lv_event_get_param(e); // 获取被点击的目标对象
	if (!target) return; // 没有目标对象就退出
	if (target == lv_scr_act()) return; // 点击屏幕空白不发声
	if (!lv_obj_has_flag(target, LV_OBJ_FLAG_CLICKABLE)) return; // 非可点击对象不发声
	yk_f_touch_sound(); // 触发点击声音
}

void yk_touch_sound_init()
{
	static bool inited = false;
	if (inited) return;
	inited = true;
	lv_indev_t * indev = NULL;
	while ((indev = lv_indev_get_next(indev)) != NULL) {
		lv_indev_add_event_cb(indev, yk_touch_sound_indev_event_cb, LV_EVENT_CLICKED, NULL);
	}
}
//INIT_APP_EXPORT(yk_f_touch_init);

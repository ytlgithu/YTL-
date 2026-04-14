#include "custom.h"


static screen_11_show_all_t *scr = NULL;


// 定义全局容器数组以便复用
static lv_obj_t* containers[YK_FOOD_SUM];


//点击的食物
void screen_11_show_all_container_3_food_clicked(lv_event_t *e)
{
    // 获取触发事件的对象
    lv_obj_t* clicked_obj = lv_event_get_target(e);
    
    // 遍历查找匹配的容器并输出索引
    for (int i = 0; i < YK_FOOD_SUM; i++) {
        if (clicked_obj == containers[i]) {
            if (yk_food_flag == i)return yk_view_set_dis_view(SCREEN_02_MAIN_ID);  //如果点击的容器和当前选中的容器相同则直接返回主界面

            if (!lv_obj_has_flag(screen_02_main_get(&ui_manager)->container_2_3_run, LV_OBJ_FLAG_HIDDEN))
            {
                yk_view_set_dis_view(SCREEN_02_MAIN_ID);
                // yk_show_toast(screen_02_main_get(&ui_manager)->container_7_toast_notification, yk_f_get_translation("busy_cannot_switch"), 3000);
                yk_ui_center_toast_show(yk_ui_tr("busy_cannot_switch", "运行中无法切换"), 3000);
                return;  //正在运行禁止切换图片
            }

            //设置四个自定义文本控件全部改为没选中状态
            lv_obj_remove_state(screen_02_main_get(&ui_manager)->label_1_airfry, LV_STATE_CHECKED);
            lv_obj_remove_state(screen_02_main_get(&ui_manager)->label_2_bake, LV_STATE_CHECKED);
            lv_obj_remove_state(screen_02_main_get(&ui_manager)->label_3_preheat, LV_STATE_CHECKED);
            lv_obj_remove_state(screen_02_main_get(&ui_manager)->label_4_dehydrate, LV_STATE_CHECKED);
            
            // 第一步：记录用户选择的食物索引（用于主界面大图与配方显示）
            yk_food_flag = i;
            // 第二步：同步 active flag，确保主界面按“当前食物/非收藏模式”刷新文本与爱心图标
            yk_active_favorite_flag = yk_food_flag;
            yk_active_from_favorites = false;
            //修改主界面右边容器边框选中透明度改为255，没选中改为0
            update_all_containers_border_opa(yk_food_flag);
            //修改温度和时间
            food_tf_time(0,0);
            yk_printf("yk_food_flag = %d \n", yk_food_flag);

            yk_main_update_ui_text();
            yk_view_set_dis_view(SCREEN_02_MAIN_ID);
            break;
        }
    }
}


//创建执行一次
void screen_11_show_all_custom_created(void)
{ 
    scr = screen_11_show_all_get(&ui_manager);

    // 初始化容器数组
    containers[0] = scr->image_0;
    containers[1] = scr->image_1;
    containers[2] = scr->image_2;
    containers[3] = scr->image_3;
    containers[4] = scr->image_4;
    containers[5] = scr->image_5;
    containers[6] = scr->image_6;
    containers[7] = scr->image_7;
    containers[8] = scr->image_8;
    containers[9] = scr->image_9;
    containers[10] = scr->image_10;
    containers[11] = scr->image_11;
    containers[12] = scr->image_12;
    containers[13] = scr->image_13;
    containers[14] = scr->image_14;
    containers[15] = scr->image_15;
    containers[16] = scr->image_16;
    
    // 为每个子容器添加独立的点击事件处理
    for (int i = 0; i < YK_FOOD_SUM; i++) {
        lv_obj_add_event_cb(containers[i], screen_11_show_all_container_3_food_clicked, LV_EVENT_CLICKED, NULL);
    }
}

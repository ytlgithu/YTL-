#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_psadc_h


#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include <rtthread.h>

#include "rtdevice.h"
#include "aic_core.h"
#include "aic_log.h"
#include "rtdevice.h"
#include "hal_psadc.h"
#ifdef AIC_SYSCFG_DRV
#include "hal_syscfg.h"
#endif

/* Global macro and variables */
#define AIC_PSADC_NAME               "psadc"
#define AIC_PSADC_ADC_MAX_VAL        0xFFF
#define AIC_PSADC_DEFAULT_VOLTAGE    3
#define AIC_PSADC_QC_MODE            0
#define AIC_PSADC_VOLTAGE_ACCURACY   10000

static rt_adc_device_t psadc_dev;

static int ret = 0;
static u32 adc_values[AIC_PSADC_CH_NUM];
static int cnt = 0;
static int chan_cnt = 0;
static u64 start_us, end_us;
static int ref_voltage = 0;
static rt_bool_t psadc_enabled = RT_FALSE;

lv_timer_t *yk_psadc_task = NULL;  //psadc任务名

int32_t ntc_voltage = 4000;//电压值{205,4029},{206,3961}


// 静态数组用于存储最近的10个ntc_voltage值
static int ntc_voltage_samples[10];
// 静态索引用于跟踪数组中的当前位置
static int sample_index = 0;
static void test_psadc_adc2voltage(float ref_voltage, int adc_value)
{
    ntc_voltage = (adc_value * ref_voltage) / AIC_PSADC_ADC_MAX_VAL;

    yk_printf("ntc_voltage %d.%02d V", ntc_voltage / 100, ntc_voltage % 100);
    return;
}
static void test_psadc_adc2voltage02(float ref_voltage, int adc_value)
{
    // 计算当前ntc_voltage
    int32_t current_ntc_voltage = (adc_value * ref_voltage) / AIC_PSADC_ADC_MAX_VAL;
    // 将当前值存储在样本数组中
    ntc_voltage_samples[sample_index] = current_ntc_voltage;
    // 更新索引以供下次调用
    sample_index = (sample_index + 1) % 10;

    // 计算并更新ntc_voltage为10次采样的平均值
    if (sample_index == 0) { // 当数组满时，计算平均值
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += ntc_voltage_samples[i];
        }
        ntc_voltage = sum / 10;
    }

    yk_printf("ntc_voltage %d.%02d V", ntc_voltage / 100, ntc_voltage % 100);
    return;
}

static rt_err_t yk_psadc_setup(float def_voltage)
{
    if (!psadc_dev) {
        psadc_dev = (rt_adc_device_t)rt_device_find(AIC_PSADC_NAME);
        if (!psadc_dev) {
            yk_printf("Failed to open %s device\n", AIC_PSADC_NAME);
            return -RT_ERROR;
        }
    }

    if (!ref_voltage) {
#ifdef AIC_SYSCFG_DRV
        ref_voltage = hal_syscfg_read_ldo_cfg();
#endif
        if (!ref_voltage) {
            yk_printf("Failed to obtain reference voltage through eFuse\n");
            ref_voltage = (int)(def_voltage) * AIC_PSADC_VOLTAGE_ACCURACY;
        }
        yk_printf("The reference voltage is %d.%02d V\n",
                   ref_voltage / AIC_PSADC_VOLTAGE_ACCURACY,
                   ref_voltage % AIC_PSADC_VOLTAGE_ACCURACY);
    }

    if (!psadc_enabled) {
        rt_err_t err = rt_adc_enable(psadc_dev, AIC_PSADC_QC_MODE);
        if (err != RT_EOK) {
            yk_printf("Failed to enable %s, err=%d\n", AIC_PSADC_NAME, err);
            return err;
        }
        psadc_enabled = RT_TRUE;
    }

    chan_cnt = rt_adc_control(psadc_dev, RT_ADC_CMD_GET_CHAN_COUNT, NULL);
    if (chan_cnt <= 0) {
        yk_printf("Invalid channel count %d\n", chan_cnt);
        return -RT_ERROR;
    }

    return RT_EOK;
}

static void yk_psadc_shutdown(void)
{
    if (psadc_dev && psadc_enabled) {
        rt_adc_disable(psadc_dev, AIC_PSADC_QC_MODE);
        psadc_enabled = RT_FALSE;
    }
}

int yk_psadc_get_adc(float def_voltage, int sample_num)
{
    yk_printf("yk_psadc_get_adc!!!!:\n");
    rt_err_t err = yk_psadc_setup(def_voltage);
    if (err != RT_EOK)
        return err;

    yk_printf("Start samplng %d samples for %d channels\n", sample_num, chan_cnt);

    for (int sample_cnt = 1; sample_cnt <= sample_num; sample_cnt++) {

        start_us = aic_get_time_us();
        ret = rt_adc_control(psadc_dev, RT_ADC_CMD_GET_VALUES, (void *)adc_values);
        end_us = aic_get_time_us();
        yk_printf("Sample time: %d us\n", abs(end_us - start_us));
        if (ret < 0) {
            yk_printf("Read failed! ret=%d\n", ret);
            yk_psadc_shutdown();
            return -RT_ERROR;
        }
        //aic_udelay(10);
        //rt_thread_mdelay(1000);
        yk_printf("[%d] PSADC:", sample_cnt);
        for (int i = 0; i < chan_cnt; i++) {
            yk_printf(" %d", adc_values[i]);
        }
        yk_printf("\nvoltage:");
        for (int i = 0; i < chan_cnt; i++) {
            test_psadc_adc2voltage(ref_voltage, adc_values[i]);
        }
        yk_printf("\n");

    }
    yk_psadc_shutdown();
    return RT_EOK;
}

//打印adc值
static void psadc_task_cb(lv_timer_t *timer)
{
    if (yk_psadc_setup(AIC_PSADC_DEFAULT_VOLTAGE) != RT_EOK)
        return;

    start_us = aic_get_time_us();
    ret = rt_adc_control(psadc_dev, RT_ADC_CMD_GET_VALUES, (void *)adc_values);
    end_us = aic_get_time_us();
    yk_printf("Sample time: %d us\n", abs(end_us - start_us));
    if (ret < 0) {
        yk_printf("Read failed! ret=%d\n", ret);
        return;
    }
    //aic_udelay(10);
    //rt_thread_mdelay(1000);
    cnt++;
    yk_printf("[%d] PSADC:", cnt);
    for (int i = 0; i < chan_cnt; i++) {
        yk_printf(" %d", adc_values[i]);
    }
    yk_printf("\nvoltage:");
    for (int i = 0; i < chan_cnt; i++) {
        test_psadc_adc2voltage02(ref_voltage, adc_values[i]);
    }
    yk_printf("\n");
}

void yk_f_psadc_create()
{
    yk_printf("yk_f_psadc_create20250224\n");
    if (yk_psadc_setup(AIC_PSADC_DEFAULT_VOLTAGE) != RT_EOK)
        return;

    start_us = aic_get_time_us();
    ret = rt_adc_control(psadc_dev, RT_ADC_CMD_GET_VALUES, (void *)adc_values);
    end_us = aic_get_time_us();
    yk_printf("Sample time: %d us\n", abs(end_us - start_us));
    if (ret < 0) {
        yk_printf("Read failed! ret=%d\n", ret);
        return;
    }
    //aic_udelay(10);
    //rt_thread_mdelay(1000);
    yk_printf("[%d] PSADC:", cnt);
    for (int i = 0; i < chan_cnt; i++) {
        yk_printf(" %d", adc_values[i]);
    }
    yk_printf("\nvoltage:");
    for (int i = 0; i < chan_cnt; i++) {
        test_psadc_adc2voltage(ref_voltage, adc_values[i]);
    }
    if(yk_psadc_task == NULL) yk_psadc_task = lv_timer_create(psadc_task_cb,100,NULL);
    lv_timer_reset(yk_psadc_task);
}

void yk_f_psadc_del()
{
    yk_printf("yk_f_psadc_del20250227\n");
    if (yk_psadc_task != NULL) {
    lv_timer_del(yk_psadc_task);
    yk_psadc_task = NULL;
    }
    yk_psadc_shutdown();
    cnt = 0;
}

//自定义psadc初始化
void yk_f_psadc_init()
{
    yk_printf("yk_f_psadc_init1!!!!\n");
    float def_voltage = AIC_PSADC_DEFAULT_VOLTAGE;
    if (yk_psadc_setup(def_voltage) != RT_EOK)
        return;

    ret = rt_adc_control(psadc_dev, RT_ADC_CMD_GET_VALUES, (void *)adc_values);
    if (ret < 0) {
        yk_printf("Init read failed! ret=%d\n", ret);
        yk_psadc_shutdown();
        return;
    }

    test_psadc_adc2voltage(ref_voltage, adc_values[0]);
    yk_printf("\n");
    yk_psadc_shutdown();
}

#endif


#endif  //YK_USE_UI_BUILDER

#ifndef YK_RTC_H
#define YK_RTC_H
#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t yk_v_hor_value;
extern uint8_t yk_v_min_value;
extern uint8_t yk_v_sec_value;


void yk_f_set_rtc_date(uint16_t year, uint8_t month, uint8_t day);
void yk_f_set_rtc_time(uint8_t hour, uint8_t minute, uint8_t second);
void yk_f_get_rtc_time(void);

#ifdef __cplusplus
}
#endif
#endif /* YK_RTC_H */
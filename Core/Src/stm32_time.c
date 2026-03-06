#include <lely/libc/time.h>
#include <stm32g4xx_hal.h>
#include <stdint.h>
#include "rtc.h"
RTC_HandleTypeDef sys_rtc;
uint32_t rtc_format;
/* @param  Format Specifies the format of the entered parameters.
*          This parameter can be one of the following values:
*            @arg RTC_FORMAT_BIN: Binary data format
*            @arg RTC_FORMAT_BCD: BCD data format
*/
void sys_set_clock_source(RTC_HandleTypeDef p_rtc,uint32_t p_format)
{
sys_rtc = p_rtc;
rtc_format = p_format;
}
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{

    RTC_TimeTypeDef gTime;
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	tp->tv_sec = gTime.Seconds + gTime.Minutes*60 + gTime.Hours*24*60*60;
	tp->tv_nsec = (gTime.SubSeconds * 1000000000UL) / (gTime.SecondFraction + 1);
return 0;
}

int clock_settime(clockid_t clock_id, const struct timespec *tp)
{

	RTC_TimeTypeDef sTime;
	sTime.Hours = tp->tv_sec/(60*60*24);
	sTime.Minutes = tp->tv_sec/60;
	sTime.Seconds = tp->tv_sec;
	sTime.SubSeconds = tp->tv_nsec;
	sTime.SecondFraction = 1000 * 1000 * 1000;
return HAL_RTC_SetTime(&sys_rtc, &sTime, rtc_format);
}

int _gettimeofday(struct timeval *tv, void *tz)
{
    return 0;
}
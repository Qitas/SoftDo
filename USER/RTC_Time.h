
#ifndef _RTC_TIME_H_
#define _RTC_TIME_H_


#include "stm32f10x.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

void RTC_Init(void);
void Time_Display(void);
void Time_Regulate(u8 step);


#endif
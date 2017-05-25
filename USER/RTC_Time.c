
/* Includes ------------------------------------------------------------------*/
#include "RTC_Time.h" 

void Time_Set(u32 t);

/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar
* Description    : 转换UNIX时间戳为日历时间
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	/* localtime转换结果的tm_year是相对值，需要转成绝对值 */
	return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix
* Description    : 写入RTC时钟当前时间
* Input          : - t: struct tm
* Output         : None
* Return         : time_t
* Attention		 : None
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900; 
	return mktime(&t);
}


/*******************************************************************************
* Function Name  : Time_GetUnixTime
* Description    : 从RTC取当前时间的Unix时间戳值
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime
* Description    : 从RTC取当前时间的日历时间（struct tm）
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;
	struct tm t_tm;

	t_t = (time_t)RTC_GetCounter();
	t_tm = Time_ConvUnixToCalendar(t_t);
	return t_tm;
}

/*******************************************************************************
* Function Name  : Time_SetUnixTime
* Description    : 将给定的Unix时间戳写入RTC
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
*******************************************************************************/
static void NVIC_Configuration(void)
{
			NVIC_InitTypeDef NVIC_InitStructure;
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
			NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the RTC.
*******************************************************************************/
static void RTC_ReConfig(void)
{
  BKP_DeInit();
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  RCC_RTCCLKCmd(ENABLE);
  RTC_WaitForSynchro();
  RTC_WaitForLastTask();
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  RTC_WaitForLastTask();
  RTC_SetPrescaler(40000); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
  RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : Time_Regulate
* Description    : None
*******************************************************************************/
void Time_Regulate(u8 step)
{
  struct tm time;
	u8 flag,buff[14] = {0};
	uint16_t index = 0;
  uint32_t cnt,tmp[7] = {0};
	cnt=0xffff;
	flag=step;
	if(flag>1)index=flag*2;
	else  printf("Please Set Years between 2000 to 2037\r\n");	
  while(flag<7 && cnt) 
  { 	
			while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
			buff[index++] = (USART_ReceiveData(USART1));	
			if ((buff[index - 1] < 0x30) || (buff[index - 1] > 0x39 || buff[index - 1] == 0x0D ))index=flag*2;	
			if(index>1)  tmp[((index-1)>>1)] = (buff[index-1] - 0x30) + ((buff[index-2] - 0x30) * 10 );	
			if(index==4 && flag==1) 
			{
					if(tmp[1] <37 && tmp[0] >18 && tmp[0] <21)	
					{			
								printf("Set Years:  %d\r\n", tmp[1]+100*tmp[0]);
								flag++;			    
						    printf("Please Set Months between  01 to 12 \r\n");
					}
					else
					{
						  index=0;
							printf("Please Set Years again!\r\n");
					}
			}
			else if(tmp[2] < 13 && index==6 && flag==2)
			{					
					printf("Set Months:  %d\r\n", tmp[2]);
					flag++;
				  printf("Please Set Days between  01 to 31 \r\n");
			}
			else if(index==6 && flag==2)
			{			
          index=4;				
					printf("Please Set Months again \r\n");
			}
			else if(tmp[3] < 32 && index==8 && flag==3) 
			{						
					printf("Set Days:  %d\r\n", tmp[3]);
					flag++;
				  printf("Please Set Hours between 00 to 23 \r\n");
			}
			else if(index==8 && flag==3)
			{			
          index=6;				
					printf("Please Set Days again \r\n");
			}
			else if(tmp[4] < 25 && index==10 && flag==4) 
			{
				 printf("Set Hours:  %d\r\n", tmp[4]);
				 flag++;
				  printf("Please Set Minutes between 00 to 59 \r\n");
			}
			else if(index==10 && flag==4) 
			{
          index=8;				
					printf("Please Set Hours again \r\n");
			}
			else if(tmp[5] < 61 && index==12 && flag==5) 
			{
					printf("Set Minutes:  %d\r\n", tmp[5]);
					flag++;
				  printf("Please Set Seconds between 00 to 59 \r\n");
			}
			else if(tmp[5] < 61 && index==12 && flag==5) 
			{
          index=10;				
					printf("Please Set Minutes again \r\n");
			}
			else if(tmp[6] < 61 && index==14 && flag==6)					
			{
					printf("Set Seconds:  %d\r\n", tmp[6]);	
					flag++;								
			}				
		}
		memset(&time, 0 , sizeof(time));	
		time.tm_year =tmp[1]+100*tmp[0];
		time.tm_mon = tmp[2]-1;
		time.tm_mday = tmp[3];
		time.tm_hour = tmp[4];
		time.tm_min = tmp[5];
		time.tm_sec = tmp[6];
		Time_SetCalendarTime(time);  
}

/*******************************************************************************
* Function Name  : RTC_Init
* Description    : RTC Initialization
*******************************************************************************/
void RTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  PWR_BackupAccessCmd(ENABLE);
  RCC_LSICmd(ENABLE);
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
				printf("RTC not yet configured....\r\n");
				RTC_ReConfig();
				Time_Regulate(1);
				printf("RTC configured....\r\n");
				BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf(" Power On Reset occurred....\r\n");
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf(" External Reset occurred....\r\n");
    }
    printf("No need to configure RTC....\r\n");
    RTC_WaitForSynchro();
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    RTC_WaitForLastTask();
  }
   NVIC_Configuration();
  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
  RCC_ClearFlag();
  return;
}

/*******************************************************************************
* Function Name  : Time_Display
* Description    : Printf Time
*******************************************************************************/
void Time_Display(void)
{
   struct tm time;
   time = Time_GetCalendarTime();
   printf("RTC:  %d-%d-%d   %02d:%02d:%02d \r\n", time.tm_year, \
                   time.tm_mon+1, time.tm_mday,\
                   time.tm_hour, time.tm_min, time.tm_sec);
}
void RTC_IRQHandler(void)
{
   if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
		{
			RTC_ClearITPendingBit(RTC_IT_SEC);
			//GPIOC->ODR^=0X4000;		
			Time_Display();	
		}
}

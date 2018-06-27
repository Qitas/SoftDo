
void RTC_set(uint8_t *p)
{
	for(int i=0;i<40;i++)
	{
		if(RxBuffer[i]=='R' && RxBuffer[i+1]=='T' && RxBuffer[i+2]=='C')
		{
				 
				  time_flag=i+3;				    
				  if(RxBuffer[time_flag]-0x30>=0 && RxBuffer[time_flag+1]-0x30>=0)
					{
						sdatestructure.Year=(RxBuffer[time_flag]-0x30)*0x10+RxBuffer[time_flag+1]-0x30; 
						printf("SET RTC year:%x \n",sdatestructure.Year);	
						time_flag+=2;
					}	
				  if(RxBuffer[time_flag]-0x30>=0 && RxBuffer[time_flag+1]-0x30>=0)
					{
							sdatestructure.Month=(RxBuffer[time_flag]-0x30)*0x10+RxBuffer[time_flag+1]-0x30; 
							printf("SET RTC mon:%x \n",sdatestructure.Month);	
						  time_flag+=2;
					}	
				  if(RxBuffer[time_flag]-0x30>=0 && RxBuffer[time_flag+1]-0x30>=0)
					{
							sdatestructure.Date=(RxBuffer[time_flag]-0x30)*0x10+RxBuffer[time_flag+1]-0x30; 
							printf("SET RTC date:%x \n",sdatestructure.Date);	
						  time_flag+=2;
					}	
				  if(RxBuffer[time_flag]-0x30>=0 && RxBuffer[time_flag]-0x30<=7)
					{
							sdatestructure.WeekDay=RxBuffer[time_flag]-0x30; 
							printf("SET RTC week:%x \n",sdatestructure.WeekDay);
							time_flag++;								
					}								
				  if(RxBuffer[time_flag]-0x30>=0 && RxBuffer[time_flag+1]-0x30>=0)
					{
							stimestructure.Hours=(RxBuffer[time_flag]-0x30)*0x10+RxBuffer[time_flag+1]-0x30; 
							printf("SET RTC hour:%x \n",stimestructure.Hours);	
						  time_flag+=2;
					}	
				  if(RxBuffer[time_flag]-0x30>=0 && RxBuffer[time_flag+1]-0x30>=0)
					{
							stimestructure.Minutes=(RxBuffer[time_flag]-0x30)*0x10+RxBuffer[time_flag+1]-0x30; 
							printf("SET RTC min:%x \n",stimestructure.Minutes);	
						  time_flag+=2;
					}	
					if(RxBuffer[time_flag]-0x30>=0 && RxBuffer[time_flag+1]-0x30>=0)
					{
							stimestructure.Seconds=(RxBuffer[time_flag]-0x30)*0x10+RxBuffer[time_flag+1]-0x30; 
							printf("SET RTC sec:%d \n",stimestructure.Seconds);	
						   time_flag+=2;
					}	
				 if(HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)  Error_Handler(); 
				 if (HAL_RTC_SetDate(&hrtc, &sdatestructure, RTC_FORMAT_BCD) != HAL_OK)  	Error_Handler();   	
				 break;							
		}
	}
}
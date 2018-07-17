#ifndef __SPI2_H
#define __SPI2_H
#include "sys.h"
				    
// SPI总线速度设置 
#define SPI2_SPEED_2   		0
#define SPI2_SPEED_4   		1
#define SPI2_SPEED_8   		2
#define SPI2_SPEED_16  		3
#define SPI2_SPEED_32 		4
#define SPI2_SPEED_64 		5
#define SPI2_SPEED_128 		6
#define SPI2_SPEED_256 		7
						  	    													  
void SPI2_Init(void);			 //初始化SPI2口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI2速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2总线读写一个字节
		 
#endif


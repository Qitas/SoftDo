#ifndef __I2C_H
#define __I2C_H

#include "Qitas.h"

void I2C_Port_Init(void);    //初始化IIC的IO口		
void I2C_SET(void);  		 
void I2C_Start(void);				//发送IIC开始信号 成功则返回1
void I2C_Stop(void);	  			//发送IIC停止信号
u8 I2C_Write(u8 txd);			  //IIC发送一个字节
u8 I2C_Read(unsigned char ack);//IIC读取一个字节
u8 I2C_Wait_Ack(void); 				//IIC等待ACK信号
void I2C_Ack(void);					//IIC发送ACK信号
void I2C_NAck(void);				//IIC不发送ACK信号
	  
#endif


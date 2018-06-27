#ifndef __I2C_H
#define __I2C_H

#include "Qitas.h"

void I2C_Port_Init(void);    //��ʼ��IIC��IO��		
void I2C_SET(void);  		 
void I2C_Start(void);				//����IIC��ʼ�ź� �ɹ��򷵻�1
void I2C_Stop(void);	  			//����IICֹͣ�ź�
u8 I2C_Write(u8 txd);			  //IIC����һ���ֽ�
u8 I2C_Read(unsigned char ack);//IIC��ȡһ���ֽ�
u8 I2C_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void I2C_Ack(void);					//IIC����ACK�ź�
void I2C_NAck(void);				//IIC������ACK�ź�
	  
#endif


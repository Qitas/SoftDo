#ifndef __IIC_H
#define __IIC_H

#include "Qitas.h"

void IIC_Port_Init(void);    //��ʼ��IIC��IO��	
void IIC_SET(void);    //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź� �ɹ��򷵻�1
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Write(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
#endif


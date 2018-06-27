
#include "Qitas.h" 
#include "IIC.h"
#include "delay.h"



#define SCL_H   GPIO_SetBits(IIC_Port, SCLK_Pin)
#define SCL_L   GPIO_ResetBits(IIC_Port, SCLK_Pin)
#define SDA_H   GPIO_SetBits(IIC_Port, SDA_Pin)
#define SDA_L   GPIO_ResetBits(IIC_Port, SDA_Pin)
#define SDA_I   GPIO_ReadInputDataBit(IIC_Port, SDA_Pin)
#define IIC_Delay   delay_us(2)


void IIC_Port_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = SCLK_Pin|SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_Port, &GPIO_InitStructure);
	SCL_H;
	SDA_H;
}
void IIC_SET(void)
{					     
	SCL_H;
	SDA_H;
}
void SDA_OUT(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = SDA_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
		GPIO_Init(IIC_Port,&GPIO_InitStructure);
}
void SDA_IN(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = SDA_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(IIC_Port,&GPIO_InitStructure);
}

void IIC_Start(void)
{
	SDA_OUT();  
	SDA_H;	  	  
	SCL_H;
	IIC_Delay;
 	SDA_L;
	IIC_Delay;
	SCL_L;
}	  

void IIC_Stop(void)
{
	SDA_OUT();
	SCL_L;
	SDA_L;
 	IIC_Delay;
	SCL_H; 
	SDA_H;
	IIC_Delay;							   	
}

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_H;IIC_Delay;	   
	SCL_H;IIC_Delay;
	SDA_IN();   
	while(SDA_I)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_L;	   
	return 0;  
} 

void IIC_Ack(void)
{
	SCL_L;
	SDA_OUT();
	SDA_L;
	IIC_Delay;
	SCL_H;
	IIC_Delay;
	SCL_L;
}
	    
void IIC_NAck(void)
{
	SCL_L;
	SDA_OUT();
	SDA_H;
	IIC_Delay;
	SCL_H;
	IIC_Delay;
	SCL_L;
}					 				     
	  
void IIC_Write(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    SCL_L;
    for(t=0;t<8;t++)
    {              
        if(txd&0x80) SDA_H;
			  else SDA_L;
        txd<<=1; 	  
		IIC_Delay;  
		SCL_H;
		IIC_Delay; 
		SCL_L;	
		IIC_Delay;
    }	 
} 	    

u8 IIC_Read(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        SCL_L; 
        IIC_Delay;
				SCL_H;
        receive<<=1;
        if(SDA_I)receive++;   
		    IIC_Delay; 
    }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();    
    return receive;
}

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;	
	IIC_Start();	
	IIC_Write(I2C_Addr);	 
	IIC_Write(addr); 
	IIC_Start();
	IIC_Write(I2C_Addr+1);  	   
	res=IIC_Read(0);	   
  IIC_Stop();
	return res;
}



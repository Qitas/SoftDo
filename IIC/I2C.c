
#include "Qitas.h" 
#include "I2C.h"
#include "delay.h"

#define SCL_SET   	GPIO_SetBits(I2C_Port, I2C_SCLK)
#define SCL_RESET   GPIO_ResetBits(I2C_Port, I2C_SCLK)
#define SDA_SET   	GPIO_SetBits(I2C_Port, I2C_SDA)
#define SDA_RESET   GPIO_ResetBits(I2C_Port, I2C_SDA)
#define SDA_GET     	GPIO_ReadInputDataBit(I2C_Port, I2C_SDA)
#define  I2C_Delay   	delay_us(2)


void I2C_Port_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = I2C_SCLK|I2C_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_Port, &GPIO_InitStructure);
	SCL_SET;
	SDA_SET;
}
void I2C_SET(void)
{					     
	SCL_SET;
	SDA_SET;
}

void SDA_O(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = I2C_SDA;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
		GPIO_Init(I2C_Port,&GPIO_InitStructure);
}
void SDA_I(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = I2C_SDA;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(I2C_Port,&GPIO_InitStructure);
}

void I2C_Start(void)
{
	SDA_O();  
	SDA_SET;	  	  
	SCL_SET;
	I2C_Delay;
 	SDA_RESET;
	I2C_Delay;
	SCL_RESET;
}	  

void I2C_Stop(void)
{
	SDA_O();
	SCL_RESET;
	SDA_RESET;
 	I2C_Delay;
	SCL_SET; 
	SDA_SET;
	I2C_Delay;							   	
}

u8 I2C_Wait_Ack(void)
{
	u8 ack=0;
	I2C_Delay;	
	SDA_SET;
	I2C_Delay;	   
	SCL_SET;
	SDA_I();   
	I2C_Delay;
	if(SDA_GET)ack=1;
  else ack=0;
	SCL_RESET;	
  I2C_Delay;	
	return ack;  
} 

void I2C_Ack(void)
{
	SCL_RESET;
	SDA_O();
	SDA_RESET;
	I2C_Delay;
	SCL_SET;
	I2C_Delay;
	SCL_RESET;
}
	    
void I2C_NAck(void)
{
	SCL_RESET;
	SDA_O();
	SDA_SET;
	I2C_Delay;
	SCL_SET;
	I2C_Delay;
	SCL_RESET;
}					 				     
	  
u8 I2C_Write(u8 txd)
{                        
    u8 t,status; 
	  SDA_O(); 	    
    SCL_RESET;
    for(t=0;t<8;t++)
    {              
        if(txd&0x80) SDA_SET;
			  else SDA_RESET;
        txd<<=1; 	  
		I2C_Delay;  
		SCL_SET;
		I2C_Delay; 
		SCL_RESET;	
		I2C_Delay;
    }
		status=I2C_Wait_Ack();
    return status;	
} 	    

u8 I2C_Read(u8 ack)
{
	u8 i,receive=0;
	SDA_I();
    for(i=0;i<8;i++ )
	{
        SCL_RESET; 
        I2C_Delay;
				SCL_SET;
        receive<<=1;
        if(SDA_GET)receive++;   
		    I2C_Delay; 
    }					 
    if (ack)I2C_Ack();         
    else I2C_NAck();       
    return receive;
}



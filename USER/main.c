/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "RTC_Time.h" 

void RCC_Configuration(void)
{
	   unsigned char temp=0;   	   
 					//RCC_DeInit(); 
         	RCC->CR = 0;
	        while(!(RCC->CR>>1));
//        	RCC->CFGR |= 0x00000400;  
	        RCC->CFGR=(0x11<<19);
//	        
//	        RCC->CFGR|=2;  
//	        while(RCC->CFGR<0x08); 	
					//RCC_HSICmd(ENABLE);   
					//while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); 
					RCC_HCLKConfig(RCC_SYSCLK_Div1);     
					RCC_PCLK1Config(RCC_HCLK_Div2);  
					RCC_PCLK2Config(RCC_HCLK_Div1);   
     			RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_14);  
					RCC_PLLCmd(ENABLE);
					while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  
					RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
					while(RCC_GetSYSCLKSource()!= 0x08); 			
}

void  Delay (uint32_t nCount)
{
   for(; nCount != 0; nCount--);
}

void GPIO_Configuration(void)
{
		RCC->APB2ENR |= 1<<4;
		GPIOC->CRH&=0XFF0FFFFF;
		GPIOC->CRH|=0X00300000;
}
void IDWG_Configuration(void)
{
		IWDG->KR=0X5555;
		IWDG->PR=6;
		IWDG->RLR=0Xfff;
		IWDG->KR=0XAAAA;
		IWDG->KR=0XCCCC;
}
//void USART_Configuration(void)
//{
//		RCC->APB2ENR |= 1<<14;
//	  RCC->APB2RSTR|=1<<14;  
//    RCC->APB2RSTR&=~(1<<14);
//	  RCC->APB2ENR|=1<<2;
//	  GPIOA->CRH&=0xFFFFF00F;
//	  GPIOA->CRH=0x000008b0;
//	  USART1->CR2=0;
//	  USART1->CR3=0;
//		USART1->BRR=0x0271;
//	  USART1->CR1 = 0x200c;
//	
//		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//		USART_Cmd(USART1, ENABLE);
////   	USART1->CR1|=1<<8; 
////    USART1->CR1|=1<<5;     
////		NVIC->ISER[1]|=1<<5;   
////		NVIC->IP[37]=0X30;
//	
////    MY_NVIC_Init(3,3,USART1_IRQn,2);
//}

void USART_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  USART_Cmd(USART1, ENABLE);
}

int main(void)
{
		//RCC_Configuration();
		GPIO_Configuration();
    USART_Configuration();
	  // RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR |RCC_APB1Periph_BKP, ENABLE); 
	   RTC_Init();
	  //PWR_BackupAccessCmd(ENABLE); 
	  //BKP_WriteBackupRegister(BKP_DR1, 0x4444);
//	IDWG_Configuration();
		//BKP->DR1=1;
    while (1)
		{
//			   GPIOC->ODR^=0X2000;
//			  __NOP();
//				__NOP();		
//				__NOP();
//				__NOP();
//			   Delay(0xfFFf);	Delay(0xfffff);
//			
////			if(BKP->DR1!=0xffff)		BKP->DR1+=1;
//			 GPIOC->ODR^=0X2000;
    }
}


//void USART1_IRQHandler(void)
//{
//    u8 res;    
//		if(USART1->SR&(1<<5))
//		{
//			
//		}
//}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

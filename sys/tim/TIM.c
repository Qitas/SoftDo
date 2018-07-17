/*-----------------------------------------------------------------*/
/*********************定时器初始化与中断函数************************/
/*-----------------------------------------------------------------*/  
#include "TIM.h" 
#include "stm32f10x.h"

extern uint8_t DATA,DATA1;  //DATA定时器暂时存储数据  DATA1主函数中用于输出的
extern __IO uint8_t receivedFlag; //接受完成标志位

/*******************************************************************************
* Function Name  : TASK
* Description    : 
*******************************************************************************/
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				//定时器配置结构体定义
	NVIC_InitTypeDef NVIC_InitStructure;						      //中断配置结构体定义

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //时钟使能
	
	//定时器TIM2初始化: 定时器频率((1+TIM_Prescaler )/72M)*(1+TIM_Period )
	TIM_TimeBaseStructure.TIM_Period = arr;    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;  //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断	TIM_IT_Update

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	//TIM_Cmd(TIM2, ENABLE);  //使能TIM2			 
}

/***********************************************************************************
*     注意：个人在调试期间发现发送时间要小于接受时间
* 9600波特率时    SendingDelay=104    TIME3_init(108,72c)
* 115200波特率时    SendingDelay=8    TIME3_init(10,72c)
************************************************************************************/
void TIM3_init(u16 arr,u16 psc)
{
 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr -1;			//从0~（arr-1）计数，定时周期为arr次
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;		//频率48mhz/psc,一般是1Mhz，1us吧
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);     

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;      //初始化了中断但没有使能
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
* Function Name  : TASK
* Description    : 
*******************************************************************************/
//定时器2中断服务程序
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );   //清除TIM2更新中断标志 
		GPIOB->ODR ^= GPIO_Pin_4;
	}
  
  if(beep_times-- <= 0)
  {
    TIM_Cmd(TIM2, DISABLE);
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);
  }  
}
void beep_start(uint16_t times)
{
	  beep_times = times;
	  TIM_Cmd(TIM2, ENABLE);
}

/*******************************************************************************
* Function Name  : TASK
* Description    : 
*******************************************************************************/
void TIM3_IRQHandler(void)
{  
	uint8_t tmp;
	static uint8_t i;
	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) != RESET)
	{
		tmp = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);
		if(tmp == 1)
		DATA |= (1 << i); 
		i++;
		if(i >= 8)
		{
			i = 0;
			DATA1=DATA; 
			DATA=0;
			receivedFlag = 1;
			EXTI->IMR |= 1<<10;   	//开启外部中断
			TIM_Cmd(TIM3,DISABLE); 	//关闭TIM3
		}
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
	}
}

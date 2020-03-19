#include "timer.h"

//定时器2 初始化
void Timer2_Init(uint16_t arr,uint16_t psc){
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义定时器结构体变量
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM4的时钟
	
	//72的分频，一个时钟周期是 1微秒 ，一次中断是 1ms ，向上计数
	TIM_TimeBaseStructure.TIM_Period = arr-1;		//设置自动重装载的周期值
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;	//时钟频率 预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//定时器初始化函数
	
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	//TIM2中断使能
	TIM_Cmd(TIM2, ENABLE);//TIM2使能
}


//void TIM2_IRQHandler(void){	
//	static int count1  , count2 , count3;
//	static _Bool flag1 = 0 ;
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){//判断是否进入TIM2更新中断
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除TIM2更新中断
//		
//	}
//}


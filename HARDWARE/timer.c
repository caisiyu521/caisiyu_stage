#include "timer.h"

//��ʱ��2 ��ʼ��
void Timer2_Init(uint16_t arr,uint16_t psc){
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//���嶨ʱ���ṹ�����
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ��TIM4��ʱ��
	
	//72�ķ�Ƶ��һ��ʱ�������� 1΢�� ��һ���ж��� 1ms �����ϼ���
	TIM_TimeBaseStructure.TIM_Period = arr-1;		//�����Զ���װ�ص�����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;	//ʱ��Ƶ�� Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//��ʱ����ʼ������
	
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	//TIM2�ж�ʹ��
	TIM_Cmd(TIM2, ENABLE);//TIM2ʹ��
}


//void TIM2_IRQHandler(void){	
//	static int count1  , count2 , count3;
//	static _Bool flag1 = 0 ;
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){//�ж��Ƿ����TIM2�����ж�
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//���TIM2�����ж�
//		
//	}
//}


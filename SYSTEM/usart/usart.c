#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE { 
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) { 
	x = x; 
} 

//USART1    �ض���fputc���� 
int fputc(int ch, FILE *f){      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[USART_REC_LEN];
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	  
u16 USART2_RX_STA=0;       //����״̬���	
u16 len ,t;
void uart1_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
	
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		if((USART1_RX_STA&0x8000)==0){//����δ���
			if(USART1_RX_STA&0x4000){//���յ���0x0d
				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
			}
			else {//��û�յ�0X0D	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}	
			}				
		}
	}   		 
}

void uart3_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  
	
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 
}

u8 temp;
extern u8 rxbuf[256];
#include "fy_mqtt.h"

void USART3_IRQHandler(void)
{
    static u8 rxlen = 0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�жϽ������ݼĴ����Ƿ�������
    {
		temp = USART3->DR;
		if(rxlen>=255) rxlen=0;
        rxbuf[rxlen++] = temp;
        rxlen%=sizeof(rxbuf);
    }
    if(USART_GetITStatus(USART3, USART_IT_IDLE)){
		temp = USART3->DR;
		temp = USART3->SR;
		_mqtt.rxlen = rxlen;
		rxlen=0;
    }
}

//����3����һ���ַ���
void Usart3_SendString(char *str)
{
    while(*str)	Usart3_SendByte(*str++);
}

//����3����һ���ֽ�
void Usart3_SendByte(u8 val)
{
    USART_SendData(USART3, val);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	//�ȴ��������
}

void Usart3_SendBuf(u8 *buf,u8 len)
{
    while(len--)	Usart3_SendByte(*buf++);
}


//����һ���ֽ�
void Usart_SendChar(USART_TypeDef * pUSARTx,  uint8_t ch){
	/* ����һ���ֽ����ݵ� USART */
	USART_SendData (pUSARTx ,ch);
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
//����һ���ַ���
void Usart_SendString(USART_TypeDef * pUSARTx, char *str){
	unsigned int k=0;
	do {
		Usart_SendChar( pUSARTx, *(str + k) );
		k++;
	} while (*(str + k)!='\0');
}

#endif

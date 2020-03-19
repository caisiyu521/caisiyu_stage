#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8  USART1_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	
extern u8  USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	
extern u16 len ,t;

// printf() ֻ�Ƕ� Usrat1 �� �� 
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void Usart_SendChar(USART_TypeDef * pUSARTx,  uint8_t ch);
void Usart_SendString(USART_TypeDef * pUSARTx, char *str);
#define USART2_TxPin GPIO_Pin_2
#define USART2_RxPin GPIO_Pin_3

void Usart3_SendString(char *str);

//����1����һ���ֽ�
void Usart3_SendByte(u8 val);
void Usart3_SendBuf(u8 *buf,u8 len);



#endif


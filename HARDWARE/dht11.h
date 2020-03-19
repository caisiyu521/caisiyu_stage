#ifndef __DHT11_H__
#define	__DHT11_H__

#include "sys.h"
#include "delay.h"

#define 	DHT_Direction_I  	{GPIOB->CRL &= 0XFFFFFFF0; GPIOB->CRL |= 8;}
#define 	DHT_Direction_O 	{GPIOB->CRL &= 0XFFFFFFF0; GPIOB->CRL |= 3;}

/*	IO Definitions	*/

#define		O_DHT_DAT		PBout(0)
#define		I_DHT_DAT		PBin(0)
#define		DHT_DAT_Pin		GPIO_Pin_0
#define    	DHTPORT         GPIOB
#define     DHTCLKLINE      RCC_APB2Periph_GPIOB

extern void DHT11Configuration(void);
extern uint8_t DHT11_IsOnline(void);
extern uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);

#endif


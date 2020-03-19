#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h" 
#include "nvic.h"
#include "dht11.h"
#include "fy_mqtt.h"
#include "fy_network.h"
 #include "string.h"
/************************************************
 ALIENTEKս��STM32������ʵ��4
 ����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

uint8_t wendu = 0,shidu = 0;
u8 txbuf[256];
u8 rxbuf[256];
uint8_t cnt_1_5_s = 0;
u8 sta;
char mqtt_message[200];

void ESP_INIT(void);

int main(void)
{		

    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart1_init(115200);	 //���ڳ�ʼ��Ϊ115200
    uart3_init(115200);	 //���ڳ�ʼ��Ϊ115200
    LED_Init();			     //LED�˿ڳ�ʼ��
    Timer2_Init(1000,72);
    DHT11Configuration();
    
    nvic_init();
    
    ESP_INIT();//ESP8266��ʼ��
    
    printf("CPU Start!!!\r\n");
    
    while(1){
        
//        printf("%d , %d",wendu,shidu);
//        LED0 = 1;
//        delay_ms(100);
//        LED0 = 0;
//        delay_ms(100);
        
        if(cnt_1_5_s == 1){
            cnt_1_5_s = 0;
            DHT11_Read_Data(&wendu,&shidu);
            sprintf(mqtt_message,"%d",wendu);
            _mqtt.PublishData("/cai/wendu",mqtt_message,0);
            sprintf(mqtt_message,"%d",shidu);
            _mqtt.PublishData("/cai/shidu",mqtt_message,0);
        }
        
        if(_mqtt.rxlen){
			Mqtt_Progress(_mqtt.rxbuf,_mqtt.rxlen);	
			memset(_mqtt.rxbuf,0,_mqtt.rxlen);
			_mqtt.rxlen = 0;
		}
    }	 
}

void ESP_INIT(void){
	if(_net.Init(rxbuf,sizeof(rxbuf),txbuf,sizeof(txbuf))!=0){
		printf("Net Init OK!\r\n");
		sta++;
	}
	else{
		printf("Net Init Error!\r\n");		sta=0;
	}
	if(sta==1){
		if(_net.ConnectAP("ipone","123456321")!=0){
//		if(_net.ConnectAP("iPhone","88888888")!=0){
			printf("Conncet AP OK!\r\n");
			sta++;
		}
		else {
			printf("Conncet AP Error!\r\n");
			sta=0;
		}
	}
	if(sta==2){	
		if(_net.ConnectServer("TCP","iizyqwf.mqtt.iot.gz.baidubce.com",1883)!=0) {//����TCP
//            if(_net.ConnectServer("TCP","118.178.59.37",1883)!=0) {//����TCP
			printf("Conncet Server OK!\r\n");
			sta++;
		}else{
			printf("Conncet Server Error!\r\n");	
			sta=0;
		}
	}
	if(sta==3){
		//��¼MQTT
		_mqtt.Init(rxbuf,sizeof(rxbuf),txbuf,sizeof(txbuf));		
		if(_mqtt.Connect(
			"danpianji",
			"iizyqwf/caisiyu",//Username
			"pAOMcfGxse9O9vZC"//Password
			) != 0){
			printf("Enter MQTT OK!\r\n");
			sta++;
		}else{		
			printf("Enter MQTT Error!\r\n");
			sta=0;
		}	
	}
	if(sta==4){
		if(_mqtt.SubscribeTopic("/cai/controlLED",0,1) != 0){
			printf("SubscribeTopic OK!\r\n");
		}else{
			printf("SubscribeTopic Error!\r\n");
		}
	}
}


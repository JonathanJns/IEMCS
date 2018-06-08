#ifndef _DHT12_H_
#define _DHT12_H_
#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"
void sensor_iic_init(void);//��ʼ��DHT12_IIC
u8 sensor_read(float* Temp,float* Humi);//��ȡ����
void Continuous_Collect(float* Temp,float* Humi,u8 times);//��βɼ���ʪ��ȡƽ��ֵ


//extern float Temprature,Humidity;
typedef struct
{

	float tempreture;
	float humidity;

} DHT12_INFO;

extern DHT12_INFO dht12Info;

#endif	//_DHT12_H_

#ifndef _PM2_5_H_
#define _PM2_5_H_
#include "stm32f10x.h"
void PM2_5_USART_init(u32 bound);


#define GP2Y1050_COLLECT_TIMES  30  //PM2.5ģ��ɼ�ƽ��������ÿ�ɼ��ô�����һ��ƽ��ֵ����ֵ���Ϊ64���������������ݲ�׼
extern u8 GP2Y1050_Times;
extern float GP2Y1050_Quality;
extern float PM2_5_Cncentration;

#endif

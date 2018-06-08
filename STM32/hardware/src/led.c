/**
	************************************************************
	*
	*	˵���� 		LED��ʼ��������LED
	*
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//LEDͷ�ļ�
#include "led.h"
#include "delay.h"



LED_STATUS ledStatus;


/*
************************************************************
*	�������ƣ�	Led_Init
*
*	�������ܣ�	LED��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		LED4-PB6	LED5-PB7	LED6-PB8	LED7-PB9
				�ߵ�ƽ�ص�		�͵�ƽ����
************************************************************
*/
void Led_Init(void)
{
	
	GPIO_InitTypeDef gpioInitStrcut;

	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	//IO����
	gpioInitStrcut.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9;
	gpioInitStrcut.GPIO_Speed = GPIO_Speed_50MHz;
	//IO��ʼ��
	GPIO_Init(GPIOB, &gpioInitStrcut);
	
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &gpioInitStrcut);
    
    Led2_Set(LED_OFF);
    Led3_Set(LED_OFF);
    Led4_Set(LED_OFF);
    Led5_Set(LED_OFF);
		Led11_Set(LED_ON);
    Led12_Set(LED_ON);
}

/*
************************************************************
*	�������ƣ�	Led2_Set
*
*	�������ܣ�	LED4����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led2_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOB, GPIO_Pin_6, status != LED_ON ? Bit_SET : Bit_RESET);
	ledStatus.Led2Sta = status;

}

/*
************************************************************
*	�������ƣ�	Led3_Set
*
*	�������ܣ�	LED5����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led3_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOB, GPIO_Pin_7, status != LED_ON ? Bit_SET : Bit_RESET);
	ledStatus.Led3Sta = status;

}

/*
************************************************************
*	�������ƣ�	Led4_Set
*
*	�������ܣ�	LED6����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led4_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOB, GPIO_Pin_8, status != LED_ON ? Bit_SET : Bit_RESET);
	ledStatus.Led4Sta = status;

}

/*
************************************************************
*	�������ƣ�	Led5_Set
*
*	�������ܣ�	LED7����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led5_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOB, GPIO_Pin_9, status != LED_ON ? Bit_SET : Bit_RESET);
	ledStatus.Led5Sta = status;

}


void Led11_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOA, GPIO_Pin_11, status != LED_ON ? Bit_SET : Bit_RESET);
	ledStatus.Led11Sta = status;

}

void Led12_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOA, GPIO_Pin_12, status != LED_ON ? Bit_SET : Bit_RESET);
	ledStatus.Led12Sta = status;

}


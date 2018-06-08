/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	adc_sensor.c
	*
	*	���ߣ� 		���
	*
	*	���ڣ� 		2018-03-20
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		��ȡ����
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "adc.h"
#include "adc_sensor.h"
#include "usart.h"
#include "led.h"
#include "delay.h"

/* ���ڱ���ת�������ĵ�ѹֵ */	 
__IO float ADC_ConvertedValueLocal[ADC_NUMOFCHANNEL];   

/* ��չ���� ------------------------------------------------------------------*/
extern __IO uint16_t ADC_ConvertedValue[ADC_NUMOFCHANNEL];
/* ˽�к���ԭ�� --------------------------------------------------------------*/
static void Delay(uint32_t time);

int Gas;
int Light;
//int Rain;


GAS_INFO gasInfo = {0};



ADCSENSOR_INFO adcsensorInfo;;

void sensor_adc_init(void)
{
	
}



void adcsensor_read()
{

	
		ADC_ConvertedValueLocal[0] =(float)ADC_ConvertedValue[0]*3.3/4096; 
		ADC_ConvertedValueLocal[1] =(float)ADC_ConvertedValue[1]*3.3/4096; 
		ADC_ConvertedValueLocal[2] =(float)ADC_ConvertedValue[2]*3.3/4096; 
		ADC_ConvertedValueLocal[3] =(float)ADC_ConvertedValue[3]*3.3/4096; 
	
		//UsartPrintf(USART_DEBUG,"CH1_PA4 value = %fV\n",ADC_ConvertedValueLocal[0]);
    //UsartPrintf(USART_DEBUG,"CH2_PA5 value = %fV\n",ADC_ConvertedValueLocal[1]);
    //UsartPrintf(USART_DEBUG,"CH3_PA6 value = %fV\n",ADC_ConvertedValueLocal[2]);
    //UsartPrintf(USART_DEBUG,"CH4_PA7 value = %fV\n",ADC_ConvertedValueLocal[3]);
    //UsartPrintf(USART_DEBUG,"\n");


/*---------------------------1.��ȡ�к���������------------------------------**/		
		Gas=(unsigned int)(ADC_ConvertedValueLocal[0]*ADC_ConvertedValueLocal[0]*100-35);		
		
		//UsartPrintf(USART_DEBUG,"CH1_PA4 value = %dV\n",Gas);
	
/*---------------------------2.��ȡ����ǿ������------------------------------**/		
		Light=10/ADC_ConvertedValueLocal[2]/ADC_ConvertedValueLocal[2];		////ADC_ConvertedValueLocal[1]/0.1
		
		//UsartPrintf(USART_DEBUG,"CH1_PA5 value = %dV\n",Light);
		
/*---------------------------3.��ȡ������------------------------------**/		
		//Rain=ADC_ConvertedValueLocal[1];	
		
		//UsartPrintf(USART_DEBUG,"CH1_PA6 value = %dV\n",Rain);

}


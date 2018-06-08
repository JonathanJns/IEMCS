/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	main.c
	*
	*	���ߣ� 		���
	*
	*	���ڣ� 		2018-03-27
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		��ɵ�Ƭ����ʼ�������IC��ʼ��������Ĵ���������
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//OS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "freertosAPI.h"

//Э��
#include "onenet.h"
#include "fault.h"

//�����豸
#include "net_device.h"

//Ӳ������
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "hwtimer.h"
#include "i2c.h"
//#include "sht20.h"
#include "iwdg.h"
#include "at24c02.h"
#include "selfcheck.h"
#include "info.h"
#include "dht12.h"
#include "pm2.5.h"
#include "HC-SR501.h"
#include "adc.h"
#include "adc_sensor.h"
#include "ctrl.h"
//C��
#include <string.h>






//���Ź�����
#define IWDG_TASK_PRIO		11
#define IWDG_STK_SIZE		64
TaskHandle_t IWDG_Task_Handler;
void IWDG_Task(void *pvParameters);

//��������
#define USART_TASK_PRIO		10
#define USART_STK_SIZE		256
TaskHandle_t USART_Task_Handler;
void USART_Task(void *pvParameters);

//��������
#define HEART_TASK_PRIO		9
#define HEART_STK_SIZE		256
TaskHandle_t HEART_Task_Handler;
void HEART_Task(void *pvParameters);

//���ϴ�������
#define FAULT_TASK_PRIO		8 //
#define FAULT_STK_SIZE		256
TaskHandle_t FAULT_Task_Handler;
void FAULT_Task(void *pvParameters);

//����������
#define SENSOR_TASK_PRIO	7
#define SENSOR_STK_SIZE		512
TaskHandle_t SENSOR_Task_Handler;
void SENSOR_Task(void *pvParameters);

//���ݷ�������
#define SEND_TASK_PRIO		6
#define SEND_STK_SIZE		512
TaskHandle_t SEND_Task_Handler;
void SEND_Task(void *pvParameters);

//�����ʼ������
#define NET_TASK_PRIO		4 //
#define NET_STK_SIZE		512
TaskHandle_t NET_Task_Handler;
void NET_Task(void *pvParameters);

//���ݷ�������
#define DATA_TASK_PRIO		3 //
#define DATA_STK_SIZE		512
TaskHandle_t DATA_Task_Handler;
void DATA_Task(void *pvParameters);

//��Ϣ��������
#define ALTER_TASK_PRIO		2 //
#define ALTER_STK_SIZE		256
TaskHandle_t ALTER_Task_Handler;
void ALTER_Task(void *pvParameters);



#define NET_TIME	60			//�趨ʱ��--��λ��
unsigned short timerCount = 0;	//ʱ�����--��λ��

TimerHandle_t t1_Thdl;






//������
DATA_STREAM dataStream[] = {
								{"Ctrl", &ctrlStatus.CtrlSta, TYPE_BOOL, 1},//����
								{"Alarm_Led", &ledStatus.Led5Sta, TYPE_BOOL, 1},//����
								{"Filter_Led", &ledStatus.Led4Sta, TYPE_BOOL, 1},//����������
								{"Humidifier_Led", &ledStatus.Led3Sta, TYPE_BOOL, 1},//��ʪ��
								{"Light_Led", &ledStatus.Led2Sta, TYPE_BOOL, 1},//��
								{"Forward", &ledStatus.Led11Sta, TYPE_BOOL, 1},//��
								{"Reverse", &ledStatus.Led12Sta, TYPE_BOOL, 1},//��
								{"temperature", &dht12Info.tempreture, TYPE_FLOAT, 1},
								{"humidity", &dht12Info.humidity, TYPE_FLOAT, 1},
								{"PM2.5", &PM2_5_Cncentration, TYPE_FLOAT, 1},
								{"gas", &Gas, TYPE_INT, 1},
								{"light", &Light, TYPE_INT, 1},
								{"errType", &faultTypeReport, TYPE_UCHAR, 1},
							};
unsigned char dataStreamLen = sizeof(dataStream) / sizeof(dataStream[0]);


/*
************************************************************
*	�������ƣ�	Hardware_Init
*
*	�������ܣ�	Ӳ����ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����Ƭ�������Լ�����豸
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);								//�жϿ�������������

	delay_init();
	
	Delay_Init();																//Timer3��ʼ����ʼ��
	
	Led_Init();																	//LED��ʼ��
	
	HC_SR501_GPIO_Init();												//HC_SR501��ʼ��
	
	ADCx_Init();                                //4ͨ��ADC��ʼ��
	
	IIC_Init();																	//���IIC���߳�ʼ��
	
	Usart1_Init(115200); 														//��ʼ������   115200bps
	
	PM2_5_USART_init(2400);										//��ʼ��PM2.5����
		
	Check_PowerOn(); 															//�ϵ��Լ�
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) 								//����ǿ��Ź���λ����ʾ
	{
		UsartPrintf(USART_DEBUG, "WARN:	IWDG Reboot\r\n");
		
		RCC_ClearFlag();														//������Ź���λ��־λ
		
		faultTypeReport = faultType = FAULT_REBOOT; 							//���Ϊ��������
		
		if(!Info_Check() && checkInfo.EEPROM_OK)								//���EEPROM������Ϣ
			Info_Read();
	}
	else
	{
		//�ȶ���ssid��pswd��devid��apikey
		if(!Info_Check() && checkInfo.EEPROM_OK)								//���EEPROM������Ϣ
		{
			//AT24C02_Clear(0, 255, 256);Iwdg_Feed();
			UsartPrintf(USART_DEBUG, "1.ssid_pswd in EEPROM\r\n");
			Info_Read();
		}
		else //û������
		{
			UsartPrintf(USART_DEBUG, "1.ssid_pswd in ROM\r\n");
		}
		
		UsartPrintf(USART_DEBUG, "2.SSID: %s,   PSWD: %s\r\n"
								"DEVID: %s,     APIKEY: %s\r\n"
								, netDeviceInfo.staName, netDeviceInfo.staPass
								, oneNetInfo.devID, oneNetInfo.apiKey);
	}
	
	Iwdg_Init(4, 1250); 														//64��Ƶ��ÿ��625�Σ�����1250�Σ�2s
	
	UsartPrintf(USART_DEBUG, "3.Hardware init OK\r\n");							//��ʾ��ʼ�����

}

/*
************************************************************
*	�������ƣ�	OS_TimerCallBack
*
*	�������ܣ�	��ʱ�������״̬��־λ
*
*	��ڲ�����	�����ʱ�����
*
*	���ز�����	��
*
*	˵����		��ʱ�����񡣶�ʱ�������״̬�������������趨ʱ�����������ӣ������ƽ̨����
************************************************************
*/
void OS_TimerCallBack(TimerHandle_t xTimer)
{
	
	if(oneNetInfo.netWork == 0)											//�������Ͽ�
	{
		if(++timerCount >= NET_TIME) 									//�������Ͽ���ʱ
		{
			UsartPrintf(USART_DEBUG, "Tips:		Timer Check Err\r\n");
			
			checkInfo.NET_DEVICE_OK = 0;								//���豸δ����־
			
			NET_DEVICE_ReConfig(0);										//�豸��ʼ����������Ϊ��ʼ״̬
			
			oneNetInfo.netWork = 0;
		}
	}
	else
	{
		timerCount = 0;													//�������
	}

}

/*
************************************************************
*	�������ƣ�	main
*
*	�������ܣ�	��ɳ�ʼ�����񣬴���Ӧ������ִ��
*
*	��ڲ�����	��
*
*	���ز�����	0
*
*	˵����		
************************************************************
*/
int main(void)
{
	
	Hardware_Init();								//Ӳ����ʼ��

	//����Ӧ������
	
	xTaskCreate((TaskFunction_t)IWDG_Task, "IWDG", IWDG_STK_SIZE, NULL, IWDG_TASK_PRIO, (TaskHandle_t*)&IWDG_Task_Handler);
	
	xTaskCreate((TaskFunction_t)USART_Task, "USART", USART_STK_SIZE, NULL, USART_TASK_PRIO, (TaskHandle_t*)&USART_Task_Handler);
	
	xTaskCreate((TaskFunction_t)HEART_Task, "HEART", HEART_STK_SIZE, NULL, HEART_TASK_PRIO, (TaskHandle_t*)&HEART_Task_Handler);
	
	xTaskCreate((TaskFunction_t)FAULT_Task, "FAULT", FAULT_STK_SIZE, NULL, FAULT_TASK_PRIO, (TaskHandle_t*)&FAULT_Task_Handler);
	
	xTaskCreate((TaskFunction_t)SENSOR_Task, "SENSOR", SENSOR_STK_SIZE, NULL, SENSOR_TASK_PRIO, (TaskHandle_t*)&SENSOR_Task_Handler);
	
	xTaskCreate((TaskFunction_t)SEND_Task, "SEND", SEND_STK_SIZE, NULL, SEND_TASK_PRIO, (TaskHandle_t*)&SEND_Task_Handler);
	
	xTaskCreate((TaskFunction_t)NET_Task, "NET", NET_STK_SIZE, NULL, NET_TASK_PRIO, (TaskHandle_t*)&NET_Task_Handler);
	
	xTaskCreate((TaskFunction_t)DATA_Task, "DATA", DATA_STK_SIZE, NULL, DATA_TASK_PRIO, (TaskHandle_t*)&DATA_Task_Handler);
	
	xTaskCreate((TaskFunction_t)ALTER_Task, "ALTER", ALTER_STK_SIZE, NULL, ALTER_TASK_PRIO, (TaskHandle_t*)&ALTER_Task_Handler);
	
	t1_Thdl = xTimerCreate("Timer1", 200, pdTRUE, (void *)1, (TimerCallbackFunction_t)OS_TimerCallBack);
	xTimerStart(t1_Thdl, 1);
	
	UsartPrintf(USART_DEBUG, "4.OSStart\r\n");		//��ʾ����ʼִ��
	
	vTaskStartScheduler();							//��ʼ�������
	
	return 0;

}

/*
************************************************************
*	�������ƣ�	IWDG_Task
*
*	�������ܣ�	������Ź�
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���Ź�����
************************************************************
*/
void IWDG_Task(void *pdata)
{

	while(1)
	{
	
		Iwdg_Feed(); 		//ι��
		
		RTOS_TimeDly(50); 	//��������250ms
	
	}

}

/*
************************************************************
*	�������ƣ�	HEART_Task
*
*	�������ܣ�	�������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		�������񡣷����������󲢵ȴ�������Ӧ�������趨ʱ����û����Ӧ������ƽ̨����
************************************************************
*/
void HEART_Task(void *pdata)
{

	while(1)
	{
		
		OneNet_HeartBeat();				//�������
		
		RTOS_TimeDly(4000);				//��������20s
	
	}

}

/*
************************************************************
*	�������ƣ�	SEND_Task
*
*	�������ܣ�	�ϴ�����������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ݷ�������
************************************************************
*/
void SEND_Task(void *pdata)
{

	while(1)
	{
		
		OneNet_SendData(kTypeSimpleJsonWithoutTime, dataStreamLen);	//�ϴ����ݵ�ƽ̨
		
		RTOS_TimeDly(3000);											//��������15s
		
	}

}

/*
************************************************************
*	�������ƣ�	USART_Task
*
*	�������ܣ�	����ƽ̨�·�������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ڽ�������������ģʽ��ʱ���ȴ�ƽ̨�·����������������
************************************************************
*/
void USART_Task(void *pdata)
{
	
	unsigned char *dataPtr;														//����ָ��

	while(1)
	{
		if(oneNetInfo.netWork && NET_DEVICE_Get_DataMode() == DEVICE_DATA_MODE)	//�������� �� ���������ģʽʱ
		{
			dataPtr = NET_DEVICE_GetIPD(0);										//���ȴ�����ȡƽ̨�·�������
			if(dataPtr != NULL)													//�������ָ�벻Ϊ�գ�������յ�������
			{
				OneNet_App(dataPtr);											//���д���
			}
		}
		
		RTOS_TimeDly(2);														//��������10ms
	
	}

}

/*
************************************************************
*	�������ƣ�	SENSOR_Task
*
*	�������ܣ�	���������ݲɼ�����ʾ
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���������ݲɼ����񡣽�����Ӵ����������ݲɼ�����ȡ����ʾ
************************************************************
*/
void SENSOR_Task(void *pdata)
{

	while(1)
	{

		if(checkInfo.SHT20_OK == DEV_OK) 						//ֻ���豸����ʱ���Ż��ȡֵ����ʾ
		{
//			SHT20_GetValue();									//�ɼ�����������
		
			sensor_read(&dht12Info.tempreture, &dht12Info.humidity);  //DHT12�ɼ���ʪ�ȡ�
		}
		
		
    adcsensor_read();

		
		RTOS_TimeDly(100); 										//��������500ms
	
	}

}

/*
************************************************************
*	�������ƣ�	DATA_Task
*
*	�������ܣ�	ƽ̨�·���������ݷ���
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ݷ�����������ƽ̨�·�ָ���ķ���������͸��ģʽ��ʱ���յ�֮���������أ���͸��ģʽ��Ϊ��Ҫ����'>'���ţ�����ʹ������ķ�ʽ��������
************************************************************
*/
void DATA_Task(void *pdata)
{

	while(1)
	{
	
		if(oneNetInfo.sendData == 1)		//������ݷ�����־λ������
		{
			oneNetInfo.sendData = OneNet_SendData(kTypeSimpleJsonWithoutTime, dataStreamLen);	//�ϴ����ݵ�ƽ̨
		}
		
		RTOS_TimeDly(10);					//��������50ms
	
	}

}

/*
************************************************************
*	�������ƣ�	FAULT_Task
*
*	�������ܣ�	����״̬������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ϴ������񡣵�������������豸����ʱ�����Ƕ�Ӧ��־λ��Ȼ���н��д���
************************************************************
*/
void FAULT_Task(void *pdata)
{

	while(1)
	{
		
		if(faultType != FAULT_NONE)									//��������־������
		{
			UsartPrintf(USART_DEBUG, "WARN:	Fault Process\r\n");
			Fault_Process();										//�����������
		}
	
		
		RTOS_TimeDly(10);											//��������50ms
	
	}

}

/*
************************************************************
*	�������ƣ�	NET_Task
*
*	�������ܣ�	�������ӡ�ƽ̨����
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���������������񡣻������������߼����������״̬������д�����״̬��Ȼ���������������
************************************************************
*/
void NET_Task(void *pdata)
{
	
	NET_DEVICE_IO_Init();													//�����豸IO��ʼ��
	NET_DEVICE_Reset();														//�����豸��λ
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);								//����Ϊ�����շ�ģʽ(����ESP8266Ҫ����AT�ķ��ػ���ƽ̨�·����ݵķ���)

	while(1)
	{
		
		if(!oneNetInfo.netWork && (checkInfo.NET_DEVICE_OK == DEV_OK))		//��û������ �� ����ģ���⵽ʱ
		{
			NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);						//����Ϊ�����շ�ģʽ
			
			if(!NET_DEVICE_Init())											//��ʼ�������豸������������
				OneNet_DevLink(oneNetInfo.devID, oneNetInfo.apiKey);		//����ƽ̨
		}
		
		if(checkInfo.NET_DEVICE_OK == DEV_ERR) 								//�������豸δ�����
		{
			NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);						//����Ϊ�����շ�ģʽ
			
			if(timerCount >= NET_TIME) 										//����������ӳ�ʱ
			{
				NET_DEVICE_Reset();											//��λ�����豸
				timerCount = 0;												//�������ӳ�ʱ����
				faultType = FAULT_NONE;										//��������־
			}
			
			if(!NET_DEVICE_SendCmd("AT\r\n", "OK"))							//�����豸���
			{
				UsartPrintf(USART_DEBUG, "NET Device :Ok\r\n");
				checkInfo.NET_DEVICE_OK = DEV_OK;							//��⵽�����豸�����
				NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);					//����Ϊ�����շ�ģʽ
			}
			else
				UsartPrintf(USART_DEBUG, "NET Device :Error\r\n");
		}
		
		RTOS_TimeDly(5);													//��������25ms
	
	}

}

/*
************************************************************
*	�������ƣ�	ALTER_Task
*
*	�������ܣ�	ͨ�����ڸ���SSID��PSWD��DEVID��APIKEY
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ĺ�ᱣ�浽EEPROM��
************************************************************
*/
void ALTER_Task(void *pdata)
{
    
    unsigned char usart1Count = 0;

    while(1)
    {
    
        memset(alterInfo.alterBuf, 0, sizeof(alterInfo.alterBuf));
		alterInfo.alterCount = 0;usart1Count = 0;
        while((strlen(alterInfo.alterBuf) != usart1Count) || (usart1Count == 0))	//�ȴ��������
        {
            usart1Count = strlen(alterInfo.alterBuf);								//���㳤��
            RTOS_TimeDly(20);														//ÿ100ms���һ��
        }
        UsartPrintf(USART_DEBUG, "\r\nusart1Buf Len: %d, usart1Count = %d\r\n",
									strlen(alterInfo.alterBuf), usart1Count);
        
		if(checkInfo.EEPROM_OK == DEV_OK)											//���EEPROM����
		{
			if(Info_Alter(alterInfo.alterBuf))										//������Ϣ
			{
				oneNetInfo.netWork = 0;												//���������ƽ̨
				NET_DEVICE_ReConfig(0);
			}
		}
    
    }

}

void PUSHDATA_Task(void *pdata)
{
	
	_Bool flag = 0;

	/*while(1)
	{
		
		if(flag)
			OneNet_PushData("4091363", "{redled}1", 9); //����kylin-v2.2
		else
			OneNet_PushData("4091363", "{redled}0", 9); //����kylin-v2.2
		
		flag = !flag;
	
		RTOS_TimeDly(2000);
	
	}*/

}

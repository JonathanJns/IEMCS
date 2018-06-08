/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	pm2.5.c
	*
	*	���ߣ� 		���
	*
	*	���ڣ� 		2018-03-15
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		���UART3��ʼ����PM2.5Ũ�Ȳɼ�
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "pm2.5.h"
#include "usart.h"

//��ʼ��IO ����3��bound:�����ʣ�	  
void PM2_5_USART_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��

 	USART_DeInit(USART3);  //��λ����3
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx;	//��ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������	3
  

	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

//GP2Y1050ת������
u8 GP2Y1050_Times;
//PM2.5Ũ�Ȼ���
float GP2Y1050_Quality;
//PM2.5ʵʱŨ��
float PM2_5_Cncentration;
////GP2Y1050ģ��ת��ϵ��
//u16 GP2Y1050_Coefficient;
//GP2Y1050���ջ���
u8 GP2Y1050_buf[5];
//GP2Y1050����״̬
//bit7 ���յ�����ͷ0XAA��˵�����Կ�ʼ��������
//bit6~0 ����������Ŀ
u8 GP2Y1050_STA=0;

//����3�жϷ������
void USART3_IRQHandler(void)                	
{
	u8 Res;
	if((USART_GetITStatus(USART3, USART_IT_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res=USART3->DR;
		//GP2Y1050���ճ���
		if(GP2Y1050_STA&(1<<7) && GP2Y1050_Times<GP2Y1050_COLLECT_TIMES)
		{
			if((GP2Y1050_STA&0x7f)<5)
			{
				GP2Y1050_buf[GP2Y1050_STA&0x7f]=Res;
				GP2Y1050_STA++;
			}
			else if(Res==0xff)
			{
				if(GP2Y1050_buf[4]==GP2Y1050_buf[0]+GP2Y1050_buf[1]+GP2Y1050_buf[2]+GP2Y1050_buf[3])  //����У��
				{
					GP2Y1050_Times++;
					GP2Y1050_Quality += (GP2Y1050_buf[0]<<8) + GP2Y1050_buf[1];
//					printf("adc:%d\r\n",(GP2Y1050_buf[0]<<8) + GP2Y1050_buf[1]);
					if(GP2Y1050_Times==0) GP2Y1050_Quality=0;
					if(GP2Y1050_Times==GP2Y1050_COLLECT_TIMES) //�ﵽ�ɼ�������ر��ж�
					{
//						USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);		//�رս����ж�
						GP2Y1050_Quality = GP2Y1050_Quality/GP2Y1050_COLLECT_TIMES*5/1023;  //ת���ɵ�ѹֵ

						
						PM2_5_Cncentration = GP2Y1050_Quality*172;
						GP2Y1050_Times=0;
//						UsartPrintf(USART_DEBUG, "PM2.5:  %f\r\n", PM2_5_Cncentration);
					}
				}
				GP2Y1050_STA=0;
			}
			else
			{
				GP2Y1050_STA=0;
			}
		}
		else if(Res==0xaa && GP2Y1050_Times<GP2Y1050_COLLECT_TIMES)
		{
			GP2Y1050_STA|=1<<7;
		}	 
	}	
} 

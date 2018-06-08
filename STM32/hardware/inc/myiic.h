#ifndef __MYIIC_H
#define __MYIIC_H
#include "DHT12.h"
//

//IO��������
#define SDA_IN()  {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=0x00000080;}//����������
#define SDA_OUT() {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=0x00000030;}//ͨ���������


//IO��������	 
//#define IIC_SCL    PBout(0) //SCL���
//#define IIC_SDA    PBout(1) //SDA���	 
//#define READ_SDA   PBin(1)  //SDA���� 

#define SDA_H	GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define SDA_L	GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define SDA_R	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)

#define SCL_H	GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define SCL_L	GPIO_ResetBits(GPIOB, GPIO_Pin_0)

extern u8 time_flag;

//IIC���в�������
void IIC_init(void);                //��ʼ��IIC��IO��				 
void IIC_start(void);				//����IIC��ʼ�ź�
void IIC_stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_ack(void);					//IIC����ACK�ź�
void IIC_Nack(void);				//IIC������ACK�ź�
void TIM3_Int_Init(u16 arr,u16 psc);

#endif

















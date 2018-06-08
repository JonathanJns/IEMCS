/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	DHT12.c
	*
	*	���ߣ� 		���
	*
	*	���ڣ� 		2018-03-22
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		���DHT12��ʼ������ʪ�Ȳɼ�
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

#include "DHT12.h"
#include "myiic.h"
//float Temprature,Humidity;//������ʪ�ȱ��� ���˱���Ϊȫ�ֱ���
DHT12_INFO dht12Info;;

/********************************************\
|* ���ܣ� DHT12_IIC��ʼ������	        *|
\********************************************/
void sensor_iic_init(void)
{
	IIC_Init();
}


/********************************************\
|* ���ܣ�DHT12��ȡ��ʪ�Ⱥ���       *|
\********************************************/
//������Humi_H��ʪ�ȸ�λ��Humi_L��ʪ�ȵ�λ��Temp_H���¶ȸ�λ��Temp_L���¶ȵ�λ��Temp_CAL��У��λ
//���ݸ�ʽΪ��ʪ�ȸ�λ��ʪ��������+ʪ�ȵ�λ��ʪ��С����+�¶ȸ�λ���¶�������+�¶ȵ�λ���¶�С����+ У��λ
//У�飺У��λ=ʪ�ȸ�λ+ʪ�ȵ�λ+�¶ȸ�λ+�¶ȵ�λ
u8 sensor_read(float* Temp,float* Humi)
{	
	u16 i=0;
	
	u8 Humi_H=0,Humi_L=0,Temp_H=0,Temp_L=0,Temp_CAL=0,temp=0;

	IIC_start();    //����������ʼ�ź�
	IIC_Send_Byte(0Xb8);    //����IIC��ַ
	if(!IIC_Wait_ack())  //�ȴ��ӻ�Ӧ���źţ�����Ӧ�𣺿���IICͨѶƵ���Ƿ�̫�죬���ߴ��������ߴ���
	{
		i=0;
		IIC_Send_Byte(0);
		while(IIC_Wait_ack())//�ȴ��ӻ�Ӧ���ź�
		{
		 	if(++i>=500)
			{
			 	break;
			}		 
		} 
		i=0;
		IIC_start();       //����������ʼ�ź�
		IIC_Send_Byte(0Xb9);     //���Ͷ�ָ��
		while(IIC_Wait_ack())    //�ȴ��ӻ�Ӧ���ź�
		{
		 	if(++i>=500)
			{
			 	break;
			}			 
		}
	
		Humi_H=IIC_Read_Byte(1);   //��ȡʪ�ȸ�λ
		Humi_L=IIC_Read_Byte(1);   //��ȡʪ�ȵ�λ
		Temp_H=IIC_Read_Byte(1);   //��ȡ�¶ȸ�λ
		Temp_L=IIC_Read_Byte(1);   //��ȡ�¶ȵ�λ
		Temp_CAL=IIC_Read_Byte(0); //��ȡУ��λ
		IIC_stop();	   //����ֹͣ�ź�	
		temp = (u8)(Humi_H+Humi_L+Temp_H+Temp_L);//ֻȡ��8λ
//		printf("Humi_H:%d Humi_L:%d Temp_H:%d Temp_L:%d Temp_CAL:%d\r\n",Humi_H,Humi_L,Temp_H,Temp_L,Temp_CAL);
		if(Temp_CAL==temp)//���У��ɹ�����������
		{
			*Humi=Humi_H*10+Humi_L; //ʪ��
			if(Temp_L&0X80)	//Ϊ���¶�
			{
				*Temp =0-(Temp_H*10+((Temp_L&0x7F)));
			}
			else   //Ϊ���¶�
			{
				*Temp=Temp_H*10+Temp_L;//Ϊ���¶�
			} 	
			//�ж���ʪ���Ƿ񳬹�������Χ���¶ȣ�-20��~60���϶ȣ�ʪ�ȣ�20%RH~95%RH��		
			if(*Humi>950) 
			{
			  *Humi=950;
			}
			if(*Humi<200)
			{
				*Humi =200;
			}
			if(*Temp>600)
			{
			  *Temp=600;
			}
			if(*Temp<-200)
			{
				*Temp = -200;
			}
			*Temp=*Temp/10.0;//����Ϊ�¶�ֵ
			*Humi=*Humi/10.0; //����Ϊʪ��ֵ
			
//			printf("\r\n�¶�Ϊ:  %.1f  ��\r\n",*Temp); //��ʾ�¶�
//			printf("ʪ��Ϊ:  %2.1f  %%RH\r\n",*Humi);//��ʾʪ��	
			return 1;
		}
		else //У��ʧ��
		{
			UsartPrintf(USART_DEBUG, "CRC Error !!\r\n");
			return 0;
		}
	}else
	{
	  UsartPrintf(USART_DEBUG, "Sensor Error !!\r\n");
		return 0;
	}	

}

//��βɼ���ʪ��ȡƽ��ֵ������timesΪ����
//void Continuous_Collect(float* Temp,float* Humi,u8 times)
//{
//	float Temp_buf=0,Humi_buf=0,Temp_Buf=0,Humi_Buf=0;
//	u8 i=times;
//	for(;i;)
//	{
//		delay_ms(40);  //��ʱ40ms��DHT12�ɼ�̫Ƶ�����ݻ����
//		if(sensor_read(&Temp_buf,&Humi_buf))
//		{
//			Temp_Buf+=Temp_buf;
//			Humi_Buf+=Humi_buf;
//			i--;
//		}
//	}
//	*Temp=Temp_Buf/times;
//	*Humi=Humi_Buf/times;
//}


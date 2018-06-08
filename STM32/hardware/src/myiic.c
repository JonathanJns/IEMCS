/**
	************************************************************
	*
	*˵����ģ��IIC���ó���
	*ע�⣺DHT12��IIC��ͨѶƵ�ʲ��ó���50KHZ
	*
	************************************************************
**/
#include "myiic.h"
#include "delay.h"

//����IIC��ʼ�ź�
void IIC_start(void)
{
	SDA_OUT();     //sda�����
	SDA_H;	  	  
	SCL_H;
	DelayUs(14);
 	SDA_L;//��ʼ����SCLΪ��ʱ�����ݴӸߵ��ͱ仯
	DelayUs(14);
	SCL_L;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_stop(void)
{
	SDA_OUT();//sda�����
	SCL_L;
	SDA_L;//ֹͣ:��SCLΪ��ʱ�����ݴӵ͵��߱仯
 	DelayUs(14);
	SCL_H;
	DelayUs(14);	 
	SDA_H;//����I2C���߽����ź�
							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_ack(void)
{
	u8 ucErrTime=0;
	SCL_L;
	SDA_H;
	SDA_IN();      //SDA����Ϊ����  
	DelayUs(14);	   
	SCL_H;
	DelayUs(14);	 
	while(SDA_R)//�ȵ�SDA���
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_stop();
			return 1;
		}
		DelayUs(1);
	}
	SCL_L;//ʱ�����0
	DelayUs(10); 
	return 0;  
} 
//����ACKӦ��
void IIC_ack(void)
{
	SCL_L;
	SDA_OUT();
	SDA_L;
	DelayUs(15);
	SCL_H;
	DelayUs(15);
	SCL_L;
}
//������ACKӦ��		    
void IIC_Nack(void)
{
	SCL_L;
	SDA_OUT();
	SDA_H;
	DelayUs(15);
	SCL_H;
	DelayUs(15);
	SCL_L;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    SCL_L;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		SCL_L;
		if((txd&0x80)>>7)
			SDA_H;
		else
			SDA_L;
		txd<<=1; 	  
		DelayUs(15);   //��ʱ
		SCL_H;
		DelayUs(15); 	
    }
	SCL_L;	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        SCL_L; 
        DelayUs(50);
		SCL_H;
		DelayUs(50);
        receive<<=1;
        if(SDA_R)receive++;   
    }
        SCL_L;						 
    if (!ack)
        IIC_Nack();//����nACK
    else
        IIC_ack(); //����ACK   
    return receive;
}




























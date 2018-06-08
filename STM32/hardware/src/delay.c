/**
	************************************************************
	*
	*	˵���� 		����Timer6������ʽ��ʱ
	*
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//delayͷ�ļ�
#include "delay.h"

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

//��ʱϵ��
unsigned char UsCount = 0;
unsigned short MsCount = 0;


//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӣ�������������SYSTICKʱ��Ƶ��ΪAHB/8
//����Ϊ�˼���FreeRTOS�����Խ�SYSTICK��ʱ��Ƶ�ʸ�ΪAHB��Ƶ�ʣ�
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init()
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//ѡ���ⲿʱ��  HCLK
	fac_us=SystemCoreClock/1000000;				//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=SystemCoreClock/1000000;				//ÿ���ӵļ������� ��λΪM  
	reload*=1000000/configTICK_RATE_HZ;			//����configTICK_RATE_HZ�趨���ʱ��
												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����	
	fac_ms=1000/configTICK_RATE_HZ;				//����OS������ʱ�����ٵ�λ	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 						//ÿ1/configTICK_RATE_HZ���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
}		



/*
************************************************************
*	�������ƣ�	Delay_Init
*
*	�������ܣ�	systick��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Delay_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM3->CR1 |= 1UL << 3;									//������ģʽ
	
	TIM3->EGR |= 1;											//��������
	
	TIM3->DIER = 0;											//��ֹ�����ж�
	
	TIM3->CR1 &= (unsigned short)~TIM_CR1_CEN;				//ֹͣ��ʱ

}

/*
************************************************************
*	�������ƣ�	DelayUs
*
*	�������ܣ�	΢�뼶��ʱ
*
*	��ڲ�����	us����ʱ��ʱ��
*
*	���ز�����	��
*
*	˵����		��ʱ��(21MHz)�����ʱ798915us
************************************************************
*/
void DelayUs(unsigned short us)
{

	TIM3->ARR = us;
	
	TIM3->PSC = 71;									//timer6Ϊ72MHz������Ϊ71+1��Ƶ��1MHz��1us����һ��
	
	TIM3->CR1 |= (unsigned short)TIM_CR1_CEN;		//��ʼ��ʱ
	
	while(!(TIM3->SR & 1));							//�ȴ������ж��¼�
	
	TIM3->SR &= ~(1UL << 0);						//�����־

}

/*
************************************************************
*	�������ƣ�	DelayXms
*
*	�������ܣ�	���뼶��ʱ
*
*	��ڲ�����	ms����ʱ��ʱ��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void DelayXms(unsigned short ms)
{

	if(ms < 32768)
	{
		TIM3->ARR = (ms << 1);						//˫������ֵ
		
		TIM3->PSC = 35999;							//timer6Ϊ72MHz������Ϊ35999+1��Ƶ��2KHz��500us����һ��
		
		TIM3->CR1 |= (unsigned short)TIM_CR1_CEN;	//��ʼ��ʱ
		
		while(!(TIM3->SR & 1));						//�ȴ������ж��¼�
		
		TIM3->SR &= ~(1UL << 0);					//�����־
	}

}

/*
************************************************************
*	�������ƣ�	DelayMs
*
*	�������ܣ�	΢�뼶����ʱ
*
*	��ڲ�����	ms����ʱ��ʱ��
*
*	���ز�����	��
*
*	˵����		��ε���DelayXms����������ʱ
************************************************************
*/
void DelayMs(unsigned short ms)
{

	unsigned char repeat = 0;
	unsigned short remain = 0;
	
	repeat = ms / 500;
	remain = ms % 500;
	
	while(repeat)
	{
		DelayXms(500);
		repeat--;
	}
	
	if(remain)
		DelayXms(remain);

}

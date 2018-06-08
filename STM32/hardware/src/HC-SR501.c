/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	HC-SR501.c
	*
	*	���ߣ� 		���
	*
	*	���ڣ� 		2018-03-15
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		 HC-SR501��������Ӧ����ʵ��
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//����ͷ�ļ�
#include "HC-SR501.h"



//���ذ���IO���ų�ʼ��.
void HC_SR501_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��(����)KEY1���Ŷ�ӦIO�˿�ʱ�� */  
  RCC_APB2PeriphClockCmd(HC_SR501_RCC_CLOCKGPIO, ENABLE);
   
  /* �趨KEY1��Ӧ����IO��� */
  GPIO_InitStructure.GPIO_Pin = HC_SR501_GPIO_PIN;  
  /* �趨KEY1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
  /* �趨KEY1��Ӧ����IOΪ��������ģʽ */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  /* ��ʼ��KEY1��Ӧ����IO */
  GPIO_Init(HC_SR501_GPIO, &GPIO_InitStructure);
}

/**
  * ��������: �򵥴ֱ�����ʱ����
  * �������: time����ʱʱ������
  * �� �� ֵ: ��
  * ˵    �����������
  */
static void HC_SR501_ScanDelay(void)
{  
  uint32_t i,j;
  for(i=0;i<100;++i)
    for(j=0;j<1000;++j){ }		
}

/**
  * ��������: ��ȡ����KEY1��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
HC_SR501_State_TypeDef HC_SR501_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(GPIO_ReadInputDataBit(HC_SR501_GPIO,HC_SR501_GPIO_PIN)==HC_SR501_ACTIVE_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HC_SR501_ScanDelay();
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(GPIO_ReadInputDataBit(HC_SR501_GPIO,HC_SR501_GPIO_PIN)==HC_SR501_ACTIVE_LEVEL)
    {      
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return HC_SR501_HIGH;
    }
  }
  /* ����û�����£�����û������״̬ */
  return HC_SR501_LOW;
}


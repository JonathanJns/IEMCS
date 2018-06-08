#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� --------------------------------------------------------------*/
typedef enum
{
  HC_SR501_LOW   = 0,
  HC_SR501_HIGH = 1,
}HC_SR501_State_TypeDef;

/* �궨�� --------------------------------------------------------------------*/
#define HC_SR501_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOA
#define HC_SR501_GPIO_PIN                 GPIO_Pin_1
#define HC_SR501_GPIO                     GPIOA
#define HC_SR501_ACTIVE_LEVEL             1

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void HC_SR501_GPIO_Init(void);
HC_SR501_State_TypeDef HC_SR501_StateRead(void);


#endif  // __BSP_KEY_H__

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

#ifndef __INITSET_H
#define __INTTSET_H
    
#include "stm8s.h"

#define led1_pin  0x10     //PD4
#define System_clk 16000000


#define LED_OFF  GPIOD->ODR |=  led1_pin
#define LED_ON   GPIOD->ODR &= ~led1_pin

void SCLK_24MInit(void);
void Delay_us(u16 nCount); 

void Delay_ms(u16 nCount) ;

void CLK_Init(void);

void LED_Init();


#endif
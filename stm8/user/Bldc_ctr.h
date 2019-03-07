#ifndef __BLDC_CTR_H
#define __BLDC_CTR_H

#define L1_Pin 0x08  //D3
#define L2_Pin 0x04  //D2
#define L3_Pin 0x08  //C3

#define Z1_Pin 0x10 //PB4
#define Z2_Pin 0x20 //PC5
#define Z3_Pin 0x20 //PB5

#define L1_ON() GPIOD->ODR |= L1_Pin
#define L2_ON() GPIOD->ODR |= L2_Pin
#define L3_ON() GPIOC->ODR |= L3_Pin

#define L1_OFF() GPIOD->ODR &= ~L1_Pin
#define L2_OFF() GPIOD->ODR &= ~L2_Pin
#define L3_OFF() GPIOC->ODR &= ~L3_Pin

void Bldc_GPIO_Init(void);
void H1_PWM(unsigned int );
void H2_PWM(unsigned int );
void H3_PWM(unsigned int );
void H1_OFF(void);
void H2_OFF(void);
void H3_OFF(void);
void chang_phase_1(unsigned int pwm);
void chang_phase_2(unsigned int pwm);
void chang_phase_3(unsigned int pwm);
void chang_phase_4(unsigned int pwm);
void chang_phase_5(unsigned int pwm);
void chang_phase_6(unsigned int pwm);
void bldc_start(void);
void text_task(void);
void close_all(void);
#endif
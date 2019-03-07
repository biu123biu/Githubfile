#include <Bldc_ctr.h>
#include <stm8s.h>
#include <InitSet.h>

extern unsigned int speed ;

void Bldc_GPIO_Init(void)
{
  GPIOD->DDR |= L1_Pin|L2_Pin;//设置PD4为输出模式
  GPIOD->CR1 |= L1_Pin|L2_Pin;//推完输出
  GPIOD->CR2 |= L1_Pin|L2_Pin;//摆率设置为10M
  
  GPIOC->DDR |= 0xD0;//设置PD4为输出模式
  GPIOC->CR1 |= 0xD0;//推完输出
  GPIOC->CR2 |= 0xD0;//摆率设置为10M
  
  
  GPIOC->DDR |= L3_Pin;//设置PD4为输出模式
  GPIOC->CR1 |= L3_Pin;//推完输出
  GPIOC->CR2 |= L3_Pin;//摆率设置为10M
  
  L1_OFF();L2_OFF();L3_OFF();
  
  
  GPIOB->DDR &= ~( Z1_Pin|Z3_Pin);  //设置PB4,PB5为输入模式
  GPIOB->CR1 &= ~( Z1_Pin|Z3_Pin);//浮空输入
  GPIOB->CR2 |=  (Z1_Pin|Z3_Pin);//打开输入中断
  
  GPIOC->DDR &= ~Z2_Pin;  //设置PC5为输入模式
  GPIOC->CR1 &= ~Z2_Pin;//浮空输入
  GPIOC->CR2 |= Z2_Pin;//打开输入中断
  
  EXTI->CR1 |= 0X3C;//打开PB,PC,上升沿、下降沿中断
  
}

void H1_PWM(unsigned int temp)
{
  
    TIM1->CCMR2 =  0x74;
    TIM1->CCR2H = (unsigned char)(temp>>8);
    TIM1->CCR2L = (unsigned char) temp;
}

void H2_PWM(unsigned int temp)
{
  
  TIM1->CCMR1 =  0x74;
  TIM1->CCR1H = temp>>8;
  TIM1->CCR1L = temp;
}

void H3_PWM(unsigned int temp)
{
  
  TIM1->CCMR4 =  0x74;
  TIM1->CCR4H = temp>>8;
  TIM1->CCR4L = temp;
}

void H1_OFF()
{
  TIM1->CCMR2 =  0x54;
 // H1_PWM(0);
}

void H2_OFF()
{
  TIM1->CCMR1 =  0x54;
 // H2_PWM(0);
}

void H3_OFF()
{
  TIM1->CCMR4 =  0x54;
 //H3_PWM(0);
}


void  chang_phase_1(unsigned int pwm)
{
     H3_OFF();L3_OFF();
      L2_ON();  
}

void chang_phase_2(unsigned int pwm)
{
      H1_OFF();L1_OFF();
      H3_PWM(pwm); 
     
}

void chang_phase_3(unsigned int pwm)
{
       H2_OFF();L2_OFF();
       L1_ON();
}

void chang_phase_4(unsigned int pwm)
{
      H3_OFF();L3_OFF();
      H2_PWM(pwm); 
}

void chang_phase_5(unsigned int pwm)
{
      H1_OFF();L1_OFF();
      L3_ON(); 
}


void chang_phase_6(unsigned int pwm)
{
      H2_OFF();L2_OFF();
      H1_PWM(pwm); 
}

void close_all(void)
{
  H1_OFF();H2_OFF();H3_OFF();
  L1_ON();L2_ON();L3_ON();
  Delay_us(10);
  L1_OFF();L2_OFF();L3_OFF();
}

void text_task(void)
{
  close_all();
  H3_OFF();L3_OFF();L1_OFF();
  H2_OFF(); L2_ON();
  H1_PWM(10);  
      
  while(1);
  
}

void bldc_start(void)
{
  unsigned int time_start = 5500;
  unsigned int step = 20;
  speed  = 200;
  
	while(step--)
	{
            chang_phase_1(speed);Delay_us(time_start);
                                 
            chang_phase_2(speed);Delay_us(time_start);
                                 
            chang_phase_3(speed);Delay_us(time_start);
                                 
            chang_phase_4(speed);Delay_us(time_start);
                                 
            chang_phase_5(speed);Delay_us(time_start);
                                 
            chang_phase_6(speed);Delay_us(time_start);
            
            if(time_start>2000) time_start-=200;
	}
}


unsigned char gpio_temp=0;
unsigned char state=1;
 unsigned char temp1,temp2;
#pragma vector=6   //端口B中断  Z1 Z3
__interrupt void GPIOB_interrupt_handler(void)
{
 temp1 = (GPIOB->IDR & 0X30); //读取引脚状态
 temp2 = gpio_temp^temp1;
 if(temp2==0x10)  //pb4中断 Z1
 {
   if(GPIOB->IDR & 0X10 )  //上升沿
   {
     if(state == 6)
     {
       chang_phase_6(speed);
       state=1;
     }
   }
   else                   //下降沿
   {
     if(state == 3)
     {
       chang_phase_3(speed);
       state=4;
     }
  }
 }
 else if(temp2==0x20)//pb5中断 Z3
 {
  if(GPIOB->IDR & 0X20) //上升沿
    {
      if(state == 2)
     {
      chang_phase_2(speed);
      state=3;
     }
    }
    else                //下降沿
    { 
      if(state == 5)
     {
      chang_phase_5(speed);
      state=6;
     }
    }
  }
  gpio_temp=temp1;
  temp1=0;temp2=0;
}

#pragma vector=7 //端口C中断 Z2
__interrupt void GPIOC_interrupt_handler(void)
{
if(GPIOC->IDR & 0X20)
  {
     if(state == 4)
     {
        chang_phase_4(speed);
        state=5;
     }
  }
  else
  {
     if(state == 1)
     {
        chang_phase_1(speed);
        state=2;
     }
  }
}
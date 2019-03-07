#include <stm8s.h>
#include <Tim.h>
/***************************************************************************
* 函 数 名: TIM1_PwmSet
* 功能说明: 配置定时器1通道1pwm数据
* 形 参： prc分频值，count 计数值，既频率，high_count占空比
* 返 回 值: 无
***************************************************************************/
void TIM1_PwmSet(unsigned int prc,unsigned int count)
{
    TIM1->ARRH = (unsigned char)(count >> 8); //设置自动重装值
    TIM1->ARRL = (unsigned char)(count);

    TIM1->PSCRH = (unsigned char)((prc-1) >> 8); //设置分频值
    TIM1->PSCRL = (unsigned char)(prc-1);
    
    TIM1->CR1 = 0X80; //使能自动重装
    
    TIM1->CCER1 = 0X33;
    TIM1->CCER2 = 0X30;
    
    TIM1->CCMR1 =  0x78;
    TIM1->CCMR2 =  0x78;
    TIM1->CCMR4 =  0x78;
    
    TIM1->CCR1H = 0;TIM1->CCR1L = 0;
    TIM1->CCR2H = 0;TIM1->CCR2L = 0;
    TIM1->CCR4H = 0;TIM1->CCR4L = 0;
    
    TIM1->BKR |= 0x80;//输出势能
    TIM1->CR1 |= 0x01;
}


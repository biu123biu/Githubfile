#include <stm8s.h>
#include <Tim.h>
/***************************************************************************
* �� �� ��: TIM1_PwmSet
* ����˵��: ���ö�ʱ��1ͨ��1pwm����
* �� �Σ� prc��Ƶֵ��count ����ֵ����Ƶ�ʣ�high_countռ�ձ�
* �� �� ֵ: ��
***************************************************************************/
void TIM1_PwmSet(unsigned int prc,unsigned int count)
{
    TIM1->ARRH = (unsigned char)(count >> 8); //�����Զ���װֵ
    TIM1->ARRL = (unsigned char)(count);

    TIM1->PSCRH = (unsigned char)((prc-1) >> 8); //���÷�Ƶֵ
    TIM1->PSCRL = (unsigned char)(prc-1);
    
    TIM1->CR1 = 0X80; //ʹ���Զ���װ
    
    TIM1->CCER1 = 0X33;
    TIM1->CCER2 = 0X30;
    
    TIM1->CCMR1 =  0x78;
    TIM1->CCMR2 =  0x78;
    TIM1->CCMR4 =  0x78;
    
    TIM1->CCR1H = 0;TIM1->CCR1L = 0;
    TIM1->CCR2H = 0;TIM1->CCR2L = 0;
    TIM1->CCR4H = 0;TIM1->CCR4L = 0;
    
    TIM1->BKR |= 0x80;//�������
    TIM1->CR1 |= 0x01;
}


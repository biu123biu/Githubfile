#include <Bldc_SpeedCtr.h>
#include <stm8s.h>

#define f_master 20000000



void UART_Init(long baud)
{
  unsigned int count ;
  count = f_master/baud;
  UART1->CR1=0x00;//1 ����ʼλ��8 ������λ����У��λ
  UART1->CR2=0x3c;//�����ж�ʹ��,���ܿ����жϣ�ʹ�ܷ��͡�����
  UART1->CR3=0x00;//1 ��ֹͣλ
  UART1->SR=0;
  
  UART1->BRR2=(unsigned char)(((count>>8)&0xf0)|(count&0x0f));;
  UART1->BRR1=(unsigned char)(count>>4);
  count = 10;
  while(count--)
  UART1_SendByte(10-count);
}

void UART1_SendByte(unsigned char dat)
{
  while((UART1->SR & 0X80)==0);
  UART1->DR=dat;
}

unsigned char temp;
#pragma vector=20
__interrupt void uart1_interrupt_handler(void)
{
    temp = UART1->SR ;


    if(temp & 0x08) //�ж��Ƿ��жѻ�����
    {
      UART1->SR &= 0xf7; //�����־λ
    }	
    else if(temp & 0x10) //�����ж�
    {
      UART1->SR &= 0xef; //�����־λ

    }
    else //���������ж�
    {
       UART1->SR &= 0xdf; //�����־λ

    }

}



/***************************************************************************
* �� �� ��: TIM1_InitSet
* ����˵��: ��ʱ��1����������ã��ͽ��в�������
* �� �Σ�  pre��Ԥ��Ƶ����count������
* �� �� ֵ: ��
***************************************************************************/
void TIM2_InitSet(unsigned int pre , unsigned int count)
{
    
    TIM2->ARRH = (unsigned char)(count >> 8); //�����Զ���װֵ
    TIM2->ARRL = (unsigned char)(count);
    
    TIM2->CNTRH = 0x00;//������  �ռ�����
    TIM2->CNTRL = 0x00;//
    TIM2->PSCR  = pre;//ʱ��
    
    TIM2->CCER2 |=0x02;//�����ز���

    TIM2->CCMR3 = 0x01;//����TIM2_CCMR3�е�CC3SλΪ1,CC3ͨ������Ϊ����,IC1ӳ�䵽TI3FP3��  
    
    TIM2->CR1  = 0x00;//��ֹԤװ��        
    TIM2->IER   |= 0x08;//CC1IE=1,ʹ�ܲ���/�Ƚ�3�ж�  �򿪸����ж�
    
    TIM2->CR1   |= 0x01;//ʹ�ܶ�ʱ/������  
    TIM2->CCER2 |=0x01;//ʹ�ܲ���
}

unsigned int CAP_VALUE;;
#pragma vector=16
__interrupt void Tim2_interrupt_handler(void)
{
 if(TIM2->SR1&0X08)
 {
     TIM2->SR1&=0XF7;
   if(TIM2->CCER2 & 0x02)  //�����ز���
   {
     TIM2->CCER2 &= 0xfd; //����Ϊ�����ز���
     TIM2->CNTRH = 0x00;//������  �ռ�����
     TIM2->CNTRL = 0x00;//
   }
   else //�½��ز���
   {
     TIM2->CCER2 |=0x02;//����Ϊ�����ز���
     CAP_VALUE =   TIM2->CNTRH;
     CAP_VALUE<<=8;
     CAP_VALUE |=  TIM2->CNTRL;
     TIM2->CNTRH = 0x00;//������  �ռ�����
     TIM2->CNTRL = 0x00;//
     UART1_SendByte(0X00);
     UART1_SendByte(CAP_VALUE>>8);
     UART1_SendByte(CAP_VALUE);
   }
 }
 TIM2->SR1=0;
}

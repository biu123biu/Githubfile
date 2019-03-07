#include <Bldc_SpeedCtr.h>
#include <stm8s.h>

#define f_master 20000000



void UART_Init(long baud)
{
  unsigned int count ;
  count = f_master/baud;
  UART1->CR1=0x00;//1 个起始位，8 个数据位，无校验位
  UART1->CR2=0x3c;//接收中断使能,是能空闲中断，使能发送、接收
  UART1->CR3=0x00;//1 个停止位
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


    if(temp & 0x08) //判断是否有堆积数据
    {
      UART1->SR &= 0xf7; //清楚标志位
    }	
    else if(temp & 0x10) //空闲中断
    {
      UART1->SR &= 0xef; //清楚标志位

    }
    else //接收数据中断
    {
       UART1->SR &= 0xdf; //清楚标志位

    }

}



/***************************************************************************
* 函 数 名: TIM1_InitSet
* 功能说明: 定时器1进行相关配置，和进行捕获配置
* 形 参：  pre：预分频器，count计数器
* 返 回 值: 无
***************************************************************************/
void TIM2_InitSet(unsigned int pre , unsigned int count)
{
    
    TIM2->ARRH = (unsigned char)(count >> 8); //设置自动重装值
    TIM2->ARRL = (unsigned char)(count);
    
    TIM2->CNTRH = 0x00;//计数器  空间清零
    TIM2->CNTRL = 0x00;//
    TIM2->PSCR  = pre;//时钟
    
    TIM2->CCER2 |=0x02;//上升沿捕获

    TIM2->CCMR3 = 0x01;//配置TIM2_CCMR3中的CC3S位为1,CC3通道配置为输入,IC1映射到TI3FP3上  
    
    TIM2->CR1  = 0x00;//禁止预装载        
    TIM2->IER   |= 0x08;//CC1IE=1,使能捕获/比较3中断  打开更新中断
    
    TIM2->CR1   |= 0x01;//使能定时/计数器  
    TIM2->CCER2 |=0x01;//使能捕获
}

unsigned int CAP_VALUE;;
#pragma vector=16
__interrupt void Tim2_interrupt_handler(void)
{
 if(TIM2->SR1&0X08)
 {
     TIM2->SR1&=0XF7;
   if(TIM2->CCER2 & 0x02)  //上升沿捕获
   {
     TIM2->CCER2 &= 0xfd; //设置为上升沿捕获
     TIM2->CNTRH = 0x00;//计数器  空间清零
     TIM2->CNTRL = 0x00;//
   }
   else //下降沿捕获
   {
     TIM2->CCER2 |=0x02;//设置为上升沿捕获
     CAP_VALUE =   TIM2->CNTRH;
     CAP_VALUE<<=8;
     CAP_VALUE |=  TIM2->CNTRL;
     TIM2->CNTRH = 0x00;//计数器  空间清零
     TIM2->CNTRL = 0x00;//
     UART1_SendByte(0X00);
     UART1_SendByte(CAP_VALUE>>8);
     UART1_SendByte(CAP_VALUE);
   }
 }
 TIM2->SR1=0;
}

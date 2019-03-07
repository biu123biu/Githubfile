/********************************************************* 
 * uarth
 * sin发生器------>串口相关函数包括串口数据解码
 * 时间：2016年11月21日
 * 终：
 * 开发者：ZGY
 * 单位：重庆理工大学
 **********************************************************/
#ifndef __UART_H
#define __UART_H

/***************************************************************************
* 函 数 名: UART_Init
* 功能说明: uart初始化韩式
* 形 参： buad：波特率选择，固定8数据位，1位校验位，1停止位
* 返 回 值: 无
***************************************************************************/
void UART_Init(uint32_t buad)
{
  
    GPIOD->DDR |= 0X20;//PD5设置为输出模式
    GPIOD->CR1 |= 0X20;//推挽输出
    GPIOD->CR2 |= 0X20;//摆率设置为10M
    
    GPIOD->DDR &=~0X40;//PD6设置为输入
    GPIOD->CR1 |= 0X40;//上拉输出
    GPIOD->CR2 &=~0X40;//禁止外部中断

   
    UART1->CR1 = 0X00;//8位数据格式 无校验位
    UART1->CR2 = 0X2C;//使能接收中断，使能发送、接收
    UART1->CR3 = 0X00;//一位停止位 
    UART1->BRR2=0X0B;
    UART1->BRR1=0X08; //115200

}

/***************************************************************************
* 函 数 名: Send_Dat
* 功能说明: uart发送一字节数据
* 形 参： dat：待发送数据
* 返 回 值: 无
***************************************************************************/
void Send_Dat(unsigned char dat)
{
        while((UART1->SR&0x80)==0);
            UART1->DR = dat;
}

extern unsigned char UART_Recbuf[10]; //串口接收缓存
extern unsigned char flag;    //bit8串口接收完成标志，
unsigned char num=0; //接收数据个数

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    UART1->SR = ~0X20;  //清除接收标志位

    if(UART1->SR & UART1_SR_OR) //判断是否有堆积数据
    {
      UART1->SR &= (~UART1_SR_OR); //清楚标志位
    }	
    else
    {
       
    }
    UART_Recbuf[num++]=UART1->DR;
    if(num==8)
    {
       flag |= 0x80;
       num=0;
    }
   
 }
/***************************************************************************
* 函 数 名: Sendnum
* 功能说明: 以字符形式发数字
* 形 参： dat：待发送数据
* 返 回 值: 无
***************************************************************************/
void Sendnum(unsigned int mun)
{
  
  unsigned char buf[8],i=0,index=0;
  unsigned int num;
  num=mun;
  while(mun/10!=0)
  {
    mun =mun/10;
    index++;
  }
  index++;

  buf[i++] = num%10;
  buf[i++] = num%100/10;
  buf[i++] = num%1000/100;
  buf[i++] = num%10000/1000;
  buf[i++] = num%100000/10000;
  while(--index)
  {
    Send_Dat(0x30+buf[index]);
  }
    Send_Dat(0x30+buf[0]);

}

void SendString(unsigned char *str)
{

  while (*str!=0)
  {
    Send_Dat(*str);
    str++;
  }
  
}
#endif

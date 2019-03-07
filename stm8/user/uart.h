/********************************************************* 
 * uarth
 * sin������------>������غ��������������ݽ���
 * ʱ�䣺2016��11��21��
 * �գ�
 * �����ߣ�ZGY
 * ��λ����������ѧ
 **********************************************************/
#ifndef __UART_H
#define __UART_H

/***************************************************************************
* �� �� ��: UART_Init
* ����˵��: uart��ʼ����ʽ
* �� �Σ� buad��������ѡ�񣬹̶�8����λ��1λУ��λ��1ֹͣλ
* �� �� ֵ: ��
***************************************************************************/
void UART_Init(uint32_t buad)
{
  
    GPIOD->DDR |= 0X20;//PD5����Ϊ���ģʽ
    GPIOD->CR1 |= 0X20;//�������
    GPIOD->CR2 |= 0X20;//��������Ϊ10M
    
    GPIOD->DDR &=~0X40;//PD6����Ϊ����
    GPIOD->CR1 |= 0X40;//�������
    GPIOD->CR2 &=~0X40;//��ֹ�ⲿ�ж�

   
    UART1->CR1 = 0X00;//8λ���ݸ�ʽ ��У��λ
    UART1->CR2 = 0X2C;//ʹ�ܽ����жϣ�ʹ�ܷ��͡�����
    UART1->CR3 = 0X00;//һλֹͣλ 
    UART1->BRR2=0X0B;
    UART1->BRR1=0X08; //115200

}

/***************************************************************************
* �� �� ��: Send_Dat
* ����˵��: uart����һ�ֽ�����
* �� �Σ� dat������������
* �� �� ֵ: ��
***************************************************************************/
void Send_Dat(unsigned char dat)
{
        while((UART1->SR&0x80)==0);
            UART1->DR = dat;
}

extern unsigned char UART_Recbuf[10]; //���ڽ��ջ���
extern unsigned char flag;    //bit8���ڽ�����ɱ�־��
unsigned char num=0; //�������ݸ���

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    UART1->SR = ~0X20;  //������ձ�־λ

    if(UART1->SR & UART1_SR_OR) //�ж��Ƿ��жѻ�����
    {
      UART1->SR &= (~UART1_SR_OR); //�����־λ
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
* �� �� ��: Sendnum
* ����˵��: ���ַ���ʽ������
* �� �Σ� dat������������
* �� �� ֵ: ��
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

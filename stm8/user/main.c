#include "InitSet.h"
#include "Bldc_ctr.h"
#include "Tim.h"
#include "Bldc_SpeedCtr.h"
#include "stm8s.h"

unsigned char UART_Recbuf[10]; //���ڽ��ջ���
unsigned char flag=0;    //bit8���ڽ�����ɱ�־��
unsigned int  time=0;
unsigned int  speed =100;
int main( void )
{
    sim();
    
    //�����ⲿʱ��20M
    SCLK_24MInit();
    
    //��ʼ��led��
    LED_Init();  
    
    //��ʼ������
    UART_Init(115200);
    
    //���ò���ͨ��
   // TIM2_InitSet(2,50000);
    
    //��ʱ
    Delay_ms(2000);

    //�¹�����
    Bldc_GPIO_Init();
    
    //�Ϲ�����
    TIM1_PwmSet(1,1000);  //20k pwm ,������1000

    //�������
    bldc_start();
 
    //������������
    rim();
    
    Delay_ms(400);
    while(1)
    {
      if(speed<=650)
     {
        speed+=10;
     }   
     Delay_ms(100);
    }
}




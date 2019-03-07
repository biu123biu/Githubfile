#include "InitSet.h"
#include "Bldc_ctr.h"
#include "Tim.h"
#include "Bldc_SpeedCtr.h"
#include "stm8s.h"

unsigned char UART_Recbuf[10]; //串口接收缓存
unsigned char flag=0;    //bit8串口接收完成标志，
unsigned int  time=0;
unsigned int  speed =100;
int main( void )
{
    sim();
    
    //配置外部时钟20M
    SCLK_24MInit();
    
    //初始化led灯
    LED_Init();  
    
    //初始化串口
    UART_Init(115200);
    
    //配置捕获通道
   // TIM2_InitSet(2,50000);
    
    //延时
    Delay_ms(2000);

    //下管驱动
    Bldc_GPIO_Init();
    
    //上管驱动
    TIM1_PwmSet(1,1000);  //20k pwm ,计数器1000

    //启动电机
    bldc_start();
 
    //启动自主换相
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




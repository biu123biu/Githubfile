#include <stm8s.h>
#include <InitSet.h>



/***************************************************************************
* 函 数 名: delay_us
* 功能说明: 微秒延时程序， 注意此函数的运行环境为(16M 时钟速度)
* 形 参： nCount 要延时的微秒数， 输入 nCount=1 微妙
* 返 回 值: 无
***************************************************************************/
void Delay_us(u16 nCount) //16M 晶振时 延时 1 个微妙
{
      nCount*=3;//等同于 nCount=nCount*3 相当于把 nCount 变量扩大 3 倍
      while(nCount--);//nCount 变量数值先减一， 再判断 nCount 的数值是否大于 0， 大于 0 循环减一， 等于 0 退出循环。
}

/***************************************************************************
* 函 数 名: delay_ms
* 功能说明: 毫秒延时程序， 注意此函数的运行环境为(16M 时钟速度)
* 形 参： nCount 要延时的毫秒数， 输入 nCount=1 毫秒
* 返 回 值: 无
***************************************************************************/
void Delay_ms(u16 nCount) //16M 晶振时 延时 1 个毫秒
{
      while(nCount--)//先判断while()循环体里的 nCount 数值是否大于0，大于 0循环，减一执行循环体， 等于 0 退出循环。
      {
        Delay_us(1000);//调用微妙延时函数， 输入 1000 等译演示 1 毫秒。
      }
}

void CLK_Init(void)
{   
        CLK->SWR |= 0XE1;//主时钟采用内部高速时钟
        CLK->CKDIVR=0X00;//HSI不分频，CPU不分频          
}

void SCLK_24MInit(void)
{
   CLK->ECKR |= 0X01;                     //开启HSE
   while(!(CLK->ECKR&0X02));             //等待HSE准备就绪
   CLK->SWCR |= 0X02;                    //时钟切换开启
   CLK->SWR = 0XB4;                        //选择HSE为主时钟源
   while((CLK->SWCR&0X08)==0);             //等待切换成功
   CLK->SWCR &= 0xf7;                     //开启CSS前，要先清除SWBSY位;清除SWIF位
   CLK->CSSR |= 0x01;                     //开启时钟安全机制 
   while(CLK->CMSR!=0XB4);               //读取当前时钟
    CLK->CKDIVR=0x00; //CPU 时钟频率为主时钟源 8 分频  主频3MHz
}
/***************************************************************************
* 函 数 名: led_init
* 功能说明: led指示灯引脚初始化
* 形 参： 
* 返 回 值: 无
***************************************************************************/
void LED_Init()
{
  GPIOA->DDR &= ~0X08;//设置PD4为输出模式
  GPIOA->CR1 &=~ 0X08;//上拉输入
  
  
  GPIOD->DDR |= 0x10;//设置PD4为输出模式
  GPIOD->CR1 |= 0x10;//推完输出
  GPIOD->CR2 |= 0x10;//摆率设置为10M
  
  LED_OFF;
  Delay_ms(100);
  LED_ON;
  Delay_ms(100);
  LED_OFF;
  
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  
  
  while (1) ;
}
#endif
#include"beep.h"
//初始化PB8为输出口，并使能这个口的时钟
//蜂鸣器初始化
void BEEP_Init(void)//PD9
{
RCC->APB2ENR|=1<<5;//使能PORTB时钟
GPIOD->CRH&=0xffffff0f;
GPIOD->CRH|=0x00000030;//PD8推挽输出
BEEP=0;//关闭蜂鸣器输出

}

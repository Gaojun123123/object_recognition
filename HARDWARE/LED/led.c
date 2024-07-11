#include "led.h"
//初始化PB5和PE5为输出口，并使能这两个口的时钟
//LED I/O初始化
void LED_Init(void)
{
	RCC->APB2ENR|=1<<5;//使能PORTD时钟
	GPIOD->CRH&=0x000ff0ff;//清空这几个位的原来的设置
	GPIOD->CRH|=0x33300300;//三个管脚都选择为推挽方式输出

	GPIOD->ODR|=1<<10;//输出低  //报警灯
	
	GPIOD->ODR|=1<<13;//输出高
	GPIOD->ODR|=1<<14;//输出高
	GPIOD->ODR|=1<<15;//输出高	
}

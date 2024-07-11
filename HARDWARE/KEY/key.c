#include"key.h"
#include"delay.h"
//初始化按键函数
void KEY_Init(void)
{
RCC->APB2ENR|=1<<3;//使能PORTB时钟
RCC->APB2ENR|=1<<6;//使能PORTE时钟
RCC->APB2ENR|=1<<5;//使能PORTD时钟
GPIOB->CRH&=0xfff0ffff;
GPIOB->CRH|=0x00080000;
GPIOE->CRH&=0x00ffffff;//PE14,15设置成输入
GPIOE->CRH|=0x88000000;//

}
/*按键处理函数
返回按键值
mode：0，不支持连续按，1，支持连续按
1：没有任何按键按下   2 KEY1按下
3：KEY2按下    4 KEY4按下
注意此函数有优先级，KEY0>KEY1>KEY2>KEY3
*/
u8 KEY_Scan(u8 mode)
{
static u8 key_up=1;//按键松开标志
if(mode)
{
key_up=1;//支持连按
}
if((key_up)&&((KEY1==0)||(KEY2==0)||(KEY3==0)))
{
delay_ms(10);//去抖动
key_up=0;
if(KEY1==0)return 1;
else if(KEY2==0)return 2;
else if(KEY3==0)return 3;

}
else if((KEY1==1)&&(KEY2==1)&&(KEY3==1))key_up=1;//无按键按下
return 0;
}

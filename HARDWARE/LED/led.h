#ifndef __LED_H
#define __LED_H
#include"sys.h"
//LED端口定义

#define MYLED  PDout(10)
#define LED0 PDout(13)//LED0
#define LED1 PDout(14)//LED1
#define LED2 PDout(15)//LED2
void LED_Init(void);//初始化
#endif


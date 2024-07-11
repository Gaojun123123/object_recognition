#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#define Echo PAin(0)
#define Trig PAout(1)

//自己定义一个时间的结构体
struct MyTime{
u16 time_hour;
u16 time_minute;
u16 time_Second; 
u16 time_count;//该变量进入中断进行累加，在50HZ=20ms 要积累50次才将秒钟加1

u8 gy96_minsecond;//舵机转动最小时间
u8 gy96_maxsecond;//舵机转动最大时间
}  ;



void timer_Run(void);//运行自定义时钟
void TIM3_Int_Init(u16 arr,u16 psc);
void InitMytimer(void);
void TIM5_Cap_Init(u16 arr,u16 psc);
void openTimer(void)	;
void closeTimer(void)	;


#endif



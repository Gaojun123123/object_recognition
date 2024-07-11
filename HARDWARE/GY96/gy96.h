#ifndef __GY96_H
#define __GY96_H
#include "sys.h"

//定义一个舵机的结构体，存储舵机的模式，左右PWM波，PWM值波的限制

struct GY96
{
  u8 mode ;//舵机的模式
	u16 horizon_PWM;//水平方向舵机的PWM
	u16 victical_PWM;//垂直方向舵机的PWM

	u16 GY96_MAX_PWM;//舵机最大的PWM值
	u16 GY96_MIN_PWM;//舵机的最小PWM值
	
	u16 aim_PWM;//目标PWM
	
};

int  check_gy96(struct GY96 *motor);//返回值：1：参数正确  返回0：不合法
void InitGY96(struct GY96 * gy96motor);//初始化舵机的参数及设置PWM波
void setGY96PWM(struct GY96 *motor);//设置舵机的PWM参数
//通过改变TIM1->CCR1 4的值来改变占空比   PA8 PA11
void TIM1_PWM_Init_CH1_2(u16 arr,u16 psc);
void control_GY96_PWM(u8 num,struct GY96 *motor);//控制GY97的运动

#endif


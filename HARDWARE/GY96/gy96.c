#include "sys.h"
#include "gy96.h"
//舵机的控制

//struct GY96 gy96motor;//定义电机的结构体，封装电机所用的数据

//舵机定位函数，将舵机的位置在规定时间内到达指定的位置
//先左右
//在规定时间到达目标
//可以将舵机移动到自己设定的角度  (水平方向)
void gotosetAngle(u16 aim_PWM,u8 second,struct GY96 *motor)
{
     motor->horizon_PWM=aim_PWM; 
	
}


//每次设置舵机的参数值时需要进行合法性检验 是否在其合法范围内
//返回值：1：参数正确  返回0：不合法
int  check_gy96(struct GY96 *motor)
{
	//超过最大PWM
	if(motor->horizon_PWM>motor->GY96_MAX_PWM)	//当PWM超过最大限制范围时，截至，
	{
		motor->horizon_PWM=motor->GY96_MAX_PWM;//限制
    return 0;		
	}
	if(motor->victical_PWM>motor->GY96_MAX_PWM)  //垂直方向
	{
		motor->victical_PWM=motor->GY96_MAX_PWM;
		return 0;
	}
	//低于最小的PWM
	if(motor->horizon_PWM<motor->GY96_MIN_PWM) 
	{
    motor->horizon_PWM=motor->GY96_MIN_PWM;
		return 0;
	}
	if(motor->victical_PWM<motor->GY96_MIN_PWM) 
	{
	motor->victical_PWM=motor->GY96_MIN_PWM;
		return 0;
	}
		return 1;//符合要求返回1
}

//控制舵机的PWM值
//num:控制的功能 1：水平左移动  2：水平向右移动 3：垂直向上移动  4：垂直向下移动 motor :对应的一个舵机
void control_GY96_PWM(u8 num,struct GY96 *motor)
{
	switch(num)
	{
		case 1:motor->horizon_PWM=motor->horizon_PWM+10;break;
		case 2:motor->horizon_PWM=motor->horizon_PWM-10;break;
		case 3:motor->victical_PWM=motor->victical_PWM-10;break;
		case 4:motor->victical_PWM=motor->victical_PWM+10;break;
		
	}
	//将motor的值设置到PWM中
	setGY96PWM(motor);
}


//初始化舵机GY96，
void InitGY96(struct GY96 * gy96motor)
{ 
	gy96motor->mode=0;
	gy96motor->horizon_PWM=794;//初始值为中间值
	gy96motor->victical_PWM=838;//初始垂直方向的舵机PWM
	gy96motor->GY96_MAX_PWM=1000;
	gy96motor->GY96_MIN_PWM=500;

	gy96motor->aim_PWM=500;
	setGY96PWM(gy96motor);
}

//设置舵机的PWM,传入参数为舵机的结构体
void setGY96PWM(struct GY96 *motor)
{
   if(check_gy96(motor)==1)//只有在限制范围内才进行设置
	 {
		 	TIM1->CCR1=motor->horizon_PWM;//设置的下面控制水平方向的舵机
	  TIM1->CCR4=motor->victical_PWM;  //设置上面控制垂直方向的舵机
	 }


}



//只用来设置频率,通过改变TIM1->CCR1的值来改变占空比   PA8 PA11
void TIM1_PWM_Init_CH1_2(u16 arr,u16 psc)
{
	RCC->APB2ENR|=1<<11; //TIM1时钟使能
	RCC->APB2ENR|=1<<2;//使能POERA时钟
	GPIOA->CRH&=0xFFFF0FF0;// PA8清除之前的设置
	GPIOA->CRH|=0x0000b00b;//PA8复用功能输出
	TIM1->ARR=arr; //设置计数器自动重装值
	TIM1->PSC=psc; //预分频设置
	TIM1->CCMR1|=6<<4;//CH1 ,PWM2模式
	TIM1->CCMR2|=6<<12; //CH4 PWM2模式
	
	
	
	TIM1->CCMR1|=1<<3;//CH1预装载使能
	TIM1->CCMR2|=1<<11; //CH4预装载使能
	
	TIM1->CCER|=1<<0;//OC1输出使能
	TIM1->CCER|=1<<12;//OC4输出使能
	
	TIM1->BDTR|=1<<15;//MOE主输出使能
	TIM1->CR1=0x0080;//ARPE使能
	TIM1->CR1|=0x01;//使能定时器1
	
	
}


#ifndef __MYFUNCTION_H
#define __MYFUNCTION_H
#include "sys.h"

//本文件用于实现主函数需要的具体功能
extern u8  TIM5CH1_CAP_STA;                 //输入捕获状态
extern u16 TIM5CH1_CAP_VAL;                 //输入捕获值

extern float mesuresize;  //测量的尺寸
extern float danwei;  //计算的一个像素对于实际单位 (mm)
extern  int coords[4];//从摄像头中读取形状，坐标，像素尺寸

extern struct MyTime mytime;//自定义时钟

struct WorkMode  //程序运行的模式
{
 u8 current_fun;//当前的功能
 u8 first_flag;//是否是第一次进入该功能，好初始化时间
 u8 finish;//是否完成功能的标志
u8 find_aim;
} ;

void Run_function_1(struct WorkMode *mode);
void ModeInit(struct WorkMode *mode);
void myfun(float x,float *y);
double get_distance(void)	;
#endif


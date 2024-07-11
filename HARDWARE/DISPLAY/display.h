#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "sys.h"
#include "timer.h"
//只进行显示

extern  struct MyTime mytime;//自定义时钟
extern int coords[4];//从摄像头中读取形状，坐标，像素尺寸
extern double   distance;//测量得距离
extern float mesuresize; //测量的尺寸
extern u8 shape;//形状

      //  Circle            Squre        triangle
enum Shape{noshape=0,Squre=1,Circle=2,triangle=3};//形状  无，圆，正方形，三角形
enum YRow{Y_OntLine=0,Y_TwoLine=15,Y_ThirdLine=30,Y_FourLine=45} ;
void showMenu(void);
void showTimer(void);//显示时间
#endif


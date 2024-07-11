#ifndef __SERIAL_H
#define __SERIAL_H

#include "sys.h"


//记录圆、正方形、三角形的数目 
struct Shapenum
{
   u16 circle_num; 
	 u16 sqrt_num;
   u16 trangle_num;
};




void getmaxnum(int  * shape);
void recieveData(void);
 int max(int a,int b);
void  InitShape_Num(void)	;
void record_shapenum(int  * shape);
#endif

#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "serial.h"
#include "stdlib.h"

u16 t;
u16 len;
u16 times=0; //不用

extern int coords[4];

struct Shapenum shapenum;//定义变量记录各个形状出现的次数


//求最大值的函数
int max(int a,int b)
{
	return a>b? a:b;
}



//初始化各种形状的数目
void  InitShape_Num(void)	
{
 shapenum.circle_num=0;
	shapenum.sqrt_num=0;
	shapenum.trangle_num=0;
}


void getmaxnum(int  * shape)
{
	int  maxnum;
	 maxnum=max(shapenum.circle_num,shapenum.sqrt_num);//圆和正方形出现数目比较
  maxnum=max(maxnum,shapenum.trangle_num);//再将较大的与三角形出现次数比较
	  printf("maxnum=%d  圆 =%d  正方形=%d  三角形=%d\r\n",maxnum,shapenum.circle_num,shapenum.sqrt_num,shapenum.trangle_num); 
      
   	
	if(maxnum==shapenum.circle_num)
       *shape=2;   
	else if(maxnum==shapenum.sqrt_num)
		    *shape=1;
	else if(maxnum==shapenum.trangle_num)
		    *shape=3;
	else *shape=0;
}



//记录形状出现的次数
void record_shapenum(int  * shape)
{

	    
	switch(*shape) 
	{
		case 1: shapenum.sqrt_num++;  break;
		case 2: shapenum.circle_num++;  break;
		case 3: shapenum.trangle_num++;  break;
	}
	
}


void recieveData(void)
{
	u8 flag=0;
	char strX[4],strY[4],strR[4],strS[4];
	u8 cnt_x=0;
	u8 cnt_y=0;
	u8 cnt_R=0;//像素
	u8 cnt_S=0;//形状
	u8 *adress = NULL;
	 	if(USART_RX_STA&0x8000)		//如果接收完数据
		{	
			LED1=!LED1;//1号指示灯变更状态
			
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			//printf("%s\r\n",USART_RX_BUF);
			adress = &USART_RX_BUF[0];	//指针adress储存字符地址，从0-len过一遍
			// printf("%s\r\n",USART_RX_BUF) ;
			//根据协议取出坐标的字符形式到数strX和strY中
			for(t=0;t<len;t++)
			{
				if(*adress>='0' && *adress<='9')
				{
					if(flag==1)
					{
						strX[cnt_x] = *adress;
						cnt_x++;
					}
					else if(flag==2)
					{
						strY[cnt_S] = *adress;
						cnt_S++;
					}	
					else if(flag==3)
					{
						strS[cnt_y] = *adress;
						cnt_y++;
					}	
					else if(flag==4)
					{
						strR[cnt_R] = *adress;
						cnt_R++;
					}	
				}
				else
				{
				  
					if(*adress=='X')    //X方向
						flag = 1;
					if(*adress=='Y')     //y方向
						flag = 2;
					if(*adress=='S')
						flag = 3;         //当前球形状
					if(*adress=='R')
						flag = 4;         //得到的像素数
				}
				adress++;	
			}
			
			//转换字符串为整型，并存储到全局变量coords中
			coords[0] = atoi(strX);
			coords[1] = atoi(strY);
			coords[2]=atoi(strS) ; //形状
			coords[3]=atoi(strR) ; //像素
		 record_shapenum(&coords[2]) ;//记录形状次数
			  
//对形状进行取众数，
			
			
			//标志位清零
			USART_RX_STA=0;
		}
	else
		{
			times++;
			if(times%30==0)LED0=!LED0;//闪烁0号指示灯,提示系统正在运行.
			delay_ms(10); 
		}
	
}

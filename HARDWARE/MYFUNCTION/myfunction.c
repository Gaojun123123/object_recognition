#include "myfunction.h"
#include "timer.h"
#include "led.h"
#include "serial.h"
#include "beep.h"//使用蜂鸣器
#include "wave.h"

double   distance;//测量得距离

 //初始化标志位：当前的功能 ，完成标志，是否是第一次进行
void ModeInit(struct WorkMode *mode)
{
  mode->current_fun=0;
	mode->finish=0;
	mode->first_flag=1;
	mode->find_aim=0;//初始没找到目标
}







/********************
功能的实现函数
功能： 判别是否第一次进入功能，第一次的化要初始化时间，
**********************/
void Run_function_1(struct WorkMode *mode)
{
	   float temp;//赞成一次记录的数据
      printf("distance =%f shape = %d mesuresize=%f\r\n",distance, coords[2], mesuresize);
     
	  if(mode->first_flag==1)//判断是不是第一次进入功能1，
		{                     			//第一次进入功能1，初始化时间
		  InitMytimer();            //初始化时间
	     mode->finish=0;
			openTimer();							//打开定时器
			MYLED=0;												//关闭灯光
			mode->first_flag=0;														//下一次不会初始时间
			InitShape_Num();												//初始化形状数目
		} else{  																//不是第一次进入，进行1读取数据并进行处理
		  get_distance(); 															 //显示在OLED中
		  recieveData();
		   myfun(distance,&danwei);
       temp=danwei*coords[3];										//单位像素对应的长度乘上像素数
			       if(temp<300)temp=300;
         else if(temp>400)	temp=400; //过滤值	
			printf("temp=%f\r\n",temp);																					//判断距离尺寸是否在范围内
			if(temp>=300&&temp<=400) 									//测量的数据在正确范围内，加入 滑动滤波
			{
			 mesuresize=SlideWave1(temp);   
			}
			MYLED=~MYLED;
			
		}

		if(mytime.time_minute*60+ mytime.time_Second>=mytime.gy96_maxsecond)   //当时间等于最大时间(s)
		{ 
		  mode->current_fun=0;//退出
			mode->finish=1;//表示完成了
			mode->first_flag=1;//下次进入可更新时间
			getmaxnum(&coords[2]);//更新最终图形
			printf("最终结果是%d\r\n",coords[2]);
			closeTimer();//关闭定时器计数
			MYLED=1; //开启灯光
			BEEP=1;
			delay_ms(1000);
			BEEP=0;
		}
}

//返回一个像素对应的实际的物理长度 单位cm
void myfun(float x,float *y)
{
	 
     *y=0.0011*x+0.0172;
}

//获得距离
double get_distance(void)	
{
	  u32 temp=0;	
   		Trig = 1;                                  //给15微秒高电平触发                       
			delay_us(15);
			Trig = 0;
			delay_ms(10);                              //捕获回响信号的高电平时间
		if(TIM5CH1_CAP_STA&0x80)
			{
			//	LED0 = ~LED0 ;
				temp = TIM5CH1_CAP_STA&0x3f;             //捕获高电平溢出的次数
				temp *=65536;                            //溢出时间和
				temp += TIM5CH1_CAP_VAL; 
         distance = temp*0.17;				//得到高电平的时间和	
		 		distance = temp*0.17;
	    	//printf("distance: %f mm\r\n",distance);
       // printf("PWM_Val=%f\r\n",PWM_Val);
				TIM5CH1_CAP_STA = 0;                     //清零，为了下一次捕获
			}	
			return distance;
}


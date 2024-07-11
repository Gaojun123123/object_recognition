#include "wave.h" 
											//	修改滑动滤波数组 #define	10
#define Slidesize 5//滑动滤波
#define ModeAvesize 5//平均中值滤波

u8 number=0;

float Slidenum1[Slidesize];//滑动滤波数组1

float lastnum;//限幅滤波中的旧值
float dennum=0;//差值滤波中的就旧值
float ModeAvenum[ModeAvesize];//平均中值滤波数组
void WaveInit()//初始化数组
{
	u8 i=0;
	for(i=0;i<Slidesize;i++)
	{
		Slidenum1[i]=0;
	}
	for(i=0;i<ModeAvesize;i++)
	{
		ModeAvenum[i]=0;
	}
}
float SlideWave1(float x)//滑动滤波1
{
	u8 i=0;
	float t=0;
	for(i=0;i<Slidesize-1;i++)//数组中的数据后移
	{
		Slidenum1[Slidesize-(i+1)]=Slidenum1[Slidesize-(i+2)];
		t+=Slidenum1[Slidesize-(i+2)];
	}
	t=t+x;
	Slidenum1[0]=x;//数组的第一个位置
	return t/Slidesize;
}











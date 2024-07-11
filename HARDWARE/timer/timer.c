#include "timer.h"
#include "sys.h"
struct MyTime mytime;//自定义时钟
//计时部分

//打开定时器计数
void openTimer(void)	
{           
	  TIM3->CR1|=0x01;//最低位为1使能定时器
}

//打开定时器计数
void closeTimer(void)	
{            
	  TIM3->CR1&=0xfe; //最低位为0关闭定时器
}

void InitMytimer(void)
	{
     mytime.time_hour=0;
	   mytime.time_minute=0;
	   mytime.time_Second=0;
	   mytime.time_count=0;
	   mytime.gy96_maxsecond=30;//最多5s
		 mytime.gy96_minsecond=30;//最少3s
		 
}

                    



//时钟正常运行
void timer_Run(void)
{
	  mytime.time_count++;
	if(mytime.time_count>=100)  //累计50次为1s
	{
		mytime.time_count=0;//清零计数
		mytime.time_Second++;//秒钟数累加
		if(mytime.time_Second>=60)//当秒钟数大于60，进位
		 {
			 mytime.time_Second=0;
			 mytime.time_minute++;//分钟累加
			 
			 if(mytime.time_minute>=60)//60分钟1小时
			 {
				      mytime.time_minute=0;
				      mytime.time_hour++;                  
				      if(mytime.time_hour>=24)
							{
								mytime.time_hour=0;//小时最多24  全部清空
								mytime.time_minute=0;
								mytime.time_Second=0;
								mytime.time_count=0;
							}	
			 }
		 }
	}
}


//定时器3中断服务程序
void TIM3_IRQHandler(void)
{
		if(TIM3->SR&0x0001)//1个表示4位。他有16位，最低位是中断标志位
		{
			
			      timer_Run();		
         LED1=!LED1;//!是逻辑非运算，~是按位进行非运算      
		}
 TIM3->SR&=(0<<0);//清除该标致位  
}
//通用定时器3初始化
//这里的时钟选择为APB1的2倍，而APB1为36MHZ
//arr：自动重装值
//psc：时钟预分频数
//这里使用的是定时器3
void TIM3_Int_Init(u16 arr,u16 psc)
{
  RCC->APB1ENR|=1<<1;
 
  TIM3->ARR=arr;//设定计数器自动重装值
  TIM3->PSC=psc;
  TIM3->DIER|=1<<0;//允许中断更新
  TIM3->CR1|=0x01;//使能定时器3，看看0x0001可以不
  MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2

  
}


//7 :   0,没有成功的捕获;  1,成功捕获到一次.
//6 :   0,还没捕获到高电平;1,已经捕获到高电平了.
//5-0: 捕获高电平后溢出的次数
u8  TIM5CH1_CAP_STA=0;                 //输入捕获状态
u16 TIM5CH1_CAP_VAL;                   //输入捕获值

/********************************************************************************************************************************************************

函 数 名：void TIM5_Cap_Init(u16 arr,u16 psc) 
功    能：输入捕获
说    明：
入口参数：u16 arr,u16 psc
返 回 值：无

*********************************************************************************************************************************************************/
void TIM5_Cap_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<3;                //使能TIM5
	RCC->APB2ENR |= 1<<2;                //使能PA0 (Echo)
  GPIOA->CRL &= 0xffffff0f;		         //清零PORTA 
	GPIOA->CRL |= 3<<4;			             //PA1 Trig 通用推挽输出50MHz	
	GPIOA->CRL &= 0XFFFFFFF0;            //PA0          
	GPIOA->CRL |= 0X00000008;            //PA0设置为上拉/下拉输入
	GPIOA->ODR |= 0<<0;                  //PA0下拉
	TIM5->ARR |= arr;                    //自动重载值,从0开始计数,计到arr归零
	TIM5->PSC |= psc-1;                  //分频
	TIM5->CCMR1 |= 1<<0;                 //CC1通道被配置为输入,IC1映射在TI1上
	TIM5->CCMR1 &= ~(1<<2);              //无预分频器
	TIM5->CCMR1 &= ~(1<<4);              //无滤波器
	TIM5->CCER &= ~(1<<1);               //上升沿捕获
	TIM5->CCER |= 1<<0;                  //捕获使能
	TIM5->DIER |= 1<<0;                  //允许更新中断
	TIM5->DIER |= 1<<1;                  //允许捕获1中断
	TIM5->CR1 |= 0x01;                   //使能计时器
	MY_NVIC_Init(0,0,TIM5_IRQn,2);
}


/********************************************************************************************************************************************************

函 数 名：void TIM5_IRQHandler()
功    能：中断服务函数
说    明：
入口参数：无
返 回 值：无

*********************************************************************************************************************************************************/
void TIM5_IRQHandler()
{
	u16 tsr;

	tsr = TIM5->SR;
	if((TIM5CH1_CAP_STA&0x80)==0)          //未成功捕获
	{	
		if(tsr&0x01)                         //溢出
		{
			if(TIM5CH1_CAP_STA&0x40)           //捕获到高电平
			{
				if((TIM5CH1_CAP_STA&0x3f)==0x3f) //高电平时间太长
				{
					TIM5CH1_CAP_STA |= 0x80;       //标记成功捕获
					TIM5CH1_CAP_VAL = 0XFFFF;      
				}
				else
					TIM5CH1_CAP_STA++;
			}	
		}
		if(tsr&0x02)                         //捕获1发生捕获
		{
			if(TIM5CH1_CAP_STA&0x40)           //捕获到下降沿
			{
				TIM5CH1_CAP_STA |= 0x80;         //标志成功捕获,捕获到一次高电平脉宽
				TIM5CH1_CAP_VAL = TIM5->CCR1;    //读取当前捕获值
				TIM5->CCER &= ~(1<<1);           //设置为上升沿捕获
			}
			else                               //第一次捕获上升沿
			{
				TIM5CH1_CAP_STA = 0;             //清零
				TIM5CH1_CAP_VAL = 0;
				TIM5CH1_CAP_STA |= 0x40;         //标志成功捕获
				TIM5->CNT = 0;                   //计数值清空
				TIM5->CCER |= 1<<1;              //设置为下降沿捕获
			}
		}
	}
	TIM5->SR = 0;                          //清空中断标志
}






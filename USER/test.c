#include "sys.h"
#include "oled.h"
#include "usart3.h"
#include "gy96.h"
#include "myfunction.h"
#include "timer.h"
#include "display.h"
#include "serial.h"
#include "pid.h"
#include "wave.h"
//头文件的声明放到 sys.h钟
//定义1个主菜单和3个次级菜单
//程序只能使用两个按键
/********************
  形状的标志 coords【2】
  0：无
  1：正方形
  2： 圆
  3：三角形
  enum Shape{noshape=0,Squre=1,Circle=2,triangle=3};
 **********************/

PID_TypeDef PID_x,PID_y;//两个PID结构体PID_x和PID_y 
u16 targetX = 110;//当前x坐标
u16 targetY = 120;//当前y坐标


extern double   distance;//测量得距离

struct WorkMode  mywork;
struct GY96 gy96motor;//定义舵机的控制结构
int coords[4];//从摄像头中读取形状，坐标，像素尺寸


float mesuresize=0;  //测量的尺寸
float danwei=0;  //计算的一个像素对于实际单位 (mm)


void search(void);
void Run_function_2(struct WorkMode *mode);
int main()
{
    u8 t,pressed=0; //成功按下确定键 没

    Stm32_Clock_Init(9);
    delay_init(72);
    KEY_Init();
    LED_Init();//初始化
    BEEP_Init();//初始化蜂鸣器
    USART2_Init(36,9600);//串口2初始化为9600
    uart_init(72,9600);
    OLED_Init();//初始化OLED
    TIM5_Cap_Init(0xffff,72);                       //以 1MHz 的频率计数
    TIM1_PWM_Init_CH1_2(10000,144);//PA8,PA11控制舵机   72000 000 /50 =144 000 0/
    TIM3_Int_Init(7199,99);//定时器初始化 ：72000 000 /7200 100=100HZ
    closeTimer();//关闭计数，等按键按下开始
    InitGY96(&gy96motor);//舵机初始化
    InitShape_Num();
    WaveInit();                       
    ModeInit(&mywork);   //初始化工作模式
    MYLED=0;
    pid_init(0.04, 0, 0.0, &PID_x);   
    pid_init(0.04, 0, 0.0, &PID_y);
    coords[0]=188;
    coords[1]=120;
    showMenu();
    OLED_Refresh_Gram();
    while(1)                                                                                     
    {
        // OLED_Clear();
        showMenu();
        t=KEY_Scan(1);//得到按键值  
                      //根据按下的按键判断执行的功能
        if(t==1)pressed=1;  //按键标志
        else if(t==2)pressed=2;  

        switch (pressed)  //使用这种方式可以减少一层while循环
        {
            case 1:  
                Run_function_1(&mywork);
                if(mywork.finish==1)//完成标志为1
                {
                    pressed=0;//清除按键，不再进入
                }							 
                break;  //功能1与功能2
            case 2:
                printf("step1\r\n");
                Run_function_2(&mywork); 
                MYLED=~MYLED;
                if(mywork.finish==1)//完成标志为1
                {
                    printf("step2\r\n");
                    pressed=0;//清除按键，不再进入
                }
                MYLED	=1;					 
                break;	 //功能3与功能4

        }

        //1、获得测量的距离 

        //printf("PWMX=%d   PWM=%d\r\n",gy96motor.horizon_PWM,gy96motor.victical_PWM);
        //printf("x=%d y=%d S=%d  R=%d\r\n",coords[0],coords[1],coords[2],coords[3]);
        //根据现在选中的功能执行相应的功能函数
        //层级是2时才代表有功能执行
        //  LED0=!LED0; 
        //  delay_ms(100); 
        //显示按下的按键值
        OLED_Refresh_Gram();
        delay_ms(50);
    }
}




void Run_function_2(struct WorkMode *mode)
{
    float temp=0;
    printf("step3\r\n");
    if(mode->first_flag==1)//判断是不是第一次进入功能1，
    {     
        //第一次进入功能1，初始化时间
        InitMytimer();            //初始化时间
        mode->finish=0;
        openTimer();							//打开定时器
        MYLED=0;												//关闭灯光
        mode->first_flag=0;														//下一次不会初始时间
        InitShape_Num();												//初始化形状数目
        mode->find_aim=0;//初始没找到目标
        printf("step4\r\n");
    }
    else{
        printf("step5\r\n");
        printf("find=%d\r\n",mode->find_aim);
        //是否找到目标的标志位
        if(mode->find_aim==0) //没有找到的话，进行搜索，并读取数据，并显示菜单
        {
            printf("step6\r\n");
            search(); //边搜索边读取
            if(coords[2]!=0)//找到目标位置
                mode->find_aim=1;	
        }  //找到目标后，靠近,pid
        else{
            recieveData();
            printf("测试pid=%d\r\n",coords[0]);
            gy96motor.horizon_PWM=gy96motor.horizon_PWM+pid(coords[0],targetX,&PID_x);

            setGY96PWM(&gy96motor);
            delay_ms(10);

        }


    }
    //当到达指定时间，并且误差小，停止舵机，发出生光信号
    if(mytime.time_Second>30&&mytime.time_Second<50)  //舵机不动，采集形状尺寸，距离    
    {

        get_distance(); 															 //显示在OLED中
        recieveData();
        myfun(distance,&danwei);
        temp=danwei*coords[3];										//单位像素对应的长度乘上像素数
        printf("temp=%f\r\n",temp);																					//判断距离尺寸是否在范围内
                                                                                                                    //限界范围
        if(temp<300)temp=300;
        else if(temp>400)	temp=400;

        if(temp>=300&&temp<=400) 									//测量的数据在正确范围内，加入 滑动滤波
        {
            mesuresize=SlideWave1(temp);   
        }

    } else if(mytime.time_Second>50)
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



//进行 扫描，发现目标便进行停止，使用pid靠近
void search(void)
{
    u16 pwmvalue=794;
    if(coords[2]!=0)
        return ;
    for(pwmvalue=794;pwmvalue>=694;pwmvalue--)     //579
    {
        gy96motor.horizon_PWM=pwmvalue;
        setGY96PWM(&gy96motor);
        recieveData();
        delay_ms(15);
        if(coords[2]!=0)   //有形状
            return ;
    }
    for(pwmvalue=694;pwmvalue<=894;pwmvalue++)
    {
        gy96motor.horizon_PWM=pwmvalue;
        setGY96PWM(&gy96motor);
        recieveData();
        delay_ms(15);
        if(coords[2]!=0)
            return ;
    }
    for(pwmvalue=894;pwmvalue>=794;pwmvalue--)
    {
        gy96motor.horizon_PWM=pwmvalue;
        setGY96PWM(&gy96motor);
        recieveData();
        delay_ms(15);
        if(coords[2]!=0)
            return ;
    }

}




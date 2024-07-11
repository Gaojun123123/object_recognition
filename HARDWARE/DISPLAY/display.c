#include "display.h"
#include "oled.h"


//显示时间
void showTimer(void)
{
	  dispchinese(0,Y_FourLine,31,2,12,12); //计时
	 //显示当前的时间：
	   OLED_ShowNum(40,Y_FourLine,mytime.time_hour,2,12);             
	   OLED_ShowString(52,Y_FourLine,":",12);
	   OLED_ShowNum(62,Y_FourLine,mytime.time_minute,2,12); 
	   OLED_ShowString(74,Y_FourLine,":",12);
		 OLED_ShowNum(84,Y_FourLine,mytime.time_Second,2,12); 
}


//显示主界面：一键自动进行操作

 void showShape(enum Shape shape) 
 {
	 switch(shape)
	 {
		 case noshape :      //为了保证OLED显示正常，显示长度都设成相同的
			           
                  //dispchinese(60,Y_TwoLine,23,1,12,12); //无
		              //dispchinese(72,Y_TwoLine,33,4,12,12); //2个空格
              		 break;  //显示空
		 case Circle: 
			            
			              dispchinese(60,Y_TwoLine,24,1,12,12); //无
		                dispchinese(72,Y_TwoLine,33,4,12,12); //2个空格
		                break;       //显示圆
		 case Squre:
			            
			              dispchinese(60,Y_TwoLine,25,3,12,12); //无
		                break;         //显示正方形
		 case triangle:
			           
			            dispchinese(60,Y_TwoLine,28,3,12,12); //无   
		               break;      //显示三角形
	 }
	 
 }

void show_distance(void)
{
	 dispchinese(40,Y_OntLine,12,2,12,12); //功能1
	 
}

  //显示主菜单
void showMenu(void)
{ 
	   OLED_ShowString(0,Y_OntLine,"1.",12);
	   OLED_ShowString(0,Y_TwoLine,"2.",12);
	   OLED_ShowString(0,Y_ThirdLine,"3.",12);
	
      dispchinese(20,Y_OntLine,12,2,12,12); //距离
	    OLED_FloatShow(60,Y_OntLine,distance,12);
	    OLED_FloatShow(60,Y_ThirdLine,mesuresize,12);//显示测量的尺寸
	    dispchinese(20,Y_TwoLine,17,2,12,12); //形状
	
	   //根据图形的形状，选择显示直径还是边长
	
	    if(coords[2]==Circle)//1表示形状 是圆  现在直径两字
			{
			   dispchinese(20,Y_ThirdLine,21,2,12,12); //直径
			}
			else   //非表示形状 不是圆 显示边长
			{
				 dispchinese(20,Y_ThirdLine,19,2,12,12); //边长
			}
			
			showShape(coords[2]);    //显示形状
			 
			showTimer();//显示时间
	  // OLED_ShowString(40,Y_OntLine,"4、",12);
}


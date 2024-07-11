#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//本次实验使用的SPI通信：
//CS片选信号 --PD3
//DC      --PD5
//RES     --PD4
//D1      --PD7  SDIN
//D0      --PD6  SCLK

////////////////////////////////////////////////////////////////////////////////// 


//OLED模式设置
//0: 4线SPI串行模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
#define OLED_MODE 	0 
		    						  
//---------------------------OLED端口定义--------------------------  					   
#define OLED_CS  PDout(3)          //cs
#define OLED_RST PDout(4) 	       //
#define OLED_RS  PDout(5)
#define OLED_WR  PGout(14)		  
#define OLED_RD  PGout(13	   
//PC0~7,作为数据线
#define DATAOUT(x) GPIOC)->ODR=(GPIOC->ODR&0xff00)|(x&0x00FF); //输出
  
//使用4线串行接口时使用 
#define OLED_SCLK PDout(6)
#define OLED_SDIN PDout(7)
		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_FloatShow (u16 x,u16 y,float num,u8 size);
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
void OLED_ShowChinese2424(u16 x,u16 y,u8 index,u8 size);   //显示汉字
 void dispchinese(u16 x,u16 y,u8 startindex,u8 len,u8 size,u8 width);
#endif  
	 




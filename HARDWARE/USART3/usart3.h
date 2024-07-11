#ifndef __USART3_H
#define __USART3_H
#include "sys.h"

#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
extern u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART3_RX_STA;       //接收状态标记	 
void uart3_init(u32 pclk2,u32 bound);//串口3  PB10: TX PB11:RX


#endif

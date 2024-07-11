 #include "usart3.h"
#include "sys.h"
#include "led.h"
 /***************************************************************************************************************************
功能：串口3中断服务程序  

注意：读取USARTx->SR能避免莫名其妙的错误   
****************************************************************************************************************************/


                          
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA=0;       //接收状态标记	  
void USART3_IRQHandler(void)
{
	u8  res;	

	LED2=!LED2;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif	 
   if(USART3->SR&(1<<5))	//USART3接收到数据
   {	
	
     res=USART3->DR;	       //接受缓冲寄存器数据
	//	  USART2->DR=res   ;
		// len= USART3_RX_STA&0x3fff;//获得长度			
			if((USART3_RX_STA&0x8000)==0)//接收未完成
			{
				if(res==0x55) //帧尾
				{
					USART3_RX_STA|=0x4000;
					USART3_RX_BUF[USART3_RX_STA&0X3F]=res ;
					if(USART3_RX_STA&0x4000)//接收到了0x55
					{
						if(USART3_RX_BUF[0]==0xaa)
						{
							USART3_RX_STA|=0x8000;	//接收完成了 
						  
						}	
						else
							USART3_RX_STA=0;//接收错误,重新开始
					}	
				}
				else    //没接受到帧尾，继续接受数据
				{
					  USART3_RX_BUF[USART3_RX_STA&0X3Fff]=res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))
						USART3_RX_STA=0;//接收数据错误,重新开始接收
				}
			  	   
		}
	#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
		OSIntExit();  											 
	#endif
	} 


}

/***************************************************************************************************************************
***函数功能：初始化串口 3
***参	 数：pclk1:PCLK1时钟频率(Mhz)  bound:波特率
***使用引脚：11：TX/PD8 RX/PD9  （00:TX/PB10 RX/PB11）
***注意事项：uart3_init(36,115200);   36倍频 (只能使用 36 倍频) 
****************************************************************************************************************************/
void uart3_init(u32 pclk2,u32 bound2)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	
	temp=(float)(pclk2*1000000)/(bound2*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction;
	RCC->APB2ENR|=1<<0;    //映射时钟打开:辅助功能IO时钟使能
	AFIO->MAPR|=3<<4;      //USART3
	//没有重映像00:（TX/PB10 RX/PB11）（位5:4）
   //（部分重映像01：TX/PC10 RX/PC11 CK/PC12 CTS/PB13 RTS/PB14）完全映像11：TX/PD8 RX/PD9
    RCC->APB2ENR|=1<<5;    //使能PORTD口时钟  TX/PD8 RX/PD9
    GPIOD->CRH&=0XFFFFFF00;//IO状态设置
	GPIOD->CRH|=0X0000008B;//IO状态设置
	RCC->APB1ENR|=1<<18; //使能串口3时钟
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//复位完毕   	   
/*波特率设置*/
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	USART3->CR2|=0<<12;  //串口使能 接收发送使能 一位停止 无校验位----------------------------------------					  
    USART3->CR1|=1<<3; //发送使能*
    USART3->CR1|=1<<13;//usart使能
   	
#if EN_USART3_RX		//如果使能了接收
    USART3->CR1|=1<<2;  //使能接收*
    USART3->CR1|=1<<5;  //接收缓冲区非空中断使能   当USART_SR中的ORE或者RXNE位为1时，将产生串口中断。
	USART3->CR1|=1<<8;  //PE中断使能
	MY_NVIC_Init(2,1,USART3_IRQn,3);//组2
#endif
}



#ifndef __KEY_H
#define __KEY_H
 #include "sys.h"
#define KEY3 PBin(12)//PB12
#define KEY1 PEin(14)//PE14
#define KEY2 PEin(15)//PE15

#define KEY_LEF 3
#define KEY_DOWN 2
#define KEY_RIGHT 1
void KEY_Init(void);//IO 初始化
u8 KEY_Scan(u8 mode);//按键扫描函数
#endif


#ifndef __MYSTRING_H
#define __MYSTRING_H
#include "sys.h"
//LED端口定义
#define MAXSIZE 100

//定义一个字符串的结构 
typedef struct
{
	char string[MAXSIZE];
	int len;
}String; 
int str_len(String *s) ;/*  串的长度计算函数 */ 
int ValueString(char *pstr,String *str,int num);//给一个Sting赋值,指针主易指向位置，不能出现野指针 
int strIndex(String strs,char str);//定位函数
int  subString(String str,String *sub,int begin,int end) ;////剪切字符串 ,需要子串，主串，起始，结束位置 
int  get_by_char(String str,String *childstr,char beginchar ,char endchar);//通过起始和结束字符剪切 


	
#endif

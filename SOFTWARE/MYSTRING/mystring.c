#include<stdio.h>
#include<string.h>
#include "mystring.h"
#include "usart.h"
#include "stdlib.h"
/* 
程序任务：
解析通过串口发送过来的数据
格式为 ：Xnum Ynum Znum 

思路来源：java的String类中的提取字符串功能的底层实现 
需要定位、 剪切函数 
*/ 


/*  串的长度计算函数 */ 
 int str_len(String *s)      
 {
 	return s->len;     
  } 
//给一个Sting赋值,指针主易指向位置，不能出现野指针 
int ValueString(char *pstr,String *str,int num)
{
	int i=0;
	
	//长度合法性验证
	if(num>MAXSIZE)
	{
		return -1;
	}
	 
	for(i=0;i<num;i++)
	{
	//	s1=(*pstr);
  	str->string[i]= (*pstr);//进行赋值 
		pstr++;//指针后移 
	}
	str->string[i]='\0'; 
	//字符串最后加上字符串的结束符 
	str->len=num;
	return 1;	
}
  

  //定位函数 
int strIndex(String strs,char str)
{
	int i=0;//循环去比较找str的位置
	for(i=0;i<strs.len;i++)
	{
	if(strs.string[i]==str)
	 {
	 	
		return i;
    	}
	 }
	 return -1;//表示没找到 
	
 } 
//剪切字符串 ,需要子串，主串，起始，结束位置 
int   subString(String str,String *sub,int begin,int end) 
{
	int length=end-begin;//得到字串的长度
	int i;//赋值变量 
	//判断合法性 
	 if(begin<0||begin>str.len){
	 	return -1;
	 } 
	 if(end<0||end>str.len){
	 	return -1;
	 } 
	 //从起始位置复制 
	 for(i=0;i<length;i++){
	 	(*sub).string[i]=str.string[i+begin];
	 } 
		(*sub).string[i]='\0';
	 return -1;
	
}





/*
 * 函数名:  String get_by_char
 * 简介:  通过起始和结束字符剪切 
 * 参数:   str:主串   
 *         child ：剪切之后的字串，注意指针的初始化 
 *         beginchar：起始字符
 *         endchar：结束字符 
 * 返回值: 函数的执行状态 
 * 功能:初始化系统时钟
 */
 
int  get_by_char(String str,String *childstr,char beginchar ,char endchar)
 {
 	int num=0; //最终得到的数字 
 	//定位字符的位置 
 	int position1= strIndex(str,beginchar); 
 	int position2= strIndex(str,endchar);
 	
 	//根据已知的位置剪切字符串 
 //	String temp;
	 subString(str,childstr,position1+1,position2);
	// printf("childstr=%s\r\n",childstr->string); 
	 num=atoi(childstr->string);
	 //printf("num=%d\n",num);
	 return num;
	 
 } 


 
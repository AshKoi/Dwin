
/**********************************************************************
*
*                  监控系统8.0__嵌入式系统构件
*
**********************************************************************

----------------------------------------------------------------------
文 件 名	: emac_api.c
文件描述	: emac模块应用
创建作者	: HCF
创建日期	: 2007.11.28
======================================================================
版本历史	:
----------------------------------------------------------------------
修改日期	:
修改内容	:
======================================================================
*/
#include "lwip/others/user/sys/sys_data.h"
#include "lwip/others/user/sys/sys_general.h"

#define  WEB_ERR_OK          0	       //没有错误
#define  WEB_ERR_NULL		 -1		   //进入的指针为空
#define  WEB_ERR_CK 		 -2		   //
#define  WEB_ERR_TO          -3		   //超时退出
#define  WEB_ERR_


#define  STR_EQ				0
#define  STR_NO_EQ			1
#define  STR_ERR			2
extern unsigned char TX_EMAC_MASG_BUF[5120];
char web_strcmp(char *str,const unsigned char *p )
{  unsigned char   i;
   	if(str==NULL)
	{
		 return STR_ERR;
	}
    for(i=0;*(p+i)!='\0';i++)
	{
		if(*(str+i) != *(p+i))
		{
		   return STR_ERR;
		}
	
	}
   return STR_EQ;
}
/*
**********************************************************************
*	函数名称:	string_cmp
*	函数功能:	数据比较函数
*	入口参数:	1、*str		:
*				2、*p 		:	
*	出口参数:	NULL     不等跟指针错误    还回str+i是比较之后的指针指向的位置
**********************************************************************
*/
char * string_cmp(char *str,const unsigned char *p )
{  unsigned char   i;
   	if(str==NULL)
	{
		 return NULL;
	}
    for(i=0;*(p+i)!='\0';i++)
	{
		if(*(str+i) != *(p+i))
		{
		   return NULL;
		}
	
	}
   return str+i;
}
/*
**********************************************************************
*	函数名称:	Web_Find_Str_Post
*	函数功能:	查找POST数据包  内数据所在位置
*	入口参数:	1、*str		:
*	出口参数:	NULL     不等跟指针错误    还回str+cnt+4是找到之后的指针指向的位置
**********************************************************************
*/
char *Web_Find_Str_Post(char *str)
{
	unsigned int str_len,cnt;
	if(str==NULL)
	{
		 return NULL;
	}
   	str_len = sys_general_strlen((const unsigned char*)str);
   	for(cnt=0; cnt<str_len; cnt++)
	{
		if((*(str+cnt) == 0x0D)&&(*(str+cnt+1) == 0x0a))
		{
		    if((*(str+cnt+2) == 0x0d)&&(*(str+cnt+3) == 0x0a))
			{
			  return str+cnt+4;
			}
		}
		else if((*(str+cnt+1)=='%')&&(*(str+cnt+2)=='2')&&(*(str+3)=='3'))
		{
		
			  return str+cnt+1;
		}
	}
   return  NULL;
}
/*
**********************************************************************
*	函数名称:	str_cope
*	函数功能:	实现数据复制功能
*	入口参数:	1、*str		:
*	出口参数:	NULL     不等跟指针错误    还回str+cnt+4是找到之后的指针指向的位置
**********************************************************************							
*/
unsigned char *str_cope(unsigned char *drc,unsigned char *src)
{
	while( *src !='\0' )
	{ *(drc++) = *(src++);  }
	return drc;
}
unsigned char *strcp(unsigned char *drc,unsigned char *src,unsigned int len)
{
   unsigned int i;

   for(i=0;i < len ;i++)
   {
   	  *(drc+i) = *(src+i);

   }
	return drc;
}
/*
**********************************************************************
*	函数名称:	Web_Combined_row
*	函数功能:	实现组包功能
*	入口参数:	1、*str		:
*	出口参数:	NULL     不等跟指针错误    还回str+cnt+4是找到之后的指针指向的位置
**********************************************************************
*/
unsigned char *Web_Combined_row(unsigned char *str,unsigned char strut,unsigned char num,unsigned char *id_1, unsigned char *id_2,
							unsigned char *name,unsigned char *dev_msg,
							unsigned char *up_time)
{
	unsigned char *p;unsigned char buf[5];
//	unsigned char i,j;
	unsigned char *num_buf;
	num_buf=buf;
	*num_buf=num+0x30;
	p=str;
	
	if(name==NULL)
	{
		 return 0;
	}
	//列信息
	p=str_cope(p,"<tr class=\"STYLE1\">");
	//首列	   序号
    p=str_cope(p,"<th scope=\"row\"><span class=\"STYLE1\">");
	if( 0 < num < 10 )
    {*(p++)=num+0x30; }
	else if( (num >= 10)&&(num<100) )
	{
	   *(p++)=num/10+0x30;
	   *(p++)=num%10+0x30;
	}
	p=str_cope(p,"</span></th>");
	//第二列   控件
   	p=str_cope(p,"<td>");
	p=str_cope(p,"<div align=\"center\">");
	p=str_cope(p,"<input type=\"checkbox\" name=\"#web_");
    p=str_cope(p,id_1);
	p=str_cope(p,"<\" id=\"#web_");
	p=str_cope(p,id_1);
	p=str_cope(p,"\" /> ");
	p=str_cope(p,"</div>");
	p=str_cope(p,"</td>");
	//第三列   参数名称
	p=str_cope(p,"<td>");
	p=str_cope(p,name);
	p=str_cope(p,"</td>");
   //第四列    本地信息
   p=str_cope(p,"<td><input name=\"text");
    p=str_cope(p,id_2);
   p=str_cope(p,"\" type=\"text\" class=\"STYLE3\" ");


   p=str_cope(p,"\" id=\"#web_");
   p=str_cope(p,id_2);

   p=str_cope(p,"\" />");
   p=str_cope(p,"</td>");
   //第五列     远程信息
   p=str_cope(p,"<td><input name=\"text");
    p=str_cope(p,id_2);
   p=str_cope(p,"\" type=\"text\" class=\"STYLE3\" ");


   p=str_cope(p,"\" id=\"#web_");
   p=str_cope(p,id_2);

   p=str_cope(p,"\" />");
   p=str_cope(p,"</td>");
  //第六列     更新时间
   p=str_cope(p,"<td><input name=\"text");
   p=str_cope(p,id_2);
   p=str_cope(p,"\" type=\"text\" class=\"STYLE3\" ");


   p=str_cope(p,"\" id=\"#web_");
   p=str_cope(p,id_2);

   p=str_cope(p,"\" />");
   p=str_cope(p,"</td>");

   //一行结束符
   p=str_cope(p,"<!tr>");


	return p;
}

char Web_User_Login_Check(unsigned char *str)
{


	return 0;
}

/*
**********************************************************************
*	函数名称:	Web_Post_Cope
*	函数功能:	取出真正的数据
*	入口参数:	1、*str		:
*	出口参数:	NULL     不等跟指针错误    还回str+cnt+4是找到之后的指针指向的位置
**********************************************************************
*/
#include "lwip/utility/trace.h"
#include "lwip/utility/assert.h"
char *Web_Post_Cope	(char *s,char *id,char *date,unsigned int *id_len,unsigned int *da_len)
{
	unsigned int tc;
	unsigned int i;
	for(i=0;i<*da_len;i++,s++)
	{
		if((*s=='%')&&(*(s+1)=='2')&&(*(s+2)=='3'))
		{	s=s+3;
			for(tc=0;*s!='=';s++,tc++)
			{
				  if(tc > 0xfff)
				  {
				  		s=NULL;
			   			return s;
				  }
				
				  *(id+tc)=*s;	
				  TRACE_ERROR("Web_Post_Cope1\n");
			}
			*id_len=tc;
			s++;
			for(tc=15;((*s != '&') &&(*s != '\0'));s++,tc--)
			{
				  if(tc == 0 )
				  {				
						s=NULL;
			   			return s;
				  }
				  *(date+tc)=*s;
				  TRACE_ERROR("Web_Post_Cope2\n");
	
			}
			 if(*s == '&')
			  {	 return s+1;   }
		     *da_len=tc;
			 if( *(s) == '\0' )
			 {
			 	s=NULL;
			   	return s;
			 }
			
		}
	

	}
	s=NULL;
	return s;

}



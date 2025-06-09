
/**********************************************************************
*
*                  ���ϵͳ8.0__Ƕ��ʽϵͳ����
*
**********************************************************************

----------------------------------------------------------------------
�� �� ��	: emac_api.c
�ļ�����	: emacģ��Ӧ��
��������	: HCF
��������	: 2007.11.28
======================================================================
�汾��ʷ	:
----------------------------------------------------------------------
�޸�����	:
�޸�����	:
======================================================================
*/
#include "lwip/others/user/sys/sys_data.h"
#include "lwip/others/user/sys/sys_general.h"

#define  WEB_ERR_OK          0	       //û�д���
#define  WEB_ERR_NULL		 -1		   //�����ָ��Ϊ��
#define  WEB_ERR_CK 		 -2		   //
#define  WEB_ERR_TO          -3		   //��ʱ�˳�
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
*	��������:	string_cmp
*	��������:	���ݱȽϺ���
*	��ڲ���:	1��*str		:
*				2��*p 		:	
*	���ڲ���:	NULL     ���ȸ�ָ�����    ����str+i�ǱȽ�֮���ָ��ָ���λ��
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
*	��������:	Web_Find_Str_Post
*	��������:	����POST���ݰ�  ����������λ��
*	��ڲ���:	1��*str		:
*	���ڲ���:	NULL     ���ȸ�ָ�����    ����str+cnt+4���ҵ�֮���ָ��ָ���λ��
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
*	��������:	str_cope
*	��������:	ʵ�����ݸ��ƹ���
*	��ڲ���:	1��*str		:
*	���ڲ���:	NULL     ���ȸ�ָ�����    ����str+cnt+4���ҵ�֮���ָ��ָ���λ��
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
*	��������:	Web_Combined_row
*	��������:	ʵ���������
*	��ڲ���:	1��*str		:
*	���ڲ���:	NULL     ���ȸ�ָ�����    ����str+cnt+4���ҵ�֮���ָ��ָ���λ��
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
	//����Ϣ
	p=str_cope(p,"<tr class=\"STYLE1\">");
	//����	   ���
    p=str_cope(p,"<th scope=\"row\"><span class=\"STYLE1\">");
	if( 0 < num < 10 )
    {*(p++)=num+0x30; }
	else if( (num >= 10)&&(num<100) )
	{
	   *(p++)=num/10+0x30;
	   *(p++)=num%10+0x30;
	}
	p=str_cope(p,"</span></th>");
	//�ڶ���   �ؼ�
   	p=str_cope(p,"<td>");
	p=str_cope(p,"<div align=\"center\">");
	p=str_cope(p,"<input type=\"checkbox\" name=\"#web_");
    p=str_cope(p,id_1);
	p=str_cope(p,"<\" id=\"#web_");
	p=str_cope(p,id_1);
	p=str_cope(p,"\" /> ");
	p=str_cope(p,"</div>");
	p=str_cope(p,"</td>");
	//������   ��������
	p=str_cope(p,"<td>");
	p=str_cope(p,name);
	p=str_cope(p,"</td>");
   //������    ������Ϣ
   p=str_cope(p,"<td><input name=\"text");
    p=str_cope(p,id_2);
   p=str_cope(p,"\" type=\"text\" class=\"STYLE3\" ");


   p=str_cope(p,"\" id=\"#web_");
   p=str_cope(p,id_2);

   p=str_cope(p,"\" />");
   p=str_cope(p,"</td>");
   //������     Զ����Ϣ
   p=str_cope(p,"<td><input name=\"text");
    p=str_cope(p,id_2);
   p=str_cope(p,"\" type=\"text\" class=\"STYLE3\" ");


   p=str_cope(p,"\" id=\"#web_");
   p=str_cope(p,id_2);

   p=str_cope(p,"\" />");
   p=str_cope(p,"</td>");
  //������     ����ʱ��
   p=str_cope(p,"<td><input name=\"text");
   p=str_cope(p,id_2);
   p=str_cope(p,"\" type=\"text\" class=\"STYLE3\" ");


   p=str_cope(p,"\" id=\"#web_");
   p=str_cope(p,id_2);

   p=str_cope(p,"\" />");
   p=str_cope(p,"</td>");

   //һ�н�����
   p=str_cope(p,"<!tr>");


	return p;
}

char Web_User_Login_Check(unsigned char *str)
{


	return 0;
}

/*
**********************************************************************
*	��������:	Web_Post_Cope
*	��������:	ȡ������������
*	��ڲ���:	1��*str		:
*	���ڲ���:	NULL     ���ȸ�ָ�����    ����str+cnt+4���ҵ�֮���ָ��ָ���λ��
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



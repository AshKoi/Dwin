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
#include "lwip/web/exec/web.h"
#include "lwip/web/exec/web_str.h"

#include "lwip/web/http/http_user.h"
#include "lwip/web/http/http_sys.h"
#include "lwip/web/http/http_main.h"
#include "lwip/web/http/http_alarm.h"
#include "lwip/web/http/http_set_msg.h"
#include "lwip/web/http/http_sup_user.h"
#include "lwip/web/http/css.h"
#include "lwip/web/http/java.h"
#include "lwip/web/http/xml.h"
#include "lwip/others/user/sys/sys_data.h"
#include "lwip/others/user/sys/sys_general.h"
#include "lwip/web/http/http_head.h"

#define DEBUG_MSG
 /*
**********************************************************************
*	函数名称:	Web_Get_Css_deal
*	函数功能:	收到css命令处理函数
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char html_date_zu_he()
{
  unsigned char *p;
  p=TX_EMAC_MASG_BUF;
  p=str_cope(p,Web_SYS_MSAGE_TABLE_HARD);

  p=Web_Combined_row(p,1,1,"0001", "0002","宸ヤ綔淇￠亾1","123","45678");
  p=Web_Combined_row(p,1,2,"0003", "0004","宸ヤ綔淇￠亾2","123","45678");
  p=Web_Combined_row(p,1,3,"0005", "0006","宸ヤ綔淇￠亾3","123","45678");
  p=Web_Combined_row(p,1,4,"0007", "0008","宸ヤ綔淇￠亾4","123","45678");
  p=Web_Combined_row(p,1,5,"0009", "000a","宸ヤ綔淇￠亾5","123","45678");
  p=Web_Combined_row(p,1,6,"000b", "000c","宸ヤ綔淇￠亾6","123","45678");
  p=Web_Combined_row(p,2,7,"000d", "000e","name2","123","123");
  p=Web_Combined_row(p,3,8,"000f", "0010","name3","789","222");
  p=Web_Combined_row(p,3,9,"000f", "0010","name3","789","222");
  p=Web_Combined_row(p,3,10,"000f", "0010","name3","789","222");
  p=Web_Combined_row(p,3,11,"000f", "0010","name3","789","222");
  p=Web_Combined_row(p,3,12,"000f", "0010","name3","789","222");
  p=Web_Combined_row(p,3,13,"000f", "0010","name3","789","222");
  p=str_cope(p,Web_SYS_MSAGE_TABLE_LAST);
  return 0;
}
/*
**********************************************************************
*	函数名称:	Web_Get_Css_deal
*	函数功能:	收到css命令处理函数
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char Web_Get_Css_deal(char *p,struct netconn *conn)
{  	
    unsigned char err=0;

   	if (string_cmp(p,Http_Address_CSS_1)  !=  NULL)
	{
	    netconn_write(conn, faile_CSS_1, sizeof(faile_CSS_1), NETCONN_COPY);
		err=1;
	}
	else if (string_cmp(p,Http_Address_CSS_2)  !=  NULL)
	{
	    netconn_write(conn, faile_CSS_2, sizeof(faile_CSS_2), NETCONN_COPY);
		err=1;
	}
	else if (string_cmp(p,Http_Address_CSS_3)  !=  NULL)
	{
	    netconn_write(conn, faile_CSS_3, sizeof(faile_CSS_3), NETCONN_COPY);
	 	err=1;	
	}
	else
	{
		err=0;
	}
    return err;
}


/*
**********************************************************************
*	函数名称:	Web_Get_Java_deal
*	函数功能:	收到java命令处理函数
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char Web_Get_Java_deal(char *p,struct netconn *conn)
{
	unsigned char err=0;

   	if (string_cmp(p,Http_Address_Java_1)  !=  NULL)
	{
	    netconn_write(conn, Java_file_1, sizeof(Java_file_1), NETCONN_COPY);
		err=1;
	}
	else if (string_cmp(p,Http_Address_Java_2)  !=  NULL)
	{
	    netconn_write(conn, Java_file_2, sizeof(Java_file_2), NETCONN_COPY);
		err=1;
	}
	else if (string_cmp(p,Http_Address_Java_3)  !=  NULL)
	{
	    netconn_write(conn, Java_file_3, sizeof(Java_file_3), NETCONN_COPY);
		err=1;
	}
	else
	{
		err=0;
	}
    return err;
}
/*
**********************************************************************
*	函数名称:	Web_Get_html_deal
*	函数功能:	收到html命令处理函数
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char Web_Get_html_deal(char *p,struct netconn *conn)
{
	unsigned char err=0;
	 //超级用户入口
	 if (string_cmp(p,Http_Address_User_Sup)  !=  NULL)
	{
	    netconn_write(conn, Web_Sup_User, sizeof(Web_Sup_User), NETCONN_COPY); 	
		err=1;
	}
	//拒绝访问
	else if (string_cmp(p,Http_Address_User_Refused)  !=  NULL)
	{
	    netconn_write(conn, first_main, sizeof(first_main), NETCONN_COPY);	
		err=1;
							
	}

	//进入重新设定密码页码
	else if (string_cmp(p,Http_Address_User_reset_pasword)  !=  NULL)	
	{
	    netconn_write(conn, Web_Reset_Password, sizeof(Web_Reset_Password), NETCONN_COPY);	
		err=1;
	}
	//帮助文档
	else if (string_cmp(p,Http_Address_User_Help)  !=  NULL)	
	{
	    netconn_write(conn, help, sizeof(help), NETCONN_COPY);	
		err=1;
	}

   	else if (string_cmp(p,Http_Address_User_Main_2)  !=  NULL)	
	{
	    netconn_write(conn, user_main_2, sizeof(user_main_2), NETCONN_COPY);	
		err=1;
	}

    //基本信息
   	else if (string_cmp(p,Http_Address_User_SYS_MSG)  !=  NULL)	
	{
	    netconn_write(conn, SYS_MSG, sizeof(SYS_MSG), NETCONN_COPY);	
		err=1;
	}						
	//参数信息
	else if (string_cmp(p,Http_Address_User_SET_MSG)  !=  NULL)	
	{
	netconn_write(conn, SET_MSG, sizeof(SET_MSG), NETCONN_COPY);	
		err=1;
	}
	//状态信息   即告警信息
	else if (string_cmp(p,Http_Address_User_ALARM)  !=  NULL)	
	{
	    netconn_write(conn, ALARM, sizeof(ALARM), NETCONN_COPY);	
		err=1;
	}

	else if (string_cmp(p,Http_Address_User_first)  !=  NULL)	
	{
	    netconn_write(conn, user_main_2, sizeof(first_main), NETCONN_COPY);	
		err=1;
	}
     return err;
}
/*
**********************************************************************
*	函数名称:	Web_Get_XML_deal
*	函数功能:	收到xml命令处理函数
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char Web_Get_XML_deal(char *p,struct netconn *conn)
{
	 unsigned char err=0;
     if (string_cmp(p,Http_Address_xml_1)  !=  NULL)
	 {
	  //  netconn_write(conn, faile_XML_1, sizeof(faile_CSS_1), NETCONN_COPY);
		err=1;
	}
	else if (string_cmp(p,Http_Address_xml_2)  !=  NULL)
	{
	   // netconn_write(conn, faile_CSS_2, sizeof(faile_CSS_2), NETCONN_COPY);
		err=1;
	}
	else if (string_cmp(p,Http_Address_xml_3)  !=  NULL)
	{
	   // netconn_write(conn, faile_CSS_3, sizeof(faile_CSS_3), NETCONN_COPY);
		err=1;
	}
	else
	{
	  	err=0;
	}
    return err;
}
/*
**********************************************************************
*	函数名称:	Web_Get_Asp_deal
*	函数功能:	收到xml命令处理函数
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/

unsigned char Web_Get_Asp_deal(char *p,struct netconn *conn)
{
	 unsigned char err=0;

	 p=string_cmp(p,Http_Address_Asp_1);
     if ( p !=  NULL)
	 {
	
		 if (string_cmp(p,Http_Address_first)  !=  NULL)
		 {
		 	  html_date_zu_he();
///		 	  DEBUG_MSG("Http_Address_first",15);
		 	  netconn_write(conn, File_Asp_1, sizeof(File_Asp_1), NETCONN_COPY);
			  err=1;
		 }
		 else if (string_cmp(p,Http_Address_secend)  !=  NULL)
		 {
		 	  html_date_zu_he();
		
		 	  netconn_write(conn, File_Asp_2, sizeof(File_Asp_2), NETCONN_COPY);
///			  	 DEBUG_MSG("supuserinter",15);
			  err=1;
		 }
		 else if (string_cmp(p,Http_Address_third)  !=  NULL)
		 {
		 	  html_date_zu_he();
		
		 	  netconn_write(conn, File_Asp_3, sizeof(File_Asp_3), NETCONN_COPY);
			  err=1;
		 }
	
	
	}
	else if (string_cmp(p,Http_Address_Asp_2)  !=  NULL)
	{
	    netconn_write(conn, File_Asp_2, sizeof(File_Asp_2), NETCONN_COPY);
		err=1;
	}
	else if (string_cmp(p,Http_Address_Asp_3)  !=  NULL)
	{
	    netconn_write(conn, File_Asp_3, sizeof(File_Asp_3), NETCONN_COPY);
		err=1;
	}
	else
	{
	  	err=0;
	}
    return err;
}

/*
**********************************************************************
*	函数名称:	Web_Get_Date_Exec
*	函数功能:	get  即客户端请求
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*				3、len		:	数据包长度
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char Web_Get_Date_Imports(char *p,struct netconn *conn,unsigned short *len)
{
	char *s;	
	unsigned char err=0;
	s=NULL;

	//一般用户登录  即初始化界面
	if(web_strcmp(p,(const unsigned char *)Http_Address_Home)==0)
	{	
	  	  netconn_write(conn, first_main, sizeof(first_main), NETCONN_COPY);	
	  	  err=1; 	return err;		
	}
	//html文件夹
	else if( (s=string_cmp(p,Http_Address_User))  !=  NULL)
	{
		err=Web_Get_html_deal(s,conn);	return err;
	}
	//jva文件夹
	else if( (s=string_cmp(p,Http_Address_Java))  !=  NULL)
	{	
		err=Web_Get_Java_deal(s,conn);	 	return err;
		
	}
   	//css文件夹
	else if( (s=string_cmp(p,Http_Address_CSS))  !=  NULL)
	{
		 err=Web_Get_Css_deal(s,conn);	return err;
	}
	//xml文件夹
	else if( (s=string_cmp(p,Http_Address_XML))  !=  NULL)
	{
		 err=Web_Get_XML_deal(s,conn); 	return err;
	}
	else if( (s=string_cmp(p,Http_Address_Asp))  !=  NULL)
	{
		 err=Web_Get_Asp_deal(s,conn);
	}
 	return err;
}
/*
**********************************************************************
*	函数名称:	Web_Post_cmd_deal
*	函数功能:	服务器POST数据入口
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*				3、da
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char Web_Post_cmd_deal(struct netconn *conn,char *p,char *date,unsigned int *id_len,unsigned int *da_len)
{
    //用户验证
	 if(p==NULL)
	{
		 return 0;
	}
//	DEBUG_MSG("Post_cmd_deal",15);
    if(Web_User_Check(p,date,conn) == 2)
 	{ //	DEBUG_MSG("UserCheck",15);
		 netconn_write(conn, user_main, sizeof(user_main), NETCONN_COPY);
		
	}


    return 0;
}
/*
**********************************************************************
*	函数名称:	Web_Post_Date_Exec
*	函数功能:	服务器POST数据入口
*	入口参数:	1、p		:	上层处理之后的数据包
*				2、conn		:	端口
*				3、da
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/

unsigned char  web_Search_Post_Packet_End(char *str,unsigned int str_len)
{
	unsigned int cnt;
	if(str==NULL)
	{
		 return NULL;
	}
	for(cnt=0; cnt<str_len; cnt++)
	{
		if((*(str+cnt) == '&')&&(*(str+cnt+1) == 'E')&&(*(str+cnt+2) == 'N')&&(*(str+cnt+3) == 'D'))
		{
			   	return 2;	//有找到数据包
		}
	}
	return 1;	   //没有找到数据包
}



unsigned char  Web_Post_Date_Imports(char *p,struct netconn *conn,unsigned int len)
{
    char *temp;char tc;
	char  datebuf[10],idbuf[10];
	unsigned err,Results=0;
	unsigned int id_len,date_len,tot_len,last_len=0;
	unsigned int *b_len;
	struct netbuf *inbuff;
	char *rq;
	b_len=&last_len;
	Results=web_Search_Post_Packet_End(p,len);	//判断是否被分包
	DEBUG_SendStr((unsigned char *)p,len);
	if( Results == 1 )
	{
		 //如果被分包进入出来函数	 	
		 inbuff = netconn_recv(conn);
         pbuf_date_com(inbuff,(void **)&rq, (u16_t*)b_len);
		 DEBUG_SendStr((unsigned char *)rq,*b_len);
		 Results=0;
		 netbuf_delete(inbuff);
		 netconn_write(conn, user_main, sizeof(user_main), NETCONN_COPY);
	}
	

//	temp=Web_Find_Str_Post(p);
//	 i=1;
//	DEBUG_MSG((const unsigned  char *)temp,*len);
//	tc=255;
//	while( ( temp != NULL ) && (tc != 0))
	{	
//		  DEBUG_MSG((const unsigned  char *)idbuf,id_len);	
	//	  tc--;
	//	  temp=Web_Post_Cope(temp,idbuf,datebuf,&id_len,&date_len);
//		  err=Web_Post_cmd_deal(conn,idbuf,datebuf,&id_len,&date_len);
	//	  if(err != 0)
		  {
	//	  		return 0;
		  }
		//  DEBUG_MSG((const unsigned  char *)idbuf,id_len);
	   	//  DEBUG_MSG((const unsigned  char *)datebuf,date_len);	
	}
		
   return 0;
}

/*
**********************************************************************
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
#include "lwip/web/exec/web_api.h"
#include "lwip/web/exec/web_str.h"
#include "lwip/others/AT91/periph/emac/dm9161.h"
//#include <web/http/http_sys.h>
#include "lwip/web/http/http_refuse.h"
//#include <web/http/http_alarm.h>
//#include <web/http/http_set_msg.h>
//#define  RX_EMAC_BUFFER_MAX_LENG   2048


 Rx_Buffer1  Emac_RxUplater;
 //y用户名  linweiping
char USER_ID[]="linweiping";
char Password[]="123456";

char SUP_USER_ID[]="linweiping";
char Sup_Password[]="123456";
//  密码   123456
/*
**********************************************************************
*	函数名称:	pbuf_date_com
*	函数功能:	组合pbuf内的各个内存的内容
*	入口参数:	1、p		:
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
err_t pbuf_date_com(struct netbuf *buf, void **dataptr, u16_t *len)
{
    struct pbuf* lwip_pst_pbuf = NULL, *lwip_pst_cur_pbuf;
	unsigned char  *str;	
    unsigned int  count,i=0;
	if (buf->ptr == NULL) {
      return ERR_BUF;
     }
	 *len=buf->ptr->tot_len ;
	if(Emac_RxUplater.status ==0 )
	{
		  str=Emac_RxUplater.Buffer1.Buffer;
		  Emac_RxUplater.status =1;
	}
	else if(Emac_RxUplater.status == 1)
	{
		  str=Emac_RxUplater.Buffer2.Buffer;
		  Emac_RxUplater.status = 0;
	}
	lwip_pst_pbuf=buf->ptr;
	 for(lwip_pst_cur_pbuf=lwip_pst_pbuf; lwip_pst_cur_pbuf!=NULL; lwip_pst_cur_pbuf=lwip_pst_cur_pbuf->next)
    {
        for(count = 0; count < lwip_pst_cur_pbuf->len;count++,i++)
        {       		
		    *(str+i) = *((unsigned char *)(lwip_pst_cur_pbuf->payload)+count);
        }
    }
	
		
	//DEBUG_SendStr(str,*len);
	*dataptr=str;
	return ERR_OK;
}

/*
**********************************************************************
*	函数名称:	Web_User_Check
*	函数功能:	用户校验
*	入口参数:	1、p		:
*				2、conn		:	端口
*	出口参数:	成功标志	:   0-失败  1-成功
**********************************************************************
*/
unsigned char Web_User_Check(char *p,char *s,struct netconn *conn)
{
	static	char flag=0;
	//校验用户名
	if(flag==0)
	{
		 //判断是否是普通用户名
		if( web_strcmp(p, "user") == 0 )
		{
					//验证用户名
				if( web_strcmp(s, (const unsigned  char *)USER_ID) != 0 )
				{	
		
				     netconn_write(conn, err_display, sizeof(err_display), NETCONN_COPY);
					 return 1;
				}

		}
		//判断是否是超级用户
		else if( web_strcmp(p, "sup_user") != 0 )
		{
			
			  	//验证用户名
				if( web_strcmp(s, (const unsigned  char *)SUP_USER_ID) != 0 )
				{	
					  netconn_write(conn, err_display, sizeof(err_display), NETCONN_COPY);
				 	  return 1;
				}
			
		}
		flag=1;return 0;
	}
	//校验密码
	else if(flag == 1)
	{
			 //判断是否是密码ID
		if( web_strcmp(p, "password") != 0 )
		{
			 flag=0;
			
			 return 1;	//密码ID出错
		}
		//校验密码
		if( web_strcmp(s, (const unsigned  char *)Password) != 0 )
		{	
			 flag=0;
			 netconn_write(conn, err_display, sizeof(err_display), NETCONN_COPY);	
		 	 return 1;
		}
		flag=0;
		return 2;
	}
    return 0;
}


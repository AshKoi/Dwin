/*
**********************************************************************
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
 //y�û���  linweiping
char USER_ID[]="linweiping";
char Password[]="123456";

char SUP_USER_ID[]="linweiping";
char Sup_Password[]="123456";
//  ����   123456
/*
**********************************************************************
*	��������:	pbuf_date_com
*	��������:	���pbuf�ڵĸ����ڴ������
*	��ڲ���:	1��p		:
*				2��conn		:	�˿�
*	���ڲ���:	�ɹ���־	:   0-ʧ��  1-�ɹ�
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
*	��������:	Web_User_Check
*	��������:	�û�У��
*	��ڲ���:	1��p		:
*				2��conn		:	�˿�
*	���ڲ���:	�ɹ���־	:   0-ʧ��  1-�ɹ�
**********************************************************************
*/
unsigned char Web_User_Check(char *p,char *s,struct netconn *conn)
{
	static	char flag=0;
	//У���û���
	if(flag==0)
	{
		 //�ж��Ƿ�����ͨ�û���
		if( web_strcmp(p, "user") == 0 )
		{
					//��֤�û���
				if( web_strcmp(s, (const unsigned  char *)USER_ID) != 0 )
				{	
		
				     netconn_write(conn, err_display, sizeof(err_display), NETCONN_COPY);
					 return 1;
				}

		}
		//�ж��Ƿ��ǳ����û�
		else if( web_strcmp(p, "sup_user") != 0 )
		{
			
			  	//��֤�û���
				if( web_strcmp(s, (const unsigned  char *)SUP_USER_ID) != 0 )
				{	
					  netconn_write(conn, err_display, sizeof(err_display), NETCONN_COPY);
				 	  return 1;
				}
			
		}
		flag=1;return 0;
	}
	//У������
	else if(flag == 1)
	{
			 //�ж��Ƿ�������ID
		if( web_strcmp(p, "password") != 0 )
		{
			 flag=0;
			
			 return 1;	//����ID����
		}
		//У������
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


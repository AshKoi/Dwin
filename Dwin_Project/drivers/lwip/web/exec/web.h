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
#include "lwip/web/user/web_include.h"
#define    NO_ERR			0
#define    ERR_NO_FIND		-1
#define    ERR_STR_NULL		-2

/*typedef struct _EmacRxBuffer
{
	unsigned int  status;
	unsigned int  strlen;
	unsigned char Buffer[1000];
} RxBuffer;
typedef struct _Emac_RxBuffer1
{
	unsigned int  status;
	RxBuffer  Buffer1;
	RxBuffer  Buffer2;
} Rx_Buffer1;*/


/*
typedef struct _RX_Uplatr
{
	unsigned int  id;
	unsigned int  date;
	unsigned char ;
} Rx_Uplater;	*/
err_t pbuf_date_com(struct netbuf *buf, void **dataptr, u16_t *len);
unsigned char Web_Post_cmd_deal(struct netconn *conn,char *p,char *date,unsigned int *id_len,unsigned int *da_len);
unsigned char Web_User_Check(char *p,char *s,struct netconn *conn);
unsigned char Web_Sup_User_Check(char *p,char *s,struct netconn *conn);


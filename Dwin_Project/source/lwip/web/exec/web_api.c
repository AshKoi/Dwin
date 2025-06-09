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
#include "lwip/web/user/web_user.h"
#include "lwip/web/exec/web_str.h"
#include "lwip/web/exec/web_exec.h"
#include "lwip/others/AT91/periph/dbgu/dbgu.h"

unsigned char chack_1[]={"GET /?%23fname="};
unsigned char chack_id[10]={"linweiping"};
unsigned char chack_2[]={"&%23lname="};
unsigned char chack_se[6]={"123456"};
/*
char web_chack_user(const unsigned char *str)
{
   unsigned char temp[100];
   web_strmov(temp,chack_1);
   web_strmov(temp,chack_id);
   web_strmov(temp,chack_2);
   web_strmov(temp,chack_se);
   if(web_strcmp(str,temp) !=0)
   {
   		return 1;
   }

   return 0;
}*/
/* This function processes an incomming connection. */
unsigned int web_os_timeout;



 extern RxBuffer  Emac_RxUplater;
static void process_connection(struct netconn *conn)
{
	struct netbuf *inbuf;
	char *rq;
	unsigned int len=0;
	unsigned int *b_len;
	char *temp;
    inbuf = netconn_recv(conn);
	b_len=&len;
    pbuf_date_com(inbuf,(void **)&rq, (u16_t*)b_len);
//	DEBUG_SendStr(rq,*b_len);	
	//普通用户登录网页主界面   Web_TEXT
	if( (temp=string_cmp(rq, "GET "))  !=  NULL)
	{
	     Web_Get_Date_Imports(temp,conn,(unsigned short*)b_len);
		 netbuf_delete(inbuf);	
	     netconn_close(conn);	
	}
	else if( (temp=string_cmp(rq, "POST "))  !=  NULL)
	{	
	     Web_Post_Date_Imports(rq,conn,(unsigned short*)b_len);	
		 netbuf_delete(inbuf);	
	     netconn_close(conn);	
	}
	

	
}
void
http_thread(void *arg)
{
	struct netconn *conn, *newconn;

	TRACE_INFO( "HTTP service enabled!\n\r" );  	

	/* Create a new TCP connection handle. */
	conn = netconn_new(NETCONN_TCP);
	/* Bind the connection to port 80 on any
	local IP address. */
	netconn_bind(conn, NULL, 80);
	/* Put the connection into LISTEN state. */
	netconn_listen(conn);
	/* Loop forever. */
	while(1)
	{	 	
		/* Accept a new connection. */
		newconn = netconn_accept(conn);
		/* Process the incomming connection. */
		process_connection(newconn);
		/* Deallocate connection handle. */
		netconn_delete(newconn);
	}
}
 void telnet_thread( void *arg)
{
 	struct netconn *conn, *newconn;
	err_t err;
	char strbuf[64], cmdbuf[30];
	u16_t strlen;
	s8_t cmd_c;
#ifdef _DEBUG_EMAC_
    u8_t ipaddr[4] = { IP_ADDR_0, IP_ADDR_1, IP_ADDR_2, IP_ADDR_3 };
#else
	u8_t ipaddr[4] = {192,168,2,61};
#endif	
	TRACE_INFO( "Telnet service enabled!\n\r" );

	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);

	netconn_bind(conn, (struct ip_addr *)&ipaddr, 23);
	netconn_listen(conn);
	while(1)
	{

		newconn = netconn_accept(conn);
		strlen = sprintf(strbuf,"Welcome to uC/OS_II demo shell \r\n");

		netconn_write(newconn, strbuf, strlen, NETCONN_COPY);
		strlen = sprintf(strbuf,"Author: hugang <hgxxx@51eda.com> \r\n");
		netconn_write(newconn, strbuf, strlen, NETCONN_COPY);
		strlen = sprintf(strbuf,"Escape command is exit\r\n\r\n\r\n");
		netconn_write(newconn, strbuf, strlen, NETCONN_COPY);	
		strlen = sprintf(strbuf,"hgxxx@shell:>");
	    cmd_c = 0;
		/* Process the new connection. */
		if(newconn != NULL)
		{  	

	  		struct netbuf *buf;
	  		void *data;
	  		u16_t len;
			u8_t *datab;
			while((buf = netconn_recv(newconn)) != NULL)
			{	
				do
				{
					netbuf_data(buf, &data, &len);
					datab = (unsigned char *) data;
					if((len == 2) && (*datab == 0x0d) && (*(datab+1) == 0x0a))
					{	

						#if   LWIP_LISTEN_TRACE_ERROR
						TRACE_ERROR("1");
						#endif
						if(cmd_c > 0)
						{
							cmdbuf[cmd_c] = 0x00;
							if(strcmp(cmdbuf, "ostime")==0)
							{
	
								INT32U os_time;
								os_time = OSTimeGet();
								strlen = sprintf(strbuf, "\r\nNow, OS time is: %d", os_time);
								netconn_write(newconn, strbuf, strlen, NETCONN_COPY);
							}
							else if(strcmp(cmdbuf, "exit")==0)
							{
				
								
								netbuf_delete(buf);
								goto t_exit;
							}
							else
							{
			
								strlen = sprintf(strbuf, "\r\nyour command is: %s unkonwn command.", cmdbuf);
								netconn_write(newconn, strbuf, strlen, NETCONN_COPY);
							}
							cmd_c = 0;
						}
						strlen = sprintf(strbuf,"\r\nhgxxx@shell:>");
						netconn_write(newconn, strbuf, strlen, NETCONN_COPY);
					}
					else if((len == 1) && (*datab >= 0x20) && (*datab <= 0x7e) && (cmd_c < 29))
					{
						#if  LWIP_LISTEN_TRACE_ERROR
						TRACE_ERROR("2");
						#endif
					
						netconn_write(newconn, data, len, NETCONN_COPY);
						cmdbuf[cmd_c] = *datab;
						cmd_c++;
					}
					else if((len == 1) && (*datab == 0x08) && (cmd_c >0))
					{
						cmd_c--;
						strlen = sprintf(strbuf,"\b \b");
						netconn_write(newconn, strbuf, strlen, NETCONN_COPY);
					}
				}
				while(netbuf_next(buf) >= 0);
				netbuf_delete(buf);
            }
        }
		t_exit:
		err = netconn_delete(newconn);
		err =err ;	
	}
}

#define UDP_REMOTE_IP_ADDR_0	192
#define UDP_REMOTE_IP_ADDR_1	168
#define UDP_REMOTE_IP_ADDR_2	2
#define UDP_REMOTE_IP_ADDR_3	41

void
udp_test_thread(void *arg)
{
   	struct netconn *conn;
	struct netbuf *buf;
	struct ip_addr addr;
	char *data;
	char text[] = "A static text";
	int i;
	
	TRACE_INFO( "UDP test thread started!\n\r" );
	TRACE_INFO( "    Remote Host: %d.%d.%d.%d\n\r", UDP_REMOTE_IP_ADDR_0, UDP_REMOTE_IP_ADDR_1, UDP_REMOTE_IP_ADDR_2, UDP_REMOTE_IP_ADDR_3 );
	TRACE_INFO( "    Remote Port: 8008\n\r" );

//	Lwip_HardInit();
	/* create a new connection */
	conn = netconn_new(NETCONN_UDP);
	
	/* set up the IP address of the remote host */
//	addr.addr = htonl(0x0a000001);
	IP4_ADDR(&addr, UDP_REMOTE_IP_ADDR_0, UDP_REMOTE_IP_ADDR_1, UDP_REMOTE_IP_ADDR_2, UDP_REMOTE_IP_ADDR_3 );
	
	/* connect the connection to the remote host */
	netconn_connect(conn, &addr, 8008);
	/* create a new netbuf */
	buf = netbuf_new();
	data = netbuf_alloc(buf, 10);
	/* create some arbitrary data */
	for(i = 0; i < 10; i++)
		data[i] = i;
		
	/* send the arbitrary data */
	netconn_send(conn, buf);
	/* reference the text into the netbuf */
	netbuf_ref(buf, text, sizeof(text));
	/* send the text */
	i=500;
	while(i)
	{
		OSTimeDly(5000);
		netconn_send(conn, buf);
		i--;
	}
	/* deallocate connection and netbuf */
	netconn_delete(conn);
	netbuf_free(buf);
	netbuf_delete(buf);
}

void
telnet_init(void)
{

  sys_thread_new(TELNET_THREAD_NAME,telnet_thread, NULL,TCPIP_THREAD_STACKSIZE ,TCPIP_THREAD_PRIO+3);
}

void
http_init(void)
{
	sys_thread_new(HTTP_THREAD_NAME,http_thread, NULL,TCPIP_THREAD_STACKSIZE ,TCPIP_THREAD_PRIO+4);
}


void
udp_test_init(void)
{
  sys_thread_new(UDP_THREAD_NAME,udp_test_thread, NULL,TCPIP_THREAD_STACKSIZE ,TCPIP_THREAD_PRIO+5);

}

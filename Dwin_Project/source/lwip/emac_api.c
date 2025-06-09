/***************************************************************
****************************************************************/
#include "emac_api.h"
//#include "apierror.h"
#include "apierrno.h"
#include "lwip/include/arch/lwipopts.h"
#include "lwip/include/lwip/err.h"
#include "lwip/include/lwip/api.h"
#include "lwip/include/lwip/mem.h"
#include "lwip/include/lwip/memp.h"
#include "lwip/others/user/sys/sys_config.h"
#include "lwip/include/lwip/netif.h"
#include "lwip/include/lwip/tcpip.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/others/user/sys/sys_data.h"
#include "lwip/utility/assert.h"
#include <string.h>
//#if !defined(NOTRACE) && (DYN_TRACES == 1)
unsigned int traceLevel = TRACE_LEVEL_ERROR;
//#endif

#if SYS_EMAC_SUPPORT

#ifdef _DEBUG_EMAC_		/* */

#define NO_CONNECT    0
#define CONNECTED     1

extern void emac_tcp_client_task(void * arg);
/*
********************************************************************************************
*             VARIABLE
********************************************************************************************
*/
//static uint8 Tcp_tp[1500];							//	数据包
static uint16 tcp_tp_len;							//	长度
static struct netconn *tcp_conn;					//	TCP 连接网络

#define STK_TEST_TASK_SIZE			128L
#define	PRIOR_TEST_TASK			    15            	// 必须在10到19之间

/* */
#include "tcpecho.h"
#include "udpecho.h"
#define TAST_LWIP_MAIN_PRIO				15
#define TASK_LWIP_TCP_SERVER_PRIO		16
#define TASK_LWIP_TCP_CLIENT_PRIO		17
#define TASK_LWIP_UDP_PRIO				18
/* EMAC */
#define EMAC_ERROR_BASE				0xB00
#define EMAC_SUCC					0
#define EMAC_ERROR_INIT				(ERRORNO_BASE+EMAC_ERROR_BASE+1)
#define EMAC_ERROR_ARGUMENTS		(ERRORNO_BASE+EMAC_ERROR_BASE+2)
#define EMAC_ERROR_TIMEOUT			(ERRORNO_BASE+EMAC_ERROR_BASE+3)

#endif	/* #ifdef _DEBUG_EMAC_ */

/********************************************************************************************
FunctionName:    Lwip_SoftInit()
Description:     完成LwIP最基本的初始化工作
input:			
output:	
********************************************************************************************/
void Lwip_SoftInit(void)
{
	/*sys_init();
	mem_init();
	memp_init();
	pbuf_init();*/
	tcpip_init(NULL,NULL);
}

/********************************************************************************************
FunctionName:    Lwip_HardInit()
Description:     设置LwIP，包括添加配置网络接口、建立接收任务等工作
input:			
output:	
********************************************************************************************/
extern err_t tcpip_input(struct pbuf *p, struct netif *inp);
extern err_t ethernetif_init(struct netif *netif);

static struct netif emac_netif;
void Lwip_HardInit( u8_t* pMacAddr, u8_t* pHost, u8_t* pMask, u8_t* pGateway )
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gateway;
	INT8U  nZeroIP[4] = { 0, 0, 0, 0 };
	
	netif_init();

#ifdef _DEBUG_EMAC_
	IP4_ADDR(&ipaddr,IP_ADDR_0,IP_ADDR_1,IP_ADDR_2,IP_ADDR_3);	
	IP4_ADDR(&netmask,NET_MASK_0,NET_MASK_1,NET_MASK_2,NET_MASK_3);
	IP4_ADDR(&gateway,GATEWAY_ADDR_0,GATEWAY_ADDR_1,GATEWAY_ADDR_2,GATEWAY_ADDR_3);
#else
	IP4_ADDR( &ipaddr, *pHost, *(pHost+1), *(pHost+2), *(pHost+3) );	
	IP4_ADDR( &netmask,*pMask, *(pMask+1), *(pMask+2), *(pMask+3) );
	IP4_ADDR( &gateway,*pGateway, *(pGateway+1), *(pGateway+2), *(pGateway+3) );
#endif
	
	// 设置MAC地址
	memcpy( emac_netif.hwaddr, pMacAddr, 6 );		

	// If all params equal to zero, enable DHCP.
	if( (memcmp( pHost, nZeroIP, sizeof(nZeroIP) ) == 0) &&
	    (memcmp( pMask, nZeroIP, sizeof(nZeroIP) ) == 0) &&
	    (memcmp( pGateway, nZeroIP, sizeof(nZeroIP) ) == 0) )
	{
		INT16U  cnt;
		
		IP4_ADDR( &ipaddr,  0,0,0,0 );
		IP4_ADDR( &netmask, 0,0,0,0 );
		IP4_ADDR( &gateway, 0,0,0,0 );
	
		netif_add( &emac_netif, &ipaddr, &netmask, &gateway, NULL, ethernetif_init, tcpip_input );
		netif_set_default(&emac_netif);
		//netif_set_up(&emac_netif);
		
		//for( i=0; i<5; i++ )
		{
			LWIP_DEBUGF( DHCP_DEBUG | DBG_TRACE | DBG_STATE, ("Start DHCP Request!!\r\n") );
			dhcp_start( &emac_netif );
			IP4_ADDR( &ipaddr, 0, 0, 0, 0 );
			for( cnt=0; (cnt<10) && (ipaddr.addr == 0); cnt++ )
			{
				ipaddr.addr = emac_netif.ip_addr.addr;
				//for(j=0;j<100000;j++);
				OSTimeDlyHMSM( 0,0,1,0 );
			}
			//dhcp_stop( &emac_netif );
			if( ipaddr.addr != 0 )
			{
				ipaddr.addr  = emac_netif.ip_addr.addr;
				gateway.addr = emac_netif.gw.addr;
				netmask.addr = emac_netif.netmask.addr;
				
				memcpy( pHost, &ipaddr, 4 );
				memcpy( pMask, &netmask, 4 );
				memcpy( pGateway, &gateway, 4 );
			}
		}
	}
	else
	{
#if 1
		// set interface
		netif_add(&emac_netif,&ipaddr,&netmask,&gateway,NULL,ethernetif_init,tcpip_input);
		netif_set_default(&emac_netif);
		netif_set_up(&emac_netif);
		
		
#else
		// set loopback interface
		IP4_ADDR( &gateway, 127,0,0,1 );
		IP4_ADDR( &ipaddr,  127,0,0,1 );
		IP4_ADDR( &netmask, 255,255,255,0 );
		netif_add( &loopback_netif, &ipaddr, &netmask, &gateway, NULL, loopif_init, tcpip_input );
		netif_set_default( &loopback_netif );
		netif_set_up( &loopback_netif );
#endif
	}
}

/********************************************************************************************
FunctionName:    T_LwIPEntry()
Description:     LwIP入口函数
input:			 <pvArg>[in/out] 保留参数
output:	
********************************************************************************************/
//struct netif *rt9161_netif ;

void sys_task_emac(void *pvArg)
{
	ENET_PARAM *pEnetParam = (ENET_PARAM*)pvArg;
	if( pEnetParam == NULL )
		TRACE_FATAL( "NO MAC ADDRESS!!!" );

    // configure settings
   // extern void DM9161_SetParam( ENET_PARAM *pEnetParam );
    //DM9161_SetParam( pEnetParam );

	Lwip_SoftInit();
 	Lwip_HardInit( pEnetParam->MACAddr, pEnetParam->TermIP, pEnetParam->SubnetIP, pEnetParam->GateIP );
	
	if( pEnetParam->semInitLwIP != NULL )
		OSSemPost( pEnetParam->semInitLwIP );

#ifdef _DEBUG_EMAC_	
	//telnet_init();
	//http_init();
	//udp_test_init();	
	//OSTaskCreate(emac_tcp_client_task, (void*)NULL, &Emac_tcp_client_stk[STK_TEST_TASK_SIZE-1], PRIOR_TEST_TASK);
#endif
	OSTaskDel(OS_PRIO_SELF);
}

#define TASK_ENET_INIT_STK_SIZE		128
OS_STK	TaskEnetInitStk[TASK_ENET_INIT_STK_SIZE];

BOOLEAN _IsEnetInitOK()
{
    extern int gDriverEmacInitStatus;
    if( gDriverEmacInitStatus == 0 )
        return TRUE;
    return FALSE;
}

int EnetInit( ENET_PARAM* pEnetParam )
{
	INT8U err;
	pEnetParam->semInitLwIP = OSSemCreate( 0 );	
    OSTaskCreate( sys_task_emac, (void*)pEnetParam, &TaskEnetInitStk[TASK_ENET_INIT_STK_SIZE-1], LWIP_EMAC_INIT_PRIO );
    OSSemPend( pEnetParam->semInitLwIP, 0, &err );
	OSSemDel( pEnetParam->semInitLwIP, OS_DEL_ALWAYS, &err );

    return _IsEnetInitOK();
}

/* 以太网重新初始化 */

void RenetInit(ENET_PARAM* pEnetParam)
{
	//GpioClearOutputEX(0x02);
	//OSTimeDlyHMSM(0, 0, 1, 500);
	//GpioSetOutputEX( 0x02);
	OSTimeDlyHMSM(0,0,2,0);
	EnetFree();													// 以太网释放	
    OSTimeDlyHMSM(0,0,2,0);
	EnetInit(pEnetParam);  		
}

extern err_t ethernetif_deinit( void );
extern void  sys_free( void );
void EnetFree( void )
{
	sys_thread_t prio;
	INT8U  err;
	OS_TCB ostcb;

	close_all_sockets();	
	
	sys_free();
	
	netif_set_down( &emac_netif );
	netif_remove( &emac_netif );
	
	ethernetif_deinit();
	// 防止 sys_task_emac 任务在某些意外情况下，未能删除自身；
	err = OSTaskQuery( LWIP_EMAC_INIT_PRIO, &ostcb );
	if( err == OS_NO_ERR )
	{
		//printf( "OSTaskDel( LWIP_EMAC_INIT_PRIO )\n" );
		OSTaskDel( LWIP_EMAC_INIT_PRIO );
	}
		
	for( prio=0; prio<LWIP_THREAD_MAX; prio++ )
	{
		err = OSTaskQuery( LWIP_THREAD_START_PRIO + prio, &ostcb );
		if( err == OS_NO_ERR )
		{
			//printf( "OSTaskDel( LWIP_THREAD_START_PRIO + %d )\n", prio );
			OSTaskDel( LWIP_THREAD_START_PRIO + prio );
		}
  	}
}

const char enetlib_version_string[] =
	"EnetLib v1.1 (" __DATE__ " - " __TIME__ ")";

char* EnetVersion( void )
{
    return (char*)enetlib_version_string;
}

#ifdef _DEBUG_EMAC_	
/********************************************************************************************
FunctionName:    emac_tcp_sendstrings()
Description:     tcp 发送数据
input:			
output:	
********************************************************************************************/
void emac_tcp_sendstrings(uint8 *pdata, uint16 len)
{
	if(tcp_conn)
	{		
		netconn_write(tcp_conn,pdata,len,NETCONN_COPY);		

	}
}


/********************************************************************************************
FunctionName:    emac_tcp_client_task()
Description:     测试任务
input:			
output:	
********************************************************************************************/
extern err_t pbuf_date_com(struct netbuf *buf, void **dataptr, u16_t *len);
void emac_tcp_client_task(void * arg)
{
    struct netbuf *buf;
    void *data;
    u16_t len;
    u8_t ipaddr[4] = {192,168,2,41};
    u16_t port=6800;
    err_t err;	
//    u8_t packflag;
  uint8 temp_tp[500]="11";
  	Lwip_HardInit( NULL, NULL, NULL, NULL );
	DEBUG_SendStr(temp_tp,len);
    while(1)
    {
		tcp_conn = netconn_new(NETCONN_TCP);
		netconn_bind(tcp_conn,NULL,port);
        netconn_connect(tcp_conn,(struct ip_addr*)ipaddr,port);

 		while((buf = netconn_recv(tcp_conn)) != NULL)
        {
				#if LWIP_DEBUG_MSG_SPFING
				//TRACE_ERROR("\n");
				#endif
				do{
	            err = netbuf_data(buf, &data, &len);
	            if(err == ERR_OK)
				{ 	
					DEBUG_SendStr(( unsigned  char *)data,len);	
				   // emac_tcp_sendstrings(data, len);
	            	tcp_tp_len += len;
					
	            }
	        }while((s8_t)netbuf_next(buf) >= (s8_t)0);
	        tcp_tp_len = 0;	
			emac_tcp_sendstrings(temp_tp, 2);
			netbuf_delete(buf);	
        }
		
        Bsp_Emac_Conn_Rpt = 0x0;
        Bsp_Emac_Connect = 0x0;

        netconn_disconnect(tcp_conn);
        netconn_delete(tcp_conn);
        OSTimeDly(OS_TICKS_PER_SEC/5);
    }
}
#endif	/* #ifdef _DEBUG_EMAC_	*/

#include "lwip/include/netif/ppp_oe.h"
int pppoeLinkState = 0;
int pppoeLinkTimeout = 0;
void pppoeLinkStatusCallBack(void *ctx, int errCode, void *arg)
{
	if( errCode == 0 )
	{
    	pppoeLinkState = 1;		// connect ok
    }
    else
    {
    	pppoeLinkState = 0;		// connect fail
    }
	//printf( "pppoeLinkStatusCallBack=%d\n\r", errCode );
}

#include "lwip/include/arch/cc.h"
#include "lwip/include/arch/sys_arch.h"
#include "lwip/netif/ppp/ppp.h"
#include "lwip/netif/ppp/fsm.h"
#include "lwip/netif/ppp/ipcp.h"

static int pppoeHandle = -1;
int pppoeGetIP(  PPPOEIPINFO* pppoeIpInfo )
{
	memset( pppoeIpInfo, 0, sizeof(PPPOEIPINFO) );
	if( pppoeHandle >= 0 )
	{
		memcpy( &pppoeIpInfo->LocalIP[0], &ipcp_gotoptions[pppoeHandle].ouraddr, 4 );
		memcpy( &pppoeIpInfo->RemoteIP[0],&ipcp_hisoptions[pppoeHandle].hisaddr, 4 );
		if( ipcp_gotoptions[pppoeHandle].dnsaddr[0] )
			memcpy( &pppoeIpInfo->DNS[0][0],&ipcp_gotoptions[pppoeHandle].dnsaddr[0], 4 );
		if( ipcp_gotoptions[pppoeHandle].dnsaddr[1] )
			memcpy( &pppoeIpInfo->DNS[1][0],&ipcp_gotoptions[pppoeHandle].dnsaddr[1], 4 );
		return 0;
	}
	return (-1);
}

void pppoeConnectTimeoutHandle( void *arg )
{
	pppoeLinkTimeout = 1;
}

int pppoeConnect( const char* username, const char* password, int timeout )
{
	pppoeHandle = -1;
	
	pppInit();
	pppSetAuth( PPPAUTHTYPE_ANY, username, password );
	/*tcpip_timeout( 3000, pppoeConnectTimeoutHandle, NULL );*/
	pppoeHandle = pppOverEthernetOpen( &emac_netif, "", "", NULL, NULL, timeout );
	/*while( (pppoeLinkState == 0) && (pppoeLinkTimeout == 0) )
	{
		OSTimeDly( OS_TICKS_PER_SEC/10 );
	}
	if( pppoeLinkTimeout == 0 )
		tcpip_untimeout( pppoeConnectTimeoutHandle, NULL );*/
	if( pppoeHandle >= 0 )
		return TRUE;
	return FALSE;
}

int pppoeDisconnect( void )
{
	extern void pppOverEthernetClose(int pd);
	pppOverEthernetClose( pppoeHandle );
	return 0;
}


#endif
/* _EMAC_API_C_ */

int _API_EMAC_init( ENET_PARAM* pEnetParam )
{
	if( EnetInit( pEnetParam ) == TRUE )
        return EMAC_SUCC;
	return EMAC_ERROR_INIT;
}
 
int _API_EMAC_free( void )
{
	EnetFree();
	return EMAC_SUCC;
}

int _API_EMAC_pppoeGetIP(  PPPOEIPINFO* pppoeIpInfo )
{
	return pppoeGetIP( pppoeIpInfo );
}

int _API_EMAC_pppoeConnect( const char* username, const char* password, int timeout )
{
	return pppoeConnect( username, password, timeout );
}

int _API_EMAC_pppoeDisconnect( void )
{
	return pppoeDisconnect();
}

#if 0
int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	return lwip_accept( s, addr, addrlen );
}

int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_bind( s, name, namelen );
}

int shutdown(int s, int how)
{
	return lwip_shutdown( s, how );
}

int getpeername (int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getpeername( s, name, namelen );
}

int getsockname (int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getsockname( s, name, namelen );
}

int getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
	return lwip_getsockopt( s, level, optname, optval, optlen );
}

int setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
	return lwip_setsockopt( s, level, optname, optval, optlen );
}

int close(int s)
{
	return lwip_close( s );
}

int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_connect( s, name, namelen );
}

int listen(int s, int backlog)
{
	return lwip_listen( s, backlog );
}

int recv(int s, void *mem, size_t len, int flags)
{
	return lwip_recv( s, mem, len, flags );
}

int read(int s, void *mem, size_t len)
{
	return lwip_read( s, mem, len );
}

int recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen)
{
	return lwip_recvfrom( s, mem, len, flags, from, fromlen );
}

int send(int s, const void *dataptr, size_t size, int flags)
{
	return lwip_send( s, dataptr, size, flags );
}

int sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen)
{
	return lwip_sendto( s, dataptr, size, flags, to, tolen );
}

int socket(int domain, int type, int protocol)
{
	return lwip_socket( domain, type, protocol );
}

int write(int s, const void *dataptr, size_t size)
{
	return lwip_write( s, dataptr, size );
}

int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                struct timeval *timeout)
{
	return lwip_select( maxfdp1, readset, writeset, exceptset, timeout );
}
#endif
int ioctl(int s, long cmd, void *argp)
{
	return lwip_ioctl( s, cmd, argp );
}

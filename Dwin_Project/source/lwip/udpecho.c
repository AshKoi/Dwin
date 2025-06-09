#if 1
#include <string.h>
#include "include/lwip/sockets.h"
#include "emac_api.h"
void UDPTestMain( void* arg )
{
	int sockUdp, ret;
	struct sockaddr_in local, remote;
	unsigned int remotelen = sizeof(remote);
	unsigned char buf[512];
	
	local.sin_family = AF_INET;
	local.sin_port   = htons( 3200 );		// 监听端口
	local.sin_addr.s_addr = INADDR_ANY;		// 本机
	
	sockUdp = socket( AF_INET, SOCK_DGRAM, 0 );
	ret = bind( sockUdp, (struct sockaddr*)&local, sizeof(local) );
	if( ret == -1 )
		goto _UdpExit;
	
	while( 1 )
	{
		memset( buf, 0, sizeof(buf) );
		ret = recvfrom( sockUdp, buf, sizeof(buf), 0, (struct sockaddr*)&remote, &remotelen );
		if( ret != -1 )
		{
			sendto( sockUdp, buf, ret, 0, (struct sockaddr*)&remote, remotelen );
		}
		OSTimeDlyHMSM( 0,0,0,10 );
	}
	
_UdpExit:
	close( sockUdp );
	OSTaskDel( OS_PRIO_SELF );
}

#if LWIP_IGMP
void MulticastTestMain( void *arg )
{
#if 0 /* LWIP_IGMP */
	int sockUdp, ret;
	struct sockaddr_in local, remote;
	unsigned int remotelen = sizeof(remote);
	unsigned char buf[512];
	
	struct ip_addr ipaddr;
	PPPOEIPINFO *pppoeIpInfo = (PPPOEIPINFO*)arg;
	IP4_ADDR( &ipaddr, *pppoeIpInfo->LocalIP, *(pppoeIpInfo->LocalIP+1), *(pppoeIpInfo->LocalIP+2), *(pppoeIpInfo->LocalIP+3) );
	
	local.sin_family = AF_INET;
	local.sin_port   = htons( 6000 );		// 监听端口
	local.sin_addr.s_addr = INADDR_ANY;		// 本机ipaddr.addr;	//
	
	sockUdp = socket( AF_INET, SOCK_DGRAM, 0 );
	
	/*int ttl = 8;
	ret = setsockopt( sockUdp, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl) );
	if( ret < 0 )
		goto _UdpExit;*/
	
	struct in_addr loif;
	loif.s_addr = ipaddr.addr;
	ret = setsockopt( sockUdp, IPPROTO_IP, IP_MULTICAST_IF, &loif, sizeof(loif) );
	if( ret < 0 )
		goto _UdpExit;
	
	/*ret = bind( sockUdp, (struct sockaddr*)&local, sizeof(local) );
	if( ret == -1 )
		goto _UdpExit;*/

	// add to multicast group	
	ip_mreq group;
	/*group.imr_multiaddr.s_addr = inet_addr( "224.1.1.1" );
	group.imr_interface.s_addr = ipaddr.addr;	//INADDR_ANY;		
	ret = setsockopt( sockUdp, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(ip_mreq) );
	if( ret < 0 )
		goto _UdpExit;*/

	/* Fill remote for sending datagrams */
	memset(&remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr( "224.1.1.1" );
	remote.sin_port = htons(6000);
	sprintf( buf, "hello, multicast!!!" );
	sendto( sockUdp, buf, strlen(buf), 0, (struct sockaddr*)&remote, sizeof(remote) );
	
	//sendto( sockUdp, buf, ret, 0, (struct sockaddr*)&remote, remotelen );
	while( 1 )
	{
		memset( buf, 0, sizeof(buf) );
		ret = recvfrom( sockUdp, buf, sizeof(buf), 0, (struct sockaddr*)&remote, &remotelen );
		if( ret != -1 )
		{
			sprintf( buf, "Multi:%s", buf );
			sendto( sockUdp, buf, strlen(ret), 0, (struct sockaddr*)&remote, remotelen );
		}
		OSTimeDlyHMSM( 0,0,0,10 );
	}
	
_UdpExit:
	// group drop
	setsockopt( sockUdp, IPPROTO_IP, IP_DROP_MEMBERSHIP, &group, sizeof(ip_mreq) );
	
	close( sockUdp );
	OSTaskDel( OS_PRIO_SELF );
#else

#if 1	
	// multicast send
	int sockUdp, ret;
	struct sockaddr_in local, remote;
	unsigned int remotelen = sizeof(remote);
	unsigned char buf[512];
	
	struct ip_addr ipaddr;
	PPPOEIPINFO *pppoeIpInfo = (PPPOEIPINFO*)arg;
	IP4_ADDR( &ipaddr, *pppoeIpInfo->LocalIP, *(pppoeIpInfo->LocalIP+1), *(pppoeIpInfo->LocalIP+2), *(pppoeIpInfo->LocalIP+3) );
	//ipaddr.addr = inet_addr( "192.168.2.59" );
	
	sockUdp = socket( AF_INET, SOCK_DGRAM, 0 );	
	
	struct in_addr loif;
	loif.s_addr = ipaddr.addr;
	ret = setsockopt( sockUdp, IPPROTO_IP, IP_MULTICAST_IF, &loif, sizeof(loif) );
	if( ret < 0 )
		goto _UdpExit;

	/* Fill remote for sending datagrams */
	memset(&remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr( "224.1.1.1" );
	remote.sin_port = htons(6000);
	sprintf( buf, "hello, multicast!!!" );
	ret = sendto( sockUdp, buf, strlen(buf), 0, (struct sockaddr*)&remote, sizeof(remote) );
	if( ret < 0 )
		printf( "multicast send failed!!!\n" );
	
#else
	
	// multicast recv
	int sockUdp, ret;
	struct sockaddr_in local, remote;
	unsigned int remotelen = sizeof(remote);
	unsigned char buf[512];
	
	struct ip_addr ipaddr;
	ipaddr.addr = inet_addr( "192.168.2.59" );		// 本机地址
	
	sockUdp = socket( AF_INET, SOCK_DGRAM, 0 );	
	
	local.sin_family = AF_INET;
	local.sin_port   = htons( 6000 );		// 监听端口
	local.sin_addr.s_addr = INADDR_ANY;		// 本机
	ret = bind( sockUdp, (struct sockaddr*)&local, sizeof(local) );
	if( ret == -1 )
		goto _UdpExit;

	// add to multicast group	
	ip_mreq group;
	group.imr_multiaddr.s_addr = inet_addr( "224.1.1.1" );
	group.imr_interface.s_addr = ipaddr.addr;	
	ret = setsockopt( sockUdp, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(ip_mreq) );
	if( ret < 0 )
		goto _UdpExit;

	while( 1 )
	{
		memset( buf, 0, sizeof(buf) );
		ret = recvfrom( sockUdp, buf, sizeof(buf), 0, (struct sockaddr*)&remote, &remotelen );
		if( ret != -1 )
		{
			dbgu_printf( "mcast data:%s", buf );
		}
		OSTimeDlyHMSM( 0,0,0,10 );
	}
	
#endif	
	
	while( 1 )
	{
		OSTimeDlyHMSM( 0,0,1,10 );
	}
	
_UdpExit:
	close( sockUdp );
	OSTaskDel( OS_PRIO_SELF );
#endif
}
#endif

#else
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "socket.h"

void UDPTestMain( void* arg )
{
	SOCKET *conn;
	INT8U  dataRecv[512];
	INT32S iRealLen;
	INT32U i = 0;
	struct ip_addr remote_addr, local_addr;
	
	struct netbuf *recvbuff;
	
	struct ip_addr paddr;
	INT16U pport;
	
	INT32U count;
	INT8U  szTemp[50];
	
	// 创建UDP连接
	conn = SocketCreate( SOCK_UDP );	
	
	// 在任意地址绑定本机端口
	SocketBind( conn, IP_ADDR_ANY, 5000 );
	
	
	while( 1 )
	{
		recvbuff = SocketRecvFrom( conn );

		sprintf( (void*)szTemp, "\nrecvbuff = 0x%X", recvbuff );
	    WriteComm( 0, szTemp, strlen(szTemp) );

		if( recvbuff != NULL )
		{
			// 获取对方IP，端口号
			SocketGetPeerIP( recvbuff, &paddr, &pport );
			
			// 从 netbuff 结构中获取实际接收到的数据
			iRealLen = SocketGetData( recvbuff, dataRecv );
			
			for(i=0;i<iRealLen;i++)
				dataRecv[i] += 1;
			
			for( count=0; count<10; count++ )
			{
				// 填充数据至 netbuff 结构，并发送出去
				SocketSetData( recvbuff, dataRecv, iRealLen );
				SocketSendTo( conn, recvbuff );
				waitms( 500 );
			}
						
			// 释放 netbuff
			SocketFreeBuff( recvbuff );
		}
		
		OSTimeDly( OS_TICKS_PER_SEC/100 );

	}
}


/*

portTASK_FUNCTION( vBasicTFTPServer , pvParameters ){
        struct netconn *conn;
        char msg[]="testing" ;
        struct netbuf *buf;
        char * data;

        conn = netconn_new( NETCONN_UDP );
        netconn_bind(conn, IP_ADDR_ANY, 1234 ); //local port

        netconn_connect(conn, IP_ADDR_BROADCAST, 1235 );
        for( ;; ){
                buf = netbuf_new();
                data =netbuf_alloc(buf, sizeof(msg));
                memcpy (data, msg, sizeof (msg));
                netconn_send(conn, buf);
                netbuf_delete(buf); // De-allocate packet buffer

                vTaskDelay( 200 ); //some delay!
        }
}
*/
#endif

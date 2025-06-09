/* */
#if 1
#include "include/lwip/sockets.h"
#include "emac_api.h"
#include "others/fm3/periph/emac/emac.h"

// TCP客户端主线程
void TCPClientTestMain( void* arg )
{
	int sockClient;
//    int len;
    struct sockaddr_in remote_addr;
	fd_set readset, writeset;
	struct timeval timeout;
	unsigned char buf[200];
	int ret, retry = 5;;
	unsigned char szSend[] = "This is TCP client speaking!";
	
	struct ip_addr ipaddr;
	PPPOEIPINFO *pppoeIpInfo = (PPPOEIPINFO*)arg;
	IP4_ADDR( &ipaddr, *pppoeIpInfo->RemoteIP, *(pppoeIpInfo->RemoteIP+1), *(pppoeIpInfo->RemoteIP+2), *(pppoeIpInfo->RemoteIP+3) );

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

    sockClient = socket( AF_INET, SOCK_STREAM, 0 );

    remote_addr.sin_len = sizeof(remote_addr);
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(5000);
	remote_addr.sin_addr.s_addr = inet_addr( "192.168.1.76" );

	#define SOCKET_ERROR (-1)

    int   keepAlive = 1;         //启动 KeepAlive 机制
    int   keepIdle = 5;          //首次探测开始前的tcp无数据收发空闲时间
    int   keepInterval = 3;      //每次探测的间隔时间(秒)
    int   keepCount = 2;         //探测次数

    if( setsockopt( sockClient, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive) ) == SOCKET_ERROR )
        printf( "setsockopt [SO_KEEPALIVE] err\n" );
    if( setsockopt( sockClient, IPPROTO_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle) ) == SOCKET_ERROR)
        printf( "setsockopt [TCP_KEEPIDLE] err\n");
    if( setsockopt( sockClient, IPPROTO_TCP, TCP_KEEPINTVL, (void*)&keepInterval, sizeof(keepInterval) ) == SOCKET_ERROR)
        printf( "setsockopt [TCP_KEEPINTVL] err\n");
    if( setsockopt( sockClient, IPPROTO_TCP, TCP_KEEPCNT, (void*)&keepCount, sizeof(keepCount) ) == SOCKET_ERROR)
        printf( "setsockopt [TCP_KEEPCNT] err\n");
	
_retry:
	ret = connect( sockClient, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr) );
	retry--;
	if( (ret == -1) && (retry>0) )
		goto _retry;
	else if( (ret == -1) && (retry==0) )
		goto _TCPCltExit;
	else
	{
		send( sockClient, szSend, strlen(szSend)+1, 0 );
		
		while( 1 )
		{
			FD_ZERO(&readset);
			FD_ZERO(&writeset);
			FD_SET(sockClient, &readset);
			FD_SET(sockClient, &writeset);
			
			ret = select(sockClient+1, &readset, &writeset, 0, &timeout);
			if(ret > 0)
			{
				if (FD_ISSET(sockClient, &readset))
				{
					memset(buf,0,100);
					if( recv( sockClient,buf,100,0) <= 0)
					{
						close(sockClient);
						sockClient = -1;
						goto _TCPCltExit;
					}
					else
					{
						int i = strlen(buf);
						send(sockClient,buf,i ,0);
					}
				}
			}
			else if( ret == 0 )
			{
				printf( "sock disconnect\n" );
			}
			OSTimeDlyHMSM( 0,0,0,10 );
		}
	}

_TCPCltExit:
	OSTaskDel( OS_PRIO_SELF );	
}

///////////////////////////////////////////////////////////////////////////////////
// TCP 服务器主线程
void TCPServerTestMain( void* arg )
{
	int listenfd, remotefd;
    int len;
    struct sockaddr_in local_addr, remote_addr;
	fd_set readset, writeset;
	struct timeval timeout;
	unsigned char buf[512];
	
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
    //test_ethernet_driver();
    listenfd = socket( AF_INET, SOCK_STREAM, 0 );

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(1234);
    local_addr.sin_len = sizeof(local_addr);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(listenfd, (struct sockaddr *) &local_addr, sizeof(local_addr)) < 0 )
        goto _TCPServerExit ;

    if( listen(listenfd, 1) == -1)
        goto _TCPServerExit;

    len = sizeof(remote_addr);
	while(1)
	{
		//这里注意一下，lwip的阻塞不是在listen函数，而是accept
		remotefd = accept(listenfd, (struct sockaddr *)&remote_addr, (socklen_t*)&len);
		if(remotefd != -1)
		{
			int ret;
			send(remotefd,"start to work!\r\n",16,0);
			for(;;)
			{
				FD_ZERO(&readset);
				FD_ZERO(&writeset);
				FD_SET(remotefd, &readset);
				FD_SET(remotefd, &writeset);
				
				ret = select(remotefd+1, &readset, &writeset, 0, &timeout);
				if(ret > 0)
				{
					if (FD_ISSET(remotefd, &readset))
					{
						memset(buf,0,50);
						if(recv(remotefd,buf,50,0) <= 0)
						{
							close(remotefd);
							remotefd = -1;
							break;
						}
						else
						{
							int i = strlen(buf);
							send(remotefd,buf,i ,0);
						}
					}
				}
				else if(ret < 0)
				{
					close(remotefd);
					remotefd = -1;
					break;
				}
				OSTimeDlyHMSM( 0,0,0,10 );
			}
		}
	}
_TCPServerExit:
	OSTaskDel( OS_PRIO_SELF );
}

#else

/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "socket.h"

SOCKET *gpConnSock;

// TCP客户端数据接收线程
void  TcpClientRecv(void *arg)
{	
	INT8U dataRecv[512];
	INT32U iRealLen;
	INT8U err;
	
	arg=arg;
	
	while( 1 )
	{
		// 接收TCP数据，并从串口转发出去
		iRealLen = SocketRecv( gpConnSock, dataRecv, 512, &err );
		if( err == ERR_OK )
		{
			if( iRealLen > 0 )
			{
				//WriteComm( 0, dataRecv, iRealLen );
				err = SocketSend( gpConnSock, dataRecv, iRealLen, SOCK_SEND_COPY );
			}
			OSTimeDly( OS_TICKS_PER_SEC/100 );
		}
		else
			break;
	}
	OSTaskDel( OS_PRIO_SELF );
}

// TCP客户端主线程
void TCPClientTestMain( void* arg )
{
	struct ip_addr remote_addr;
	INT16U remote_port;
	INT8S err;
	INT8U recv[200];
	INT32S len;
	INT32S hRecvThread;
	
	// 创建TCP类型的socket
	gpConnSock = SocketCreate( SOCK_TCP );
	
	IP4_ADDR( &remote_addr, 192,168,2,41 );
	remote_port = 3333;	
	
	// 向指定TCP服务器发起连接
	err = SocketConnect( gpConnSock, &remote_addr, remote_port );
	if( err != ERR_OK )
	{
		// TCP连接失败
		SocketClose( gpConnSock );
		WriteComm( 0, (INT8U*)"\n\rTCP client connect err",24 );
	}
	else
	{
		// 创建TCP接收线程
		hRecvThread = SocketThreadNew( TcpClientRecv, NULL );
	}
	
	SocketSend( gpConnSock, "This is TCP client thread!!", 27, SOCK_SEND_COPY );
	while( 1 )
	{
		// 从串口接收数据，然后通过socket转发出去
		len = ReadCommMs( 0, recv, 200, 100, 1000 );
		if( len > 0 )
		{
			err = SocketSend( gpConnSock, recv, len, SOCK_SEND_COPY );
			if( err != ERR_OK )
				break;
		}
		OSTimeDly( OS_TICKS_PER_SEC / 100 );
	}
	
	SocketThreadFree( hRecvThread );
	SocketClose( gpConnSock );
	OSTaskDel( OS_PRIO_SELF );
}

///////////////////////////////////////////////////////////////////////////////////
// TCP 服务器主线程
void TCPServerTestMain( void* arg )
{
	SOCKET *conn, *newconn;
	INT8U dataRecv[512];
	INT32S iRealLen;
	INT8U err;
	
	struct ip_addr ipaddr;
	INT16U port ;
	
	// 创建TCP类型的socket
	conn = SocketCreate( SOCK_TCP );
	// 绑定本机端口
	SocketBind( conn, NULL, 3000 );
	// 在指定端口等待客户端连接
	SocketListen( conn );
	
	while( 1 )
	{
		// 响应客户端连接请求，进入实际收发过程
		dbgu_printf( "\nSocketAccept Start" );
		newconn = SocketAccept( conn );
		dbgu_printf( "\nSocketAccept End" );
		if( newconn != NULL )
		{
			while( 1 )
			{
				dbgu_printf( "\nSocketRecv Start" );
				iRealLen = SocketRecv( newconn, dataRecv, 512, &err );
				dbgu_printf( "\nSocketRecv End" );
				if( err != ERR_OK )
					break;
				if( iRealLen > 0 )
				{
					SocketGetPeerIP( NULL, &ipaddr, &port );
					err = SocketSend( newconn, dataRecv, iRealLen, SOCK_SEND_COPY );
					if( err != ERR_OK )	
						break;
				}
				OSTimeDly( OS_TICKS_PER_SEC / 100 );
			}
			SocketClose( newconn );
		}
		
	}
	
}
#endif

#ifndef __TCP_ECHO_H__
#define __TCP_ECHO_H__

// TCP客户端数据接收线程
void  TcpClientRecv(void *arg);

// TCP客户端主线程
void TCPClientTestMain( void *parg );

// TCP 服务器主线程
void TCPServerTestMain( void* parg );

#endif
/*
*/
#ifndef _EMAC_API_HEADER_
#define _EMAC_API_HEADER_

#include "include/lwip/sockets.h"
#include "emac_fm3.h"

#define ENET_MODE_SHANDONG      0x5555AAAA
#define ENET_MODE_GWJZQII       0xAAAA5555

typedef struct _tagENET_PARAM{
    INT8U              MACAddr[6];			           /*MAC地址*/		
    INT8U              TermIP[4];                      /*终端IP地址*/
    INT8U              SubnetIP[4];                    /*子网掩码地址*/
    INT8U              GateIP[4];                      /*网关地址*/
    OS_EVENT	       *semInitLwIP;                   /*信号量*/
    INT32U             nTimeout;                       /*初始化超时时间（秒）: 取值范围 10 ~ 5*60*60秒，默认为60秒 */
    INT8U              nDebugLevel;                    /*调试级别: 0 - 无调试信息
                                                                   1 - FATAL
                                                                   2 - ERROR
                                                                   3 - WARNING
                                                                   4 - INFO     */
}ENET_PARAM,*pENET_PARAM;

typedef struct _pppoeIPInfo{
	INT8U LocalIP[4];
	INT8U RemoteIP[4];
	INT8U DNS[2][4];
}PPPOEIPINFO;

extern int gDriverEmacInitStatus;
void Lwip_SoftInit(void);
void Lwip_HardInit( unsigned char* pMacAddr, unsigned char* pHost, unsigned char* pMask, unsigned char* pGateway );
void sys_task_emac(void *pvArg);

// 初始化 lwip 环境

// 释放 lwip


//
void pppoeConnectTimeoutHandle( void *arg );

// 连接 pppoe 服务器


// 断开 pppoe

// 获取 pppoe 地址信息

int _API_EMAC_init( ENET_PARAM* pEnetParam );
int _API_EMAC_free(void);
int _API_EMAC_pppoeConnect( const char* username, const char* password, int timeout );
int _API_EMAC_pppoeDisconnect( void );
int _API_EMAC_pppoeGetIP(  PPPOEIPINFO* pppoeIpInfo );
/* ===============================================================================
                   以太网驱动接口
 ===============================================================================*/
int  EnetInit( ENET_PARAM* pEnetParam );
void EnetFree( void );
void RenetInit(ENET_PARAM* pEnetParam);

int  pppoeConnect( const char* username, const char* password, int timeout );
int  pppoeDisconnect( void );
int  pppoeGetIP(  PPPOEIPINFO* pppoeIpInfo );
int  accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int  bind(int s, const struct sockaddr *name, socklen_t namelen);
int  shutdown(int s, int how);
int  getpeername (int s, struct sockaddr *name, socklen_t *namelen);
int  getsockname (int s, struct sockaddr *name, socklen_t *namelen);
int  getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);
int  setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen);
int  close(int s);
int  connect(int s, const struct sockaddr *name, socklen_t namelen);
int  listen(int s, int backlog);
int  recv(int s, void *mem, size_t len, int flags);
int  read(int s, void *mem, size_t len);
int  recvfrom(int s, void *mem, size_t len, int flags,
              struct sockaddr *from, socklen_t *fromlen);
int  send(int s, const void *dataptr, size_t size, int flags);
int  sendto(int s, const void *dataptr, size_t size, int flags,
            const struct sockaddr *to, socklen_t tolen);
int  socket(int domain, int type, int protocol);
int  write(int s, const void *dataptr, size_t size);
int  select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
            struct timeval *timeout);
int  ioctl(int s, long cmd, void *argp);

#endif	/* _EMAC_API_HEADER_ */

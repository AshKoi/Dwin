/*
*/
#ifndef _EMAC_API_HEADER_
#define _EMAC_API_HEADER_

#include "include/lwip/sockets.h"
#include "emac_fm3.h"

#define ENET_MODE_SHANDONG      0x5555AAAA
#define ENET_MODE_GWJZQII       0xAAAA5555

typedef struct _tagENET_PARAM{
    INT8U              MACAddr[6];			           /*MAC��ַ*/		
    INT8U              TermIP[4];                      /*�ն�IP��ַ*/
    INT8U              SubnetIP[4];                    /*���������ַ*/
    INT8U              GateIP[4];                      /*���ص�ַ*/
    OS_EVENT	       *semInitLwIP;                   /*�ź���*/
    INT32U             nTimeout;                       /*��ʼ����ʱʱ�䣨�룩: ȡֵ��Χ 10 ~ 5*60*60�룬Ĭ��Ϊ60�� */
    INT8U              nDebugLevel;                    /*���Լ���: 0 - �޵�����Ϣ
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

// ��ʼ�� lwip ����

// �ͷ� lwip


//
void pppoeConnectTimeoutHandle( void *arg );

// ���� pppoe ������


// �Ͽ� pppoe

// ��ȡ pppoe ��ַ��Ϣ

int _API_EMAC_init( ENET_PARAM* pEnetParam );
int _API_EMAC_free(void);
int _API_EMAC_pppoeConnect( const char* username, const char* password, int timeout );
int _API_EMAC_pppoeDisconnect( void );
int _API_EMAC_pppoeGetIP(  PPPOEIPINFO* pppoeIpInfo );
/* ===============================================================================
                   ��̫�������ӿ�
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

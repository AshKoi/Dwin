/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#ifndef RMOTLINK_H
#define RMOTLINK_H
#include "Os_cpu.h"
#include "fifo.h"
/**************************************************************************************************
                                           宏及枚举定义
***************************************************************************************************/
//客户机、服务器编号
enum _tagLINK_TYPE
{
	LINK_CLNT=0, 			// 客户机
	LINK_SRVC,				// 服务器
	
	LINK_NUM
	
};

//模块类型编号
enum _tagMODL_TYPE
{
	MODL_TELIT=0,			// telit
	MODL_NEO,
	
	MODL_NUM
	
};

#define LOGIN_TCP                       	0
#define LOGIN_UDP                       	1
// 自检时间
#define MODL_SELF_INTVL					(17 * 60)
// 信号强度读取间隔
#define MODL_RDSIGNAL_INTVL				30


// 简单情况下的at命令函数
#define SendAtCmdAndWaitATRespSlip(pcDataAddr, pKeyWord, nTimeOut, nCnt)		SendAtCmdAndWaitATResp(pcDataAddr, pKeyWord, nTimeOut, nCnt, 0, NULL, 0)

/**************************************************************************************************
                                           结构
***************************************************************************************************/
//远程通信参数
typedef struct _tagRMOT_PARM
{
	INT8U						nApn[50];
	INT8U						nMainIp[4];						//ascii码   10.101.1.185
	INT16U						nMainPort;
	INT8U						nBakIp[4];
	INT16U						nBakPort;
	INT8U						nTermIp[4];
    INT8U                       nTerminalMask[4];                   //(F7)终端子网掩码
    INT8U                       nTerminalGate[4];                	//(F7)终端网关
	INT16U						nTermPort;							//侦听端口
	INT8U						nLoginUserName[33];
	INT8U						nLoginPswd[33];

	INT8U						nNetProt;							//Tcp或者Udp
	INT8U						nWorkMode;							//混合、客户机、服务器
    INT8U                       nOnlineType;                        //在线方式
	INT32U						nLoginIntvl;						//重拨间隔单位: 秒
	INT32U						nBeatIntvl;						    //心跳，单位:秒
	INT32U						nUnCommTimes;						//无通信掉线时间,单位:秒
	INT32U						nCommTOS;							//通信超时时间,单位:秒
}RMOT_PARM, *pRMOT_PARM;


//远程通信中间状态变量
typedef struct _tagRMOT_VAL_CLR
{
	INT8U						nNdStart;							//重启标志
	INT8U						nBaseCmd;							//base命令标志
	INT8U						nModlVer[30];						//版本号
	INT8U						nPPPCmd;							//ppp命令标志

	INT8U						nOpenPortCmd;						//侦听端口命令标志
	INT8U						nLoginSucc;						    //链接成功标志:  客户机登录报文成功为2

	INT8U						nSignalVal;						    //信号强度
    INT8U                       nRmotCtrlFlg;                       //远程链接控制 F38 F39
}RMOT_VAL_CLR, *pRMOT_VAL_CLR;


/**************************************************************************************************
                                           变量声明
***************************************************************************************************/
extern pRMOT_PARM		g_pRomtParm;
extern pRMOT_VAL_CLR	g_pRmotValClr;
extern INT16U			g_nLoginCnt;

//模块所用的通用AT返回码
extern const char 	   	g_Modl_OK[];							    //OK\x0D\x0A
extern const char 	   	g_Modl_ERROR[];					        //ERROR
extern const char      	g_Modl_CONNECT[];			        		//"CONNECT"
extern const char 	   	g_Modl_0D0A[];				    			//\x0D\x0A
extern const char 	   	g_Modl_TELIT[];							//Telit
extern const char 	   	g_Modl_NEO[];					    		//NEOWAY


/**************************************************************************************************
                                           声明
***************************************************************************************************/
INT32S	GetTotalBaseBufPtr(INT32U* pProcPtr, INT32U* pRecvPtr);

INT32S 	RmotParamTrans(void);					//转化通信参数
void 	RmotBufValidDataInfoMake(INT32S nPtr, INT32S nSize);
INT32S 	RmotAppBufLenGet(INT8U nLinkType);
INT32S 	RmotRecv(INT8U* pBuf, INT32S nRdLen);
INT32S  RmotSend(INT8U* pBuf, INT32S nWrLen);

INT32S 	BaseBufChkLabel(pBASE_BUF pBaseBuf, INT32S nPosBase, INT8U* pLabel, INT8U nVal, INT8U nClr);
void 	SendAtOrder(const char *pcDataAddr);
INT32S 	SendAtCmdAndWaitATResp(const char *pcDataAddr,const char *pKeyWord, INT32S nTimeOut, INT8U nCnt, INT16U nSemFlg, INT8U *pBuf, INT32U nSize);

INT32S 	ModlPowerCtrl(INT8U dir);
INT32S 	ModlOnOff(INT8U dir);
INT32S 	ModlBpsAdpt(INT8U nCnt);
INT32S 	ModlGetHardVer(void);
void    ModlSignalRecord(INT8U nSignal);

INT32S 	TcpLinkFrame(INT8U nFn);
INT32S  NetLinkFrame(INT32S nSock, INT8U nFn);


#endif




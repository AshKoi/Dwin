/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#ifndef RMOTLINK_H
#define RMOTLINK_H
#include "Os_cpu.h"
#include "fifo.h"
/**************************************************************************************************
                                           �꼰ö�ٶ���
***************************************************************************************************/
//�ͻ��������������
enum _tagLINK_TYPE
{
	LINK_CLNT=0, 			// �ͻ���
	LINK_SRVC,				// ������
	
	LINK_NUM
	
};

//ģ�����ͱ��
enum _tagMODL_TYPE
{
	MODL_TELIT=0,			// telit
	MODL_NEO,
	
	MODL_NUM
	
};

#define LOGIN_TCP                       	0
#define LOGIN_UDP                       	1
// �Լ�ʱ��
#define MODL_SELF_INTVL					(17 * 60)
// �ź�ǿ�ȶ�ȡ���
#define MODL_RDSIGNAL_INTVL				30


// ������µ�at�����
#define SendAtCmdAndWaitATRespSlip(pcDataAddr, pKeyWord, nTimeOut, nCnt)		SendAtCmdAndWaitATResp(pcDataAddr, pKeyWord, nTimeOut, nCnt, 0, NULL, 0)

/**************************************************************************************************
                                           �ṹ
***************************************************************************************************/
//Զ��ͨ�Ų���
typedef struct _tagRMOT_PARM
{
	INT8U						nApn[50];
	INT8U						nMainIp[4];						//ascii��   10.101.1.185
	INT16U						nMainPort;
	INT8U						nBakIp[4];
	INT16U						nBakPort;
	INT8U						nTermIp[4];
    INT8U                       nTerminalMask[4];                   //(F7)�ն���������
    INT8U                       nTerminalGate[4];                	//(F7)�ն�����
	INT16U						nTermPort;							//�����˿�
	INT8U						nLoginUserName[33];
	INT8U						nLoginPswd[33];

	INT8U						nNetProt;							//Tcp����Udp
	INT8U						nWorkMode;							//��ϡ��ͻ�����������
    INT8U                       nOnlineType;                        //���߷�ʽ
	INT32U						nLoginIntvl;						//�ز������λ: ��
	INT32U						nBeatIntvl;						    //��������λ:��
	INT32U						nUnCommTimes;						//��ͨ�ŵ���ʱ��,��λ:��
	INT32U						nCommTOS;							//ͨ�ų�ʱʱ��,��λ:��
}RMOT_PARM, *pRMOT_PARM;


//Զ��ͨ���м�״̬����
typedef struct _tagRMOT_VAL_CLR
{
	INT8U						nNdStart;							//������־
	INT8U						nBaseCmd;							//base�����־
	INT8U						nModlVer[30];						//�汾��
	INT8U						nPPPCmd;							//ppp�����־

	INT8U						nOpenPortCmd;						//�����˿������־
	INT8U						nLoginSucc;						    //���ӳɹ���־:  �ͻ�����¼���ĳɹ�Ϊ2

	INT8U						nSignalVal;						    //�ź�ǿ��
    INT8U                       nRmotCtrlFlg;                       //Զ�����ӿ��� F38 F39
}RMOT_VAL_CLR, *pRMOT_VAL_CLR;


/**************************************************************************************************
                                           ��������
***************************************************************************************************/
extern pRMOT_PARM		g_pRomtParm;
extern pRMOT_VAL_CLR	g_pRmotValClr;
extern INT16U			g_nLoginCnt;

//ģ�����õ�ͨ��AT������
extern const char 	   	g_Modl_OK[];							    //OK\x0D\x0A
extern const char 	   	g_Modl_ERROR[];					        //ERROR
extern const char      	g_Modl_CONNECT[];			        		//"CONNECT"
extern const char 	   	g_Modl_0D0A[];				    			//\x0D\x0A
extern const char 	   	g_Modl_TELIT[];							//Telit
extern const char 	   	g_Modl_NEO[];					    		//NEOWAY


/**************************************************************************************************
                                           ����
***************************************************************************************************/
INT32S	GetTotalBaseBufPtr(INT32U* pProcPtr, INT32U* pRecvPtr);

INT32S 	RmotParamTrans(void);					//ת��ͨ�Ų���
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




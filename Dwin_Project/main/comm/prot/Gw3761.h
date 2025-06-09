/******************************************************************************************************
* Copyright (c) 2014
* 文件名:
* 创建人: 王一凡
* 日　期: 2014/01/18
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#ifndef GW_376_1_H
#define GW_376_1_H
#include "Os_cpu.h"
#include "Macro.h"

extern INT8U 				g_nAsynFlg;
extern INT8U				g_nPackAssitFlg;
extern INT8U 				g_nReport;

extern INT8U               Ascend;
extern INT8U               Descend;

extern INT8U           g_nGprsWatchFlg;
/**************************************************************************************************
                                             类型定义
***************************************************************************************************/
//376.1规约协议
typedef struct _tagGW376_1_PROT
{
	INT8U					nHead1;
	INT8U					nLen1[2];
	INT8U					nLen2[2];
	INT8U					nHead2;
	INT8U					nCtrl;
	INT8U					nTermAddr[4];
	INT8U					nMasterAddr;
	INT8U					nAfn;
	INT8U					nSeq;
	INT8U					nDadtData[1];
}GW376_1_PROT, *pGW376_1_PROT;

typedef INT32S (*pGW3761PROC_AFN)(INT8U* pRcv, INT32S nRcvlen, INT8U* pSnd, INT32S nSndlen);
typedef struct _tagGW3761_PROC_AFN
{
	INT8U					nAfn;
	void*					pGw3761ProcAfnSub;
}GW3761_PROC_AFN, *pGW3761_PROC_AFN;

typedef INT32S (*pGW3761PROC_AFN_SUB)(INT16U nPn, INT8U nFn, INT8U* pRcv, INT32S nRcvlen, INT8U* pSnd, INT32S nSndlen, INT16U *pOffset);


//376.1规约协议相关宏
#define GW376_1_HEAD_LEN 			((INT32S)(sizeof(GW376_1_PROT) - 1))					//报文头长度:dadt之前的长度
#define GW376_1_HEAD_TAIL_LEN 	((INT32S)(GW376_1_HEAD_LEN + 2))						//报头报尾长度
#define GW376_1_BASE_LEN 			((INT32S)(GW376_1_HEAD_TAIL_LEN + 4))					//报文基本长度:报头报尾+dadt
#define GW376_1_DATA_POS 			((INT32S)(GW376_1_HEAD_LEN + 4))						//报文dadt的内容位置



    




  
//光一规约层
/**************************************************************************************************
                                          相 关 宏 定 义
***************************************************************************************************/
#define		    GB3761_DATA_LEN			1024		//GB3761数据体长度
#define		    GB3761_PWD_LEN			16			//PWD长度
#define		    GB3761_EC_LEN			2			//EC长度
#define		    GB3761_TP_LEN			6			//TP长度
#define         GB3761_TAIL_LEN         2           //帧尾长度 CS 16
#define         GB3761_HEAD_LEN         6           //帧头长度 68 L L L L 68
#define         GB3761_PROT_HEAD_OFFSET GY_PROT_HEAD_OFFSET //规约头起始位置

#define         GB3761_HEADER               0x68               //报文头
#define         GB3761_TAILER               0x16               //报文尾

//偏移长度
#define         GB3761_HEAD1_IDX                0           //0x68 偏移
#define			GB3761_LEN_IDX     				1			//Len偏移
#define         GB3761_HEAD2_IDX                5           //0x68 偏移
#define			GB3761_CTRL_IDX					6			//Ctrl偏移
#define			GB3761_AREA_IDX  				7			//Area偏移
#define			GB3761_ADDR_IDX					9			//Addr偏移
#define			GB3761_ADDR_FLAG_IDX			11			//AddrFlag偏移
#define			GB3761_AFN_IDX					12			//Afn偏移
#define			GB3761_SEQ_IDX					13			//Seq偏移
#define			GB3761_DATA_IDX					14			//Data偏移

//AFN功能码
#define         GB3761_AFN_ACK           		0x00		//确认否认帧
#define         GB3761_AFN_RESET         		0x01		//复位命令
#define         GB3761_AFN_LINK          		0x02		//链路接口检测
#define         GB3761_AFN_RELAY         		0x03		//中继终端命令
#define         GB3761_AFN_SET_PARAM     		0x04		//终端参数设置
#define         GB3761_AFN_CTRL          		0x05		//控制命令
#define         GB3761_AFN_CHECK_PWD     		0x06		//密码验证
#define         GB3761_AFN_REQ_RPT       		0x08		//请求被级联终端主动上报
#define         GB3761_AFN_REQ_CFG       		0x09		//查询终端配置状态
#define         GB3761_AFN_QUERY_PARAM   		0x0A		//终端参数查询
#define         GB3761_AFN_REQ_TASK      		0x0B		//任务数据查询
#define         GB3761_AFN_REQ_CLASS_I   		0x0C		//请求I类数据
#define         GB3761_AFN_REQ_CLASS_II  		0x0D		//请求II类数据
#define         GB3761_AFN_REQ_CLASS_III 		0x0E		//请求III类数据
#define         GB3761_AFN_TRANS_FILE    	   	0x2F//0x0F  //文件传输,柜子上终端和表计都在总线上，需要区分
                                                            //I型集中器：0x0F，II型集中器:0x1F,电表:0x2F
#define         GB3761_AFN_TRANS_DATA    		0x10		//数据转发
#define         GB3761_AFN_TEST_EXT    		    0xDF		//内部检测
//扩展AFN
#define         GB3761_AFN_SET_PRM_EXT          0xF4//0xA4        //扩展参数设置
#define         GB3761_AFN_QUERY_PRM_EXT        0xFA//0xAA        //扩展参数查询
#define         GB3761_AFN_GY_EXPAND_II         0xF2        //光一扩展

//GB3761的启动站的链路功能码
#define         GB3761_LFN_RESET            0x01            //复位
#define         GB3761_LFN_SEND_DATA        0x04            //用户数据
#define         GB3761_LFN_REQ_LINK         0x09            //链路测试
#define         GB3761_LFN_REQ_LEVEL1       0x0A            //请求1级数据
#define         GB3761_LFN_REQ_LEVEL2       0x0B            //请求2级数据

//GB3761的从动站的链路功能码
#define         GB3761_LFN_ACK              0x0             //复位确认
#define         GB3761_LFN_RESP_DATA        0x08            //用户数据
#define         GB3761_LFN_RESP_DENY        0x09            //否认无所召唤数据
#define         GB3761_LFN_RESP_LINK        0x0B            //链路状态
#define         GB3761_LFN_RESP_NULL        0x1E            //无效LFN

//GB3761响应确认否认帧类型
#define         GB3761_RESP_DENY            0x02              //否认帧
#define         GB3761_RESP_NO_DATA         0x03              //否认无所召唤数据
#define         GB3761_RESP_ACK             0x01              //确认帧
#define         GB3761_RESP_PART            0x04              //部分确认帧
#define         GB3761_RESP_PWD_DENY        0x08              //身份认证错误
#define         GB3761_RESP_DATA            0x05              //正常数据
#define         GB3761_RESP_TERM_REG        0x06              //终端注册

//部分确认帧错误类型
#define         GB3761_PART_ACK             0x0               //正确
#define         GB3761_PART_OTHER           0x01              //其他错误
#define         GB3761_PART_ADDR            0x02              //表地址重复

//身份认证错误码
#define         GB3761_CHECK_SIGN_ERROR    0x01               //签名校验错误
#define         GB3761_CHECK_CIPHER_ERROR  0x02               //密文校验错误
#define         GB3761_CHECK_MAC_ERROR     0x03               //MAC验证(明文)错误

//规约极限值
#define			GB3761_MAX_FN_NUM				8			//最大Fn数量
#define         GB3761_FRM_RES_LEN              (2 + GB3761_DATA_IDX + GB3761_EC_LEN + GB3761_TP_LEN + GB3761_TAIL_LEN)//填充报文时帧尾预留的长度 固定帧头长度+事件长度+时间标签长度
#define         GB3761_MIN_FRM_LEN              20          //最小帧长度

//无效数据类型
#define         GB3761_INVLD_BCD          INVLD_DATA
#define         GB3761_INVLD_BS8          0x0
#define         GB3761_INVLD_BIN          0x0
#define         GB3761_INVLD_ASC          '\0'

//数据转发延时
#define         GB3761_MSG_CARR_TIMEOUT1    (60 * OS_TICKS_PER_SEC)  //穿透抄表载波延时1 60秒
#define         GB3761_MSG_CARR_TIMEOUT2    (40 * OS_TICKS_PER_SEC)  //穿透抄表载波延时2 40秒
#define         GB3761_MSG_485_TIMEOUT      (30 * OS_TICKS_PER_SEC)  //穿透抄表485延时   30秒
#define         GB3761_MSG_DEAFULT_TIMEOUT  (1 * OS_TICKS_PER_SEC)   //穿透抄表默认延时  1秒

#define         GB3761_FILETRANS_HEAD_LEN       16                   //文件传输数据头长度

//等待FTP延时
#define         GB3761_FTP_TIMEOUT          30                       //FTP链接默认延时30秒
/**************************************************************************************************
                                          相 关 数 据 结 构 定 义
***************************************************************************************************/
//GB3761帧Map信息
typedef struct  _tagGB3761_FRM_MAP
{
	INT8U               nHead1;
	union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nProtId : 2;//0：禁用 1:Q/GDW 130-2005  2:GB3761-2009  3:保留
            INT8U       nL1     : 6;
        }sDet;
    }uLen1;
    INT8U               nLen1;
    union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nProtId : 2;//0：禁用 1:Q/GDW 3761-2005  2:GB3761  3:保留
            INT8U       nL2     : 6;
        }sDet;
    }uLen2;
    INT8U               nLen2;
    INT8U               nHead2;
    union
    {
        INT8U           nWord;
        struct
        {
            INT8U       nFun : 4;
            INT8U       nFcv : 1;
			INT8U       nFcb : 1;
            INT8U       nPrm : 1;
            INT8U       nDir : 1;
        }sDet;
    }uCtrl;
    INT8U       		nArea[LEN_AREA_CODE];
    INT8U       		nAddr[LEN_TERM_ADDR];
    union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nGrpAddFlag : 1;
            INT8U       nMSA     : 7;
        }sDet;
    }uA3;
    INT8U               nAfn;
    union
    {
        INT8U           nWord;
        struct
        {
            INT8U       nPRSeq : 4;
            INT8U       nCon   : 1;
            INT8U       nFin   : 1;
            INT8U       nFir   : 1;
            INT8U       nTpv   : 1;
        }sDet;
    }uSEQ;
}GB3761_FRM_MAP, *pGB3761_FRM_MAP;

//GB3761帧信息
typedef struct  _tagGB3761_FRM_INFO
{
    BOOLEAN             bAllMpFlag;                        //全体测量点标志
    BOOLEAN             bSendFlag;                         //内部发送标志
    BOOLEAN             bFirstFlag;                        //第一帧标志
    BOOLEAN             bSucFlag;                          //TRUE  有成功DADT
    BOOLEAN             bFailFlag;                         //FALSE 有失败DADT
    BOOLEAN             bNoDataFlag;                       //无所召唤数据标识
    BOOLEAN             bAutoRptFlag;                      //主动上报任务
    INT16U              nAutoFrmNo;                        //主动上报的帧号
    INT8U               nAckType;                          //确认否认帧类型
    INT8U               nPFC;                              //启动帧序号计数器  TP
    INT8U               nDelay;                            //允许发送传输延时时间 TP
    INT16U              nRxDataLen;                        //数据体长度  DADT+数据单元
    INT16U              nTxDataLen;                        //发送帧长度  DADT+数据单元
}GB3761_FRM_INFO, *pGB3761_FRM_INFO;

//应用通讯
typedef struct _tagAPP_COMM
{
	INT8U       *pTxFrmBuf;                     //发送帧信息
	INT8U       *pRxFrmBuf;                     //接收帧信息
    void        *pFrmInfo;                      //接收帧信息
	INT16U		nFrmLen;						//组帧长度
}APP_COMM, *pAPP_COMM;

//国网规约选项
#define TERM_QGDW130_SCH				0          				//国网2005方案
#define TERM_QGDW3761_SCH           	1          				//国网2009方案

#define TERM_SCH_TYPE   				TERM_QGDW3761_SCH       		//国网规约






/**************************************************************************************************
                                             声明
***************************************************************************************************/
INT32S Gw3761FrmWrite(INT8U nCom, INT8U* pData, INT32S nDatalen);
INT32S Gw3761FrmRead(INT8U nCom, INT8U* pData, INT32S nMaxDatalen, INT32U iInterval, INT32U ms);

INT32S GB3761PnFn2DADT(INT16U nPn, INT8U nFn, INT8U* pOutBuf);
INT8S GB3761DaDt2PnFn(const INT8U *pDaDt, INT16U *pPn, INT16U *pPnNum, INT16U *pFn, INT8U *pFnNum, BOOLEAN *pAllMp);
INT8S GB3761GetAllMpNo(INT8U nAfn, INT16U nFn, INT16U *pPnID, INT16U *pPnNum, INT16U nMaxLen);

INT32S Gw3761MakeFrm(INT8U nCtrl, INT8U nAfn, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen);

INT32S Gw3761CheckPackage(INT8U* pData, INT32S nDatalen);
INT32S Gw3761Proc(INT8U nCom, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen);

INT32S Gw3761AsynCheckPackageTmp(void* pPara);
INT32S Gw3761AsynCheckPackage(void* pPara);
INT32S Gw3761AsynProcTmp(void* pPara);
INT32S Gw3761AsynProc(void* pPara);

INT32S ForwardMakeFrmSub(INT8U nBoradFlg, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U nTimeOutSec, INT8U* pBuf, INT32S nMaxDatalen);
INT32S ForwardMakeFrmSub2(INT8U nProt, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U nTimeOutSec, INT8U* pBuf, INT32S nMaxDatalen);
INT32S Gw3761SelfForwardMakeFrm(INT8U nPort, INT8U* pBuf, INT32S nMaxDatalen);
INT32S Gw3761ForwardMakeFrm(INT8U nProt, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U* pBuf, INT32S nMaxDatalen);

INT32S GetTermAreaAddr(void);

INT8S GB3761TermTest(pAPP_COMM pComm);
INT8S GB3761TermTestProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761TermTestFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT32S GB3761TermTestF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset, INT8U* pBuf);

INT8S GB3761TermReset(pAPP_COMM pComm);
INT8S GB3761TermResetProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761TermResetFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


INT32S Gw3761AsynProcSub(INT8U* pRcv, INT8U* pSnd, INT32S nSndlen);

#endif

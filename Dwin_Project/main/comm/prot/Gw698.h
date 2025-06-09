#ifndef GW_698_H
#define GW_698_H

#include "Base.h"
#include "SysRes.h"
#include "linkcomExt.h"
#include "AssitProt.h"

#include "GuiwinCfg.h"
#include "Guiwin.h"

#include "Fun.h"
#include "Guimenu.h"
#include "GB3761CtrlCmd.h"
#include "GB3761GetClassI.h"
#include "GB3761GetClassII.h"
#include "GB3761GetPrm.h"
#include "GB3761SetPrm.h"

#include "GB3761GetPrmExt.h"
#include "GB3761SetPrmExt.h"
#include "FileTrans.h"
#include "GB3761Trans.h"

#include "Login.h"

#include "MemTbl.h"

#include "meter645.h"

#include "Reset.h"
#include "AutoTest.h"
#include "Telit.h"

#include "Login.h"
#include "Version.h"

#define         GB698_SAADDR_LEN    		        0x06		//地址长度
extern INT16U   g_nHeadLen;

/**************************************************************************************************
                                             类型定义
***************************************************************************************************/
//698规约协议
typedef struct _tagGW698_PROT
{
	INT8U					nHead1;
	INT8U					nDataLen[2];
	INT8U					nCtrl;
//    INT8U                   nAddrLen;//地址的字节数,逻辑地址,服务器地址的地址类型
    INT8U                   nMeterAddrSAFlg;//地址类型
    INT8U					nMeterAddr[GB698_SAADDR_LEN];// 不一定是5 吧
    INT8U                   nClientAddr;
    INT8U					nHCS[2];
    //INT8U                   nRequestType[2];
    //INT8U                   nPIID;
    //INT8U                   nOAD;
	INT8U					nDadtData[1];
}GW698_PROT, *pGW698_PROT;



//GB698帧Map信息
typedef struct  _tagGB698_FRM_MAP
{
	INT8U               nHead;
	union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nProtId : 2;//保留
            INT8U       nL1     : 6;
        }sDet;
    }uLen;
    INT8U               nLen;
    union
    {
        INT8U           nWord;
        struct
        {
            INT8U       nFun : 3;
            INT8U       nFcv : 2;
			INT8U       nFcb : 1;
            INT8U       nPrm : 1;
            INT8U       nDir : 1;
        }sDet;
    }uCtrl;
    INT8U               nSAFlg;
    INT8U       		nSAAddr[6];
    INT8U       		nCA;
    INT8U       		nlHCS;
    INT8U       		nhHCS;
    INT8U       		nRequestType;
    INT8U       		nRequestObject;
    INT8U               nPIID;
    INT8U       		nData[2];
    INT8U               nResultType;
    INT8U               nFollwReport;
    INT8U               nTimeTag;
}GB698_FRM_MAP, *pGB698_FRM_MAP;

//GB698帧信息
typedef struct  _tagGB698_FRM_INFO
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
}GB698_FRM_INFO, *pGB698_FRM_INFO;

//698规约协议相关宏
#define GW698_HEAD_LEN 			((INT32S)(sizeof(GW698_PROT) - 1))					//报文头长度:dadt之前的长度
#define GW698_HEAD_TAIL_LEN 	((INT32S)(GW698_HEAD_LEN + 2))						//报头报尾长度
#define GW698_BASE_LEN 			((INT32S)(GW698_HEAD_TAIL_LEN + 4))					//报文基本长度:报头报尾+dadt
#define GW698_DATA_POS 			((INT32S)(GW698_HEAD_LEN + 4))						//报文dadt的内容位置

#define GW698_APDU_LEN1 		3                                               //APDU Data前的长度

#define         GB698_REQUEST_CONNECT    		    0x02		//建立应用连接请求
#define         GB698_REQUEST_RELEASE    		    0x03		//断开应用连接请求
#define         GB698_REQUEST_GET    		        0x05		//读取请求
#define         GB698_REQUEST_SET    		        0x06		//设置请求
#define         GB698_REQUEST_ACTION    		    0x07		//操作请求
#define         GB698_REQUEST_REPORT    		    0x08		//上报应答
#define         GB698_REQUEST_PROXY    		        0x09		//代理请求
#define         GB698_REQUEST_SECURITY    		        0x10		//代理请求


enum    _tagGW698DATATYPE
{
    e_null=0,
    e_array,
    e_structure,
    e_bool,
    e_bit_string,
    e_double_long,
    e_double_long_unsigned,
    e_octet_string=9,
    e_visible_string,
    e_UTF8_string = 12,
    e_integer=15,
    e_long,
    e_unsigned,
    e_long_unsigned,
    e_long64=20,
    e_long64_unsigned,
    e_enum,
    e_ufloat32,
    e_float64,
    e_date_time,
    e_date,
    e_time,
    e_date_time_s,
    e_OI = 80,
    e_OAD,
    e_ROAD,
    e_OMD,
    e_TI,
    e_TSA,
    e_MAC,
    e_RN,
    e_Region,
    e_Scaler_Unit,
    e_RSD,
    e_CSD,
    e_MS,
    e_SID,
    e_SID_MAC,
    e_COMDCB,
    e_RCSD,
    e_SRD,
    e_SI,
    e_END     
};


enum    _tagGW698DARTYPE
{
    e_Suc = 0,
    e_HardFault,
    e_RefuseRW = 3,
    e_NoDefObject,
    e_NoObject = 6,
    e_TypeErr,
    e_BeyondLim,
    e_DarEND
};

/**************************************************************************************************
                                             声明
***************************************************************************************************/
INT32S Gw698AsynCheckPackage(INT32U nDataNum, INT8U* pBuf);
INT32S Gw698AsynProcSub(INT8U* pRcv, INT8U* pSnd, INT32S nSndlen);
#endif
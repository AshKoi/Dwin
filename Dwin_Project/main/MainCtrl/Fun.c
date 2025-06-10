/******************************************************************************************************
* 2013-2014 益邦科技有限公司
* 文件名: 
* 创建人: 王一凡
* 日　期: 2013/09/21
* 修改人: 
* 日　期: 
* 描　述: 
* 版　本: V1.0
*****************************************************************************************************/
#include "Fun.h"
#include "Base.h"
#include "Macro.h"
#include "GuiwinCfg.h"
#include "Guiwin.h"
#include "Guimenu.h"
#include "SysRes.h"
#include "linkcomExt.h"
#include "linkcom.h"
#include "MemDrvExt.h"
#include "MemTbl.h"
#include "rmotlink.h"
#include "Ethnetlink.h"
#include "MemData.h"
#include "Ac7022.h"
#include "SoftTimer.h"
#include "Meter645.h"
#include "math.h"
#include "MemData.h"
#include "CarrCol.h"
#include "GB3761SetPrmExt.h"
#include "Gw698.h"
#include "Dwin.h"
/**************************************************************************************************
                                           宏定义
***************************************************************************************************/
#define	HAVEHEAD_ROW				(g_nReservHeight + g_nCharHeight)		//留一个字符高度

#define  PSW_FAC    "000000"

/**************************************************************************************************
                                           变量声明定义
***************************************************************************************************/



//INT8U           g_nRdMtProt = 2;      //第二路485规约切换控制，1：645规约 2：3761规约 
INT8U           g_nDispErr  = 0;      //显示故障 
//INT32U          g_nCtrlStartSecs  = 0;
//INT32U          g_nCtrlEndSecs    = 0;
INT8U   g_nMemDataBuf[512] = {0};
INT32U  g_nUART_BPS_NUM   = 2400;
// 通讯口配置
static INT8S	g_nSynUartMode			= 0;					// 通讯口,红外	同步命令切换端口备份用
static INT8S	g_nSynUartBPS			= 0;					// 波特率,1200
static INT8S	g_nSynUartParity 		= 0;					// 校验位，偶校验

// 同步或者集抄工具   终端类型通讯口默认参数配置
const OBJ_COMM_PRM	g_sObjCommPrmBak[] = {
	{0,		COMM_WH232, 	9600,	    2},
	{1,		COMM_WH485, 	9600,		2},
	{2,		COMM_WH485, 	2400,		2},
	{0xFF}
};
// 同步或者集抄工具   终端类型通讯口参数配置    出厂需要配置
static OBJ_COMM_PRM	g_sObjCommPrm[3];

// 异步通信口初始化配置列表
const COM_CFG	g_sComCfg[] = {
#if ASSIST_RUN == ASSIST_RUN_CARRIER
	{COMM_HOST, 			UART_BPS_115200, 	EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
	{COMM_CARR_RCV_DEBUG,	UART_BPS_1200, 		EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
	{COMM_CARR_SND_DEBUG,	UART_BPS_9600, 		EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
#endif

#if ASSIST_RUN == ASSIST_RUN_FZZD
    {COMM_WH4852,			UART_BPS_2400,	    EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
	{COMM_IR,			    UART_BPS_1200,		EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
    {COMM_SMQ,			UART_BPS_115200,	EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
    {COMM_BLE,			    UART_BPS_2400,		EVEN_PARITY, 	 ONE_STOP_BIT, EIGHT_CHAR_BIT},
    {COMM_NULL,			    UART_BPS_115200,	NO_PARITY,   ONE_STOP_BIT, EIGHT_CHAR_BIT},
    {COMM_WH485,			UART_BPS_2400,	EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
    {COMM_232,			    UART_BPS_115200,		NO_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},

#endif

#if ASSIST_RUN == ASSIST_RUN_YUNWEISCJ
    {COMM_IR,			    UART_BPS_1200,		EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},
	{COMM_485_1,			UART_BPS_9600,		EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT},

#endif

	{0xFF},
};

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: AllCommInit
* 输　入:  
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT32S AllCommInit(void)
{
	pCOM_CFG    pComCfg = (pCOM_CFG)&g_sComCfg[0];
	INT8U       nComNum = 0;
	INT8U	    i	    = 0;
    
    i = 0;
    while (1)
    {
        if (0xFF == pComCfg[i].nCom)
        {
            nComNum = i;
            break;
        }
        i++;
    }

	for (i = 0; i < nComNum; i++)
    {
    	CommInit(pComCfg[i].nCom);
    }

	return 0;
}

/********************************************************************************
* 函数名: AsynCommInit
* 输　入:  0xFD时全部初始化
* 输　出:
* 返  回:
* 功  能: 异步通讯口初始化
*********************************************************************************/
INT32S AsynCommInit(INT8U nCom)
{
	pCOM_CFG    pComCfg = (pCOM_CFG)&g_sComCfg[0];
	INT8U	    i	    = 0;
    
	if (0xFD == nCom)
	{
		while(1)
		{
			if (0xFF == pComCfg[i].nCom)
			{
				break;
			}
			nCom = pComCfg[i].nCom;
			CloseComm(nCom);
            if(nCom == COMM_WH485)
              {
                   pComCfg[i].nBaudRate=CheckCom1Baud();
              }
		    if(0 != OpenComm(nCom, pComCfg[i].nBaudRate, pComCfg[i].nParity, pComCfg[i].nStopBits, pComCfg[i].nCharLen))
		    {
				return -1;
		    }
			i++;
		}
		
		return 0;
	}

	
	while(1)
	{
		if (0xFF == pComCfg[i].nCom)
		{
			break;
		}
		if (nCom == pComCfg[i].nCom)
		{
			CloseComm(nCom);
	    	if( 0 != OpenComm(nCom, pComCfg[i].nBaudRate, pComCfg[i].nParity, pComCfg[i].nStopBits, pComCfg[i].nCharLen) )
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
		i++;
    }	

	return 0;
}

/********************************************************************************
* 函数名:GetObjCommPrmType
* 输　入:
*			INT8U			nType     终端类型 :0,I型集中器
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
pOBJ_COMM_PRM GetObjCommPrmType(INT8U nType)
{
	pOBJ_COMM_PRM		pObjCommPrm 	= &g_sObjCommPrm[0];
	INT8U				nNum			= ARR_SIZE(g_sObjCommPrm);
	INT8U				i				= 0;

	for (i = 0; i < nNum; i++)
	{
		if (pObjCommPrm->nObj == nType)
		{
			return pObjCommPrm;
		}

		pObjCommPrm++;
	}

	return NULL;
}

/********************************************************************************
* 函数名:GetObjCommPrmCom
* 输　入:
*			INT8U			nCom     
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
static pOBJ_COMM_PRM GetObjCommPrmCom(INT8U nCom)
{
	pOBJ_COMM_PRM		pObjCommPrm 	= &g_sObjCommPrm[0];
	INT8U				nNum			= ARR_SIZE(g_sObjCommPrm);
	INT8U				i				= 0;

	for (i = 0; i < nNum; i++)
	{
		if (pObjCommPrm->nCom == nCom)
		{
			return pObjCommPrm;
		}

		pObjCommPrm++;
	}

	return NULL;
}

/********************************************************************************
* 函数名:GetCommPrmFromObj
* 输　入:
*			INT8U		nType			终端类型:0，I型终端
* 输　出:
*			INT8S*		pMode
*			INT8S*		pBaud
*			INT8S*		pParity
* 返  回:
* 功  能: 根据同步或者集抄工具通讯参数配置获取对应的通讯口参数表列下标
*********************************************************************************/
INT32S GetCommPrmFromObj(INT8U nType, INT8S* pMode, INT8S* pBaud, INT8S* pParity)
{
	pOBJ_COMM_PRM		pObjCommPrm 	= NULL;
	INT8U				i 				= 0;

	pObjCommPrm = GetObjCommPrmType(nType);
	if (NULL == pObjCommPrm)
	{
		return -1;
	}

	// 匹配出串口序列g_nUartArray下标
	for (i = 0; i < ARR_SIZE(g_nUartArray); i++)
	{
		if (g_nUartArray[i] == pObjCommPrm->nCom)
		{
			if (NULL != pMode)
			{
				*pMode = i;
			}
			break;
		}
	}

	// 匹配出串口波特率序列g_nUartBpsArray下标
	for (i = 0; i < ARR_SIZE(g_nUartBpsArray); i++)
	{
		if (g_nUartBpsArray[i] == pObjCommPrm->nBaud)
		{
			if (NULL != pBaud)
			{
				*pBaud = i;
			}
			break;
		}
	}

	// 匹配出串口校验序列g_nParityArray下标
	for (i = 0; i < ARR_SIZE(g_nParityArray); i++)
	{
		if (g_nParityArray[i] == pObjCommPrm->nParity)
		{
			if (NULL != pParity)
			{
				*pParity = i;
			}
			break;
		}
	}

	return 0;
}


/********************************************************************************
* 函数名:SetObjCommPrm
* 输　入:
*				INT8S			nMode
*				INT8S			nBaud
*				INT8S			nParity
* 输　出:
* 返  回:
* 功  能: 存入同步或者集抄工具的通讯参数
*********************************************************************************/
INT32S SetObjCommPrm(INT8S nMode, INT8S nBaud, INT8S nParity)
{
	pOBJ_COMM_PRM		pObjCommPrm = NULL;

	//根据通讯参数表列g_nUartArray获得通讯口，然后获得对应配置参数
	pObjCommPrm = GetObjCommPrmCom(g_nUartArray[nMode]);
	if (NULL == pObjCommPrm)
	{
		return -1;
	}
	//pObjCommPrm->nCom 	= g_nUartArray[nMode];
	pObjCommPrm->nBaud 	    = g_nUartBpsArray[nBaud];
	pObjCommPrm->nParity 	= g_nParityArray[nParity];

	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.sObjCommPrm[0]), (INT8U*)g_sObjCommPrm, STR_SIZE(pFRAM_STORE, sSysParam.sObjCommPrm));
	
	return 0;
}

/********************************************************************************
* 函数名:RestoreComPrmCfg
* 输　入:
* 输　出:
* 返  回:
* 功  能: 恢复默认的同步或者集抄工具的通讯参数
*********************************************************************************/
void RestoreComPrmCfg( void )
{
    memcpy(&g_sObjCommPrm[0], &g_sObjCommPrmBak[0], sizeof(g_sObjCommPrm));
    _API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.sObjCommPrm[0]), (INT8U*)g_sObjCommPrm, STR_SIZE(pFRAM_STORE, sSysParam.sObjCommPrm));  
}

/********************************************************************************
* 函数名: SetCommCfg
* 输　入:
*				INT8S			nMode
*				INT8S			nBaud
*				INT8S			nParity
* 输　出:
* 返  回:配置串口号
* 功  能: 根据通讯参数表列配置当前串口: 
*********************************************************************************/
INT8U SetCommCfg(INT8S nMode, INT8S nBaud, INT8S nParity)
{
    INT8U       nCom  	= 0;

	//新的串口赋值
	g_nSynUartMode		= nMode;        					// 通讯口,RS485
	g_nSynUartBPS 		= nBaud;        					// 波特率,9600
    g_nSynUartParity  = nParity;      					// 校验位
    
	g_nSynBaseStopBit = 0;        						// 停止位,1
	g_nSynBaseCharBit = 1;        						// 数据位,8

	nCom = g_nUartArray[g_nSynUartMode];					//新的当前口
	
	return nCom;
}


/********************************************************************************
* 函数名:OpenCommPort
* 输　入:
*			INT8U		port	
* 输　出:
* 返  回:
* 功  能: 初始化串口
*********************************************************************************/
INT32S OpenCommPort( INT8U port )
{
	INT32S ret = -1;
	
	CloseComm( port );
	ret = OpenComm( port,
					g_nUartBpsArray[g_nSynUartBPS],
					g_nParityArray[g_nSynUartParity],
					g_nStopBitArray[g_nSynBaseStopBit],
					g_nCharBitArray[g_nSynBaseCharBit] );
	if ( ret==COMM_SUCC )
	{
		CommFlush( port );
		return ret;    
	}
	CloseComm( port );
	return ret;
}

/********************************************************************************
* 函数名: ProcCommCfgPend
* 输　入:				
* 输　出:
* 返  回:
* 功  能: 根据新的配置通讯串口号，处理串口切换。为了防止切换时丢数据，需要下述处理
*********************************************************************************/
INT8U ProcCommCfgPend(INT8U nCom)
{
    INT32S      i           = 0;
	INT8U		nType		= 0;
	//INT8U 		nCom 		= g_nUartArray[g_nSynUartMode];
	
	nType = IsAysnCom(nCom);									//是否有异步通信串口
	if (nType)
	{
		while(CommPending(nCom) > 0)							//new com 是否有数据:如果有，等待异步接收完毕
		{
			if (i > 1000 / 20)									//最多超时1s
			{
				break;
			}
			i++;
			OSTimeDlyHMSM(0, 0, 0, 20);
		}
	}
	COM_PEND_SEM(nCom);
	OpenCommPort(nCom);
	return nCom;
}

/********************************************************************************
* 函数名: ProcCommCfgPost
* 输　入:					
* 输　出:
* 返  回:
* 功  能: 同步或者集抄工具处理完后，恢复为异步口通信参数
*********************************************************************************/
INT8U ProcCommCfgPost(INT8U nCom)
{
	//INT8U nCom = g_nUartArray[g_nSynUartMode];
	
	AsynCommInit(nCom);	
	COM_POST_SEM(nCom);

	return 0;
}

/*
*****************************************************************************
* 函数名:   SetCommCfgExt
* 输　入: 
*				INT8U			nTermType    终端类型:0，I型终端
*				INT8U			nIrFlg		 红外是否选择标志(检测过程中选择红外通讯方式)
* 输  出: 
* 返  回: 
* 功  能:    
*****************************************************************************
*/
void SetCommCfgExt(INT8U nTermType, INT8U nIrFlg)
{
	INT8S				nMode			= 0;
	INT8S				nBaud			= 0;
	INT8S				nParity			= 0;
	
    if (nIrFlg)										//选择了红外方式，不区分终端类型，都是1200波特率
    {
        //23.2.1为修改698电表修改波特率功能g_nUartBpsArray【】数组变更
        SetCommCfg(0, 2, 2);//SetCommCfg(0, 3, 2); 							//当前口配置: 红外口 1200波特率 偶校验   
    }
    else
    {
		//获取相应的通讯参数表列选择下标
		if (GetCommPrmFromObj(nTermType, &nMode, &nBaud, &nParity) < 0)//没有获取到时
		{
			SetCommCfg(2, 6, 2); 						//当前口配置: 232口 9600波特率 偶校验
		}
		else
		{
			SetCommCfg(nMode, nBaud, nParity); 		//当前口配置: 232口 9600波特率 偶校验
		}   
    }
}


#if 0
#endif
/********************************************************************************
* 函数名:AreaRunGet
* 输　入:
*			INT8U			nType    获取方式: 1,直接从铁电获取；其他,从ram全局变量获取
* 输　出:
* 返  回:
* 功  能: 获取地区运行的地区
*********************************************************************************/
INT8U AreaRunGet(INT8U nType)
{
	INT8U		nAreaRun	= 0;

	if (1 == nType)
	{
		_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nAreaRun), (INT8U*)&nAreaRun, STR_SIZE(pFRAM_STORE, sSysParam.nAreaRun));
	}
	else
	{
		nAreaRun = g_pParam->nAreaRun;
	}
	return nAreaRun;
}


/********************************************************************************
* 函数名:AreaRunSet
* 输　入:
*					INT8U		nAreaRun    地区选择
* 输　出:
* 返  回:
* 功  能: 同时同步铁电和ram变量
*********************************************************************************/
void AreaRunSet(INT8U nAreaRun)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nAreaRun), (INT8U*)&nAreaRun, STR_SIZE(pFRAM_STORE, sSysParam.nAreaRun));	
	g_pParam->nAreaRun = nAreaRun;
}

/********************************************************************************
* 函数名:BackClsTimeGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 从铁电取背光时间
*********************************************************************************/
INT8U BackClsTimeGet(void)
{
	INT8U		nBackClsTime	= 0;

	_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nBackClsTime), (INT8U*)&nBackClsTime, STR_SIZE(pFRAM_STORE, sSysParam.nBackClsTime));

	return nBackClsTime;
}

/********************************************************************************
* 函数名:BackClsTimeSet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
void BackClsTimeSet(INT8U nBackClsTime)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nBackClsTime), (INT8U*)&nBackClsTime, STR_SIZE(pFRAM_STORE, sSysParam.nBackClsTime));	
}

/********************************************************************************
* 函数名:TermClsTimeGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT8U TermClsTimeGet(void)
{
	INT8U		nTermClsTime	= 0;

	_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nTermClsTime), (INT8U*)&nTermClsTime, STR_SIZE(pFRAM_STORE, sSysParam.nTermClsTime));

	return nTermClsTime;
}

/********************************************************************************
* 函数名:TermClsTimeSet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
void TermClsTimeSet(INT8U nTermClsTime)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nTermClsTime), (INT8U*)&nTermClsTime, STR_SIZE(pFRAM_STORE, sSysParam.nTermClsTime));	
}

/********************************************************************************
* 函数名:ConKeyFlgGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT8U ConKeyFlgGet(void)
{
	INT8U		nConKeyFlg	= 0;

	_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nConKeyFlg), (INT8U*)&nConKeyFlg, STR_SIZE(pFRAM_STORE, sSysParam.nConKeyFlg));

	return nConKeyFlg;
}

/********************************************************************************
* 函数名:TermClsTimeSet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
void ConKeyFlgSet(INT8U nConKeyFlg)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nConKeyFlg), (INT8U*)&nConKeyFlg, STR_SIZE(pFRAM_STORE, sSysParam.nConKeyFlg));	
}

/********************************************************************************
* 函数名:MenuModeGet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 简易菜单选择: 1简易；其他正常型
*********************************************************************************/
INT8U MenuModeGet(void)
{
  INT8U nMenuMode = 0;
  
  _API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nSimpleMode), (INT8U*)&nMenuMode, STR_SIZE(pFRAM_STORE, sSysParam.nSimpleMode));
  
  if (1 != nMenuMode)
  {
	  nMenuMode = 0;	 
  }
  return nMenuMode;
}

/********************************************************************************
* 函数名:MenuModeSet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 简易菜单选择: 1简易；其他正常型
*********************************************************************************/
void MenuModeSet(INT8U nMenuMode)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nSimpleMode), (INT8U*)&nMenuMode, STR_SIZE(pFRAM_STORE, sSysParam.nSimpleMode));
}


/********************************************************************************
* 函数名:CarrierComGet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 获取与载波测试仪的通讯口
*********************************************************************************/
INT8U CarrierComGet(INT8U nType)
{
	INT8U nCarrierCom = 0;

	if (1 == nType)
	{
		_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nCarrierCom), (INT8U*)&nCarrierCom, STR_SIZE(pFRAM_STORE, sSysParam.nCarrierCom));
	}
	else
	{
		nCarrierCom = g_pParam->nCarrierCom;
	}
	if (1 != nCarrierCom)
	{
	  nCarrierCom = 0;	 
	}
	return nCarrierCom;
}

/********************************************************************************
* 函数名:CarrierComSet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 载波测试仪的通讯口
*********************************************************************************/
void CarrierComSet(INT8U nCarrierCom)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nCarrierCom), (INT8U*)&nCarrierCom, STR_SIZE(pFRAM_STORE, sSysParam.nCarrierCom));
	g_pParam->nCarrierCom = nCarrierCom;
}

/********************************************************************************
* 函数名:WorkOrderGet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 是否需要工单--开关
*********************************************************************************/
INT8U WorkOrderGet(INT8U nType)
{
	INT8U nOrder = 0;

	if (1 == nType)
	{
		_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nWorkOrder), (INT8U*)&nOrder, STR_SIZE(pFRAM_STORE, sSysParam.nWorkOrder));
	}
	else
	{
		nOrder = g_pParam->nWorkOrder;
	}
	if (1 != nOrder)
	{
	  nOrder = 0;	 
	}
	return nOrder;
}

/********************************************************************************
* 函数名:WorkOrderSet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 工单--开关
*********************************************************************************/
void WorkOrderSet(INT8U nOrder)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nWorkOrder), (INT8U*)&nOrder, STR_SIZE(pFRAM_STORE, sSysParam.nWorkOrder));
	g_pParam->nWorkOrder = nOrder;
}

/********************************************************************************
* 函数名:JcMenuGet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 选择集抄工具简易菜单
*********************************************************************************/
INT8U JcMenuGet(INT8U nType)
{
	INT8U nJcMenu = 0;

	if (1 == nType)
	{
		_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nJcMenu), (INT8U*)&nJcMenu, STR_SIZE(pFRAM_STORE, sSysParam.nJcMenu));
	}
	else
	{
		nJcMenu = g_pParam->nJcMenu;
	}
	if (1 != nJcMenu)
	{
	  nJcMenu = 0;	 
	}
	return nJcMenu;
}

/********************************************************************************
* 函数名:JcMenuSet
* 输　入:
* 输　出:
* 返	回:
* 功	能:选择集抄工具简易菜单
*********************************************************************************/
void JcMenuSet(INT8U nJcMenu)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nJcMenu), (INT8U*)&nJcMenu, STR_SIZE(pFRAM_STORE, sSysParam.nJcMenu));
	g_pParam->nJcMenu = nJcMenu;
}


/********************************************************************************
* 函数名:DefaultServGet
* 输　入:
* 输　出:
* 返	回:
* 功	能: 选择默认(公司)服务器
*********************************************************************************/
INT8U DefaultServGet(INT8U nType)
{
	INT8U nDefaultServ = 0;

	if (1 == nType)
	{
		_API_FRAM_Read(STR_OFF(pFRAM_STORE, sSysParam.nDefaultServ), (INT8U*)&nDefaultServ, STR_SIZE(pFRAM_STORE, sSysParam.nDefaultServ));
	}
	else
	{
		nDefaultServ = g_pParam->nDefaultServ;
	}
	if (1 != nDefaultServ)
	{
	  nDefaultServ = 0;	 
	}
	return nDefaultServ;
}

/********************************************************************************
* 函数名:DefaultServSet
* 输　入:
* 输　出:
* 返	回:
* 功	能:选择默认(公司)服务器
*********************************************************************************/
void DefaultServSet(INT8U nDefaultServ)
{
	_API_FRAM_Write(STR_OFF(pFRAM_STORE, sSysParam.nDefaultServ), (INT8U*)&nDefaultServ, STR_SIZE(pFRAM_STORE, sSysParam.nDefaultServ));
	g_pParam->nDefaultServ = nDefaultServ;
}



/********************************************************************************
* 函数名:LoadParmFromFram
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
void LoadParmFromFram(void)
{	
  INT8U        nBuf[2]   = {0};
	//2.FLASH恢复
	//2.1.通讯参数恢复
	memset(&g_pMem->sTermComm, 0, sizeof(g_pMem->sTermComm));
	MemRead(COMM_PRM_FILE, MEM_FLASH, 0, sizeof(g_pMem->sTermComm), &g_pMem->sTermComm);
    
    //控制参数恢复
    memset(&g_pMem->sCtrlPrm, 0, sizeof(g_pMem->sCtrlPrm));
    MemRead(CTRL_FILE, MEM_FLASH, 0, sizeof(g_pMem->sCtrlPrm), &g_pMem->sCtrlPrm);
	
	//2.2.交互数据恢复
	memset(&g_pMem->sXFlhData, 0, sizeof(g_pMem->sXFlhData));
	MemRead(FLASH_XMP_DATA_FILE, MEM_FLASH, 0, sizeof(g_pMem->sXFlhData), &g_pMem->sXFlhData);
    
    //交采第一次写入数据
    FirstWriteAcENData();
    
    //交采交互数据flh
    memset(&g_pMem->sACBackFlhData, 0, sizeof(g_pMem->sACBackFlhData));
    MemRead(AC_EVT_X_BACK_FLH_FILE, MEM_FLASH, 0, sizeof(g_pMem->sACBackFlhData), &g_pMem->sACBackFlhData);

    //Fram密码初始化设置
    MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPswSetFlg),&nBuf[0] ,1, MEM_FRAM);
    if(nBuf[0] != 1)
    {
      memset(&nBuf[0] , 0 ,sizeof(nBuf));
      MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPswSetFlg), &nBuf[0], 1, MEM_FRAM);
    }
}

/********************************************************************************
* 函数名:LoadParmFromFram
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
void LoadParmFromFram0(void)
{
    //1.RAM交互数据初始化
	memset(&g_pMem->sXRamData, 0, sizeof(&g_pMem->sXRamData));
    
    //3.铁电恢复
	memset(&g_pMem->sXFramData, 0, sizeof(g_pMem->sXFramData));
	MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData), (INT8U*)&g_pMem->sXFramData, sizeof(g_pMem->sXFramData), MEM_FRAM);
    
    //交采实时数据
    memset(&g_pMem->sACRealData, 0, sizeof(g_pMem->sACRealData));
	MemoryRead(STR_OFF(pMEM_FRAM_STORE, sAcRealData), (INT8U*)&g_pMem->sACRealData, sizeof(g_pMem->sACRealData), MEM_FRAM);
    
    //交采交互数据铁电
    memset(&g_pMem->sACBackData, 0, sizeof(g_pMem->sACBackData));
    MemoryRead(STR_OFF(pMEM_FRAM_STORE, sACBackData), (INT8U*)&g_pMem->sACBackData, sizeof(g_pMem->sACBackData), MEM_FRAM);

    //载波专变选择
    MemReadCommPrm(F85, &g_nCarrZbSelFlg, 1);
    
    //GPRS监测开关
    MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nGprsFlg),&g_nGprsWatchFlg ,1, MEM_FRAM);
    if (g_nGprsWatchFlg != 1)
    {
        g_nGprsWatchFlg = 0;
    }
}





  


#if 0
#endif
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 硬件初始化接口
*********************************************************************************/
void ResetHardInit(void)
{
    //INT32U nRet = 0;
    
    //HWD_UnlockReg(HWD_UNLOCK_ALL);
    //HWD_EnableReset();   
    
    /* Enable hardware watchdog interrupt */ 
    //HWD_UnlockReg(HWD_UNLOCK_ALL);
    //HWD_EnableInt(HWDIntCallbackFunc);
	//OSSchedLock();
	//while(TRUE);
    if (g_sEnetStateClr.bEnetCreatSockSucc && (g_sEnetStateClr.nSock >= 0))
    {
        //close(g_sEnetStateClr.nSock);
    }
    SendAtCmdAndWaitATRespSlip((char*)"AT#SH=1\r\n", (char*)"NJ\x0c\x09", 1000, 1);
    
    __set_FAULTMASK(1);          			// 关闭所有中断
	NVIC_SystemReset();        			// 复位
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 硬件初始化接口
*********************************************************************************/
void ResetDelayHardInit(INT32U nDelay)
{	
#if 1
	INT16U nMsec = 0;
	INT8U  nSec  = 0;
	INT8U  nMin  = 0;
	
	do
	{
		if (0 == nDelay)
		{
			break;
		}
		
		if (nDelay >= ((59 * 60 * 1000) + (59 * 1000) + 999))
		{
			nMin  = 59;
			nSec  = 59;
			nMsec = 999;
		}
		else
		{
			nMin  = nDelay / (60 * 1000);
			nSec  = (nDelay % (60 * 1000)) / 1000;
			nMsec = (nDelay % (60 * 1000)) % 1000;
		}
		OSTimeDlyHMSM(0, nMin, nSec, nMsec);
		//break;
	}while(0);
	
	//复位	
	ResetHardInit();
#endif
    
}	

/*
*****************************************************************************
* 函数名: GuiwinRecDataRdExt
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 备份全屏
*****************************************************************************
*/
void GuiwinRecDataRdExt(void)
{
    GuiwinRecDataRd(0, HAVEHEAD_ROW, 239, 319);    
}


/*
*****************************************************************************
* 函数名: GuiwinRecDataRestoreExt
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 恢复全屏
*****************************************************************************
*/
void GuiwinRecDataRestoreExt(void)
{
    GuiwinRecDataRestore(0, HAVEHEAD_ROW, 239, 319);       
}


/********************************************************************************
* 函数名:DispPrintProcess
* 输　入:
*			FP32 fPercent :  0.3458显示为34.58%

* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT32S DispPrintProcess(FP32 fPercent)
{
	INT8U           pPrintf[18]    	= {0};
	INT32U			nInter				= 0;
	INT32U			nDim				= 0;

	nDim = (INT32U)(fPercent * 10000);
	if (nDim >= 10000)
	{
		nDim = 10000;
	}
	nInter = (nDim / 100);
	nDim = (nDim - nInter * 100) % 100;
	memset(&pPrintf[0], 0, sizeof(pPrintf));
	sprintf((char*)&pPrintf[0], "已下装:%d.%02d", nInter, nDim);
    pPrintf[strlen((char*)&pPrintf[0])] = '%';
    GuiwinDynMboxCreate(0, &pPrintf[0]);

	return 0;
}

//打印报文
/********************************************************************************
* 函数名:PrintCommHex
* 输　入:
*			

* 输　出:
* 返  回:
* 功  能: 打印报文
*********************************************************************************/
INT8U PrintCommHex(INT8U *pBuff, INT32S nDataLen)
{
     INT32S         nLen            	= 0;
     INT32S         nIdex           	= 0;
     INT8U          nKey            	= 0;
	 INT8U			nBuff[6]			= {0};
     
     if (!g_nCharDispFrm[g_nShowHex])
     {
        return 0; 
     }
     if (nDataLen <= 0)
     {
        return 0;
     }
     nLen = GET_MIN(1024, nDataLen); 
     nIdex = 0;
     while(nIdex < nLen)
     {
		memset(&nBuff[0], 0, sizeof(nBuff));
		memcpy(&nBuff[0], &pBuff[nIdex], GET_MIN(6, nLen - nIdex));
        GuiwinTextOutAuto("%02X %02X %02X %02X %02X %02X",
                       nBuff[0],nBuff[1], nBuff[2], 
                       nBuff[3],nBuff[4], nBuff[5]);
        nIdex += 6;   
     }
     GuiwinAutoDispProc("通信报文", NULL, 0, &nKey); 
     return  nKey;
}


/********************************************************************************
* 函数名:
* 输　入:
*		
* 输　出:
* 返  回:
* 功  能: 扫描获取条形码
*********************************************************************************/
INT32S  ReadCode(INT8U* pBuf)
{
    INT32S      nLen    = 0;
    if (NULL == pBuf)
    {
        return -1;    
    }
    
    TXM_WAKE_PFR = 0x00;
    TXM_WAKE_DDR = 0x01;
    OSTimeDlyHMSM(0,0,0,10);	
    TXM_WAKE_PDOR = 0x01;
    OSTimeDlyHMSM(0,0,0,200);	

    TXM_TRIG_PFR = 0x00;
    TXM_TRIG_DDR = 0x01;
    TXM_TRIG_PDOR = 0x00;
    OSTimeDlyHMSM(0, 0, 1, 0);
    TXM_TRIG_PDOR = 0x01;

    OSTimeDlyHMSM(0,0,0,500);	
    //nLen = ReadCommMs(COMM_TXM, pBuf, 100, 100, 300); 
    if (nLen < 0)
    {
        return -1;
    }
    return nLen;
}
//打印报文
/********************************************************************************
* 函数名:BAK_stat
* 输　入:
*			

* 输　出:
* 返  回:
* 功  能: 电池检测
*********************************************************************************/
INT8U BAK_stat(void)
{

	BAK_CHRG_PFR = 0x00;
	BAK_SHRG_PFR = 0x00;
	BAK_CHRG_DDR = 0x00;
	BAK_SHRG_DDR = 0x00;

	if ((BAK_CHG_RED == 0x00)&&(BAK_CHG_GRN == 0x01))
	{
		return 1;
	}
	else if ((BAK_CHG_RED == 0x01)&&(BAK_CHG_GRN == 0x00))
	{
		return 2;
	}

	return 0;
}






#if 0
#endif
/********************************************************************************
* 函数名:PasswordChk
* 输　入:
*			INT8U*		pOperInput :
*			INT8S		nType :  0时仅判断超级密码;1时仅判断管理员密码;2时仅判普通操作员密码;
								 3时判断超级密码与管理员；
								 4时判断管理员与普通操作员

* 输　出:
* 返  回:
* 功  能: 密码
*********************************************************************************/
INT32S PasswordChk(INT8U* pOperInput, INT8U* pPswInput, INT8S nType)
{

	OPERTOR_PRM			sOpertor;
	pOPERTOR_PRM		pOpertor	= &sOpertor;
    INT8U               nPsw[12]     ={0};
	INT16U				i			= 0;
	INT8U				nBuf[STR_SIZE(pOPERTOR_PRM, nOperPsw) + 1]		= {0};
		
	if (NULL == pPswInput)
	{
		return -1;
	}
	if (0 == nType)
	{
        MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPsw[0]),&nPsw[0] ,6, MEM_FRAM);
		StringToBuf(&nPsw[0], &nBuf[0], sizeof(pOpertor->nOperPsw) * 2);
		if (0 != memcmp(pPswInput, &nBuf[0], sizeof(pOpertor->nOperPsw)))
		{
			return -1;
		}
		//return S_PSW_RET;
	}
#if 0
	else if (1 == nType)
	{
		memset(pOpertor, 0, sizeof(OPERTOR_PRM));
		GET_M_PSW_PRM((INT8U*)pOpertor, sManagerOpertor);
		if (NULL != pOperInput)
		{
			if (0 != memcmp(pOperInput, &(pOpertor->nOpertor[0]), sizeof(pOpertor->nOpertor)))
			{
				return -1;
			}
		}
		if (0 != memcmp(pPswInput, &(pOpertor->nOperPsw[0]), sizeof(pOpertor->nOperPsw)))
		{
			return -1;
		}
		return M_PSW_RET;
	}
	else if (2 == nType)
	{
		for (i = 0; i < ARR_SIZE_EXT(pFLASH_STORE, sOpertor); i++)
		{
			memset(pOpertor, 0, sizeof(OPERTOR_PRM));
			GET_PSW_PRM((INT8U*)pOpertor, i);
			if (NULL != pOperInput)
			{
				if (0 != memcmp(pOperInput, &(pOpertor->nOpertor[0]), sizeof(pOpertor->nOpertor)))
				{
					continue;
				}
			}
			if (0 != memcmp(pPswInput, &(pOpertor->nOperPsw[0]), sizeof(pOpertor->nOperPsw)))
			{
				continue;
			}
			return i;
		}
	}
	else if (3 == nType)
	{
		for (;;)
		{
			StringToBuf(SUPER_PSW, &nBuf[0], sizeof(pOpertor->nOperPsw) * 2);
			if (0 != memcmp(pPswInput, &nBuf[0], sizeof(pOpertor->nOperPsw)))
			{
				break;
			}
			return S_PSW_RET;
		}

		for (;;)
		{
			memset(pOpertor, 0, sizeof(OPERTOR_PRM));
			GET_M_PSW_PRM((INT8U*)pOpertor, sManagerOpertor);
			if (NULL != pOperInput)
			{
				if (0 != memcmp(pOperInput, &(pOpertor->nOpertor[0]), sizeof(pOpertor->nOpertor)))
				{
					break;
				}
			}
			if (0 != memcmp(pPswInput, &(pOpertor->nOperPsw[0]), sizeof(pOpertor->nOperPsw)))
			{
				break;
			}
			return M_PSW_RET;
		}
		return -1;
	}
	else if (4 == nType)
	{
		for (;;)
		{
			memset(pOpertor, 0, sizeof(OPERTOR_PRM));
			GET_M_PSW_PRM((INT8U*)pOpertor, sManagerOpertor);
			if (NULL != pOperInput)
			{
				if (0 != memcmp(pOperInput, &(pOpertor->nOpertor[0]), sizeof(pOpertor->nOpertor)))
				{
					break;
				}
			}
			if (0 != memcmp(pPswInput, &(pOpertor->nOperPsw[0]), sizeof(pOpertor->nOperPsw)))
			{
				break;
			}
			return M_PSW_RET;
		}
		for (i = 0; i < ARR_SIZE_EXT(pFLASH_STORE, sOpertor); i++)
		{
			memset(pOpertor, 0, sizeof(OPERTOR_PRM));
			GET_PSW_PRM((INT8U*)pOpertor, i);
			if (NULL != pOperInput)
			{
				if (0 != memcmp(pOperInput, &(pOpertor->nOpertor[0]), sizeof(pOpertor->nOpertor)))
				{
					continue;
				}
			}
			if (0 != memcmp(pPswInput, &(pOpertor->nOperPsw[0]), sizeof(pOpertor->nOperPsw)))
			{
				continue;
			}
			return i;
		}
		
		return -1;
	}
#endif
	return 1;
}

/********************************************************************************
* 函数名:PasswordSave
* 输　入:
*			INT8U*		pOperInput :  
*			INT8S		nType :  0时管理员;1时先查找操作员号,刷新对应操作员,如没有相应操作员，则直接存储

* 输　出:
* 返  回:
* 功  能: 密码
*********************************************************************************/
INT32S PasswordSave(INT8U* pOperInput, INT8U* pPswInput, INT8S nType)
{
#if 0
	OPERTOR_PRM			sOpertor;
	pOPERTOR_PRM		pOpertor		= &sOpertor;
	INT16U				nOpertorIdx	= 0;
	INT16U				i				= 0;
	
	if (NULL == pPswInput)
	{
		return -1;
	}

	if (0 == nType)
	{
		memset(pOpertor, 0, sizeof(OPERTOR_PRM));
		GET_M_PSW_PRM((INT8U*)pOpertor, sManagerOpertor);
		for (;;)
		{
			if (NULL != pOperInput)
			{
				if (0 != memcmp(pOperInput, &(pOpertor->nOpertor[0]), sizeof(pOpertor->nOpertor)))
				{
					break;
				}
			}
			if (0 != memcmp(pPswInput, &(pOpertor->nOperPsw[0]), sizeof(pOpertor->nOperPsw)))
			{
				break;
			}
			return 0;
		}
		if (NULL != pOperInput)
		{
			memcpy(&(pOpertor->nOpertor[0]), pOperInput, sizeof(pOpertor->nOpertor));
		}
		memcpy(&(pOpertor->nOperPsw[0]), pPswInput, sizeof(pOpertor->nOperPsw));
		SET_M_PSW_PRM((INT8U*)pOpertor, sManagerOpertor);
		return 0;
	}
	else 
	{
		for (;;)
		{
			if (NULL == pOperInput)
			{
				break;
			}
			for (i = 0; i < ARR_SIZE_EXT(pFLASH_STORE, sOpertor); i++)
			{
				memset(pOpertor, 0, sizeof(OPERTOR_PRM));
				GET_PSW_PRM((INT8U*)pOpertor, i);
				if (0 == memcmp(pOperInput, &(pOpertor->nOpertor[0]), sizeof(pOpertor->nOpertor)))
				{
					break;
				}
			}
			if (i >= ARR_SIZE_EXT(pFLASH_STORE, sOpertor))
			{
				break;
			}
			memcpy(&(pOpertor->nOperPsw[0]), pPswInput, sizeof(pOpertor->nOperPsw));
			SET_PSW_PRM((INT8U*)pOpertor, i);
			return 0;
		}

		GET_M_PSW_PRM((INT8U*)&nOpertorIdx, nOpertorIdx);
		if (nOpertorIdx >= ARR_SIZE_EXT(pFLASH_STORE, sOpertor))
		{
			nOpertorIdx = 0;
		}
		
		memset(pOpertor, 0, sizeof(OPERTOR_PRM));
		GET_PSW_PRM((INT8U*)pOpertor, nOpertorIdx);
		if (NULL != pOperInput)
		{
			memcpy(&(pOpertor->nOpertor[0]), pOperInput, sizeof(pOpertor->nOpertor));
		}
		memcpy(&(pOpertor->nOperPsw[0]), pPswInput, sizeof(pOpertor->nOperPsw));
		SET_PSW_PRM((INT8U*)pOpertor, nOpertorIdx);
		
		nOpertorIdx++;
		if (nOpertorIdx >= ARR_SIZE_EXT(pFLASH_STORE, sOpertor))
		{
			nOpertorIdx = 0;
		}
		SET_M_PSW_PRM((INT8U*)&nOpertorIdx, nOpertorIdx);
		return 0;
	}

#endif	
    return 0;
}



/********************************************************************************
* 函数名:CurrWorkidAttrProc
* 输　入:

* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT32S CurrWorkidAttrProc(void)
{
	INT32S nRet = -1;

	do
	{
		if ((nRet = PasswordChk((INT8U*)&g_pParam->sOpertor.nOpertor[0], (INT8U*)&g_pParam->sOpertor.nOperPsw[0], 0)) >= M_PSW_RET)
		{
			break;
		}

		if ((nRet = PasswordChk((INT8U*)&g_pParam->sOpertor.nOpertor[0], (INT8U*)&g_pParam->sOpertor.nOperPsw[0], 1)) >= M_PSW_RET)
		{
			break;
		}

		if ((nRet = PasswordChk((INT8U*)&g_pParam->sOpertor.nOpertor[0], (INT8U*)&g_pParam->sOpertor.nOperPsw[0], 2)) >= 0)
		{
			break;
		}

		nRet = -1;
	}while(0);
	g_pParam->nCurrWorkidAttr = nRet;
	
	return nRet;
}

/********************************************************************************
* 函数名:ChkCurrWorkidProc
* 输　入:
*				INT8U*				pOperator   8位
* 输　出:
* 返  回:0,符合;-1不符合
* 功  能: 
*********************************************************************************/
INT32S ChkCurrWorkidProc(INT8U* pOperator)
{
	INT8U	nBuf[STR_SIZE(pOPERTOR_PRM, nOpertor) + 1] = {0};
	
	if (g_pParam->nCurrWorkidAttr < 0)
	{
		return -1;
	}
	if (g_pParam->nCurrWorkidAttr >= M_PSW_RET)
	{
		return 0;
	}

	StringToBuf(pOperator, &nBuf[0], STR_SIZE(pOPERTOR_PRM, nOpertor) * 2);
	if (0 == memcmp(&nBuf[0], &g_pParam->sOpertor.nOpertor[0], STR_SIZE(pOPERTOR_PRM, nOpertor)))
	{
		return 0;
	}

#ifdef UNCHK_OPETOR_SWICTH
    return 0;
#else
	return -1;
#endif
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查电表参数是否有效
*********************************************************************************/
BOOLEAN IsVldMeterPrm(const pMEM_METER_PRM pMtPrm)
{
    INT16U      nSeq    = 0;
    INT16U      nMp     = 0;

    nSeq = Buf2INT16U(&pMtPrm->nSeq[0]);
    nMp = Buf2INT16U(&pMtPrm->nMp[0]);

	if (NULL == pMtPrm)
	{
		return FALSE;
	}
	if (!pMtPrm->bVld)
	{
		return FALSE;
	}
	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return FALSE;
	}
	if (!ChkRange(nMp, 1, MAX_MP))
	{
		return FALSE;
	}
	if (!ChkRange(pMtPrm->nCom, 1, MAX_COM_NUM - 1))
	{
		return FALSE;
	}
	return TRUE;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查是否重复电表地址
*********************************************************************************/
BOOLEAN IsRedupMtAddr(const pMEM_METER_PRM pMtPrm)
{
    BOOLEAN bRet    = FALSE;
    INT16U  nSeq    = 0;
    INT16U  nTmpSeq = 0;
    MEM_METER_PRM sMtPrm;

    if (NULL == pMtPrm)
	{
		return FALSE;
	}
    
    for (nSeq = 1; nSeq <= MAX_METER_NUM; nSeq++)
    {
        nTmpSeq = Buf2INT16U(&pMtPrm->nSeq[0]);
        if (nSeq == nTmpSeq)//相同电表序号不比对
        {
            continue;
        }
        memset(&sMtPrm, 0, sizeof(sMtPrm));
        MemReadMeterPrm(nSeq, F10, &sMtPrm, sizeof(MEM_METER_PRM));
        if (!IsVldMeterPrm(&sMtPrm))
        {
            continue;
        }
        if (0 != memcmp(&pMtPrm->nAddr[0], &sMtPrm.nAddr[0], LEN_METER_ADDR))
        {
            continue;
        }
        bRet = TRUE;
        break;
    }
    return bRet;
}

/****************************************************************************
* 函数名:BaudChange
* 输　入:INT32U nBaud 波特率
* 输　出:
* 返  回:代号
* 功  能:从波特率值转换成代号
****************************************************************************/
INT8U BaudChange(INT32U nBaud)
{
    INT32U GB3761BaudAry[8] = {0, 600, 1200, 2400, 4800, 7200, 9600, 19200};
    INT32S nBaudDiff = 0;
    INT8U i = 0;

    for (i = 0; i < 8; i++)
    {
        nBaudDiff = nBaud - GB3761BaudAry[i];
        if ((0 == nBaudDiff) || (50 == abs(nBaudDiff)))
        {
            return i ;
        }
    }
    return 0;       //  未找到匹配的 用默认值返回
}

/****************************************************************************
* 函数名:RealDataMakeAndStore
* 输　入:
* 输　出:
* 返  回:
* 功  能:实时数据转换并存储
****************************************************************************/
INT32S RealDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq)
{
    INT8U               i           = 0;
    MEM_METER_PRM       sMtPrm;
    pMEM_METER_PRM      pMtPrm = &sMtPrm;
    pMEM_MP_REAL_DATA   pRealData   = (pMEM_MP_REAL_DATA)(&g_nMemDataBuf[0]);
    
    if(NULL == pRs485ColData)
    {
        return RET_ERR_ERROR;
    }
    memset(&sMtPrm, 0, sizeof(sMtPrm));
    memset(pRealData, 0, sizeof(MEM_MP_REAL_DATA));
    //判断是否为有效测量点
    MemReadMeterPrm(nSeq, F10, pMtPrm, sizeof(MEM_METER_PRM));
    
    if (!IsVldMeterPrm(pMtPrm))
	{
		return RET_ERR_ERROR;
	}
    
    memcpy(&pRealData->nReadTime[0],  &pRs485ColData->uCol07Data.sReadTime.nSec,   6);                 //读表时间
    
    memcpy(&pRealData->nEnergyPP, pRs485ColData->uCol07Data.nEnergyPP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nEnergyNP, pRs485ColData->uCol07Data.nEnergyNP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nEnergyPN, pRs485ColData->uCol07Data.nEnergyPN, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nEnergyNN, pRs485ColData->uCol07Data.nEnergyNN, ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pRealData->nQ1Energy, pRs485ColData->uCol07Data.nQ1Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nQ2Energy, pRs485ColData->uCol07Data.nQ2Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nQ3Energy, pRs485ColData->uCol07Data.nQ3Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nQ4Energy, pRs485ColData->uCol07Data.nQ4Energy, ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pRealData->nPPDemand, pRs485ColData->uCol07Data.nPPDemand, ((MAX_FEE_NUM + 1) * 8));
    memcpy(&pRealData->nNPDemand, pRs485ColData->uCol07Data.nNPDemand, ((MAX_FEE_NUM + 1) * 8));
    memcpy(&pRealData->nPNDemand, pRs485ColData->uCol07Data.nPNDemand, ((MAX_FEE_NUM + 1) * 8));
    memcpy(&pRealData->nNNDemand, pRs485ColData->uCol07Data.nNNDemand, ((MAX_FEE_NUM + 1) * 8));
    
    //电压
    for (i = 0; i < 3; i++)
    {
        memcpy(&pRealData->nVolt[i], pRs485ColData->uCol07Data.nVolt[i], 2);
    }
    
    //电流
    for (i = 0; i < 3; i++)
    {
        memcpy(&pRealData->nCurr[i], pRs485ColData->uCol07Data.nCurr[i], 3);
    }
    
    //有功功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nPWatt[i], pRs485ColData->uCol07Data.nPWatt[i], 3);
    }
    
    //无功功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nNWatt[i], pRs485ColData->uCol07Data.nNWatt[i], 3);
    }
    
    //视在功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nSWatt[i], pRs485ColData->uCol07Data.nSWatt[i], 3);
    }
    
    //功率因数
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nFactor[i], pRs485ColData->uCol07Data.nFactor[i], 2);
    }
    
    //存储
    MemWriteRealData(nSeq, FEND, pRealData, sizeof(MEM_MP_REAL_DATA));
    
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:DayDataMakeAndStore
* 输　入:
* 输　出:
* 返  回:
* 功  能:日数据转换并存储
****************************************************************************/
INT32S DayDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq, INT8U nPre)
{
    INT16U              nHistIdx    = 0;
    STD_TIME            sMeterTime;
    FORMAT20            sHistTime;
    MEM_METER_PRM       sMtPrm;
    pMEM_METER_PRM      pMtPrm = &sMtPrm;
    pMEM_MP_DAY_DATA    pDayData     = (pMEM_MP_DAY_DATA)(&g_nMemDataBuf[0]);
    
    
    if(NULL == pRs485ColData)
    {
        return RET_ERR_ERROR;
    }
    
    memset(&sMtPrm, 0, sizeof(sMtPrm));
    memset(&sMeterTime, 0, sizeof(sMeterTime));
    memset(pDayData, 0, sizeof(MEM_MP_DAY_DATA));
    
    //判断是否为有效测量点
    MemReadMeterPrm(nSeq, F10, pMtPrm, sizeof(MEM_METER_PRM));
    
    if (!IsVldMeterPrm(pMtPrm))
	{
		return RET_ERR_ERROR;
	}
    //赋值时间
    //memcpy(&sMeterTime, &pRs485ColData->uCol07Data.nTime[0], 7);
    sMeterTime.nWeek = pRs485ColData->uCol07Data.nDate[0];
    memcpy(&sMeterTime.nDay, &pRs485ColData->uCol07Data.nDate[1], 3);
    memcpy(&sMeterTime.nSec, &pRs485ColData->uCol07Data.nTime[0], 3);
    
    memset(&sHistTime, 0, sizeof(sHistTime));
    MemFindHistIdxSub(nPre, &sHistTime, &nHistIdx,1);
    
    //判断如果电表日期不一致则不存储
    if (RET_ERR_OK != CheckCol2DayData(GB_07_PROT, &sMeterTime, pRs485ColData))
	{
		return RET_ERR_ERROR;
	}
    
    memcpy(&pDayData->nMeterAddr[0],    &pMtPrm->nAddr[0],   LEN_METER_ADDR);          //电表地址(标签)
    memcpy(&pDayData->sDataTime,        &sHistTime,             sizeof(sHistTime));	        //日时标标签
    memcpy(&pDayData->nReadTime,        &pRs485ColData->uCol07Data.sReadTime,       6);     //读表时间
    
    memcpy(&pDayData->nDEnergyPP, pRs485ColData->uCol07Data.nDEnergyPP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDEnergyNP, pRs485ColData->uCol07Data.nDEnergyNP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDEnergyPN, pRs485ColData->uCol07Data.nDEnergyPN, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDEnergyNN, pRs485ColData->uCol07Data.nDEnergyNN, ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pDayData->nDQ1Energy, pRs485ColData->uCol07Data.nDQ1Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDQ2Energy, pRs485ColData->uCol07Data.nDQ2Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDQ3Energy, pRs485ColData->uCol07Data.nDQ3Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDQ4Energy, pRs485ColData->uCol07Data.nDQ4Energy, ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pDayData->nDPPDemand, pRs485ColData->uCol07Data.nDPPDemand, ((MAX_FEE_NUM + 1) * 8));
    memcpy(&pDayData->nDNPDemand, pRs485ColData->uCol07Data.nDNPDemand, ((MAX_FEE_NUM + 1) * 8));
    //memcpy(&pDayData->nDPNDemand, pRs485ColData->uCol07Data.nPNDemand, ((MAX_FEE_NUM + 1) * 8));
    //memcpy(&pDayData->nDNNDemand, pRs485ColData->uCol07Data.nNNDemand, ((MAX_FEE_NUM + 1) * 8));
    
    //存储
    MemWriteDayData(nSeq, FEND, 1, pDayData, sizeof(MEM_MP_DAY_DATA));
    
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:MonDataMakeAndStore
* 输　入:
* 输　出:
* 返  回:
* 功  能:月数据转换并存储
****************************************************************************/
INT32S MonDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq, INT8U nPre)
{
    pMEM_MP_MON_DATA    pMonData     = (pMEM_MP_MON_DATA)(&g_nMemDataBuf[0]);
    STD_TIME            sMeterTime;
    MEM_METER_PRM       sMtPrm;
    pMEM_METER_PRM      pMtPrm = &sMtPrm;
    FORMAT21            sHistTime;
    INT16U              nHistIdx    = 0;
    
    if(NULL == pRs485ColData)
    {
        return RET_ERR_ERROR;
    }
    //判断是否为有效测量点
    memset(&sMtPrm, 0, sizeof(sMtPrm));
    memset(&sMeterTime, 0, sizeof(sMeterTime));
    memset(pMonData, 0, sizeof(MEM_MP_MON_DATA));
    
    MemReadMeterPrm(nSeq, F10, pMtPrm, sizeof(MEM_METER_PRM));
    if (!IsVldMeterPrm(pMtPrm))
	{
		return RET_ERR_ERROR;
	}
    
    //赋值时间
    sMeterTime.nWeek = pRs485ColData->uCol07Data.nDate[0];
    memcpy(&sMeterTime.nDay, &pRs485ColData->uCol07Data.nDate[1], 3);
    memcpy(&sMeterTime.nSec, &pRs485ColData->uCol07Data.nTime[0], 3);
    
    memset(&sHistTime, 0, sizeof(sHistTime));
    MemFindHistIdxSub(nPre, &sHistTime, &nHistIdx,2);
    
    //判断如果电表日期不一致则不存储
    if (RET_ERR_OK != CheckCol2MonData(GB_07_PROT, &sMeterTime, pRs485ColData))
	{
		return RET_ERR_ERROR;
	}
    
    memcpy(&pMonData->nMeterAddr[0],    &pMtPrm->nAddr[0],   LEN_METER_ADDR);          //电表地址(标签)
    memcpy(&pMonData->sDataTime,        &sHistTime,             sizeof(sHistTime));	   //月时标标签
    memcpy(&pMonData->nReadTime,        &pRs485ColData->uCol07Data.sReadTime,       6);     //读表时间
    
    
    memcpy(&pMonData->nMEnergyPP, pRs485ColData->uCol07Data.nMEnergyPP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pMonData->nMEnergyNP, pRs485ColData->uCol07Data.nMEnergyNP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pMonData->nMEnergyPN, pRs485ColData->uCol07Data.nMEnergyPN, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pMonData->nMEnergyNN, pRs485ColData->uCol07Data.nMEnergyNN, ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pMonData->nMQ1Energy, pRs485ColData->uCol07Data.nMQ1Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pMonData->nMQ2Energy, pRs485ColData->uCol07Data.nMQ2Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pMonData->nMQ3Energy, pRs485ColData->uCol07Data.nMQ3Energy, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pMonData->nMQ4Energy, pRs485ColData->uCol07Data.nMQ4Energy, ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pMonData->nMPPDemand, pRs485ColData->uCol07Data.nMPPDemand, ((MAX_FEE_NUM + 1) * 8));
    memcpy(&pMonData->nMNPDemand, pRs485ColData->uCol07Data.nMNPDemand, ((MAX_FEE_NUM + 1) * 8));
    memcpy(&pMonData->nMPNDemand, pRs485ColData->uCol07Data.nMPNDemand, ((MAX_FEE_NUM + 1) * 8));
    memcpy(&pMonData->nMNNDemand, pRs485ColData->uCol07Data.nMNNDemand, ((MAX_FEE_NUM + 1) * 8));
    
    //存储
    MemWriteMonData(nSeq, FEND, 1, pMonData, sizeof(MEM_MP_MON_DATA));
    
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:CrvDataMakeAndStore
* 输　入:
* 输　出:
* 返  回:
* 功  能:曲线数据转换并存储
****************************************************************************/
INT32S CrvDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq)
{
    INT8U               i           = 0;
    pMEM_MP_CRV_DATA    pCrvData    = (pMEM_MP_CRV_DATA)(&g_nMemDataBuf[0]);
    
    if(NULL == pRs485ColData)
    {
        return RET_ERR_ERROR;
    }
    //判断是否为有效测量点
    memset(pCrvData, 0, sizeof(MEM_MP_CRV_DATA));
    
    memcpy(&pCrvData->nCrvEnergyPP, pRs485ColData->uCol07Data.nEnergyPP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pCrvData->nCrvEnergyNP, pRs485ColData->uCol07Data.nEnergyNP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pCrvData->nCrvEnergyPN, pRs485ColData->uCol07Data.nEnergyPN, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pCrvData->nCrvEnergyNN, pRs485ColData->uCol07Data.nEnergyNN, ((MAX_FEE_NUM + 1) * 4));
    
    //电压
    for (i = 0; i < 3; i++)
    {
        memcpy(&pCrvData->nCrvVolt[i], pRs485ColData->uCol07Data.nVolt[i], 2);
    }
    
    //电流
    for (i = 0; i < 3; i++)
    {
        memcpy(&pCrvData->nCrvCurr[i], pRs485ColData->uCol07Data.nCurr[i], 3);
    }
    
    //有功功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pCrvData->nCrvPWatt[i], pRs485ColData->uCol07Data.nPWatt[i], 3);
    }
    
    //无功功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pCrvData->nCrvNWatt[i], pRs485ColData->uCol07Data.nNWatt[i], 3);
    }
    
    //视在功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pCrvData->nCrvSWatt[i], pRs485ColData->uCol07Data.nSWatt[i], 3);
    }
    
    //功率因数
    for (i = 0; i < 4; i++)
    {
        memcpy(&pCrvData->nCrvFactor[i], pRs485ColData->uCol07Data.nFactor[i], 2);
    }
    
    //存储
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:RdDataMakeAndStore
* 输　入:
* 输　出:
* 返  回:
* 功  能:抄表日数据转换并存储
****************************************************************************/
INT32S RdDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq)
{
    pMEM_MP_RDAY_DATA   pRdData     = (pMEM_MP_RDAY_DATA)(&g_nMemDataBuf[0]);
    
    if(NULL == pRs485ColData)
    {
        return RET_ERR_ERROR;
    }
    //判断是否为有效测量点
    memset(pRdData, 0, sizeof(MEM_MP_RDAY_DATA));
    
    memcpy(&pRdData->nRDEnergyPP, pRs485ColData->uCol07Data.nEnergyPP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRdData->nRDEnergyNP, pRs485ColData->uCol07Data.nEnergyNP, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRdData->nRDEnergyPN, pRs485ColData->uCol07Data.nEnergyPN, ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRdData->nRDEnergyNN, pRs485ColData->uCol07Data.nEnergyNN, ((MAX_FEE_NUM + 1) * 4));
    
    //存储
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:RealDataMakeAndStore
* 输　入:
* 输　出:
* 返  回:
* 功  能:实时数据转换并存储
****************************************************************************/
INT32S AcRealDataMakeAndStore(pMEM_AC_REAL_DATA pAcRealData)
{
    INT8U               i           = 0;
    pMEM_MP_REAL_DATA   pRealData   = (pMEM_MP_REAL_DATA)(&g_nMemDataBuf[0]);   //加信号量保护
    STD_TIME            sTime;
    MEM_METER_PRM       sMtPrm;
    pMEM_METER_PRM      pMtPrm = &sMtPrm;
    FORMAT11            sFort11;
    
    memset(&sTime, 0, sizeof(sTime));
    memset(pMtPrm, 0, sizeof(sMtPrm));
    memset(&sFort11, 0, sizeof(sFort11));
    memset(pRealData, 0, sizeof(MEM_MP_REAL_DATA));
    
    GetSysClk(&sTime);
    //判断是否为有效测量点
    MemReadMeterPrm(1, F10, pMtPrm, sizeof(MEM_METER_PRM));
    
    if (!IsVldMeterPrm(pMtPrm))
	{
		return RET_ERR_ERROR;
	}
    
    memcpy(&pRealData->nMeterAddr[0],  &pMtPrm->nAddr[0],   6);     //读表时间
    memcpy(&pRealData->nReadTime[0],  &sTime.nSec,   6);            //读表时间
    
    memcpy(&pRealData->nEnergyPP[0], &pAcRealData->sAcEnergy.sPPEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nEnergyNP[0], &pAcRealData->sAcEnergy.sNPEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nEnergyPN[0], &pAcRealData->sAcEnergy.sPNEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nEnergyNN[0], &pAcRealData->sAcEnergy.sNNEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pRealData->nQ1Energy[0], &pAcRealData->sAcQEnergy.sQ1Energy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nQ2Energy[0], &pAcRealData->sAcQEnergy.sQ2Energy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nQ3Energy[0], &pAcRealData->sAcQEnergy.sQ3Energy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pRealData->nQ4Energy[0], &pAcRealData->sAcQEnergy.sQ4Energy[0], ((MAX_FEE_NUM + 1) * 4));

    //电压
    for (i = 0; i < 3; i++)
    {
        memcpy(&pRealData->nVolt[i][0], &pDataTable->sStatAverage.sStatVolt[i], 2);
    }
    
    //电流
    for (i = 0; i < 3; i++)
    {
        memcpy(&pRealData->nCurr[i], &pDataTable->sStatAverage.sStatCurr[i], 3);
    }
    
    //有功功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nPWatt[i], &pDataTable->sStatAverage.sStatP[i], 3);
    }
    
    //无功功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nNWatt[i], &pDataTable->sStatAverage.sStatQ[i], 3);
    }
    
    //视在功率
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nSWatt[i], &pDataTable->sStatAverage.sStatS[i], 3);
    }
    
    //功率因数
    for (i = 0; i < 4; i++)
    {
        memcpy(&pRealData->nFactor[i], &pDataTable->sStatAverage.sStatPf[i], 2);
    }
    
    //存储
    MemWriteRealData(1, FEND, pRealData, sizeof(MEM_MP_REAL_DATA));
    
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:AcRealDataMakeAndStoreProc
* 输　入:
* 输　出:
* 返  回:
* 功  能:实时数据转换并存储
****************************************************************************/
INT32S AcRealDataMakeAndStoreProc(pMEM_AC_REAL_DATA pAcRealData)
{
    INT8U			nErr						= 0;
    
    OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
    AcRealDataMakeAndStore(pAcRealData);
    OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
    
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:AcDayDataMakeAndStore
* 输　入:
* 输　出:
* 返  回:
* 功  能:交采日数据转换并存储
****************************************************************************/
INT32S AcDayDataMakeAndStore(pSTD_TIME pTime)
{
    INT8U               nErr        = 0;
    INT16U              nHistIdx    = 0;
    FORMAT20            sHistTime;
    MEM_METER_PRM       sMtPrm;
    pMEM_METER_PRM      pMtPrm = &sMtPrm;
    pMEM_MP_DAY_DATA    pDayData        = (pMEM_MP_DAY_DATA)(&g_nMemDataBuf[0]);
    pMEM_AC_REAL_DATA   pAcRealData     = (pMEM_AC_REAL_DATA)&nAcTmpBuf[0];
    
    memset(&sMtPrm, 0, sizeof(sMtPrm));
    memset(pDayData, 0, sizeof(MEM_MP_DAY_DATA));
    
    //OSSemPend(g_pSem[SEM_AC_BUF_IDX], 0, &nErr);
    //判断是否为有效测量点
    MemReadMeterPrm(1, F10, pMtPrm, sizeof(MEM_METER_PRM));
    
    if (!IsVldMeterPrm(pMtPrm))
	{
        //OSSemPost(g_pSem[SEM_AC_BUF_IDX]);
		return RET_ERR_ERROR;
	}
    
    if (MemReadAcRealData(FEND, pAcRealData, sizeof(MEM_AC_REAL_DATA)) < 0)
    {
        //OSSemPost(g_pSem[SEM_AC_BUF_IDX]);
        return RET_ERR_ERROR;
    }
    
    memset(&sHistTime, 0, sizeof(sHistTime));
    MemFindHistIdxSub(1, &sHistTime, &nHistIdx,1);
    
    memcpy(&pDayData->nMeterAddr[0],    &pMtPrm->nAddr[0],   LEN_METER_ADDR);          //电表地址(标签)
    memcpy(&pDayData->sDataTime,        &sHistTime,             sizeof(sHistTime));	        //日时标标签
    memcpy(&pDayData->nReadTime,        &pTime->nSec,       6);     //读表时间
    
    memcpy(&pDayData->nDEnergyPP[0], &pAcRealData->sAcEnergy.sPPEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDEnergyNP[0], &pAcRealData->sAcEnergy.sNPEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDEnergyPN[0], &pAcRealData->sAcEnergy.sPNEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDEnergyNN[0], &pAcRealData->sAcEnergy.sNNEnergy[0], ((MAX_FEE_NUM + 1) * 4));
    
    memcpy(&pDayData->nDQ1Energy[0], &pAcRealData->sAcQEnergy.sQ1Energy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDQ2Energy[0], &pAcRealData->sAcQEnergy.sQ2Energy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDQ3Energy[0], &pAcRealData->sAcQEnergy.sQ3Energy[0], ((MAX_FEE_NUM + 1) * 4));
    memcpy(&pDayData->nDQ4Energy[0], &pAcRealData->sAcQEnergy.sQ4Energy[0], ((MAX_FEE_NUM + 1) * 4));
    
    //存储
    MemWriteDayData(1, FEND, 1, pDayData, sizeof(MEM_MP_DAY_DATA));
    //OSSemPost(g_pSem[SEM_AC_BUF_IDX]);
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:AcDayDataMakeAndStoreProc
* 输　入:
* 输　出:
* 返  回:
* 功  能:日数据转换并存储
****************************************************************************/
INT32S AcDayDataMakeAndStoreProc(pSTD_TIME pTime)
{
    INT8U			nErr						= 0;
    
    OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
    AcDayDataMakeAndStore(pTime);
    OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
    
    return RET_ERR_OK;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查日数据是否正常
*********************************************************************************/
INT8S CheckCol2DayData(INT8U nMeterType, pSTD_TIME pMeterTime, pCOL_DATA pRs485ColData)
{
    STD_TIME  sTermTime;

    if ((NULL == pMeterTime) || (NULL == pRs485ColData))
    {
        return RET_ERR_ERROR;
    }
    memset(&sTermTime, 0, sizeof(STD_TIME));
	GetSysClk(&sTermTime);
    
    /*if (RET_INVLD == CheckVldFormat11((pFORMAT11*)&pRs485ColData->uCol07Data.nDEnergyPP[0][0]))
    {
        return RET_ERR_ERROR;
    }*/
    if ((GB_07_PROT == nMeterType) || (GB_97_PROT == nMeterType))
    {
        if ((pMeterTime->nHour > 0) || (pMeterTime->nMin >= 0))
        {
            if (0 != memcmp(&sTermTime.nDay, &pRs485ColData->uCol07Data.nDate[1], 3))
            {
                return RET_ERR_ERROR;
            }
        }
        else
        {
            return RET_ERR_ERROR;
        }
    }
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查月数据是否正常
*********************************************************************************/
INT8S CheckCol2MonData(INT8U nMeterType, pSTD_TIME pMeterTime, pCOL_DATA pRs485ColData)
{
    STD_TIME  sTermTime;

    if ((NULL == pMeterTime) || (NULL == pRs485ColData))
    {
        return RET_ERR_ERROR;
    }
    memset(&sTermTime, 0, sizeof(STD_TIME));
	GetSysClk(&sTermTime);
    
    /*if (RET_INVLD == CheckVldFormat11((pFORMAT11*)&pRs485ColData->uCol07Data.nDEnergyPP[0][0]))
    {
        return RET_ERR_ERROR;
    }*/
    if ((GB_07_PROT == nMeterType) || (GB_97_PROT == nMeterType))
    {
        if ((pMeterTime->nHour > 0) || (pMeterTime->nMin >= 0))
        {
            if (0 != memcmp(&sTermTime.nMon, &pRs485ColData->uCol07Data.nDate[2], 2))
            {
                return RET_ERR_ERROR;
            }
        }
        else
        {
            return RET_ERR_ERROR;
        }
    }
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名: Format11ToFormat14
* 输　入: pData pFORMAT11格式
* 输　出: pBuf  pFORMAT14格式
* 返  回: 0  错误
          sizeof(FORMAT11) + 1  正确
* 功  能: 根据DATA值判断 填充0x0 还是0xEE  （pBuf长度不能小于sizeof(FORMAT11) + 1）
****************************************************************************/
INT8U Format11ToFormat14(const pFORMAT11 pData, INT8U *pBuf)
{
    if ((NULL == pData) || (NULL == pBuf))
    {
        return 0;
    }
    if ((INVLD_DATA == pData->nD1)
        && (INVLD_DATA == pData->nD2)
        && (INVLD_DATA == pData->nD3)
        && (INVLD_DATA == pData->nD4))
    {
        pBuf[0] = INVLD_DATA;
    }
    else
    {
        pBuf[0] = 0x0;
    }
    memcpy(&pBuf[1], pData, sizeof(FORMAT11));
    return (sizeof(FORMAT11) + 1);
}

/****************************************************************************
* 函数名: Format11ToFormat26
* 输　入: pData pFORMAT25格式
* 输　出: pBuf  pFORMAT26格式
* 返  回: 0  错误
          sizeof(FORMAT11) + 1  正确
* 功  能: 根据DATA值判断 填充0x0 还是0xEE  （pBuf长度不能小于sizeof(FORMAT11) + 1）
****************************************************************************/
INT8U Format26ToFormat25(const pFORMAT26 pData, INT8U *pBuf)
{
    if ((NULL == pData) || (NULL == pBuf))
    {
        return 0;
    }
    if ((INVLD_DATA == pData->nD1)
        && (INVLD_DATA == pData->nD2))
    {
        pBuf[0] = INVLD_DATA;
    }
    else
    {
        pBuf[0] = 0x0;
    }
    memcpy(&pBuf[1], pData, sizeof(FORMAT26));
    return (sizeof(FORMAT26) + 1);
}

/********************************************************************************
* 函数名:RdMeterSemProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 置业务标志时发送信号量
*********************************************************************************/
INT8S RdMeterSemProc(void)
{
    OS_SEM_DATA 	    sSemData;
    
    memset(&sSemData, 0, sizeof(sSemData));
    //获取信号量
    OSSemQuery(g_pSem[SEM_RS485_1_IDX], &sSemData);
    if (sSemData.OSCnt == 0)
    {
        OSSemPost(g_pSem[SEM_RS485_1_IDX]); 
    }
    
    memset(&sSemData, 0, sizeof(sSemData));
    OSSemQuery(g_pSem[SEM_RS485_2_IDX], &sSemData);
    if (sSemData.OSCnt == 0)
    {
        OSSemPost(g_pSem[SEM_RS485_2_IDX]); 
    }
    
    memset(&sSemData, 0, sizeof(sSemData));
    OSSemQuery(g_pSem[SEM_CARR_IDX], &sSemData);
    if (sSemData.OSCnt == 0)
    {
        OSSemPost(g_pSem[SEM_CARR_IDX]); 
    }
    
    return 0;
}

/****************************************************************************
* 函数名:RmotCtrlKeepPowerProc
* 输　入:
* 输　出:
* 返  回:
* 功  能:
****************************************************************************/
INT32S  RmotCtrlKeepPowerProc(pSTD_TIME pTime)
{
    INT32S          nRet        = 0;
    INT32U          nBaseSecs   = 0;
    INT32U          nSecs       = 0;
    INT32U          nKeepSecs   = 0;
    KEEP_POWER_PRM  sKeepPowerPrm;
    
    MemReadCtrlPrm(F25, 0, &sKeepPowerPrm, sizeof(sKeepPowerPrm));
    if (sKeepPowerPrm.nKeepPowerFlg == 0x55)
    {
        if (sKeepPowerPrm.nKeepPowerTime == 0)
        {
             return 0;
        }
        //判断时间
        nBaseSecs = GetSecsFrom2KYear(&sKeepPowerPrm.sKeepStartTime);
        nSecs = GetSecsFrom2KYear(pTime);
        nKeepSecs = sKeepPowerPrm.nKeepPowerTime * 1800;
        if (nSecs > nBaseSecs + nKeepSecs)
        {
            return -1;
        }
        return 0;
    }
    return -1;
}

/****************************************************************************
* 函数名:RmotCtrlProcSub
* 输　入:
* 输　出:
* 返  回:
* 功  能:
****************************************************************************/
void RmotCtrlProcSub(pSTD_TIME pTime, INT8U nTrun)
{
    INT8U   nTripFlg    = 0;
    INT8U   nLockFlg    = 0;
    
    MemReadRamXmpData(F6, nTrun, &nTripFlg, 1);
    MemReadRamXmpData(F7, nTrun, &nLockFlg, 1);
    if (nLockFlg)
    {
        nLockFlg = 0;
        MemWriteRamXmpData(F7, nTrun, &nLockFlg, 1);
        //合闸
        RcSetCtrl(pTime,0, nTrun);
    }
    if (nTripFlg)
    {
        nTripFlg = 0;
        MemWriteRamXmpData(F6, nTrun, &nTripFlg, 1);
        //跳闸
        if (RmotCtrlKeepPowerProc(pTime) == 0)          //保电状态
        {
            return;
        }
        RcSetCtrl(pTime, 1, nTrun);
    }
}

INT8U   g_nTurnSwitckCnt[MAX_TURN_NUM]    = {0};
INT8U   g_nTripFistFlg[MAX_TURN_NUM]      = {0};
INT8U   g_nCycelCnt[MAX_TURN_NUM]         = {0};
/****************************************************************************
* 函数名:RmotCtrlProc
* 输　入:
* 输　出:
* 返  回:
* 功  能:遥控处理
****************************************************************************/
void RmotCtrlProc(void)
{
    CTRL_TURN_PRM   sCtrlPrm;
    STD_TIME        sBaseTime;
    STD_TIME        sStartTime;
    STD_TIME        sEndTime;
    STD_TIME 		sTermTime;
    pSTD_TIME       pTime = &sTermTime;
    INT8U           nFlg        = 1;
    INT32S          nRet        = 0;
    INT32U          nSecs       = 0;
    INT32U          nBaseSecs   = 0;
    INT32U          nStartSecs  = 0;
    INT32U          nEndSecs    = 0;
    INT8U           nCtrlFlg    = 0;
    INT8U           i           = 0;
    
    if (g_nRemotCtrlFlg)
    {
        return;
    }
    
    if (g_nCarrZbSelFlg == TYPE_CARR)
    {
        return;
    }
    
    memset(&sTermTime, 0, sizeof(STD_TIME));
    GetSysClk(&sTermTime);
    
    for (i = 0; i < MAX_TURN_NUM; i++)
    {
        memset(&sCtrlPrm, 0, sizeof(sCtrlPrm));
        memset(&sBaseTime, 0, sizeof(sBaseTime));
        memset(&sStartTime, 0, sizeof(sStartTime));
        memset(&sEndTime, 0, sizeof(sEndTime));
    
        MemReadCtrlPrm(F1, i, &sCtrlPrm, sizeof(sCtrlPrm));//第i + 1轮控制
        memcpy(&sBaseTime.nSec, &sCtrlPrm.sCtrlCmdSetTime, 6);
        
        if (!IsVldStdTime(&sBaseTime))
        {
            //告警时刻为无效时，置合闸标志，退出
            MemWriteRamXmpData(F7, i, &nFlg, 1);
            continue;
        }
        nSecs = GetSecsFrom2KYear(pTime);
        nBaseSecs = GetSecsFrom2KYear(&sBaseTime);
        nStartSecs = nBaseSecs + sCtrlPrm.nAlmMin * 60;
        nEndSecs = nStartSecs + sCtrlPrm.nAstPower30Min * 1800;
        if ((nSecs >= nStartSecs) && (nSecs <= nEndSecs))
        {
            if (g_nTripFistFlg[i] == 0)
            {
                g_nTripFistFlg[i] = 1;
                //置跳闸标志
                MemWriteRamXmpData(F6, i, &nFlg, 1);
            }
            else
            {
                g_nTurnSwitckCnt[i]++;
                if (g_nTurnSwitckCnt[i] > 2)
                {
                    g_nTurnSwitckCnt[i] = 0;
                    //置合闸标志
                    MemWriteRamXmpData(F7, i, &nFlg, 1);
                }
            }
            g_nCycelCnt[i]++;
            if (g_nCycelCnt[i] > 120)      //两分钟后再次置跳闸标志
            {
                g_nCycelCnt[i] = 0;
                g_nTripFistFlg[i] = 0;
                //置跳闸标志
                MemWriteRamXmpData(F6, i, &nFlg, 1);
            } 
        }
        else
        {
            g_nCycelCnt[i] = 0;
            //置合闸标志
            MemWriteRamXmpData(F7, i, &nFlg, 1);
        }
        RmotCtrlProcSub(pTime, i);
    }
    
}

INT8U   g_nCtrlInfo[2]      = {0};
/****************************************************************************
* 函数名:RcSetCtrl
* 输　入:
* 输　出:
* 返  回:
* 功  能:遥控
****************************************************************************/
void RcSetCtrl(pSTD_TIME pTime, INT8U nType, INT8U nTurns)
{
    INT8U       i           = 0;
    INT8U       nTmpBuf[20] ={0};
    INT8U       nTmpLen     = 0;
    INT8U       nAddr[6]    ={0};
    INT8U       nCtrlInfo[2]    ={0};
    
    memset(&nAddr, 0xAA, sizeof(nAddr));
    memset(&nTmpBuf, 0, sizeof(nTmpBuf));
    
    SetCommCfg(4, 6, 0);            //COMM_ZB专变，9600，无校验
    
    if (nType)//跳闸
    {
        if (nTurns == 0)
        {
            SetBufBit(&g_nCtrlInfo[0], 2, TURN1_ID);
            SetBufBit(&g_nCtrlInfo[0], 2, TX1_ID);
        }
        else if (nTurns == 1)
        {
            SetBufBit(&g_nCtrlInfo[0], 2, TURN2_ID);
            SetBufBit(&g_nCtrlInfo[0], 2, TX2_ID);
        }
        //生成跳闸事件记录
        RcEvtProc(pTime, nTurns);
    }
    else    //合闸
    {
        if (nTurns == 0)
        {
            ClrBufBit(&g_nCtrlInfo[0], 2, TURN1_ID);
            ClrBufBit(&g_nCtrlInfo[0], 2, TX1_ID);
        }
        else if (nTurns == 1)
        {
            ClrBufBit(&g_nCtrlInfo[0], 2, TURN2_ID);
            ClrBufBit(&g_nCtrlInfo[0], 2, TX2_ID);
        }
    }
    
    for (i = 0; i < 2; i++)
    {
        nCtrlInfo[i] = g_nCtrlInfo[i] + 0x33;
    }
    
    memcpy(&nTmpBuf[0], &nCtrlInfo[0], 2);
    nTmpLen = Meter645MakeFrm(&nTmpBuf[0], 2, sizeof(nTmpBuf), &nAddr[0], 0x15);
    nTmpLen = Meter645Proc(COMM_ZB, &nTmpBuf[0], nTmpLen, sizeof(nTmpBuf));
}

INT8S RcEvtMemInfoProc(void)
{
	EVT_MEM_INFO		sEvtInfo;

	memset(&sEvtInfo, 0, sizeof(sEvtInfo));
    
    if (RET_ERR_OK != MemReadDbfExt(MEM_OBJ_X_FLH_DATA, 0, F6, &sEvtInfo, sizeof(EVT_MEM_INFO), NULL))
    {
        return RET_ERR_ERROR;
    }
    if (sEvtInfo.nNext == 0xFF)
    {
        sEvtInfo.nNext = 0;
    }
    if (sEvtInfo.nMemNum == 0xFF)
    {
        sEvtInfo.nMemNum = 0;
    }
    sEvtInfo.nNext = (sEvtInfo.nNext + 1) % MAX_RC_EVT_NUM;
    (sEvtInfo.nMemNum < MAX_RC_EVT_NUM) ? (sEvtInfo.nMemNum++) : (sEvtInfo.nMemNum = MAX_RC_EVT_NUM);
    MemWriteDbfExt(MEM_OBJ_X_FLH_DATA, 0, F6, &sEvtInfo, sizeof(EVT_MEM_INFO), NULL);
	return 0;
}

//跳闸事件记录
INT32S  RcEvtProc(pSTD_TIME pTime, INT8U nTurns)
{
    RC_EVT_DATA     sRcEvtData;
    EVT_MEM_INFO	sEvtInfo;
    INT8U           nIdx        = 0;
    
    memset(&sRcEvtData, 0, sizeof(sRcEvtData));
    memset(&sEvtInfo, 0, sizeof(sEvtInfo));
   
    MemReadDbfExt(MEM_OBJ_X_FLH_DATA, 0, F6, &sEvtInfo, sizeof(EVT_MEM_INFO), NULL);
    if (sEvtInfo.nNext == 0xFF)
    {
        sEvtInfo.nNext = 0;
    }
    if (sEvtInfo.nMemNum == 0xFF)
    {
        sEvtInfo.nMemNum = 0;
    }
    nIdx = sEvtInfo.nNext;
    sRcEvtData.nTurn = nTurns;
    memcpy(&sRcEvtData.sTime.nSec, &pTime->nSec, 6);
    MemWriteFlhXmpData(F7, nIdx, (INT8U*)&sRcEvtData, sizeof(RC_EVT_DATA));
    //memset(&sRcEvtData, 0, sizeof(sRcEvtData));
    //MemReadFlhXmpData(F7, nIdx, (INT8U*)&sRcEvtData, sizeof(RC_EVT_DATA));
    
    //偏移
    RcEvtMemInfoProc();
}


/****************************************************************************
* 函数名:FunCalcSinglePhaseSWatt
* 输　入:pPWatt 有功功率
*        pQWatt 无功功率
* 输　出:pSWatt 视在功率
* 返  回:
* 功  能:由P和Q计算S(视在功率)
****************************************************************************/
INT8S FunCalcSinglePhaseSWatt(const pFORMAT09 pPWatt, const pFORMAT09 pQWatt, pFORMAT09 pSWatt)
{
    FP32 fPVal = 0;
    FP32 fQVal = 0;
    FP32 fSVal = 0;

    if ((NULL == pPWatt) || (NULL == pQWatt) || (NULL == pSWatt))
    {
        return RET_ERR_ERROR;
    }

    fPVal = Format092Float(pPWatt);
    fQVal = Format092Float(pQWatt);
    fSVal = fPVal * fPVal + fQVal * fQVal;
    fSVal = sqrt(fSVal);

    if (RET_ERR_ERROR == Float2Format09(fSVal, pSWatt))
    {
        return RET_ERR_ERROR;
    }
    return RET_ERR_OK;
}


/****************************************************************************
                                故障类处理
****************************************************************************/

/****************************************************************************
* 函数名:DispBlackCtrl
* 输　入:
* 输　出:
* 返  回:
* 功  能:黑屏
****************************************************************************/
void DispBlackCtrl(void)
{
    LcdInit();
    LCD_BG_PDOR = 0;
}

/****************************************************************************
* 函数名:DispWhilteCtrl
* 输　入:
* 输　出:
* 返  回:
* 功  能:白屏
****************************************************************************/
void DispWhilteCtrl(void)
{
    g_nDispErr = 1; //按键不响应
    LcdInit();
    LCD_BG_PDOR = 1;
}

void DispRecCtrl(void)
{
    g_nDispErr = 0; //按键不响应
    //LcdInit();
    LCD_BG_PDOR = 1;
}

//GPRS电源等不亮
void GprsPowerOff(void)
{
    g_nGprsLedFlg = 1;
    //GpioClearOutput(IO_PORT5, IO_PINx9);                    //灭
}

/****************************************************************************
* 函数名:GprsLightFlash
* 输　入:
* 输　出:
* 返  回:
* 功  能:GPRS模块通讯指示灯
****************************************************************************/
//GPRS模块通讯指示灯不闪烁
void GprsLightUnflash(void)
{
    g_nGprsLedFlg = 1;
     //GpioClearOutput(IO_PORT5, IO_PINx9);                    //灭
    //INT8U   i = 0;
    
    /*if (SendAtCmdAndWaitATRespSlip((char*)"AT#LED=1", (char*)g_Modl_OK, 1000, 1) < 0)
	{
		//i = 0;	
	}
    if (SendAtCmdAndWaitATRespSlip((char*)"AT#LED=2", (char*)g_Modl_OK, 1000, 1) < 0)
	{
		//i = 0;	
	}
    if (SendAtCmdAndWaitATRespSlip((char*)"AT#LED=3", (char*)g_Modl_OK, 1000, 1) < 0)
	{
		//i = 0;
	}*/
}

//GPRS模块通讯指示灯闪烁
void GprsLightFlash(void)
{
    g_nGprsLedFlg = 0;
    //GpioSetOutput(IO_PORT5, IO_PINx9);                    //亮
}

/****************************************************************************
* 函数名:GprsCommNormal
* 输　入:
* 输　出:
* 返  回:
* 功  能:GPRS模块信号强度
****************************************************************************/
//GPRS模块不能通信
void GprsCommAbnormal(void)
{
    g_nSignalErrFlg = 1;
}

//GPRS通信正常
void GprsCommNormal(void)
{
    g_nSignalErrFlg = 0;
}

/****************************************************************************
* 函数名:GprsPowerOn
* 输　入:
* 输　出:
* 返  回:
* 功  能:终端通信
****************************************************************************/
//不能正常上线
void TermOffline(void)
{

}

//正常上线
void TermOnline(void)
{

}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:SIM卡
****************************************************************************/
//SIM卡不能操作控制
void SimAbnormal(void)
{

}

//SIM卡正常运行
void SimNormal(void)
{

}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:载波模块的工作状态
****************************************************************************/
//载波模块不运行
void CarrModeNwork(void)
{
    g_nCarrErrFlg = 1;
}

//载波模块正常运行
void CarrModework(void)
{
    g_nCarrErrFlg = 0;
}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:载波模块表计参数
****************************************************************************/
//表计不能载波抄读
void CarrCopyAbnRd(void)
{
    g_nCarrErrFlg = 1; 
}

//正常抄读
void CarrCopyRd(void)
{
    g_nCarrErrFlg = 0;
}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:终端死机
****************************************************************************/
//黑屏
void BlackScreen(void)
{
    LcdInit();
    LCD_BG_PDOR = 0;
}
//白屏
void WhiteScreen(void)
{
    g_nDispErr = 1; //按键不响应
    LcdInit();
    LCD_BG_PDOR = 1;
}
//按键不响应
void Buttonrespond(void)
{
    g_nDispErr = 1; //按键不响应
}
//指示灯异常
void LightAbnormal(void)
{ 
    GprsPowerOff();
}
//终端通讯不响应
void TermCommNrespond(void)
{

}
//恢复
void Restore(void)
{
    g_nDispErr = 0;
    LcdInit();
    LCD_BG_PDOR = 1;
    
    GprsPowerOn();
}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:终端电源故障
****************************************************************************/
//专变终端无电
void TremNoPower(void)
{

}
//显示板无电
void ScreenNoPower(void)
{

}
//I/O接口板无电
void InterfaceNoPower(void)
{

}
//12v输出故障
void TwelveVoltFault(void)
{

}
//5v输出故障
void FiveVoltFault(void)
{

}
//恢复
void Recover(void)
{

}

/****************************************************************************
* 函数名:GprsLightFlash
* 输　入:
* 输　出:
* 返  回:
* 功  能:电台故障
****************************************************************************/
//终端有接收信号，无发射信号
void RadioAbnormal(void)
{

}

//电台正常运行
void RadioRunNormal(void)
{

}


/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:主板与接口板交互故障
****************************************************************************/
//与显示板通信故障
void ScreenFault(void)
{

}

//与锚板通信故障
void AnchorFault(void)
{

}

//无故障
void InterfaceNoFault(void)
{

}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:脉冲口故障
****************************************************************************/
//短路
void PulseMouthFault(void)
{

}

//脉冲口状态
void PulseState(void)
{

}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:485端口故障
****************************************************************************/
//短路
void PortFault(void)
{
    g_nRs485ErrFlg = 1;
}

//开路
void PortOpenCircuit(void)
{
    g_nRs485ErrFlg = 1;
}

//默认状态
void PortState(void)
{
    g_nRs485ErrFlg = 0;
}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:遥控端口故障
****************************************************************************/
//短路
void RemoteFault(void)
{
    g_nRemotCtrlFlg = 1;
}

//开路
void RemoteOpenCircuit(void)
{
    g_nRemotCtrlFlg = 1;
}

//默认状态
void RemoteState(void)
{
    g_nRemotCtrlFlg = 0;
}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:遥信端口故障
****************************************************************************/
//短路
void ShakeFault(void)
{

}

//开路
void ShakeOpenCircuit(void)
{

}

//默认状态
void ShakeState(void)
{

}
/****************************************************************************
* 函数名:GprsPowerOn
* 输　入:
* 输　出:
* 返  回:
* 功  能:GPRS电源等亮
****************************************************************************/
void GprsPowerOn(void)
{
    g_nGprsLedFlg = 0;
    //GpioSetOutput(IO_PORT5, IO_PINx9);                    //亮
}



/**************************************************************************************************
                                           载波模块
***************************************************************************************************/


/**************************************************************************************************
                                           测试工具手持设备模块
***************************************************************************************************/
#if ASSIST_RUN == ASSIST_RUN_FZZD
void GpioConfigUp( INT8U port, INT32U pin, INT8U mode )       //配置IO为输入或者输出;首先将IO配置为GPIO
{                                                           //mode :GPIO_MODE_INPUT;   GPIO_MODE_OUTPUT
	IO_ConfigPort(port, pin, IO_GPIO_MODE);             
	IO_ConfigGPIOPin(port, pin, mode, IO_PULLUP_ON);
}
/*
*****************************************************************************
* 函数名: GpioModlCfg
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*****************************************************************************
*/
void GpioModlCfg(void)
{ 
	FM3_GPIO->ADE = 0x0000;
    
	//继电器控制
	GpioConfig(IO_PORT7, IO_PINx6, GPIO_MODE_OUTPUT);	    //继电器控制逆序接入
	GpioConfig(IO_PORT7, IO_PINx7, GPIO_MODE_OUTPUT);	    //继电器控制正序接入
	GpioConfig(IO_PORT7, IO_PINx8, GPIO_MODE_OUTPUT);	    //继电器控制C切入
	GpioConfig(IO_PORT7, IO_PINx9, GPIO_MODE_OUTPUT);	    //继电器控制B切入
	GpioConfig(IO_PORT7, IO_PINxA, GPIO_MODE_OUTPUT);	    //继电器控制A切入
    
	GpioSetOutput(IO_PORT7, IO_PINx6);                      //初始为高电平状态
	GpioSetOutput(IO_PORT7, IO_PINx7);	                    //初始为高电平状态
	GpioSetOutput(IO_PORT7, IO_PINx8);	                    //初始为高电平状态
	GpioSetOutput(IO_PORT7, IO_PINx9);	                    //初始为高电平状态
	GpioSetOutput(IO_PORT7, IO_PINxA);	                    //初始为高电平状态

	//3.
	GpioConfigUp(IO_PORTC, GPIO_ALL_PIN, GPIO_MODE_OUTPUT);	//LCD DATA
	GpioConfigUp(IO_PORTD, IO_PINx1, GPIO_MODE_OUTPUT);	    //E_TXD1
	GpioConfigUp(IO_PORTD, IO_PINx2, GPIO_MODE_OUTPUT);	    //E_TXD0
	GpioConfigUp(IO_PORTD, IO_PINx3, GPIO_MODE_OUTPUT);	    //E_TXEN

	GpioSetOutput(IO_PORTD, IO_PINx1);	                    //初始为高电平状态
	GpioSetOutput(IO_PORTD, IO_PINx2);	                    //初始为高电平状态
	GpioSetOutput(IO_PORTD, IO_PINx3);	                    //初始为高电平状态

	//4.串口
	GpioConfigUp(IO_PORT1, IO_PINx1, GPIO_MODE_OUTPUT);	    //USART1_RX
	GpioConfigUp(IO_PORT1, IO_PINx2, GPIO_MODE_OUTPUT);	    //USART1_TX
	GpioConfigUp(IO_PORT1, IO_PINx4, GPIO_MODE_OUTPUT);	    //USART0_RX
	GpioConfigUp(IO_PORT1, IO_PINx5, GPIO_MODE_OUTPUT);	    //USART0_TX
	GpioConfigUp(IO_PORT1, IO_PINx6, GPIO_MODE_INPUT);	    //I2C2_SCL
	GpioConfig(IO_PORT1, IO_PINx9, GPIO_MODE_INPUT);	    //SP2_CS 
	GpioConfigUp(IO_PORT1, IO_PINx7, GPIO_MODE_OUTPUT);	    //I2C2_SDA

	GpioSetOutput(IO_PORT1, IO_PINx1);	                    //初始为高电平状态
	GpioSetOutput(IO_PORT1, IO_PINx2);	                    //初始为高电平状态
	GpioSetOutput(IO_PORT1, IO_PINx4);	                    //初始为高电平状态
	GpioSetOutput(IO_PORT1, IO_PINx5);	                    //初始为高电平状态
	GpioSetOutput(IO_PORT1, IO_PINx7);	                    //初始为低电平状态

    GpioConfigUp(IO_PORT5, IO_PINx9, GPIO_MODE_INPUT);	    //连接按钮
    GpioConfigUp(IO_PORT5, IO_PINxA, GPIO_MODE_INPUT);	    //断开按钮
    //7.蜂鸣器
    GpioConfigEX(0x5B, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    
    //6.FLASH
    GpioConfigEX(0xA5, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    SET_IO(0xA5, 0);
    
    //接线检查
    GpioConfigEX(0x40, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x41, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x42, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x43, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x44, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x49, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x4A, GPIO_MODE_INPUT, IO_PULLUP_ON );

    
    //9.电池供电引脚
    GpioConfigEX(0x21, GPIO_MODE_OUTPUT, IO_PULLUP_ON ); 
    SET_IO(0x21, 1);
    
    GpioConfigEX(0x1E, GPIO_MODE_INPUT, IO_PULLUP_ON ); 
    
    GpioConfigEX(0x20, GPIO_MODE_OUTPUT, IO_PULLUP_ON ); 
    SET_IO(0x20, 1);
}
#endif



//电能量异常
INT8S  PowerDataFault(INT8U     nType)
{
     INT8S  nRet = e_Suc;
     
    if( 0 == nType)
    {
        
    }
    else
    {
        nRet = e_NoObject;
    }
    return  nRet;
}

//需量异常
INT8S  DemandFault(INT8U     nType)
{
     INT8S  nRet = e_Suc;
    if( 0 == nType)
    {
    }
    else
    {
        nRet = e_NoObject;
    }
    return  nRet;
}




/*
*****************************************************************************
* 函数名: 
* 输　入: 
*		INT8U* 				pAddr					表地址
*		INT8U* 				pBuf					接收的缓冲区
*		INT16U 				nBufsize				缓冲区大小
* 输  出: 
*		INT8U* 				pBuf					接收的缓冲区
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 普通缓冲区(非循环缓冲)寻找645帧: 有完整帧，返回0成功，否则-1
*****************************************************************************
*/
INT32S DwinCheckPackage(INT8U* pAddr, INT8U* pBuf, INT16U nBufsize)
{
	INT16U		nHeadLen		= STR_OFF(pDB_LINK_PROT, nData);
	INT16U 		i        		= 0;
    INT8U* 		pData    		= pBuf;
    INT8U   	nCs     		= 0;
	INT16U		nDataLen		= 0;
	
    if ((NULL == pBuf))
    {
        return -1;
    }
    if (nBufsize < nHeadLen + 2)
    {
        return -1;
    }

    for (i = 0; i < nBufsize - (nHeadLen + 2 - 1); i++)
    {
		pData = &pBuf[i];							// 
		// 1. 报文头
		if (pData[0] != 0x68)
        {
            continue;
        }
		// 2. 6个字节地址
		if (NULL != pAddr)
		{
	        if (0 != memcmp(pAddr, &pData[1], 6))
	        {
	            continue;
	        }
		}
		// 3. 第二个68
        if (pData[7] != 0x68)
        {
            continue;
        }
		// 4. 控制码

		// 5. 数据域长度
        nDataLen = pData[9];
        if (nDataLen > nBufsize - nHeadLen - 2 - i)
        {
            continue;
        }

		// 6. CS
        nCs = CheckSum(&pBuf[0], i, nDataLen + nHeadLen);
        if (nCs != pData[nDataLen + nHeadLen])
        {
            continue;
        }

		// 7. 报文尾
        if (0x16 != pData[nDataLen + nHeadLen + 1])
        {
            continue;
        }

		nDataLen = nDataLen + nHeadLen + 2;
		memmove(&pBuf[0], &pData[0], nDataLen);
        return (INT32S)nDataLen;
    }

    return -1;
}


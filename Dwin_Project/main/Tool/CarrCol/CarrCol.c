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
#include <stdio.h>
#include <string.h>
#include "Base.h"
#include "CarrCol.h"
#include "DataItem.h"
#include "Gb07.h"
#include "Meter645.h"
#include "Gw3762.h"
#include "SysRes.h"
#include "Fun.h"
#include "MemDrvExt.h"
#include "GB3761SetPrmExt.h"


/**************************************************************************************************
                                           �궨��
***************************************************************************************************/
		
/**************************************************************************************************
										   ��������
***************************************************************************************************/
// �ز����ݽṹ
static COL_DATA 					g_sCarrColData;
// �ز����ݽṹ:
INT8U						g_nCarrModType = 0;

INT8U   g_nCarrZbSelFlg         = TYPE_CARR;

//����3762ʱʹ���ز�����С����
INT8U   g_nModl                = GW3762_CARR;

INT8U   g_nCarrNodeRef          = 0;

INT8U   g_nNodeAddFlg           = 0;

INT8U   g_nCarrInitFlg          = 0;        //�ȳ�ʼ������������ò���

//ģ���ʼ����Ϣ
const CARR_MODE_INFO g_sCarrModeInfo[] = {
	//ģ������	����ģʽnReadType		�·��ڵ㵽·��ģ��nSetNodeRouter	�·��ɼ�����ַ�����ַnSetColNode
	//			(0x55������,0xaaģ����) (0x55�·� 0xaa���·�)				(0x55Ϊ�ɼ�����ַ 0xaaΪ����ַ)?
	{CARR_MOD_XC, 		0x55,					0x55,								0xAA,			0x13, 9600},//����
	{CARR_MOD_DR, 		0xAA,					0x55,								0xAA,			0x13, 9600},//����
	{CARR_MOD_DX, 		0xAA,					0x55,								0xAA,			0x13, 9600},//����
	{CARR_MOD_LHW, 		0xAA,					0x55,								0xAA,			0x13, 2400},//����΢
	{CARR_MOD_RSK, 		0x55,					0xAA,								0xAA,			0x02, 9600},//��˹��
	{CARR_MOD_XN, 		0x55,					0xAA,								0xAA,			0x02, 2400},//Ѷ��
	{CARR_MOD_GY, 		0xAA,					0x55,								0xAA,			0x13, 9600},//��һ����
    {CARR_MOD_YXD, 		0x55,					0x55,								0x55,			0x02, 9600},//��Ѷ������
    {CARR_MOD_0FDM, 	0x55,					0x55,								0x55,			0x02, 9600},//����ز�
    
	{0xFF},
};
	
	
/**************************************************************************************************
										   ��غ���ʵ��
***************************************************************************************************/

/*
*****************************************************************************
* ������: CarrModeInfoGet
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
*****************************************************************************
*/
static pCARR_MODE_INFO	CarrModeInfoGet(INT8U nModeType)
{
	pCARR_MODE_INFO pCarrModeInfo = (pCARR_MODE_INFO)(&g_sCarrModeInfo[0]);

	while(1)
	{
		if (0xFF == pCarrModeInfo->nModeType)
		{
			return NULL;
		}
		if (nModeType == pCarrModeInfo->nModeType)
		{
			return pCarrModeInfo;
		}
		pCarrModeInfo++;
	}
}
	
/*
*****************************************************************************
* ������: CarrModlSel
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ���ݲ�ͬ��ģ������ѡ�����ò�����
*****************************************************************************
*/
static INT32S	CarrModlSel(INT8U nModeType)
{
	pCARR_MODE_INFO pCarrModeInfo = NULL;

	pCarrModeInfo = CarrModeInfoGet(nModeType);
	if (NULL == pCarrModeInfo)
	{
		return -1;
	}
	SetComm(COMM_CARR_RCV_DEBUG, pCarrModeInfo->nBaud, EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);
	return 0;
}

/*
*****************************************************************************
* ������: GetCarrModlAfn
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ���ݲ�ͬ��ģ������ѡ������AFN
*****************************************************************************
*/
static INT32S	GetCarrModlAfn(INT8U nModeType)
{
	pCARR_MODE_INFO pCarrModeInfo = NULL;

	pCarrModeInfo = CarrModeInfoGet(nModeType);
	if (NULL == pCarrModeInfo)
	{
		return -1;
	}
	return pCarrModeInfo->nTranAfn;
}
	
/*
*****************************************************************************
* ������: CarrMainAddr
* �䡡��: 
*				INT8U* 			pAddr
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ���õ�ַ·�ɲ���
*****************************************************************************
*/
static INT32S CarrMainAddr(INT8U* pAddr)
{
	INT8U	nBuf[100]	= {0};
	INT8U*	pBuf		= &nBuf[0];
	INT32S	nLen		= 0;

	if (NULL == pAddr)
	{
		memcpy(&pBuf[nLen], pAddr, 6);
	}
	else
	{
		memset(&pBuf[nLen], 0, 6);
	}
	nLen += 6;
	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x05, F1);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));

	return nLen;
}
	
/*
*****************************************************************************
* ������: CarrReg
* �䡡��: 
*			INT8U nFlg					1ע�ᣬ0ֹͣ
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
*****************************************************************************
*/
static INT32S CarrReg(INT8U nFlg)
{
	INT8U	nBuf[100]	= {0};
	INT8U*	pBuf		= &nBuf[0];
	INT32S	nLen		= 0;
	INT16U	nFn 		= F5;

	if (!nFlg)
	{
		nFn = F6;
	}

	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x11, nFn);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));

	return nLen;
}

/*
*****************************************************************************
* ������: CarrStopRdMeter
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
*****************************************************************************
*/
static INT32S CarrStopRdMeter(void)
{
	INT8U	nBuf[100]	= {0};
	INT8U*	pBuf		= &nBuf[0];
	INT32S	nLen		= 0;

	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x12, F2);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));

	return nLen;
}

/*
*****************************************************************************
* ������: CarrRouteInit
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
*****************************************************************************
*/
INT32S CarrRouteInit(void)
{
	INT8U	nBuf[100]	= {0};
	INT8U*	pBuf		= &nBuf[0];
	INT32S	nLen		= 0;

	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x01, F2);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));

	return nLen;
}

/*
*****************************************************************************
* ������: ReadCarrVer
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ��ȡ�汾��
*****************************************************************************
*/
static INT32S ReadCarrVer(void)
{
    INT8U	nBuf[100]	= {0};
	INT8U*	pBuf		= &nBuf[0];
	INT32S	nLen		= 0;

	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x03, F1);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));
    
    if (nLen > 0)
    {
        if ((pBuf[13] == 'C') && (pBuf[14] == 'F') && (pBuf[15] == 'M') && (pBuf[16] == 'J'))
        {
            g_nCarrModType = CARR_MOD_YXD;
            g_nModl = GW3762_WLAN;
        }
        else if ((pBuf[13] == 'H') && (pBuf[14] == 'R') && (pBuf[15] == '0') && (pBuf[16] == '0'))
        {
            g_nCarrModType = CARR_MOD_0FDM;
            g_nModl = GW3762_CARR;
        }
        else
        {
            g_nCarrModType = CARR_MOD_DX;
            g_nModl = GW3762_CARR;
        }
    }

	return nLen;
}



/*
*****************************************************************************
* ������: CarrRouteDel
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
*****************************************************************************
*/
static INT32S CarrRouteDel(INT8U* pAddr)
{
	INT8U	nBuf[100]	= {0};
	INT8U*	pBuf		= &nBuf[0];
	INT32S	nLen		= 0;

	pBuf[nLen++] = 1;
	if (NULL != pAddr)
	{
		memcpy(&pBuf[nLen], pAddr, 6);
	}
	else
	{
		memset(&pBuf[nLen], 0, 6);
	}
	nLen += 6;
	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x11, F2);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));

	return nLen;
}

/*
*****************************************************************************
* ������: CarrRouteAdd
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ����·��
*****************************************************************************
*/
INT32S CarrRouteAdd(INT8U* pAddr, INT8U nProt)
{
	INT8U			nBuf[100]		= {0};
	INT8U*			pBuf			= &nBuf[0];
	INT32S			nLen			= 0;

	if (NULL == pAddr)
	{
		return -1;
	}
		
	nLen = CarrRouteDel(pAddr);
	if (nLen < 0)
	{
		return -1;
	}

	nLen = 0;
	pBuf[nLen++] = 1;
	if (NULL != pAddr)
	{
		memcpy(&pBuf[nLen], pAddr, 6);
	}
	else
	{
		memset(&pBuf[nLen], 0, 6);
	}
	nLen += 6;
	pBuf[nLen++] = nProt;
	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x11, F1);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));
    
    if (nLen > 0)
    {
        if (pBuf[11] == 1)
        {
            g_nNodeAddFlg = 1;
        }
    }

	return nLen;
}


#if 0
#endif
/********************************************************************************
* ������:CarrRdCmdProc
* �䡡��:
*			
* �䡡��:
* ��  ��:
			0, ����ɹ�
			-1,����ʧ��
			1,�˳�����
* ��  ��:����
*********************************************************************************/
// static ��
INT32S CarrRdCmdProc(INT8U nCom, INT8U nProt, INT8U* pAddr, INT32U nDi, INT8U* pData, INT32S nSize)
{
#if 0
	INT8U				nCtrl					= 0;
	INT32S				i						= 0;
	INT8U				nDiLen					= 0;
	INT32S				nLen					= 0;
	INT8U				nAfn					= 0x13;
	pDB_LINK_PROT		pDbLinkProt			= NULL;

	nCom = nCom;
	if (GB_97_PROT == nProt)
	{
		nCtrl = 0x01;
		nDiLen = 2;
	}
	else
	{
		nCtrl = 0x11;
		nDiLen = 4;
	}
	memset(&pData[0], 0, nSize);
	Long2Buf(pData, nDi, nDiLen);
    for (i = 0; i < nDiLen; i++)
    {
        pData[i] += 0x33;
    }
	nLen = nDiLen;
	nLen = Meter645MakeFrm(pData, nLen, nSize, pAddr, nCtrl);
	nAfn = GetCarrModlAfn(g_nCarrModType);
	if (0x13 == nAfn)
	{
		memmove(&pData[1], &pData[0], nLen);
		nLen++;
		pData[0] = 0;
	}
	memmove(&pData[1], &pData[0], nLen);
	nLen++;
	if (GB_97_PROT == nProt)
	{
		pData[0] = 1;
	}
	else
	{
		pData[0] = 2;
	}
	nLen = Gb3762ProtMakeFrm(pAddr, pData, nLen, nSize, GW3762_DOWN_FRM, GW3762_START_PRM, 
									g_nModl, GW3762_METER_NODE, 0, nAfn, F1);
	nLen = Gw3762Proc(pData, nLen, nSize);
	nLen = Meter645CheckPackage(pAddr, pData, nLen);    
	if (nLen < 12)
	{
		return -1;
	}
	pDbLinkProt = (pDB_LINK_PROT)pData;
	if((pDbLinkProt->nCtrl & 0x80) != 0x80)
    {
        return -1;
    }
    if((pDbLinkProt->nCtrl & 0x40) == 0x40)
    {
        return -1;
    }

	return nLen;
#else
    
    INT8U				nCtrl					= 0;
	INT32S				i						= 0;
	INT8U				nDiLen					= 0;
	INT32S				nLen					= 0;
    
    //SetCommCfg(3, 4, 2);

	if (GB_97_PROT == nProt)
	{
		nCtrl = 0x01;
		nDiLen = 2;
	}
	else
	{
		nCtrl = 0x11;
		nDiLen = 4;
	}
	// ���峭��
	memset(&pData[0], 0, nSize);
	Long2Buf(pData, nDi, nDiLen);
    for (i = 0; i < nDiLen; i++)
    {
        pData[i] += 0x33;
    }
	nLen = nDiLen;
	nLen = Meter645MakeFrm(pData, nLen, nSize, pAddr, nCtrl);
    //nLen = Meter645Proc(nRd485Com, pData, nLen, nSize);
    nLen = CarrPointRd(pData, nLen);
	return nLen;
#endif
}




#if 0
#endif
extern INT32S Rs485RdProc(INT16U nSeq, INT8U nCom, INT8U nChkPort, void* pData);
#define CarrRdProc		Rs485RdProc
/********************************************************************************
* ������:TaskCarrColProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:������ :
*********************************************************************************/
static INT32S TaskCarrColProc(void)
{
	INT32S				nRet			= 0;
	INT32S				i 				= 0;
	INT8U				nPort			= 0;
	pCOL_DATA			pCarrColData	= NULL;
	INT8U				nCom			= COMM_CARR_SND_DEBUG;
    
    if (g_nCarrZbSelFlg == TYPE_ZB)
    {
        return -1;
    }
	
	pCarrColData = (pCOL_DATA)&g_sCarrColData;

	//��ȡ��Ӧ��485�˿ں�
	nRet = GetPortIdx(CARR_PORT_IDX);	
	if (nRet < 0)
	{
		return -1;
	}
	nPort = (INT8U)nRet;
	
	for (i = 0; i < MAX_METER_NUM; i++)
	{
		CarrRdProc(i + 1, nCom, nPort, pCarrColData);
	}

	return 0;
}


#if 0
#endif
/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: Rs485�ɼ���ʼ��
*********************************************************************************/
INT32S CarrColInit(void)
{
    //�ز���
    GpioConfigEX(0x38, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x39, GPIO_MODE_INPUT, IO_PULLUP_ON );
    OSTimeDlyHMSM(0, 0, 2, 0);
    SET_IO(0x38, 1);
    
    if (g_nCarrZbSelFlg == TYPE_ZB)
    {
        g_nCarrInitFlg = 1;
        return 0;
    }
    
#if 1
//���߲���
	//SET_IO(CARR_JZQ1_RST, 0);
	
	OSTimeDlyHMSM(0, 0, 0, 300);
	//SET_IO(CARR_JZQ1_RST, 1);

    OSTimeDlyHMSM(0, 0, 10, 0);
//���û���
    ReadCarrVer();          //�ж����ز�ģ�黹������ģ��
    //if (WrilessInit() < 0)
	{
		//ErrLedSet(1);
	}
    
    CarrModlSel(g_nCarrModType);
    OSTimeDlyHMSM(0, 0, 0, 100);
	//SET_IO(CARR_JZQ1_RST, 1);

	//OSTimeDlyHMSM(0, 0, 3, 0);
	//ErrLedSet(0);
	if (CarrReg(0) < 0)
	{
		//ErrLedSet(1);
	}
	if (CarrStopRdMeter() < 0)
	{
		//ErrLedSet(1);
	}
	//if (CarrRouteInit() < 0)
	{
		//ErrLedSet(1);
	}
	//if (CarrReg(0) < 0)
	{
		//ErrLedSet(1);
	}
	//if (CarrStopRdMeter() < 0)
	{
		//ErrLedSet(1);
	}
    g_nCarrInitFlg = 1;
#endif
	return 0;
}

INT32S  CarrPointRd(INT8U* pData, INT32S nLen)
{
    INT32S              nLentmp         = 0;
    INT8U				nIdx			= 0;
    INT8U				nAfn			= 0;
    INT8U               nTmp            = 2;
    INT8U               nAddr[6]        = {0};
    pGW3762_PROT		pGw3762Prot	    = NULL;
    pGW3762_PROT_SUB	pGw3762ProtSub	= NULL;
    pCARR_MODE_INFO		pCarrModeInfo	= NULL;
    
    //�ز�ģ�鲻��������
    if (g_nCarrErrFlg)
    {
        return -1;
    }
    
    memcpy(&nAddr[0], &pData[1], 6);
    
    //��ȡ�汾���ж����ز�ģ�黹������ģ��
    //ReadCarrVer();
    pCarrModeInfo = CarrModeInfoGet(g_nCarrModType);
    if (NULL == pCarrModeInfo)
    {
        nAfn = 0x13;
    }
    else
    {
        nAfn = pCarrModeInfo->nTranAfn;
        if (0x55 == pCarrModeInfo->nSetNodeRouter)
        {
            //��͸ʱ�����ն����иñ�Ʋ���
            //if (g_nNodeAddFlg == 0)
            /*{
                nLentmp = CarrRouteAdd(&nAddr[0], 2);
                if (nLentmp < 0)
                {
                    return -1;
                }
            }*/
            //���Ϊ����ģ�飬ÿ�γ�����ߵ㳭ʱ�����һ�α��ַ
            /*if (g_nCarrModType == CARR_MOD_DX)
            {
                nLentmp = CarrRouteAdd(&nAddr[0], 2);
                if (nLentmp < 0)
                {
                    return -1;
                }
            }*/
        }
    }
    
    if (pCarrModeInfo->nTranAfn == 0x02)
    {
        nTmp = 2;
        memmove(&pData[nTmp], &pData[0], nLen);
        pData[0] = 2;					
        pData[1] = nLen;
    }
    else
    {
        nTmp = 3;
        memmove(&pData[nTmp], &pData[0], nLen);
        pData[0] = 2;					
        pData[1] = 0;
        pData[2] = nLen;
    }
    
    INT8U nnnnn = 1;
    if (nnnnn)
    {
        nLentmp = Gb3762ProtMakeFrm(&nAddr[0], pData, nLen + nTmp, 300, GW3762_DOWN_FRM, GW3762_START_PRM, 
                                    g_nModl, GW3762_METER_NODE, 0, nAfn, F1);
        nLentmp = Gw3762Proc(pData, nLentmp, 150);
        if (nLentmp < 0)
        {
            //nLentmp = 0;	
            nLentmp = -1;	
        }
        else
        {
            pGw3762Prot = (pGW3762_PROT)pData;
            pGw3762ProtSub = (pGW3762_PROT_SUB)(pGw3762Prot->nAppData);

            if (TestBufBit(&pGw3762ProtSub->nInfo[0], 1, 2))
            {
                nIdx = GW3762_PROT_APP_ADDR_LEN;
            }
            if (nLentmp < 15 + nIdx + 2)
            {
                nLentmp = 0;	
            }
            else
            {
                nLentmp -= 15 + nIdx + 2;
                memmove(&pData[0], &pGw3762ProtSub->nUseData[nIdx + 3 + 2], nLentmp);
                //�ж�645֡
                nLentmp = Meter645CheckPackage(NULL, pData, nLentmp);
            }
        }
    }
    return nLentmp;
}




/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: Rs485�ɼ�����
*********************************************************************************/
void CarrColTask(void *pData)
{
	INT8U 		nErr 			= 0;
    
    if (CarrColInit() < 0)
	{
		return;
	}
	for (; ;)
	{
        if (g_nCarrInitFlg == 0)
        {
            continue;
        }
		OSSemPend(g_pSem[SEM_CARR_IDX ], 0, &nErr);
        //OSTimeDlyHMSM(0, 0, 0, 200);
        TaskCarrColProc();
        
		OSTimeDlyHMSM(0, 0, 0, 100);
        //SysSetTaskRunOnceTOS(TASK_RS485_1_COL_IDX);
	}
}


INT32S WrilessInit(void)
{
    INT8U	nBuf[100]	= {0};
	INT8U*	pBuf		= &nBuf[0];
	INT32S	nLen		= 0;
	INT16U	nFn 		= F5;
    INT8U  nWirelessChannel = 0;                       //(12�淶F90)�����ŵ���
    INT8U  nWirelessPower = 0;                         //(12�淶F90)�������緢�书��
    
    nWirelessChannel = 1;
    nWirelessPower = 0x84;
    
    pBuf[0] = nWirelessChannel;
    pBuf[1] = nWirelessPower;
    
    nLen = 2;

	nLen = Gb3762ProtMakeFrm(NULL, pBuf, nLen, sizeof(nBuf), GW3762_DOWN_FRM, GW3762_START_PRM, 
								g_nModl, GW3762_MAIN_SELF, 0, 0x05, nFn);
	nLen = Gw3762Proc(pBuf, nLen, sizeof(nBuf));
    
    if (nLen > 0)
    {
        if (pBuf[11] == 0x01)
        {
            g_nCarrModType = CARR_MOD_YXD;
            g_nModl = GW3762_WLAN;
        }
        else
        {
            g_nCarrModType = CARR_MOD_DX;
            g_nModl = GW3762_CARR;
        }
    }
    
	return nLen;
}

void YXD(void)
{
    
}









/******************************************************************************************************
* 2013-2014 ���Ƽ����޹�˾
* �ļ���: 
* ������: ��һ��
* �ա���: 2013/09/21
* �޸���: 
* �ա���: 
* �衡��: 
* �桡��: V1.0
*****************************************************************************************************/
#include "SysRes.h"
/**************************************************************************************************
                                           ��
***************************************************************************************************/



/**************************************************************************************************
                                           ���ȫ�ֱ���ʵ��
***************************************************************************************************/
//���̶�ջ
static OS_STK		g_nTask10MSecStk[TASK_10MSEC_STK];              	//10���뼶
static OS_STK		g_nTaskMSecStk[TASK_MSEC_STK];              		//���뼶
static OS_STK		g_nTaskSecStk[TASK_SEC_STK];               		    //�뼶
static OS_STK		g_nTaskDispStk[TASK_DISP_STK];              		//��ʾ
static OS_STK		g_nTaskLocalLinkStk[TASK_LOCAL_LINK_STK];   		//����232
static OS_STK		g_nTaskLocalLink2Stk[TASK_LOCAL_LINK2_STK];   		//����ά��485
static OS_STK		g_nTaskBleLinkStk[TASK_BLE_LINK_STK];   		        //��������
static OS_STK		g_nTaskCarrCol2Stk[TASK_CARR_COL2_STK];   		    //�ز��ɼ�
static OS_STK		g_nTaskAcColStk[TASK_AC_COL_STK];   		        //����
static OS_STK		g_nTaskIdleStk[TASK_IDLE_STK];   		            //����

//static OS_STK		g_nTaskEnetCliLinkStk[TASK_ENETCLI_LINK_STK];   	//��̫���ͻ���Link
//static OS_STK		g_nTaskEnetSerLinkStk[TASK_ENETSER_LINK_STK];   	//��̫�������Link
//static OS_STK		g_nTaskRemotCtrlStk[TASK_REMOT_CTRL_STK];   	    //ң��
//static OS_STK		g_nTaskRs485ColStk[TASK_RS485_COL_STK];   		    //485�ɼ�
//static OS_STK		g_nTaskCarrColStk[TASK_CARR_COL_STK];   		    //�ز��ɼ�
//static OS_STK		g_nTaskRmotLinkStk[TASK_RMOT_LINK_STK];   		    //Զ��GPRS

//���񴴽��б�
TASK_USER_DATA 		g_sTaskUserData[MAX_USER_TASK_NUM];


//�������ⲿ����
extern void SecTask(void *pData);
extern void MSecTask(void *pData);
extern void M10SecTask(void *pData);
extern void DispTask(void *pData);
extern void LocalLinkTask(void *pData);
extern void LocalLink2Task(void *pData);
extern void LocalLinkBleTask(void *pData);
extern void CarrCol2Task(void *pData);
extern void IdleTask(void *pData);

//extern void RmotLinkTask(void *pData);
//extern void Rs485ColTask(void *pData);
//extern void CarrColTask(void *pData);
//extern void EthnetLinkTask(void *pData);
//extern void EthnetLinkSerTask(void *pData);
//extern void RmotCtrlTask(void *pData);


static INT8U	g_nDispTaskArg = 0;

const TASK_CREATE g_sTaskCreateTbl[] = {

	{// 1	
		TASK_SEC,     "SecTask",	  SecTask,         NULL, &g_nTaskSecStk[0],	        TASK_SEC_PRIO,	        TASK_SEC_STK,
		&g_sTaskUserData[0],
		1,
		10 * 60 * 2
	},
    
	{// 2	
		TASK_10MSEC,  "M10SecTask",	  M10SecTask,        NULL, &g_nTask10MSecStk[0],	    TASK_10MSEC_PRIO,	    TASK_10MSEC_STK,
		&g_sTaskUserData[0],
		1,
		10 * 60
	},

	{// 3	
		TASK_MSEC,	  "MSecTask",	  MSecTask,        NULL, &g_nTaskMSecStk[0],	    TASK_MSEC_PRIO,	        TASK_MSEC_STK,
		&g_sTaskUserData[0],
		1,
		10 * 60
	},

	{// 4	
		TASK_DISP,	  "DispTask",	  DispTask,       (pVOID)(&g_nDispTaskArg), &g_nTaskDispStk[0],	    TASK_DISP_PRIO,	        TASK_DISP_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF//10 * 60 * 3
	},

#if 1
	{// 5	232
		TASK_LLINK,	  "LocalLinkTask",LocalLinkTask,   NULL, &g_nTaskLocalLinkStk[0],   TASK_LOCAL_LINK_PRIO,	TASK_LOCAL_LINK_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF//10 * 60 * 3
	},
#endif    

    {// 6	4851
		TASK2_LLINK,	  "LocalLink2Task",LocalLink2Task,   NULL, &g_nTaskLocalLink2Stk[0],   TASK_LOCAL_LINK2_PRIO,	TASK_LOCAL_LINK2_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF//10 * 60 * 3
	},

#if 1
    
    {// 7 	����
		TASK_BLELINK,	  "LocalLinkBleTask",LocalLinkBleTask,   NULL, &g_nTaskBleLinkStk[0],   TASK_BLE_LINK_PRIO,	TASK_BLE_LINK_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF//10 * 60 * 3
	},
#endif
    
#if 0     
    {// 8 485�ɼ�����
		TASK_RS485_COL,	  "Rs485ColTask",Rs485ColTask,   (INT8U*)1, &g_nTaskRs485ColStk[0],   TASK_RS485_COL_PRIO,	TASK_RS485_COL_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF//10 * 60 * 3
	},
#endif

#if 1
    
    {// ɨ��ǹ	
		TASK_CARR2_COL,	  "CarrCol2Task",CarrCol2Task,   NULL, &g_nTaskCarrCol2Stk[0],   TASK_CARR_COL2_PRIO,	TASK_CARR_COL2_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF//10 * 60 * 3
	},
#endif

#if REMOT_CTRL_EN == 1
    {// 11	
		TASK_REMOT_CTRL,	  "RmotCtrlTask",RmotCtrlTask,   NULL, &g_nTaskRemotCtrlStk[0],   TASK_REMOT_CTRL_PRIO,	TASK_REMOT_CTRL_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF
	},
#endif

#if AC_FUNC_EN == 1
    {// 11	
		TASK_AC_COL,	  "AcColTask",AcColTask,   NULL, &g_nTaskAcColStk[0],   TASK_AC_COL_PRIO,	TASK_AC_COL_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF
	},
#endif

#if 1
    {// 11	
		TASK_IDLE,	  "IdleTask",IdleTask,   NULL, &g_nTaskIdleStk[0],   TASK_IDLE_PRIO,	TASK_IDLE_STK,
		&g_sTaskUserData[0],
		10,
		0xFFFFFFFF
	},
#endif

    
    
};

//const pTASK_CREATE g_pTaskCreateTbl = &g_sTaskCreateTbl[0];

                                                        	
//������Դ                                              	
OS_EVENT  	*g_pQueue[QUEUE_NUM];						   		//����
void      	*g_pQueueMsg[QUEUE_NUM][QUEUE_MSG_NUM];		    //������Ϣ�ռ�
OS_EVENT  	*g_pMBox[MBOX_NUM];									//����
OS_EVENT  	*g_pSem[SEM_NUM];							 		//�ź���


PARAM       g_sParam;
pPARAM  	g_pParam = &g_sParam;

/**************************************************************************************************
                                            ���񴴽�
***************************************************************************************************/
/********************************************************************************
* ������:SysQueryTaskIndex
* �䡡��:
*		INT8U  nId									����ID
*		INT8U* pIndex								�����б��ƫ��λ��
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
static INT8S SysQueryTaskIndex(INT8U nId, INT8U* pIndex)
{
	INT8U	i 		= 0;
	INT8U	nErr	= 0;
	BOOLEAN bFlg	= FALSE;
	
	if (nId >= MAX_USER_TASK_NUM)
	{
		return RET_ERR_ERROR;
	}
	if (NULL == pIndex)
	{
		return RET_ERR_ERROR;
	}
	
	OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
	for (i = 0; i < (sizeof(g_sTaskCreateTbl) / sizeof(TASK_CREATE)); i++)
	{
		if (nId == g_sTaskCreateTbl[i].nId)
		{
			bFlg = TRUE;
            break;
		}
	}
	OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);
	if (!bFlg)
	{
		return RET_ERR_ERROR;
	}
	*pIndex = i;
	
	return RET_ERR_OK;
}

/********************************************************************************
* ������:SysQueryTaskPrio
* �䡡��:
*		INT8U  nId									����ID
*		INT8U* pPrio								�������ȼ�
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
INT8S SysQueryTaskPrio(INT8U nId, INT8U* pPrio)
{
	INT8U	nIndex 	= 0;
	INT8U	nErr	= 0;
	
	if (nId >= MAX_USER_TASK_NUM)
	{
		return RET_ERR_ERROR;
	}
	if (NULL == pPrio)
	{
		return RET_ERR_ERROR;
	}
	
	if (RET_ERR_OK != SysQueryTaskIndex(nId, &nIndex))
	{
	    return RET_ERR_ERROR;
	}
	
	OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
	*pPrio = g_sTaskCreateTbl[nIndex].nPrio;
	OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);

	return RET_ERR_OK;
}

/********************************************************************************
* ������:SysQueryTaskUserData
* �䡡��:
*		INT8U  				nId									����ID
*		TASK_USER_DATA** 	ppTaskUserData						����
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
static INT8S SysQueryTaskUserData(INT8U nId, TASK_USER_DATA** ppTaskUserData)
{
	INT8U			nIndex 			= 0;
	INT8U			nErr			= 0;
	
	if (nId >= MAX_USER_TASK_NUM)
	{
		return RET_ERR_ERROR;
	}
	if (NULL == ppTaskUserData)
	{
		return RET_ERR_ERROR;
	}
	
	if (RET_ERR_OK != SysQueryTaskIndex(nId, &nIndex))
	{
	    return RET_ERR_ERROR;
	}
	
	OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
	*ppTaskUserData = (pTASK_USER_DATA)(g_sTaskCreateTbl[nIndex].pExt);
	OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);

	return RET_ERR_OK;
}

/********************************************************************************
* ������:SysSetTaskRunOnceTOS
* �䡡��:
*		INT8U  nId									����ID
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/

INT8S SysSetTaskRunOnceTOS(INT8U nId)
{
	INT8U			nIndex 			= 0;
	INT8U			nErr			= 0;
	pTASK_USER_DATA pTaskUserData 	= NULL;
	
	if (nId >= MAX_USER_TASK_NUM)
	{
		return RET_ERR_ERROR;
	}
	
	if (RET_ERR_OK != SysQueryTaskIndex(nId, &nIndex))
	{
	    return RET_ERR_ERROR;
	}
	
	OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
	pTaskUserData = (pTASK_USER_DATA)(g_sTaskCreateTbl[nIndex].pExt);
    if (NULL != pTaskUserData)
    {
        pTaskUserData = &pTaskUserData[nId];
		if (NULL != pTaskUserData)
		{
			pTaskUserData->nTaskRunVal = g_sTaskCreateTbl[nIndex].nRunOnceTOS;
		}
	}
	OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);

	return RET_ERR_OK;
}

/********************************************************************************
* ������:SysGetTaskRunOnceTOS
* �䡡��:
*		INT8U  	nId									����ID
*		INT32U* pTos								��ʱʱ��
*		INT8U 	nFlg								0ʱֻ�ǻ�ȡʱ�䣻1ʱ��1�ٻ�ȡ
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
INT8S SysGetTaskRunOnceTOS(INT8U nId, INT32U* pTos, INT8U nFlg)
{
	INT8U			nErr			= 0;
	pTASK_USER_DATA pTaskUserData 	= NULL;
	
	if (nId >= MAX_USER_TASK_NUM)
	{
		return RET_ERR_ERROR;
	}
	if (NULL == pTos)
	{
		return RET_ERR_ERROR;
	}

	if (RET_ERR_OK != SysQueryTaskUserData(nId, &pTaskUserData))
	{
	    return RET_ERR_ERROR;
	}
	OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
	do
	{
		if (NULL == pTaskUserData)
		{
			*pTos = 0xFFFFFFFE;
			break;
		}
		pTaskUserData = &pTaskUserData[nId];
		if (NULL == pTaskUserData)
		{
			*pTos = 0xFFFFFFFE;
			break;
		}
		
		*pTos = pTaskUserData->nTaskRunVal;
		if (0xFFFFFFFF == pTaskUserData->nTaskRunVal)
		{
			break;
		}
		if (0 == pTaskUserData->nTaskRunVal)
		{
			break;
		}
		if (1 == nFlg)
		{
			(pTaskUserData->nTaskRunVal)--;
		}
		*pTos = pTaskUserData->nTaskRunVal;
		
		break;
	}while(0);
	OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);

	return RET_ERR_OK;
}

/********************************************************************************
* ������:SysTaskCreate
* �䡡��:
*		INT8U  nId									����ID
* �䡡��:
* ��  ��:
* ��  ��: ��������
*********************************************************************************/
INT8S SysTaskCreate(INT8U nId)
{
	INT8U 			nIndex 			= 0;
	INT8U			nRet			= 0;
    INT8U           nErr            = 0;
	pTASK_USER_DATA pTaskUserData 	= NULL;
    pTASK_USER_DATA pTaskUserData2 	= NULL;
	
	if (RET_ERR_OK != SysQueryTaskIndex(nId, &nIndex))
	{
	    return RET_ERR_ERROR;
	}

    OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
    if (g_sTaskCreateTbl[nIndex].nStackSize < 8)
    {
        nRet = 1;
    }
    OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);

    if (0 != nRet)
    {
    	return RET_ERR_ERROR;
    }

    OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
	pTaskUserData = (pTASK_USER_DATA)(g_sTaskCreateTbl[nIndex].pExt);
    if (NULL == pTaskUserData)
    {
        pTaskUserData2 = NULL;
    }
    else
    {
        pTaskUserData2 = &pTaskUserData[nId];
    }
    if (NULL != pTaskUserData2)
	{
		strcpy((char*)(&(pTaskUserData2->nTaskName[0])),(char*)(g_sTaskCreateTbl[nIndex].pName));
		pTaskUserData2->nTaskCheckTOS = g_sTaskCreateTbl[nIndex].nCheckTOS;
		pTaskUserData2->nTaskRunVal   = g_sTaskCreateTbl[nIndex].nRunOnceTOS;
	}
    nRet = OSTaskCreateExt(
                            (pTASK_FUNC)(g_sTaskCreateTbl[nIndex].pTaskFunc),
                            (pVOID)(g_sTaskCreateTbl[nIndex].pArg),
                            (pOS_STK)(&(g_sTaskCreateTbl[nIndex].pStack[g_sTaskCreateTbl[nIndex].nStackSize - 1])),
                            g_sTaskCreateTbl[nIndex].nPrio,
                            g_sTaskCreateTbl[nIndex].nPrio,
                            (pOS_STK)(&(g_sTaskCreateTbl[nIndex].pStack[0])),
                            g_sTaskCreateTbl[nIndex].nStackSize,
                            (void*)pTaskUserData2,
                            OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR
                          );
#if OS_TASK_NAME_SIZE > 7
    OSTaskNameSet(g_sTaskCreateTbl[nIndex].nPrio, (INT8U*)(g_sTaskCreateTbl[nIndex].pName), &nErr);
#endif
	OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);
	
	if (OS_NO_ERR != nRet)
	{
		return RET_ERR_ERROR;
	}

	return RET_ERR_OK;
}

/********************************************************************************
* ������:SysSetTaskRunOnceTOS
* �䡡��:
*		INT8U  nId									����ID
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
INT8S SysSetTaskInit(INT8U nId)
{
	INT8U			nErr			= 0;
	pTASK_USER_DATA pTaskUserData 	= NULL;
	
	if (nId >= MAX_USER_TASK_NUM)
	{
		return RET_ERR_ERROR;
	}
	
	if (RET_ERR_OK != SysQueryTaskUserData(nId, &pTaskUserData))
	{
	    return RET_ERR_ERROR;
	}
	
	OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
    if (NULL != pTaskUserData)
    {
        pTaskUserData = &pTaskUserData[nId];
		if (NULL != pTaskUserData)
		{
			pTaskUserData->nTaskCheckTOS = 0xFF;
			pTaskUserData->nTaskRunVal 	 = 10 * 60 * 10;
		}
	}
	OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);

	return RET_ERR_OK;
}

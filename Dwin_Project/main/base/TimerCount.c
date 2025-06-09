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
#include "Base.h"
#include "TimerCount.h"

/**************************************************************************************************
                                           ����
***************************************************************************************************/
static	TTIMER		g_sTimer[TIMER_NUM];						//��ʱ��
pTTIMER 			g_pTimer = &g_sTimer[0];					//��ʱ��


/**************************************************************************************************
                                           ����
***************************************************************************************************/
/****************************************************************************
* ������:TimerStatGet
* �䡡��:
*				pTTIMER 			pTimer
* �䡡��:
* ��  ��: 0��û�д򿪣�1����
* ��  ��: 
****************************************************************************/
INT32S TimerStatGet(pTTIMER pTimer)
{
	if (NULL == pTimer)
	{
		return -1;
	}

	return pTimer->bOn;
}

/****************************************************************************
* ������:TimerStatSet
* �䡡��:
*				pTTIMER 			pTimer
* �䡡��:
* ��  ��:
* ��  ��: 
****************************************************************************/
INT32S TimerStatSet(pTTIMER pTimer, INT8U nOn)
{
	if (NULL == pTimer)
	{
		return -1;
	}
	if (nOn != pTimer->bOn)
	{
		pTimer->bOn = nOn;
	}

	return 0;
}



/****************************************************************************
* ������:TimerValGet
* �䡡��:
*				pTTIMER 			pTimer
* �䡡��:
* ��  ��:
* ��  ��: 
****************************************************************************/
INT32S TimerValGet(pTTIMER pTimer)
{
	if (NULL == pTimer)
	{
		return -1;
	}
	if (FALSE == pTimer->bOn)
	{
		return -1;
	}
	
	return pTimer->nValue;
}

/****************************************************************************
* ������:TimerValSet
* �䡡��:
*				pTTIMER 			pTimer
* �䡡��:
* ��  ��:
* ��  ��: 
****************************************************************************/
INT32S TimerValSet(pTTIMER pTimer, INT16U nVal)
{
	if (NULL == pTimer)
	{
		return -1;
	}
	if (FALSE == pTimer->bOn)
	{
		return -1;
	}
	
	pTimer->nValue = nVal;

	return 0;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʱ����
*********************************************************************************/
INT32S TimerOpen(pTTIMER pThis, INT16U nInitValue)
{
    if (NULL == pThis)
    {
    	return -1;
    }
    pThis->bOn = TRUE;
    pThis->nValue = nInitValue;
    return 0;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʱ���ر�
*********************************************************************************/
INT32S TimerClose(pTTIMER pThis)
{
    if (NULL == pThis)
    {
    	return -1;
    }
    pThis->bOn = FALSE;
    pThis->nValue = 0;
    return 0;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʱ����ʼ��
*********************************************************************************/
INT32S TimerInit(pTTIMER pThis, INT8U nType)
{
    if (NULL == pThis)
    {
    	return -1;
    }
    pThis->bOn = FALSE;
    pThis->nType = nType;
    pThis->nValue = 0;
    return 0;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʱ������
*********************************************************************************/
static void TimerRun(pTTIMER pThis)
{
	if (NULL == pThis)
	{
		return;
	}

	if (pThis->bOn)
    {
    	switch (pThis->nType)
	    {
	    	case TIMER_TYPE_SEC_FWARD:
	    	//case TIMER_TYPE_MIN_FWARD:
	    		if (pThis->nValue < 0xFFFF)
	    		{
	    			pThis->nValue++;
	    		}
	    		break;

	    	case TIMER_TYPE_SEC_BACK:
	    	//case TIMER_TYPE_MIN_BACK:
	    		if (pThis->nValue > 0)
	    		{
	    			pThis->nValue--;
	    		}
	    		break;

	    	default:
	    		break;
	    }
    }
}


/********************************************************************************
* ������:TimerCtrlProc
* �䡡��:nSecMinFlg = 0 ��ʾ�뼶
*                   = 1 ��ʾ���Ӽ�
* �䡡��:
* ��  ��:
* ��  ��: �ն˶�ʱ�����ƴ������
*********************************************************************************/
void TimerCtrlProc(INT8U nSecMinFlg)
{
	INT32S 		kk 		= 0;
	INT8U 		nType1 	= 0;
	INT8U 		nType2 	= 0;
	pTTIMER 	pThis 	= NULL;
	
	//if(nSecMinFlg == 0)
	//{
		nType1 = TIMER_TYPE_SEC_FWARD;
		nType2 = TIMER_TYPE_SEC_BACK;
	//}
	//else
	//{
		//nType1 = TIMER_TYPE_MIN_FWARD;
		//nType2 = TIMER_TYPE_MIN_BACK;
	//}
	for (kk = 0; kk < TIMER_NUM; kk++)
	{
		pThis = &g_sTimer[kk];
		if ((pThis->nType == nType1) || (pThis->nType == nType2))
		{
			TimerRun(pThis);
		}
	}
}



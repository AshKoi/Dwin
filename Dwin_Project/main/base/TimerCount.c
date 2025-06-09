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
#include "Base.h"
#include "TimerCount.h"

/**************************************************************************************************
                                           变量
***************************************************************************************************/
static	TTIMER		g_sTimer[TIMER_NUM];						//计时器
pTTIMER 			g_pTimer = &g_sTimer[0];					//计时器


/**************************************************************************************************
                                           函数
***************************************************************************************************/
/****************************************************************************
* 函数名:TimerStatGet
* 输　入:
*				pTTIMER 			pTimer
* 输　出:
* 返  回: 0，没有打开；1，打开
* 功  能: 
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
* 函数名:TimerStatSet
* 输　入:
*				pTTIMER 			pTimer
* 输　出:
* 返  回:
* 功  能: 
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
* 函数名:TimerValGet
* 输　入:
*				pTTIMER 			pTimer
* 输　出:
* 返  回:
* 功  能: 
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
* 函数名:TimerValSet
* 输　入:
*				pTTIMER 			pTimer
* 输　出:
* 返  回:
* 功  能: 
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 计时器打开
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 计时器关闭
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 计时器初始化
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 计时器运行
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
* 函数名:TimerCtrlProc
* 输　入:nSecMinFlg = 0 表示秒级
*                   = 1 表示分钟级
* 输　出:
* 返  回:
* 功  能: 终端定时器控制处理过程
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



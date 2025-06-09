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
#include "SysRes.h"

/**************************************************************************************************
                                           变量
***************************************************************************************************/
#if 1 == SOFT_TIME_EN
static STD_TIME gs_sTime;
#endif

/**************************************************************************************************
                                           函数
***************************************************************************************************/
/****************************************************************************
* 函数名:SoftTimerVal
* 输　入:
*				INT8U		nDir     0,读取；1,写入
* 输　出:
* 返  回:
* 功  能: 
****************************************************************************/
static void SoftTimerVal(pSTD_TIME pTime, INT8U nDir)
{
#if 1 == SOFT_TIME_EN
	INT8U	nErr = 0;
	
	if (NULL == pTime)
	{
		return;
	}
	OSSemPend(g_pSem[SEM_SOFTTIMER_VAL], 0, &nErr);
	if (nDir)
	{
		memset(&gs_sTime, 0, sizeof(STD_TIME));
		memcpy(&gs_sTime, pTime, sizeof(STD_TIME));
	}
	else
	{
		memset(pTime, 0, sizeof(STD_TIME));
		memcpy(pTime, &gs_sTime, sizeof(STD_TIME));
	}
	OSSemPost(g_pSem[SEM_SOFTTIMER_VAL]);
#endif
}


/****************************************************************************
* 函数名:GetSysClk
* 输　入:
* 输　出:
*		pSTD_TIME pTime
* 返  回:
*		正常，0；异常，-1
* 功  能: 获取系统时钟
		1. 读取软时钟，核查有效性
		2. 读取硬时钟，核查有效性
		3. 硬时钟无效，返回-1
****************************************************************************/
INT32S GetSysClk(pSTD_TIME pTime)
{
	if (NULL == pTime)
	{
		return -1;
	}
#if 1 == SOFT_TIME_EN
	// 1. 获取软时钟
	SoftTimerVal(pTime, 0);
	if (IsVldStdTime(pTime))
	{
		return 0;
	}
#endif
	// 2. 获取硬时钟
	memset(pTime, 0, sizeof(STD_TIME));
	RTCDateTimeRead(pTime);
	if (IsVldStdTime(pTime))
	{
#if 1 == SOFT_TIME_EN		
		// 同步软时钟
		SoftTimerVal(pTime, 1);
#endif
		return 0;
	}

	return -1;
}


/****************************************************************************
* 函数名:SyncSysClk
* 输　入:
* 		pSTD_TIME 		pTime				空时以当前硬时钟同步；非空时以变量同步所有时钟(设置时钟时使用)
* 输　出:
* 返  回:
*		正常，0；异常，-1
* 功  能: 同步系统时钟。每分钟同步一次
		1. 读取硬时钟,核查有效性
		2. 读取软时钟，核查有效性
		3. 软时钟无效，返回-1 : 用时钟2000-01-01 00:05:00 刷新
****************************************************************************/
INT32S SyncSysClk(pSTD_TIME pTime)
{
	STD_TIME 	sTime;
		
	if (NULL == pTime)
	{
		memset(&sTime, 0, sizeof(sTime));
		RTCDateTimeRead(&sTime);	
		if (IsVldStdTime(&sTime))
		{
#if 1 == SOFT_TIME_EN
			// 同步软时钟
			SoftTimerVal(pTime, 1);
#endif
			return 0;
		}
		else
		{
#if 1 == SOFT_TIME_EN
			//获取软时钟
			SoftTimerVal(&sTime, 0);
			if (IsVldStdTime(&sTime))
			{
				// 同步硬时钟
				if (0 != RTCDateTimeWrite(&sTime))
				{
					return -1;
				}
				else
				{
					return 0;
				}
			}
			else
#endif
			{
				//刷新一个2000-01-01 00:05:00
				memset(&sTime, 0, sizeof(sTime));
				sTime.nMon = 1;
				sTime.nDay = 1;
				sTime.nMin = 5;
#if 1 == SOFT_TIME_EN
				SoftTimerVal(&sTime, 1);
#endif				
				// 同步硬时钟
				if (0 != RTCDateTimeWrite(&sTime))
				{
					return -1;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	else
	{
		if (IsVldStdTime(pTime))
		{
			// 1. 同步硬时钟
			if (0 != RTCDateTimeWrite(pTime))
			{
				return -1;
			}
#if 1 == SOFT_TIME_EN
			// 2. 同步软时钟
			SoftTimerVal(pTime, 1);
#endif
			return 0;
		}
		else
		{
			return -1;
		}
	}

}

/****************************************************************************
* 函数名:SoftTimerRefresh
* 输　入:
* 输　出:
* 返  回:
* 功  能: 软时钟每秒刷新一次
****************************************************************************/
INT32S SoftTimerRefresh(void)
{
#if 1 == SOFT_TIME_EN
	STD_TIME 	sTime;
	INT32U		nSecs 	= 0;
    
	SoftTimerVal(&sTime, 0);
	
    nSecs = GetSecsFrom2KYear(&sTime);
    if (0 == nSecs)
    {
		return -1;
    }
	
    nSecs++;
	memset(&sTime, 0, sizeof(sTime));
    GetLocalTime(nSecs, &sTime);
	
	SoftTimerVal(&sTime, 1);
#endif	
	return 0;
}





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
#include "SysRes.h"

/**************************************************************************************************
                                           ����
***************************************************************************************************/
#if 1 == SOFT_TIME_EN
static STD_TIME gs_sTime;
#endif

/**************************************************************************************************
                                           ����
***************************************************************************************************/
/****************************************************************************
* ������:SoftTimerVal
* �䡡��:
*				INT8U		nDir     0,��ȡ��1,д��
* �䡡��:
* ��  ��:
* ��  ��: 
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
* ������:GetSysClk
* �䡡��:
* �䡡��:
*		pSTD_TIME pTime
* ��  ��:
*		������0���쳣��-1
* ��  ��: ��ȡϵͳʱ��
		1. ��ȡ��ʱ�ӣ��˲���Ч��
		2. ��ȡӲʱ�ӣ��˲���Ч��
		3. Ӳʱ����Ч������-1
****************************************************************************/
INT32S GetSysClk(pSTD_TIME pTime)
{
	if (NULL == pTime)
	{
		return -1;
	}
#if 1 == SOFT_TIME_EN
	// 1. ��ȡ��ʱ��
	SoftTimerVal(pTime, 0);
	if (IsVldStdTime(pTime))
	{
		return 0;
	}
#endif
	// 2. ��ȡӲʱ��
	memset(pTime, 0, sizeof(STD_TIME));
	RTCDateTimeRead(pTime);
	if (IsVldStdTime(pTime))
	{
#if 1 == SOFT_TIME_EN		
		// ͬ����ʱ��
		SoftTimerVal(pTime, 1);
#endif
		return 0;
	}

	return -1;
}


/****************************************************************************
* ������:SyncSysClk
* �䡡��:
* 		pSTD_TIME 		pTime				��ʱ�Ե�ǰӲʱ��ͬ�����ǿ�ʱ�Ա���ͬ������ʱ��(����ʱ��ʱʹ��)
* �䡡��:
* ��  ��:
*		������0���쳣��-1
* ��  ��: ͬ��ϵͳʱ�ӡ�ÿ����ͬ��һ��
		1. ��ȡӲʱ��,�˲���Ч��
		2. ��ȡ��ʱ�ӣ��˲���Ч��
		3. ��ʱ����Ч������-1 : ��ʱ��2000-01-01 00:05:00 ˢ��
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
			// ͬ����ʱ��
			SoftTimerVal(pTime, 1);
#endif
			return 0;
		}
		else
		{
#if 1 == SOFT_TIME_EN
			//��ȡ��ʱ��
			SoftTimerVal(&sTime, 0);
			if (IsVldStdTime(&sTime))
			{
				// ͬ��Ӳʱ��
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
				//ˢ��һ��2000-01-01 00:05:00
				memset(&sTime, 0, sizeof(sTime));
				sTime.nMon = 1;
				sTime.nDay = 1;
				sTime.nMin = 5;
#if 1 == SOFT_TIME_EN
				SoftTimerVal(&sTime, 1);
#endif				
				// ͬ��Ӳʱ��
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
			// 1. ͬ��Ӳʱ��
			if (0 != RTCDateTimeWrite(pTime))
			{
				return -1;
			}
#if 1 == SOFT_TIME_EN
			// 2. ͬ����ʱ��
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
* ������:SoftTimerRefresh
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʱ��ÿ��ˢ��һ��
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





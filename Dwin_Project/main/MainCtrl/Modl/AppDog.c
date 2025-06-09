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
#include "AppDog.h"
#include "Base.h"
#include "SysRes.h"
#include "Fun.h"
/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/******************************************************************************************************
* ������:AppDogTaskStat
* �䡡��:
* �䡡��:
*****************************************************************************************************/
static BOOLEAN  AppDogTaskStat (INT8U nId)
{
    BOOLEAN  		bFlg			= FALSE;
    INT8U			nErr			= 0;
    pTASK_USER_DATA pTaskUserData 	= NULL;
    //STD_TIME 		sTermTime;

    if (nId >= MAX_USER_TASK_NUM)
	{
		return FALSE;
	}
	
    //�Ƚ�����ʱ�Ӹ��²�ֵ	 MAX_TASK_STOP_TIME
    OSSemPend(g_pSem[SEM_TASK_CHECK_IDX], 0, &nErr);
    do
    {
	    pTaskUserData = &g_sTaskUserData[nId];
	
	    if(    (0 == pTaskUserData->sLastRunTime.nYear)
	    	&& (0 == pTaskUserData->sLastRunTime.nMon)
	    	&& (0 == pTaskUserData->sLastRunTime.nDay)
	    	&& (0 == pTaskUserData->sLastRunTime.nHour)
	    	&& (0 == pTaskUserData->sLastRunTime.nMin)   )
	    {
	    	break;
	    }
	    if (0xFF == pTaskUserData->nTaskCheckTOS)
	    {
	    	break;
	    }
    	//if (RET_ERR_OK != GetSysClk(&sTermTime))
	    {
	    	//break;
	    }
    	//if (RET_ERR_OK != DiffFreeTime(&sTermTime, &pTaskUserData->sLastRunTime, GET_MAX(pTaskUserData->nTaskCheckTOS, MAX_TASK_STOP_TIME)))
    	{
    		//break;
    	}
	    bFlg = TRUE;

	    break;
	}while(0);
    OSSemPost(g_pSem[SEM_TASK_CHECK_IDX]);

    return bFlg;
}

/********************************************************************************
* ������: AppDogProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���񹷴���
*********************************************************************************/
void AppDogProc(void)
{
	BOOLEAN bTaskFlag 	= FALSE;
	INT8U	i		 	= 0;
	INT32U	nTos	 	= 0;
	
	for (i = 0; i < (sizeof(g_sTaskCreateTbl) / sizeof(TASK_CREATE)); i++)
	{
		bTaskFlag = FALSE;
		if (RET_ERR_OK != SysGetTaskRunOnceTOS(i, &nTos, 1))
		{
			bTaskFlag = TRUE;
		}
		if (0 == nTos)
		{
			bTaskFlag = TRUE;
		}
		if (bTaskFlag /*|| (AppDogTaskStat(i))*/)
		{
			ResetDelayHardInit(0);
		}
	}
}



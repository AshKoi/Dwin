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
#include "MSecTask.h"
#include "Base.h"
#include "AppDog.h"
#include "key.h"
#include "SysRes.h"
#include "pca9555.h"
#include "CarrCol.h"
#include "Fun.h"
#include "Dwin.h"

extern    INT8U                g_nCmt;
/**************************************************************************************************
                                           相关宏
***************************************************************************************************/

/**************************************************************************************************
                                           相关变量
***************************************************************************************************/
extern INT32S 	MsecRFPacketReceived(void);
extern void 	DispMSecProc(void);
extern INT32S	SoftTimerRefresh(void);



/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 毫秒级任务初始化
*********************************************************************************/
static BOOLEAN MSecTaskInit(void)
{
    //CarrColInit();
	return TRUE;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 运行灯
*********************************************************************************/
static void MSecRunLedProc(void)
{
	static INT8U   	nLedCnt 	= 0;
    static INT8U   	nLedSta 	= 0;
    //点亮Led
    if (++nLedCnt >= 5)
    {
		nLedCnt = 0;
		if (nLedSta == 1)
		{
			//GpioSetOutputEX( 0x48 );	//GLED_RUN
			nLedSta = 0;
		}	
		else
		{
			//GpioClearOutputEX( 0x48 );
			nLedSta = 1;
		}
    }
	
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获取时钟
*********************************************************************************/
static void MSecTimeProc(void)
{
	static INT8U   	nTCnt 		= 0;
	
	//刷新软时钟
    if (++nTCnt >= 9)
    {
		nTCnt = 0;
		// 刷新软时钟
		SoftTimerRefresh();
	}
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 毫秒级任务处理
*********************************************************************************/
void MSecTaskProc(void)
{
    MSecRunLedProc();

    //喂硬件狗
    WatchDogClear(CH_DOG_OUT);

    MSecTimeProc();
	
	//向迪文屏发送时间更新
	UpDateTime();

#if ((ASSIST_RUN == ASSIST_RUN_SIGNAL) || (ASSIST_RUN == ASSIST_RUN_HANDSHIP))
	MsecRFPacketReceived();
#endif

	//显示相关
	DispMSecProc();

	//进程监控处理
    AppDogProc();
}

void UpDateTime()
{
	STD_TIME        sTime;
	INT8U nDateTime[16];
	INT8U nAddr[2] = {0};
	INT16U offset = 0;
	//显示日期时间
    memset(&sTime, 0, sizeof(sTime));  
	 
	nAddr[0] = 0x00;
	nAddr[1] = 0x10;
    // 获取日期和时钟
    RTCDateTimeRead(&sTime);
    //日期和时间
    nDateTime[offset++] = sTime.nYear;  
    nDateTime[offset++] = sTime.nMon;   
    nDateTime[offset++] = sTime.nDay;   
    nDateTime[offset++] = 0x00;   
    nDateTime[offset++] = sTime.nHour;   
    nDateTime[offset++] = sTime.nMin;   
    nDateTime[offset++] = sTime.nSec;   
    nDateTime[offset++] = 0x00;   
	
	offset = DwinMakeFrm(&nDateTime[0],offset,&nAddr[0],0x82);
    //sendDataToLCD(nDateTime, offset); 
}




/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 毫秒级任务(100ms)
*********************************************************************************/
void MSecTask(void *pData)
{
    INT8U nErr = 0;

    pData = pData;

	if (!MSecTaskInit())
	{
		return;
	}
    for (; ;)
    {
        OSSemPend(g_pSem[SEM_MSEC_IDX], 0, &nErr);
        MSecTaskProc();
        SysSetTaskRunOnceTOS(TASK_MSEC);
    }
}




#if 0
// 10ms任务
#endif
extern void GetKeyValProc(void);                                   //声明
extern void GetKeyValProc0(void);                                   //声明


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 毫秒级任务(10ms)
*********************************************************************************/
void M10SecTask(void *pData)
{

	INT8U nErr = 0;

    pData = pData;

	OSTimeDlyHMSM(0, 0, 1, 300);
	while(OSSemAccept(g_pSem[SEM_10MSEC_IDX]) > 0);
    
    EMULATOR_9555_INIT() ;
    //KEY_POW_DDR = 0x00;													//硬件电池供电打开
	//while (KEY_POW_PDIR == 0x00);											//按键必须松开
    for (; ;)
    {
        OSSemPend(g_pSem[SEM_10MSEC_IDX], 0, &nErr);
        if (g_nDispErr == 0)
        {
            GetKeyValProc0();   
        }
    
        SysSetTaskRunOnceTOS(TASK_10MSEC);
    }
}










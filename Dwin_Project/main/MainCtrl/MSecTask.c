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
                                           ��غ�
***************************************************************************************************/

/**************************************************************************************************
                                           ��ر���
***************************************************************************************************/
extern INT32S 	MsecRFPacketReceived(void);
extern void 	DispMSecProc(void);
extern INT32S	SoftTimerRefresh(void);



/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���뼶�����ʼ��
*********************************************************************************/
static BOOLEAN MSecTaskInit(void)
{
    //CarrColInit();
	return TRUE;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���е�
*********************************************************************************/
static void MSecRunLedProc(void)
{
	static INT8U   	nLedCnt 	= 0;
    static INT8U   	nLedSta 	= 0;
    //����Led
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡʱ��
*********************************************************************************/
static void MSecTimeProc(void)
{
	static INT8U   	nTCnt 		= 0;
	
	//ˢ����ʱ��
    if (++nTCnt >= 9)
    {
		nTCnt = 0;
		// ˢ����ʱ��
		SoftTimerRefresh();
	}
}


/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���뼶������
*********************************************************************************/
void MSecTaskProc(void)
{
    MSecRunLedProc();

    //ιӲ����
    WatchDogClear(CH_DOG_OUT);

    MSecTimeProc();
	
	//�����������ʱ�����
	UpDateTime();

#if ((ASSIST_RUN == ASSIST_RUN_SIGNAL) || (ASSIST_RUN == ASSIST_RUN_HANDSHIP))
	MsecRFPacketReceived();
#endif

	//��ʾ���
	DispMSecProc();

	//���̼�ش���
    AppDogProc();
}

void UpDateTime()
{
	STD_TIME        sTime;
	INT8U nDateTime[16];
	INT8U nAddr[2] = {0};
	INT16U offset = 0;
	//��ʾ����ʱ��
    memset(&sTime, 0, sizeof(sTime));  
	 
	nAddr[0] = 0x00;
	nAddr[1] = 0x10;
    // ��ȡ���ں�ʱ��
    RTCDateTimeRead(&sTime);
    //���ں�ʱ��
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���뼶����(100ms)
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
// 10ms����
#endif
extern void GetKeyValProc(void);                                   //����
extern void GetKeyValProc0(void);                                   //����


/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���뼶����(10ms)
*********************************************************************************/
void M10SecTask(void *pData)
{

	INT8U nErr = 0;

    pData = pData;

	OSTimeDlyHMSM(0, 0, 1, 300);
	while(OSSemAccept(g_pSem[SEM_10MSEC_IDX]) > 0);
    
    EMULATOR_9555_INIT() ;
    //KEY_POW_DDR = 0x00;													//Ӳ����ع����
	//while (KEY_POW_PDIR == 0x00);											//���������ɿ�
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










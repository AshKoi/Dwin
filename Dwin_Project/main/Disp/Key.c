/***************************************************************
****************************************************************/
#include "Key.h"
#include "SysRes.h"
#include "PowerOn.h"
#include "SecTask.h"

extern INT8S    	g_nBeepFlg;  
extern INT8S 		g_nContinueKeyFlg;
extern INT8S           g_nChargeKeyFlg;  
extern INT8U       g_RealMeterCoverFlag;
KEY_SAMP		g_sKeySamp;								    //键值采集
pKEY_SAMP	g_pKeySamp = &g_sKeySamp;						//键值采集

INT8U                g_nCmt;
 INT32S               g_nKey_Flg = -1;


/*******************************************************************************
  * *函数名称	  Delay_us
  * *函数说明   插入一段延时时间  在为分频即主频80Mhz的情况下 为1us延时,
  * *输入参数   nCount: 指定延时时间长度  为10时 延时时间为10us
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void Delay_us(INT32U nCount)
{
    int i,j;
    for (i=0; i<nCount; i++)
        for (j=0; j<11; j++);           //80MHz下1us为11          
}
#if 0
/********************************************************************************
* 函数名:SetK0xInK1xOut
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
static void SetK0xInK1xOut(void)
{
    //设置k0x为输入
    GpioConfigEX(K01, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K02, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K03, GPIO_MODE_INPUT, IO_PULLUP_ON );
    
    //GpioConfigEX(PA5, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioClearOutputEX(PA5);
    
    //设置k1x为输出，并置为低电平
    GpioConfigEX(K11, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K12, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K13, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K14, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K15, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K16, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K17, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K18, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    
    GpioClearOutputEX(K11);
    GpioClearOutputEX(K12);
    GpioClearOutputEX(K13);
    GpioClearOutputEX(K14);
    GpioClearOutputEX(K15);
    GpioClearOutputEX(K16);
    GpioClearOutputEX(K17);
    GpioClearOutputEX(K18);      
}

/********************************************************************************
* 函数名:SetK1xInK0xOut
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
static void SetK1xInK0xOut(void)
{
    //设置k1x为输入
    GpioConfigEX(K11, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K12, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K13, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K14, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K15, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K16, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K17, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(K18, GPIO_MODE_INPUT, IO_PULLUP_ON );

    //设置k0x为输出，并置为低电平
    GpioConfigEX(K01, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K02, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(K03, GPIO_MODE_OUTPUT, IO_PULLUP_ON ); 
    GpioClearOutputEX(K01);
    GpioClearOutputEX(K02);
    GpioClearOutputEX(K03);
}
#endif


/********************************************************************************
* 函数名:GetKey
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
static INT8U GetKey(void)
{
    return PCA9555Read( 0, 1 );
		}


/*************************************************************************************
* 函数名:
* 输　入:10ms调用一次
* 输　出:
* 返  回:
* 功  能: 键值采集处理过程
**************************************************************************************/
void GetKeyValProc(void)
{
    INT8U 			nKeyVal 		= KEY_NULL;
	INT8U			nKeyCntNum		= 3;
	OS_SEM_DATA 	sSemData;
	
	if (g_nContinueKeyFlg)
	{
		nKeyCntNum = 6;
	}
	/*if (KEY_POW_PDIR == 0x00)													//运行过程中，若有按键，直接关电
	{
		OSSemQuery(g_pSem[SEM_FLASH_IDX], &sSemData);
		if (sSemData.OSCnt > 0)
		{
			GpioClearOutput(IO_PORTA, IO_PINx5);
		}
		return;
	}*/
   
	nKeyVal = GetKey();
    
    if (g_MeterPowerOff)
    {
        return;
    }
    
	if (KEY_NULL != nKeyVal)
	{
		g_pKeySamp->nPressCntBak = 0;
		if ((nKeyVal == g_pKeySamp->nKeyValBak))
		{
			if (g_pKeySamp->nPressCnt < nKeyCntNum)
			{
				g_pKeySamp->nPressCnt++;
			}
			if (g_pKeySamp->nPressCnt >= nKeyCntNum)
			{
				if (g_nContinueKeyFlg)
				{
					if ((KEY_ENTER == nKeyVal) || (KEY_CANCEL == nKeyVal) || (KEY_FN == nKeyVal) || (KEY_FUN2 == nKeyVal))
					{
						
					}
					else
					{
						g_pKeySamp->nKeyVal = nKeyVal;
						g_pKeySamp->nPressCnt = 0;
					}
				}
				else
				{

				}
                if (g_nBeepFlg)
                {
                    beep();    
                }
			}
		}
		else
		{
			g_pKeySamp->nKeyVal = KEY_NULL;
			g_pKeySamp->nPressCnt = 0;
		}
	}
	else
	{
		if (g_pKeySamp->nPressCnt >= nKeyCntNum)
		{
			if (g_nContinueKeyFlg)
			{
				if ((KEY_ENTER == g_pKeySamp->nKeyValBak) || (KEY_CANCEL == g_pKeySamp->nKeyValBak) || (KEY_FN == g_pKeySamp->nKeyValBak) || (KEY_FUN2 == g_pKeySamp->nKeyValBak))
				{
					g_pKeySamp->nKeyVal = g_pKeySamp->nKeyValBak;
				}
				else
				{

				}
			}
			else
			{
				g_pKeySamp->nKeyVal = g_pKeySamp->nKeyValBak;
			}
		}
			
		g_pKeySamp->nPressCnt = 0;
		g_pKeySamp->nPressCntBak++;
		if (g_pKeySamp->nPressCntBak >= 120)
		{
			g_pKeySamp->nKeyVal = KEY_NULL;
			g_pKeySamp->nPressCntBak = 0;
		}
	}
//#endif	
	g_pKeySamp->nKeyValBak = nKeyVal;
}


/*************************************************************************************
* 函数名:
* 输　入:10ms调用一次
* 输　出:
* 返  回:
* 功  能: 键值采集处理过程
**************************************************************************************/
void GetKeyValProc0(void)
{
    INT8U 			nKeyVal 		= KEY_NULL;
    INT8U 			nKeyValTmp 		= KEY_NULL;
	INT8U			nKeyCntNum		= 3;
	OS_SEM_DATA 	sSemData;
	INT8U			i,j,k,M;
    g_RealMeterCoverFlag = KEY_NULL;
    //if (g_nContinueKeyFlg)
	//{
	//	nKeyCntNum = 6;
	//}
	
	k =  GpioGetInput(IO_PORT5, IO_PINx9);		//连接按钮
	M =  GpioGetInput(IO_PORT5, IO_PINxA);		//断开按钮
	
	nKeyVal = (k<<1)|M;
	/*if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) || (TRIPHASE_METER_220ZT == g_nHardType))
	{  
		GpioConfig(IO_PORTC, IO_PINxF, GPIO_MODE_INPUT);
		i = GpioGetInput(IO_PORTC, IO_PINxF);
		if(0x00 == i)
		nKeyVal = KEY_UP;

		GpioConfig(IO_PORT4, IO_PINx4, GPIO_MODE_INPUT);
		M =  GpioGetInput(IO_PORT4, IO_PINx4);                   //开接线盖检测口
		if(0x01 == M)
		nKeyValTmp = KEY_JG;
	}
    GpioConfig(IO_PORT4, IO_PINx3, GPIO_MODE_INPUT);
    k =  GpioGetInput(IO_PORT4, IO_PINx3);                   //开表盖检测口
    if(0x01 == k )
    {
        nKeyValTmp = KEY_BG;
        g_RealMeterCoverFlag=KEY_BG;
    }
        
	GpioConfig(IO_PORTC, IO_PINxE, GPIO_MODE_INPUT);
    j = GpioGetInput(IO_PORTC, IO_PINxE);

	 if(0x00 == j)
	{
      nKeyVal = KEY_DOWN;
    }
	if (KEY_NULL != nKeyValTmp)
    {
        KEY_test(nKeyValTmp);
    }
    
	if (KEY_NULL != nKeyVal)
	{       
             g_nKey_Flg = KEY_test(nKeyVal);
                g_nCmt = 0;       
		g_pKeySamp->nPressCntBak = 0;
		if ((nKeyVal == g_pKeySamp->nKeyValBak))	
		{
			if (g_pKeySamp->nPressCnt < nKeyCntNum)
			{
				g_pKeySamp->nPressCnt++;
			}
			if (g_pKeySamp->nPressCnt >= nKeyCntNum)
			{
				if (g_nContinueKeyFlg)
				{
					if ( (KEY_FN == nKeyVal) || (KEY_FUN2 == nKeyVal))
					{
						
					}
					else
					{
						g_pKeySamp->nKeyVal = nKeyVal;
						g_pKeySamp->nPressCnt = 0;
					}
				}
                                else
				{

				}
			}
                  }
		else
		{
			g_pKeySamp->nKeyVal = KEY_NULL;
			g_pKeySamp->nPressCnt = 0;
		}
	}
	else
	{
		if (g_pKeySamp->nPressCnt >= nKeyCntNum)
		{
			if (g_nContinueKeyFlg)
			{
				if ((KEY_FN == g_pKeySamp->nKeyValBak) || (KEY_FUN2 == g_pKeySamp->nKeyValBak))
				{
					g_pKeySamp->nKeyVal = g_pKeySamp->nKeyValBak;
				}
				else
				{

				}
			}
			else
			{
				g_pKeySamp->nKeyVal = g_pKeySamp->nKeyValBak;
			}
		}
                g_pKeySamp->nPressCnt = 0;
		g_pKeySamp->nPressCntBak++;
		if (g_pKeySamp->nPressCntBak >= 120)
		{
			g_pKeySamp->nKeyVal = KEY_NULL;
			g_pKeySamp->nPressCntBak = 0;
		}
	}*/
                
	g_pKeySamp->nKeyValBak = nKeyVal;
}






/*************************************************************************************
* 函数名:
* 输　入:nType = 1, 取 nKeyVal， 其他 取nKeyVal2
* 输　出:
* 返  回:
* 功  能: 键值获取
**************************************************************************************/
INT8U GetKeyVal(INT8U nType)
{
	INT8U 	nKey = KEY_NULL;
	INT8U	nErr = 0;
	
    OSSemPend(g_pSem[SEM_KEY_IDX], 0, &nErr);
	nKey = g_pKeySamp->nKeyVal;
    
	switch(nKey)
	{
		case KEY_FN:
		case KEY_FUN2:
			
			break;
		default:
			g_pKeySamp->nKeyVal = KEY_NULL;
			break;
	}
	OSSemPost(g_pSem[SEM_KEY_IDX]);
		
	OSTimeDlyHMSM(0, 0, 0, 30);
    //OSTimeDly(5);
	return nKey;
}


/*************************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 键值获取
**************************************************************************************/
INT8U GetKeyVal2(void)
{
	INT8U nKey = KEY_NULL;
	INT8U	nErr = 0;
	
    OSSemPend(g_pSem[SEM_KEY_IDX], 0, &nErr);
	nKey = g_pKeySamp->nKeyVal;
	switch(nKey)
	{
		case KEY_FN:
		case KEY_FUN2:
			g_pKeySamp->nKeyVal = KEY_NULL;
			break;
		default:
			break;
	}
	OSSemPost(g_pSem[SEM_KEY_IDX]);
	
	return nKey;
	
}

void beep(void)
{
	int i;

	FMQ_PFR = 0x00;
	FMQ_DDR = 0x01;
	FMQ_PDOR = 0x01;
	for (i=0;i<80;i++)
	{
		
		Delay_us(350);
		FMQ_PDOR = 0x00;
		Delay_us(350);
		FMQ_PDOR = 0x01;
	}
	FMQ_PDOR = 0x00;
}




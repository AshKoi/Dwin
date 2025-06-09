/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
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

//设置电表日冻结数据
INT8U g_nSetDayDataFlg  = 0;
INT8U g_nDltSetDayBuf[1024] = {0};//INT8U g_nDltSetDayBuf[800] = {0};
//INT8S   g_nAlterUartBpsflg = 1;

static INT32S IdleTaskProc(void)
{
   // INT8U   i   = 0;
    //static INT8U   nSetBSPDelay   = 0;
   // INT8U   nComWH485   = 0;
    //MEM_AC_PRM	sAc_Param;
    
   // memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
    if (g_nSetDayDataFlg)
    {
        Dlt07SetDayData(&g_nDltSetDayBuf[0]);
        g_nSetDayDataFlg = 0;
        memset(&g_nDltSetDayBuf[0], 0, sizeof(g_nDltSetDayBuf));
    }
	
    /*
    if(1 == g_nAlterUartBpsflg)
   {
       INT8U   i   = 0;
    static INT8U   nSetBSPDelay   = 0;
    INT8U   nComWH485   = 0;
   		MEM_AC_PRM	sAc_Param;
        memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
     nSetBSPDelay++;
     if(nSetBSPDelay > 20)
     {
       MemReadAcPrm(F27, &sAc_Param.nCom1Baud, sizeof(sAc_Param.nCom1Baud));
       i =  sAc_Param.nCom1Baud;

      switch (i)
      {
        case 0x02:
          nComWH485 = 1;
          break;
        case 0x04:
          nComWH485 = 2;
          break;
        case 0x08:
          nComWH485 = 3;
          break;
        case 0x10:
          nComWH485 = 4;
          break;
        case 0x20:
          nComWH485 = 6;
          break;
        case 0x40:
          nComWH485 = 10;
          break;
        default:
          nComWH485 = 3;        //g_nUART_BPS_NUM = e_UART_BPS_2400;
          break;
        }
        SetCommCfg(1,nComWH485, 2);        //2400 偶校验
        OpenCommPort(COMM_WH485);
        g_nAlterUartBpsflg = 0;
        nSetBSPDelay = 0;
     }
   }
    */
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 空闲时执行相关操作任务
*********************************************************************************/
void IdleTask(void *pData)
{
    INT8U 		nErr 			= 0;
    
	for (; ;)
    {
        IdleTaskProc();
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}






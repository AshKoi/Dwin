/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0  终端单板程序
*****************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "AppHead.h"
#include "AutoTest.h"
#include "Key.h"

#define GET_KEY_VAL(nKey, nType)	nKey = GetKeyVal(nType)

INT8U           g_nErrBuf[2]    = {0x00,0x1E};
INT8U           g_nTestStep     = 0;

INT8U           g_IrTestFlg = 0;

extern INT8U g_nHardNo;

/*
*****************************************************************************
* 函数名:MainF5Proc
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:F5
*****************************************************************************
*/
INT32S  MainF5Proc(INT8U nCom, INT8U nCmd)
{
    INT8U           nBuf[100]  	    = {0};
    INT8U*          pBuf 			= &nBuf[0];
	pGW376_1_PROT	pGw3761Prot	    = (pGW376_1_PROT)pBuf;
	INT8U*			pData			= NULL;
	INT32S			nLen			= 0;
	INT8U			nFlg			= 0;
    INT16U          nPn             = 0;
    INT8U           nFn             = 0;
    //INT8U           nCom            = COMM_ERR;
    
    g_nAsynFlg = 1;
    //SetCommCfg(2, 6, 2);        //232口，波特率：115200
    
    //
    memset(pBuf, 0, sizeof(nBuf));
    GB3761PnFn2DADT(0, F5, &pBuf[nLen]);
    nLen += 4;
    pBuf[nLen++] = nCmd;        //所有的都跳
    nLen = Gw3761MakeFrm(0x4B, 0xDF, pBuf, nLen, sizeof(nBuf));
	nLen = Gw3761Proc(nCom, pBuf, nLen, sizeof(nBuf));
    
    pData = pGw3761Prot->nDadtData;
    //if (GB3761DaDt2PnFn(pData, &nPn, NULL, &nFn, NULL) < 0)
    {
        //nFlg = 1;
    }
    //if (0 != nPn)
    {
        //nFlg = 1;
    }
    //if (F5 != nFn)
    {
        //nFlg = 1;
    } 
    
    g_nAsynFlg = 0;
    
    if (nFlg == 1)
    {
        return -1;    
    }
    
    return 0;    
}

/******************************************************************************
* 函数名:FlashTest
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:FLASH测试
*****************************************************************************/
INT32S FlashTest(void)
{
	#define COMP_LEN	200
    #define FLH_ADDR	(4096 - 100)
    INT16U	i;
	INT8U 	sTmp[COMP_LEN];
	INT8U 	sRmp[COMP_LEN];

	for (i=0;i<COMP_LEN;i++)
    {
        sTmp[i] = i;
    }
		
	if( _API_FLASH_EraseWrite(  sTmp, FLH_ADDR,COMP_LEN ) < COMP_LEN )		// 向指定地址写入n字节
    {
        return -1;
    }
		
	if( _API_FLASH_Read( sRmp, FLH_ADDR, COMP_LEN ) == 0 )
    {
        return -1;
    }
		
	if(memcmp(sTmp,sRmp,COMP_LEN)!=0)
    {
        return -1; 
    }
    
	return 0;
}

/******************************************************************************
* 函数名:FramTest
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:铁电测试
*****************************************************************************/
INT32S FramTest(void)
{
    #define EEPROM_TEST_SIZE		1024
  
	INT8U  sTmp[EEPROM_TEST_SIZE]; 
	INT32S iRet;
	INT32U addr, len;

	memset( sTmp, 0xA5, EEPROM_TEST_SIZE );
	addr = STR_OFF(pFRAM_STORE, nPos);
	len = EEPROM_TEST_SIZE;

	iRet = _API_FRAM_Write( addr, sTmp, len );
	if( iRet != len )
    {
        return -1;
    }
		
	memset( sTmp, 0, EEPROM_TEST_SIZE );
	iRet =  _API_FRAM_Read( addr, sTmp, len );
	if( iRet != len )
    {
        return -1;
    }
    
    if (!IsAllEqualVal(sTmp, len, 0xA5))
    {
        return -1;
    }
    
	//////////////////////////////////////////////////////
	memset( sTmp, 0x5A, EEPROM_TEST_SIZE );
	addr = STR_OFF(pFRAM_STORE, nPos);
	len = EEPROM_TEST_SIZE;
	iRet = _API_FRAM_Write( addr, sTmp, len );
	if( iRet != len )
    {
       return -1; 
    }
			
	memset( sTmp, 0, EEPROM_TEST_SIZE );
	iRet =  _API_FRAM_Read( addr, sTmp, len );
	if( iRet != len )
    {
       return -1;  
    }
    
    if (!IsAllEqualVal(sTmp, len, 0x5A))
    {
       return -1; 
    }
    
	return 0;
}

/******************************************************************************
* 函数名:ClockTest
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:时钟测试
*****************************************************************************/
INT32S ClockTest(void)
{
    STD_TIME stime;
    STD_TIME sTmptime;
    INT16S  sRet;
    
    //g_nHardNo = 0;
    
    memset(&stime, 0, sizeof(stime));
    memset(&sTmptime, 0, sizeof(sTmptime));
    stime.nYear = 0x14;
    stime.nMon = 0x12;
    stime.nDay = 0x15;
    stime.nHour = 0x12;
    stime.nMin = 0x12;
    stime.nSec = 0x12;
    
    RTCDateTimeWrite(&stime);
    OSTimeDlyHMSM( 0, 0, 2, 0);
    stime.nSec = 0x14;
    
    sRet = RTCDateTimeRead(&sTmptime);
    
    if (memcmp((INT8U*)&sTmptime, (INT8U*)&stime, 6) != 0)
    {
        return -1; 
    }
    
    return 0;
}

/******************************************************************************
* 函数名:IR_test
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:红外测试
*****************************************************************************/
INT32S IR_test(void)
{
    INT8U		l_buff[30] = {0};
    INT8U       nCom       = COMM_IR;
    INT8S       nLen       = 0;
	INT8U		nTxTmpBuf[] = {0x68,0xAA,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0x68 ,0x13 ,0x00 ,0xDF ,0x16};
    
    g_IrTestFlg = 0;
    
    COM_PEND_SEM(nCom);
    //发送一段报文
	WriteCommTest( COMM_IR, nTxTmpBuf, 12);
	OSTimeDlyHMSM( 0, 0, 0, 300 );
	memset(l_buff,0x00,sizeof(l_buff));
	ReadCommMs(COMM_IR, l_buff, 100, 500, 500);
    
    if( memcmp(l_buff,nTxTmpBuf,12)!=0x00)
    {
        g_IrTestFlg = 1;
        WriteCommTest( COMM_IR, nTxTmpBuf, 12);
        OSTimeDlyHMSM( 0, 0, 0, 300 );
        memset(l_buff,0x00,sizeof(l_buff));
        ReadCommMs(COMM_IR, l_buff, 100, 500, 500);
        
        COM_POST_SEM(nCom);
        
        if( memcmp(l_buff,nTxTmpBuf,12)==0x00)
        {
            //GuiwinTextOut(nX, nY, 0,"通信成功");		
        }
        else
        {
            //GuiwinTextOut(nX, nY, 0,"通信失败");	
            //GuiwinTextOutAuto("红外错误");
          return -1;
          
        }
    }
    else
    {
        //GuiwinTextOut(nX, nY, 0,"红外短路");	
        //GuiwinTextOutAuto("红外错误:短路"); 
        return  -1;
    }
    return 0;
}

/******************************************************************************
* 函数名:GPRS_test
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:GPRS测试
*****************************************************************************/
INT32S GPRS_test(void)
{
    INT8U   nCom  = COMM_GPRS;
    INT8U   nLen  = 0;
    INT8U   nBuf[100] = {0};
    INT32S  ret = -1;
    
    WriteComm( COMM_GPRS, "AT\r", 3);
	OSTimeDlyHMSM(0, 0, 0, 300);
        
    memset(nBuf,0,sizeof(nBuf));
    nLen = ReadCommMs(COMM_GPRS, nBuf, 100, 500, 500);
    if (nLen == 0)
    {
        return -1;
    }
    return 0;
}

/******************************************************************************
* 函数名:ERR_BROAD_test
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:故障板测试
*****************************************************************************/
INT32S ERR_BROAD_test(void)
{
    return MainF5Proc(COMM_ERR, 0xFF);
}

/******************************************************************************
* 函数名:RS4851_test
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:4851测试
*****************************************************************************/
INT32S RS4851_test(void)
{
    return MainF5Proc(COMM_WH485, 0xFF);
}

/******************************************************************************
* 函数名:RS4852_test
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:4852测试
*****************************************************************************/
INT32S RS4852_test(void)
{
    return MainF5Proc(COMM_WH4852, 0xFF);
}



/******************************************************************************
* 函数名:CTRL_BROAD_test
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:控制板板测试
*****************************************************************************/
INT32S CTRL_BROAD_test(void)
{
    INT8U	l_buff[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x32, 0x32, 0x47, 0x16};
    INT8U	l_buff2[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x33, 0x33, 0x49, 0x16};
	INT8U   l_Tmpbuff[30] = {0};
    INT8U	l_ret = 0;
    
    WriteComm(COMM_ZB, l_buff, 14);
    l_ret = ReadCommMs(COMM_ZB, l_Tmpbuff, 100, 500, 500);
    
    if (l_ret == 0)
    {
        return -1;
    }
    return 0;
}

INT8U   g_nCntFlgs = 0;

/******************************************************************************
* 函数名:KEY_test
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:KEY测试
*****************************************************************************/

INT32S KEY_test(INT8U nKey)
{            
     if(KEY_UP == nKey)
     {
        ClrBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), KEY_UP_TEST);
     }         
     if(KEY_DOWN == nKey)
     {
        ClrBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), KEY_DOWN_TEST);
     }
     if(KEY_BG == nKey)
     {
        ClrBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), KEY_BG_TEST);
     }
     if(KEY_JG == nKey)
     {
        ClrBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), KEY_JG_TEST);
     }
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) || (TRIPHASE_METER_220ZT == g_nHardType))
    {
     if (g_nErrBuf[1] == 0)
     {
            if (g_nCntFlgs == 0)
            {
          g_nTestStep++;
     }            
            g_nCntFlgs = 1;
       }            
    }
    else if(SINGLEPHASE_METER == g_nHardType)
    {
     if (g_nErrBuf[1] == 0x12)
     {
            if (g_nCntFlgs == 0)
            {
          g_nTestStep++;
     }  
            g_nCntFlgs = 1;
            
       }  
    }
}

/******************************************************************************
* 函数名:AutoTestProc
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:自动检测---终端单板程序
*****************************************************************************/
INT32S  AutoTestProc(void)
{
    INT32S  nRet    = 0;
    
    g_nTestStep = 0;
    memset(&g_nErrBuf[0], 0, sizeof(g_nErrBuf));
    
    //1.FLASH测试
    if (FlashTest() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), FLASH_TEST);
    }
    g_nTestStep++;
    
    //2.铁电测试
    if (FramTest() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), FRAM_TEST);
    }
    g_nTestStep++;
    
    
    //3.时钟测试
    if (ClockTest() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), CLOCK_TEST);
    }
    g_nTestStep++;
    
    //4.红外测试--COMM_IR
    if (IR_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), IR_TEST);
    }
    g_nTestStep++;
    
    //5.GPRS测试--COMM_GPRS
    if (GPRS_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), GPRS_TEST);
    }
    g_nTestStep++;
    
    //6.故障板测试--COMM_ERR
    if (ERR_BROAD_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), ERR_TEST);
    }
    g_nTestStep++;
    //7.控制板测试
    if (CTRL_BROAD_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), CTRL_TEST);
    }
    g_nTestStep++;
    
    //8.4851测试
    if (RS4851_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), RS4851_TEST);
    }
    g_nTestStep++;
    
    //9.4852测试
    if (RS4852_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), RS4852_TEST);
    }
    g_nTestStep++;
    
    
    
    g_nAutoTestFlg = 0;
    
    return nRet;
}
/******************************************************************************
* 函数名:AutoTestProc1
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:自动检测---电表单板程序
*****************************************************************************/
INT32S  AutoTestProc1(void)
{
    INT32S  nRet    = 0;
    
    g_nTestStep = 0;
    memset(&g_nErrBuf[0], 0, sizeof(g_nErrBuf));
    g_nErrBuf[1] = 0x1E;
    g_nCntFlgs = 0;
    
    //1.FLASH测试
    if (FlashTest() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), FLASH_TEST);
    }
    g_nTestStep++;
    
    //2.铁电测试
    if (FramTest() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), FRAM_TEST);
    }
    g_nTestStep++;
    
    
    //3.时钟测试
    if (ClockTest() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), CLOCK_TEST);
    }
    g_nTestStep++;
    
    //4.红外测试--COMM_IR
    if (IR_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), IR_TEST);
    }
    g_nTestStep++;
    
    
    //5.4851测试
    /*if (RS4851_test() < 0)
    {
        SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), RS4851_TEST);
    }
    g_nTestStep++;*/
  
    //AcPrmPrmInit();       //在做出厂检测时放开
    g_nTestStep++;
    

    //nKey_Flg[0] = 0x0f;
    //g_nKey_Flg = -1;
    
    //g_nTestStep++;
    
    
    
    //9.4852测试
 //   if (RS4852_test() < 0)
  //  {
  //      SetBufBit(&g_nErrBuf[0], sizeof(g_nErrBuf), RS4852_TEST);
  //  }
  //  g_nTestStep++;

    g_nAutoTestFlg1 = 0;
    
    return nRet;
}


/******************************************************************************
* 函数名:WriteCommTest
* 输　入: 
* 输  出: 
* 返  回: 
* 功  能:红外自发自收
*****************************************************************************/
INT32S WriteCommTest( INT8U port, INT8U *buffer, INT32U bufsize )
{
	INT8U	i;
	INT32S  ret;
	
	if( bufsize == 0 )
		return 0;
	if (port == COMM1)
	{
		MFS_UART0RXDisable(MFS_Ch1);      //ESAM    发送数据的时候关闭接收，发完再开启接收  
	}
	else if (port == COMM2)
	{
		//#ifndef TEST_IR_TXRX             	//红外在测试的时候是自发自收
        if (!g_nAutoTestFlg1)
        {
            MFS_UART0RXDisable(MFS_Ch2);      //红外  发送数据的时候关闭接收，发完再开启接收    
        } 
        //#endif  
        if (g_IrTestFlg)
        {
            IrCommPwmOut( ENABLE );    
        }
	}
	else if((port == COMM6))
	{
		GpioSetOutput(IO_PORT4, IO_PINxC);	//初始为高电平状态
        OSTimeDlyHMSM(0, 0, 0 , 30);
	}
		
	ret = CommPutStr( port, buffer, bufsize, OS_TICKS_PER_SEC*3 );
    if(port == COMM1)       				//ESAM  在发送完成后开启接收
    {
        while(MFS_UART0StatusGetTXBusIdle(MFS_Ch1) != SET)
		{
			OSTimeDlyHMSM(0, 0, 0, 10); 	//TX发送完成   才开始接收数据
			i++;
			if(i==10)
				return 0;
		}
        MFS_UART0RXEnable(MFS_Ch1);                               
    }
    else if(port == COMM2)      			//红外 在发送完成后开启接收
    {
        OSTimeDlyHMSM(0, 0, 0, 50);
        if (g_IrTestFlg)
        {
            IrCommPwmOut( DISABLE );     
        }
        
        //#ifndef TEST_IR_TXRX   
        if (!g_nAutoTestFlg1)
        {
            MFS_UART0RXEnable(MFS_Ch2);    
        }
        //#endif       
    }
	else if((port == COMM6))				//485
	{
		OSTimeDlyHMSM(0, 0, 0 , 10);
		GpioClearOutput(IO_PORT4, IO_PINxC);	
        OSTimeDlyHMSM(0, 0, 0 , 10);
	}

	if( ret == UART_OK )
		return (INT32S)bufsize;
	else
		return 0;
}












#include "GB698GetRequest.h"
#include "Gw698.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Reset.h"
#include "MemData.h"
#include "SoftTimer.h"
#include "rmotlink.h"
#include "MemTbl.h"
#include "PowerOn.h"
#include "SecTask.h"
#include "IdleTask.h"


//INT8U   nRDJTimeFlg = 0;
INT8U      n_RSDSelector9flg = 0;
/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: GB698GetRequest
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数查询
*********************************************************************************/
INT8S GB698GetRequest(pAPP_COMM pComm)
{
    //INT8U     	        nData[720] = {0};//nData[256];  考虑读取日冻结数据量大，抄表485缓冲区为1024，接收指针预估占取300字节，占定700  不能大于900否则电表回出现死机现象，此故障为缓冲区大小溢出 22.9.4 NDY
    INT16U              nDataLen = 0;       
    INT8S               nRet;
    INT8U               nGetReqLen    = GW698_APDU_LEN1;
    pGB698_FRM_MAP      pMap        = NULL;
    pGB698_FRM_INFO     pInfo  = NULL;
    INT32U              nOffset  = 0;
    INT8U               nRequestObject = 0;
    INT8U               i = 0;   
    
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    
    //使用全局变量
    memset(g_nDltSetDayBuf,0,sizeof(g_nDltSetDayBuf));

    pInfo = (pGB698_FRM_INFO)(pComm->pFrmInfo); //存放 帧的一些信息
    pMap    = (pGB698_FRM_MAP)&pComm->pRxFrmBuf[0]; //接受到的帧数据
    nRequestObject =  pMap->nRequestObject;
    
    if( 1 == nRequestObject)
    {//nRequestObject 读取一个对象 ==1   ： 
        memcpy(&g_nDltSetDayBuf[nOffset], &(pMap->nData[0]), OAD_LEN);//40 00 02 00
        nOffset += 4;
        nDataLen = GB698GetRequestMakeFrm(pMap->nData, &g_nDltSetDayBuf[nOffset + 1], 0xFFFFFFFF, NULL);  //40 00 02 00 nn 1c 07 e4 09 17 0a 21 33
        if(nDataLen < 2)
        {
             g_nDltSetDayBuf[nOffset] = 0;                      //DAR     后跟随错误信息
        }
        else
        {
            g_nDltSetDayBuf[nOffset] = 1;    //数据    后跟随Data  40 00 02 00 01 1c 07 e4 09 17 0a 21 33
        }
        nOffset ++;
        nOffset += nDataLen;
        
    }
    else if( 2 == nRequestObject)
    {
        g_nDltSetDayBuf[0] = pMap->nData[0];
        nOffset ++; 
        for(i = 0; i < pMap->nData[0]; i ++)
        {
            memcpy(&g_nDltSetDayBuf[nOffset], &(pMap->nData[1]) + OAD_LEN*i, OAD_LEN);
            nOffset += 4;
            nDataLen = GB698GetRequestMakeFrm(&(pMap->nData[1]) + OAD_LEN*i, &g_nDltSetDayBuf[nOffset + 1], 0xFFFFFFFF, NULL);
            if(nDataLen < 2)
            {
                 g_nDltSetDayBuf[nOffset] = 0;                      //DAR     后跟随错误信息
            }
            else
            {
                g_nDltSetDayBuf[nOffset] = 1;    //数据    后跟随Data
            }
            nOffset ++;
            nOffset += nDataLen;
        }
    }
    else if( 3 == nRequestObject)
    {
        memcpy(&g_nDltSetDayBuf[nOffset], &(pMap->nData[0]), OAD_LEN);
        nOffset += 4;
        nRet = GB698GetRecordResp(pMap->nData, &g_nDltSetDayBuf[nOffset], &nDataLen);

        nOffset += nDataLen;
    }
    //只将数据部分复制到 TXbuf里
    pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[g_nHeadLen + nGetReqLen], &g_nDltSetDayBuf[0], nOffset);
    
    //使用完清零，此数组为设置日冻结数组
    memset(g_nDltSetDayBuf,0,sizeof(g_nDltSetDayBuf));
    
    return RET_ERR_OK;
}
                          
INT8S GB698GetRecordResp(INT8U *pOAD, INT8U *pData, INT16U *pDatalen)
{
    INT16U      nOI;  //INT16U
    INT32U      nOAD;
    //INT8S       nLen = 0;
    INT8U       nDatalen = 0;
    INT8U       nRcsd_Len= 0;
    //INT8U       nROAD_Len= 0;
    INT8U       nCopyStart = 0;
    INT8U       nOAD_Arr[15][5] = {0};
    INT8U       nCsdType = 0;
    INT8U       nTemp[2] = {0};
    INT8U*      pRecBuf  = &pOAD[4];

    //INT8U       ii = 0;
    MEM_METER_PRM   sMeterPrm;
    pMEM_METER_PRM  pMeterPrm = &sMeterPrm;
    //MT_REPORT_DATA          sAc_Report;
    INT8U       i = 0;
    INT8U       nOadLen = 0;
    INT8U       nRecordLen = 0;
    INT16U       nTxLen  = 0;
    INT8U       nErrLen = 0;
    STD_TIME    sDjTime;
    pSTD_TIME   pDjTime = &sDjTime;
    STD_TIME    sDjTime2;
    pSTD_TIME   pDjTime2 = &sDjTime2;
    INT8S       nDays = 0;
    
    nOI = ((pOAD[0] << 8) | pOAD[1]);
    nOAD = ((pOAD[0]<<24)|(pOAD[1]<<16)|(pOAD[2]<< 8)|pOAD[3]);
    switch (nOI)
    {
        case 0x3000:
        case 0x3001:
        case 0x3002:
        case 0x3003:
        case 0x3004:
        case 0x3005:
        case 0x3006:   
        case 0x5000:
        case 0x5002:
        case 0x5004:
        case 0x5006:
              n_RSDSelector9flg = 0;
              if( 1 == pRecBuf[nDatalen ++] )  //后期需要重写方法
              {
                    nDatalen += 4;      //OAD    数据冻结时间OAD
                    
                    if(pRecBuf[nDatalen ++] != e_date_time_s)
                    {
                        pData[nTxLen++] = 0;
                        pData[nTxLen++] = 0;
                        pData[nTxLen ++] = e_RefuseRW;
                        *pDatalen = nTxLen;
                        return 1;
                    }
                    nRecordLen = 1;
                    memcpy(&nTemp[0], &pRecBuf[nDatalen], 2);  
                    sDjTime.nYear = INT8U_TO_BCD((INT8U)((INT16U)(nTemp[0]) * 256 + (INT16U)nTemp[1] - 2000));
                    nDatalen += 2;
                    sDjTime.nMon = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nDay = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nHour = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nMin = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nSec = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    
                    nCopyStart = nDatalen;
                    nRcsd_Len = pRecBuf[nDatalen ++];
                    pData[nTxLen ++] = nRcsd_Len;
                    //以下部分为源代码，之前被注释掉，放在代码外，为不影响事件类数据读取取消注释
                   for(i = 0; i < nRcsd_Len; i ++)
                    {
                        nCsdType = pRecBuf[nDatalen];
                        if(nCsdType == 0)
                        {
                            memcpy(&nOAD_Arr[nOadLen ++], &pRecBuf[nDatalen], 5);   //Type+OAD
                            nDatalen += 5;
                            nTxLen += 5;
                        }
                        
                    }
                    memcpy(pData, pRecBuf + nCopyStart, nTxLen);
                    pData[nTxLen++] = 1;
                    pData[nTxLen++] = nRecordLen;
                    for(i = 0; i < nRecordLen; i ++)
                    {
                        nTxLen += GB698RecordDataProc(nOAD_Arr,nOadLen, nOAD, pDjTime, &pData[nTxLen]);
                        if(pData[5*nRcsd_Len+3] == 0)  //读取日冻结数据时标不对置一，直接置0返回   22.8.28
                        {
                            pData[5*nRcsd_Len+ 2] = 0;
                            //nTxLen -= 2; 
                        }
                    }
              }
              else if( 2 == pRecBuf[nDatalen -1] )  //方法2  22.8.28 NDY  连云港项目
              {
                    nDatalen += 4;      //OAD    数据冻结时间OAD
                    
                    if((pRecBuf[nDatalen ++] != e_date_time_s)&&((pRecBuf[nDatalen + 8] != e_date_time_s)))
                    {
                        pData[nTxLen++] = 0;
                        pData[nTxLen++] = 0;
                        pData[nTxLen ++] = e_RefuseRW;
                        *pDatalen = nTxLen;
                        return 1;
                    }
                    nRecordLen = 1;
                    //起始时间
                    memcpy(&nTemp[0], &pRecBuf[nDatalen], 2);  
                    sDjTime2.nYear = INT8U_TO_BCD((INT8U)((INT16U)(nTemp[0]) * 256 + (INT16U)nTemp[1] - 2000));
                    nDatalen += 2;
                    sDjTime2.nMon = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime2.nDay = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime2.nHour = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime2.nMin = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime2.nSec = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    nDatalen ++;
                    //结束时间
                    memcpy(&nTemp[0], &pRecBuf[nDatalen], 2);  
                    sDjTime.nYear = INT8U_TO_BCD((INT8U)((INT16U)(nTemp[0]) * 256 + (INT16U)nTemp[1] - 2000));
                    nDatalen += 2;
                    sDjTime.nMon = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nDay = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nHour = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nMin = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    sDjTime.nSec = INT8U_TO_BCD(pRecBuf[nDatalen]);
                    nDatalen ++;
                    
                    if(pRecBuf[nDatalen ++] != e_TI)
                    {
                        pData[nTxLen++] = 0;
                        pData[nTxLen++] = 0;
                        pData[nTxLen ++] = e_RefuseRW;
                        *pDatalen = nTxLen;
                        return 1;
                    }
                    nDatalen += 2;
                    nDays = GetDaysFrom2KYear(pDjTime) - GetDaysFrom2KYear(pDjTime2);
                    if(nDays != 0)
                    {
                        if(( nDays != pRecBuf[nDatalen])||(nDays < 0)) //可删除？
                        {
                            pData[nTxLen++] = 0;
                            pData[nTxLen++] = 0;
                            pData[nTxLen ++] = e_RefuseRW;
                            *pDatalen = nTxLen;
                            return 1;
                        }
                    }
                    nDatalen ++;
                    //占时定义只有1天
                    //nRecordLen =  nDays;
                    
                    nCopyStart = nDatalen;
                    nRcsd_Len = pRecBuf[nDatalen ++];
                    pData[nTxLen ++] = nRcsd_Len;
                    
                   for(i = 0; i < nRcsd_Len; i ++)
                    {
                        nCsdType = pRecBuf[nDatalen];
                        if(nCsdType == 0)
                        {
                            memcpy(&nOAD_Arr[nOadLen ++], &pRecBuf[nDatalen], 5);   //Type+OAD
                            nDatalen += 5;
                            nTxLen += 5;
                        }
                        
                    }
                    memcpy(pData, pRecBuf + nCopyStart, nTxLen);
                    pData[nTxLen++] = 1;
                    pData[nTxLen++] = nRecordLen;
                    for(i = 0; i < nRecordLen; i ++)
                    {
                        nTxLen += GB698RecordDataProc(nOAD_Arr,nOadLen, nOAD, pDjTime2, &pData[nTxLen]);
                        //pDjTime2->nDay = pDjTime2->nDay + 1; //nDays天数大于2时使用
                        if(pData[5*nRcsd_Len+3] == 0)  //直接置0返回   22.8.28
                        {
                            pData[5*nRcsd_Len+ 2] = 0;
                            //nTxLen -= 2; 
                        }
                    }
                    
              }
              else if( 9 == pRecBuf[nDatalen -1] )  //方法9  
              {
                  n_RSDSelector9flg = pRecBuf[nDatalen ++];         //上几次事件记录
                  nCopyStart = nDatalen;
                  nRcsd_Len = pRecBuf[nDatalen ++];
                  if(nRcsd_Len == 0)
                  {
                        pData[nTxLen++] = 0;
                        pData[nTxLen++] = 0;
                        pData[nTxLen ++] = e_RefuseRW;
                        *pDatalen = nTxLen;
                        return 1;
                  }
                  pData[nTxLen ++] = nRcsd_Len;
                  nRecordLen = 1;
                  for(i = 0; i < nRcsd_Len; i ++)
                  {
                      nCsdType = pRecBuf[nDatalen];
                      if(nCsdType == 0)
                      {
                          memcpy(&nOAD_Arr[nOadLen ++], &pRecBuf[nDatalen], 5);   //Type+OAD
                          nDatalen += 5;
                          nTxLen += 5;
                      }
                      
                  }
                  memcpy(pData, pRecBuf + nCopyStart, nTxLen);
                  pData[nTxLen++] = 1;
                  pData[nTxLen++] = nRecordLen;
                  for(i = 0; i < nRecordLen; i ++)
                  {
                      nTxLen += GB698RecordDataProc(nOAD_Arr,nOadLen, nOAD, pDjTime, &pData[nTxLen]);
                  }
              }
            break;

        case 0x301B:    //开盖事件
            {
                if( pOAD[2]==2 )
                {
                    if( 9 == pRecBuf[nDatalen ++] )   //方法9
                    {
                        nTxLen = GB698GetRecordEventMakeFrm(F30,&pData[nTxLen]);
                    }
                }
            }
            break;
        case 0x301F:    //跳闸事件
            {
                if( pOAD[2]==2 )
                {
                    if( 9 == pRecBuf[nDatalen ++] )   //方法9
                    {
                        nTxLen = GB698GetRecordEventMakeFrm(F31,&pData[nTxLen]);
                    }
                }
            }
            break;
        case 0x3020:
            if( 2 == pOAD[2] )   //事件记录表
            {
                if( 9 == pRecBuf[nDatalen ++] )   //方法9
                {
                    nTxLen = GB698GetRecordEventMakeFrm(F32,&pData[nTxLen]);
                }
                
            }
            break;
        case 0x3026:    //密钥更新事件
            {
                if( pOAD[2]==2 )
                {
                    if( 9 == pRecBuf[nDatalen ++] )   //方法9
                    {
                        nTxLen = GB698GetRecordEventMakeFrm(F33,&pData[nTxLen]);
                    }
                   
                }
            }
            break;
        case 0x302b:    //负荷开关误触动事件
            {
                if( pOAD[2]==2 )
                {
                    if( 9 == pRecBuf[nDatalen ++] )   //方法9
                    {
                        nTxLen = GB698GetRecordEventMakeFrm(F34,&pData[nTxLen]);
                    }
                }
            }
            break;
        case 0x302C:    //电源异常事件
            {
                if( pOAD[2]==2 )
                {
                    if( 9 == pRecBuf[nDatalen ++] )   //方法9
                    {
                        nTxLen = GB698GetRecordEventMakeFrm(F35,&pData[nTxLen]);
                    }
                }
            }
            break;
        case 0x302E:    //时钟故障事件
            {
                if( pOAD[2]==2 )
                {
                    if( 9 == pRecBuf[nDatalen ++] )   //方法9
                    {
                        nTxLen = GB698GetRecordEventMakeFrm(F36,&pData[nTxLen]);
                    }
                }
            }
            break;
        case 0x302F:    //计量芯片故障事件
            {
                if( pOAD[2]==2 )
                {
                    if( 9 == pRecBuf[nDatalen ++] )   //方法9
                    {
                        nTxLen = GB698GetRecordEventMakeFrm(F37,&pData[nTxLen]);
                    }
                }
            }
            break;
            
        default:break;
    }
    
    if( (nTxLen - nErrLen) <= (2 + nOadLen))
    {
        pData[nErrLen ++ ] = 0;
        pData[nErrLen ++ ] = pData[--nTxLen];
        
        *pDatalen = nErrLen;
    }
    else
    {
        *pDatalen = nTxLen;
    }
    
    return 1;
}
                           
INT32S  GB698RecordDataProc(INT8U nOAD_Arr[][5], INT8U	nOadLen, INT32U  nOAD, pSTD_TIME pDjTime, INT8U  *pData)
{
    INT8U   i = 0;
    INT16U   nDatalen  = 0;

    for(i = 0; i < nOadLen; i ++)
    {
        if( 0 == nOAD_Arr[i][0] )
        {
            nDatalen += GB698GetRequestMakeFrm(&nOAD_Arr[i][1], pData + nDatalen, nOAD, pDjTime);       
        }
    }
    return  nDatalen;
    
}


INT8S GB698GetRequestMakeFrm(INT8U *OAD, INT8U *pData, INT32U  nMasterOAD, pSTD_TIME pDjTime)
{                  
    INT16U      nOI;
    INT8U       nDataLen = 0;
    STD_TIME    sTime;
    INT8U       nData[100];
    INT8U      *pBuf  = &nData[0];
    pSTD_TIME   pTimeVld = &sTime;
    STD_TIME    sTmpTime;
    INT16U      nTemp;
    INT8U       nTempBuf[50];
    //INT8U       nNum = 0;
    INT8U       i =0;
    INT32U      nConvert = 0;
    INT8U       nArrayNum = 0;
    INT32U      nSec = 0;
    //INT32U      nDays = 0;
    INT32U      nFn;
    INT8U*      pBuf2 = pData ;  
    INT8U       nRDJTimeFlg = 0;
    INT8U       ii = 0;
    INT32U      nEveFn;
    INT32U      nSJXHNum = 0;
    INT32U       nNumFn;
    INT32S nEndSec   = 0;
    INT32S nStartSec = 0;
    INT32U			     pRcvNumb =0;
    INT8U       nSucflg = 0;
    STD_TIME    sTime2;
    pSTD_TIME   pTimeVld2 = &sTime2;
    STD_TIME    sEveTime;
    pSTD_TIME   pEveTime = &sEveTime;
    
    MT_DGN_ENERGY_DATA	sAcEn;
    MT_DGN_Q_ENERGY_DATA  sAcQEn;
    MEM_AC_DAY_DATA     sAcHistDayData;
    MEM_AC_MIN_DATA      sAcHistMinData;
    //MEM_AC_RDAY_DATA     sAcHistData;
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;  
    MT_DGN_TRI_ENERGY_DATA  sAcTriEn;
    MT_DGN_TRI_Q_EN_DATA  sAcTriQEn;
    MEM_AC_PRM	sAc_Param;
    MT_REPORT_DATA          sAc_Report;
    MT_DGN_DEMAND_DATA      sAcXLEnergy;
    //MEM_EVE_DATA     sAcEveDayData;
	
    //memset(&nEveData, 0, LEN_AC_EVENT);
    memset(&sAcTriEn, 0, sizeof(sAcTriEn));
    memset(&sTmpTime, 0, sizeof(STD_TIME));
    memset(pTimeVld, 0, sizeof(STD_TIME));
    //memset(&sAcHistData, 0, sizeof(sAcHistData));
    memset(&sAcHistMinData, 0, sizeof(sAcHistMinData));
    memset(&sAcHistDayData, 0, sizeof(sAcHistDayData));
    memset(&sAcTriQEn, 0, sizeof(MT_DGN_TRI_Q_EN_DATA));
    memset(&sAcXLEnergy, 0, sizeof(MT_DGN_DEMAND_DATA));
    //memset(&sAcEveDayData, 0, sizeof(sAcEveDayData));
    
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
    MemReadAcRealData(F3, &sAcTriEn, sizeof(sAcTriEn));
	MemReadAcRealData(F4, &sAcTriQEn, sizeof(sAcTriQEn));
    
    nOI = ((OAD[0] << 8) | OAD[1]);
    if( 0xFFFFFFFF != nMasterOAD)
    {
        switch(nMasterOAD&0xFFFF0000)
        {
            case  0x50000000:
                pDjTime = NULL;
                if( ((nOI & 0x00F0 ) >> 4) > 0 && ((nOI & 0x00F0 ) >> 4) <= 4 )
                {
                    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
                }
                else if(((nOI & 0x00F0 ) >> 4) > 4 && ((nOI & 0x00F0 ) >> 4) <= 8)
                {
                    MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
                }
                RTCDateTimeRead(pTimeVld);
                break;
            case  0x50020000:
                if(n_RSDSelector9flg != 0)
                {
                    memset(&sAcHistMinData, 0, sizeof(sAcHistMinData));
                    MemReadAcHistDataProtProc(F5, n_RSDSelector9flg, &sAcHistMinData, sizeof(sAcHistMinData));
                    memcpy(&sAcEn, &sAcHistMinData.sAcEnergy, sizeof(sAcEn));
                    memcpy(&sAcXLEnergy, &sAcHistMinData.sAcDemand, sizeof(sAcXLEnergy));
                    memcpy(&pTimeVld->nMin, &sAcHistMinData.sTime, sizeof(STD_TIME));
                    memcpy(&nSJXHNum, &sAcHistMinData.sPriority, 4); //读取次数
                    if(!IsVldStdTime(pTimeVld))
                    {
                        nRDJTimeFlg = 1;
                    }
                }
                else
                {
                    RTCDateTimeRead(&sTmpTime);
                    if(GetDiffTimeSecs(&sTmpTime, pDjTime, &nSec) < 0)
                    {
                        break;
                    }
                    if(nSec < 0)
                    {
                        break;
                    }
                    
                    for(int ii = 0; ii<31; ii++)
                    {
                        memset(&sAcHistMinData, 0, sizeof(sAcHistMinData));
                        MemReadAcHistDataProtProc(F5, ii, &sAcHistMinData, sizeof(sAcHistMinData));
                        if(( 0 == memcmp(&pDjTime->nDay,&sAcHistMinData.sTime.nDay,3))&&( 0 > memcmp(&pDjTime->nMin,&sAcHistMinData.sTime.nMin,4)))
                        {
                            memcpy(&sTmpTime.nMin, &sAcHistMinData.sTime.nMin, sizeof(FORMAT15));
                            nEndSec = GetMinsFrom2KYear(&sTmpTime);
                            nStartSec = GetMinsFrom2KYear(pDjTime);
                            pRcvNumb =nEndSec-nStartSec;
                            if(pRcvNumb <=0x0f)
                            {
                                ii = 32;
                                nSucflg = 1;
                            }
                            
                        }
                    }
                    if(nSucflg ==1)
                    {
                        memcpy(&sAcEn, &sAcHistMinData.sAcEnergy, sizeof(sAcEn));
                        memcpy(&sAcXLEnergy, &sAcHistMinData.sAcDemand, sizeof(sAcXLEnergy));
                        memcpy(&pTimeVld->nMin, &sAcHistMinData.sTime, sizeof(STD_TIME));
                        memcpy(&nSJXHNum, &sAcHistMinData.sPriority, 4); //读取次数 
                    }
                    else
                    {
                        nRDJTimeFlg = 1;
                    }
                }
                break;
            case  0x50040000:
                {
                    if(n_RSDSelector9flg != 0)
                    {
                        memset(&sAcHistDayData, 0, sizeof(sAcHistDayData));
                        MemReadAcHistDataProtProc(F1, n_RSDSelector9flg, &sAcHistDayData, sizeof(sAcHistDayData));
                        memcpy(&sAcEn, &sAcHistDayData.sAcEnergy, sizeof(sAcEn));
                        memcpy(&sAcXLEnergy, &sAcHistDayData.sAcDemand, sizeof(sAcXLEnergy));
                        memcpy(&pTimeVld->nMin, &sAcHistDayData.sTime, sizeof(STD_TIME)); 
                    }
                    else
                    {
                        RTCDateTimeRead(&sTmpTime);
                        if(GetDiffTimeSecs(&sTmpTime, pDjTime, &nSec) < 0)
                        {
                            break;
                        }
                        if(nSec < 0)
                        {
                            break;
                        }
                        
                        for(int ii = 0; ii<31; ii++)
                        {
                            memset(&sAcHistDayData, 0, sizeof(sAcHistDayData));
                            MemReadAcHistDataProtProc(F1, ii, &sAcHistDayData, sizeof(sAcHistDayData));
                            if( 0 == memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))
                            {
                                ii = 32;
                            }
                        }
                        //nDays = GetDaysFrom2KYear(&sTmpTime) - GetDaysFrom2KYear(pDjTime) + 1;
                        //MemReadAcHistDataProtProc(F1, nDays, &sAcHistDayData, sizeof(sAcHistDayData));
                        if( 0 == memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))
                        {
                            memcpy(&sAcEn, &sAcHistDayData.sAcEnergy, sizeof(sAcEn));
                            memcpy(&sAcXLEnergy, &sAcHistDayData.sAcDemand, sizeof(sAcXLEnergy));
                            memcpy(&pTimeVld->nMin, &sAcHistDayData.sTime, sizeof(STD_TIME)); 
                            nSJXHNum = sAcHistDayData.sPriority;
                        }
                        else
                        {
                            //memcpy(&sAcEn, 0, 1);
                            //读取日冻结数据时标不对置一
                            nRDJTimeFlg = 1;
                        }
                    }
                } 
                break;
            case 0x50060000:
            
                break;

            case 0x30000000:
            case 0x30010000:
            case 0x30020000:
            case 0x30030000:
            case 0x30040000:
            case 0x30050000:
            case 0x30060000:
                {
                    RTCDateTimeRead(&sTmpTime);
                    if(GetDiffTimeSecs(&sTmpTime, pDjTime, &nSec) < 0)
                    {
                        break;
                    }
                    if(nSec < 0)
                    {
                        break;
                    }
                    if(0x30000700 == nMasterOAD)
                    {
                        nEveFn = F1;
                        nNumFn = F28; 
                    }
                    else if(0x30000800 == nMasterOAD)
                    {
                        nEveFn = F2;
                        nNumFn = F29;
                    }
                    else if(0x30000900 == nMasterOAD)
                    {
                        nEveFn = F3;
                        nNumFn = F30;
                    }
                    else if(0x30010700 == nMasterOAD)
                    {
                        nEveFn = F49;
                        nNumFn = F105; 
                    }
                    else if(0x30010800 == nMasterOAD)
                    {
                        nEveFn = F50;
                        nNumFn = F106;
                    }
                    else if(0x30010900 == nMasterOAD)
                    {
                        nEveFn = F51;
                        nNumFn = F107;
                    }
                    else if(0x30020700 == nMasterOAD)
                    {
                        nEveFn = F18;
                        nNumFn = F52;
                    }
                    else if(0x30020800 == nMasterOAD)
                    {
                        nEveFn = F19;
                        nNumFn = F53;
                    }
                    else if(0x30020900 == nMasterOAD)
                    {
                        nEveFn = F20;
                        nNumFn = F54;
                    }
                    else if(0x30030700 == nMasterOAD)
                    {
                        nEveFn = F4;
                        nNumFn = F28;   
                    }
                    else if(0x30030800 == nMasterOAD)
                    {
                        nEveFn = F5;
                        nNumFn = F29;
                    }
                    else if(0x30030900 == nMasterOAD)
                    {
                        nEveFn = F6;
                        nNumFn = F30;
                    }
                    else if(0x30040700 == nMasterOAD)
                    {
                        nEveFn = F9;
                        nNumFn = F38;   
                    }
                    else if(0x30040800 == nMasterOAD)
                    {
                        nEveFn = F10;
                        nNumFn = F39;
                    }
                    else if(0x30040900 == nMasterOAD)
                    {
                        nEveFn = F11;
                        nNumFn = F40;
                    }
                    else if(0x30050700 == nMasterOAD)
                    {
                        nEveFn = F21;
                        nNumFn = F61;   
                    }
                    else if(0x30050800 == nMasterOAD)
                    {
                        nEveFn = F22;
                        nNumFn = F62;
                    }
                    else if(0x30050900 == nMasterOAD)
                    {
                        nEveFn = F23;
                        nNumFn = F63;
                    }
                    else if(0x30060700 == nMasterOAD)
                    {
                        nEveFn = F52;
                        nNumFn = F109;   
                    }
                    else if(0x30060800 == nMasterOAD)
                    {
                        nEveFn = F53;
                        nNumFn = F110;
                    }
                    else if(0x30060900 == nMasterOAD)
                    {
                        nEveFn = F54;
                        nNumFn = F111;
                    }
                    
                    if(n_RSDSelector9flg != 0)
                    {
                        memset(&nData, 0, LEN_AC_EVENT);
                        MemReadAcEvtProc(nEveFn,ii, &nData,LEN_AC_EVENT); //用这个g_nAcEventBuf比nData好？
                        memcpy(&sAcEn, &sAcHistDayData.sAcEnergy, sizeof(sAcEn));
                        memcpy(&sAcXLEnergy, &sAcHistDayData.sAcDemand, sizeof(sAcXLEnergy));
                        memcpy(&pTimeVld->nMin, &sAcHistDayData.sTime, sizeof(STD_TIME));
                        memcpy(&nSJXHNum, &nData[252], 4); //读取次数
                    }
                    else
                    {
                        for(int ii = 0; ii<10; ii++)
                        {
                            memset(&nData, 0, LEN_AC_EVENT);
                            MemReadAcEvtProc(nEveFn,ii, &nData,LEN_AC_EVENT); //用这个g_nAcEventBuf比nData好？
                            sEveTime.nYear = nData[5];
                            sEveTime.nMon = nData[4];
                            sEveTime.nDay = nData[3];
                            sEveTime.nHour = nData[2];
                            sEveTime.nMin = nData[1];
                            sEveTime.nSec = nData[0];
                            if( 0 == memcmp(&pDjTime->nDay,&sEveTime.nDay,3))
                            {
                                ii = 12;
                            }
                        }  
                        
                        if( 0 == memcmp(&pDjTime->nDay,&sEveTime.nDay,3))
                        {
                            memcpy(&nSJXHNum, &nData[252], 4); //读取次数
                            //MemReadAcXFlhBackData(nNumFn , &nSJXHNum, sizeof(nSJXHNum));
                        }
                        else
                        {
                            nRDJTimeFlg = 1;
                        }
                    }
                }
                break;
            default: break;
        }
    }

    switch (nOI)
    {
 
        case 0x2022:
            if((0 != memcmp(&pDjTime->nDay,&sEveTime.nDay,3))&&(nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
            {  
                nRDJTimeFlg = 0;
            }
            else
            {
                pData[nDataLen++] = e_double_long_unsigned;
                pData[nDataLen++] = 0x00;
                pData[nDataLen++] = 0x00;
                pData[nDataLen++] = 0x00;
                pData[nDataLen++] = nSJXHNum;
            }
            break;
        case 0x2023:
            if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
            {  
                nRDJTimeFlg = 0;
            }
            else
            {
                pData[nDataLen++] = e_double_long_unsigned;
                pData[nDataLen++] = ((nSJXHNum>>24)&0xFF);
                pData[nDataLen++] = ((nSJXHNum>>16)&0xFF);
                pData[nDataLen++] = ((nSJXHNum>>8)&0xFF);
                pData[nDataLen++] = (nSJXHNum&0xFF);
            }
            break;
        case 0x201E:
              if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
              {  
                  nRDJTimeFlg = 0;
              }
              else
              {
                  //sAcEveDayData.sEveStartClk
                  pData[nDataLen++] = e_date_time_s;
                  nTempBuf[0] = pTimeVld->nYear;
                  nTempBuf[1] = 0x20;
                  nTemp = BCD2INT32U(&nTempBuf[0], 4);
                  INT16U2Buf(&nTempBuf[0], nTemp);
                  
                  pData[nDataLen++] = nTempBuf[1];
                  pData[nDataLen++] = nTempBuf[0];
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMon);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nDay);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nHour);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMin);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nSec);
              }
             break;
         case 0x2020:
             if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
              {  
                  nRDJTimeFlg = 0;
              }
              else
              {
                  pData[nDataLen++] = e_date_time_s;
                  nTempBuf[0] = pTimeVld->nYear;
                  nTempBuf[1] = 0x20;
                  nTemp = BCD2INT32U(&nTempBuf[0], 4);
                  INT16U2Buf(&nTempBuf[0], nTemp);
                  
                  pData[nDataLen++] = nTempBuf[1];
                  pData[nDataLen++] = nTempBuf[0];
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMon);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nDay);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nHour);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMin);
                  pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nSec);
              }
             break;

        case 0x4000:
            if (RET_ERR_OK != GetSysClk(pTimeVld))
            {
                pData[nDataLen++] = e_HardFault;
                return -1;
            }
            pData[nDataLen++] = e_date_time_s;

            nTempBuf[0] = pTimeVld->nYear;
            nTempBuf[1] = 0x20;
            nTemp = BCD2INT32U(&nTempBuf[0], 4);
            INT16U2Buf(&nTempBuf[0], nTemp);
            
            pData[nDataLen++] = nTempBuf[1];
            pData[nDataLen++] = nTempBuf[0];
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMon);
            
            if (0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障
            {
                if(pTimeVld->nDay < 0x27)
                 {
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nDay+2);
                 }
                else
                {
                    pData[nDataLen++] = 3;
                }
            }
            else
            {
              pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nDay);
            }
            
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nHour);
            if (0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障
            {
                if (pTimeVld->nMin < 0x48)
                 {
                    pData[nDataLen++] =BCD_TO_INT8U(pTimeVld->nMin+0x10);
                 }
                else
                {
                    pData[nDataLen++] = 5;
                }
            }
            else
            {
              pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMin);
            }
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nSec);
    
            break;
        case 0x4001:
        {
            memset(nTempBuf, 0, sizeof(nTempBuf));
            MemReadAcPrm(F19, &nTempBuf[0], 6);
            
            pData[nDataLen++] = e_octet_string;
            pData[nDataLen++] = 6;           
            ReverseOutput(&nTempBuf[0], pData + nDataLen, 6); 
            nDataLen += 6;          
            break;
        }
        case 0x4002:
        {
            memset(nTempBuf, 0, sizeof(nTempBuf));
            MemReadAcPrm(F20, &nTempBuf[0], 6);//F21->F20
            
            pData[nDataLen++] = e_octet_string;
            pData[nDataLen++] = 6;           
            ReverseOutput(&nTempBuf[0], pData + nDataLen, 6); 
            nDataLen += 6;          
            break;
        }
        case 0x2021:
        { 
            if(nRDJTimeFlg == 1)
            {
                break;
            }
            pData[nDataLen++] = e_date_time_s;
            nTempBuf[0] = pTimeVld->nYear;
            nTempBuf[1] = 0x20;
            nTemp = BCD2INT32U(&nTempBuf[0], 4);
            INT16U2Buf(&nTempBuf[0], nTemp);
            
            pData[nDataLen++] = nTempBuf[1];
            pData[nDataLen++] = nTempBuf[0];
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMon);
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nDay);
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nHour);
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nMin);
            pData[nDataLen++] = BCD_TO_INT8U(pTimeVld->nSec);
            break;
        }
        
        case 0x0000:
        {   
			MemReadAcRealData(F5, &sAcCombEnergy.sPPEnergy[0], sizeof(sAcCombEnergy)); 
			if( OAD[3]==0 )
			{
				if (!IsAllBcd(&sAcCombEnergy.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
				{
					memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
				}
                if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                }
                else
                {
                    pData[nDataLen++]  = e_array;
                    pData[nDataLen++];
                    for(i = 0; i <= MAX_FEE_NUM; i ++)
                    {
                        pData[nDataLen++] = e_double_long;
                        if (g_MeterAcErrFlg[0])
                        {
                            memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
                            if (i < 2)
                            {
                                nTempBuf[0] = 0x66;
                                nTempBuf[3] = 0x78;
                            }
                            nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                        }
                        else if (g_MeterAcErrFlg[3])
                        {
                            memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
                            if (0 == i)
                            {
                                nTempBuf[0] = 0x66;
                                nTempBuf[1] = 0x34;
                                nTempBuf[2] = 0x05;
                            }
                            else if(1 == i)
                            {
                                nTempBuf[0] = 0x33;
                                nTempBuf[1] = 0x67;
                                nTempBuf[2] = 0x04;
                            }
                            nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                        }
                        else
                        {
                            nConvert = Bcdbuf2Long(&sAcCombEnergy.sPPEnergy[i].nD1, sizeof(FORMAT11));
                        }
                        
                        Long2Buf(&sAcCombEnergy.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                        ReverseOutput(&sAcCombEnergy.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                        nDataLen += sizeof(FORMAT11);             
                        nArrayNum ++;
                    }
                }
				pData[1] = nArrayNum;
				break;
			}
			else
			{
                if( OAD[3]>5 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
				if (!IsAllBcd(&sAcCombEnergy.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
				{
					memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
				}        
				i = OAD[3]-1;
				pData[nDataLen++] = e_double_long;
                if (g_MeterAcErrFlg[0])
                {
                    memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
                    if (i < 2)
                    {
                        nTempBuf[0] = 0x66;
                        nTempBuf[3] = 0x78;
                    }
                    nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                }
                else if (g_MeterAcErrFlg[3])
                {
                    memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
                    if (0 == i)
                    {
                        nTempBuf[0] = 0x66;
                        nTempBuf[1] = 0x34;
                        nTempBuf[2] = 0x05;
                    }
                    else if(1 == i)
                    {
                        nTempBuf[0] = 0x33;
                        nTempBuf[1] = 0x67;
                        nTempBuf[2] = 0x04;
                    }
                    nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                }
                else
                {
                    nConvert = Bcdbuf2Long(&sAcCombEnergy.sPPEnergy[i].nD1, sizeof(FORMAT11));
                }
				Long2Buf(&sAcCombEnergy.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
				ReverseOutput(&sAcCombEnergy.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
				nDataLen += sizeof(FORMAT11);             
				nArrayNum ++;
				break;
			}		  
            
        }
        case 0x0010:
        {    
          
          if( OAD[3]==0 )
          {
              if (!IsAllBcd(&sAcEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
              {
                  memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
              }        
              if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
              {  
                  //for(i = 0; i <= MAX_FEE_NUM; i ++)  //22.8.28注，考虑返回报文时标不一致时，直接返回长度0，不在返回空，已在前面内容修改
                  {
                      //pData[nDataLen++] = e_null;           
                      //nArrayNum ++;
                  }
                  nRDJTimeFlg = 0;
              }
              else
              {
                  pData[nDataLen++]  = e_array;
                  pData[nDataLen++];
                  for(i = 0; i <= MAX_FEE_NUM; i ++)
                  {
                      pData[nDataLen++] = e_double_long_unsigned;
                        if (g_MeterAcErrFlg[0])
                        {
                            memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[i], sizeof(FORMAT11));
                            if (i < 2)
                            {
                                nTempBuf[0] = 0x66;
                                nTempBuf[3] = 0x78;
                            }
                            nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                        }
                        else if (g_MeterAcErrFlg[3])
                        {
                            memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[i], sizeof(FORMAT11));
                            if (0 == i)
                            {
                                nTempBuf[0] = 0x66;
                                nTempBuf[1] = 0x34;
                                nTempBuf[2] = 0x05;
                            }
                            else if(1 == i)
                            {
                                nTempBuf[0] = 0x33;
                                nTempBuf[1] = 0x67;
                                nTempBuf[2] = 0x04;
                            }
                            nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                        }
                        else
                        {
                            nConvert = Bcdbuf2Long(&sAcEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                        }
                      Long2Buf(&sAcEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                      ReverseOutput(&sAcEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                      nDataLen += sizeof(FORMAT11);             
                      nArrayNum ++;
                      
                  }
              }
              pData[1] = nArrayNum;
              break;
          }
          else
          {
              if( OAD[3]>5 )
              {
                  nDataLen = 0;
                  pData[nDataLen++] = e_RefuseRW;
                  break;
              }
              if (!IsAllBcd(&sAcEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
              {
                  memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
              }        
             
              pData[nDataLen++] = e_double_long_unsigned;
              i=OAD[3]-1;
               if (g_MeterAcErrFlg[0])
                {
                    memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[i], sizeof(FORMAT11));
                    if (i < 2)
                    {
                        nTempBuf[0] = 0x66;
                        nTempBuf[3] = 0x78;
                    }
                    nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                }
                else if (g_MeterAcErrFlg[3])
                {
                    memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[i], sizeof(FORMAT11));
                    if (0 == i)
                    {
                        nTempBuf[0] = 0x66;
                        nTempBuf[1] = 0x34;
                        nTempBuf[2] = 0x05;
                    }
                    else if(1 == i)
                    {
                        nTempBuf[0] = 0x33;
                        nTempBuf[1] = 0x67;
                        nTempBuf[2] = 0x04;
                    }
                    nConvert = Bcdbuf2Long(&nTempBuf[0], sizeof(FORMAT11));
                }
                else
                {
                    nConvert = Bcdbuf2Long(&sAcEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                }
              Long2Buf(&sAcEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
              ReverseOutput(&sAcEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
              nDataLen += sizeof(FORMAT11);             
              break;
          }    
        }
       
        case 0x0020:
        {   
            if( OAD[3]==0 )
            {
                if (!IsAllBcd(&sAcEn.sNPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }   
                
                if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    //for(i = 0; i <= MAX_FEE_NUM; i ++)
                    {
                     //   pData[nDataLen ++] = e_null;          
                    //    nArrayNum ++;
                    }
                    nRDJTimeFlg = 0;
                }
                else
                {
                    pData[nDataLen++]  = e_array;
                    pData[nDataLen++];
                    for(i = 0; i <= MAX_FEE_NUM; i ++)
                    {
                        pData[nDataLen ++] = e_double_long_unsigned;
                        nConvert = Bcdbuf2Long(&sAcEn.sNPEnergy[i].nD1, sizeof(FORMAT11));
                        Long2Buf(&sAcEn.sNPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                        ReverseOutput(&sAcEn.sNPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                        nDataLen += sizeof(FORMAT11);
                        nArrayNum ++;
                        
                    }
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {
                if( OAD[3]>5 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcEn.sNPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }   
                i=OAD[3]-1;
                pData[nDataLen ++] = e_double_long_unsigned;
                nConvert = Bcdbuf2Long(&sAcEn.sNPEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcEn.sNPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcEn.sNPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);
                break;
            }
            
        }
        case 0x2000:
        {
            if( OAD[3]==0 )
            {
                pData[nDataLen++] = e_array;
                pData[nDataLen++];
                for(i = 0; i < MAX_PHASE_NUM; i ++)
                {
                    pData[nDataLen++] = e_long_unsigned;
                    if (1 == g_MeterEventWrong[i]&0x01) //|| ((0x20+(1<<i)) == (g_MeterVoltPlusFlg&(0x20+(1<<i))))||(g_MeterVoltPlusFlg == 0x01))
                    { 
                        memcpy(nTempBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                        DivideTwo(nTempBuf);
                        nConvert = Bcdbuf2Long(&nTempBuf[0], 2);
                    }
                    else if (((0x20+(1<<i)) == (g_MeterVoltPlusFlg&(0x20+(1<<i))))||(g_MeterVoltPlusFlg == 0x01))
                    { 
                        memcpy(nTempBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                        DivideTwo2(nTempBuf);
                        nConvert = Bcdbuf2Long(&nTempBuf[0], 2);
                    }
                    else if ((0x10+(1<<i)) == (g_MeterVoltPlusFlg&(0x10+(1<<i))))//电压越上限
                    {
                        memcpy(nTempBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                        if (0 != nTempBuf[1])
                        {
                           nTempBuf[1] += 0x05;
                        }
                        nConvert = Bcdbuf2Long(&nTempBuf[0], 2);
                    }
                    else if (0x02 ==(g_MeterEventWrong[i]&0x02))
                    {
                        nConvert = 0;
                    }
                    else
                    {
                       nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealVolt[i].nFrac, 2);
                    }

                    pData[nDataLen++] = (nConvert & 0xFF00) >> 8;
                    pData[nDataLen++] = nConvert & 0xFF;
                }
                nArrayNum = 3;
                pData[1] = nArrayNum;
                break;
            }
            else
            {
                if( OAD[3]>3 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                i=OAD[3]-1;
                pData[nDataLen++] = e_long_unsigned;
                if (1 == g_MeterEventWrong[i]&0x01) //|| ((0x20+(1<<i)) == (g_MeterVoltPlusFlg&(0x20+(1<<i))))||(g_MeterVoltPlusFlg == 0x01))
                { 
                    memcpy(nTempBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                    DivideTwo(nTempBuf);
                    nConvert = Bcdbuf2Long(&nTempBuf[0], 2);
                }
                else if (((0x20+(1<<i)) == (g_MeterVoltPlusFlg&(0x20+(1<<i))))||(g_MeterVoltPlusFlg == 0x01))
                { 
                    memcpy(nTempBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                    DivideTwo2(nTempBuf);
                    nConvert = Bcdbuf2Long(&nTempBuf[0], 2);
                }
                else if ((0x10+(1<<i)) == (g_MeterVoltPlusFlg&(0x10+(1<<i))))//电压越上限
                {
                    memcpy(nTempBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                    if (0 != nTempBuf[1])
                    {
                       nTempBuf[1] += 0x05;
                    }
                    nConvert = Bcdbuf2Long(&nTempBuf[0], 2);
                }
                else if (0x02==(g_MeterEventWrong[i]&0x02))
                {
                    nConvert = 0;
                }
                else
                {
                   nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealVolt[i].nFrac, 2);
                }
                
                pData[nDataLen++] = (nConvert & 0xFF00) >> 8;
                pData[nDataLen++] = nConvert & 0xFF;
                break;
            }    
            
        }
        case 0x2001:
        {
            if(OAD[2]==2)
            {
                if( OAD[3]==0 )
                {
                    pData[nDataLen++] = e_array;
                    pData[nDataLen++];
                    for(i = 0; i < MAX_PHASE_NUM; i ++)
                    {
                        pData[nDataLen++] = e_double_long;
                        if ((0x04==(g_MeterEventWrong[i]&0x04)) || (0x02==(g_MeterEventWrong[i]&0x02)))
                        {
                            nConvert = 0;
                        }  
                        else
                        {
                           nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealCurr[i].nD1, 3); 
                        }
                        memset(nTempBuf, 0, sizeof(nTempBuf));
                        Long2Buf(nTempBuf, nConvert, 4);
                        ReverseOutput(nTempBuf, pData + nDataLen, 4);
                        nDataLen += 4;
                        nArrayNum ++;
                    }
                    pData[1] = nArrayNum;
                    break;
                }
                else
                {
                    if( OAD[3]>3 )
                    {
                        nDataLen = 0;
                        pData[nDataLen++] = e_RefuseRW;
                        break;
                    }
                    i=OAD[3]-1;
                    pData[nDataLen++] = e_double_long;
                    if ((0x04==(g_MeterEventWrong[i]&0x04)) || (0x02==(g_MeterEventWrong[i]&0x02)))
                    {
                        nConvert = 0;
                    }  
                    else
                    {
                       nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealCurr[i].nD1, 3); 
                    }
                    memset(nTempBuf, 0, sizeof(nTempBuf));
                    Long2Buf(nTempBuf, nConvert, 4);
                    ReverseOutput(nTempBuf, pData + nDataLen, 4);
                    nDataLen += 4;
                    break;
                } 
            }
            else if(OAD[2]== 0x04)
            {
                if(g_nSetShortcurFlg == 1)
                {
                    INT8U nRealCurr[3] = {0x96,0x29,0x00};
                    pData[nDataLen++] = e_double_long;
                    nConvert = Bcdbuf2Long(&nRealCurr[0], 3); 
                    memset(nTempBuf, 0, sizeof(nTempBuf));
                    Long2Buf(nTempBuf, nConvert, 4);
                    ReverseOutput(nTempBuf, pData + nDataLen, 4);
                    nDataLen += 4;
                }
                else
                {
                    pData[nDataLen++] = e_double_long;
                    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealCurr[3].nD1, 3); 
                    memset(nTempBuf, 0, sizeof(nTempBuf));
                    Long2Buf(nTempBuf, nConvert, 4);
                    ReverseOutput(nTempBuf, pData + nDataLen, 4);
                    nDataLen += 4;
                }
                break;
            }
        }	

        case 0x2004:
        {
            if( OAD[3]==0 )
            {//索引是0 要求总
                pData[nDataLen++] = e_array;
                pData[nDataLen++];
                for(i = 0; i < MAX_PHASE_NUM + 1; i ++)
                {
                    pData[nDataLen++] = e_double_long;
                    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealP[i].nFrac0, 3);
                    memset(nTempBuf, 0, sizeof(nTempBuf));
                    Long2Buf(nTempBuf, nConvert, 4);
                    ReverseOutput(nTempBuf, pData + nDataLen, 4);
                    nDataLen += 4;
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>4 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                i=OAD[3]-1;
                pData[nDataLen++] = e_double_long;
                nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealP[i].nFrac0, 3);
                memset(nTempBuf, 0, sizeof(nTempBuf));
                Long2Buf(nTempBuf, nConvert, 4);
                ReverseOutput(nTempBuf, pData + nDataLen, 4);
                nDataLen += 4;
                break;
            }    
        }
        case 0x2005:
        {
            if( OAD[3]==0 )
            {//索引是0 要求总
                pData[nDataLen++] = e_array;
                pData[nDataLen++];
                for(i = 0; i < MAX_PHASE_NUM + 1; i ++)
                {
                    pData[nDataLen++] = e_double_long;
                    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealQ[i].nFrac0, 3);
                    memset(nTempBuf, 0, sizeof(nTempBuf));
                    Long2Buf(nTempBuf, nConvert, 4);
                    ReverseOutput(nTempBuf, pData + nDataLen, 4);
                    nDataLen += 4;
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>4 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                i=OAD[3]-1;
                pData[nDataLen++] = e_double_long;
                nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealQ[i].nFrac0, 3);
                memset(nTempBuf, 0, sizeof(nTempBuf));
                Long2Buf(nTempBuf, nConvert, 4);
                ReverseOutput(nTempBuf, pData + nDataLen, 4);
                nDataLen += 4;
                nArrayNum ++;
                break;
            }    
        }
        case 0x2006:
        {
            if( OAD[3]==0 )
            {//索引是0 要求总
                pData[nDataLen++] = e_array;
                pData[nDataLen++];
                for(i = 0; i < MAX_PHASE_NUM + 1; i ++)
                {
                    pData[nDataLen++] = e_double_long;
                    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealS[i].nFrac0, 3);
                    memset(nTempBuf, 0, sizeof(nTempBuf));
                    Long2Buf(nTempBuf, nConvert, 4);
                    ReverseOutput(nTempBuf, pData + nDataLen, 4);
                    nDataLen += 4;
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>4 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                i=OAD[3]-1;
                pData[nDataLen++] = e_double_long;
                nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealS[i].nFrac0, 3);
                memset(nTempBuf, 0, sizeof(nTempBuf));
                Long2Buf(nTempBuf, nConvert, 4);
                ReverseOutput(nTempBuf, pData + nDataLen, 4);
                nDataLen += 4;
                nArrayNum ++;
                break;
            }    

        }
        case 0x200A:
        {
            if( OAD[3]==0 )
            {//索引是0 要求总
                pData[nDataLen++] = e_array;
                pData[nDataLen++];
                for(i = 0; i < MAX_PHASE_NUM + 1; i ++)
                {
                    pData[nDataLen++] = e_long;
                    nConvert = BCD_TO_INT8U(pDataTable->sRealInstant.sRealPf[i].nFrac) + ((INT16U)(BCD_TO_INT8U(pDataTable->sRealInstant.sRealPf[i].nSint & 0x7F)) * 100);
                    memset(nTempBuf, 0, sizeof(nTempBuf));
                    Long2Buf(nTempBuf, nConvert, 2);
                    ReverseOutput(nTempBuf, pData + nDataLen,2);
                    nDataLen += 2;
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>4 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                //删除如下2行
                //pData[nDataLen++]  = e_array;
                //pData[nDataLen++];
                //在for循环前
                
                //删除如下2行
                //nArrayNum ++;
                //pData[1] = nArrayNum;
                i=OAD[3]-1;
                pData[nDataLen++] = e_long;
                nConvert = BCD_TO_INT8U(pDataTable->sRealInstant.sRealPf[i].nFrac) + ((INT16U)(BCD_TO_INT8U(pDataTable->sRealInstant.sRealPf[i].nSint & 0x7F)) * 100);
                memset(nTempBuf, 0, sizeof(nTempBuf));
                Long2Buf(nTempBuf, nConvert, 2);
                ReverseOutput(nTempBuf, pData + nDataLen,2);
                nDataLen += 2;
                break;
            }    
            
        }
        //以下是三相表特有的，单相表读了回复 6
        case 0x0030:
        {   
			if( g_nHardType==1 )
            {//单相表
                if((0 != memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                else if((0 == memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 0))
                {
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = 0x00;
                }
                else
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_NoObject;
                }
                break;
            }
			else
			{//
				if( OAD[3]==0 )
				{
					if (!IsAllBcd(&sAcEn.sPNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
                    if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                    {  
                        nRDJTimeFlg = 0;
                        break;
                    }
					pData[nDataLen++]  = e_array;
					pData[nDataLen++];
					for(i = 0; i <= MAX_FEE_NUM; i ++)
					{
						pData[nDataLen ++] = e_double_long;
						nConvert = Bcdbuf2Long(&sAcEn.sPNEnergy[i].nD1, sizeof(FORMAT11));
						Long2Buf(&sAcEn.sPNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
						ReverseOutput(&sAcEn.sPNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
						nDataLen += sizeof(FORMAT11);
						nArrayNum ++;
						
					}
					pData[1] = nArrayNum;
					break;
				}
				else
				{
					if( OAD[3]>5 )
						break;
					if (!IsAllBcd(&sAcEn.sPNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
					i=OAD[3]-1;
					pData[nDataLen ++] = e_double_long;
					nConvert = Bcdbuf2Long(&sAcEn.sPNEnergy[i].nD1, sizeof(FORMAT11));
					Long2Buf(&sAcEn.sPNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
					ReverseOutput(&sAcEn.sPNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
					nDataLen += sizeof(FORMAT11);			
					break;
				}
			}
        }
        case 0x0040:
        {   

            if (!IsAllBcd(&sAcEn.sNNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
            {
                memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
            } 
            if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
            {  
                nRDJTimeFlg = 0;
                break;
            }
            pData[nDataLen++]  = e_array;
            pData[nDataLen++];
            for(i = 0; i <= MAX_FEE_NUM; i ++)
            {
                pData[nDataLen ++] = e_double_long;
                nConvert = Bcdbuf2Long(&sAcEn.sNNEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcEn.sNNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcEn.sNNEnergy[0].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);
                nArrayNum ++;
                
            }
            pData[1] = nArrayNum;
            break;
        }
        case 0x0050:
        {   
		    if( g_nHardType==1 )
            {//单相表
                if((0 != memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                else if((0 == memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 0))
                {
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = 0x00;
                }
                else
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_NoObject;
                }
                break;
            }
            else
            {
                if( OAD[3]==0 )
				{//索引是0 要求总
					if (!IsAllBcd(&sAcQEn.sQ1Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
                    if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                    {  
                        nRDJTimeFlg = 0;
                        break;
                    }
					pData[nDataLen++]  = e_array;
					pData[nDataLen++];
					for(i = 0; i <= MAX_FEE_NUM; i ++)
					{
						pData[nDataLen ++] = e_double_long_unsigned;
						nConvert = Bcdbuf2Long(&sAcQEn.sQ1Energy[i].nD1, sizeof(FORMAT11));
						Long2Buf(&sAcQEn.sQ1Energy[i].nD1, nConvert, sizeof(FORMAT11));
						ReverseOutput(&sAcQEn.sQ1Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
						nDataLen += sizeof(FORMAT11);
						nArrayNum ++;
						
					}
					pData[1] = nArrayNum;
					break;
				}
				else
				{//OAD[3] != 0
					if( OAD[3]>5 )
						break;
					if (!IsAllBcd(&sAcQEn.sQ1Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
					i=OAD[3]-1;
					pData[nDataLen ++] = e_double_long_unsigned;
					nConvert = Bcdbuf2Long(&sAcQEn.sQ1Energy[i].nD1, sizeof(FORMAT11));
					Long2Buf(&sAcQEn.sQ1Energy[i].nD1, nConvert, sizeof(FORMAT11));
					ReverseOutput(&sAcQEn.sQ1Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
					nDataLen += sizeof(FORMAT11);
					break;
				} 
                
            }
            break;
        }

        case 0x0060:
        {   
			if( g_nHardType==1 )
			{//单相表
                if((0 != memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                else if((0 == memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 0))
                {
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = 0x00;
                }
                else
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_NoObject;
                }
				break;
			}
			else
			{
                if( OAD[3]==0 )
				{//索引是0 要求总
					if (!IsAllBcd(&sAcQEn.sQ2Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
                    if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                    {  
                        nRDJTimeFlg = 0;
                        break;
                    }
					pData[nDataLen++]  = e_array;
					pData[nDataLen++];
					for(i = 0; i <= MAX_FEE_NUM; i ++)
					{
						pData[nDataLen ++] = e_double_long_unsigned;
						nConvert = Bcdbuf2Long(&sAcQEn.sQ2Energy[i].nD1, sizeof(FORMAT11));
						Long2Buf(&sAcQEn.sQ2Energy[i].nD1, nConvert, sizeof(FORMAT11));
						ReverseOutput(&sAcQEn.sQ2Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
						nDataLen += sizeof(FORMAT11);
						nArrayNum ++;
						
					}
					pData[1] = nArrayNum;
					break;
				}
				else
				{//OAD[3] != 0
					if( OAD[3]>5 )
						break;
					if (!IsAllBcd(&sAcQEn.sQ2Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
					i=OAD[3]-1;
					pData[nDataLen ++] = e_double_long_unsigned;
					nConvert = Bcdbuf2Long(&sAcQEn.sQ2Energy[i].nD1, sizeof(FORMAT11));
					Long2Buf(&sAcQEn.sQ2Energy[i].nD1, nConvert, sizeof(FORMAT11));
					ReverseOutput(&sAcQEn.sQ2Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
					nDataLen += sizeof(FORMAT11);
					break;
				} 
				
			}
            break;
        }
        case 0x0070:
        {   
			if( g_nHardType==1 )
			{//单相表
                if((0 != memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                else if((0 == memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 0))
                {
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = 0x00;
                }
                else
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_NoObject;
                }
				break;
			}
			else
			{
                 if( OAD[3]==0 )
				{//索引是0 要求总
					if (!IsAllBcd(&sAcQEn.sQ3Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
                    if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                    {  
                        nRDJTimeFlg = 0;
                        break;
                    }
					pData[nDataLen++]  = e_array;
					pData[nDataLen++];
					for(i = 0; i <= MAX_FEE_NUM; i ++)
					{
						pData[nDataLen ++] = e_double_long_unsigned;
						nConvert = Bcdbuf2Long(&sAcQEn.sQ3Energy[i].nD1, sizeof(FORMAT11));
						Long2Buf(&sAcQEn.sQ3Energy[i].nD1, nConvert, sizeof(FORMAT11));
						ReverseOutput(&sAcQEn.sQ3Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
						nDataLen += sizeof(FORMAT11);
						nArrayNum ++;
						
					}
					pData[1] = nArrayNum;
					break;
				}
				else
				{//OAD[3] != 0
					if( OAD[3]>5 )
						break;
					if (!IsAllBcd(&sAcQEn.sQ3Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
					i=OAD[3]-1;
					pData[nDataLen ++] = e_double_long_unsigned;
					nConvert = Bcdbuf2Long(&sAcQEn.sQ3Energy[i].nD1, sizeof(FORMAT11));
					Long2Buf(&sAcQEn.sQ3Energy[i].nD1, nConvert, sizeof(FORMAT11));
					ReverseOutput(&sAcQEn.sQ3Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
					nDataLen += sizeof(FORMAT11);
					break;
				} 
				
			}
            
            break;
        }
        case 0x0080:
        {   
			if( g_nHardType==1 )
            {//单相表
                if((0 != memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                else if((0 == memcmp(&pDjTime->nDay,&sAcHistDayData.sTime.nDay,3))&&(nRDJTimeFlg == 0))
                {
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = 0x00;
                }
                else
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_NoObject;
                }
				break;
			}
			else
			{
                if( OAD[3]==0 )
				{//索引是0 要求总
					if (!IsAllBcd(&sAcQEn.sQ4Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
                    if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                    {  
                        nRDJTimeFlg = 0;
                        break;
                    }
					pData[nDataLen++]  = e_array;
					pData[nDataLen++];
					for(i = 0; i <= MAX_FEE_NUM; i ++)
					{
						pData[nDataLen ++] = e_double_long_unsigned;
						nConvert = Bcdbuf2Long(&sAcQEn.sQ4Energy[i].nD1, sizeof(FORMAT11));
						Long2Buf(&sAcQEn.sQ4Energy[i].nD1, nConvert, sizeof(FORMAT11));
						ReverseOutput(&sAcQEn.sQ4Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
						nDataLen += sizeof(FORMAT11);
						nArrayNum ++;
						
					}
					pData[1] = nArrayNum;
					break;
				}
				else
				{//OAD[3] != 0
					if( OAD[3]>5 )
						break;
					if (!IsAllBcd(&sAcQEn.sQ4Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
					{
						memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
					} 
					i=OAD[3]-1;
					pData[nDataLen ++] = e_double_long_unsigned;
					nConvert = Bcdbuf2Long(&sAcQEn.sQ4Energy[i].nD1, sizeof(FORMAT11));
					Long2Buf(&sAcQEn.sQ4Energy[i].nD1, nConvert, sizeof(FORMAT11));
					ReverseOutput(&sAcQEn.sQ4Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
					nDataLen += sizeof(FORMAT11);
					break;
				}
		
			}
            break;
        } 
        case 0x1010:   //正向有功最大需量
        {
            if ( OAD[3]==0 )
            {
                if (!IsAllBcd(&sAcXLEnergy.sPPDemand[0].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                } 
                if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]   ;
                
                for(i=0; i<(MAX_FEE_NUM+1); i++)
                {
                    pData[nDataLen++]  = e_structure;
                    pData[nDataLen++]  = 0x02;
                    pData[nDataLen ++] = e_double_long_unsigned;
                    nConvert = Bcdbuf2Long(&sAcXLEnergy.sPPDemand[i].nFrac0, sizeof(FORMAT23));
                    Long2Buf(&sAcXLEnergy.sPPDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                    ReverseOutput(&sAcXLEnergy.sPPDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                    nDataLen += sizeof(FORMAT11);
                    
                    memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sPPDemandTime[i].nMin, sizeof(FORMAT15));
                    pData[nDataLen++] = e_date_time_s;
                    nTempBuf[0] = pTimeVld2->nYear;
                    nTempBuf[1] = 0x20;
                    nTemp = BCD2INT32U(&nTempBuf[0], 4);
                    INT16U2Buf(&nTempBuf[0], nTemp);
                    
                    pData[nDataLen++] = nTempBuf[1];
                    pData[nDataLen++] = nTempBuf[0];
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                    
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>5 )
                    break;
                if (!IsAllBcd(&sAcXLEnergy.sPPDemand[i].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                } 
                i=OAD[3]-1;
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]  = 0x01 ;
                pData[nDataLen++]  = e_structure;
                pData[nDataLen++]  = 0x02;
                pData[nDataLen ++] = e_double_long_unsigned;
                nConvert = Bcdbuf2Long(&sAcXLEnergy.sPPDemand[i].nFrac0, sizeof(FORMAT23));
                Long2Buf(&sAcXLEnergy.sPPDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcXLEnergy.sPPDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);
                
                memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sPPDemandTime[i].nMin, sizeof(FORMAT15));
                pData[nDataLen++] = e_date_time_s;
                nTempBuf[0] = pTimeVld2->nYear;
                nTempBuf[1] = 0x20;
                nTemp = BCD2INT32U(&nTempBuf[0], 4);
                INT16U2Buf(&nTempBuf[0], nTemp);
                
                pData[nDataLen++] = nTempBuf[1];
                pData[nDataLen++] = nTempBuf[0];
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                break;
            }
            break;
        }
        case 0x1020:
        {
            if ( OAD[3]==0 )
            {
                if (!IsAllBcd(&sAcXLEnergy.sNPDemand[0].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                } 
                if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]   ;
                
                for(i=0; i<(MAX_FEE_NUM+1); i++)
                {
                    pData[nDataLen++]  = e_structure;
                    pData[nDataLen++]  = 0x02;
                    pData[nDataLen ++] = e_double_long_unsigned;
                    nConvert = Bcdbuf2Long(&sAcXLEnergy.sNPDemand[i].nFrac0, sizeof(FORMAT23));
                    Long2Buf(&sAcXLEnergy.sNPDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                    ReverseOutput(&sAcXLEnergy.sNPDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                    nDataLen += sizeof(FORMAT11);
                    
                    memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sNPDemandTime[i].nMin, sizeof(FORMAT15));
                    pData[nDataLen++] = e_date_time_s;
                    nTempBuf[0] = pTimeVld2->nYear;
                    nTempBuf[1] = 0x20;
                    nTemp = BCD2INT32U(&nTempBuf[0], 4);
                    INT16U2Buf(&nTempBuf[0], nTemp);
                    
                    pData[nDataLen++] = nTempBuf[1];
                    pData[nDataLen++] = nTempBuf[0];
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                    
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>5 )
                    break;
                if (!IsAllBcd(&sAcXLEnergy.sNPDemand[i].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                } 
                i=OAD[3]-1;
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]  = 0x01 ;
                pData[nDataLen++]  = e_structure;
                pData[nDataLen++]  = 0x02;
                pData[nDataLen ++] = e_double_long_unsigned;
                nConvert = Bcdbuf2Long(&sAcXLEnergy.sNPDemand[i].nFrac0, sizeof(FORMAT23));
                Long2Buf(&sAcXLEnergy.sNPDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcXLEnergy.sNPDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);
                
                memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sNPDemandTime[i].nMin, sizeof(FORMAT15));
                pData[nDataLen++] = e_date_time_s;
                nTempBuf[0] = pTimeVld2->nYear;
                nTempBuf[1] = 0x20;
                nTemp = BCD2INT32U(&nTempBuf[0], 4);
                INT16U2Buf(&nTempBuf[0], nTemp);
                
                pData[nDataLen++] = nTempBuf[1];
                pData[nDataLen++] = nTempBuf[0];
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                break;
            }
            break;
        }
        case 0x1030:
        {
            if ( OAD[3]==0 )
            {
                if (!IsAllBcd(&sAcXLEnergy.sPNDemand[0].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                } 
                if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]   ;
                
                for(i=0; i<(MAX_FEE_NUM+1); i++)
                {
                    pData[nDataLen++]  = e_structure;
                    pData[nDataLen++]  = 0x02;
                    pData[nDataLen ++] = e_double_long;
                    nConvert = Bcdbuf2Long(&sAcXLEnergy.sPNDemand[i].nFrac0, sizeof(FORMAT23));
                    Long2Buf(&sAcXLEnergy.sPNDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                    ReverseOutput(&sAcXLEnergy.sPNDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                    nDataLen += sizeof(FORMAT11);
                    
                    memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sPNDemandTime[i].nMin, sizeof(FORMAT15));
                    pData[nDataLen++] = e_date_time_s;
                    nTempBuf[0] = pTimeVld2->nYear;
                    nTempBuf[1] = 0x20;
                    nTemp = BCD2INT32U(&nTempBuf[0], 4);
                    INT16U2Buf(&nTempBuf[0], nTemp);
                    
                    pData[nDataLen++] = nTempBuf[1];
                    pData[nDataLen++] = nTempBuf[0];
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                    
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>5 )
                    break;
                if (!IsAllBcd(&sAcXLEnergy.sPNDemand[i].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                } 
                i=OAD[3]-1;
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]   ;
                pData[nDataLen++]  = e_structure;
                pData[nDataLen++]  = 0x02;
                pData[nDataLen ++] = e_double_long;
                nConvert = Bcdbuf2Long(&sAcXLEnergy.sPNDemand[i].nFrac0, sizeof(FORMAT23));
                Long2Buf(&sAcXLEnergy.sPNDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcXLEnergy.sPNDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);
                
                memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sPNDemandTime[i].nMin, sizeof(FORMAT15));
                pData[nDataLen++] = e_date_time_s;
                nTempBuf[0] = pTimeVld2->nYear;
                nTempBuf[1] = 0x20;
                nTemp = BCD2INT32U(&nTempBuf[0], 4);
                INT16U2Buf(&nTempBuf[0], nTemp);
                
                pData[nDataLen++] = nTempBuf[1];
                pData[nDataLen++] = nTempBuf[0];
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                break;
            }
          
            break;
        }
        case 0x1040:
        {
            if ( OAD[3]==0 )
            {
                if (!IsAllBcd(&sAcXLEnergy.sNNDemand[0].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                }
                if((nRDJTimeFlg == 1))  //读取日冻结数据时，时标不对进入，返回NULL
                {  
                    nRDJTimeFlg = 0;
                    break;
                }
                
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]   ;
                
                for(i=0; i<(MAX_FEE_NUM+1); i++)
                {
                    pData[nDataLen++]  = e_structure;
                    pData[nDataLen++]  = 0x02;
                    pData[nDataLen ++] = e_double_long;
                    nConvert = Bcdbuf2Long(&sAcXLEnergy.sNNDemand[i].nFrac0, sizeof(FORMAT23));
                    Long2Buf(&sAcXLEnergy.sNNDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                    ReverseOutput(&sAcXLEnergy.sNNDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                    nDataLen += sizeof(FORMAT11);
                    
                    memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sNNDemandTime[i].nMin, sizeof(FORMAT15));
                    pData[nDataLen++] = e_date_time_s;
                    nTempBuf[0] = pTimeVld2->nYear;
                    nTempBuf[1] = 0x20;
                    nTemp = BCD2INT32U(&nTempBuf[0], 4);
                    INT16U2Buf(&nTempBuf[0], nTemp);
                    
                    pData[nDataLen++] = nTempBuf[1];
                    pData[nDataLen++] = nTempBuf[0];
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                    pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                    
                    nArrayNum ++;
                }
                pData[1] = nArrayNum;
                break;
            }
            else
            {//OAD[3] != 0
                if( OAD[3]>5 )
                    break;
                if (!IsAllBcd(&sAcXLEnergy.sNNDemand[i].nIntg, (MAX_FEE_NUM+1)*sizeof(FORMAT23)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT23));
                } 
                i=OAD[3]-1;
                pData[nDataLen++]  = e_array;
                pData[nDataLen++]   ;
                pData[nDataLen++]  = e_structure;
                pData[nDataLen++]  = 0x02;
                pData[nDataLen ++] = e_double_long;
                nConvert = Bcdbuf2Long(&sAcXLEnergy.sNNDemand[i].nFrac0, sizeof(FORMAT23));
                Long2Buf(&sAcXLEnergy.sNNDemand[i].nIntg, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcXLEnergy.sNNDemand[i].nIntg, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);
                
                memcpy(&pTimeVld2->nMin, &sAcXLEnergy.sNNDemandTime[i].nMin, sizeof(FORMAT15));
                pData[nDataLen++] = e_date_time_s;
                nTempBuf[0] = pTimeVld2->nYear;
                nTempBuf[1] = 0x20;
                nTemp = BCD2INT32U(&nTempBuf[0], 4);
                INT16U2Buf(&nTempBuf[0], nTemp);
                
                pData[nDataLen++] = nTempBuf[1];
                pData[nDataLen++] = nTempBuf[0];
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMon);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nDay);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nHour);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nMin);
                pData[nDataLen++] = BCD_TO_INT8U(pTimeVld2->nSec);
                break;
            }
            break;
        }
        case 0x0011:
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                
                if (!IsAllBcd(&sAcTriEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 0;//分A B C三相 0 1 2 
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0021://sPPEnergy -> sNPEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                MemReadAcRealData(F3, &sAcTriEn, sizeof(sAcTriEn));
                if (!IsAllBcd(&sAcTriEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 0;
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0031: //sPPEnergy ->sPNEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                MemReadAcRealData(F3, &sAcTriEn, sizeof(sAcTriEn));
                if (!IsAllBcd(&sAcTriEn.sPNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 0;
                pData[nDataLen++] = e_double_long; //值 5
                nConvert = Bcdbuf2Long(&sAcTriEn.sPNEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0041: //sPPEnergy ->sNNEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                MemReadAcRealData(F3, &sAcTriEn, sizeof(sAcTriEn));
                if (!IsAllBcd(&sAcTriEn.sNNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 0;
                pData[nDataLen++] = e_double_long; //值 5
                nConvert = Bcdbuf2Long(&sAcTriEn.sNNEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sNNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sNNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        
        case 0x0012:
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 1;
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0022://sPPEnergy -> sNPEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 1;//分A B C三相 0 1 2 
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0032: //sPPEnergy ->sPNEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sPNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 1;
                pData[nDataLen++] = e_double_long; //值 5
                nConvert = Bcdbuf2Long(&sAcTriEn.sPNEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0042: //sPPEnergy ->sNNEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sNNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 1;
                pData[nDataLen++] = e_double_long; //值 5
                nConvert = Bcdbuf2Long(&sAcTriEn.sNNEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sNNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sNNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0013:
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 2;
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0023://sPPEnergy -> sNPEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sPPEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 2;//分A B C三相 0 1 2 
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriEn.sPPEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPPEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPPEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0033: //sPPEnergy ->sPNEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sPNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 2;
                pData[nDataLen++] = e_double_long; //值 5
                nConvert = Bcdbuf2Long(&sAcTriEn.sPNEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sPNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sPNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0043: //sPPEnergy ->sNNEnergy
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriEn.sNNEnergy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = 2;
                pData[nDataLen++] = e_double_long; //值 5
                nConvert = Bcdbuf2Long(&sAcTriEn.sNNEnergy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriEn.sNNEnergy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriEn.sNNEnergy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        
        case 0x0051: // &sAcTriQEn.sQ1Energy[0]
        case 0x0052:
        case 0x0053:  //A\B\C 三相 第一象限
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriQEn.sQ1Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = (OAD[1]&0x0F)-1;  //0x51  i = 0;
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriQEn.sQ1Energy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriQEn.sQ1Energy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriQEn.sQ1Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0061: // &sAcTriQEn.sQ2Energy[0]
        case 0x0062:
        case 0x0063:
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriQEn.sQ2Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = (OAD[1]&0x0F)-1; //分A B C三相 0 1 2 
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriQEn.sQ2Energy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriQEn.sQ2Energy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriQEn.sQ2Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0071: // &sAcTriQEn.sQ3Energy[0]
        case 0x0072:
        case 0x0073:
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriQEn.sQ3Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = (OAD[1]&0x0F)-1;
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriQEn.sQ3Energy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriQEn.sQ3Energy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriQEn.sQ3Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0x0081: // &sAcTriQEn.sQ4Energy[0]
        case 0x0082:
        case 0x0083:
        {
            if( g_nHardType==1 )
            {//单相表
                nDataLen = 0;
                pData[nDataLen++] = e_NoObject;
                break;
            }
            else
            {//三相表
                if( OAD[3]>1 )
                {
                    nDataLen = 0;
                    pData[nDataLen++] = e_RefuseRW;
                    break;
                }
                if (!IsAllBcd(&sAcTriQEn.sQ4Energy[0].nD1, (MAX_FEE_NUM+1)*sizeof(FORMAT11)))
                {
                    memset(pBuf , 0xEE ,(MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                if( OAD[3]==0 )
                {//索引是0 要求总
                    pData[nDataLen++]  = e_array;//数据 1
                    pData[nDataLen++]; //值 1
                }
                i = (OAD[1]&0x0F)-1;
                pData[nDataLen++] = e_double_long_unsigned; //值 6
                nConvert = Bcdbuf2Long(&sAcTriQEn.sQ4Energy[i].nD1, sizeof(FORMAT11));
                Long2Buf(&sAcTriQEn.sQ4Energy[i].nD1, nConvert, sizeof(FORMAT11));
                ReverseOutput(&sAcTriQEn.sQ4Energy[i].nD1, pData + nDataLen, sizeof(FORMAT11));
                nDataLen += sizeof(FORMAT11);   
                if( OAD[3]==0 )
                {		
                    nArrayNum ++;
                    pData[1] = nArrayNum;
                }
                break;
            }
        }
        case 0xFF16://获取设备类型
        {
                memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
                MemReadAcPrm(F44, &sAc_Param.nHardType, sizeof(sAc_Param.nHardType)); 
                pData[nDataLen++] = 0x11;
                pData[nDataLen++] = sAc_Param.nHardType+3;
                break;
        }
        case 0xFF99://获取版本号
        {            
              pData[nDataLen++] = 0x09;
              pData[nDataLen++] = 16;
              strcat((char*)pData, (char*)"VM");
              strcat((char*)pData, (char*)PRODUCT);
              #ifdef MIN_PRODUCT
              strcat((char*)pData, (char*)MIN_PRODUCT);
              #endif
              strcat((char*)pData, (char*)"-");
              strcat((char*)pData, (char*)MAIN_VERSION_INFO);
              strcat((char*)pData, (char*)VERSION_INFO);
              #ifdef MIN_VERSION_INFO
              strcat((char*)pData, (char*)"-");
              strcat((char*)pData, (char*)MIN_VERSION_INFO);
              #endif
              nDataLen +=  16;
              break;
        }
                

        case 0x301B:    //开盖事件
            {
                if( OAD[2]==2 )
                {
                    nDataLen = GB698GetEventMakeFrm(F30,&pData[nDataLen],F41);
                }
            }
            break;
        case 0x301F:    //跳闸事件
            {
                if( OAD[2]==2 )
                {
                    nDataLen = GB698GetEventMakeFrm(F31,&pData[nDataLen],F42);
                }
            }
            break;
        case 0x3020:    //合闸事件
            {
                if( OAD[2]==2 )
                {
                    nDataLen = GB698GetEventMakeFrm(F32,&pData[nDataLen],F43);
                }
            }
            break;
        case 0x3026:    //密钥更新事件
            {
                if( OAD[2]==2 )
                {
                    memset(&sAc_Report, 0, sizeof(MT_REPORT_DATA));
                    MemReadAcEvtProc(F33,1, &sAc_Report,sizeof(sAc_Report));
                    
                    pData[nDataLen++] = e_array;
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = e_structure;    
                    pData[nDataLen++] = 0x06;
               
                    pData[nDataLen++] = e_double_long_unsigned;
                    //nDataLen +=3;
                    pData[nDataLen++] = 0x00;
                    pData[nDataLen++] = 0x00;
                    pData[nDataLen++] = 0x00;
                    pData[nDataLen++] = sAc_Report.sOrderNum;
                    
                    pData[nDataLen++] = e_date_time_s;
                    pData[nDataLen++] = 0x07;
                    pData[nDataLen++] = sAc_Report.sStartTimestamp.nYear + 0xC4;//BCD2INT32U(&nTempBuf[0], 4)
                    pData[nDataLen++] = sAc_Report.sStartTimestamp.nMon;
                    pData[nDataLen++] = sAc_Report.sStartTimestamp.nDay;
                    pData[nDataLen++] = sAc_Report.sStartTimestamp.nHour;
                    pData[nDataLen++] = sAc_Report.sStartTimestamp.nMin;
                    pData[nDataLen++] = sAc_Report.sStartTimestamp.nSec;

                    pData[nDataLen++] = e_date_time_s;
                    pData[nDataLen++] = 0x07;
                    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nYear + 0xC4;
                    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMon;
                    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nDay;
                    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nHour;
                    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMin;
                    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nSec;

                    pData[nDataLen++] = 0x00;
                    
                    pData[nDataLen++] = e_array;
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = e_structure;
                    pData[nDataLen++] = 0x02;
                    pData[nDataLen++] = e_OAD;
                    pData[nDataLen++] = 0xf2;//上报通道待改
                    pData[nDataLen++] = 0x01;
                    pData[nDataLen++] = 0x02;
                    pData[nDataLen++] = 0x00;
                    pData[nDataLen++] = e_unsigned;
                    pData[nDataLen++] = 0x01;
                    
                    pData[nDataLen++] = e_array;
                    pData[nDataLen++] = 0x00;
                }
            }
            break;
        case 0x302b:    //负荷开关误触动事件
            {
                if( OAD[2]==2 )
                {
                    nDataLen = GB698GetEventMakeFrm(F34,&pData[nDataLen],F45);
                }
            }
            break;
        case 0x302C:    //电源异常事件
            {
                if( OAD[2]==2 )
                {
                    nDataLen = GB698GetEventMakeFrm(F35,&pData[nDataLen],F46);
                }
            }
            break;
        case 0x302E:    //时钟故障事件
            {
                if( OAD[2]==2 )
                {
                    nDataLen = GB698GetEventMakeFrm(F36,&pData[nDataLen],F47);
                }
            }
            break;
        case 0x302F:    //计量芯片故障事件
            {
                if( OAD[2]==2 )
                {
                    nDataLen = GB698GetEventMakeFrm(F37,&pData[nDataLen],F48);
                }
            }
            break;
         
        default:
            nDataLen = 0;
            pData[nDataLen++] = e_NoObject;
            break;
    }

    return nDataLen;
}   

//事件通用函数
INT8S GB698GetEventMakeFrm(INT8U nFn, INT8U *pData,INT8U nFn2)
{
    INT8U             nDataLen = 0;
    INT32U            nConvert = 0;
    INT8U             j;
    INT8U             i;
    INT8U             ii;
    INT16U            nOI3;
    INT8U             nCount = 0;
    INT8U             nCount2 = 0;
    pLINK_COM  	 	  pLinkCom  ;// = (pLINK_COM)pPara;
    MT_REPORT_DATA    sAc_Report;
    MT_OAD_DATA       nOAD;
    memset(&sAc_Report, 0, sizeof(MT_REPORT_DATA));
    memset(&nOAD, 0, sizeof(MT_OAD_DATA));
    MemReadAcEvtProc(nFn,1, &sAc_Report,sizeof(sAc_Report));
    MemReadAcEvtProc(nFn2,1, &nOAD, sizeof(MT_OAD_DATA));
    
    pData[nDataLen++] = e_array;
    pData[nDataLen++] = 0x01;
    pData[nDataLen++] = e_structure;
    pData[nDataLen++] = 0x06;

    pData[nDataLen++] = e_double_long_unsigned;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = sAc_Report.sOrderNum;
    
    pData[nDataLen++] = e_date_time_s;
    pData[nDataLen++] = 0x07;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nYear + 0xC4;//BCD2INT32U(&nTempBuf[0], 4)
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nMon;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nDay;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nHour;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nMin;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nSec;

    pData[nDataLen++] = e_date_time_s;
    pData[nDataLen++] = 0x07;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nYear + 0xC4;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMon;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nDay;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nHour;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMin;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nSec;

    pData[nDataLen++] = 0x00;
    
    pData[nDataLen++] = e_array;
    pData[nDataLen++] = 0x01;
    pData[nDataLen++] = e_structure;
    pData[nDataLen++] = 0x02;
    pData[nDataLen++] = e_OAD;
    pData[nDataLen++] = 0xf2;//上报通道待改
    //
    if((g_nFlg_Commu==2))
    {
        pData[nDataLen++] = 0x01;
    }
    else if(g_nFlg_Commu==4)
    {
        pData[nDataLen++] = 0x09;
    }
    else if(g_nFlg_Commu==3)
    {
        pData[nDataLen++] = 0x02;
    }
    
    pData[nDataLen++] = 0x02;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = e_unsigned;
    pData[nDataLen++] = 0x01;
    
    pData[nDataLen++] = e_array;
    for(j =0;j<64;j++)
    {
        if(nOAD.sOAD[j].nD3 !=0x00)
        {
            nCount2++;
        }
    }
    pData[nDataLen++] = nCount2 + nCount2;
    for(i=0;i<nCount2;i++)
    {
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = nOAD.sOAD[i].nD1;
        pData[nDataLen++] = nOAD.sOAD[i].nD2;
        pData[nDataLen++] = 0x22;//nOAD.sOAD[i].nD3;
        pData[nDataLen++] = nOAD.sOAD[i].nD4;
        //nOAD.sOAD[i].nD4;
        nOI3 = (((INT16U)(nOAD.sOAD[i].nD1 << 8) | nOAD.sOAD[i].nD2));
        if((nOI3==0x0010)||(nOI3==0x0020)||(nOI3==0x0030)||(nOI3==0x0040)||(nOI3==0x0050)||(nOI3==0x0060)||(nOI3==0x0070)
           ||(nOI3==0x0080))
        {
                switch(nOAD.sOAD[i].nD4)
                {
                   case 0:
                      pData[nDataLen++] = 0x01;
                      pData[nDataLen++] = 0x05;
                      for(ii=0;ii<= MAX_FEE_NUM;ii++)
                      {
                          pData[nDataLen++] = e_double_long_unsigned; 
                          nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT11));
                          Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT11));
                          ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
                          nDataLen += sizeof(FORMAT11);
                          nCount++;
                      }
                      break;
                    case 1:
                    case 2:
                    case 3:
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = e_double_long_unsigned; 
                        nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT11));
                        Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT11));
                        ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
                        nDataLen += sizeof(FORMAT11);
                        nCount++;
                        break;
                }
        }
        else if(nOI3==0x2000)
        {
                switch(nOAD.sOAD[i].nD4)
                {
                   case 0:
                      pData[nDataLen++] = 0x01;
                      pData[nDataLen++] = 0x03;
                      for(ii=0;ii< MAX_PHASE_NUM;ii++)
                      {
                          pData[nDataLen++] = e_long_unsigned; 
                          nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT07));
                          Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT07));
                          ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT07));
                          nDataLen += sizeof(FORMAT07);
                          nCount++;
                      }
                      break;
                    case 1:
                    case 2:
                    case 3:
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = e_long_unsigned; 
                        nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT07));
                        Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT07));
                        ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT07));
                        nDataLen += sizeof(FORMAT07);
                        nCount++;
                        break;
                }
        }
        else if(nOI3==0x2001)
        {
                switch(nOAD.sOAD[i].nD4)
                {
                   case 0:
                      pData[nDataLen++] = 0x01;
                      pData[nDataLen++] = 0x03;
                      for(ii=0;ii< MAX_PHASE_NUM;ii++)
                      {
                          pData[nDataLen++] = e_double_long; 
                          nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT11));
                          Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT11));
                          ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
                          nDataLen += sizeof(FORMAT11);
                          nCount++;
                      }
                      break;
                    case 1:
                    case 2:
                    case 3:
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = e_double_long; 
                        nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT25));
                        Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT25));
                        ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT25));
                        nDataLen += sizeof(FORMAT25);
                        nCount++;
                        break;
                }
        }
        else
        {
            //pData[nDataLen++] = 0x01;
            pData[nDataLen++] = 0x00;
            nCount++;
        }
        
    }
    nCount = 0;
    for(i=0;i<nCount2;i++)//结束数据
    {
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = nOAD.sOAD[i].nD1;
        pData[nDataLen++] = nOAD.sOAD[i].nD2;
        pData[nDataLen++] = 0x82;//nOAD.sOAD[i].nD3;
        pData[nDataLen++] = nOAD.sOAD[i].nD4;
        //nOAD.sOAD[i].nD4;
        nOI3 = (((INT16U)(nOAD.sOAD[i].nD1 << 8) | nOAD.sOAD[i].nD2));
        if((nOI3==0x0010)||(nOI3==0x0020)||(nOI3==0x0030)||(nOI3==0x0040)||(nOI3==0x0050)||(nOI3==0x0060)||(nOI3==0x0070)
           ||(nOI3==0x0080))
        {
                switch(nOAD.sOAD[i].nD4)
                {
                   case 0:
                      pData[nDataLen++] = 0x01;
                      pData[nDataLen++] = 0x05;
                      for(ii=0;ii<= MAX_FEE_NUM;ii++)
                      {
                          pData[nDataLen++] = e_double_long_unsigned; 
                          nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT11));
                          Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT11));
                          ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
                          nDataLen += sizeof(FORMAT11);
                          nCount++;
                      }
                      break;
                    case 1:
                    case 2:
                    case 3:
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = e_double_long_unsigned; 
                        nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT11));
                        Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT11));
                        ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
                        nDataLen += sizeof(FORMAT11);
                        nCount++;
                        break;
                }
        }
        else if(nOI3==0x2000)
        {
                switch(nOAD.sOAD[i].nD4)
                {
                   case 0:
                      pData[nDataLen++] = 0x01;
                      pData[nDataLen++] = 0x03;
                      for(ii=0;ii< MAX_PHASE_NUM;ii++)
                      {
                          pData[nDataLen++] = e_long_unsigned; 
                          nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT07));
                          Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT07));
                          ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT07));
                          nDataLen += sizeof(FORMAT07);
                          nCount++;
                      }
                      break;
                    case 1:
                    case 2:
                    case 3:
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = e_long_unsigned; 
                        nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT07));
                        Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT07));
                        ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT07));
                        nDataLen += sizeof(FORMAT07);
                        nCount++;
                        break;
                }
        }
        else if(nOI3==0x2001)
        {
                switch(nOAD.sOAD[i].nD4)
                {
                   case 0:
                      pData[nDataLen++] = 0x01;
                      pData[nDataLen++] = 0x03;
                      for(ii=0;ii< MAX_PHASE_NUM;ii++)
                      {
                          pData[nDataLen++] = e_double_long; 
                          nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT11));
                          Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT11));
                          ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
                          nDataLen += sizeof(FORMAT11);
                          nCount++;
                      }
                      break;
                    case 1:
                    case 2:
                    case 3:
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = 0x01;
                        pData[nDataLen++] = e_double_long; 
                        nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT25));
                        Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT25));
                        ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT25));
                        nDataLen += sizeof(FORMAT25);
                        nCount++;
                        break;
                }
        }
        else
        {
            pData[nDataLen++] = 0x01;
            pData[nDataLen++] = 0x00;
            nCount++;
        }
        
    }
    return nDataLen;
}

//分项事件通用函数
INT8S GB698GetFXEventMakeFrm(INT8U nFn, INT8U *pData,INT8U nNum)
{
    INT8U       nDataLen = 0;
    INT32U      nConvert = 0;
    INT8U       nEveData[LEN_AC_EVENT];
    INT8U       nEveData2[6];
    INT8U       nTempBuf[50];
    INT8U       nCount = 0;
    INT8U       ii;
    INT8U       n;
    MT_REPORT_DATA          sAc_Report;
    
    memset(&sAc_Report, 0, sizeof(MT_REPORT_DATA));
    MemReadAcEvtProc(nFn,1, &sAc_Report,sizeof(MT_REPORT_DATA));
    //MemReadAcEvtProc(F38 + ii, 1, &nEveData, LEN_AC_EVENT);
    
    pData[nDataLen++] = e_array;
    pData[nDataLen++] = 0x01;
    pData[nDataLen++] = e_structure;
    pData[nDataLen++] = 0x06;

    pData[nDataLen++] = e_double_long_unsigned;
    //nDataLen +=3;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = sAc_Report.sOrderNum;
    
    pData[nDataLen++] = e_date_time_s;
    pData[nDataLen++] = 0x07;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nYear+0xC4;
    //memcpy(&pData[nDataLen++],&sAc_Report.sStartTimestamp,5);
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nMon;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nDay;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nHour;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nMin;
    pData[nDataLen++] = sAc_Report.sStartTimestamp.nSec;
    //nDataLen +=4;

    pData[nDataLen++] = e_date_time_s;
    pData[nDataLen++] = 0x07;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nYear+0xC4;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMon;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nDay;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nHour;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMin;
    pData[nDataLen++] = sAc_Report.sFinishTimestamp.nSec;
    //memcpy(&pData[nDataLen++],&nEveData[54],5);
    //nDataLen +=4;

    pData[nDataLen++] = 0x00;
    
    pData[nDataLen++] = e_array;
    pData[nDataLen++] = 0x01;
    pData[nDataLen++] = e_structure;
    pData[nDataLen++] = 0x02;
    pData[nDataLen++] = e_OAD;
    pData[nDataLen++] = 0xf2;//上报通道待改
    
    if((g_nFlg_Commu==2))
    {
        pData[nDataLen++] = 0x01;
    }
    else if(g_nFlg_Commu==4)
    {
        pData[nDataLen++] = 0x09;
    }
    else if(g_nFlg_Commu==3)
    {
        pData[nDataLen++] = 0x02;
    }
    
    pData[nDataLen++] = 0x02;
    pData[nDataLen++] = 0x00;
    pData[nDataLen++] = e_unsigned;
    pData[nDataLen++] = 0x01;
    
    pData[nDataLen++] = e_array;
    if(nNum ==0)
    {
        pData[nDataLen++] = 0x08;
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x10;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x01;
        
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        //pData[nDataLen++] = e_double_long_unsigned;
        
        //for(ii=0;ii<= MAX_FEE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            nCount++;
        }

        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
 
        //for(ii=0;ii<= MAX_FEE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            nCount++;
        }
        
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x01;
        //pData[nDataLen++] = e_long_unsigned;
        nCount = 8;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x03;
        for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT07));
            Long2Buf(&sAc_Report.sPAData[nCount].nD1, nConvert, sizeof(FORMAT07));
            ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT07));
            nDataLen += sizeof(FORMAT07);
            nCount+=6;
        }
        
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x03;
        nCount = 9;
        for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAData[nCount].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAData[nCount].nD1, 1.5*nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            nCount+=6;
        }
        
        //结束数据
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x10;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x01;
        
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        //pData[nDataLen++] = e_double_long_unsigned;
        nCount =0;
        //for(ii=0;ii<= MAX_FEE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            nCount++;
        }

        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
 
        //for(ii=0;ii<= MAX_FEE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            nCount++;
        }
        
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x01;
        //pData[nDataLen++] = e_long_unsigned;
        nCount = 8;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x03;
        for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT07));
            Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT07));
            ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT07));
            nDataLen += sizeof(FORMAT07);
            nCount+=6;
        }
        
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x03;
        nCount = 9;
        for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[nCount].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAFinshData[nCount].nD1, nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAFinshData[nCount].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            nCount+=6;
        }
    }
    else //if(nNum ==1)
    {
        n = 6*(nNum -1);
        pData[nDataLen++] = 0x08;
    
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x10+nNum;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = e_double_long_unsigned; 
        nConvert = Bcdbuf2Long(&sAc_Report.sPAData[4+n].nD1, sizeof(FORMAT11));
        Long2Buf(&sAc_Report.sPAData[4+n].nD1, nConvert, sizeof(FORMAT11));
        ReverseOutput(&sAc_Report.sPAData[4+n].nD1, pData + nDataLen, sizeof(FORMAT11));
        nDataLen += sizeof(FORMAT11);

        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x20+nNum;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = e_double_long_unsigned;
        nConvert = Bcdbuf2Long(&sAc_Report.sPAData[5+n].nD1, sizeof(FORMAT11));
        Long2Buf(&sAc_Report.sPAData[5+n].nD1, nConvert, sizeof(FORMAT11));
        ReverseOutput(&sAc_Report.sPAData[5+n].nD1, pData + nDataLen, sizeof(FORMAT11));
        nDataLen += sizeof(FORMAT11);
        
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x01;
        
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;

        //for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAData[8+n].nD1, sizeof(FORMAT07));
            Long2Buf(&sAc_Report.sPAData[8+n].nD1, nConvert, sizeof(FORMAT07));
            ReverseOutput(&sAc_Report.sPAData[8+n].nD1, pData + nDataLen, sizeof(FORMAT07));
            nDataLen += sizeof(FORMAT07);
            //nCount+=6;
        }
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x22;
        pData[nDataLen++] = 0x01;
        
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        //nCount = 10;
        //for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAData[9+n].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAData[9+n].nD1, 1.5*nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAData[9+n].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            //nCount+=6;
        }
        
        //jieshu
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x10+nNum;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = e_double_long_unsigned; 
        nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[4+n].nD1, sizeof(FORMAT11));
        Long2Buf(&sAc_Report.sPAFinshData[4+n].nD1, nConvert, sizeof(FORMAT11));
        ReverseOutput(&sAc_Report.sPAFinshData[4+n].nD1, pData + nDataLen, sizeof(FORMAT11));
        nDataLen += sizeof(FORMAT11);

        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x20+nNum;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = e_double_long_unsigned;
        nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[5+n].nD1, sizeof(FORMAT11));
        Long2Buf(&sAc_Report.sPAFinshData[5+n].nD1, nConvert, sizeof(FORMAT11));
        ReverseOutput(&sAc_Report.sPAFinshData[5+n].nD1, pData + nDataLen, sizeof(FORMAT11));
        nDataLen += sizeof(FORMAT11);
        
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x01;
        
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;

        //for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_long_unsigned; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[8+n].nD1, sizeof(FORMAT07));
            Long2Buf(&sAc_Report.sPAFinshData[8+n].nD1, nConvert, sizeof(FORMAT07));
            ReverseOutput(&sAc_Report.sPAFinshData[8+n].nD1, pData + nDataLen, sizeof(FORMAT07));
            nDataLen += sizeof(FORMAT07);
            //nCount+=6;
        }
        pData[nDataLen++] = e_RSD;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x20;
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x82;
        pData[nDataLen++] = 0x01;
        
        pData[nDataLen++] = 0x01;
        pData[nDataLen++] = 0x01;
        //nCount = 10;
        //for(ii=0;ii< MAX_PHASE_NUM;ii++)
        {
            pData[nDataLen++] = e_double_long; 
            nConvert = Bcdbuf2Long(&sAc_Report.sPAFinshData[9+n].nD1, sizeof(FORMAT11));
            Long2Buf(&sAc_Report.sPAFinshData[9+n].nD1, nConvert, sizeof(FORMAT11));
            ReverseOutput(&sAc_Report.sPAFinshData[9+n].nD1, pData + nDataLen, sizeof(FORMAT11));
            nDataLen += sizeof(FORMAT11);
            //nCount+=6;
        }
    }

    return nDataLen;
}

/*********************************
*功能：记录型事件函数 03
*函数名：GB698GetRecordEventMakeFrm
*输入：nFn
*输出:
***********************************/

INT8S GB698GetRecordEventMakeFrm(INT8U nFn, INT8U *pData)
{
    INT8U             nDataLen = 0;
    pLINK_COM  	 	  pLinkCom  ;// = (pLINK_COM)pPara;
    MT_REPORT_DATA    sAc_Report;
    MT_OAD_DATA       nOAD;
    memset(&sAc_Report, 0, sizeof(MT_REPORT_DATA));
    memset(&nOAD, 0, sizeof(MT_OAD_DATA));
    MemReadAcEvtProc(nFn,1, &sAc_Report,sizeof(sAc_Report));
    
    pData[nDataLen++] = 0x03;
    pData[nDataLen++] = e_null;
    pData[nDataLen++] = 0x20;    
    pData[nDataLen++] = 0x22;
    pData[nDataLen++] = 0x02;
    pData[nDataLen++] = 0x00;
    
    pData[nDataLen++] = e_null;//e_CSD
    pData[nDataLen++] = 0x20;    
    pData[nDataLen++] = 0x1e;
    pData[nDataLen++] = 0x02;
    pData[nDataLen++] = 0x00;
    
    pData[nDataLen++] = e_null;
    pData[nDataLen++] = 0x20;    
    pData[nDataLen++] = 0x20;
    pData[nDataLen++] = 0x02;
    pData[nDataLen++] = 0x00;
    
    pData[nDataLen++] = e_array;
    
    pData[nDataLen++] = 0x01;
    
    if( 0x00== sAc_Report.sOrderNum)  //一般不会用到
    {
        pData[nDataLen++] = e_null;
        pData[nDataLen++] = e_null;
        pData[nDataLen++] = e_null;
        
        return nDataLen;
    }
    else
    {
        pData[nDataLen++] = e_double_long_unsigned;
        //nDataLen +=3;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = 0x00;
        pData[nDataLen++] = sAc_Report.sOrderNum;//sAc_Report.sOrderNum
    }
    
    if( 0xFE== sAc_Report.sStartTimestamp.nYear)  //不会用到
    {
        pData[nDataLen++] = e_null;
    }
    else
    {
        pData[nDataLen++] = e_date_time_s;
        pData[nDataLen++] = 0x07;
        pData[nDataLen++] = sAc_Report.sStartTimestamp.nYear + 0xC4;//BCD2INT32U(&nTempBuf[0], 4)
        pData[nDataLen++] = sAc_Report.sStartTimestamp.nMon;
        pData[nDataLen++] = sAc_Report.sStartTimestamp.nDay;
        pData[nDataLen++] = sAc_Report.sStartTimestamp.nHour;
        pData[nDataLen++] = sAc_Report.sStartTimestamp.nMin;
        pData[nDataLen++] = sAc_Report.sStartTimestamp.nSec;
    }
    
    if( 0xFE== sAc_Report.sFinishTimestamp.nYear)
    {
        pData[nDataLen++] = e_null;
    }
    else
    {
        pData[nDataLen++] = e_date_time_s;
        pData[nDataLen++] = 0x07;
        pData[nDataLen++] = sAc_Report.sFinishTimestamp.nYear + 0xC4;
        pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMon;
        pData[nDataLen++] = sAc_Report.sFinishTimestamp.nDay;
        pData[nDataLen++] = sAc_Report.sFinishTimestamp.nHour;
        pData[nDataLen++] = sAc_Report.sFinishTimestamp.nMin;
        pData[nDataLen++] = sAc_Report.sFinishTimestamp.nSec;
    }
    return nDataLen;
}



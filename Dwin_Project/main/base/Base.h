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
#ifndef _BASE_H_
#define _BASE_H_
#include <stdio.h>
#include <string.h>
#include "Os_cpu.h"
#include "global.h"
#include "Macro.h"
#include "Table.h"
/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
INT8U IsNumber(INT8U nAscii );
INT8U IsAbc(INT8U nAscii );

INT8U IsOneBcd(INT8U nData);
BOOLEAN IsAllBcd(const INT8U *pData, INT32U nLen);
//判断数据是否在范围内
BOOLEAN ChkRange(INT32U nData, INT32U nMin, INT32U nMax);
BOOLEAN IsAllEqualVal(const INT8U* pData, INT32U nLen, INT8U nByte);
INT8U 	CheckSum(const INT8U *pData, INT32U nStart, INT32U nLen);
INT8S 	ReplaceItem(INT32U *pItem, INT32U nLen, INT32U nVal);
INT32S 	StrCmp(const INT8U* pSrc, INT32S nSrcSize, INT8U* pCmp, INT32S nCmpSize);

INT32U 	MemCopy(void *pDst, void *pSrc, INT32U nLen);
INT32U  memcmp2(INT8U *pDst, INT8U *pSrc, INT32U nLen);
//半字节换位
INT8U 	HalfByteRel(INT8U nVal);
//判断是否能整除
BOOLEAN CheckCalMode(INT32U nSrc,INT32U nBase);
//计算整除
INT32U 	CalModeVal(INT32U nSrc,INT32U nBase);

INT8U 	SwapBit(INT8U nData);
//倒置缓冲区赋值函数
void 	SwapBufVal(INT8U* pData, INT16U nMaxLen);






INT8U 	AsciiToVal(INT8U AsciiVal);
INT32U 	StringToVal(INT8U* pSrc, INT32S nLen);
INT32S 	StringToBuf(INT8U* pSrc, INT8U* pDes, INT32S nLen);
INT32S 	StringToBufSwap(INT8U* pSrc, INT8U* pDes, INT32S nLen);


//把一个数据值转换为ASCII码
INT8U 	ValToAsciiSub(INT8U *pBuf,INT32U nVal,INT8U nMaxLen, INT8U nFormat, INT8U nFormatLen);
INT8U 	ValToAscii(INT8U *pBuf,INT32U nVal,INT8U nMaxLen);
INT8U 	ValHexToAscii(INT8U *pBuf,INT32U nVal,INT8U nMaxLen);
INT8S 	Long2Bcdbuf(INT32U nVal, INT8U *pBuf, INT8U nLen);
INT32U 	Bcdbuf2Long(const INT8U* pBuf, INT8U nLen);
INT8S 	Long2Buf(INT8U *pBuf, INT32U nVal, INT8U nLen);
INT32U 	Buf2Long(const INT8U *pBuf, INT8U nLen);
INT8S 	INT16U2Buf(INT8U *pBuf,INT16U nVal);
INT16U 	Buf2INT16U(const INT8U *pBuf);

void    INT32U2BCD(INT32U nSrc, INT8U nLen, INT8U *pBuf);
INT32U  BCD2INT32U(INT8U *pBuf, INT8U nLen);

void    Bcd2DecBuf(INT8U* nData, INT8U   nSize);


//置位和清位操作
BOOLEAN TestBufBit(const INT8U *pData, INT32U nDataLen, INT32U nBit);
BOOLEAN SetBufBit(INT8U *pData, INT32U nDataLen, INT32U nBit);
BOOLEAN ClrBufBit(INT8U *pData, INT32U nDataLen, INT32U nBit);





//判断两个缓冲区值大小
INT8S 	CompareTwoBufVal(const INT8U* const pSrc1, const INT8U* const pSrc2, INT16U nMaxLen, INT8U* pFlg);
//两个缓冲区值相减
INT8S 	CalcTwoBufDiff(const INT8U* const pSrc1, const INT8U* const pSrc2, INT8U* pDes, INT16U nMaxLen, INT8U nFormat, BOOLEAN bFlg);




//时间相关的函数
INT32S  GetDiffTime2(const pSTD_TIME pEnd, const pSTD_TIME pStart);
INT8S   GetDiffTime3(const pSTD_TIME pEnd, const pSTD_TIME pStart, INT32S *pDelta);
BOOLEAN IsVldStdTime(const pSTD_TIME pTime);
BOOLEAN IsVldStdTime2(INT8U nYear, INT8U nMon, INT8U nDay, INT8U nHour, INT8U nMin, INT8U nSec);
BOOLEAN IsLeapyear(INT16U nYear);
INT32U 	GetSecsFrom2KYear(const pSTD_TIME pTime);
INT8S   GetDiffTimeSecs(const pSTD_TIME pEnd, const pSTD_TIME pStart, INT32S* pSec);
INT8S 	GetLocalTime(INT32U nSec, pSTD_TIME pDest);
INT8S 	GetPtrDay(const pSTD_TIME pCurrTime, pSTD_TIME pLastTime, INT32S nDays);
INT8S 	GetPtrMon(const pSTD_TIME pCurrTime, pSTD_TIME pLastTime, INT32S nMonths);
INT8S 	GetLastDay(pSTD_TIME pLastTime, const pSTD_TIME pCurrTime);
INT8S  	GetNextDay(pSTD_TIME pNextTime, const pSTD_TIME pCurrTime);
INT8S  	GetLastMon(pSTD_TIME pLastTime, const pSTD_TIME pCurrTime);
INT8S  	GetNextMon(pSTD_TIME pLastTime, const pSTD_TIME pCurrTime);
INT16U  GetDaysFrom2MMDD(INT8U Year, INT8U Mon, INT8U Day);



INT32S 	BufUtf8ToGbkHz(INT8U* pGbkBuf, INT32S nGbkSize, const INT8U* pUtfBuf, INT32S nUtfSize);
INT32S 	BufGbkHzToUtf8(const INT8U* pGbkBuf, INT32S nGbkSize, INT8U* pUtfBuf, INT32S nUtfSize);



//模块空函数
void 	VoidFunc(void *pData);



INT8S 	CheckVldFormat09(const pFORMAT09 pVal);
INT8S   CheckVldFormat11(const pFORMAT11 pVal);
INT8S 	CheckVldFormat25(const pFORMAT25 pVal);
FP32 	Format072Float(pFORMAT07 pSrc);
FP32 	Format092Float(const pFORMAT09 pSrc);
FP32 	Format252Float(pFORMAT25 pSrc);
INT8S   Float2Format05(FP32 fSrc, pFORMAT05 pDest);
INT8S   Float2Format09(FP32 fSrc, pFORMAT09 pDest);
INT8S   Float2Format11(FP32 fSrc, pFORMAT11 pDest);
INT8S   Float2Format13(FP32 fSrc, pFORMAT13 pDest);

INT16U CalcBlockCRC16(INT16U nCount, INT16U nCrc, INT8U *pBuffer);

INT8U ReplaceAscii(INT8U *pData, INT8U nLen, INT8U nSrc, INT8U nDest);
void DrawFormat25(pFORMAT25 pFmat25, INT8U* nDispBuf);

INT32U Float2BCD(float nSrc);
INT8S   ReverseOutput(INT8U*  Src, INT8U*   Des, INT8U  nLen);
BOOLEAN IsAddrFieldVal(const INT8U* pData, INT32U nLen);

#endif

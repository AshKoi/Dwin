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
#include "Fun.h"
#include "PowerOn.h"

const unsigned short ccitt_16[256]=
{               /*- CRC余式表 -*/
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/

/**************************************************************************************************
                                           数值函数
***************************************************************************************************/
/****************************************************************************
* 函数名:IsNumber
* 输　入:
*		INT8U	nAscii
* 输　出:
*		TRUE		是数字
*		FALSE
* 功  能:
****************************************************************************/
INT8U IsNumber(INT8U nAscii )
{
	if ((nAscii >= '0') && (nAscii <= '9'))
	{
		return TRUE;
	}

	return FALSE;
}

/****************************************************************************
* 函数名:IsAbc
* 输　入:
*		INT8U	nAscii
* 输　出:
*		TRUE		是数字
*		FALSE
* 功  能:
****************************************************************************/
INT8U IsAbc(INT8U nAscii )
{
	if ((nAscii >= 'a') && (nAscii <= 'z'))
	{
		return TRUE;
	}

	if ((nAscii >= 'A') && (nAscii <= 'Z'))
	{
		return TRUE;
	}

	return FALSE;
}



/****************************************************************************
* 函数名:IsOneBcd
* 输　入:
*		INT8U	nData  BCD字节输入参数
* 输　出:
*		TRUE        在上下限范围内
*		FALSE       在上下限范围外
* 功  能:单字节BCD有效性判断
****************************************************************************/
INT8U IsOneBcd(INT8U nData )
{
	INT8U nVal = 0;

    nVal = nData & 0x0F;
	if (nVal > 9)
    {
        return FALSE;
    }
    nVal = (nData & 0xF0) >> 4;
	if (nVal > 9)
    {
        return FALSE;
    }
    return TRUE;
}

/****************************************************************************
* 函数名:IsAllBcd
* 输　入:
*		const INT8U* 		pData      BCD码输入数据源
*       INT32U 				nLen       要求判断的数据长度
* 输　出:
*		TRUE        在上下限范围内
*		FALSE       在上下限范围外
* 功  能:多字节BCD有效性判断
****************************************************************************/
BOOLEAN IsAllBcd(const INT8U* pData, INT32U nLen)
{
	INT32U i = 0;

	if((NULL == pData) || (nLen == 0))
    {
        return FALSE;
    }
	for(i = 0; i < nLen; i++)
    {
        if(!IsOneBcd(pData[i]))
        {
            return FALSE;
        }
	}
    return TRUE;
}

/********************************************************************************
* 函数名:ChkRange
* 输　入:
*		INT32U		nData       要求核查的输入数据
*		INT32U		nMin        下限
*		INT32U		nMax        上限
* 输　出:
*		TRUE        在上下限范围内
*		FALSE       在上下限范围外
* 返  回:
* 功  能: 核查数值范围
*********************************************************************************/
BOOLEAN ChkRange(INT32U nData, INT32U nMin, INT32U nMax)
{
    if ((nData >= nMin) && (nData <= nMax))
    {
        return TRUE;
    }

    return FALSE;
}

/***************************************************************************
* 函数名:IsAllEqualVal
* 输　入:
*		const INT8U*	pData    数据源
*       INT32U			nLen     需要判断的数据长度
*       INT8U			nByte    特征字
* 输　出:
* 返  回:
*		TRUE     是
*       FALSE    否
* 功  能:判断数据是否全为某无效特征字数据(如0xEE,0xFF,0x00等)
***************************************************************************/
BOOLEAN IsAllEqualVal(const INT8U* pData, INT32U nLen, INT8U nByte)
{
    INT32U i = 0;
    
    if(pData == NULL)
    {
        return FALSE;
    }
	for(i = 0; i < nLen; i++)
    {
	    if((pData[i] != nByte))
        {
	        return FALSE;
        }
	}

	return TRUE;
}

/****************************************************************************
* 函数名:CheckSum
* 输　入:INT8U*       pData             缓冲数据
*        INT16U       nStart            计算效验的开始点
*        INT16U       nLeng             计算长度
* 输　出:
* 返  回:效验和
* 功  能:计算效验和
* 版本：Version 1.0
****************************************************************************/
INT8U CheckSum(const INT8U *pData, INT32U nStart, INT32U nLen)
{
	INT8U   cs = 0;
	INT32U  i  = 0;

    if(NULL == pData)
    {
        return 0;
    }
	for (i = nStart; i < (nStart + nLen); i++)
	{
		cs = cs + pData[i];
	}

    return cs;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 替换数组中重复数值
*********************************************************************************/
INT8S ReplaceItem(INT32U *pItem, INT32U nLen, INT32U nVal)
{
	INT32U  i = 0;
	INT32U  j = 0;

	if (NULL == pItem)
	{
		return RET_ERR_POINT;
	}
	if (nLen <= 1)
	{
		return RET_ERR_OK;
	}

	for (i = 0; i < nLen-1; i++)
	{
		for (j = i+1; j < nLen; j++)
		{
			if (pItem[i] == pItem[j])
			{
				pItem[j] = nVal;
			}
		}
	}
    
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 替换数组中重复数值
*********************************************************************************/
INT32S StrCmp(const INT8U* pSrc, INT32S nSrcSize, INT8U* pCmp, INT32S nCmpSize)
{
	INT32S i = 0;

	if (NULL == pSrc)
	{
		return -1;
	}
	if (NULL == pCmp)
	{
		return -1;
	}
	if (nSrcSize < nCmpSize)
	{
		return -1;
	}
	else if (nSrcSize == nCmpSize)
	{
		if (0 == memcmp(pSrc, pCmp, nCmpSize))
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	for (i = 0; i <= nSrcSize - nCmpSize; i++)
	{
		if (0 == memcmp(&pSrc[i], pCmp, nCmpSize))
		{
			return 0;
		}
	}

	return -1;
}

/********************************************************************************
* 函数名: MemCopy
* 输　入:
* 输　出:
* 返  回:
* 功  能: 内存拷贝
*********************************************************************************/
INT32U MemCopy(void *pDst, void *pSrc, INT32U nLen)
{
	if ((NULL == pDst) || (NULL == pSrc))
	{
		return 0;
	}
	memcpy(pDst, pSrc, nLen);
	return nLen;
}

/********************************************************************************
* 函数名: memcmp2
* 输　入:
* 输　出:
* 返  回:
* 功  能: 内存拷贝
*********************************************************************************/
INT32U memcmp2(INT8U *pDst, INT8U *pSrc, INT32U nLen)
{
    INT32U      j = 0;

    for (j = 0; j < nLen; j++)
    {
        if (pDst[j] != pSrc[j])
        {
            break;
        }
    }

    if (j < nLen)
    {
        return j + 1;
    }

    return 0;
}



/*****************************************************************************
* 函 数 名:HalfByteRel
* 输　  入:nVal     待转换的源数据
* 输　  出:
* 返    回:
* 功能描述:0x68->0x86
******************************************************************************/
INT8U HalfByteRel(INT8U nVal)
{
    INT8U   nRet  = 0;
 	INT8U   nTmp1 = 0;
 	INT8U   nTmp2 = 0;

 	nTmp1 = (nVal >> 4) & 0x0F;
 	nTmp2 = nVal & 0x0F;

 	nRet = ((nTmp2 << 4) & 0xF0) | (nTmp1);

 	return nRet;
}


/*****************************************************************************
* 函 数 名:CheckCalMode
* 输　  入:
* 输　  出:
* 返    回:
* 功能描述:判断是否能整除:能返回真
******************************************************************************/
BOOLEAN CheckCalMode(INT32U nSrc,INT32U nBase)
{
	if (0 == nBase)
	{
		return FALSE;
	}
	else if (1 == nBase)
	{
		return TRUE;
	}
	else
	{
		if (0 == (nSrc % nBase))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}
/*****************************************************************************
* 函 数 名:CalModeVal
* 输　  入:
* 输　  出:
* 返    回:
* 功能描述:计算整除
******************************************************************************/
INT32U CalModeVal(INT32U nSrc,INT32U nBase)
{
	INT32U	nRet	= 0;

	if(0 == nBase)
	{
		return 0;
	}
	else if(1 == nBase)
	{
		return 0;
	}
	else
	{
		nRet = nSrc % nBase;
	}

	return nRet;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT8U SwapBit(INT8U nData)
{
	INT8U i = 0;
	INT8U nTmp = 0;

	for (i = 0; i < 8; i++)
	{
		if (nData & (1 << i))
		{
			nTmp |= (1 << (7 - i));
		}
	}
	return nTmp;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:倒置缓冲区赋值函数：pData[3] = {0,1,2} -> pData[3] = {2,1,0}
*********************************************************************************/
void SwapBufVal(INT8U* pData, INT16U nMaxLen)
{
	INT8U	nTmp = 0;
	INT16U  i    = 0;

	if ((NULL == pData) || (nMaxLen < 2 ))
	{
		return;
	}
	for (i = 0; i < (nMaxLen / 2); i++)
	{
		nTmp = pData[i];
		pData[i] = pData[nMaxLen - 1 - i];
		pData[nMaxLen - 1 - i] = nTmp;
	}
}






/*****************************************************************************
* 函 数 名:AsciiToVal
* 输　  入:AsciiVal     待转换的源数据
* 输　  出:
* 返    回:
* 功能描述:把Ascii转换成实际数值
******************************************************************************/
INT8U AsciiToVal(INT8U AsciiVal)
{
    INT8U nTemp = 0;

	nTemp = AsciiVal;
    if ((nTemp >= '0') && (nTemp <= '9'))
	{
		nTemp -= '0';
	}
	else if ((nTemp >= 'A') && (nTemp <= 'F'))
	{
		nTemp = (nTemp - 'A' + 10) & 0x0F;
	}
	else if ((nTemp >= 'a') && (nTemp <= 'f'))
	{
		nTemp = (nTemp - 'a' + 10) & 0x0F;
	}
	else
	{
		nTemp = 0;
	}

    return nTemp;
}

/*****************************************************************************
* 函 数 名:StringToVal
* 输　  入:StringToVal     待转换的源数据
*				INT32S		nLen    需为偶数
* 输　  出:
* 返    回:
* 功能描述:把Ascii转换成实际数值数组
******************************************************************************/
INT32U StringToVal(INT8U* pSrc, INT32S nLen)
{
	INT32S 	i 		= 0;
	INT32U	nRet	= 0;

	for (i = 0; i < nLen; i++)
	{
		nRet = nRet * 10 + AsciiToVal(pSrc[i]);
	}

	return nRet;
}

/*****************************************************************************
* 函 数 名:StringToBuf
* 输　  入:StringToVal     待转换的源数据
*				INT32S		nLen    需为偶数
* 输　  出:
* 返    回:
* 功能描述:把Ascii转换成实际数值数组
******************************************************************************/
INT32S StringToBuf(INT8U* pSrc, INT8U* pDes, INT32S nLen)
{
    INT32S 	i = 0;
	INT32S	j = 0;

	if ((NULL == pSrc) || (NULL == pDes) || (0 == nLen))
	{
		return -1;
	}
	if ((nLen % 2))
	{
		j = (nLen - 1) / 2 + 1;
	}
	else
	{
		j = nLen / 2;
	}
	for (i = 0; i < j; i++)
	{
		if ((nLen % 2))
		{
			if (0 == i)
			{
				pDes[0] = AsciiToVal(pSrc[0]);
			}
			else
			{
				pDes[i] = AsciiToVal(pSrc[(i - 1) * 2 + 1]) * 0x10 + AsciiToVal(pSrc[(i - 1) * 2 + 2]);
			}
		}
		else
		{
			pDes[i] = AsciiToVal(pSrc[i * 2 + 0]) * 0x10 + AsciiToVal(pSrc[i * 2 + 1]);
		}
	}

	return j;
}

/*****************************************************************************
* 函 数 名:StringToBufSwap
* 输　  入:StringToBufSwap     待转换的源数据
*				INT32S		nLen    需为偶数
* 输　  出:
* 返    回:
* 功能描述:把Ascii转换成实际数值数组
******************************************************************************/
INT32S StringToBufSwap(INT8U* pSrc, INT8U* pDes, INT32S nLen)
{
	INT32S nTmp = 0;

	nTmp = StringToBuf(pSrc, pDes, nLen);
	if (nTmp < 0)
	{
		return -1;
	}

	SwapBufVal(pDes, nTmp);

	return nTmp;
}

/********************************************************************************
* 函数名:
* 输　入:
*			INT8U *pBuf				转换后存储的缓冲区
*			INT32U nVal             待转换值
*			INT8U nMaxLen           转换后存储的缓冲区的最大长度
*           INT8U nFormat			转换格式：0为%d,1为%x
*			, INT8U nFormatLen		转换补齐长度格式:0为%x,2为%02x,...依次类推，8为%08x。
* 输　出:
* 返  回:
* 功  能:把HEX码转换为ASCII码,返回转换的长度:1转换为0x31,返回长度为1;100转换为0x31,0x30,0x30,返回长度为3
*********************************************************************************/
INT8U ValToAsciiSub(INT8U *pBuf,INT32U nVal,INT8U nMaxLen, INT8U nFormat, INT8U nFormatLen)
{
 	INT8U   nRet  = 0;
 	INT8U	nTempBuf[11];//0xFFFFFFFF -> 4294967295,共10位数

 	if ((NULL == pBuf) || (0 == nMaxLen))
 	{
 		return 0;
 	}
 	memset(&nTempBuf[0],0,sizeof(nTempBuf));
 	if (1 == nFormat)
 	{
 		if (2 == nFormatLen)
 		{
 			sprintf((char*)(&nTempBuf[0]),"%02x",nVal);
 		}
 		else if (4 == nFormatLen)
 		{
 			sprintf((char*)(&nTempBuf[0]),"%04x",nVal);
 		}
 		else if (8 == nFormatLen)
 		{
 			sprintf((char*)(&nTempBuf[0]),"%08x",nVal);
 		}
 		else
 		{
	 		sprintf((char*)(&nTempBuf[0]),"%x",nVal);
	 	}
 	}
 	else
 	{
 		if (2 == nFormatLen)
 		{
 			sprintf((char*)(&nTempBuf[0]),"%02d",nVal);
 		}
 		else if (4 == nFormatLen)
 		{
 			sprintf((char*)(&nTempBuf[0]),"%04d",nVal);
 		}
 		else if (8 == nFormatLen)
 		{
 			sprintf((char*)(&nTempBuf[0]),"%08d",nVal);
 		}
 		else
 		{
	 		sprintf((char*)(&nTempBuf[0]),"%d",nVal);
	 	}
 	}
 	nRet = strlen((char*)(&nTempBuf[0]));
 	if (nRet > nMaxLen)
 	{
 		return 0;
 	}
 	memcpy(pBuf,&nTempBuf[0],nRet);

 	return nRet;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:把bin码转换为ASCII码,返回转换的长度:1转换为0x31,返回长度为1;100转换为0x31,0x30,0x30,返回长度为3
*********************************************************************************/
INT8U ValToAscii(INT8U *pBuf,INT32U nVal,INT8U nMaxLen)
{
 	return ValToAsciiSub(pBuf,nVal,nMaxLen, 0, 0);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:把HEX码转换为ASCII码,返回转换的长度:1转换为0x31,返回长度为1;100转换为0x31,0x30,0x30,返回长度为3
*********************************************************************************/
INT8U ValHexToAscii(INT8U *pBuf,INT32U nVal,INT8U nMaxLen)
{
 	return ValToAsciiSub(pBuf,nVal,nMaxLen, 1, 0);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT8S Long2Bcdbuf(INT32U nVal, INT8U *pBuf, INT8U nLen)
{
	INT8U i 	= 0;
	INT8U nTmp	= 0;

 	if ((NULL == pBuf) || (0 == nLen))
 	{
 		return RET_ERR_POINT;
 	}

	for (i = 0; i < nLen; i++)
	{
		pBuf[i] = 0;
	}

	i = 0;
	while(0 != nVal)
	{
		if (i >= nLen)
		{
			return RET_ERR_ERROR;
		}
		nTmp  = (INT8U)(nVal % 100);
		pBuf[i] = INT8U_TO_BCD(nTmp);
		nVal /= 100;
		i++;
	}

 	return RET_ERR_OK;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/

INT32U Bcdbuf2Long(const INT8U* pBuf, INT8U nLen)
{
	INT32U 	nRtn	= 0;
	INT16S 	i		= 0;
	INT8U 	nTmp	= 0;

	if ((NULL == pBuf) || (0 == nLen))
 	{
 		return 0;
 	}
	if (!IsAllBcd(pBuf, nLen))
	{
		return 0;
	}

	for (i = (nLen - 1); i >= 0; i--)
	{
		nRtn *= 100;
		nTmp  = BCD_TO_INT8U(pBuf[i]);
		nRtn += nTmp;
	}

	return (nRtn);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT8S Long2Buf(INT8U *pBuf, INT64U nVal, INT8U nLen)
{
	INT8U i 	= 0;
	INT8U nTmp	= 0;

 	if ((NULL == pBuf) || (0 == nLen))
 	{
 		return RET_ERR_POINT;
 	}

	for (i = 0; i < nLen; i++)
	{
		pBuf[i] = 0;
	}

	i = 0;
	while(0 != nVal)
	{
		if (i >= nLen)
		{
			return RET_ERR_ERROR;
		}
		nTmp  = (INT8U)(nVal % 0x100);
		pBuf[i] = (nTmp);
		nVal /= 0x100;
		i++;
	}

 	return RET_ERR_OK;

	/*
 	if (NULL == pBuf)
 	{
 		return RET_ERR_POINT;
 	}

 	pBuf[0] = nVal % 0x100;
 	pBuf[1] = nVal % 0x10000 / 0x100;
 	pBuf[2] = nVal % 0x1000000 / 0x10000;
 	pBuf[3] = nVal / 0x1000000;

 	return RET_ERR_OK;
 	*/
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT32U Buf2Long(const INT8U *pBuf, INT8U nLen)
{
	INT32U 	nRtn	= 0;
	INT16S 	i		= 0;
	INT8U 	nTmp	= 0;

	if ((NULL == pBuf) || (0 == nLen))
 	{
 		return 0;
 	}
	for (i = (nLen - 1); i >= 0; i--)
	{
		nRtn *= 0x100;
		nTmp  = (pBuf[i]);
		nRtn += nTmp;
	}

	return (nRtn);
	/*
	INT32U nVal = 0;

 	if (NULL == pBuf)
 	{
 		return 0;
 	}
 	nVal = pBuf[0] + pBuf[1] * 0x100 + pBuf[3] * 0x10000 + pBuf[4] * 0x1000000;

 	return nVal;
 	*/
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT8S INT16U2Buf(INT8U *pBuf,INT16U nVal)
{
 	return Long2Buf(pBuf, (INT32U)nVal, 2);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT16U Buf2INT16U(const INT8U *pBuf)
{
 	return Buf2Long(pBuf, 2);
}

/****************************************************************************
* 函数名：INT32U2BCD
* 输  入：nSrc 4字节数字
*         nLen 字节数
* 输  出：pBuf 数据缓冲区
* 返  回：无
* 功  能：将数据从BIN码转换到BCD格式
****************************************************************************/
void INT32U2BCD(INT32U nSrc, INT8U nLen, INT8U *pBuf)
{
    //Long2Bcdbuf(nSrc, pBuf, nLen);
#if 1
   	INT8U nTmp = 0;
    INT8U i    = 0;

	if( (nSrc > 9999) && (nLen == 4) )
	{
		pBuf[0] = 0;
		pBuf[1] = 0;
		return;
	}

   	if((nSrc > 999999) && ( nLen == 6) )
	{
		pBuf[0] = 0;
		pBuf[1] = 0;
		pBuf[2] = 0;
		return;
	}
	if(( nSrc > 99999999 ) && ( nLen == 8) )
	{
		pBuf[0] = 0;
		pBuf[1] = 0;
		pBuf[2] = 0;
		pBuf[3] = 0;
		return;
	}
	if(nLen <= 0)
    {
        return;
    }
	if(nLen > 20)
    {
        return;
    }
	for(i = 1; i <= nLen; i++)
	{
		nTmp >>= 4;
		nTmp += ((INT32U)nSrc % 10) << 4;
		nSrc /= 10;
		if((i % 2)!=0)
        {
            continue;
        }
		*pBuf++ = nTmp;
		nTmp = 0;
	}
	if((nLen % 2) != 0)
    {
        *pBuf = (nTmp >> 4);
    }
#endif
}


/****************************************************************************
* 函数名：BCD2INT32U
* 输  入：pBuf BCD数据缓冲
*         nLen 字节数
* 输  出：
* 返  回：4字节数据
* 功  能：将数据从BCD码转换到BIN格式
****************************************************************************/
INT32U  BCD2INT32U(INT8U *pBuf, INT8U nLen)
{
    //return Bcdbuf2Long(pBuf, nLen);
#if 1
	INT32U   nTmp = 0;
	INT8U    i    = 0;

	if(nLen > 20)
    {
		return 0;
    }
	if(nLen <= 0)
    {
		return 0;
    }

	for(i = nLen; i > 0; i--)
	{
		nTmp *= 10;
		if(i % 2)
        {
			nTmp += ((*(pBuf + ((i - 1)/2))) & 0xF);
        }
		else
        {
			nTmp += ((*(pBuf + ((i - 1)/2))) >> 4);
        }
	}
	return nTmp;
#endif
}




/********************************************************************************
* 函数名:
* 输　入:
*				const INT8U* const pSrc1				源缓冲区1
*				const INT8U* const pSrc2				源缓冲区2
*				INT16U nMaxLen							缓冲区长度
*				INT8U* pFlg
* 输　出:
* 返  回:
* 功  能:判断pSrc1缓冲区是否大于pSrc2缓冲区：大于时为1，小于为0，等于为2。pSrc1[nMaxLen - 1]为高位
*********************************************************************************/
INT8S CompareTwoBufVal(const INT8U* const pSrc1, const INT8U* const pSrc2, INT16U nMaxLen, INT8U* pFlg)
{
	INT16U		i				= 0;
	INT16U		j				= 0;
	INT8U		nSign			= 0;

	if ((NULL == pSrc1) || (NULL == pSrc2) || (0 == nMaxLen))
	{
		return RET_ERR_ERROR;
	}

	for (i = 0; i < nMaxLen; i++)
	{
		if (pSrc1[nMaxLen - 1 - i] > pSrc2[nMaxLen - 1 - i])
		{
			nSign = 1;														//pSrc1大于pSrc2标志
			break;
		}
		else if (pSrc1[nMaxLen - 1 - i] < pSrc2[nMaxLen - 1 - i])
		{
			nSign = 0;														//pSrc1小于pSrc2标志
			break;
		}
		else
		{
			j++;
		}
	}

	if (nMaxLen == j)
	{
		*pFlg = 2;
	}
	else
	{
		*pFlg = nSign;
	}

	return RET_ERR_OK;
}
/********************************************************************************
* 函数名:
* 输　入:
*				const INT8U* const pSrc1				源缓冲区1
*				const INT8U* const pSrc2				源缓冲区2
*				INT8U* 	pDes							目标缓冲区
*				INT16U 	nMaxLen							缓冲区长度
*				INT8U 	nFormat							格式：0为BCD,1为HEX
*				BOOLEAN bFlg							判断缓冲区大小标志：真时判断
* 输　出:
* 返  回:
* 功  能:pSrc1 - pSrc2,pSrc1[nMaxLen - 1]为高位
*********************************************************************************/
INT8S CalcTwoBufDiff(const INT8U* const pSrc1, const INT8U* const pSrc2, INT8U* pDes, INT16U nMaxLen, INT8U nFormat, BOOLEAN bFlg)
{
	INT16U		i				= 0;
	INT8U		nSign			= 0;
	INT8U		nTmp1			= 0;
	INT8U		nTmp2			= 0;

	if ((NULL == pSrc1) || (NULL == pSrc2) || (NULL == pDes) || (0 == nMaxLen))
	{
		return RET_ERR_ERROR;
	}

	//1.判断大小
	if (bFlg)
	{
		if (RET_ERR_OK != CompareTwoBufVal(pSrc1, pSrc2, nMaxLen, &nSign))
		{
			return RET_ERR_ERROR;
		}
		if (2 == nSign)															//pSrc1等于pSrc2标志
		{
			memset(pDes, 0, nMaxLen);
			return RET_ERR_OK;
		}
		else if (0 == nSign)													//pSrc1小于pSrc2不做计算
		{
			return RET_ERR_ERROR;
		}
	}

	//2.计算
	nSign = 0;																//借位标志清0
	if (1 == nFormat)														//16进制
	{
		for (i = 0; i < nMaxLen; i++)
		{
			nTmp1 = pSrc1[i];
			nTmp2 = pSrc2[i];
			if(nTmp1 < (nSign + nTmp2))
			{
				pDes[i] = (nTmp1 + 0x100 - nSign - nTmp2);
				nSign   = 1;
			}
			else
			{
				pDes[i] = nTmp1 - nSign - nTmp2;
				nSign   = 0;
			}
		}
	}
	else																	//BCD
	{
		if ((!IsAllBcd((INT8U*)pSrc1, nMaxLen)) || (!IsAllBcd((INT8U*)pSrc2, nMaxLen)))		//非BCD不做计算
		{
			return RET_ERR_ERROR;
		}
		for (i = 0; i < nMaxLen; i++)
		{
			nTmp1 = BCD_TO_INT8U(pSrc1[i]);
			nTmp2 = BCD_TO_INT8U(pSrc2[i]);
			if(nTmp1 < (nSign + nTmp2))
			{
				pDes[i] = INT8U_TO_BCD((nTmp1 + 100 - nSign - nTmp2));
				nSign   = 1;
			}
			else
			{
				pDes[i] = INT8U_TO_BCD(nTmp1 - nSign - nTmp2);
				nSign   = 0;
			}
		}
	}

	return RET_ERR_OK;
}

/******************************************************************************
* 函数名:TestBit
* 输　入:pData        源数据
*        nBit         指定位
* 输　出:
* 返  回:TRUE  指定位为1
*        FALSE 指定位为0
* 功  能: 数据位测试([0, n])
*******************************************************************************/
BOOLEAN TestBufBit(const INT8U *pData, INT32U nDataLen, INT32U nBit)
{
    INT8U nRet = 0;
    if(NULL == pData)
    {
        return FALSE;
    }
    if((nBit / 8) >= nDataLen)
    {
        return FALSE;
    }
	nRet = TEST_BIT(pData[nBit / 8], (nBit % 8));
    return ((nRet == 0) ? (FALSE): (TRUE));
}

/********************************************************************************
* 函数名:SetBufBit
* 输　入:pData        源数据
*        nBit         指定位
* 输　出:
* 返  回:
* 功  能: 数据位置位([0, n])
*********************************************************************************/
BOOLEAN SetBufBit(INT8U *pData, INT32U nDataLen, INT32U nBit)
{
    if(NULL == pData)
    {
        return FALSE;
    }
    if((nBit / 8) >= nDataLen)
    {
        return FALSE;
    }
    SET_BIT(pData[nBit / 8], (nBit % 8));
    return TRUE;
}

/********************************************************************************
* 函数名:ClrBufBit
* 输　入:pData        源数据
*        nBit         指定位
* 输　出:
* 返  回:
* 功  能: 数据位清位([0, n])
*********************************************************************************/
BOOLEAN ClrBufBit(INT8U *pData, INT32U nDataLen, INT32U nBit)
{
    if(NULL == pData)
    {
        return FALSE;
    }
    if((nBit / 8) >= nDataLen)
    {
        return FALSE;
    }
    CLR_BIT(pData[nBit / 8], (nBit % 8));
    return TRUE;
}




/**************************************************************************************************
                                           时钟函数
***************************************************************************************************/
/***************************************************************************
* 函 数 名:IsVldDateTime
* 输　  入:nYear        年 BCD码输入
*          nMon         月 BCD码输入
*          nDay         日 BCD码输入
* 输　  出:
* 返    回:TRUE          日期有效
*          FALSE         日期无效
* 功能描述:判断输入的日期是否有效
***************************************************************************/
BOOLEAN IsVldDateTime(INT8U nYear, INT8U nMon, INT8U nDay)
{
    if((nMon == 0) || (nMon > 0x12))
    {
        return FALSE;
    }
    if((nDay == 0) || (nDay > 0x31))
    {
        return FALSE;
    }
    if ((!IsOneBcd(nYear))
     	|| (!IsOneBcd(nMon))
     	|| (!IsOneBcd(nDay)))
    {
        return FALSE;
    }

    if(   ((nMon == 0x04) && (nDay > 0x30))
       || ((nMon == 0x06) && (nDay > 0x30))
       || ((nMon == 0x09) && (nDay > 0x30))
       || ((nMon == 0x11) && (nDay > 0x30)))
    {
        return FALSE;
    }
    if(IsLeapyear(BCD_TO_INT8U(nYear)))
    {
        if((nMon == 0x02) && (nDay > 0x29))
        {
            return FALSE;
        }
    }
    else
    {
        if((nMon == 0x02) && (nDay > 0x28))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/***************************************************************************
* 函 数 名:IsVldClkTime
* 输　  入:nHour        时 BCD码输入
*          nMin         分 BCD码输入
*          nSec         秒 BCD码输入
* 输　  出:
* 返    回:TRUE          时钟有效
*          FALSE         时钟无效
* 功能描述:判断输入的时钟是否有效
***************************************************************************/
BOOLEAN IsVldClkTime(INT8U nHour, INT8U nMin, INT8U nSec)
{
    if ((nHour > 0x23) || (nMin > 0x59) || (nSec > 0x59))
    {
        return FALSE;
    }
	if ((!IsOneBcd(nHour))
     	|| (!IsOneBcd(nMin))
     	|| (!IsOneBcd(nSec)))
    {
        return FALSE;
    }
    return TRUE;
}

/********************************************************************************
* 函数名:GetDiffTime
* 输　入:pTime      标准数据格式输入
* 输　出:
* 返  回:秒数
* 功  能:同一天内的时间相减，输入参数中的日月年不参与计算
********************************************************************************/
INT32S GetDiffTime2(const pSTD_TIME pEnd, const pSTD_TIME pStart)
{
    INT32S nDelta = 0;

    INT8U nEndHour, nEndMin, nEndSec, nStartHour, nStartMin, nStartSec;

	nEndHour   = BCD_TO_INT8U(pEnd->nHour);
	nEndMin    = BCD_TO_INT8U(pEnd->nMin);
	nEndSec    = BCD_TO_INT8U(pEnd->nSec);
	nStartHour = BCD_TO_INT8U(pStart->nHour);
	nStartMin  = BCD_TO_INT8U(pStart->nMin);
	nStartSec  = BCD_TO_INT8U(pStart->nSec);

	nDelta = (INT32S)(nEndHour * 3600 + nEndMin * 60 + nEndSec)
            -(INT32S)(nStartHour * 3600 + nStartMin * 60 + nStartSec);

	return nDelta;
}

/********************************************************************************
* 函数名:GetDiffTime3
* 输　入:pTime      标准数据格式输入
* 输　出:
* 返  回:秒数
* 功  能:时间相差24小时内的时间相减，输入参数中的日月年不参与计算
********************************************************************************/
INT8S GetDiffTime3(const pSTD_TIME pEnd, const pSTD_TIME pStart, INT32S *pDelta)
{
    INT8S nRet = RET_ERR_ERROR;
    STD_TIME sTmpTime;
    pSTD_TIME pTmpTime = &sTmpTime;

    if (0 == (pEnd->nDay - pStart->nDay))    //同一天
    {
        *pDelta = GetDiffTime2(pEnd, pStart);
        nRet = RET_ERR_OK;
    }
    else
    {
        if (RET_ERR_OK == GetLastDay(pTmpTime, pEnd))
        {
            if (0 == (pTmpTime->nDay - pStart->nDay))
            {
                *pDelta = GetDiffTime2(pTmpTime, pStart) + 24 * 60 * 60;//加上一天的时间
                nRet = RET_ERR_OK;
            }
            else
            {
                if (GetNextDay(pTmpTime, pEnd))
                {
                    if (0 == (pTmpTime->nDay - pStart->nDay))
                    {
                        *pDelta = GetDiffTime2(pTmpTime, pStart) - 24 * 60 * 60;//减去一天的时间
                        nRet = RET_ERR_OK;
                    }
                }
            }
        }
    }
    return nRet;
}

/***************************************************************************
* 函 数 名:IsVldStdTime
* 输　  入:pTime         需要判断的时钟输入
* 输　  出:
* 返    回:TRUE          sTime有效
*          FALSE         sTime无效
* 功能描述:判断输入的标准时间格式sTime是否有效
***************************************************************************/
BOOLEAN IsVldStdTime(const pSTD_TIME pTime)
{
    BOOLEAN bRet1 = FALSE;
    BOOLEAN bRet2 = FALSE;

    if (NULL == pTime)
    {
        return FALSE;
    }

    bRet1 = IsVldDateTime(pTime->nYear, pTime->nMon, pTime->nDay);
    bRet2 = IsVldClkTime(pTime->nHour, pTime->nMin, pTime->nSec);

    return (bRet1 & bRet2);
}

/***************************************************************************
* 函 数 名:IsVldStdTime2
* 输　  入:nYear        年 BCD码输入
*          nMon         月 BCD码输入
*          nDay         日 BCD码输入
*          nHour        时 BCD码输入
*          nMin         分 BCD码输入
*          nSec         秒 BCD码输入
* 输　  出:
* 返    回:TRUE          标准时间有效
*          FALSE         标准时间无效
* 功能描述:判断输入的标准时间是否有效
***************************************************************************/
BOOLEAN IsVldStdTime2(INT8U nYear, INT8U nMon, INT8U nDay, INT8U nHour, INT8U nMin, INT8U nSec)
{
    BOOLEAN bRet1 = FALSE;
    BOOLEAN bRet2 = FALSE;

    bRet1 = IsVldDateTime(nYear, nMon, nDay);
    bRet2 = IsVldClkTime(nHour, nMin, nSec);

    return (bRet1 & bRet2);
}

/*
****************************************************************************
* 函数名:IsLeapyear
* 输　入: nYear       年，DEC输入格式
* 输  出：TRUE  润年
*         FALSE 非润年
* 功能描述:判断闰年
* 作　者:
* 日　期:
* 修　改:
* 日　期:
* 版本：Version 1.0
****************************************************************************/
BOOLEAN IsLeapyear(INT16U nYear)
{
    if((((nYear % 4) == 0) && ((nYear % 100) != 0)) || ((nYear % 400) == 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/********************************************************************************
* 函数名:GetDaysOfMonth
* 输　入:nMon      月 DEC格式输入
*        nYear     年 DEC格式输入
* 输　出:
* 返  回:天数
* 功  能:获取每月天数
********************************************************************************/
INT8U GetDaysOfMonth(INT8U nMon, INT16U nYear)
{
    INT8U		c_nDaysOfMonth[12] 	= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    INT8U  		nDaysPerMon 		= 0;

    if (!ChkRange(nMon, 1, 12))
    {
    	return 31;
    }
    nDaysPerMon = c_nDaysOfMonth[nMon - 1];
    if (2 == nMon)
    {
    	if (IsLeapyear(nYear))
        nDaysPerMon++;
    }

	return nDaysPerMon;
}

/********************************************************************************
* 函数名:GetDaysOfMonth
* 输　入:nYear      年 DEC格式输入  直接就是200x年
* 输　出:
* 返  回:天数
* 功  能:根据年取得当年的天数
********************************************************************************/
INT16U GetDaysOfYear(INT16U nYear)
{
   	if (nYear < 2000)
	{
   		return 0;
   	}
    if(IsLeapyear(nYear))
    {
        return 366;
    }
    else
    {
        return 365;
    }
}

/********************************************************************************
* 函数名:GetDaysFrom2KYear
* 输　入:pTime      标准数据格式输入
* 输　出:
* 返  回:天数
* 功  能:根据日期取得从2000年1月1日到现在的天数
********************************************************************************/
INT32U GetDaysFrom2KYear(const pSTD_TIME  pTime)
{
	INT32U 	nDayOfSum	= 0;
	INT8U 	nDay 		= 0;
	INT8U 	nMon 		= 0;
	INT16U 	nYear		= 0;
	INT16U 	i			= 0;

	if(NULL == pTime)
    {
        return 0;
    }
    if (!IsVldStdTime(pTime))
	{
		return 0;
	}

	nDay  = BCD_TO_INT8U(pTime->nDay);
	nMon  = BCD_TO_INT8U(pTime->nMon);
	nYear = BCD_TO_INT8U(pTime->nYear) + 2000;

	for (i = 2000; i < nYear; i++)
    {
		nDayOfSum += GetDaysOfYear(i);
    }
	for (i = 1; i < nMon; i++)
    {
		nDayOfSum += GetDaysOfMonth(i, nYear);
    }
	nDayOfSum += nDay;

	return nDayOfSum;
}

/********************************************************************************
* 函数名: GetDaysFrom2MMDD
* 输　入:pTime      标准数据格式输入
* 输　出:
* 返  回:天数
* 功  能:根据输入的年月日计算该日到1月1日的总天数
********************************************************************************/
INT16U GetDaysFrom2MMDD(INT8U Year, INT8U Mon, INT8U Day)
{
	INT8U nDay = 0, nMon = 0;
	INT8U nYear = 0;
	INT16U i, nDayOfSum = 0;

	nDay  = BCD_TO_INT8U(Day);
	nMon  = BCD_TO_INT8U(Mon);
	nYear = BCD_TO_INT8U(Year);

	for(i = 1; i < nMon; i++)
	{
		nDayOfSum += GetDaysOfMonth(i, 2000 + nYear);
	}
	nDayOfSum += nDay;

	return nDayOfSum;
}

/********************************************************************************
* 函数名:GetHoursFrom2KYear
* 输　入:pTime      标准数据格式输入
* 输　出:
* 返  回:分数
* 功  能:根据时间取得从2000年1月1日0时0分到现在的小时数
********************************************************************************/
INT32U GetHoursFrom2KYear(const pSTD_TIME pTime)
{
	INT32U nHours = 0;
	INT32U nDays  = 0;

    if(NULL == pTime)
    {
        return 0;
    }
    if (!IsVldStdTime(pTime))
	{
		return 0;
	}

	nDays   = GetDaysFrom2KYear(pTime) - 1;  //当天不计算在内
    nHours += nDays * 24;
    nHours += BCD_TO_INT8U(pTime->nHour);

	return nHours;
}

/********************************************************************************
* 函数名:GetMinsFrom2KYear
* 输　入:pTime      标准数据格式输入
* 输　出:
* 返  回:分数
* 功  能:根据时间取得从2000年1月1日0时0分到现在的分钟数
********************************************************************************/
INT32U GetMinsFrom2KYear(const pSTD_TIME pTime)
{
	INT32U nMins = 0;
	INT32U nDays = 0;
	INT8U  nHour = 0;
	INT8U  nMin  = 0;

    if(NULL == pTime)
    {
        return 0;
    }
    if (!IsVldStdTime(pTime))
	{
		return 0;
	}
	nHour = BCD_TO_INT8U(pTime->nHour);
	nMin  = BCD_TO_INT8U(pTime->nMin);

	nDays  = GetDaysFrom2KYear(pTime) - 1;  //当天不计算在内
	nMins += nDays * 1440;
	nMins += nHour * 60 + nMin;

	return nMins;
}

/********************************************************************************
* 函数名:GetSecsFrom2KYear
* 输　入:pTime      标准数据格式输入
* 输　出:
* 返  回:秒数
* 功  能:根据时间取得从2000年1月1日0时0分到现在的秒数
********************************************************************************/
INT32U GetSecsFrom2KYear(const pSTD_TIME pTime)
{
#if 1
	INT32U  nMins = 0, nSecs = 0;

    if(NULL == pTime)
    {
        return 0;
    }
    if (!IsVldStdTime(pTime))
	{
		return 0;
	}
	nMins = GetMinsFrom2KYear(pTime);
	nSecs = nMins * 60;
	nSecs += BCD_TO_INT8U(pTime->nSec);

	return nSecs;
#else
	INT8U		byYearHex		= 0;
    INT8U  		byMonthHex		= 0;
    INT8U 	 	byDayHex		= 0;
    INT8U  		byHourHex		= 0;
    INT8U  		byMinuteHex	= 0;
    INT8U  		bySecondHex	= 0;
    INT8U    	byDaysOfMon 	= 0;
    INT8U  		i				= 0;
    INT16U		wTemp			= 0;
	INT32U		nSecs			= 0;
    INT8U 		strSysTime;

    if (pTime == NULL)
	{
    	return 0;
    }

    memset(&strSysTime, 0, sizeof(strSysTime));
    memcpy((INT8U*)(&strSysTime), (INT8U*)pTime, sizeof(STD_TIME));
	if (!IsVldClkTime(&strSysTime))
	{
		return 0;
	}
	byYearHex 	= strSysTime.nYear;//BCD2DEC(strSysTime.year);
    byMonthHex 	= (strSysTime.nMon);
    byDayHex 	= (strSysTime.nDay);
    byHourHex 	= (strSysTime.nHour);
    byMinuteHex = (strSysTime.nMin);
    bySecondHex = (strSysTime.nSec);
    nSecs = 0;
    for (i = 0;i < byYearHex; i++)
	{
        wTemp = 2000 + i;
        if( IsLeapyear(wTemp) )
		{
            nSecs += 31622400;
        }
        else
		{
        	nSecs += 31536000;
        }
    }
    wTemp = 2000 + i;
    for(i = 1; i < byMonthHex; i++){
        GetDaysOfMonth(wTemp,i,&byDaysOfMon);
        nSecs += (INT32U)((INT32U)byDaysOfMon * 24 * 3600);
    }
    nSecs += (INT32U)(byDayHex - 1) * 24 * 3600;
    nSecs += (INT32U)byHourHex * 3600;
    nSecs += (INT32U)byMinuteHex * 60;
    nSecs += (INT32U)bySecondHex;

	return nSecs;
#endif
}

/****************************************************************************
* 函数名:GetDiffTimeSecs
* 输入:
*       const pSTD_TIME pEnd								计算时间
*       const pSTD_TIME pStart								初始时间
* 输出:
*       INT32S* 		pSec								返回相差的秒数
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:两个标准时间之差：单位为秒。
****************************************************************************/
INT8S GetDiffTimeSecs(const pSTD_TIME pEnd, const pSTD_TIME pStart, INT32S* pSec)
{
    INT32S nEndSec   = 0;
    INT32S nStartSec = 0;

    if ((NULL == pEnd) || (NULL == pStart) || (NULL == pSec))
    {
        return RET_ERR_POINT;
    }
    if (!IsVldStdTime(pEnd))
    {
    	return RET_ERR_RANG;
    }
    if (!IsVldStdTime(pStart))
    {
    	return RET_ERR_RANG;
    }
    nEndSec = GetSecsFrom2KYear(pEnd);
    nStartSec = GetSecsFrom2KYear(pStart);
    *pSec = nEndSec - nStartSec;

    return RET_ERR_OK;
}


/****************************************************************************************************
* 函 数 名:GetLocalTime
* 输　  入:nSec          要求转换的秒数
* 输　  出:pDest         输出一个标准格式的时钟
* 返    回:RET_ERR_OK    时标有效
*          RET_ERR_POINT 输入无效指针
* 功能描述:根据输入秒数，计算其日期
*****************************************************************************************************/
INT8S GetLocalTime(INT32U nSec, pSTD_TIME pDest)
{
    STD_TIME 	sTmpDest;
	INT32U  	nTmpSec = 0, nTmpMin = 0, nTmpHour = 0, nTmpDay = 0;
    INT8U 		nDaysBuf[13]	= {0,31,28,31,30,31,30,31,31,30,31,30,31};
    BOOLEAN 	bFlg 			= TRUE;
	INT8U 		nMon 			= 1;

	if ((NULL == pDest) || (nSec == 0))
	{
		return RET_ERR_POINT;
	}

	memset(&sTmpDest, 0, sizeof(STD_TIME));

	sTmpDest.nDay = 1;

	nTmpSec  = nSec % 60;			//sec
	nTmpMin  = nSec / 60;
	nTmpHour = nTmpMin / 60;
	nTmpMin  = nTmpMin % 60;		//min
	nTmpDay  = nTmpHour / 24 + 1;
	nTmpHour = nTmpHour % 24;		//hour

	while(nTmpDay >= 28)
	{
        if(bFlg && (nMon == 2))
        {
            if(nTmpDay > 29)
            {
                nTmpDay -= 29;
                nMon++;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(nTmpDay > nDaysBuf[nMon])
            {
                nTmpDay -= nDaysBuf[nMon++];
            }
            else
            {
                break;
            }
        }
        if(nMon > 12)
        {
            nMon = 1;
            sTmpDest.nYear++;
            bFlg = IsLeapyear(sTmpDest.nYear);
        }
    }
    sTmpDest.nDay  = INT8U_TO_BCD(nTmpDay);
	sTmpDest.nMon  = INT8U_TO_BCD(nMon);
	sTmpDest.nHour = INT8U_TO_BCD(nTmpHour);
	sTmpDest.nMin  = INT8U_TO_BCD(nTmpMin);
	sTmpDest.nSec  = INT8U_TO_BCD(nTmpSec);
	sTmpDest.nYear = INT8U_TO_BCD(sTmpDest.nYear);

	*pDest = sTmpDest;

    return RET_ERR_OK;
}

/*****************************************************************************
* 函数名:GetPtrDay
* 输　入:
*        pCurrTime     当前时钟
* 输　出:
* 返  回:RET_ERR_OK    获取昨天时钟
*        RET_ERR_POINT 输入指针为空
* 功  能: 1为下一日；-1为上一日
*****************************************************************************/
INT8S GetPtrDay(const pSTD_TIME pCurrTime, pSTD_TIME pLastTime, INT32S nDays)
{
	INT32U	nSecs	= 0;

	if ( 0 == (nSecs = GetSecsFrom2KYear(pCurrTime)))
	{
		return RET_ERR_ERROR;
	}

	nSecs += nDays * 86400;					// 1440 * 60
	if (RET_ERR_OK != GetLocalTime(nSecs, pLastTime))
	{
		return RET_ERR_ERROR;
	}

	return RET_ERR_OK;
}

/*****************************************************************************
* 函数名:GetPtrMon
* 输　入:
*        pCurrTime     当前时钟
* 输　出:
* 返  回:RET_ERR_OK    获取昨天时钟
*        RET_ERR_POINT 输入指针为空
* 功  能: 1为下一月；-1为上一月
*****************************************************************************/
INT8S GetPtrMon(const pSTD_TIME pCurrTime, pSTD_TIME pLastTime, INT32S nMonths)
{
	INT32U	nMonthsAbs	= 0;
	INT8U	nFlg		= 0;
	INT8S	nRet		= RET_ERR_ERROR;
    INT32U  i           = 0;

	if (nMonths < 0)
	{
		nMonthsAbs  = 0 - nMonths;
		nFlg		= 1;
	}
	else
	{
		nMonthsAbs = nMonths;
	}

	for (i = 0; i < nMonthsAbs; i++)
	{
		if (nFlg)
		{
			nRet = GetLastMon(pLastTime, pCurrTime);
		}
		else
		{
			nRet = GetNextMon(pLastTime, pCurrTime);
		}

		if (RET_ERR_OK != nRet)
		{
			return nRet;
		}
	}

	return RET_ERR_OK;
}


/*****************************************************************************
* 函数名:GetLastDay
* 输　入:pLastTime     获取昨天时钟
*        pCurrTime     当前时钟
* 输　出:pTime         返回昨天时钟（返回值为TRUE时获取）
* 返  回:RET_ERR_OK    获取昨天时钟
*        RET_ERR_POINT 输入指针为空
* 功  能: 获取昨天
*****************************************************************************/
INT8S GetLastDay(pSTD_TIME pLastTime, const pSTD_TIME pCurrTime)
{
    return GetPtrDay(pCurrTime, pLastTime, -1);
}

/********************************************************************************
* 函数名:GetLastMon
* 输　入:pLastTime     获取上月时钟
*        pCurrTime     当前时钟
* 输　出:pTime         返回上月时钟（返回值为TRUE时获取）
* 返  回:TRUE          获取上月时钟
*        FALSE         未获取
*        RET_ERR_POINT 输入指针为空
* 功  能: 获取上月
********************************************************************************/
INT8S  GetLastMon(pSTD_TIME pLastTime, const pSTD_TIME pCurrTime)
{
    if((NULL == pLastTime) || (NULL == pCurrTime))
    {
        return RET_ERR_POINT;
    }
	memcpy(pLastTime, pCurrTime, sizeof(STD_TIME));

    if(pLastTime->nMon > 1)
    {
        pLastTime->nMon = INT8U_TO_BCD(BCD_TO_INT8U(pLastTime->nMon) - 1);
    }
	else if(pLastTime->nMon == 1)
	{
	    pLastTime->nMon = 0x12;
	    pLastTime->nYear = INT8U_TO_BCD(BCD_TO_INT8U(pLastTime->nYear) - 1);
	}
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:GetNextDay
* 输　入:pNextTime     获取下一天时钟
*        pCurrTime     当前时钟
* 输　出:pTime         返回下一天时钟（返回值为TRUE时获取）
* 返  回:TRUE          获取下一天时钟
*        FALSE         未获取
*        RET_ERR_POINT 输入指针为空
* 功  能: 获取下一天时间
********************************************************************************/
INT8S  GetNextDay(pSTD_TIME pNextTime, const pSTD_TIME pCurrTime)
{
	return GetPtrDay(pCurrTime, pNextTime, 1);
}

/********************************************************************************
* 函数名:GetNextMon
* 输　入:pLastTime     获取上月时钟
*        pCurrTime     当前时钟
* 输　出:pTime         返回上月时钟（返回值为TRUE时获取）
* 返  回:TRUE          获取上月时钟
*        FALSE         未获取
*        RET_ERR_POINT 输入指针为空
* 功  能: 获取下月
********************************************************************************/
INT8S  GetNextMon(pSTD_TIME pLastTime, const pSTD_TIME pCurrTime)
{
    if((NULL == pLastTime) || (NULL == pCurrTime))
    {
        return RET_ERR_POINT;
    }
	memcpy(pLastTime, pCurrTime, sizeof(STD_TIME));

    if(pLastTime->nMon >= 12)
    {
        pLastTime->nMon = 1;
        pLastTime->nYear = INT8U_TO_BCD(BCD_TO_INT8U(pLastTime->nYear) + 1);
    }
	else if(pLastTime->nMon > 0)
	{
	    pLastTime->nMon = INT8U_TO_BCD(BCD_TO_INT8U(pLastTime->nMon) + 1);
	}
    return RET_ERR_OK;
}




/*****************************************************************************
* 函 数 名:Utf8ToUnicode
* 输　  入:
*				const INT8U* pUtfBuf
* 输　  出:
*				INT8U* pUniBuf
* 返    回:
* 功能描述:
******************************************************************************/
INT32S Utf8ToUnicode(INT8U* pUniBuf, const INT8U* pUtfBuf)
{
	if ((NULL == pUniBuf) || (NULL == pUtfBuf))
	{
		return -1;
	}
	pUniBuf[0] = ((pUtfBuf[0] << 4) & 0xF0) | ((pUtfBuf[1] >> 2) & 0x0F);
	pUniBuf[1] = ((pUtfBuf[1] << 6) & 0xC0) | (pUtfBuf[2] & 0x3F);

	return 0;
}


/*****************************************************************************
* 函 数 名:UnicodeToUtf8
* 输　  入:
*				const INT8U* pUniBuf
* 输　  出:
*				INT8U* pUtfBuf
* 返    回:
* 功能描述:
******************************************************************************/
INT32S UnicodeToUtf8(const INT8U* pUniBuf, INT8U* pUtfBuf)
{
	if ((NULL == pUniBuf) || (NULL == pUtfBuf))
	{
		return -1;
	}

	pUtfBuf[0] = ((pUniBuf[0] >> 4) & 0x0F) | 0xE0;
	pUtfBuf[1] = ((pUniBuf[0] << 2) & 0x3C) | ((pUniBuf[1] >> 6) & 0x03) | 0x80;
	pUtfBuf[2] = (pUniBuf[1] & 0x3F) | 0x80;

	return 0;
}




/*************************************************************************************************************************
* 函数    :   u16 OneGBKtoUNICODE(u16 GBKCode)
* 功能    :   将GBK编码转换为unicode编码
* 参数    :   GBK
* 返回    :   unicode
* 依赖    :   底层读写函数
* 作者    :   cp1300@139.com
* 时间    :   20120602
* 最后修改时间 : 20120602
* 说明    :   需要flash中的码表支持
            GBK码范围,高8位:0x81~0xfe;低8位:0x40~0xfe
*************************************************************************************************************************/
INT32S OneGBKtoUNICODE(const INT8U* pGBKCode, INT8U* pUnicode)
{
#define G_TO_U_ADDR			0//(MEM_FLH_G2U_UP_ADDR)
#ifdef G_TO_U_ADDR
    INT8U 	ch, cl;
    INT8U 	data[2] = {0};
	INT32U	offset = 0;

	if ((NULL == pGBKCode) || (NULL == pUnicode))
	{
		return -1;
	}

    ch = pGBKCode[0];
    cl = pGBKCode[1];

    ch -= 0x81;
    cl -= 0x40;

	if (ch<=0x7d && cl<=0xbe)
	{
        offset = (ch * 0xbf + cl);
        offset *= 2;
		offset += G_TO_U_ADDR;
        memset(data, 0, sizeof(data));
		//_API_FLASH_Read(data, offset, 2);
		MemRead(G2U_U2G_FILE, MEM_FLASH, offset, 2, data);
	}
	else
	{
		data[0] = 0xff;
        data[1] = 0x1f;
	}
	pUnicode[1] = data[0];
	pUnicode[0] = data[1];
#endif
    return 0;



   /*   ch = GBKCode >> 8;
    cl = GBKCode & 0x00ff;

    ch -= 0x81;
    cl -= 0x40;
    return (ch<=0x7d && cl<=0xbe) ? wUnicodes[ch*0xbf+cl] : 0x1fff;      */

}



/*************************************************************************************************************************
* 函数    :   u16 OneUNICODEtoGBK(u16 unicode)
* 功能    :   将unicode编码转换为GBK编码
* 参数    :   unicode
* 返回    :   GBK
* 依赖    :   底层读写函数
* 作者    :   cp1300@139.com
* 时间    :   20120602
* 最后修改时间 : 20120602
* 说明    :   需要flash中的码表支持
            GBK码范围,高8位:0x81~0xfe;低8位:0x40~0xfe
*************************************************************************************************************************/
INT32S OneUNICODEtoGBK(INT8U* pGBKCode, const INT8U* pUnicode)  //用二分查找算法
{
#define U_TO_G_ADDR			0xbc20//(MEM_FLH_G2U_UP_ADDR + 0xbc20)
#ifdef U_TO_G_ADDR
    INT32U  offset;
	INT16U	nUnicode = 0;

	if ((NULL == pGBKCode) || (NULL == pUnicode))
	{
		return -1;
	}

	nUnicode = MAKEWORD(pUnicode[1], pUnicode[0]);

    if (nUnicode <= 0X9FA5)
    {
        if (nUnicode >= 0X4E00)
            offset = nUnicode - 0X4E00;//0x1b87      //0X4E00,汉字偏移起点
        else
            return 0x2020;      //不能显示的字符就给两个空格填充,否则乱码
    }
    else if (nUnicode > 0X9FA5)//是标点符号
    {
        if (nUnicode < 0XFF01 || nUnicode > 0XFF61)
            return 0x2020;//没有对应编码  //不能显示的字符就给两个空格填充,否则乱码
        offset = nUnicode - 0XFF01 + 0X9FA6 - 0X4E00;
    }
    offset *= 2;
	offset += U_TO_G_ADDR;
	//_API_FLASH_Read(pGBKCode, offset, 2);
	MemRead(G2U_U2G_FILE, MEM_FLASH, offset, 2, pGBKCode);
#endif
    return 0;    //返回找到的编码
}


/*****************************************************************************
* 函 数 名:Utf8ToGbkHz
* 输　  入:
*				const INT8U* pUtfBuf
* 输　  出:
*				INT8U* pGbkBuf
* 返    回:
* 功能描述:
******************************************************************************/
static INT32S Utf8ToGbkHz(INT8U* pGbkBuf, const INT8U* pUtfBuf)
{
	INT8U  nBuf[2] = {0};

	Utf8ToUnicode(nBuf, pUtfBuf);
	OneUNICODEtoGBK(pGbkBuf, nBuf);

	return 0;
}

/*****************************************************************************
* 函 数 名:GbkHzToUtf8
* 输　  入:
*				const INT8U* pGbkBuf
* 输　  出:
*				INT8U* pUtfBuf
* 返    回:
* 功能描述:
******************************************************************************/
static INT32S GbkHzToUtf8(const INT8U* pGbkBuf, INT8U* pUtfBuf)
{
	INT8U  nBuf[2] = {0};

	OneGBKtoUNICODE(pGbkBuf, nBuf);
	UnicodeToUtf8(nBuf, pUtfBuf);
    return 0;
}

/*****************************************************************************
* 函 数 名:BufUtf8ToGbkHz
* 输　  入:
*				const INT8U* pUtfBuf
* 输　  出:
*				INT8U* pGbkBuf
* 返    回:
* 功能描述:
******************************************************************************/
INT32S BufUtf8ToGbkHz(INT8U* pGbkBuf, INT32S nGbkSize, const INT8U* pUtfBuf, INT32S nUtfSize)
{
	INT32S 	kk1 = 0;
	INT32S	kk2 = 0;

	if (pGbkBuf == NULL)
	{
		return -1;
	}

	if (pUtfBuf == NULL)
	{
		return -1;
	}

	while(1)
	{
		if (kk1 >= nUtfSize)
		{
			break;
		}
		if (kk2 >= nGbkSize)
		{
			break;
		}
		if (pUtfBuf[kk1] < 0x80)
		{
			pGbkBuf[kk2] = pUtfBuf[kk1];
			kk1++;
			kk2++;
		}
		else
		{
			Utf8ToGbkHz(&pGbkBuf[kk2], &pUtfBuf[kk1]);
			kk1 += 3;
			kk2 += 2;
		}
	}
	return 0;
}


/*****************************************************************************
* 函 数 名:BufGbkHzToUtf8
* 输　  入:
*				const INT8U* pGbkBuf
* 输　  出:
*				INT8U* pUtfBuf
* 返    回:
* 功能描述:
******************************************************************************/
INT32S BufGbkHzToUtf8(const INT8U* pGbkBuf, INT32S nGbkSize, INT8U* pUtfBuf, INT32S nUtfSize)
{
	INT32S 	kk1 = 0;
	INT32S	kk2 = 0;

	if (pGbkBuf == NULL)
	{
		return -1;
	}

	if (pUtfBuf == NULL)
	{
		return -1;
	}

	memset(pUtfBuf, 0, nUtfSize);

	while(1)
	{
		if (kk2 >= nGbkSize)
		{
			break;
		}
		if (pGbkBuf[kk2] < 0x80)
		{
			pUtfBuf[kk1] = pGbkBuf[kk2];
			kk1++;
			kk2++;
		}
		else
		{
			GbkHzToUtf8(&pGbkBuf[kk2], &pUtfBuf[kk1]);
			kk1 += 3;
			kk2 += 2;
		}
	}
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:空函数
*********************************************************************************/
void VoidFunc(void *pData)
{
	//pData = pData;
}



/********************************************************************************
* 函数名:CheckVldFormat09
* 输　入:pVal    输入数据格式09
* 输　出:
* 返  回:RET_ERR_ERROR => 输入数据无效
*        TRUE          => 数据数据有效但非零
* 功  能:判断输入的数据格式09数值是否有效
*********************************************************************************/
INT8S CheckVldFormat09(const pFORMAT09 pVal)
{
    INT8S nFlg = RET_INVLD;

    if (NULL == pVal)
    {
        return nFlg;
    }
    if ((0xEE == pVal->nFrac0) || (0xEE == pVal->nFrac1) || (0xEE == pVal->nSint))
    {
        nFlg = RET_INVLD;
    }
    else if ((0 == pVal->nFrac0) && (0 == pVal->nFrac1) && ((0 == pVal->nSint) || (0x80 == pVal->nSint)))
    {
        nFlg = RET_ERR_VAL_ZERO;
    }
    else
    {
        if ((!IsOneBcd(pVal->nFrac0)) || (!IsOneBcd(pVal->nFrac1)) || (!(IsOneBcd((INT8U)(pVal->nSint & 0x7F)))))
        {
            nFlg = RET_INVLD;
        }
        else
        {
            nFlg = RET_VLD;
        }
    }
    return nFlg;
}

/********************************************************************************
* 函数名:CheckVldFormat11
* 输　入:pVal    输入数据格式11
* 输　出:
* 返  回:RET_ERR_ERROR => 输入数据无效
*        TRUE          => 数据数据有效但非零
* 功  能:判断输入的数据格式11数值是否有效
*********************************************************************************/
INT8S CheckVldFormat11(const pFORMAT11 pVal)
{
    INT8S nFlg = RET_INVLD;

    if (NULL == pVal)
    {
        return nFlg;
    }
    if ((0xEE == pVal->nD1) || (0xEE == pVal->nD2) || (0xEE == pVal->nD3) || (0xEE == pVal->nD4))
    {
        nFlg = RET_INVLD;
    }
    else if ((0 == pVal->nD1) && (0 == pVal->nD2) && (0 == pVal->nD3) && (0 == pVal->nD4))
    {
        nFlg = RET_ZERO;
    }
    else
    {
        if ((!IsOneBcd(pVal->nD1)) || (!IsOneBcd(pVal->nD2)) || (!IsOneBcd(pVal->nD3)) || (!IsOneBcd(pVal->nD4)))
        {
            nFlg = RET_INVLD;
        }
        else if (IsMaxByte((pVal->nD1 & 0x0F), 9) || IsMaxByte((pVal->nD2 & 0x0F), 9) || IsMaxByte((pVal->nD3 & 0x0F), 9) || IsMaxByte((pVal->nD4 & 0x0F), 9))
        {
            nFlg = RET_INVLD;
        }
        else
        {
            nFlg = RET_VLD;
        }
    }
    return nFlg;
}

/********************************************************************************
* 函数名:CheckVldFormat25
* 输　入:pVal    输入数据格式25
* 输　出:
* 返  回:RET_ERR_ERROR => 输入数据无效
*        RET_ERR_OK    => 输入数据为0
*        TRUE          => 数据数据有效但非零
* 功  能:判断输入的数据格式25数值是否为0
*********************************************************************************/
INT8S CheckVldFormat25(const pFORMAT25 pVal)
{
    INT8S nFlg = RET_INVLD;

    if (NULL == pVal)
    {
        return nFlg;
    }
    if ((0xEE == pVal->nD1) || (0xEE == pVal->nD2) || (0xEE == pVal->nD3))
    {
        nFlg = RET_INVLD;
    }
    else if ((0 == pVal->nD1) && (0 == pVal->nD2) && ((0 == pVal->nD3) || (0x80 == pVal->nD3)))
    {
        nFlg = RET_ERR_VAL_ZERO;
    }
    else
    {
        if ((!IsOneBcd(pVal->nD1)) || (!IsOneBcd(pVal->nD2)) || (!IsOneBcd((pVal->nD3 & 0x7F))))
        {
            nFlg = RET_INVLD;
        }
        else
        {
            nFlg = RET_VLD;
        }
    }
    return nFlg;
}

/********************************************************************************
* 函数名:Format072Float
* 输　入:sSrc    输入数据格式07
* 输　出:
* 返  回:数据格式07转换成实际的浮点格式数值
* 功  能:数据格式07转换为浮点数据
*********************************************************************************/
FP32 Format072Float(pFORMAT07 pSrc)
{
	FP32 	fVal 		= 0;
	INT8U 	nLValue 	= 0;
	INT8U 	nHValue 	= 0;

	if (NULL == pSrc)
    {
        return 0;
    }

	if((!IsOneBcd(pSrc->nFrac)) || (!IsOneBcd(pSrc->nIntg)))
    {
        return 0;
    }
	nLValue = BCD_TO_INT8U(pSrc->nFrac);
	nHValue = BCD_TO_INT8U(pSrc->nIntg);
	fVal = (FP32)((nHValue * 100 + nLValue) / 10.0);
    return fVal;
}

/****************************************************************************
* 函数名：Format092Float
* 输  入：sSrc Format09格式的数据
*
* 输  出：
* 返  回：
* 功  能：将Format09数据转换为float数据
****************************************************************************/
FP32 Format092Float(const pFORMAT09 pSrc)
{
	FP32 	fVal 	= 0;
	INT8U 	lValue 	= 0;
	INT8U 	mValue 	= 0;
	INT8U 	hValue 	= 0;

    if (NULL == pSrc)
    {
        return 0;
    }
    if (RET_VLD != CheckVldFormat09(pSrc))
    {
        return 0;
    }
	lValue = BCD_TO_INT8U(pSrc->nFrac0);
	mValue = BCD_TO_INT8U(pSrc->nFrac1);
	hValue = BCD_TO_INT8U((pSrc->nSint & 0x7F));
	fVal = (FP32)(lValue/10000.0 + mValue/100.0 + hValue);
    return fVal;
}

/********************************************************************************
* 函数名:Format252Float
* 输　入:sSrc    输入数据格式25
* 输　出:
* 返  回:数据格式25转换成实际的浮点格式数值
* 功  能:数据格式25转换为浮点数据
*********************************************************************************/
FP32 Format252Float(pFORMAT25 pSrc)
{
	FP32 fVal = 0;
	INT8U bsSign = (pSrc->nD3 & 0x80);

    if (RET_VLD != CheckVldFormat25(pSrc))
    {
        return 0;
    }
	fVal = (FP32) (BCD_TO_INT8U((INT8U)(pSrc->nD3 & 0x7F)) * 10 + (FP32)BCD_TO_INT8U(pSrc->nD2)/10.0 + (FP32)BCD_TO_INT8U(pSrc->nD1)/1000.0);
	if (bsSign == 0x80)
	{
		fVal = 0 - fVal;
	}
    return fVal;
}

/********************************************************************************
* 函数名:Float2Format05
* 输　入:fSrc    输入浮点数
* 输　出:pDest   输出格式05
* 返  回:实际的浮点格式数值转换成数据格式05
* 功  能:浮点数据转换为数据格式05
*********************************************************************************/
INT8S Float2Format05(FP32 fSrc, pFORMAT05 pDest)
{
	FP32 fVal = 0;
	INT32U nTmpVal = 0;
	INT8U bsSign = 0;

    if (NULL == pDest)
    {
        return RET_ERR_ERROR;
    }

    if (fSrc > 0)
	{
		bsSign = 0;
		fVal = fSrc;
	}
	else
	{
		bsSign = 0x80;
		fVal = 0 - fSrc;
	}
	nTmpVal = (INT32U)(fVal * 10);
	pDest->nFrac = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
	pDest->nSint = bsSign | INT8U_TO_BCD((INT8U)((nTmpVal / 100) % 100));

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:Float2Format09
* 输　入:fSrc    输入浮点数
* 输　出:pDest   输出格式09
* 返  回:实际的浮点格式数值转换成数据格式09
* 功  能:浮点数据转换为数据格式09
*********************************************************************************/
INT8S Float2Format09(FP32 fSrc, pFORMAT09 pDest)
{
	FP32 fVal = 0;
	INT32U nTmpVal = 0;
	INT8U bsSign = 0;

    if (NULL == pDest)
    {
        return RET_ERR_ERROR;
    }

    if (fSrc >= 0)
	{
		bsSign = 0;
		fVal = fSrc;
	}
	else
	{
		bsSign = 0x80;
		fVal = 0 - fSrc;
	}
	nTmpVal = (INT32U)(fVal * 10000);
	pDest->nFrac0 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
    pDest->nFrac1 = INT8U_TO_BCD((INT8U)((nTmpVal/100) % 100));
	pDest->nSint = bsSign | INT8U_TO_BCD((INT8U)((nTmpVal / 10000) % 100));

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:Float2Format11
* 输　入:fSrc    输入浮点数
* 输　出:pDest   输出格式11
* 返  回:实际的浮点格式数值转换成数据格式11
* 功  能:浮点数据转换为数据格式11
*********************************************************************************/
INT8S Float2Format11(FP32 fSrc, pFORMAT11 pDest)
{
	INT32U nTmpVal = 0;

    if (NULL == pDest)
    {
        return RET_ERR_ERROR;
    }

	nTmpVal = (INT32U)(fSrc * 100);
	pDest->nD1 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
    nTmpVal /= 100;
    pDest->nD2 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
    nTmpVal /= 100;
	pDest->nD3 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
    nTmpVal /= 100;
    pDest->nD4 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:Float2Format13
* 输　入:fSrc    输入浮点数
* 输　出:pDest   输出格式13
* 返  回:实际的浮点格式数值转换成数据格式13
* 功  能:浮点数据转换为数据格式13
*********************************************************************************/
INT8S Float2Format13(FP32 fSrc, pFORMAT13 pDest)
{
	INT32U nTmpVal = 0;

    if (NULL == pDest)
    {
        return RET_ERR_ERROR;
    }

	nTmpVal = (INT32U)(fSrc * 10000);
	pDest->nD1 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
    nTmpVal /= 100;
    pDest->nD2 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
    nTmpVal /= 100;
	pDest->nD3 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));
    nTmpVal /= 100;
    pDest->nD4 = INT8U_TO_BCD((INT8U)(nTmpVal % 100));

    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 将输入的一串字节处理为携带2字节CRC校验码
****************************************************************************/
INT16U CalcBlockCRC16(INT16U nCount, INT16U nCrc, INT8U *pBuffer)
{
#if 1
   	INT8U *p=pBuffer;

   	while (nCount--!=0)
   	{
	  nCrc=(nCrc<<8)^ccitt_16[(nCrc>>8)^*p++];
	}
   	return(nCrc);
#endif
}

/******************************************************************************
* 函数名:ReplaceAscii
* 输　入:pData        源数据
*        nLen         长度
*        nSrc         指定字符
*        nDest        目标字符
* 输　出:
* 返  回:替换字节数
* 功  能: 将指定字符替换为目标字符,直到第一位非指定字符为止
*******************************************************************************/
INT8U ReplaceAscii(INT8U *pData, INT8U nLen, INT8U nSrc, INT8U nDest)
{
    INT8U ii = 0;

    if (NULL == pData)
    {
        return FALSE;
    }
    for (ii = 0; ii < nLen; ++ii)
    {
        if (pData[ii] == nSrc)
        {
            pData[ii] = nDest;
        }
        else
        {
            break;
        }
    }
    return ii;
}

//显示功率
void DrawFormat09(pFORMAT09 pFmat09, INT8U* nDispBuf)
{
    INT8U nStr[sizeof(FORMAT09) * 2 + 1];
    //INT8U nDispBuf[9];
    BOOLEAN bNeg = FALSE;
    FORMAT09 sFmat09;

    memset(&nStr[0], 0, sizeof(nStr));
    memset(&nDispBuf[0], 0, sizeof(nDispBuf));
    sprintf((char*)&nDispBuf[0], " --.----");
    if (NULL != pFmat09)
    {
        if (RET_INVLD != CheckVldFormat09(pFmat09))
        {
            memcpy(&sFmat09, pFmat09,sizeof(FORMAT09));
            bNeg = TestBufBit(&pFmat09->nSint, 1, 7);
            sFmat09.nSint = sFmat09.nSint & 0x7F;
            BcdToAscii(&sFmat09.nFrac0, sizeof(FORMAT09), TRUE, &nStr[0]);
            if (bNeg)
            {
                sprintf((char*)&nDispBuf[0], "-");
            }
            else
            {
                sprintf((char*)&nDispBuf[0], " ");
            }
            memcpy(&nDispBuf[1], &nStr[0], 2);
            ReplaceAscii(&nDispBuf[1], 1, '0', ' ');
            memcpy(&nDispBuf[4], &nStr[2], 4);
        }
    }
}

//显示电流
void DrawFormat25(pFORMAT25 pFmat25, INT8U* nDispBuf)
{
    INT8U nStr[sizeof(FORMAT25) * 2 + 1];
    //INT8U nDispBuf[9];
    BOOLEAN bNeg = FALSE;
    FORMAT25 sFmt25;

    memset(&nStr[0], 0, sizeof(nStr));
    memset(&nDispBuf[0], 0, sizeof(nDispBuf));
    sprintf((char*)&nDispBuf[0], " ---.---");
    if (NULL != pFmat25)
    {
        if (RET_INVLD != CheckVldFormat25(pFmat25))
        {
            memcpy(&sFmt25, pFmat25,sizeof(FORMAT25));
            bNeg = TestBufBit(&pFmat25->nD3, 1, 7);
            sFmt25.nD3 = sFmt25.nD3 & 0x7F;
            BcdToAscii(&sFmt25.nD1, sizeof(FORMAT25), TRUE, &nStr[0]);
            if (bNeg)
            {
                sprintf((char*)&nDispBuf[0], "-");
            }
            else
            {
                sprintf((char*)&nDispBuf[0], " ");
            }
            memcpy(&nDispBuf[1], &nStr[0], 3);
            ReplaceAscii(&nDispBuf[1], 2, '0', ' ');
            memcpy(&nDispBuf[5], &nStr[3], 3);
        }
    }
}

/****************************************************************************
* 函数名：Float2BCD
* 输  入：nSrc 输入的float数据
*
* 输  出：
* 返  回：
* 功  能：把float转化为规约格式
****************************************************************************/
INT32U Float2BCD(float nSrc)
{
	INT8U nTmp[4];
	INT8U nFlag=0;

   	if (nSrc < 0)
   	{
   		nSrc = -nSrc;
   		nFlag = 0x10;
   	}

	if (nSrc > (1e10))							//10^10  科学计数法
	{
		return 0;
	}

	if (nSrc > 9999999)              			//9e6:9*10^6
	{
		nSrc /= 1000;
		INT32U2BCD((INT32U)nSrc, 7, nTmp);
		nTmp[3] |= 0x40;              			//G=1 Mwh
		nTmp[3] |= nFlag;
		return (*(INT32U*)nTmp);
	}

	INT32U2BCD((INT32U)nSrc, 7, nTmp);
	nTmp[3] |= nFlag;      						//G位或

	return (*(INT32U*)nTmp);
}

/********************************************************************************
* 函数名: Bcd2Dec
* 输　入:
* 输　出:
* 返  回:
* 功  能: Bcd转化Dec
*********************************************************************************/
INT8U Bcd2Dec(INT8U nData)
{
    return (((nData) & 0x0F) + ((nData) >> 4) * 10);
}


/********************************************************************************
* 函数名: Bcd2Dec
* 输　入:
* 输　出:
* 返  回:
* 功  能: Bcd转化Dec批量转换
*********************************************************************************/
void Bcd2DecBuf(INT8U* nData, INT8U   nSize)
{
    INT8U       i = 0;
    for(i = 0; i < nSize; i ++)
    {
        nData[i] = (((nData[i]) & 0x0F) + ((nData[i]) >> 4) * 10);
    }
    return ;
}

/********************************************************************************
* 函数名: Dec2Bcd
* 输　入:
* 输　出:
* 返  回:
* 功  能: Dec转化Bcd
*********************************************************************************/
INT8U Dec2Bcd(INT8U nData)
{
	return ((((nData) / 10) << 4) + (nData) % 10);
}


/********************************************************************************
* 函数名:ReverseOutput
* 输　入:Src Des   nLen
* 输　出: nLen
* 返  回:
* 功  能:得到逆向数组
*********************************************************************************/
INT8S   ReverseOutput(INT8U*  Src, INT8U*   Des, INT8U  nLen)
{
    INT8U   i = 0;
    for(i = 0; i < nLen; i++)
    {
        Des[i] = Src[nLen - i - 1];
    }
    return i;
}

/***************************************************************************
* 函数名:IsAllEqualVal
* 输　入:
*		const INT8U*	pData    数据源
*       INT32U			nLen     需要判断的数据长度
* 输　出:
* 返  回:
*		TRUE     是
*       FALSE    否
* 功  能:判断数据是否全为某无效特征字数据(如0xEE,0xFF,0x00等)
***************************************************************************/
BOOLEAN IsAddrFieldVal(const INT8U* pData, INT32U nLen)
{
    INT32U i = 0;
    INT8U nByte = 0xAA;
    
    if(pData == NULL)
    {
        return FALSE;
    }
	for(i = 0; i < nLen; i++)
    {
        if(pData[i] != nByte)
        {
            if((pData[i]&0xF0) != 0xA0)
            {
                if(pData[i] !=g_nMemAddr[i])
                {
                   return FALSE;
                }
            }
            else if((pData[i]&0xF0) == 0xA0)
            {
                if((pData[i]&0x0F) !=(g_nMemAddr[i]&0x0F))
                {
                    return FALSE;
                }
            }
        }
	}

	return TRUE;
}




#include "Dwin.h"
#include "Linkcom.h"
#include "LinkcomExt.h"
#include "ChkCrc.h"
#include <math.h>

INT8U       g_DwinPageID = 0;
INT8U       g_DwinRDTime = 0;

//����������
/********************************************************************************
* ������:DwinMakeFrm
* �䡡��:	pData		��������
*			nDataLen	���ݳ���
*			pVDxx		������ַ
*			nCmd		ָ��    0x82��0x83
* �䡡��:	pData
* ��  ��:	nSendLen
* ��  ��: ��������֡  ������֡����
*********************************************************************************/
INT32S DwinMakeFrm(INT8U* pData,INT32S nDataLen,INT16U pVDxx,INT8U nCmd)
{
	pDwin_LINK_PROT pDwinLinkProt = (pDwin_LINK_PROT)pData;
	INT32U	nSendLen	=	0;
	INT16U offset = 0,  crc = 0;
	
	if((NULL ==pData)||(nDataLen  < 0)||(NULL == pVDxx))
	{
		return -1;
	}
	
	memmove(&pDwinLinkProt->nData[0],pData,nDataLen);
	pDwinLinkProt->nHead1[0] = 0x5A;
	pDwinLinkProt->nHead1[1] = 0xA5;
	pDwinLinkProt->nDatalen[0] = (INT8U)(nDataLen + 5);
	pDwinLinkProt->nCmd	=	nCmd;
	//memmove(&pDwinLinkProt->nAddr[0],pVDxx,2);
    pDwinLinkProt->nAddr[0] =  (pVDxx>>8)&0Xff;
    pDwinLinkProt->nAddr[1] =  pVDxx&0Xff;
	offset = nDataLen + 3;
	// ����CRC
    crc = crc16(&pDwinLinkProt->nCmd,offset);
	pDwinLinkProt->nData[nDataLen] = crc & 0xFF;
    pDwinLinkProt->nData[nDataLen + 1] = (crc >> 8) & 0xFF;
	nSendLen = nDataLen + 5 + 3;
	
	return nSendLen;
}

/*
*****************************************************************************
* ������: Meter645AsynCheckPackage
* �䡡��: 
*		pLINK_COM pLinkCom
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: ������֡������0�ɹ�������-1
*****************************************************************************
*/
INT32S DwinAsynCheckPackage(void* pPara)
{
	INT16U		nHeadLen		= STR_OFF(pDwin_LINK_PROT, nData);
	pLINK_COM   pLinkCom    	= (pLINK_COM)pPara;
    pBASE_BUF	pBaseBuf		= NULL;
	INT32U		i				= 0;
	INT32U		nDataNum		= 0;
	INT16U		nDataLen		= 0;
	INT8U 		nHHfs			= 0;
	INT8U 		nLHfs			= 0;
	INT8S		nRtn			= -1;
	
	LINK_COM_VALID ;
	
	
	if (BASEBUF_RECV_PTR(0) < BASEBUF_PROC_PTR(0))
	{
		if (0 != FifoMoveLeftBaseBuf(pBaseBuf, BASEBUF_PROC_PTR(0)))
		{
			return -1;
		}
	}
	
	nDataNum = BASEBUF_BYTE_NUM(pBaseBuf);
	
	if(nDataNum < nHeadLen+ 2)
	{
		return -1;
	}
	
	for(i = 0; i < nDataNum - (nHeadLen+ 2 - 1); i++)
	{
		//1.����ͷ
		if((0x5A != BASEBUF_PROC_PBUF(i + 0))&&(0xA5 != BASEBUF_PROC_PBUF(i + 1)))
		{
			continue;
		}
		
		if(BASEBUF_PROC_PBUF(i + 2) <= 5)
		{
			pLinkCom->sBaseBuf.nRecvPtr = 0;
			pLinkCom->sBaseBuf.nProcPtr = 0;
			pLinkCom->nFrmLen = 0;
			return -1;
		}
		
		nDataLen = BASEBUF_PROC_PBUF(i + 2);
		
		if (i + 3 + nDataLen  > nDataNum)
		{
			pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt++;
			if (pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt > MAX_RCV_CONTINUE_NUM)
			{
				pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt = 0;
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt = 0;    
		}
		//֡βУ�� 
       	getDwinCrcProc(&BASEBUF_PROC_PBUF(i+3), &nLHfs, &nHHfs, nDataLen-2);
		
		if (nLHfs != BASEBUF_PROC_PBUF(i + nDataLen + 1))
		{
			pLinkCom->sBaseBuf.nRecvPtr = 0;
			pLinkCom->sBaseBuf.nProcPtr = 0;
			pLinkCom->nFrmLen = 0;
			return -1;
		}
		
		if (nHHfs != BASEBUF_PROC_PBUF(i + nDataLen + 2))
		{
			pLinkCom->sBaseBuf.nRecvPtr = 0;
			pLinkCom->sBaseBuf.nProcPtr = 0;
			pLinkCom->nFrmLen = 0;
			return -1;
		}
		
		pLinkCom->nFrmLen = nDataLen + 3;
		pLinkCom->sFrmBuf.pBuf = &pBaseBuf->pBuf[BASEBUF_PROC_PTR(i + 0)];
		nRtn = 0;
		break;
	}
	
	pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(i + 0);
	return nRtn;
}

/*
*****************************************************************************
* ������: DwinAsynProc
* �䡡��: 
*		pLINK_COM pLinkCom
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: ������֡������0�ɹ�������-1
*****************************************************************************
*/
INT32S DwinAsynProc(void* pPara)
{
	pDwin_LINK_PROT	pUpRcvLinkProt				= NULL;
	pLINK_COM  	 	pLinkCom                    = (pLINK_COM)pPara;
	pBASE_BUF		pBaseBuf					= NULL;
	INT8S 			nRet  						= 0;
    INT32S			nSendLen					= 0;
    INT8U           nDataBuf[300]                ={0};
    
	LINK_COM_VALID ;

	if (0 == pLinkCom->nFrmLen)
	{
		return 0;
	}
    
    nSendLen = DwinAnalyse(&pLinkCom->sFrmBuf.pBuf[0],pLinkCom->nFrmLen,&nDataBuf[0]);
    
  //END:
	// ����������崦��
	LINK_COM_FRAME_CLR ;
	
	return nRet;
}

/*
*****************************************************************************
* ������: DwinAsynProc
* �䡡��: 
*		pLINK_COM pLinkCom
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: ������֡������0�ɹ�������-1
*****************************************************************************
*/

INT8U DwinAnalyse(const INT8U *pValidData,INT32U nValidLen,INT8U *pSendData)
{
	INT8U 	nCtrlCode 				= 0;
	INT8U	nSendLen 				= 0;
	
	nCtrlCode = pValidData[3];
	
	switch(nCtrlCode)
	{
		case 0x80: 							//���ʼĴ���	д������
			break;
		case 0x81:							//��ȡ����
			break;
		case 0x82:							//���ʱ����洢����RAM�� д������
			break;
		case 0x83:							//��ȡ����
			nSendLen = DwinCode83(pValidData, nValidLen, pSendData);
			break;
			
		default:
			nSendLen = 0;
		break;	
	}
	
	
	return nSendLen;
}


// ����IEEE 754���룬���ʮ����floatֵ
float ieee754_to_decimal(INT32U bits) {
    int S = (bits >> 31) & 0x1;
    int E = (bits >> 23) & 0xFF;
    INT32U M = bits & 0x7FFFFF;

    if (E == 255) { // �������
        if (M == 0) return S ? -INFINITY : INFINITY;
        else return NAN;
    }
    if (E == 0) { // �ǹ����
        return (S ? -1 : 1) * (M / (float)(1 << 23)) * powf(2, -126);
    }
    // �����
    return (S ? -1 : 1) * (1 + M / (float)(1 << 23)) * powf(2, E - 127);
}

// ����IEEE 754 double���룬���ʮ����doubleֵ
double ieee754_double_to_decimal(uint64_t bits) {
    int S = (bits >> 63) & 0x1;
    int E = (bits >> 52) & 0x7FF;
    uint64_t M = bits & 0xFFFFFFFFFFFFFull;

    if (E == 2047) { // �������
        if (M == 0) return S ? -INFINITY : INFINITY;
        else return NAN;
    }
    if (E == 0) { // �ǹ����
        return (S ? -1 : 1) * (M / pow(2.0, 52)) * pow(2.0, -1022);
    }
    // �����
    return (S ? -1 : 1) * (1 + M / pow(2.0, 52)) * pow(2.0, E - 1023);
}
    
/*
*****************************************************************************
* ������: DwinAsynProc
* �䡡��: 
*		pLINK_COM pLinkCom
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: ������֡������0�ɹ�������-1
*****************************************************************************
*/
INT8S DwinCode83(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
	//INT8U 	nPageID 				= 0;
	INT16U 	nVdxx 					= 0;
	INT8S   nRet					= 0;
    INT32U  nValue                  = 0;
    INT8U   nData[30]               = {0x5a,0xa5,0x06,0x83,0x15,0xa0,0x03,0x00,0x65 };
    pSET_MEM_INFO nSET_MEM_INFO     ={0};
	
	nVdxx = ((pValidBuf[4]<<8)|pValidBuf[5]);
    memset(nSET_MEM_INFO,0,sizeof(nSET_MEM_INFO));
    
    MemReadAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
    //MemWriteAcXFlhBackData(INT32U nFn, void *pBuf, INT32U nLen)
	
	switch(nVdxx)
	{
		case 0x009C:										//����ʱ��
            if((pValidBuf[7] == 0x5A)&&(pValidBuf[8] == 0xA5))
            {
                //������ȡ����ʱ���־
                g_DwinRDTime = 1;
                DwinProc(COMM_232, nData, 9, 30);  //�����ص�ʱ�䱣��
            }
			break;
		case 0x5000:										//��ҳ��
			g_DwinPageID = ((pValidBuf[7]<<8)|pValidBuf[8]);
			nRet = 0;
			break;
		case 0x1270:										//��������
            nValue = ((pValidBuf[7]<<8)|pValidBuf[8]); 
            nSET_MEM_INFO->nBJDL = nValue;
            MemWriteAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
			break;
		case 0x12A0:										//�����¶� �Ե������洢����Ϊ׼��������Ĭ�ϵ��籣���˲������ڵ������޸Ĳ���ʱ�Զ��ϴ����ݲ�����
            nValue = ((pValidBuf[7]<<8)|pValidBuf[8]); 
            nSET_MEM_INFO->nBJWD = nValue;
            MemWriteAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
			break;
		case 0x12D0:										//��������
            nValue = ((pValidBuf[7]<<8)|pValidBuf[8]); 
            nSET_MEM_INFO->nBJWS = nValue;
            MemWriteAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
			break;
		case 0x1300:										//�жϵ���
            nValue = ((pValidBuf[7]<<8)|pValidBuf[8]); 
            nSET_MEM_INFO->nQDDL = nValue;
            MemWriteAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
			break;
		case 0x1330:										//�ж��¶�
            nValue = ((pValidBuf[7]<<8)|pValidBuf[8]); 
            nSET_MEM_INFO->nQDWD = nValue;
            MemWriteAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
			break;                                          
		case 0x1360:										//�ж�����
            nValue = ((pValidBuf[7]<<8)|pValidBuf[8]); 
            nSET_MEM_INFO->nQDWS = nValue;
            MemWriteAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
			break;                                          
		case 0x1390:                                        //��⿪��
            nValue = pValidBuf[8]; 
            nSET_MEM_INFO->nJCKG = nValue;
            MemWriteAcXFlhBackData(F113, &nSET_MEM_INFO, sizeof(nSET_MEM_INFO));
			break;
		case 0x5030:										//�洢
			break;
	}
	
	return nRet;
}


INT32S DwinProc(INT8U nCom, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen)
{
    INT8U			nCnt			= 0;
    INT8U           nLHfs           = 0;
    INT8U           nHHfs           = 0;
	INT32S			nTmp			= 0;
	INT32S			nTimeOut		= 2000;
	pDwin_LINK_PROT   pDbLinkProt     = (pDwin_LINK_PROT)pData;
    
    if ((NULL == pData) || (nDatalen < 0) || (nMaxDatalen <= 0))
	{
		return -1;
	}

	nCom = ProcCommCfgPend(nCom);
	WriteComm((INT8U)nCom, pData, nDatalen);

	memset(pData, 0, nMaxDatalen);
	nDatalen = 0;
    
    while(1)
	{
		if (nTimeOut < 0)
		{
			nTmp = -1;
			break;
		}
		nTmp = 0;
        nCnt++;
       
		nTmp = ReadCommMs(nCom, &pData[nDatalen], nMaxDatalen - nDatalen, 50, 200);
		if (nTmp > 0)
		{
			nDatalen += nTmp;
		}
		nTimeOut -= 100;

		if (nDatalen >= 7)
		{
			nTmp = DwinRecvCheckPackage(NULL,pData, nDatalen);
			if (nTmp > 0)
			{
				nDatalen = nTmp;
				break;
			}
		}
	}
	ProcCommCfgPost(nCom);
	return nTmp;
}



INT32S DwinRecvCheckPackage(INT16U nAddr, INT8U* pBuf, INT16U nBufsize)
{
	INT16U		nHeadLen		= STR_OFF(pDwin_LINK_PROT, nData);
	INT16U 		i        		= 0;
    INT8U* 		pData    		= pBuf;
    INT8U       nLHfs           = 0;
    INT8U       nHHfs           = 0;
	INT16U		nDataLen		= 0;
	
    if ((NULL == pBuf))
    {
        return -1;
    }
    if (nBufsize < nHeadLen + 2)
    {
        return -1;
    }

    for (i = 0; i < nBufsize - (nHeadLen + 2 - 1); i++)
    {
		pData = &pBuf[i];							// 
		// 1. ����ͷ
		if ((pData[0] != 0x5A)&&(pData[1] != 0xA5))
        {
            continue;
        }
		/* 2. 6���ֽڵ�ַ
		if (NULL != pAddr)
		{
	        if (0 != memcmp(pAddr, &pData[1], 6))
	        {
	            continue;
	        }
		}*/
		// 4. ������

		// 5. �����򳤶�
        nDataLen = pData[2];
        if (nDataLen > nBufsize - 2 - 2 - i)
        {
            continue;
        }

		// 6. CS
        getDwinCrcProc(&pData[i+ 3], &nLHfs, &nHHfs, nBufsize- 5 - i);
 
        
        if ((nLHfs != pData[nDataLen + 5-2])||(nHHfs != pData[nDataLen + 5-1]))
        {
            continue;
        }

		nDataLen = nDataLen + 2 + 2;
		memmove(&pBuf[0], &pData[0], nDataLen);
        return (INT32S)nDataLen;
    }

    return -1;
}











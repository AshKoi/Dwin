#include "Dwin.h"
#include "Linkcom.h"
#include "LinkcomExt.h"
#include "ChkCrc.h"


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
INT32S DwinMakeFrm(INT8U* pData,INT32S nDataLen,INT8U* pVDxx,INT8U nCmd)
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
	memcpy(&pDwinLinkProt->nAddr[0],pVDxx,2);
	offset = nDataLen + 3;
	// ����CRC
    crc = crc16(&pDwinLinkProt->nCmd,offset);
	pDwinLinkProt->nData[offset++] = crc & 0xFF;
    pDwinLinkProt->nData[offset++] = (crc >> 8) & 0xFF;
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
		
		if (i + nHeadLen + nDataLen  > nDataNum)
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
		//֡βУ�� У�麯������д
       	getCrcProc(&BASEBUF_PROC_PBUF(i+3), &nLHfs, &nHHfs, nDataLen-2);
		
		if (nLHfs != BASEBUF_PROC_PBUF(i + nDataLen-1))
		{
			pLinkCom->sBaseBuf.nRecvPtr = 0;
			pLinkCom->sBaseBuf.nProcPtr = 0;
			pLinkCom->nFrmLen = 0;
			return -1;
		}
		
		if (nHHfs != BASEBUF_PROC_PBUF(i + nDataLen))
		{
			pLinkCom->sBaseBuf.nRecvPtr = 0;
			pLinkCom->sBaseBuf.nProcPtr = 0;
			pLinkCom->nFrmLen = 0;
			return -1;
		}
		
		pLinkCom->nFrmLen = nDataLen + 2;
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
    
	LINK_COM_VALID ;

	if (0 == pLinkCom->nFrmLen)
	{
		return 0;
	}
    

    
  END:
	// ����������崦��
	LINK_COM_FRAME_CLR ;
	
	return nRet;
}


















#ifndef	DWIN_H
#define	DWIN_H
#include "base.h"


extern INT8U       g_DwinPageID;

//�������й�Լ֡��Ϣ
typedef struct _tag_Dwin_LINK_PROT
{
	INT8U					nHead1[2];                        	//ͷ     
    INT8U				    nDatalen[1];					    //����
    INT8U					nCmd;                        		//ָ��
	INT8U					nAddr[2];						   	//��ַ    
	INT8U				    nData[1];							//��������
}Dwin_LINK_PROT, *pDwin_LINK_PROT;

	
	
	
INT32S DwinMakeFrm(INT8U* pData,INT32S nDataLen,INT8U* pVDxx,INT8U nCmd);
INT32S DwinAsynCheckPackage(void* pPara);
INT32S DwinAsynProc(void* pPara);


INT8S DwinCode83(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
#endif
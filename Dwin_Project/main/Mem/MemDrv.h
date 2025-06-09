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
#ifndef _MEM_DRV_H_
#define _MEM_DRV_H_

#include "Os_cpu.h"

/**************************************************************************************************
                                          ��غ궨��
***************************************************************************************************/
//�洢����
#define     MEM_MEDIUM_RAM					0					//RAM�洢
#define     MEM_MEDIUM_FLASH				1					//FLASH�洢
#define     MEM_MEDIUM_EEROM				2					//EEPROM�洢
#define     MEM_MEDIUM_FRAM				    3					//FRAM�洢
#define     MEM_MEDIUM_INVLD				0xFF				//��Ч�洢
		


//������Ŀ
#define 	MEM_MAX_BACK_NUM				3					//��������

#define     ITEM_COL_END                    0xFFFF


//MEM_OFF_INFO����ʱʹ�� 
#define MEM_OFF_INFO_DEFINE(nFn, pStr, nUnit)			{nFn, 1, STR_OFF(pStr, nUnit), 0, 0}
#define MEM_OFF_INFO_DEFINE_CHK(nFn, pStr, nUnit)		{nFn, 1, STR_OFF(pStr, nUnit), 0, 1}  

#define MEM_OFF_INFO_DEFINE_EXT(nFn, nNum, pStr, nUnit)			{nFn, nNum, STR_OFF(pStr, nUnit), 0, 0}

/**************************************************************************************************
                                          ������ݽṹ����
***************************************************************************************************/
//�洢ƫ����Ϣ:�������ݿ��ʹ��
typedef struct _tagMEM_OFF_INFO
{
    INT32U      nItmTblId;              	//���ʶ��
    INT32U    	nItmTblNum;			        //������Ŀ
    INT32U      nAddr;						//��ַ
    INT32S      nLen;						//����
    BOOLEAN		bChk;						//�Ƿ�У��	
}MEM_OFF_INFO, *pMEM_OFF_INFO;




//�洢��ַ��Ϣ�ӽṹ
typedef struct _tagMEM_BASE_INFO_SUB
{
    INT8U				nMedium;			//����
	INT32U				nAddr;				//��ַ
	const char* 		pFileName;			//�ļ���
}MEM_BASE_INFO_SUB, *pMEM_BASE_INFO_SUB;
//�洢������Ϣ
typedef struct _tagMEM_INFO
{
    INT32U				nTblId;				//���ݿ���
	INT32U				nRowMaxNum;
	pMEM_OFF_INFO		pCol;
	
    MEM_BASE_INFO_SUB	sMemBaseInfoSub[MEM_MAX_BACK_NUM];
}MEM_INFO, *pMEM_INFO;




//�洢��λ��Ϣ
typedef struct _tagMEM_LOC_INFO
{
	INT8U			nMedium;
	char* 			pFileName;				//�ļ���
	
	INT32U			nAddr;
	INT32U			nLen;

}MEM_LOC_INFO, *pMEM_LOC_INFO;





/**************************************************************************************************
                                          ����ӿں���
***************************************************************************************************/
/*
***************************************************************************
* ������:MemReadDbf
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���:��ͬ�ı��еĴ�0���еĴ�1�������ȵ�
*       INT32U 			nItmTblId								������ʶ��
*       INT32U          nItmId                                  ĳһ��ĵڼ���:��0��ʼ
*       INT32U 			nOffset									��׼ƫ��
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:���ӿ�
***************************************************************************
*/
INT8S MemReadDbf(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nItmId, INT32U nOffset, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* ������:MemWriteDbf
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���
*       INT32U 			nItmTblId								������ʶ��
*       INT32U          nItmId                                  ĳһ��ĵڼ���
*       INT32U 			nOffset									��׼ƫ��
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:д�ӿ�
***************************************************************************
*/
INT8S MemWriteDbf(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nItmId, INT32U nOffset, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, const void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* ������:MemReadDbfExt
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���:��ͬ�ı��еĴ�0���еĴ�1�������ȵ�
*       INT32U 			nItmTblId								������ʶ��
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:���ӿ�
***************************************************************************
*/
INT32S 	MemReadDbfExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* ������:MemWriteDbfExt
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���
*       INT32U 			nItmTblId								������ʶ��
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:д�ӿ�
***************************************************************************
*/
INT32S 	MemWriteDbfExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, const void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* ������:MemReadHistDataExt
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���:��ͬ�ı��еĴ�0���еĴ�1�������ȵ�
*       INT32U 			nItmTblId								������ʶ��
*       INT32U          nItmId                                  ĳһ��ĵڼ���:��0��ʼ
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:���ӿ�
***************************************************************************
*/
INT32S 	MemReadHistDataExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* ������:MemWriteHistDataExt
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���
*       INT32U 			nItmTblId								������ʶ��
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:д�ӿ�
***************************************************************************
*/
INT32S 	MemWriteHistDataExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen);



#endif

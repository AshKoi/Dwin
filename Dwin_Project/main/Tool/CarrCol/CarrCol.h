/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#ifndef _CARRCOL_H
#define _CARRCOL_H

#include "Os_cpu.h"

extern INT8U	g_nCarrModType;
extern INT8U   g_nCarrNodeRef;
extern INT8U   g_nCarrInitFlg;
extern INT8U   g_nCarrZbSelFlg;

/**************************************************************************************************
											 ��غ궨�弰�ṹö�ٶ���
***************************************************************************************************/

enum _tagCARR_MOD_TYPE
{
	CARR_MOD_XC = 1			,				   	// ����
	CARR_MOD_DR 			,				    // ���� 
	CARR_MOD_DX				,				   	// ����   
	CARR_MOD_LHW			,				    // ����΢ 
	CARR_MOD_RSK			,				    // ��˹��
	CARR_MOD_XN				,					// Ѷ��
	CARR_MOD_GY				,					// ��һ����
    CARR_MOD_YXD			,				    //��Ѷ��΢��������
    CARR_MOD_0FDM           ,                   //����ز�
	CARR_MOD_NUM			
};

#define GW3762_DOWN_FRM					0			// ����
#define GW3762_UP_FRM					1			// ����

#define GW3762_START_PRM				1			// ����
#define GW3762_SON_PRM					0			// �Ӷ�

#define GW3762_CARR						1			// �ز�
#define GW3762_WLAN					    0			// С����

#define GW3762_MAIN_SELF				0			// ���ڵ�
#define GW3762_METER_NODE				1			// congjiedian



#define TYPE_CARR                       0
#define TYPE_ZB                         1


/**************************************************************************************************
                                           �ṹ����
***************************************************************************************************/
typedef struct _tag_CARR_MODE_INFO{
	INT8U				 nModeType;
    INT8U                nReadType;						//����ģʽ��0x55��ʾ������������0xaa��ʾģ��������
    INT8U                nSetNodeRouter;				//�Ƿ��·��ڵ㵽·��ģ�飨0x55�·� 0xaa���·���
    INT8U                nSetColNode;					//�·��ɼ�����ַ�����ַ��0x55Ϊ�ɼ�����ַ 0xaaΪ����ַ��
	INT8U                nTranAfn;						//����ʹ�õ�AFN
	INT32U				 nBaud;							//������
}CARR_MODE_INFO, *pCARR_MODE_INFO;



/**************************************************************************************************
                                             ��������
***************************************************************************************************/







/**************************************************************************************************
                                             ��������
***************************************************************************************************/
INT32S  CarrPointRd(INT8U* pData, INT32S nLen);
INT32S CarrColInit(void);

INT32S WrilessInit(void);
INT32S CarrRouteAdd(INT8U* pAddr, INT8U nProt);
INT32S CarrRouteInit(void);


#endif


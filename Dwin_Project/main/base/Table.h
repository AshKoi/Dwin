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
#ifndef _TABLE_H_
#define _TABLE_H_
#include "Os_cpu.h"
#include "config.h"
#include "AppCfg.h"
#include "Macro.h"
#include "RX8025T.h"

/**************************************************************************************************
                                          �������ݽṹ����I
***************************************************************************************************/
//FORMAT01
typedef struct _tagFORMAT01
{
    INT8U        nSec;                                   //��
    INT8U        nMin;								    //��
    INT8U        nHour;									//ʱ
    INT8U        nDay;									//��
    INT8U        nMon;									//����-��
    INT8U        nYear;									//��
}FORMAT01, *pFORMAT01;

//FORMAT02
typedef struct _tagFORMAT02
{
    INT8U       nVal[2];        //��λ,ʮλ,��λ������λ
}FORMAT02, *pFORMAT02;

//FORMAT03
typedef struct _tagFORMAT03
{
    INT8U       nVal[4];        //��λ-����λ������λ
}FORMAT03, *pFORMAT03;

//FORMAT04
typedef struct _tagFORMAT04
{
    INT8U       nVal;           //���λ-�ϸ�
}FORMAT04, *pFORMAT04;

//FORMAT05
typedef struct _tagFORMAT05
{
    INT8U       nFrac;           //��λ��ʮ��λ
    INT8U       nSint;           //ʮλ�Ͱ�λ,���ֵΪ799.9  s��ʾ.7λ��s = 0��ʾ��ֵ;s = 1��ʾ��ֵ
}FORMAT05, *pFORMAT05;

//FORMAT06
typedef struct _tagFORMAT06
{
    INT8U       nFrac;           //�ٷ�λ��ʮ��λ
    INT8U       nSint;           //��λ��ʮλ,���ֵΪ79.99   s��ʾ.7λ��s = 0��ʾ��ֵ;s = 1��ʾ��ֵ
}FORMAT06, *pFORMAT06;

//FORMAT07
typedef struct _tagFORMAT07
{
    INT8U       nFrac;           //��λ��ʮ��λ
    INT8U       nIntg;           //ʮλ�Ͱ�λ
}FORMAT07, *pFORMAT07;

//FORMAT08
typedef struct _tagFORMAT08
{
    INT8U       nD1;             //��λ��ʮλ
    INT8U       nD2;             //��λ��ǧλ
}FORMAT08, *pFORMAT08;

//FORMAT09
typedef struct _tagFORMAT09
{
    INT8U       nFrac0;          //���λ��ǧ��λ
    INT8U       nFrac1;          //�ٷ�λ��ʮ��λ
    INT8U       nSint;           //��λ��ʮλ��s��
}FORMAT09, *pFORMAT09;

//FORMAT10
typedef struct _tagFORMAT10
{
    INT8U       nD1;              //��λ��ʮλ
    INT8U       nD2;              //��λ��ǧλ
    INT8U       nD3;              //��λ��ʮ��λ
}FORMAT10, *pFORMAT10;

//FORMAT11
typedef struct _tagFORMAT11
{
    INT8U        nD1;             //�ٷ�λ��ʮ��λ
    INT8U        nD2;             //��λ��ʮλ
    INT8U        nD3;             //��λ��ǧλ
    INT8U        nD4;             //��λ��ʮ��λ
}FORMAT11, *pFORMAT11;

//FORMAT12
typedef struct _tagFORMAT12
{
    INT8U        nD1;             //��λ��ʮλ
    INT8U        nD2;             //��λ��ǧλ
    INT8U        nD3;             //��λ��ʮ��λ
    INT8U        nD4;             //����λ��ǧ��λ
    INT8U        nD5;             //��λ��ʮ��λ
    INT8U        nD6;             //����λ��ǧ��λ
}FORMAT12, *pFORMAT12;

//FORMAT13
typedef struct _tagFORMAT13
{
    INT8U        nD1;             //���λ��ǧ��λ
    INT8U        nD2;             //�ٷ�λ��ʮ��λ
    INT8U        nD3;             //��λ��ʮλ
    INT8U        nD4;             //��λ��ǧλ
}FORMAT13, *pFORMAT13;

//FORMAT14
typedef struct _tagFORMAT14
{
    INT8U        nD1;             //���λ��ǧ��λ
    INT8U        nD2;             //�ٷ�λ��ʮ��λ
    INT8U        nD3;             //��λ��ʮλ
    INT8U        nD4;             //��λ��ǧλ
    INT8U        nD5;             //��λ��ʮ��λ
}FORMAT14, *pFORMAT14;

//FORMAT15
typedef struct _tagFORMAT15
{
    INT8U        nMin;			  //��
    INT8U        nHour;			  //ʱ
    INT8U        nDay;			  //��
    INT8U        nMon;			  //��
    INT8U        nYear;			  //��
}FORMAT15, *pFORMAT15;

//FORMAT17
typedef struct _tagFORMAT17
{
    INT8U        nMin;								    //��
    INT8U        nHour;									//ʱ
    INT8U        nDay;									//��
    INT8U        nMon;									//��
}FORMAT17, *pFORMAT17;

//FORMAT18
typedef struct _tagFORMAT18
{
    INT8U       nMin;           //��
    INT8U       nHour;          //ʱ
    INT8U       nDay;           //��
}FORMAT18, *pFORMAT18;

//FORMAT19
typedef struct _tagFORMAT19
{
    INT8U       nMin;           //��
    INT8U       nHour;          //ʱ
}FORMAT19, *pFORMAT19;

//FORMAT20
typedef struct _tagFORMAT20
{
    INT8U       nDay;           //��
    INT8U       nMon;           //��
    INT8U       nYear;          //��
}FORMAT20, *pFORMAT20;

//FORMAT21
typedef struct _tagFORMAT21
{
    INT8U       nMon;           //��
    INT8U       nYear;          //��
}FORMAT21, *pFORMAT21;
//FORMAT22
typedef struct _tagFORMAT22
{
    INT8U       nD1;            //��λ��ʮ��λ
}FORMAT22, *pFORMAT22;

//FORMAT23
typedef struct _tagFORMAT23
{
    INT8U       nFrac0;         //���λ��ǧ��λ
    INT8U       nFrac1;         //�ٷ�λ��ʮ��λ
    INT8U       nIntg;          //��λ��ʮλ
}FORMAT23, *pFORMAT23;

//FORMAT24
typedef struct _tagFORMAT24
{
    INT8U       nHour;          //ʱ
    INT8U       nDay;           //��
}FORMAT24, *pFORMAT24;

//FORMAT25
typedef struct _tagFORMAT25
{
    INT8U       nD1;            //ǧ��λ���ٷ�λ
    INT8U       nD2;            //ʮ��λ����λ
    INT8U       nD3;            //ʮλ����λS
}FORMAT25, *pFORMAT25;

//FORMAT26
typedef struct _tagFORMAT26
{
    INT8U       nD1;            //ǧ��λ���ٷ�λ
    INT8U       nD2;            //ʮ��λ����λ
}FORMAT26, *pFORMAT26;

//FORMAT27
typedef struct _tagFORMAT27
{
    INT8U       nD1;            //��λ��ʮλ
    INT8U       nD2;            //��λ��ǧλ
    INT8U       nD3;            //��λ��ʮ��λ
    INT8U       nD4;            //����λ��ǧ��λ
}FORMAT27, *pFORMAT27;

//FORMAT40(������ʽ)
typedef struct _tagFORMAT40
{
    INT8U       nDay;           //��
    INT8U       nMon;           //��
}FORMAT40, *pFORMAT40;

//FORMAT41
typedef struct _tagFORMAT41
{
    INT8U        nD1;           //�ٷ�λ��ʮ��λ
    INT8U        nD2;           //��λ��ʮλ
    INT8U        nD3;           //��λ��ǧλ
}FORMAT41, *pFORMAT41;

//FORMAT42
typedef struct _tagFORMAT42
{
    INT8U        nD1;           //�ٷ�λ��ʮ��λ
    INT8U        nD2;           //��λ��ʮλ
}FORMAT42, *pFORMAT42;

//FORMAT43
typedef struct _tagFORMAT43
{
    INT8U        nD1;           //�ٷ�λ��ʮ��λ
    INT8U        nD2;           //��λ��ʮλ
    INT8U        nD3;           //��λ������λ
}FORMAT43, *pFORMAT43;

//FORMAT44
typedef struct _tagFORMAT44
{
    INT8U        nD1;           //�ٷ�λ��ǧ��λ
    INT8U        nD2;           //��λ��ʮ��λ
    INT8U        nD3;           //ʮλ������λ
}FORMAT44, *pFORMAT44;

//FORMAT45
typedef struct _tagFORMAT45
{
    INT8U       nD1;            //���λ��ʮ���λ
    INT8U       nD2;            //�ٷ�λ��ǧ��λ
    INT8U       nD3;            //��λ.ʮ��λ
}FORMAT45, *pFORMAT45;

//FORMAT46
typedef struct _tagFORMAT46
{
    INT8U       nD1;            //ʮ��λ����λ
    INT8U       nD2;            //ʮλ����λ
    INT8U       nD3;            //ǧλ����λ
    INT8U       nD4;            //ʮ��λ������λ
}FORMAT46, *pFORMAT46;

//FORMAT47
typedef struct _tagFORMAT47
{
    INT8U        nD1;           //��λ��ʮλ
    INT8U        nD2;           //��λ��ǧλ
    INT8U        nD3;           //��λ��ʮ��λ
    INT8U        nD4;           //����λ��ǧ��λ
    INT8U        nD5;           //��λ��ʮ��λ
}FORMAT47, *pFORMAT47;

//FORMAT48
typedef struct _tagFORMAT48
{
    INT8U       nD1;            //ʮ��λ����λ
    INT8U       nD2;            //ʮλ����λ
    INT8U       nD3;            //ǧλ����λ
    INT8U       nD4;            //ʮ��λ������λ
    INT8U       nD5;            //ǧ��λ,����λ
}FORMAT48, *pFORMAT48;

//FORMAT49
typedef struct _tagFORMAT49
{
    INT8U        nSec;			//��
    INT8U        nMin;			//��
    INT8U        nHour;			//ʱ
    INT8U        nDay;			//��
    INT8U        nMon;			//��
    INT8U        nYear;			//��
}FORMAT49, *pFORMAT49;

//FORMAT50
typedef struct _tagFORMAT50
{
    INT8U        nD1;           //�ٷ�λ��ʮ��λ
    INT8U        nD2;           //��λ��ʮλ
    INT8U        nS3;           //����
}FORMAT50, *pFORMAT50;

//FORMAT51
typedef struct _tagFORMAT51
{
    INT8U        nD1;           //ʮλ����λ
    INT8U        nD2;           //��λ��ǧλ(s)
}FORMAT51, *pFORMAT51;

//FORMAT52
typedef struct _tagFORMAT52
{
    INT8U       nD1;            //�ٷ�λ��ʮ��λ
    INT8U       nD2;            //��λ��ʮλ
    INT8U       nD3;            //��λ��ǧλ
    INT8U       nD4;            //��λ��ʮ��λ
    INT8U       nS5;            //����λ
}FORMAT52, *pFORMAT52;

//MM_DD_NN
typedef struct _tagMM_DD_NN
{
    INT8U	nNo;
    INT8U       nDay;
    INT8U       nMon;
}MM_DD_NN,*pMM_DD_NN;

//hh_mm_NN
typedef struct _taghh_mm_NN
{
    INT8U	nNo;
    INT8U       nMin;
    INT8U       nHour;
}hh_mm_NN,*phh_mm_NN;

//YY_MM_DD_NN
typedef struct _tagYY_MM_DD_NN
{
    INT8U	nNo;
    INT8U       nDay;
    INT8U       nMon;
    INT8U	nYear;
}YY_MM_DD_NN, *pYY_MM_DD_NN;

//HH_MM_SS
typedef struct _tagHH_MM_SS
{
    INT8U        nSec;			//��
    INT8U        nMin;			//��
    INT8U        nHour;			//ʱ
}HH_MM_SS, *pHH_MM_SS;

//YY_MM_DD_WW
typedef struct _tagYY_MM_DD_WW
{
    INT8U	    nWeek;
    INT8U       nDay;
    INT8U       nMon;
    INT8U	    nYear;
}YY_MM_DD_WW, *pYY_MM_DD_WW;


//F25
typedef struct _tagF25DATA
{
	INT8U					nTime[5];
	FORMAT09				sPpower[4];
	FORMAT09				sNpower[4];
	FORMAT05				sFact[4];
	FORMAT07				sVolt[3];
	FORMAT25				sCurr[4];
	FORMAT09				sPower[4];
}F25DATA, *pF25DATA;

//�����¼�״̬
typedef	struct _tagAC_EVENT_STAT
{
	INT8U			nLVolFlag[MAX_PHASE_NUM];					//ʧѹ״̬
    INT8U			nOVolFlag[MAX_PHASE_NUM];					//��ѹ״̬
	INT8U			nLPhaseFlag[MAX_PHASE_NUM];					//����״̬
	INT8U			nLCurrFlag[MAX_PHASE_NUM];					//ʧ��״̬
    INT8U			nOCurrFlag[MAX_PHASE_NUM];					//����״̬
    INT8U			nOloadFlag[MAX_PHASE_NUM];					//����״̬
    INT8U			nReflowFlag[MAX_PHASE_NUM];					//����״̬
	INT8U			nNVolFlag;									//ȫʧѹ״̬
	INT8U			nPFailFlag;									//����״̬
	INT8U			nVoltPErrFlag;								//��ѹ������״̬
	INT8U			nSQNum;										//��ǰʱ����
	INT8U			nSDNum;										//��ǰʱ�α�
    INT8U			nUVolFlag[MAX_PHASE_NUM];					//Ƿѹ״̬
    INT8U			nFCurrFlag[MAX_PHASE_NUM];					//����״̬
}AC_EVENT_STAT, *pAC_EVENT_STAT;

//���������־
typedef	struct _tagAC_FRE_STAT
{
	INT8U			nFlag;										//������־
	INT8U			nMasterNo[4];								//��������
}AC_FRE_STAT, *pAC_FRE_STAT;

//���������־
typedef	struct _tagAC_EVECLR_STAT
{
	INT8U			nFlag;										//������־
	INT8U			nMasterNo[4];								//��������
	INT8U			nDataID[4];									//���ݱ�ʶ��
}AC_EVECLR_STAT, *pAC_EVECLR_STAT;

typedef struct _tagMEM_AC_EVT_DATA
{
	INT8U	nData[LEN_AC_EVENT];
}MEM_AC_EVT_DATA, *pMEM_AC_EVT_DATA;


//���ɵ��������建��ṹ
typedef	struct _tagENERGY_DATA
{
	INT16S			nEnergy[MAX_FEE_NUM+1];			//�ܵ��� �����ʵ���
}ENERGY_DATA, *pENERGY_DATA;

//���ɷ�������޵��������建��ṹ
typedef	struct _tagPH_ENERGY_DATA
{
	INT16U			nEnergy[4];						//�����й����� �����й����� �����޹����� �����޹�����/�����޵���
}PH_ENERGY_DATA, *pPH_ENERGY_DATA;

/**************************************************************************************************
                                   		   ACҵ����Ϣ�ṹ
***************************************************************************************************/
//��������Ϣ
typedef struct _tagAC_DAY_MEM_INFO
{
    INT8U		nNext;
    INT16U		nMemNum;
    //FORMAT20	sTime[MAX_AC_DAY_DATA_NUM];
}AC_DAY_MEM_INFO, *pAC_DAY_MEM_INFO;

//�¼��洢��Ϣ
typedef struct _tagAC_EVT_MEM_INFO
{
	INT8U		nNext;
	INT8U		nMemNum;
}AC_EVT_MEM_INFO, *pAC_EVT_MEM_INFO;

//���ò���
typedef struct _tag_SET_MEM_INFO
{
    INT16U      nBJDL;      //��������
    INT16U      nBJWD;      //�����¶�
    INT16U      nBJWS;      //��������
    INT16U      nQDDL;      //�жϵ���
    INT16U      nQDWD;      //�ж��¶�
    INT16U      nQDWS;      //�ж�����
    INT16U      nJCKG;      //��⿪��
}SET_MEM_INFO,*pSET_MEM_INFO;
//�๦�ܱ����ʾֵ
typedef struct _tagMT_DGN_ENERGY_DATA
{
    FORMAT11            sPEnergy[1+MAX_FEE_NUM];        //����й��ܵ���
    FORMAT11            sPPEnergy[1+MAX_FEE_NUM];       //�����й�����ʾֵ
    FORMAT11            sNPEnergy[1+MAX_FEE_NUM];       //�����й�����ʾֵ
    FORMAT11            sPNEnergy[1+MAX_FEE_NUM];       //�����޹�����ʾֵ
    FORMAT11            sNNEnergy[1+MAX_FEE_NUM];       //�����޹�����ʾֵ
    FORMAT11            sPAEnergy[1+MAX_FEE_NUM];       //���������ܵ���
    FORMAT11            sNAEnergy[1+MAX_FEE_NUM];       //���������ܵ���
}MT_DGN_ENERGY_DATA, *pMT_DGN_ENERGY_DATA;

//�๦�ܱ����ʾֵ
typedef struct _tagMT_DGN_C_ENERGY_DATA
{
    FORMAT11            sPPEnergy[1+MAX_FEE_NUM];       //��������й�����ʾֵ
}MT_DGN_C_ENERGY_DATA, *pMT_DGN_C_ENERGY_DATA;

//�๦�ܱ����ʾֵ
typedef struct _tagMT_DGN_Q_ENERGY_DATA
{
    FORMAT11            sQ1Energy[1+MAX_FEE_NUM];       //һ�����޹�����ʾֵ
    FORMAT11            sQ2Energy[1+MAX_FEE_NUM];       //�������޹�����ʾֵ
    FORMAT11            sQ3Energy[1+MAX_FEE_NUM];       //�������޹�����ʾֵ
    FORMAT11            sQ4Energy[1+MAX_FEE_NUM];       //�������޹�����ʾֵ
}MT_DGN_Q_ENERGY_DATA, *pMT_DGN_Q_ENERGY_DATA;

//�๦�ܱ��������ʾֵ
typedef struct _tagMT_DGN_TRI_ENERGY_DATA
{
    FORMAT11            sPPEnergy[MAX_PHASE_NUM];       //�����й�����ʾֵ
    FORMAT11            sNPEnergy[MAX_PHASE_NUM];       //�����й�����ʾֵ
    FORMAT11            sPNEnergy[MAX_PHASE_NUM];       //�����޹�����ʾֵ
    FORMAT11            sNNEnergy[MAX_PHASE_NUM];       //�����޹�����ʾֵ
}MT_DGN_TRI_ENERGY_DATA, *pMT_DGN_TRI_ENERGY_DATA;

//�๦�ܱ��������ʾֵ
typedef struct _tagMT_DGN_TRI_Q_EN_DATA
{
    FORMAT11            sQ1Energy[MAX_PHASE_NUM];       //һ�����޹�����ʾֵ
    FORMAT11            sQ2Energy[MAX_PHASE_NUM];       //�������޹�����ʾֵ
    FORMAT11            sQ3Energy[MAX_PHASE_NUM];       //�������޹�����ʾֵ
    FORMAT11            sQ4Energy[MAX_PHASE_NUM];       //�������޹�����ʾֵ
}MT_DGN_TRI_Q_EN_DATA, *pMT_DGN_TRI_Q_EN_DATA;

//��ѹ����ĵ���ʾֵ
typedef struct _tagLOSER_ENERGY_DATA
{
    FORMAT11            sCUEnergy;       //ͭ���й��ܵ���ʾֵ
    FORMAT11            sFEEnergy;       //�����й��ܵ���ʾֵ
}LOSER_ENERGY_DATA, *pLOSER_ENERGY_DATA;

//���ɱ�ѹ����ĵ���������ṹ
typedef	struct _tagLOSER_ACCU_DATA
{
	FP32	nEnergy[2];						//�ܵ���
}LOSER_ACCU_DATA, *pLOSER_ACCU_DATA;

//�๦�ܱ�����ʾֵ
typedef struct _tagMT_DGN_DEMAND_DATA
{
    FORMAT23    		sPPDemand[1+MAX_FEE_NUM];       //�����й��������
    FORMAT15    		sPPDemandTime[1+MAX_FEE_NUM];   //�����й������������ʱ��
    FORMAT23    		sNPDemand[1+MAX_FEE_NUM];       //�����й��������
    FORMAT15    		sNPDemandTime[1+MAX_FEE_NUM];   //�����й������������ʱ��
    FORMAT23    		sPNDemand[1+MAX_FEE_NUM];       //�����޹��������
    FORMAT15    		sPNDemandTime[1+MAX_FEE_NUM];   //�����޹������������ʱ��
    FORMAT23    		sNNDemand[1+MAX_FEE_NUM];       //�����޹��������
    FORMAT15    		sNNDemandTime[1+MAX_FEE_NUM];   //�����޹������������ʱ��
}MT_DGN_DEMAND_DATA, *pMT_DGN_DEMAND_DATA;

//�๦�ܱ�����������
typedef struct _tagMT_DGN_Q_DE_DATA
{
    FORMAT23    		sQ1Demand[1+MAX_FEE_NUM];       //�����й��������
    FORMAT15    		sQ1DemandTime[1+MAX_FEE_NUM];   //�����й������������ʱ��
    FORMAT23    		sQ2Demand[1+MAX_FEE_NUM];       //�����й��������
    FORMAT15    		sQ2DemandTime[1+MAX_FEE_NUM];   //�����й������������ʱ��
    FORMAT23    		sQ3Demand[1+MAX_FEE_NUM];       //�����޹��������
    FORMAT15    		sQ3DemandTime[1+MAX_FEE_NUM];   //�����޹������������ʱ��
    FORMAT23    		sQ4Demand[1+MAX_FEE_NUM];       //�����޹��������
    FORMAT15    		sQ4DemandTime[1+MAX_FEE_NUM];   //�����޹������������ʱ��
}MT_DGN_Q_DE_DATA, *pMT_DGN_Q_DE_DATA;

//������ϵ������建��ṹ
typedef	struct _tagCOMB_ENERGY_DATA
{
    INT16S          nCombEpEnergy[MAX_FEE_NUM+1];
}COMB_ENERGY_DATA, *pCOMB_ENERGY_DATA;


/**************************************************************************************************
                                          �������ݽṹ����II
***************************************************************************************************/
//IP�ṹ
typedef struct _tagTCPIP_IP_PORT
{
    INT8U       nIp[4];             				//�ĸ��ֽڵĶ��������IP��ַ
    INT8U       nPort[2];                      //
}TCPIP_IP_PORT, *pTCPIP_IP_PORT;

// ͨ�ſ�����
typedef struct _tagCOM_CFG_SUB
{
	INT8U					nCom;
	INT32U 				    nCommByte;
}COM_CFG_SUB, *pCOM_CFG_SUB;

// ͨ�ſ�����
typedef struct _tagCOM_CFG
{
	INT8U					nCom;
	INT32U 				    nBaudRate;
	INT8U 					nParity;
	INT8U 					nStopBits;
	INT8U 					nCharLen;
}COM_CFG, *pCOM_CFG;

//�ն�����ͨѶ����
typedef struct _tagOBJ_COMM_PRM
{
	INT8U						nObj;
	INT8U						nCom;
	INT32U						nBaud;
	INT8U						nParity;
	
}OBJ_COMM_PRM, *pOBJ_COMM_PRM;


//����Ա����
typedef struct _tagOPERTOR_PRM
{
	INT8U					nOpertor[4];
	INT8U					nOperPsw[3];
}OPERTOR_PRM, *pOPERTOR_PRM;


//�����ṹ
typedef struct _tag_PARAM
{
	//INT8U					nTermAddr[LEN_TERMADDR];                      // 
	INT8U					nAreaRun;
	INT8U					nCarrierCom;
	INT8U					nWorkOrder;
	INT8U					nJcMenu;
	INT8U					nDefaultServ;
	OPERTOR_PRM				sOpertor;
	INT32S					nCurrWorkidAttr;								//��ǰ����������:-1�Ƿ�;0xEC����Ա;����,����Ա

}PARAM, *pPARAM;


//���أ�������
typedef struct _tagDOWNLOAD_STR
{	
	INT32U			nDownSize;
	INT16U			nLastId;			//��һ֡��
	INT16U			nLastLen;			//��һ֡����
	BOOLEAN			bSucc;				//���سɹ���־
}DOWNLOAD_STR, *pDOWNLOAD_STR;


/**************************************************************************************************
                                          �������ݽṹ����III
***************************************************************************************************/
//��������Ϣ
typedef struct _tagDAY_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT20	sTime[MAX_DAY_DATA_NUM];
}DAY_MEM_INFO, *pDAY_MEM_INFO;

//��������Ϣ
typedef struct _tagMON_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT21	sTime[MAX_MON_DATA_NUM];
}MON_MEM_INFO, *pMON_MEM_INFO;

//����������Ϣ
typedef struct _tagCRV_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT20	sTime[MAX_CRV_DATA_NUM];
}CRV_MEM_INFO, *pCRV_MEM_INFO;

//������������Ϣ
typedef struct _tagRDAY_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT20	sTime[MAX_RDAY_DATA_NUM];
}RDAY_MEM_INFO, *pRDAY_MEM_INFO;

//�¼�������Ϣ
typedef struct _tagEVT_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
}EVT_MEM_INFO, *pEVT_MEM_INFO;

typedef struct _tagRC_EVT_DATA
{
    INT8U		nTurn;
    STD_TIME    sTime;
}RC_EVT_DATA, *pRC_EVT_DATA;

typedef struct _tagEVT_TRIP_DATA
{
    INT8U		nEvtTrip;
    FORMAT49    sTime;
}EVT_TRIP_DATA, *pEVT_TRIP_DATA;

typedef struct _tagEVT_GUARPOWER_DATA
{
    INT8U		nGuarPower;
    FORMAT49    sTime;
}EVT_GUARPOWER_DATA, *pEVT_GUARPOWER_DATA;

typedef struct _tagEVT_ALARM_DATA
{
    INT8U		nAlarm;
    FORMAT49    sTime;
}EVT_ALARM_DATA, *pEVT_ALARM_DATA;

typedef  struct  _tag698FZDBFN
{
    INT16U   nDI;
    INT16U   nFn;
}MT_698FZDBFN,*pMT_698FZDBFN;


//��׼�¼��ϱ�״̬
typedef struct _tagMT_REPORTPASS_DATA
{
     //FORMAT46           sOAD;
     INT8U              nSTate;    
}MT_REPORTPASS_DATA, *pMT_REPORTPASS_DATA;

//��׼�¼���¼��
typedef struct _tagMT_REPORT_DATA
{
    INT8U               sOrderNum; //   INT8U    
    FORMAT01            sStartTimestamp;       
    FORMAT01            sFinishTimestamp;       
    //INT8U               sReason;      
    MT_REPORTPASS_DATA  sChannelState;       
    FORMAT11            sPAData[30];
    FORMAT11            sPAFinshData[30]; 
}MT_REPORT_DATA, *pMT_REPORT_DATA;

typedef struct _tagMT_OAD_DATA
{
    FORMAT27            sOAD[64];
}MT_OAD_DATA, *pMT_OAD_DATA;

/**************************************************************************************************
                                          �������ݽṹ����IV
***************************************************************************************************/
//�������洢�ṹ MEM_METER_PRM
typedef struct _tagMEM_METER_PRM
{
    BOOLEAN     bVld;									//��Ч��־
	INT8U		nSeq[2];								//(F10)���������
	INT8U		nMp[2]; 								//(F10)�������
	INT8U		nCom;									//(F10)������˿�
	INT8U		nBaud[4];								//(F10)�����㲨����
	INT8U		nProt;									//(F10)������ͨѶ��Լ
	INT8U		nAddr[LEN_METER_ADDR];					//(F10)������ͨѶ��ַ
	INT8U		nPwd[LEN_METER_PWD];					//(F10)����������
	INT8U		nFeeNum;								//(F10)��������ʸ���
	INT8U		nXsws;									//(F10)������С��λ��
	INT8U		nZsws;									//(F10)����������λ��
	INT8U		nColAddr[LEN_METER_ADDR];				//(F10)������ɼ��ն˵�ַ
	INT8U		nMainType;								//(F10)���������
	INT8U		nPartType;								//(F10)������С��

}MEM_METER_PRM, *pMEM_METER_PRM;

//F25�������� MEM_MT_BASE_PRM
typedef struct _tagMEM_MT_BASE_PRM
{
    INT8U				nPt[2];							//(F25)��ѹ����
	INT8U				nCt[2];							//(F25)��������
	INT8U				nStdVolt[2];					//(F25)���ѹ
	INT8U				nStdCurr;						//(F25)�����
	INT8U				nStdWatt[3];					//(F25)�����
	INT8U				nPhaseMod;						//(F25)���߷�ʽ

}MEM_MT_BASE_PRM, *pMEM_MT_BASE_PRM;

//��������洢
typedef struct _tagMEM_PULSE_PRM
{
    BOOLEAN     bVld;								//��Ч��־
    INT8U       nPulsePort;							//(F11)�˿ں�
    INT8U       nPulseMp;						    //(F11)�����������
    INT8U       nPulseAttr;							//(F11)��������
    INT8U       nPulseConst[2];						//(F11)���峣��
}MEM_PULSE_PRM, *pMEM_PULSE_PRM;

typedef struct _tagPULSE_RATE_NUM
{
	INT64U nPulseNo[MAX_PULSE_NUM];
}PULSE_RATE_NUM, *pPULSE_RATE_NUM;

//ģ���������洢
typedef struct _tagANLOG_PRM
{
    INT8U       nAnlogPort;							//(F13)�˿ں�
    INT8U       nAnlogMp;								//(F13)�����������
    INT8U       nAnlogAttr;							//(F13)����
}ANLOG_PRM, *pANLOG_PRM;

//������洢
typedef struct _tagDIFF_PRM
{
    INT8U       nDiffSeq;								//(F15)�˿ں�
    INT8U       nCmpGrpSeq;							//(F15)�Ա��ܼ����
    INT8U       nBaseGrpSeq;							//(F15)�����ܼ����
    INT8U		nDiffAttr;								//�����ĵ�������ʱ�����估�Աȷ�����־
	INT8U		nDiffVal;								//���ƫ��ֵ
	INT8U		nDiffAbsVal[4];						//����ֵ
}DIFF_PRM, *pDIFF_PRM;


//�ն˶˿ڲ����洢�ṹ MEM_OBJ_PRM_COM
typedef struct _tagMEM_COM_PRM
{
    INT8U       nCom;									//�˿ں�
    INT16U		bsRunCtrl;								//(F33)�������п�����
    INT8U		bsReadDay[4];							//(F33)��������
    FORMAT19	sReadTime;								//(F33)����ʱ��
    INT16U      nReadIntrvl;							//(F33)������
    FORMAT18	sBoardTime;								//(F33)�㲥Уʱ��ʱʱ��
    INT8U       nReadPerdNum;							//(F33)����ʱ����
    FORMAT19	sReadPerd[MAX_READ_PERD_NUM][2];		//(F33)����ʱ��
    INT8U       nCtrlWord;								//(F34)ͨ�ſ�����
    INT32U      nBaud;									//(F34)ͨ������
    INT16U      nReReadIntrvl;                          //�������
    BOOLEAN     bComStop;								//(���� F49/F50)����ָ��ͨ�Ŷ˿���ͣ/�ָ�
}MEM_COM_PRM, *pMEM_COM_PRM;

//����ģ��ͨѶ�����洢
typedef struct _tagDOWN_MODL_PRM
{
    INT8U       nCommPort;								//(F34)ͨ�Ŷ˿ں�
    INT8U       nCommCtrl;								//(F34)ͨ�ſ�����
    INT8U       nCommBps[4];							//(F34)ͨ�Ų�����
}DOWN_MODL_PRM, *pDOWN_MODL_PRM;


//��С��Ų�������Ϣ������洢
typedef struct _tagMETER_TYPE_INFO_PRM
{
    INT8U       nInfoNum;								//(F38)��Ϣ����Ŀ
    INT8U       nInfoAttr[31];							//(F38)��Ϣ��Ԫ��־
}METER_TYPE_INFO_PRM, *pMETER_TYPE_INFO_PRM;


//ʱ�οط�������
typedef struct _tagWATT_CTRL_PERIOD_PRM
{
    INT8U       nPeriodBS;								//(F41)ʱ�α�־
    INT8U       nPeriodDef[8][2];						//(F41)ʱ�ζ�ֵ
}WATT_CTRL_PERIOD_PRM, *pWATT_CTRL_PERIOD_PRM;

//�������ܼ������
typedef struct _tagCTRL_GRP_PRM
{
	INT8U		nPeriodBS;								//(F9/F17)ʱ�ο�Ͷ���־ 
	INT8U		nPeriodPrj;							//ʱ�οط���

	INT8U		nFactory;								//(F10/F18)���ݿ�Ͷ���־
	INT8U		nBussiness;							//(F11/F19)Ӫҵ��ͣ��Ͷ���־

	INT8U		nDownCtrl;								//(F12/F20)
	FORMAT01	sDownCtrlCmdSetTime;					//(F12)�¸���
	INT8U		nDownSlipTime;							//����ʱ��
	INT8U		nDownCoff;								//�¸�ϸ��
	INT8U		nDownDelayMin;							//������ʱʱ��
	INT8U		nDownCtrlTimes30Min;					//����ʱ��
	INT8U		nDownAlmMin[4];						//�ִθ澯ʱ��

	INT8U		nMonCtrl;								//(F15/F23)�µ��Ͷ��/���
	INT8U		nBuyCtrl;								//(F16/F24)�����Ͷ��/���
}CTRL_GRP_PRM, *pCTRL_GRP_PRM;

//�������ִβ���
typedef struct _tagCTRL_TURN_PRM
{
	FORMAT01	sCtrlCmdSetTime;						//(F1)ң����բ
    INT8U       nAlmMin;								//�澯��ʱʱ��
    INT8U       nAstPower30Min;						    //ң����բ �޵�ʱ�� 
}CTRL_TURN_PRM, *pCTRL_TURN_PRM;

//����Ͷ��������
typedef struct _tagKEEP_POWER_PRM
{
	INT8U		nKeepPowerFlg;							//(F25/F33)����Ͷ��/���
    STD_TIME    sKeepStartTime;                         //��ʼ����ʱ��
    INT8U		nKeepPowerTime;							//(F25/F33)����ʱ�� 
}KEEP_POWER_PRM, *pKEEP_POWER_PRM;


// ���ʶ�ֵ��ʱ������
typedef struct _tagWATT_PARAM
{
    INT8U       nStrHour;                            	//��ʼBCD ��Сʱ  ----- = 0xFF : ��ʾ��Чʱ��
    INT8U       nStrMin;                            	//��ʼBCD �����
    INT8U       nEndHour;                              	//����BCD ��Сʱ
    INT8U       nEndMin;                            	//����BCD �����
    INT8U       nKind;                                 	//����ʱ�Σ�0 ���� 1 ���� ����ʱ�Σ�����1-48
}WATT_PARAM, *pWATT_PARAM;

//������ͳ��
typedef struct _tagMT_POWER_STAT
{
    FORMAT13            sPPPower[1+MAX_FEE_NUM];	    //�����й�������
    FORMAT13            sPNPower[1+MAX_FEE_NUM];	    //�����޹�������
    FORMAT13	        sNPPower[1+MAX_FEE_NUM];	    //�����й�������
    FORMAT13	        sNNPower[1+MAX_FEE_NUM];	    //�����޹�������
}MT_POWER_STAT, *pMT_POWER_STAT;

extern pPARAM              g_pParam;
#endif

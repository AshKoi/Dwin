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
#ifndef _MEM_DATA_H_
#define _MEM_DATA_H_

#include "Os_cpu.h"
#include "Macro.h"
#include "Table.h"
#include "global.h"
#include "Ac7022.h"


/**************************************************************************************************
                                        ������ʵʱ����
***************************************************************************************************/
typedef struct _tagMEM_MP_REAL_DATA
{
	INT8U       	     		nMeterAddr[LEN_METER_ADDR];		    	    //����ַ(��ǩ)
	INT8U						nReadTime[6];             	                //����ʱ��								//����ʱ��YY-MM-DD-HH-MM-SS
	
	INT8U						nDate[4];									//�������
	INT8U						nTime[3];									//���ʱ��
	INT8U						nEnergyPP[MAX_FEE_NUM + 1][4];				//�����й�
	INT8U						nEnergyNP[MAX_FEE_NUM + 1][4];				//�����й�
	INT8U						nEnergyPN[MAX_FEE_NUM + 1][4];				//�����޹�
	INT8U						nEnergyNN[MAX_FEE_NUM + 1][4];				//�����޹�					
	INT8U                       nQ1Energy[MAX_FEE_NUM + 1][4];				//һ����
	INT8U                       nQ2Energy[MAX_FEE_NUM + 1][4];				//������
	INT8U                       nQ3Energy[MAX_FEE_NUM + 1][4];				//������
	INT8U                       nQ4Energy[MAX_FEE_NUM + 1][4];				//������
	INT8U                       nPPDemand[MAX_FEE_NUM + 1][8];				//�����й�����				
	INT8U                       nNPDemand[MAX_FEE_NUM + 1][8];				//�����й�����
	INT8U                       nPNDemand[MAX_FEE_NUM + 1][8];				//�����޹�����
	INT8U                       nNNDemand[MAX_FEE_NUM + 1][8];				//�����޹�����
	INT8U                       nVolt[3][2];								//��ѹ
	INT8U                       nCurr[3][3];								//����
	INT8U						nPWatt[4][3];								//�й�����
	INT8U						nNWatt[4][3];								//�޹�����
	INT8U						nSWatt[4][3];								//���ڹ���
	INT8U                       nFactor[4][2];								//��������
    
    MT_POWER_STAT       	    sDayPowerStat;                		        //�յ���ͳ��
    MT_POWER_STAT       	    sMonPowerStat;                		        //�µ���ͳ��
}MEM_MP_REAL_DATA,*pMEM_MP_REAL_DATA;


/**************************************************************************************************
                                        ������������
***************************************************************************************************/
typedef struct _tagMEM_MP_DAY_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//����ַ(��ǩ)
	INT8U						sDataTime[3];							//ʱ��(��ǩ)YY-MM-DD
	INT8U						nReadTime[6];							//����ʱ��YY-MM-DD-HH-MM-SS
	
	INT8U                       nDEnergyPP[MAX_FEE_NUM + 1][4];			//�������й�
	INT8U                       nDEnergyNP[MAX_FEE_NUM + 1][4];			//�շ����й�
	INT8U                       nDEnergyPN[MAX_FEE_NUM + 1][4];			//�������޹�
	INT8U                       nDEnergyNN[MAX_FEE_NUM + 1][4];			//�շ����޹�
    INT8U                       nDQ1Energy[MAX_FEE_NUM + 1][4];			//��һ����
	INT8U                       nDQ2Energy[MAX_FEE_NUM + 1][4];			//�ն�����
	INT8U                       nDQ3Energy[MAX_FEE_NUM + 1][4];			//��������
	INT8U                       nDQ4Energy[MAX_FEE_NUM + 1][4];			//��������
	INT8U                       nDPPDemand[MAX_FEE_NUM + 1][8];			//�������й�����
	INT8U                       nDNPDemand[MAX_FEE_NUM + 1][8];			//�շ����й�����
	INT8U                       nDPNDemand[MAX_FEE_NUM + 1][8];			//�������޹�������
	INT8U                       nDNNDemand[MAX_FEE_NUM + 1][8];			//�����޹�����

}MEM_MP_DAY_DATA,*pMEM_MP_DAY_DATA;


/**************************************************************************************************
                                        ������������
***************************************************************************************************/
typedef struct _tagMEM_MP_MON_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//����ַ(��ǩ)
	INT8U						sDataTime[2];								//ʱ��(��ǩ)YY-MM
	INT8U						nReadTime[6];								//����ʱ��YY-MM-DD-HH-MM-SS
	
	INT8U                       nMEnergyPP[MAX_FEE_NUM + 1][4];			//�������й�
	INT8U                       nMEnergyNP[MAX_FEE_NUM + 1][4];			//�·����й�
	INT8U                       nMEnergyPN[MAX_FEE_NUM + 1][4];			//�������޹�
	INT8U                       nMEnergyNN[MAX_FEE_NUM + 1][4];			//�·����޹�
    INT8U                       nMQ1Energy[MAX_FEE_NUM + 1][4];			//��һ����
	INT8U                       nMQ2Energy[MAX_FEE_NUM + 1][4];			//�¶�����
	INT8U                       nMQ3Energy[MAX_FEE_NUM + 1][4];			//��������
	INT8U                       nMQ4Energy[MAX_FEE_NUM + 1][4];			//��������
	INT8U                       nMPPDemand[MAX_FEE_NUM + 1][8];			//�������й�����
	INT8U                       nMNPDemand[MAX_FEE_NUM + 1][8];			//�·����й�����
	INT8U                       nMPNDemand[MAX_FEE_NUM + 1][8];			//�������޹�����
	INT8U                       nMNNDemand[MAX_FEE_NUM + 1][8];			//�·����޹�����
	

}MEM_MP_MON_DATA,*pMEM_MP_MON_DATA;


/**************************************************************************************************
                                        ��������������
***************************************************************************************************/
typedef struct _tagMEM_MP_CRV_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//����ַ(��ǩ)
	INT8U						sDataTime[5];								//ʱ��(��ǩ)YY-MM-DD-HH-MM
	
	INT8U                       nCrvEnergyPP[MAX_FEE_NUM + 1][4];			//�������й�
	INT8U                       nCrvEnergyNP[MAX_FEE_NUM + 1][4];			//�·����й�
	INT8U                       nCrvEnergyPN[MAX_FEE_NUM + 1][4];			//�������޹�
	INT8U                       nCrvEnergyNN[MAX_FEE_NUM + 1][4];			//�·����޹�
    INT8U                       nCrvVolt[3][2];							//��ѹ
	INT8U                       nCrvCurr[3][3];							//����
	INT8U						nCrvPWatt[4][3];							//�й�����
	INT8U						nCrvNWatt[4][3];							//�޹�����
	INT8U						nCrvSWatt[4][3];							//���ڹ���
	INT8U                       nCrvFactor[4][2];							//��������
	
}MEM_MP_CRV_DATA,*pMEM_MP_CRV_DATA;


/**************************************************************************************************
                                        �����㳭��������
***************************************************************************************************/
typedef struct _tagMEM_MP_RDAY_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//����ַ(��ǩ)
	INT8U						sDataTime[3];								//ʱ��(��ǩ)YY-MM-DD
	
	INT8U						nRDEnergyPP[MAX_FEE_NUM + 1][4];			//�����й�
	INT8U						nRDEnergyNP[MAX_FEE_NUM + 1][4];			//�����й�
	INT8U						nRDEnergyPN[MAX_FEE_NUM + 1][4];			//�����޹�
	INT8U						nRDEnergyNN[MAX_FEE_NUM + 1][4];			//�����޹�	
	
}MEM_MP_RDAY_DATA,*pMEM_MP_RDAY_DATA;





/**************************************************************************************************
                                        (RAM��ʱ����)
***************************************************************************************************/
typedef struct _tagMEM_X_RAM_DATA
{
	INT8U               nRs485Real;                         //Rs485ʵʱ����־
    INT8U		        nRs485Day;                          //Rs485���ձ�־
    INT8U		        nRs485Mon;                          //Rs485���±�־
    INT8U		        nRs485Crv;                          //Rs485���߱�־
    INT8U               nResetFlg;                          //��λ��־
    INT8U               nTripFlg[MAX_TURN_NUM];             //��բ��־
    INT8U               nLockFlg[MAX_TURN_NUM];             //��բ��־
    INT8U		        nRmotSigl;							//Զ��Link�ź�ǿ��
    INT8U               nLinkReq;                           //Զ���������F38,F39
    
    AC_FRE_STAT		    sAcFreeze;							//AC���������־
	AC_FRE_STAT		    sAcInit;							//AC�����־
	AC_EVECLR_STAT	    sAcEvent[3];						//AC�¼������־
    
    INT8U               nRdMtProtFlg;                       //1:645 2400,2:3761 9600,��ʹ�ã�
}MEM_X_RAM_DATA,*pMEM_X_RAM_DATA;



/**************************************************************************************************
                                        (FLASH��ʱ����)
***************************************************************************************************/
typedef struct _tagMEM_X_FLH_DATA
{
	INT8U				nRealNext;
	DAY_MEM_INFO		sDayMemInfo;
	MON_MEM_INFO		sMonMemInfo;
	CRV_MEM_INFO		sCrvMemInfo;
	RDAY_MEM_INFO		sRDayMemInfo;
    
    EVT_MEM_INFO        sRcEvtInfo;         //ң���¼���Ϣ
    RC_EVT_DATA         sRcEvtData[MAX_RC_EVT_NUM];     //ң���¼���¼
    
    INT8U		        nACRunStat[LEN_RUN_STAT_FLAG]; 
}MEM_X_FLH_DATA,*pMEM_X_FLH_DATA;


/**************************************************************************************************
                                        (FRAM��ʱ����)
***************************************************************************************************/
typedef struct _tagMEM_X_FRAM_DATA
{
    INT8U               nMacAddr[6];
    INT8U               nFileInitFlg;
	STD_TIME			sTermClk;
    INT64U              nPulseNum[MAX_PULSE_NUM];
    PULSE_RATE_NUM      nPerRateNum[MAX_FEE_NUM];
    
    PULSE_RATE_NUM      nDayRateNum[MAX_FEE_NUM];
    PULSE_RATE_NUM      nMonRateNum[MAX_FEE_NUM];
    
    INT8U               nPsw[6];
    INT8U               nPswSetFlg;
    INT8U               nGprsFlg;

}MEM_X_FRAM_DATA,*pMEM_X_FRAM_DATA;


/**************************************************************************************************
                                          ��������
***************************************************************************************************/

/**************************************************************************************************
                                          ˲ʱ���ݴ洢
***************************************************************************************************/
//����˲ʱ���� MEM_OBJ_AC_INSTANT
typedef struct _tagMEM_AC_INST_DATA
{
	AC_INSTANT_DATA	sData;
	//AC_SYNC_DATA	sSyncData;
    AC_AVEARGE_DATA sAver;
}MEM_AC_INST_DATA, *pMEM_AC_INST_DATA;

/**************************************************************************************************
                                          ʵʱ���ݴ洢
***************************************************************************************************/
//����ʵʱ���� MEM_OBJ_AC_REAL
typedef struct _tagMEM_AC_REAL_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
    INT32U						nENChkSum;
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	ENERGY_DATA					sEPulse[8];
	PH_ENERGY_DATA				sPhEPulse[MAX_PHASE_NUM];
	PH_ENERGY_DATA				sPhQEPulse[MAX_PHASE_NUM];
    COMB_ENERGY_DATA            sCombEPulse;
	INT32U						nAcDemand[MAX_P_Q_NUM+4][MAX_FEE_NUM+1];
	INT8U						nVer[8];
	MT_DGN_ENERGY_DATA          sAcDamage;
    MT_DGN_ENERGY_DATA          sAcFirmLoss;
    MT_DGN_ENERGY_DATA          sAcCopperLoss;
    MT_DGN_ENERGY_DATA          sAcIronLoss;
    
	INT8U						nBack[4];
}MEM_AC_REAL_DATA, *pMEM_AC_REAL_DATA;



#if 0
/**************************************************************************************************
                                          ʵʱ���ݴ洢
***************************************************************************************************/
//����ʵʱ���� MEM_OBJ_AC_REAL
typedef struct _tagMEM_AC_REAL_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
	ENERGY_DATA					sEPulse[8];
	PH_ENERGY_DATA				sPhEPulse[MAX_PHASE_NUM];
	PH_ENERGY_DATA				sPhQEPulse[MAX_PHASE_NUM];
	INT16U						nEnergyCrc;

    LOSER_ENERGY_DATA			sAcLoserEnergy;
    LOSER_ACCU_DATA	            sLoserAccuData;
    INT16U						nLoserCrc;
    
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	FP32						fAcDemand[MAX_P_Q_NUM+4][MAX_FEE_NUM+1];
	INT16U						nDemandCrc;
	
	INT8U						nVer[8];
	//INT8U						nBack[130];
}MEM_AC_REAL_DATA, *pMEM_AC_REAL_DATA;
#endif

/**************************************************************************************************
                                          ʵʱ���ݱ���
***************************************************************************************************/
//����ʵʱ���ݱ��� MEM_OBJ_AC_BACK
typedef struct _tagMEM_AC_REAL_BACK
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
	ENERGY_DATA					sEPulse[8];
	PH_ENERGY_DATA				sPhEPulse[MAX_PHASE_NUM];
	PH_ENERGY_DATA				sPhQEPulse[MAX_PHASE_NUM];
	INT16U						nEnergyCrc;
}MEM_AC_REAL_BACK, *pMEM_AC_REAL_BACK;

/**************************************************************************************************
                                          �����ݴ洢
***************************************************************************************************/
//���������� MEM_OBJ_AC_DAY
typedef struct _tagMEM_AC_DAY_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;
    INT32U	                    sPriority;        //��¼��� 23.2.24
}MEM_AC_DAY_DATA, *pMEM_AC_DAY_DATA;

/**************************************************************************************************
                                          �������ݴ洢
***************************************************************************************************/
//���ɷ������� MEM_OBJ_AC_DAY
typedef struct _tagMEM_AC_MIN_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;   
    FORMAT07		            sRealVolt[MAX_PHASE_NUM];  
	FORMAT25		            sRealCurr[MAX_PHASE_NUM+1]; 
    INT32U	                    sPriority;        //��¼���
}MEM_AC_MIN_DATA, *pMEM_AC_MIN_DATA;

typedef struct _tagMEM_EVE_DATA
{
    STD_TIME   sEveStartClk;
    STD_TIME   sEveFinshClk;

}MEM_EVE_DATA, *pMEM_EVE_DATA;

/**************************************************************************************************
                                          ���������ݴ洢
***************************************************************************************************/
//���ɽ��������� MEM_OBJ_AC_RDAY
typedef struct _tagMEM_AC_RDAY_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;
}MEM_AC_RDAY_DATA, *pMEM_AC_RDAY_DATA;

/**************************************************************************************************
                                          �������ݴ洢
***************************************************************************************************/
//�������� MEM_OBJ_AC_SSDJ, MEM_OBJ_AC_DSDJ, MEM_OBJ_AC_SQDJ, MEM_OBJ_AC_SDDJ
typedef struct _tagMEM_AC_DJ_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;
}MEM_AC_DJ_DATA, *pMEM_AC_DJ_DATA;

/**************************************************************************************************
                                           ��ʷ���ݴ洢
***************************************************************************************************/
typedef struct _tagMEM_AC_HIST_DATA
{
    MEM_AC_DAY_DATA     sDayData[MAX_AC_DAY_DATA_NUM];
    MEM_AC_RDAY_DATA    sRdDayData[MAX_AC_RDAY_DATA_NUM];
    MEM_AC_DJ_DATA      sSsdjData[MAX_AC_SSDJ_DATA_NUM];
    MEM_AC_DJ_DATA      sDsdjData[MAX_AC_DSDJ_DATA_NUM];
    MEM_AC_MIN_DATA     sSqdjData[MAX_AC_SQDJ_DATA_NUM];
    MEM_AC_DJ_DATA      sSddjData[MAX_AC_SDDJ_DATA_NUM];
    MEM_AC_DJ_DATA      sMonDayData[MAX_AC_MON_DATA_NUM];
    MEM_AC_DJ_DATA      sNULLDayData[1];
}MEM_AC_HIST_DATA, *pMEM_AC_HIST_DATA;


/**************************************************************************************************
                                           �¼����ݴ洢
***************************************************************************************************/
typedef struct _tagMEM_AC_EVT
{
	MEM_AC_EVT_DATA			sACEvtSyA[MAX_AC_EVT_DATA_NUM];			//ʧѹ�¼�(A)
    MEM_AC_EVT_DATA			sACEvtSyB[MAX_AC_EVT_DATA_NUM];			//ʧѹ�¼�(B)
	MEM_AC_EVT_DATA			sACEvtSyC[MAX_AC_EVT_DATA_NUM];			//ʧѹ�¼�(C)
	MEM_AC_EVT_DATA			sACEvtDxA[MAX_AC_EVT_DATA_NUM];			//�����¼�(A)
	MEM_AC_EVT_DATA			sACEvtDxB[MAX_AC_EVT_DATA_NUM];			//�����¼�(B)
	MEM_AC_EVT_DATA			sACEvtDxC[MAX_AC_EVT_DATA_NUM];			//�����¼�(C)
	MEM_AC_EVT_DATA			sACEvtQsy[MAX_AC_EVT_DATA_NUM];			//ȫʧѹ�¼�
	MEM_AC_EVT_DATA			sACEvtDynx[MAX_AC_EVT_DATA_NUM];		//��ѹ�����¼�
	MEM_AC_EVT_DATA			sACEvtSlA[MAX_AC_EVT_DATA_NUM];			//ʧ���¼�(A)
	MEM_AC_EVT_DATA			sACEvtSlB[MAX_AC_EVT_DATA_NUM];			//ʧ���¼�(B)
	MEM_AC_EVT_DATA			sACEvtSlC[MAX_AC_EVT_DATA_NUM];			//ʧ���¼�(C)
	MEM_AC_EVT_DATA			sACEvtDd[MAX_AC_EVT_DATA_NUM];			//�����¼�
	MEM_AC_EVT_DATA			sACEvtXlql[MAX_AC_EVT_DATA_NUM];		//���������¼�
	MEM_AC_EVT_DATA			sACEvtDbql[MAX_AC_EVT_DATA_NUM];		//��������¼�
	MEM_AC_EVT_DATA			sACEvtSjql[MAX_AC_EVT_DATA_NUM];		//�¼������¼�
	MEM_AC_EVT_DATA			sACEvtBc[MAX_AC_EVT_DATA_NUM];			//����¼�
	MEM_AC_EVT_DATA			sACEvtJs[MAX_AC_EVT_DATA_NUM];			//Уʱ�¼�
    
    MEM_AC_EVT_DATA			sACEvtGyA[MAX_AC_EVT_DATA_NUM];			//��ѹ�¼�(A)
    MEM_AC_EVT_DATA			sACEvtGyB[MAX_AC_EVT_DATA_NUM];			//��ѹ�¼�(B)
	MEM_AC_EVT_DATA			sACEvtGyC[MAX_AC_EVT_DATA_NUM];			//��ѹ�¼�(C)
    
    MEM_AC_EVT_DATA			sACEvtGlA[MAX_AC_EVT_DATA_NUM];			//�����¼�(A)
    MEM_AC_EVT_DATA			sACEvtGlB[MAX_AC_EVT_DATA_NUM];			//�����¼�(B)
	MEM_AC_EVT_DATA			sACEvtGlC[MAX_AC_EVT_DATA_NUM];			//�����¼�(C)
    
    MEM_AC_EVT_DATA			sACEvtClA[MAX_AC_EVT_DATA_NUM];			//���������¼�(A)
    MEM_AC_EVT_DATA			sACEvtClB[MAX_AC_EVT_DATA_NUM];			//���������¼�(B)
	MEM_AC_EVT_DATA			sACEvtClC[MAX_AC_EVT_DATA_NUM];			//���������¼�(C)
    
    MEM_AC_EVT_DATA			sACEvtGzA[MAX_AC_EVT_DATA_NUM];			//�����¼�(A)
    MEM_AC_EVT_DATA			sACEvtGzB[MAX_AC_EVT_DATA_NUM];			//�����¼�(B)
	MEM_AC_EVT_DATA			sACEvtGzC[MAX_AC_EVT_DATA_NUM];			//�����¼�(C)
    
    MEM_AC_EVT_DATA         sACEvtKG[MAX_AC_EVT_DATA_NUM];
    
    MEM_AC_EVT_DATA         sACEvtTZ[MAX_AC_EVT_DATA_NUM];          //��բ�¼�22.2.16
    MEM_AC_EVT_DATA         sACEvtHZ[MAX_AC_EVT_DATA_NUM];          //��բ�¼�22.2.17
    MEM_AC_EVT_DATA         sACEvtMY[MAX_AC_EVT_DATA_NUM];          //��Կ�¼�22.2.17
    MEM_AC_EVT_DATA         sACEvtFH[MAX_AC_EVT_DATA_NUM];          //�����¼�22.2.17
    MEM_AC_EVT_DATA         sACEvtDY[MAX_AC_EVT_DATA_NUM];          //��Դ�¼�22.2.17
    MEM_AC_EVT_DATA         sACEvtSZ[MAX_AC_EVT_DATA_NUM];          //ʱ���¼�22.2.17
    MEM_AC_EVT_DATA         sACEvtJL[MAX_AC_EVT_DATA_NUM];          //����оƬ�¼�22.2.17
    MEM_AC_EVT_DATA         sACEvtglA[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtglB[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtglC[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSKG[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSTZ[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSHZ[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSMY[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSFH[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSDY[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSSZ[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSJL[MAX_AC_EVT_DATA_NUM];          //
    
    MEM_AC_EVT_DATA			sACEvtQyA[MAX_AC_EVT_DATA_NUM];			//Ƿѹ�¼�(A)
    MEM_AC_EVT_DATA			sACEvtQyB[MAX_AC_EVT_DATA_NUM];			//Ƿѹ�¼�(B)
	MEM_AC_EVT_DATA			sACEvtQyC[MAX_AC_EVT_DATA_NUM];			//Ƿѹ�¼�(C)
    MEM_AC_EVT_DATA			sACEvtDlA[MAX_AC_EVT_DATA_NUM];			//�����¼�(A)
	MEM_AC_EVT_DATA			sACEvtDlB[MAX_AC_EVT_DATA_NUM];			//�����¼�(B)
	MEM_AC_EVT_DATA			sACEvtDlC[MAX_AC_EVT_DATA_NUM];			//�����¼�(C)
}MEM_AC_EVT, *pMEM_AC_EVT;

/**************************************************************************************************
                                             ���ɱ������ݴ洢
***************************************************************************************************/
//(FRAM����)�������� MEM_OBJ_AC_X_BACK
typedef struct _tagMEM_AC_BACK_DATA
{
	INT32U				nSyTime;			//ʧѹ���ۼ�ʱ��
	INT32U				nAxsyTime;			//A��ʧѹ�ۼ�ʱ��
	INT32U				nBxsyTime;			//B��ʧѹ�ۼ�ʱ��
	INT32U				nCxsyTime;			//C��ʧѹ�ۼ�ʱ��
	INT32U				nAxdxTime;			//A������ۼ�ʱ��
	INT32U				nBxdxTime;			//B������ۼ�ʱ��
	INT32U				nCxdxTime;			//C������ۼ�ʱ��
	INT32U				nQsyTime;			//ȫʧѹ�ۼ�ʱ��
	INT32U				nDynxTime;			//��ѹ�����ۼ�ʱ��
	INT32U				nAxslTime;			//A��ʧ���ۼ�ʱ��
	INT32U				nBxslTime;			//B��ʧ���ۼ�ʱ��
	INT32U				nCxslTime;			//C��ʧ���ۼ�ʱ��
	AC_EVENT_STAT		sEventStat;			//�����¼�״̬
    
    
    INT32U				nGyTime;			//��ѹ���ۼ�ʱ��
    INT32U				nAxgyTime;			//A���ѹ�ۼ�ʱ��
	INT32U				nBxgyTime;			//B���ѹ�ۼ�ʱ��
	INT32U				nCxgyTime;			//C���ѹ�ۼ�ʱ��
    INT32U				nGlTime;			//�������ۼ�ʱ��
    INT32U				nAxglTime;			//A������ۼ�ʱ��
	INT32U				nBxglTime;			//B������ۼ�ʱ��
	INT32U				nCxglTime;			//C������ۼ�ʱ��
    INT32U				nClTime;			//�����������ۼ�ʱ��
    INT32U				nAxclTime;			//A�ೱ�������ۼ�ʱ��
	INT32U				nBxclTime;			//B�ೱ�������ۼ�ʱ��
	INT32U				nCxclTime;			//C�ೱ�������ۼ�ʱ��
    INT32U				nGzTime;			//�������ۼ�ʱ��
    INT32U				nAxgzTime;			//A������ۼ�ʱ��
	INT32U				nBxgzTime;			//B������ۼ�ʱ��
	INT32U				nCxgzTime;			//C������ۼ�ʱ��

    STD_TIME			sTermClk;           //���ʱ��
    
    INT32U				nQyTime;			//Ƿѹ���ۼ�ʱ��
	INT32U				nAxqyTime;			//A��Ƿѹ�ۼ�ʱ��
	INT32U				nBxqyTime;			//B��Ƿѹ�ۼ�ʱ��
	INT32U				nCxqyTime;			//C��Ƿѹ�ۼ�ʱ��
    
    INT32U				nDlTime;			//�������ۼ�ʱ��
	INT32U				nAxDlTime;			//A������ۼ�ʱ��
	INT32U				nBxDlTime;			//B������ۼ�ʱ��
	INT32U				nCxDlTime;			//C������ۼ�ʱ��
    
    INT32U				nRTCTime;			//ʱ�ӵ�����ۼ�ʱ��
    STD_TIME			sBcClk;             //���һ�α��ʱ��
}MEM_AC_BACK_DATA, *pMEM_AC_BACK_DATA;


//(FLASH����)�������� MEM_OBJ_AC_X_BACK_FLH
typedef struct _tagMEM_AC_BACK_FLH_DATA
{
	INT8U				nXBackBuff[14];		//���ݻ��汣����14�ֽ�

	AC_DAY_MEM_INFO		sDayInfo;			//��������Ϣ
	AC_DAY_MEM_INFO		sRDayInfo;			//������������Ϣ
	AC_DAY_MEM_INFO		sSsdjInfo;			//˲ʱ����������Ϣ
	AC_DAY_MEM_INFO		sDsdjInfo;      	//��ʱ����������Ϣ
	AC_DAY_MEM_INFO		sSqdjInfo;      	//Լ������������Ϣ
	AC_DAY_MEM_INFO		sSddjInfo;      	//Լ������������Ϣ
    AC_DAY_MEM_INFO		sMONInfo;			//��������Ϣ
    
	AC_EVT_MEM_INFO		sEvtAxsyInfo;		//A��ʧѹ�¼���Ϣ
	AC_EVT_MEM_INFO		sEvtBxsyInfo;		//B��ʧѹ�¼���Ϣ
	AC_EVT_MEM_INFO		sEvtCxsyInfo;		//C��ʧѹ�¼���Ϣ
	AC_EVT_MEM_INFO		sEvtAxdxInfo;		//A������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtBxdxInfo;		//B������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtCxdxInfo;		//C������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtQsyInfo; 		//ȫʧѹ�¼���Ϣ
	AC_EVT_MEM_INFO		sEvtDynxInfo;		//��ѹ�����¼���Ϣ
	AC_EVT_MEM_INFO		sEvtAxslInfo;		//A��ʧ��������Ϣ
	AC_EVT_MEM_INFO		sEvtBxslInfo;		//B��ʧ��������Ϣ
	AC_EVT_MEM_INFO		sEvtCxslInfo;		//C��ʧ��������Ϣ
	AC_EVT_MEM_INFO		sEvtDDInfo;     	//�����¼���Ϣ
	AC_EVT_MEM_INFO		sEvtXlqlInfo;		//���������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtDbqlInfo;		//��������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtSjqlInfo;		//�¼������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtBcInfo;     	//����¼���Ϣ
	AC_EVT_MEM_INFO		sEvtJsInfo;     	//Уʱ�¼���Ϣ	

	INT32U     			nSyNUM;				//��ʧѹ����
	STD_TIME			sSyFssk;			//���1��ʧѹ����ʱ��(�ֽ�Week��Ч)
	STD_TIME			sSyJssk;			//���1��ʧѹ����ʱ��(�ֽ�Week��Ч)
	INT32U				nAxsyNUM;			//A��ʧѹ����
	INT32U				nBxsyNUM;			//B��ʧѹ����
	INT32U				nCxsyNUM;			//C��ʧѹ����
	INT32U     			nDxNUM;				//�ܶ������
	INT32U				nAxdxNUM;			//A��������
	INT32U				nBxdxNUM;			//B��������
	INT32U				nCxdxNUM;			//C��������
	INT32U				nQsyNUM;			//ȫʧѹ�ܴ���
	INT32U				nDynxNum;			//��ѹ�������
	INT32U     			nSlNUM;				//��ʧ������
	INT32U				nAxslNUM;			//A��ʧ������
	INT32U				nBxslNUM;			//B��ʧ������
	INT32U				nCxslNUM;			//C��ʧ������
	INT32U				nDdNum;				//�������
	INT32U				nXlqlNum;			//�����������
	INT32U				nDbqlNum;			//����������
	INT32U				nSjqlNum;			//�¼��������
	INT32U				nBcNum;				//��̴���
	INT32U				nJsNum;				//Уʱ����
	INT8U				nBroadJsFlag;		//0x55Ϊ�㲥Уʱ����
    MEM_AC_REAL_DATA    sRealData;		    //ʵʱ����
    
    INT32U     			nGyNUM;				//�ܹ�ѹ����
	STD_TIME			sGyFssk;			//���1�ι�ѹ����ʱ��(�ֽ�Week��Ч)
	STD_TIME			sGyJssk;			//���1�ι�ѹ����ʱ��(�ֽ�Week��Ч)
	INT32U				nAxgyNUM;			//A���ѹ����
	INT32U				nBxgyNUM;			//B���ѹ����
	INT32U				nCxgyNUM;			//C���ѹ����
    AC_EVT_MEM_INFO		sEvtAxgyInfo;		//A���ѹ�¼���Ϣ
	AC_EVT_MEM_INFO		sEvtBxgyInfo;		//B���ѹ�¼���Ϣ
	AC_EVT_MEM_INFO		sEvtCxgyInfo;		//C���ѹ�¼���Ϣ
    
    INT32U     			nGlNUM;				//�ܹ�������
	STD_TIME			sGlFssk;			//���1�ι�������ʱ��(�ֽ�Week��Ч)
	STD_TIME			sGlJssk;			//���1�ι�������ʱ��(�ֽ�Week��Ч)
	INT32U				nAxglNUM;			//A���������
	INT32U				nBxglNUM;			//B���������
	INT32U				nCxglNUM;			//C���������
    AC_EVT_MEM_INFO		sEvtAxglInfo;		//A������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtBxglInfo;		//B������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtCxglInfo;		//C������¼���Ϣ
    
    INT32U     			nClNUM;				//�ܳ����������
	STD_TIME			sClFssk;			//���1�γ���������ʱ��(�ֽ�Week��Ч)
	STD_TIME			sClJssk;			//���1�γ����������ʱ��(�ֽ�Week��Ч)
	INT32U				nAxclNUM;			//A�ೱ���������
	INT32U				nBxclNUM;			//B�ೱ���������
	INT32U				nCxclNUM;			//C�ೱ���������
    AC_EVT_MEM_INFO		sEvtAxclInfo;		//A�ೱ�������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtBxclInfo;		//B�ೱ�������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtCxclInfo;		//C�ೱ�������¼���Ϣ
    
    INT32U     			nGzNUM;				//�ܹ��ش���
	STD_TIME			sGzFssk;			//���1�ι��ط���ʱ��(�ֽ�Week��Ч)
	STD_TIME			sGzJssk;			//���1�ι��ؽ���ʱ��(�ֽ�Week��Ч)
	INT32U				nAxgzNUM;			//A����ش���
	INT32U				nBxgzNUM;			//B����ش���
	INT32U				nCxgzNUM;			//C����ش���
    AC_EVT_MEM_INFO		sEvtAxgzInfo;		//A������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtBxgzInfo;		//B������¼���Ϣ
	AC_EVT_MEM_INFO		sEvtCxgzInfo;		//C������¼���Ϣ
    
    INT8U				nACEvtTrip;		//��բ״̬
    
    INT32U				nOpenCoverNum;			//���Ǵ���
    AC_EVT_MEM_INFO     sOpenCoverInfo;        //������Ϣ 
    
    INT32U              nACEvtTripNum;          //��բ����
    AC_EVT_MEM_INFO     sACEvtTripInfo;        //��բ��Ϣ
    
    INT32U              nEvtSwitchOnNum;          //��բ����
    AC_EVT_MEM_INFO     sEvtSwitchOnInfo;        //��բ��Ϣ
    
    INT32U              nEvtSecretKeyNum;          //��Կ����
    AC_EVT_MEM_INFO     sEvtSecretKeyInfo;        //��Կ��Ϣ
    
    INT32U              nEvtLoadSwitchNum;          //���ɿ��ش���
    AC_EVT_MEM_INFO     sEvtLoadSwitchInfo;        //���ɿ�����Ϣ
    
    INT32U              nEvtPowerAbnlNum;          //��Դ�쳣����
    AC_EVT_MEM_INFO     sEvtPowerAbnlInfo;        //��Դ�쳣��Ϣ 
    
    INT32U              nEvtClockTrbNum;          //ʱ���쳣����
    AC_EVT_MEM_INFO     sEvtClockTrbInfo;        
    
    INT32U              nEvtChipTrbNum;          //оƬ�쳣����
    AC_EVT_MEM_INFO     sEvtChipTrbInfo;     
    
    INT32U     			nQyNUM;				//��Ƿѹ����
	STD_TIME			sQyFssk;			//���1��Ƿѹ����ʱ��(�ֽ�Week��Ч)
	STD_TIME			sQyJssk;			//���1��Ƿѹ����ʱ��(�ֽ�Week��Ч)
	INT32U				nAxqyNUM;			//A��Ƿѹ����
	INT32U				nBxqyNUM;			//B��Ƿѹ����
	INT32U				nCxqyNUM;			//C��Ƿѹ����
    
    INT32U     			nDlNUM;				//�ܶ�������
	INT32U				nAxdlNUM;			//A���������
	INT32U				nBxdlNUM;			//B���������
	INT32U				nCxdlNUM;			//C���������
    INT8U				nACOffset;		//����ƫ��
    SET_MEM_INFO        sSetParametersInfo; 
    

}MEM_AC_BACK_FLH_DATA, *pMEM_AC_BACK_FLH_DATA;


#endif

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
#ifndef _MEM_PRM_H_
#define _MEM_PRM_H_

#include "Os_cpu.h"
#include "Macro.h"
#include "Table.h"
#include "RX8025T.h"


/**************************************************************************************************
                                        1.�ն�ͨѶ�����洢
***************************************************************************************************/
//�ն�ͨѶ�����洢�ṹ MEM_TERM_COMM_PRM
typedef struct _tagMEM_TERM_COMM_PRM
{
    INT8U		nAreaCode[LEN_AREA_CODE];				//(F94)��������
    INT8U       nTermAddr[LEN_TERM_ADDR];				//(F94)�ն˵�ַ
    
    INT8U		nAreaCodeTmp[LEN_AREA_CODE];			//(F95)��������
    INT8U       nTermAddrTmp[LEN_TERM_ADDR];			//(F95)�ն˵�ַ
    
    INT8U       nCarrZbSelect;                          //(F85)�ز�ר��ѡ��0x55ר�䡢0xAA������
    
    INT8U		nRadioDlyMs;						    //(F1)�ն�������ʱ(��λ��MS)
    INT8U    	nTransDlyS;							    //(F1)�ն���Ϊ����վ�����ͷ��صĴ�����ʱʱ��(��λ��S)
    INT8U 		nRespTOS;							    //(F1)�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��(��λ��S)
    INT8U		nRptTimes;							    //(F1)�ط�����
    INT8U    	nReplyFlag;							    //(F1)��Ҫ��վȷ�ϵ�ͨ�ŷ���ı�־(D0��1�������Զ��ϱ�,D1��2�������Զ��ϱ�)
    INT8U  	    nBeatCycS;							    //(F1)��������(��λ��S)

    INT8U       nMainIP[LEN_IP_ADDR];					//(F3)����IP��ַ
    INT8U       nMainIPPort[2];						    //(F3)����IP�˿�
    INT8U       nBackIp[LEN_IP_ADDR];					//(F3)����IP��ַ
    INT8U       nBackIpPort[2];             			//(F3)����IP�˿�
    INT8U       nAPN[LEN_APN];                       	//(F3)APN

    INT8U       nGsmCdma[8];                       	    //(F4)GSM/CDMA
    INT8U       nTelePhonNum[8];                 	    //(F4)�绰����

    INT8U       nTerminalIp[LEN_IP_ADDR];          	    //(F7)�ն�IP��ַ
    INT8U       nTerminalMask[4];                  	    //(F7)�ն���������
    INT8U       nTerminalGate[4];                		//(F7)�ն�����
    INT8U       nTerminalAgent;                  		//(F7)��������
    INT8U       nTerminalAddr[4];                 		//(F7)�����������ַ
    INT8U       nTerminalPorts[2];                 	    //(F7)����������˿�
    INT8U       nProxyLinkModl;                         //(F7)�������ӷ�ʽ
    INT8U       nProxyUserLen;                   		//(F7)�����û�������
    INT8U       nUserName[20];                     	    //(F7)����������û���
    INT8U       nPassWordLen;                      	    //(F7)������������볤��
    INT8U       nPassWord[20];                       	//(F7)�������������
	INT8U       nTerminalPort[2];                   	//(F7)�ն˼����˿�

	INT8U       nWorkMode;                        		//(F8)����ģ�(���ģʽ���ͻ���ģʽ��������ģʽ)
    INT8U       nOnlineMode[2];                     	//(F8)�������ߡ�ʱ������ģʽ�ز����(��λ:��)
    INT8U       nActivMode;                         	//(F8)��������ģʽ�ز�����(��λ:��)
    INT8U       nBreakoutMode;                      	//(F8)��������ģʽ������ͨ���Զ�����ʱ(��λ:min)
    INT8U       nAllowOnMode[3];                     	//(F8)ʱ������ģʽ��������ʱ�α���("1"��ʾ��������ʱ�Σ���"0"��ʾ��ֹ����ʱ��)

    INT8U       nVirtuNetUser[32];                 	    //(F16)����ר���û���
    INT8U       nVirtuNetPassw[32];                	    //(F16)����ר������

	INT8U       nFlowCommun[4];                     	//(F36)��ͨ����������

	INT8U		nChkNum;								//У��
}MEM_TERM_COMM_PRM, *pMEM_TERM_COMM_PRM;


/**************************************************************************************************
                                       2.�������洢
***************************************************************************************************/
//�������洢�ṹ MEM_METER_PRM
typedef struct _tagMEM_METER_EXT_PRM
{
    MEM_METER_PRM		sMeterPrm;						//(F10)
    //MEM_PULSE_PRM       sPulsePrm;   
    MEM_MT_BASE_PRM     sMtBasePrm; 
}MEM_METER_EXT_PRM, *pMEM_METER_EXT_PRM;


/**************************************************************************************************
                                       3.�ܼ�������洢
***************************************************************************************************/
//�ܼ�������洢
typedef struct _tagMEM_GRP_PRM
{
    MEM_PULSE_PRM       sPulsePrm[MAX_PULSE_NUM];       //��F11��������� 
    
    INT8U       nStats;                                 //��F12��״̬�������־λ
    INT8U       nAttr;                                  //��F12��״̬����������λ
  
    INT8U       nGrpSeq;								//(F14)�ܼ������
    INT8U       nGrpMpNum;								//(F14)�ܼ������������
    INT8U       nGrpMpAttr[MAX_GRP_MP_NUM];			    //(F14)�ܼ��������ż��ܼ۱�־
    
	INT8U		nWattTurnBS;							//(F45)�ִ�
	
	INT8U		nWattCtrlSlip;							//(F43)����ʱ��
	
    INT8U       nPeriodPrjBS;							//(F41)ʱ�οط�����־
    WATT_CTRL_PERIOD_PRM	sWattCtrlPeriodPrm[3];	    //(F41)ʱ�ζ�ֵ

	INT8U		nFactoryDef[2];						    //(F42)���ݿض�ֵ
	FORMAT19	sFactoryTime;							//�޵�ʱ��
	INT8U		nFactoryCntTimes;						//����ʱ��
	INT8U		nFactoryDateBS;						    //�޵��־��

	FORMAT20	sBusinessTimeS;						    //(F44)Ӫҵ��ͣ����ʼʱ��
	FORMAT20	sBusinessTimeE;						    //Ӫҵ��ͣ�ؽ���ʱ��
	INT8U		nBusinessDef[2];						//��ֵ

	INT8U		nEnergyTurnBS;							//(F48)�ִ�
	
	INT8U       nEnergyDef[4];                    		//(F46)�µ������ض�ֵ
	INT8U       nEnergyAlmCoff[4];                      //(F46)����ϵ��
	
    INT8U       nBuyEnergyNo[4];						//(F47)���絥��
    INT8U		nBuyRfresh;							    //(F47)ˢ�±�־
	INT8U		nBuyEnergy[4];							//(F47)������
	INT8U		nBuyEnergyAlm[4];						//����
	INT8U		nBuyEnergySwitch;						//��բ

}MEM_GRP_PRM, *pMEM_GRP_PRM;

/**************************************************************************************************
                                       4.�������в����洢
***************************************************************************************************/
//�������в����洢
typedef struct _tagMEM_RDMETER_PRM
{
    MEM_COM_PRM     sComPrm[MAX_COM_NUM];
}MEM_RDMETER_PRM, *pMEM_RDMETER_PRM;

/**************************************************************************************************
                                       5.6.��С��Ų����洢
***************************************************************************************************/
//��С��Ų����洢
typedef struct _tagMEM_METER_TYPE_PRM
{
    METER_TYPE_INFO_PRM		sMeterTypeInfoPrm[MAX_METER_MINTYPE_NUM];		//(F38)��Ϣ�����
}MEM_METER_TYPE_PRM, *pMEM_METER_TYPE_PRM;

/**************************************************************************************************
                                       7.8.��������洢
***************************************************************************************************/
//�������
typedef struct _tagMEM_TASK_PRM
{
	INT8U		nTaskValid;							//F67
	
	INT8U		nCycleUnit;							//(F65)��ʱ�ϱ����ڼ���λ
    FORMAT01    sReportBaseTime;						//�ϱ���׼
    INT8U		nRate;									//���߱���
    INT8U		nDADTNum;								//���ݵ�Ԫ��Ŀ
    INT8U		nDADT[255];							//���ݵ�Ԫ
}MEM_TASK_PRM, *pMEM_TASK_PRM;


/**************************************************************************************************
                                       9.���Ʋ����洢
***************************************************************************************************/
//���������
typedef struct _tagMEM_CTRL_PRM
{
	CTRL_TURN_PRM   sCtrlTurnPrm[MAX_TURN_NUM];        //(F1)ң����բ

	CTRL_GRP_PRM	sCtrlGrpPrm[MAX_GRP_NUM];			//(F9~F16)����Ͷ��

	KEEP_POWER_PRM  sKeepPowerPrm;                      //F25
    
	INT8U		nPressValid;							//(F26/F34)�߷�Ͷ��/���
	INT8U		nCommValid;							    //(F27/F35)�ն�����վͨ��Ͷ��/���
	INT8U		nRejectValid;							//(F28/F36)�޳�Ͷ��/���
	INT8U		nReportValid;							//(F29/F37)�����ϱ�Ͷ��/���
	INT8U		nSelfFindMeterValid;					//(F149)�Զ�ά������
    
	
}MEM_CTRL_PRM, *pMEM_CTRL_PRM;


/**************************************************************************************************
                                       10.���������洢
***************************************************************************************************/
//���������洢�ṹ MEM_OTHER_PRM
typedef struct _tagMEM_OTHER_PRM
{
	INT8U		nSlaveCommPort; 						//(F37)����ͨ�Ŷ˿�
	INT8U		nSlaveCommCtrl; 						//(F37)����ͨ�ſ�����
	INT8U		nSlaveFrmTOS;							//(F37)���ĳ�ʱʱ��
	INT8U		nSlaveByteTOS;							//(F37)�ֽڳ�ʱʱ��
	INT8U		nSlaveRpt;								//(F37)ʧ���ط�����
	INT8U		nSlaveItvl; 							//(F37)Ѳ������
	INT8U		nSlaveNumBS;							//(F37)������־������
	INT8U		nSlave[3][4];							//(F37)������ַ

	
	

	INT8U		nSafeNo;								//(F5)��Ϣ��֤��
	INT8U		nSafePrm;								//(F5)��Ϣ�ϲ���
	INT8U		nTermGrpAddr[8][2];					    //(F6)���ַ
	INT8U		nErcRecordBS[8];						//(F9)��¼��־
	INT8U		nErcGradeBS[8];						//(F9)�ȼ���־
	INT8U		nStateVarFlgs;							//(F12)״̬�������־λ����Ӧ1��8·״̬��"1"�����룬��"0"��δ����)
	INT8U		nStateVarproper;						//(F12)״̬�����Ա�־λ����Ӧ1��8·״̬��"1"�����룬��"0"��δ����)
	INT8U		nTermDispMeterAddr[12];				//(F29)�ն˵�����ʾ���ܱ��
	INT8U		nStopRdMeter;							//(F30)̨�����г���ͣ��/Ͷ��
	INT8U		nMeterValidBS[256];					//(F150)��������Ч��־
	INT8U		nMeterChangeBS[256];					//(F150)��������������־	
	INT8U		nAnlogBS;								//(F61)ֱ��ģ��������
	INT8U		nAnlogValS[2];							//(F81)ֱ��ģ����������ʼ
	INT8U		nAnlogValE[2];							//(F81)ֱ��ģ�������̽���
	INT8U		nAnlogLimitUp[2];						//(F82)ֱ��ģ��������
	INT8U		nAnlogLimitDown[2];					//(F82)ֱ��ģ��������
	INT8U		nAnlogFrez;							//(F83)ֱ��ģ�����������
	



	
	INT8U		nMeterDiff1;							//(F59)���ܱ���
	INT8U		nMeterDiff2;							//(F59)���ܱ����
	INT8U		nMeterStop15Min;						//(F59)���ܱ�ͣ��
	INT8U		nMeterAdjustMin;						//(F59)���ܱ�Уʱ



	
	
	INT8U       nPressFeeAla[3];                    	//(F23)�߷Ѹ澯����M��ֹ��־λ
	INT8U       nSoundBS[3];                        	//(F57)�����澯
	INT8U       nProtectMin;                        	//(F58)����ʱ��




	INT8U       nWattSafeDef[2];                     	//(F17)������ֵ
	INT8U       nWattDefCoeff;                        	//(F19)ʱ�ι��ض�ֵ����ϵ��(��λ:%)
	INT8U       nWattPeriodBS[12];                 	//(F18)ÿ��Сʱ����λ�����ʾ���ֿ���״̬��ȡֵ0��3���α�ʾ�����ơ�����1������2��������    
	INT8U       nTurnAlmTime[MAX_TURN_NUM];           	//(F49)�����ִθ澯ʱ��



	
    INT8U       nEnergyDefCoeff;                    	//(F20)�µ������ض�ֵ����ϵ��(��λ:%)
	//INT8U       nEnergyPeriodBS[12];            		//(F21)ÿ30����Ϊһ��ʱ�Σ���ʱ�η��ʺ�ȡֵ0��47���α�ʾ����1������48������ֵ��Ч��
    //INT8U       nEnergyMFeeNum1;                		//(F21)��������ȡֵ��Χ1��48��������Ч��
    WATT_PARAM	sPowerPerd[MAX_FEE_PERD_NUM];			//(F21)�ն˵���������ʱ��(APP)
	INT8U       nEnergyMFeeNum2;                 		//(F22)������M(��λ:��)
    INT8U       nEnergyFeeVal[MAX_FEE_NUM][3];       	//(F22)����(��λ:��/Ԫ)
    



    //PULSE_PRM	sPulsePrm[MAX_PULSE_NUM];				//(F11)�������
    ANLOG_PRM	sAnlogPrm[MAX_ANLOG_NUM];				//(F13)��ѹ/����ģ����
	DIFF_PRM	sDiffPrm[MAX_DIFF_NUM];				//(F15)���
	DOWN_MODL_PRM	sDownModlPrm[PORT_NUM];			//(F34)���ж˿ڲ�������


	
	
	INT8U       nImMeterNum;							//(F35)�ص�����Ŀ
    INT8U       nImSeq[MAX_IM_METER_NUM][2];			//(F35)�ص������
	INT8U       nRelayTermNum;							//(F2)�м��ն���Ŀ
    INT8U       nRelayTermAddr[MAX_RELAY_TERM_NUM][2];//(F2)�м��ն�
    INT8U       nColNodeNum;							//(F31)�ӽڵ㸽���ڵ���Ŀ
    INT8U       nColNodeAddr[MAX_RELAY_TERM_NUM][6];	//(F31)�ӽڵ㸽���ڵ�
    
}MEM_OTHER_PRM, *pMEM_OTHER_PRM;

/**************************************************************************************************
                                       11.У�����
***************************************************************************************************/
typedef struct _tagMEM_AC_CAL_PRM
{
	INT32U	nModeCfg;			//ģʽ��ؿ���
	INT32U	nPGActrl;			//ADC��������
	INT32U	nEMUCfg;			//EMU��Ԫ����
	INT32U	nPgain[3];			//�й���������
	INT32U	nQgain[3];			//�޹���������
	INT32U	nSgain[3];			//���ڹ�������
	INT32U	nPhSregpq0[3];  	//��λУ��0
	INT32U	nPhSregpq1[3];		//��λУ��1
	INT32U	nPoffset[3];		//�й�����offsetУ��
	INT32U	nQPhscal;			//�޹���λУ��
	INT32U	nUgain[3];			//��ѹ����
	INT32U	nIgain[3];			//��������
	INT32U	nIstarup;			//�𶯵�����ֵ����
	INT32U	nHfconst;			//��Ƶ�����������
	INT32U	nFailVoltage;		//ʧѹ��ֵ����
	INT32U	nGainADC7;			//����·ADC�����ź�����
	INT32U	nQoffset[3];		//�޹�����offsetУ��
	INT32U	nURmsoffse[3];		//��ѹ��ЧֵoffsetУ��
	INT32U	nIRmsoffse[3];		//������ЧֵoffsetУ��
	INT32U	nUoffset[3];		//��ѹͨ��ADC offsetУ��
	INT32U	nIoffset[3];		//����ͨ��ADC offsetУ��
	INT32U	nEMUIE;				//�ж�ʹ��
	INT32U	nModuleCFG;			//��·ģ�����üĴ���
	INT32U	nAllGain;			//ȫͨ�����棬����Vref���¶�У��
	INT32U	nHFDouble;			//���峣���ӱ�ѡ��
	INT32U	nLineGain;			//��������У��
	INT32U	nPineCtrl;			//����Pin����������ѡ�����
	INT32U	nPstart;			//�𶯹������üĴ���
	INT32U	nIregion;			//��λ�����������üĴ���
	INT32U	nReserved[2];		//reserved

	INT32U	nCheckSumEntire;	// ����У��Ĵ������ۼӺ�
	INT32U	nCheckSumReset;		// Ĭ��У��Ĵ���ֵ
	INT32U	nChkCode;			// Checksum code
    
    //INT8U   nChkSum;            
}MEM_AC_CAL_PRM, *pMEM_AC_CAL_PRM;

/**************************************************************************************************
                                       12.���ɲ���
***************************************************************************************************/
typedef struct _tagMEM_AC_PRM
{
	INT8U		nDemandPeriod; 
    INT8U		nSlipTime;
	INT8U		nDispTime1;                                           
	INT8U		nDispNum1;                                        
	INT8U		nDispNum2;                                           
	INT8U		nDispTime2;                                    
	INT8U		nDispPoint1;                                    
	INT8U		nDispPoint2;                                       
	INT8U		nCurveDay;                                         
	FORMAT11	sPBaseEnergy;                                          
	FORMAT11	sQBaseEnergy;                                        
                                                                      
	FORMAT15	sYearChangeTime;                                     
	FORMAT15	sDayChangeTime;                                      
	INT8U		nYearSegmNum;                                       
	INT8U		nDayTableNum;                                          
	INT8U		nDaySegmNum;                                         
	INT8U		nFeeNumber;                                           
	INT16U		nHolidayNumber;                                        
	INT8U		nHarmcNumber;                                         
	INT8U       nMeterAddr[LEN_METER_ADDR];                           
	INT8U		nMeterNumber[LEN_METER_ADDR];                        
	INT8U		nMeterProNum[LEN_METER_GLBH];                         
	INT8U		nEpCompFlag;                                         
	INT8U		nEq1CompFlag;                                         
	INT8U		nEq2CompFlag;                                         
	INT8U		nIR1Baud;                                          
	INT8U		nIR2Baud;                                            
	INT8U		nCom1Baud;                                           
	INT8U		nCom2Baud;                                            
	INT8U		nCom3Baud;                                            
	INT8U		nHolidayFlag;                                        
	INT8U		nHolidayTable;                                        
	FORMAT24	sMonthJS1;                                           
	FORMAT24	sMonthJS2;                                            
	FORMAT24	sMonthJS3;                                           
	INT8U		nMeterPWD[10][LEN_METER_PASSWORD];                  
	FORMAT23	sPosPowLimt;                   
	FORMAT23	sNegPowLimt;                                         
	FORMAT07	sUpLimtVolt;                                         
	FORMAT07	sLowLimtVolt;                                         
	MM_DD_NN	sYearSegmTable[2][MAX_YEAR_SEGM_NUM];                 
	hh_mm_NN	sDaySegmTable[2][MAX_DAY_TABLE_NUM][MAX_DAY_SEGM_NUM];
	YY_MM_DD_NN	sHolidayTable[MAX_HOLIDAY_NUM];
	FORMAT17	sPeriodicityDJ;
    //INT32U      nObjId; 
    INT32U		nHardType;
    INT16U      nCTRate;
    INT16U      nPTRate; 
    FP32        pTransR[3];
    FP32        pTransX[3]; 
    FP32        pTransG[3]; 
    FP32        pTransB[3]; 
    INT8U		nDispModel;          //����ģʽ��0������  1������
    //FORMAT17    sPeriodicityMinDJ;
    //������Ӳ��ɳ���1024����Ϊ���ݿ����޶���1024������ֽ���
  //  INT8U       nMeter698Addr[LEN_METER_ADDR];
   // INT8U       nMeter698ProNum[LEN_METER_ADDR];
    INT8U		        nChkNum;								//У��
}MEM_AC_PRM, *pMEM_AC_PRM;

/**************************************************************************************************
                                       12.���������洢
***************************************************************************************************/
//���������
typedef struct _tagMEM_EXT_PRM
{
	INT8U		nTmp;							//(F1)

	
}MEM_EXT_PRM, *pMEM_EXT_PRM;


#endif

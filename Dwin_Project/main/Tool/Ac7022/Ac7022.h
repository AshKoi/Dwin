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
#ifndef _AC_7022_H_
#define _AC_7022_H_

#include "Os_cpu.h"
#include "Config.h"
#include "MemPrm.h"


/**************************************************************************************************
                                          ��غ궨��
***************************************************************************************************/

#define	PosEp			0		//�����й�
#define	NegEp			1		//�����й�
#define	PosEq			2		//�����޹�
#define	NegEq			3		//�����޹�

#define AC_FLASH_WR_TIME	60//900	//ACʵʱ���ݱ���ʱ����900S
#define AC_FRAM_WR_TIME		1//5		//ACʵʱ����дʱ����180S

//#define RATED_VOLT              2200                         //���ѹ
#define RATED_CURR              1500//5                         //�����
#define MAX_CURR                6000                         //������

#define VOLT_PH_ERR_RATIO       0.6                         //��ѹ�������ѹ����
#define VOLT_POWER_FAIL_RATIO   0.6                         //����
#define LOSS_VOLT_OC_RATIO      0.78                        //������ѹ��Զ��ѹ�ı���(ʧѹ)
#define LOSS_VOLT_RE_RATIO      0.85                        //���ص�ѹ��Զ��ѹ�ı���(ʧѹ)
#define LOSS_PH_OC_RATIO        0.6//0.78                         //������ѹ��Զ��ѹ�ı���(����)
#define LOSS_PH_RE_RATIO        0.85                        //���ص�ѹ��Զ��ѹ�ı���(����)
#define LOSS_CURR_RATIO         0.7                         //�ٽ���ѹ��Զ��ѹ�ı���(ʧ��)
#define OVER_VOLT_OC_RATIO      1.15                        //������ѹ��Զ��ѹ�ı���(��ѹ)
#define FAIL_CURR_RATIO         0.6                         //�ٽ���ѹ��Զ��ѹ�ı���(����)



#define OC_CURR                 0.005                       //����������Զ�����ı���
#define RE_CURR                 0.05                        //���ص�����Զ�����ı���
#define LOSS_PH_CURR            0.005                       //���෢����������������
#define OV_CURR                 1.2                        //��������������ı���


/************************************ֻд�Ĵ���***************************************************/

#define w_ModeCfg		0x01	//ģʽ��ؿ���
#define w_PGACtrl		0x02	//ADC��������
#define w_EMUCfg		0x03	//EMU��Ԫ����
#define w_PgainA		0x04	//A���й���������
#define w_PgainB		0x05	//B���й���������
#define w_PgainC		0x06	//C���й���������
#define w_QgainA		0x07	//A���޹���������
#define w_QgainB		0x08	//B���޹���������
#define w_QgainC		0x09	//C���޹���������
#define w_SgainA		0x0A	//A�����ڹ�������
#define w_SgainB		0x0B	//B�����ڹ�������
#define w_SgainC		0x0C	//C�����ڹ�������
#define w_PhSregApq0	0x0D	//A����λУ��0
#define w_PhSregBpq0	0x0E	//B����λУ��0
#define w_PhSregCpq0	0x0F	//C����λУ��0
#define w_PhSregApq1	0x10	//A����λУ��1
#define w_PhSregBpq1	0x11	//B����λУ��1
#define w_PhSregCpq1	0x12	//C����λУ��1
#define w_PoffsetA		0x13	//A���й�����offsetУ��
#define w_PoffsetB		0x14	//B���й�����offsetУ��
#define w_PoffsetC		0x15	//C���й�����offsetУ��
#define w_QPhscal		0x16	//�޹���λУ��
#define w_UgainA		0x17	//A���ѹ����
#define w_UgainB		0x18	//B���ѹ����
#define w_UgainC		0x19	//C���ѹ����
#define w_IgainA		0x1A	//A���������
#define w_IgainB		0x1B	//B���������
#define w_IgainC		0x1C	//C���������
#define w_Istarup		0x1D	//�𶯵�����ֵ����
#define w_Hfconst		0x1E	//��Ƶ�����������
#define w_FailVoltage	0x1F	//ʧѹ��ֵ���ã���������ģʽ��
#define w_GainADC7		0x20	//����·ADC�����ź�����
#define w_QoffsetA		0x21	//A���޹�����offsetУ��
#define w_QoffsetB		0x22	//B���޹�����offsetУ��
#define w_QoffsetC		0x23	//C���޹�����offsetУ��
#define w_UaRmsoffse	0x24	//A���ѹ��ЧֵoffsetУ��
#define w_UbRmsoffse	0x25	//B���ѹ��ЧֵoffsetУ��
#define w_UcRmsoffse	0x26	//C���ѹ��ЧֵoffsetУ��
#define w_IaRmsoffse	0x27	//A�������ЧֵoffsetУ��
#define w_IbRmsoffse	0x28	//B�������ЧֵoffsetУ��
#define w_IcRmsoffse	0x29	//C�������ЧֵoffsetУ��
#define w_UoffsetA		0x2A	//A���ѹͨ��ADC offsetУ��
#define w_UoffsetB		0x2B	//B���ѹͨ��ADC offsetУ��
#define w_UoffsetC		0x2C	//C���ѹͨ��ADC offsetУ��
#define w_IoffsetA		0x2D	//A�����ͨ��ADC offsetУ��
#define w_IoffsetB		0x2E	//B�����ͨ��ADC offsetУ��
#define w_IoffsetC		0x2F	//C�����ͨ��ADC offsetУ��
#define w_EMUIE			0x30	//�ж�ʹ��
#define w_ModuleCFG		0x31	//��·ģ�����üĴ���
#define w_AllGain		0x32	//ȫͨ�����棬����Vref���¶�У��
#define w_HFDouble		0x33	//���峣���ӱ�ѡ��
#define w_LineGain		0x34	//��������У��
#define w_PinCtrl		0x35	//����pin����������ѡ�����
#define w_Pstart		0x36	//�𶯹������üĴ���
#define w_Iregion		0x37	//��λ�����������üĴ���

/************************************ֻ���Ĵ���***************************************************/
#define	r_ChkSum		0x3E	//У�����У��Ĵ���

/***˲ʱ��***/
/***˲ʱ��***/
#define	r_Pa			0x01	//A���й�����
#define	r_Pb			0x02	//B���й�����
#define	r_Pc			0x03	//C���й�����
#define	r_Pt			0x04	//�����й�����
#define	r_Qa			0x05	//A���޹�����
#define	r_Qb			0x06	//B���޹�����
#define	r_Qc			0x07	//C���޹�����
#define	r_Qt			0x08	//�����޹�����
#define	r_Sa			0x09	//A�����ڹ���
#define	r_Sb			0x0A	//B�����ڹ���
#define	r_Sc			0x0B	//C�����ڹ���
#define	r_St			0x0C	//�������ڹ���
#define	r_UaRms			0x0D	//A���ѹ��Чֵ
#define	r_UbRms			0x0E	//B���ѹ��Чֵ
#define	r_UcRms			0x0F	//C���ѹ��Чֵ
#define	r_IaRms			0x10	//A�������Чֵ
#define	r_IbRms			0x11	//B�������Чֵ
#define	r_IcRms			0x12	//C�������Чֵ
#define	r_ItRms			0x13	//ABC�����ʸ���͵���Чֵ
#define	r_Pfa			0x14	//A�๦������
#define	r_Pfb			0x15	//B�๦������
#define	r_Pfc			0x16	//C�๦������
#define	r_Pft			0x17	//���๦������
#define	r_Pga			0x18	//A��������ѹ���
#define	r_Pgb			0x19	//B��������ѹ���
#define	r_Pgc			0x1A	//C��������ѹ���
#define	r_INTFlag		0x1B	//�жϱ�־����������
#define	r_Freq			0x1C	//��Ƶ��

#define	r_YUaUb			0x26	//Ua��Ub�ĵ�ѹ�н�
#define	r_YUaUc			0x27	//Ua��Uc�ĵ�ѹ�н�
#define	r_YUbUc			0x28	//Ub��Uc�ĵ�ѹ�н�
#define r_I0Rms			0x29	//���ߵ���I0ͨ����Чֵ
#define	r_TempD			0x2A	//�¶ȴ����������
#define	r_UtRms			0x2B	//�����ѹʸ���͵���Чֵ
#define	r_Sflag			0x2C	//��Ŷ��ࡢ����SIG�ȱ�־״̬

#define r_Sample_IA     0x2F	//A�����ͨ��ADC��������
#define r_Sample_IB     0x30	//B�����ͨ��ADC��������
#define r_Sample_IC     0x31	//C�����ͨ��ADC��������
#define r_Sample_UA     0x32	//A���ѹͨ��ADC��������
#define r_Sample_UB     0x33	//B���ѹͨ��ADC��������
#define r_Sample_UC     0x34	//C���ѹͨ��ADC��������

#define	r_PFlag			0x3D	//�й����޹����ʷ�������Ϊ0����Ϊ1

#define	r_Sample_I0		0x3F	//���ߵ���I0ͨ�������������
#define	r_LinePa		0x40	//A������й�����
#define	r_LinePb		0x41	//B������й�����
#define	r_LinePc		0x42	//C������й�����
#define	r_LinePt		0x43	//��������й�����

#define	r_LineUaRrms	0x48	//����A���ѹ��Чֵ
#define	r_LineUbRrms	0x49	//����B���ѹ��Чֵ
#define	r_LineUcRrms	0x4A	//����C���ѹ��Чֵ
#define	r_LineIaRrms	0x4B	//����A�������Чֵ
#define	r_LineIbRrms	0x4C	//����B�������Чֵ
#define	r_LineIcRrms	0x4D	//����C�������Чֵ

/****ȫ������ֵ*****/
#define	r_EFlag			0x1D	//���ܼĴ����Ĺ���״̬����������

#define	r_Epa			0x1E	//A���й����ܣ�������Ϊ�������㣩
#define	r_Epb			0x1F	//B���й����ܣ�������Ϊ�������㣩
#define	r_Epc			0x20	//C���й����ܣ�������Ϊ�������㣩
#define	r_Ept			0x21	//�����й����ܣ�������Ϊ�������㣩
#define	r_Eqa			0x22	//A���޹����ܣ�������Ϊ�������㣩
#define	r_Eqb			0x23	//B���޹����ܣ�������Ϊ�������㣩
#define	r_Eqc			0x24	//C���޹����ܣ�������Ϊ�������㣩
#define	r_Eqt			0x25	//�����޹����ܣ�������Ϊ�������㣩

#define r_BckReg		0x2D	//ͨѶ���ݱ��ݼĴ���
#define r_ComChksum     0x2E	//ͨѶУ��ͼĴ���

#define	r_Esa			0x35	//A�����ڵ��ܣ�������Ϊ�������㣩
#define	r_Esb			0x36	//B�����ڵ��ܣ�������Ϊ�������㣩
#define	r_Esc			0x37	//C�����ڵ��ܣ�������Ϊ�������㣩
#define	r_Est			0x38	//�������ڵ��ܣ�������Ϊ�������㣩
#define r_FstCntA		0x39	//A������������
#define r_FstCntB		0x3A	//B������������
#define r_FstCntC		0x3B	//C������������
#define r_FstCntT		0x3C	//��������������

#define r_LineEpa		0x44	//A������й����ܣ�������Ϊ�������㣩
#define r_LineEpb		0x45	//B������й����ܣ�������Ϊ�������㣩
#define r_LineEpc		0x46	//C������й����ܣ�������Ϊ�������㣩
#define r_LineEpt		0x47	//��������й����ܣ�������Ϊ�������㣩

#define r_LEFlag		0x4E	//�������ܼĴ����Ĺ���״̬����������
#define r_PtrWavebuff	0x7E	//������ָ�룬ָʾ�ڲ�����buffer�������ݳ���
#define r_WaveBuff   	0x7F	//�������ݼĴ������ڲ������棬�ظ���ȡֱ�����껺�����ݳ���


/**************************************************************************************************
                                             ��ؽṹ����
***************************************************************************************************/
#if 0
typedef	struct
{
	FP32 fVal[CALUCATES_PER_MINUTE];					//��������
	INT8U nSlips;										//��������
	INT8U nIsRev;										//�Ƿ�ת
}Slip_Data;
#endif

typedef	struct
{
	INT32S nVal[CALUCATES_PER_MINUTE];					//��������
	INT8U nSlips;										//��������
	INT8U nIsRev;										//�Ƿ�ת
}Slip_Data;

typedef struct
{
	FP32 fVal[PARAM_MAX_SLIP_TIME];//��������
	INT8U nSlips;					//��������
	INT8U nIsRev;					//�Ƿ�ת
}DemandSlip_Data1;

typedef struct
{
	FP32 fVal[PARAM_MAX_DEMAND_PERIOD];//��������
	INT8U nSlips;						//��������
	INT8U nIsRev;						//�Ƿ�ת
}DemandSlip_Data2;

//����˲ʱ����
typedef struct _tagAC_INSTANT_DATA
{
	FORMAT07		sRealVolt[MAX_PHASE_NUM];   //���β��ѹ
	FORMAT25		sRealCurr[MAX_PHASE_NUM+1]; //���β����
	FORMAT09		sRealP[MAX_PHASE_NUM+1];    //���β��й�����
	FORMAT09		sRealQ[MAX_PHASE_NUM+1];    //���β��޹�����
	FORMAT09		sRealS[MAX_PHASE_NUM+1];    //���β����ڹ���
	FORMAT05		sRealPf[MAX_PHASE_NUM+1];   //��������
	FORMAT07		sRealY[MAX_PHASE_NUM];      //��ѹ���
	FORMAT07		sRealPg[MAX_PHASE_NUM];     //�������
    FORMAT11		sRealVolt1[MAX_PHASE_NUM];  //һ�β��ѹ
    FORMAT11		sRealCurr1[MAX_PHASE_NUM];  //һ�β����
    FORMAT46		sRealP1[MAX_PHASE_NUM+1];   //һ�β��й�����
	FORMAT46		sRealQ1[MAX_PHASE_NUM+1];   //һ�β��޹�����
	FORMAT46		sRealS1[MAX_PHASE_NUM+1];   //һ�β����ڹ���
	INT16U			bsRunStat[7];
}AC_INSTANT_DATA, *pAC_INSTANT_DATA;

typedef struct _tagAC_STAT_DATA
{
	FORMAT07		sStatVolt[MAX_PHASE_NUM];  //���β��ѹ
	FORMAT25		sStatCurr[MAX_PHASE_NUM+1]; //���β����
	FORMAT09		sStatP[MAX_PHASE_NUM+1];   //���β��й�����
	FORMAT09		sStatQ[MAX_PHASE_NUM+1];   //���β��޹�����
	FORMAT09		sStatS[MAX_PHASE_NUM+1];   //���β����ڹ���
	FORMAT05		sStatPf[MAX_PHASE_NUM+1];  //��������
	FORMAT07		sStatY[MAX_PHASE_NUM];     //��ѹ���
	FORMAT07		sStatPg[MAX_PHASE_NUM];    //�������
    FORMAT04		sStatTp;                    //�¶�
}AC_AVEARGE_DATA, *pAC_AVERAGE_DATA;

typedef struct
{
	FP32 fVal[PARAM_MAX_DEMAND_PERIOD];//��������
    INT16U nStep;						//����
	INT8U nSlips;						//��������
	INT8U nIsRev;						//�Ƿ�ת
}DemandSlip_Data;

typedef	struct
{
	AC_INSTANT_DATA		sRealInstant;                           //ʵʱ
    AC_AVEARGE_DATA     sStatAverage;                           //���ӻ���
    INT32U				nPulseConst;							//�������峣��
	INT32U	    		nHFConst;		    					//��Ƶ�������
    INT8U               nCombEp;                                //����й�������
    INT8U               nCombEq1;                               //����޹�1������
    INT8U               nCombEq2;                               //����޹�2������
    INT32U				nCheckSumReset;							//��ʼУ�飬����ģʽ�ж�
	INT32U				nCheckSumEntire;						//У��У�飬����״̬�ж�
	
	INT16U				nPulsePCounter;							//�й����������
	INT16U				nPulseQCounter;							//�й����������
	INT16U				nFramCounter;							//����д����
	INT16U				nFlashCounter;							//����д����
    INT32U				nACRunStat;								//����״̬�֣�d0�ڴ桢d1����У�顢d2����У�顢d3�������d4�����

    INT16U				nPowerPulse[MAX_P_Q_NUM+4][MAX_PHASE_NUM+1];	//�ַ����й��޹���������
    INT16U				nPowerPulse1[MAX_P_Q_NUM+4][MAX_PHASE_NUM+1];	//�ַ����й��޹���������
    INT16U				nCurrHFPulse;
    INT16U				nSaveHFPulse[MAX_P_Q_NUM+4][2];
	Slip_Data			sVoltSlip[MAX_PHASE_NUM];				//�����ѹ
	FP32				fVolt[MAX_PHASE_NUM];
	Slip_Data			sCurrSlip[MAX_PHASE_NUM+1];				//������� �������
	FP32				fCurr[MAX_PHASE_NUM+1];
	Slip_Data			sPSlip[MAX_PHASE_NUM+1];				//��������й�����
	Slip_Data			sQSlip[MAX_PHASE_NUM+1];				//��������޹�����
	Slip_Data			sSSlip[MAX_PHASE_NUM+1];				//����������ڹ���
	Slip_Data			sPfSlip[MAX_PHASE_NUM+1];				//������ܹ�������
	Slip_Data			sTpSlip;                 //�¶�
    
	INT8U				nPFlag;									//�й����޹����ʷ�������Ϊ0����Ϊ1
	INT8U				nSFlag;									//���� ʧѹ��־
	INT16U				nRealEnergy[MAX_P_Q_NUM+2][MAX_PHASE_NUM+1];
	INT8U				nRateNo;								//��ǰ���ʺ�
	INT8U				nSQNum;									//��ǰʱ����
	INT8U				nSDNum;									//��ǰʱ�α�
    INT8U               nAEMode;
    INT8U               nPREMode;
    INT8U               nNREMode;
	INT8U				nReadFlag;
	DemandSlip_Data 	sDemandSlipData1[MAX_P_Q_NUM+4];		//������������ʱ����
	DemandSlip_Data 	sDemandSlipData2[MAX_P_Q_NUM+4];		//�ַ�������������ʱ����
    FP32				fFeAPower[MAX_PHASE_NUM+1];	//�����й�
    FP32				fFeRPower[MAX_PHASE_NUM+1];	//�����޹�
    FP32				fCuAPower[MAX_PHASE_NUM+1];	//ͭ���й�
    FP32				fCuRPower[MAX_PHASE_NUM+1];	//ͭ���޹�
}REAL_DATA, *pREAL_DATA;

//�������
typedef	struct
{
	INT32U			nDemand[MAX_FEE_NUM+1];						//�����ʼ����������
	FORMAT15		sTimeData[MAX_FEE_NUM+1];					//����ʱ��
}AC_DEMAND_DATA;

//�����������
typedef struct _tagPULSE_CONST
{
	INT32U	    nPulConst;	        //���峣��
	INT32U	    nHFConst1;		    //��Ӧ�������߸�Ƶ�������
	INT32U      nHFConst2;		    //��Ӧ�������߸�Ƶ�������
}PULSE_CONST, *pPULSE_CONST;

#if 0
typedef	struct
{
	ENERGY_DATA		sEnergyData[8];								//�����й����� �����й�����	�����޹����� �����޹����� �����޵���
	PH_ENERGY_DATA	sPhEnergyData[MAX_PHASE_NUM];
	PH_ENERGY_DATA	sPhQEnergyData[MAX_PHASE_NUM];

	AC_DEMAND_DATA	sDemandData[8];								//�������
	INT16U			nENCrcChk;
	INT16U			nDECrcChk;
	INT32U			nPulseConst;								//�������峣������
	INT32U	    	nHFConst;		    						//��Ƶ�����������
}SAVE_REAL_DATA, *pSAVE_REAL_DATA;
#endif

#if 1
typedef	struct
{
    MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
    INT32U				        nENSumChk;
	ENERGY_DATA			        sEnergyData[8];								//�����й����� �����й�����	�����޹����� �����޹����� �����޵���
	PH_ENERGY_DATA		        sPhEnergyData[MAX_PHASE_NUM];
	PH_ENERGY_DATA		        sPhQEnergyData[MAX_PHASE_NUM];
    COMB_ENERGY_DATA            sCombEPulse;
    INT32U                      nEPSumChk;
	AC_DEMAND_DATA		        sDemandData[8];								//�������
	INT32U				        nDESumChk;
	INT32U				        nPulseConst;								//�������峣������
	INT32U	    		        nHFConst;		    						//��Ƶ�����������
    INT8U                       nCombEp;                                    //����й�������
    INT8U                       nCombEq1;                                   //����޹�1������
    INT8U                       nCombEq2;                                   //����޹�2������
}SAVE_REAL_DATA, *pSAVE_REAL_DATA;
#endif



/**************************************************************************************************
                                             ��������
***************************************************************************************************/
extern INT32U      g_nConstant;
extern INT32U      g_nHFConst;
extern INT8U    g_nSetCtrlcurFlg;
extern pREAL_DATA pDataTable;
extern  PULSE_CONST nPulTable[];
extern pSAVE_REAL_DATA pSavDataTable;
extern INT8U nAcTmpBuf[1024];//���Žṹ�Ĵ�С���ı䣬��û�һ�ַ�ʽ����
//extern INT8U g_nFreeChgFlg;
extern     INT8U g_SimMeterCoverFlag;
extern     INT8U g_RealMeterCoverFlag;
extern     INT8U g_SimMeterCoverTimeBuf[];
extern     INT8U g_SimMeterPasswordFlag;

extern     INT8U g_SimMeterTripFlag;//ģ����բ��־ 0xFF�� 0xBF ��
extern     INT8U g_RealMeterTripFlag;//ʵ����բ��־

extern     INT8U g_SimMeterSwitchOnFlag;//ģ���բ��־ 0xFF�� 0xBF ��
extern     INT8U g_RealMeterSwitchOnFlag;//ʵ�ʺ�բ��־
extern     INT8U g_SimLoadSwitchAcFlag;//ģ�⸺�ɿ���������־ 0xFF�� 0xBF ��
extern     INT8U g_RealLoadSwitchAcFlag;//ʵ�ʸ��ɿ���������־
extern     INT8U g_SimPowerAbnlFlag;//ģ���Դ�쳣��־ 0xFF�� 0xBF ��
extern     INT8U g_RealPowerAbnlFlag;//ʵ�ʵ�Դ�쳣��־
extern     INT8U g_SimClockTrbFlag ;//ģ��ʱ�ӹ��ϱ�־ 0xFF�� 0xBF ��
extern     INT8U g_RealClockTrbFlag;//ʵ��ʱ�ӹ��ϱ�־
extern     INT8U g_SimChipTrbFlag;//ģ�����оƬ���ϱ�־ 0xFF�� 0xBF ��
extern     INT8U g_RealChipTrbFlag;//ʵ�ʼ���оƬ���ϱ�־
extern     INT8U g_SimOCPTrbFlag;//ģ��������ϱ�־ 0xFF�� 0xBF ��
extern     INT8U g_RealOCPTrbFlag;//ʵ�ʹ������ϱ�־
extern     INT8U       nWuCh;                //���%��
extern     INT8U       nZFWuCh;             //�������
extern     INT8U g_ACOffset;
/**************************************************************************************************
                                             ��������
***************************************************************************************************/
void AcColTask(void *pData);

BOOLEAN     CkeckACCalPrm(void);
void        AcCalDataInit(void);
INT8S       ReadAcPluPrm(void);
INT8S       AcCalcJbParamInit(pMEM_AC_CAL_PRM pJB_Param);
INT8S       AcCalcJust7022(pMEM_AC_CAL_PRM pJB_Param);
BOOLEAN     AcCalcInit(void);
FP32        AcValueSwitch(INT32U uConstant, INT32U uHFConst, INT8U uR_ID, INT32U uData);
void        AcCalcProc(void);
void        AcCalcRead7022(void);
void        AcCalcBaseReal(void);
void        AcCalcEnergy(void);
void AcCalcEventProc(void);
void AcCalaClrProc(void);
void AcCalcClrMeterEve(const INT8U* pData);
void AcCalcDataInitProc(void);
void AcCalcClrDemandEve(const INT8U* pData);
void AcCalcDemInitProc(void);
void AcCalcClrEventEve(const INT8U* pData);
INT8S AcCalcEventInitProc(const INT8U* pData);
INT32S  AcDayDataClr(void);
INT32S  AcDayRDataClr(void);
void        AcCalcVoltPhaseErr(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcNoVoltage(INT8U nFlag, pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcPowerFail(INT8U nFlag, pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcLossOfVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcUnderVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcLossOfPhase(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcLossOfCurrent(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcOverVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcOverOfCurrent(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcOverload(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcReflow(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcCurrentFailure(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);

void        OpenMeterCover();
void        TripEvent();
void        SwitchOnEvent();
void        SecretKeyEvent();
void        ClockTrbEvent();
void        LoadSwitchActEvent();
void        PowerAbnlEvent();
void        ChipTrbEvent();
void        OverCurrentEvent();//�����¼�
void        GB698GetStartEventResp(INT8U nFn,INT8U nFn2);
void        GB698GetFinshEventResp(INT8U nFn,INT8U nFn2);

FP32        AcCalcReadValid(INT32U uConstant, INT32U uHFConst, INT8U uR_ID, INT32U uData);
INT32U      CheckINT32USum(const INT32U *pData, INT16U nStart, INT16U nLen);

INT8S       ReadAcENData(void);
INT8S       WriteAcENData(void);
INT8S   ReadAcDEData(void);
INT8S   WriteAcDEData(void);
INT8S AcCalcPerChanged(const pSTD_TIME pTime);
INT8S    AcCalcDemand(const pSTD_TIME pTime);

void ACMinCalcProc(void);
void AcCalcPerDataRdyProc(const pSTD_TIME pTime);
void AcMinFreezeDataRdyProc(const pSTD_TIME pTime);
void AcCalcDayDataRdyProc(const pSTD_TIME pTime);
void AcCalcRDayDataRdyProc(const pSTD_TIME pTime);
void AcCalcMeterPrmChanEve(const INT8U* pData, INT8U nType);
void AcCalcMonDataRdyProc(const pSTD_TIME pTime);
void CombEnergyCal(pFORMAT11 pEnergy, INT16S *pPulse, INT16U nPulseCnt);

void BackupAcData(void);
FP32 AcCalcChangePg(INT8U uPhase, FP32 fPg, FP32 fY, INT8U uPFlag);

INT8S ReadAcCombPrm(void);

INT8S       FirstWriteAcENData(void);

INT8S AcCalcReset7022(pMEM_AC_CAL_PRM pJB_Param);

void AcCalcJustData1(void);

void AcCalcJustData2(void);

void AcCalcJustData3(void);

void AcCalcJustData4(void);

void AcCalcJustData5(void);

void AcCalcJustData6(void);
#endif

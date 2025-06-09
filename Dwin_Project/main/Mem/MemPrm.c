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
#include "MemPrm.h"
#include "MemDrv.h"
/**************************************************************************************************
                                        �����洢ƫ����Ϣ
********************************************************************************************/
//1ͨѶ�����洢�ṹ
const MEM_OFF_INFO g_sTermCommOff[] =
{
	MEM_OFF_INFO_DEFINE(F94, pMEM_TERM_COMM_PRM, nAreaCode[0]),	         	    //F94��������͵�ַ
    MEM_OFF_INFO_DEFINE(F95, pMEM_TERM_COMM_PRM, nAreaCodeTmp[0]),	         	//F95�������
    MEM_OFF_INFO_DEFINE(F85, pMEM_TERM_COMM_PRM, nCarrZbSelect),	         	//F85�ز�ר��ѡ��
    MEM_OFF_INFO_DEFINE(F1, pMEM_TERM_COMM_PRM, nRadioDlyMs),	         	    //F1ͨѶ����
	MEM_OFF_INFO_DEFINE(F3, pMEM_TERM_COMM_PRM, nMainIP[0]),	             	//F3��վIP��ַ�Ͷ˿�
	MEM_OFF_INFO_DEFINE(F4, pMEM_TERM_COMM_PRM, nGsmCdma[0]),				//F4��վ�绰����Ͷ������ĺ���
    MEM_OFF_INFO_DEFINE(F7, pMEM_TERM_COMM_PRM, nTerminalIp[0]),		     	//F7�ն�IP��ַ�Ͷ˿�
    MEM_OFF_INFO_DEFINE(F8, pMEM_TERM_COMM_PRM, nWorkMode),		          	    //F8�ն�����ͨ�Ź�����ʽ����̫ר��������ר����
    MEM_OFF_INFO_DEFINE(F16, pMEM_TERM_COMM_PRM, nVirtuNetUser[0]),             //F16����ר���û���������
	MEM_OFF_INFO_DEFINE(F36, pMEM_TERM_COMM_PRM, nFlowCommun[0]),          	    //F36ͨ������
	
	{FEND, 1, 0, sizeof(MEM_TERM_COMM_PRM), 1}
};

//2�������洢�ṹ
const MEM_OFF_INFO g_sMeterOff[] =
{
	MEM_OFF_INFO_DEFINE(F10, pMEM_METER_EXT_PRM, sMeterPrm),	                //F10������
    //MEM_OFF_INFO_DEFINE(F11, pMEM_METER_EXT_PRM, sPulsePrm),	                //F11�������
	MEM_OFF_INFO_DEFINE(F25, pMEM_METER_EXT_PRM, sMtBasePrm),	                //F25��������
	
	{FEND, 1, 0, sizeof(MEM_METER_EXT_PRM), 0}
};

//3�ܼ�������洢�ṹ
const MEM_OFF_INFO g_sGrpOff[] =
{
    MEM_OFF_INFO_DEFINE_EXT(F11, MAX_PULSE_NUM, pMEM_GRP_PRM, sPulsePrm[0]),	                    //F11�������ò���
    
    MEM_OFF_INFO_DEFINE(F12, pMEM_GRP_PRM, nStats),	                    		//F12�ն�״̬�����������
	MEM_OFF_INFO_DEFINE(F14, pMEM_GRP_PRM, nGrpSeq),	                    	//F14�ܼ������
	
	MEM_OFF_INFO_DEFINE(F45, pMEM_GRP_PRM, nWattTurnBS),	                    //F45�����ִ�
	MEM_OFF_INFO_DEFINE(F43, pMEM_GRP_PRM, nWattCtrlSlip),	                    //F43����
	MEM_OFF_INFO_DEFINE(F41, pMEM_GRP_PRM, nPeriodPrjBS),	                    	//F41ʱ�οز���
	MEM_OFF_INFO_DEFINE(F42, pMEM_GRP_PRM, nFactoryDef[0]),	                    //F42���ݿز���
	MEM_OFF_INFO_DEFINE(F44, pMEM_GRP_PRM, sBusinessTimeS),	                    //F44��ͣ�ز���

	MEM_OFF_INFO_DEFINE(F48, pMEM_GRP_PRM, nEnergyTurnBS),	                    //F48����ִ�
	MEM_OFF_INFO_DEFINE(F46, pMEM_GRP_PRM, nEnergyDef[0]),	                    //F46�µ�ز���
	MEM_OFF_INFO_DEFINE(F47, pMEM_GRP_PRM, nBuyEnergyNo[0]),	                    //F47����ز���

	{FEND, 1, 0, sizeof(MEM_GRP_PRM), 0}
};

//4�ն����в����洢�ṹ
const MEM_OFF_INFO g_sRdMeterOff[] =
{
	MEM_OFF_INFO_DEFINE_EXT(F33, MAX_COM_NUM, pMEM_RDMETER_PRM, sComPrm),	                    	//F33�ն����в���

	{FEND, 1, 0, sizeof(MEM_GRP_PRM), 0}
};

//5��С������洢�ṹ
const MEM_OFF_INFO g_sMeterTypeIOff[] =
{
	MEM_OFF_INFO_DEFINE(F38, pMEM_METER_TYPE_PRM, sMeterTypeInfoPrm[0]),	    	//F38�ն����в���

	{FEND, 1, 0, sizeof(MEM_METER_TYPE_PRM), 0}
};

//6��С������洢�ṹ
const MEM_OFF_INFO g_sMeterTypeIIOff[] =
{
	MEM_OFF_INFO_DEFINE(F39, pMEM_METER_TYPE_PRM, sMeterTypeInfoPrm[0]),	    	//F39�ն����в���

	{FEND, 1, 0, sizeof(MEM_METER_TYPE_PRM), 0}
};

//7һ����������洢�ṹ
const MEM_OFF_INFO g_sTaskIOff[] =
{
	MEM_OFF_INFO_DEFINE(F67, pMEM_TASK_PRM, nTaskValid),	    					//F67��������
	MEM_OFF_INFO_DEFINE(F65, pMEM_TASK_PRM, nCycleUnit),	    					//F65����

	{FEND, 1, 0, sizeof(MEM_TASK_PRM), 0}
};

//8������������洢�ṹ
const MEM_OFF_INFO g_sTaskIIOff[] =
{
	MEM_OFF_INFO_DEFINE(F68, pMEM_TASK_PRM, nTaskValid),	    					//F67��������
	MEM_OFF_INFO_DEFINE(F66, pMEM_TASK_PRM, nCycleUnit),	    					//F65����

	{FEND, 1, 0, sizeof(MEM_TASK_PRM), 0}
};

//9����������洢�ṹ
const MEM_OFF_INFO g_sCtrlOff[] =
{
	MEM_OFF_INFO_DEFINE_EXT(F1, MAX_TURN_NUM, pMEM_CTRL_PRM, sCtrlTurnPrm[0]),	    			//F1 ң�ز���
	MEM_OFF_INFO_DEFINE(F9, pMEM_CTRL_PRM, sCtrlGrpPrm[0]),	    				//F9~F16 ����Ͷ��
    MEM_OFF_INFO_DEFINE_EXT(F25, 1, pMEM_CTRL_PRM, sKeepPowerPrm),	    			//(F25/F33)����Ͷ��/���
    MEM_OFF_INFO_DEFINE(F26, pMEM_CTRL_PRM, nPressValid),	    				//(F26/F34)�߷�Ͷ��/���
    MEM_OFF_INFO_DEFINE(F27, pMEM_CTRL_PRM, nCommValid),	    				//(F27/F35)�ն�����վͨ��Ͷ��/���
    MEM_OFF_INFO_DEFINE(F28, pMEM_CTRL_PRM, nRejectValid),	    				//(F28/F36)�޳�Ͷ��/���
    MEM_OFF_INFO_DEFINE(F29, pMEM_CTRL_PRM, nReportValid),	    				//(F29/F37)�����ϱ�Ͷ��/���
    MEM_OFF_INFO_DEFINE(F149, pMEM_CTRL_PRM, nSelfFindMeterValid),	    		//(F149)�Զ�ά������

	{FEND, 1, 0, sizeof(MEM_CTRL_PRM), 0}
};


//10���������洢�ṹ
const MEM_OFF_INFO g_sOtherPrmOff[] =
{
	MEM_OFF_INFO_DEFINE(F37, pMEM_OTHER_PRM, nSlaveCommPort),	            	//F37��������

	MEM_OFF_INFO_DEFINE(F5,  pMEM_OTHER_PRM, nSafeNo),	            			//F5��Ϣ��֤
	MEM_OFF_INFO_DEFINE(F6,  pMEM_OTHER_PRM, nTermGrpAddr[0][0]),	            //F6���ַ
	MEM_OFF_INFO_DEFINE(F9,  pMEM_OTHER_PRM, nErcRecordBS),	            		//F9�¼���¼
	MEM_OFF_INFO_DEFINE(F12, pMEM_OTHER_PRM, nStateVarFlgs),	            	//F12״̬��
	MEM_OFF_INFO_DEFINE(F29, pMEM_OTHER_PRM, nTermDispMeterAddr[0]),	        //F29�ն˵�����ʾ���ܱ��
	MEM_OFF_INFO_DEFINE(F30, pMEM_OTHER_PRM, nStopRdMeter),	            		//F30ͣ����־
	MEM_OFF_INFO_DEFINE(F150,pMEM_OTHER_PRM, nMeterValidBS),	            	//F150�������־
	MEM_OFF_INFO_DEFINE(F61, pMEM_OTHER_PRM, nAnlogBS),	            			//F61ֱ��ģ��������
	MEM_OFF_INFO_DEFINE(F81, pMEM_OTHER_PRM, nAnlogValS[0]),	            	//F81ֱ��ģ����������ʼ
	MEM_OFF_INFO_DEFINE(F82, pMEM_OTHER_PRM, nAnlogLimitUp[0]),	            	//F82ֱ��ģ��������
	MEM_OFF_INFO_DEFINE(F83, pMEM_OTHER_PRM, nAnlogFrez),	            			//F83ֱ��ģ�����������

	MEM_OFF_INFO_DEFINE(F59, pMEM_OTHER_PRM, nMeterDiff1),	            		//F59�������ͣ��Уʱ
	
	MEM_OFF_INFO_DEFINE(F23, pMEM_OTHER_PRM, nPressFeeAla[0]),	            	//F23�߷Ѹ澯����M��ֹ��־λ
	MEM_OFF_INFO_DEFINE(F57, pMEM_OTHER_PRM, nSoundBS[0]),	            		//F57�����澯
	MEM_OFF_INFO_DEFINE(F58, pMEM_OTHER_PRM, nProtectMin),	            		//F58����ʱ��	

	MEM_OFF_INFO_DEFINE(F17, pMEM_OTHER_PRM, nWattSafeDef[0]),	            	//F17������ֵ
	MEM_OFF_INFO_DEFINE(F19, pMEM_OTHER_PRM, nWattDefCoeff),	            		//F19ʱ�οض�ֵ����ϵ��
	MEM_OFF_INFO_DEFINE(F18, pMEM_OTHER_PRM, nWattPeriodBS[0]),	            	//F18ʱ�ο�ʱ��
	MEM_OFF_INFO_DEFINE(F49, pMEM_OTHER_PRM, nTurnAlmTime[0]),	            	//F49���ظ澯ʱ��

	MEM_OFF_INFO_DEFINE(F20, pMEM_OTHER_PRM, nEnergyDefCoeff),	            	//F20�µ�ض�ֵ����ϵ��
	//MEM_OFF_INFO_DEFINE(F21, pMEM_OTHER_PRM, nEnergyPeriodBS[0]),	            	//F21�µ��ʱ��
    MEM_OFF_INFO_DEFINE(F21, pMEM_OTHER_PRM, sPowerPerd[0]),	            	//F21�µ��ʱ��
	MEM_OFF_INFO_DEFINE(F22, pMEM_OTHER_PRM, nEnergyMFeeNum2),	            	//F22�µ�ط��ʲ���	
	
	//MEM_OFF_INFO_DEFINE(F11, pMEM_OTHER_PRM, sPulsePrm[0]),		        		//F11�������
    MEM_OFF_INFO_DEFINE(F13, pMEM_OTHER_PRM, sAnlogPrm[0]),		        		//F13ģ����
    MEM_OFF_INFO_DEFINE(F15, pMEM_OTHER_PRM, sDiffPrm[0]),              			//F15�й��ܵ������Խ���¼���������

    MEM_OFF_INFO_DEFINE(F35, pMEM_OTHER_PRM, nImMeterNum),	            		//F35�ص������
	MEM_OFF_INFO_DEFINE(F2,  pMEM_OTHER_PRM, nRelayTermNum),	            		//F2�м���Ŀ
    MEM_OFF_INFO_DEFINE(F31, pMEM_OTHER_PRM, nColNodeNum),	            		//F31�ӽڵ㸽���ڵ���Ŀ
	

	{FEND, 1, 0, sizeof(MEM_OTHER_PRM), 0}
};

//12У������洢�ṹ
const MEM_OFF_INFO g_sAcCalPrmOff[] =
{
	MEM_OFF_INFO_DEFINE(F1, pMEM_AC_CAL_PRM, nModeCfg),	    							   //F1ģʽ��ؿ���                                           
    MEM_OFF_INFO_DEFINE(F2, pMEM_AC_CAL_PRM, nPGActrl),	    							   //F2ADC��������                                            
    MEM_OFF_INFO_DEFINE(F3, pMEM_AC_CAL_PRM, nEMUCfg),	    							   //F3EMU��Ԫ����                                            
    MEM_OFF_INFO_DEFINE(F4, pMEM_AC_CAL_PRM, nPgain[0]),	    						   //F4�й���������                                           
    MEM_OFF_INFO_DEFINE(F5, pMEM_AC_CAL_PRM, nQgain[0]),	    						   //F5�޹���������                                           
    MEM_OFF_INFO_DEFINE(F6, pMEM_AC_CAL_PRM, nSgain[0]),	    						   //F6���ڹ�������                                           
    MEM_OFF_INFO_DEFINE(F7, pMEM_AC_CAL_PRM, nPhSregpq0[0]),	    					   //F7��λУ��0                                              
    MEM_OFF_INFO_DEFINE(F8, pMEM_AC_CAL_PRM, nPhSregpq1[0]),	    					   //F8��λУ��1                                              
    MEM_OFF_INFO_DEFINE(F9, pMEM_AC_CAL_PRM, nPoffset[0]),	    						   //F9�й�����offsetУ��                                     
    MEM_OFF_INFO_DEFINE(F10, pMEM_AC_CAL_PRM, nQPhscal),	    						   //F10�޹���λУ��                                           
    MEM_OFF_INFO_DEFINE(F11, pMEM_AC_CAL_PRM, nUgain[0]),	    						   //F11��ѹ����                                               
    MEM_OFF_INFO_DEFINE(F12, pMEM_AC_CAL_PRM, nIgain[0]),	    						   //F12��������                                               
    MEM_OFF_INFO_DEFINE(F13, pMEM_AC_CAL_PRM, nIstarup),	    						   //F13�𶯵�����ֵ����                                       
    MEM_OFF_INFO_DEFINE(F14, pMEM_AC_CAL_PRM, nHfconst),	    						   //F14��Ƶ�����������                                       
    MEM_OFF_INFO_DEFINE(F15, pMEM_AC_CAL_PRM, nFailVoltage),	    					   //F15ʧѹ��ֵ����                                           
    MEM_OFF_INFO_DEFINE(F16, pMEM_AC_CAL_PRM, nGainADC7),	    						   //F16����·ADC�����ź�����                                  
    MEM_OFF_INFO_DEFINE(F17, pMEM_AC_CAL_PRM, nQoffset[0]),	    						   //F17�޹�����offsetУ��                                     
    MEM_OFF_INFO_DEFINE(F18, pMEM_AC_CAL_PRM, nURmsoffse[0]),	    					   //F18��ѹ��ЧֵoffsetУ��                                   
    MEM_OFF_INFO_DEFINE(F19, pMEM_AC_CAL_PRM, nIRmsoffse[0]),	    					   //F19������ЧֵoffsetУ��                                   
    MEM_OFF_INFO_DEFINE(F20, pMEM_AC_CAL_PRM, nUoffset[0]),	    					       //F20��ѹͨ��ADC offsetУ��                                 
    MEM_OFF_INFO_DEFINE(F21, pMEM_AC_CAL_PRM, nIoffset[0]),	    						   //F21����ͨ��ADC offsetУ��                                 
    MEM_OFF_INFO_DEFINE(F22, pMEM_AC_CAL_PRM, nEMUIE),	    							   //F22�ж�ʹ��                                               
    MEM_OFF_INFO_DEFINE(F23, pMEM_AC_CAL_PRM, nModuleCFG),	    						   //F23��·ģ�����üĴ���                                     
    MEM_OFF_INFO_DEFINE(F24, pMEM_AC_CAL_PRM, nAllGain),	    						   //F24ȫͨ�����棬����Vref���¶�У��                         
    MEM_OFF_INFO_DEFINE(F25, pMEM_AC_CAL_PRM, nHFDouble),	    						   //F25���峣���ӱ�ѡ��                                       
    MEM_OFF_INFO_DEFINE(F26, pMEM_AC_CAL_PRM, nLineGain),	    						   //F26��������У��                                           
    MEM_OFF_INFO_DEFINE(F27, pMEM_AC_CAL_PRM, nPineCtrl),	    						   //F27����Pin����������ѡ�����                              
    MEM_OFF_INFO_DEFINE(F28, pMEM_AC_CAL_PRM, nPstart),	    							   //F28�𶯹������üĴ���                                     
    MEM_OFF_INFO_DEFINE(F29, pMEM_AC_CAL_PRM, nIregion),	    						   //F29��λ�����������üĴ���                                 
    MEM_OFF_INFO_DEFINE(F30, pMEM_AC_CAL_PRM, nReserved[0]),	    					   //F30reserved                                               
                                                                                                                                                    
    MEM_OFF_INFO_DEFINE(F31, pMEM_AC_CAL_PRM, nCheckSumEntire),	    					   // F31����У��Ĵ������ۼӺ�                                
    MEM_OFF_INFO_DEFINE(F32, pMEM_AC_CAL_PRM, nCheckSumReset),	    					   // F32Ĭ��У��Ĵ���ֵ                                      
    //MEM_OFF_INFO_DEFINE(F33, pMEM_AC_CAL_PRM, nChkCode),	    					       // F33Checksum code                                         

    {FEND, 1, 0, sizeof(MEM_AC_CAL_PRM), 1}
};

//13���ɲ���
const MEM_OFF_INFO g_sAcPrmOff[] =
{
	MEM_OFF_INFO_DEFINE(F1, pMEM_AC_PRM, nDemandPeriod),         	
    MEM_OFF_INFO_DEFINE(F2, pMEM_AC_PRM, nDispTime1),            
    MEM_OFF_INFO_DEFINE(F3, pMEM_AC_PRM, nDispNum1),             
    MEM_OFF_INFO_DEFINE(F4, pMEM_AC_PRM, nDispNum2),             
    MEM_OFF_INFO_DEFINE(F5, pMEM_AC_PRM, nDispTime2),            
    MEM_OFF_INFO_DEFINE(F6, pMEM_AC_PRM, nDispPoint1),           
    MEM_OFF_INFO_DEFINE(F7, pMEM_AC_PRM, nDispPoint2),           
    MEM_OFF_INFO_DEFINE(F8, pMEM_AC_PRM, nCurveDay),            
    MEM_OFF_INFO_DEFINE(F9, pMEM_AC_PRM, sPBaseEnergy),          
    MEM_OFF_INFO_DEFINE(F10, pMEM_AC_PRM, sQBaseEnergy),
              
    MEM_OFF_INFO_DEFINE(F11, pMEM_AC_PRM, sYearChangeTime),                   
    MEM_OFF_INFO_DEFINE(F12, pMEM_AC_PRM, sDayChangeTime),                    
    MEM_OFF_INFO_DEFINE(F13, pMEM_AC_PRM, nYearSegmNum),                      
    MEM_OFF_INFO_DEFINE(F14, pMEM_AC_PRM, nDayTableNum),                      
    MEM_OFF_INFO_DEFINE(F15, pMEM_AC_PRM, nDaySegmNum),                       
    MEM_OFF_INFO_DEFINE(F16, pMEM_AC_PRM, nFeeNumber),                        
    MEM_OFF_INFO_DEFINE(F17, pMEM_AC_PRM, nHolidayNumber),                    
    MEM_OFF_INFO_DEFINE(F18, pMEM_AC_PRM, nHarmcNumber),                      
    MEM_OFF_INFO_DEFINE(F19, pMEM_AC_PRM, nMeterAddr[0]),        
    MEM_OFF_INFO_DEFINE(F20, pMEM_AC_PRM, nMeterNumber[0]),      
    MEM_OFF_INFO_DEFINE(F21, pMEM_AC_PRM, nMeterProNum[0]),      
    MEM_OFF_INFO_DEFINE(F22, pMEM_AC_PRM, nEpCompFlag),                       
    MEM_OFF_INFO_DEFINE(F23, pMEM_AC_PRM, nEq1CompFlag),                      
    MEM_OFF_INFO_DEFINE(F24, pMEM_AC_PRM, nEq2CompFlag),                      
    MEM_OFF_INFO_DEFINE(F25, pMEM_AC_PRM, nIR1Baud),                          
    MEM_OFF_INFO_DEFINE(F26, pMEM_AC_PRM, nIR2Baud),                          
    MEM_OFF_INFO_DEFINE(F27, pMEM_AC_PRM, nCom1Baud),                         
    MEM_OFF_INFO_DEFINE(F28, pMEM_AC_PRM, nCom2Baud),                         
    MEM_OFF_INFO_DEFINE(F29, pMEM_AC_PRM, nCom3Baud),                         
    MEM_OFF_INFO_DEFINE(F30, pMEM_AC_PRM, nHolidayFlag),                      
    MEM_OFF_INFO_DEFINE(F31, pMEM_AC_PRM, nHolidayTable),                     
    MEM_OFF_INFO_DEFINE(F32, pMEM_AC_PRM, sMonthJS1),                         
    MEM_OFF_INFO_DEFINE(F33, pMEM_AC_PRM, sMonthJS2),                         
    MEM_OFF_INFO_DEFINE(F34, pMEM_AC_PRM, sMonthJS3),                         
    MEM_OFF_INFO_DEFINE(F35, pMEM_AC_PRM, nMeterPWD[0][0]), 
    MEM_OFF_INFO_DEFINE(F36, pMEM_AC_PRM, sPosPowLimt),
    MEM_OFF_INFO_DEFINE(F37, pMEM_AC_PRM, sNegPowLimt),
    
    MEM_OFF_INFO_DEFINE(F38, pMEM_AC_PRM, sUpLimtVolt),                                               
    MEM_OFF_INFO_DEFINE(F39, pMEM_AC_PRM, sLowLimtVolt),                                              
    MEM_OFF_INFO_DEFINE(F40, pMEM_AC_PRM, sYearSegmTable[0][0]),                      
    MEM_OFF_INFO_DEFINE(F41, pMEM_AC_PRM, sDaySegmTable[0][0][0]),     
    MEM_OFF_INFO_DEFINE(F42, pMEM_AC_PRM, sHolidayTable[0]),                           
    MEM_OFF_INFO_DEFINE(F43, pMEM_AC_PRM, sPeriodicityDJ),                                            
    //MEM_OFF_INFO_DEFINE(F44, pMEM_AC_PRM, nObjId),  
    MEM_OFF_INFO_DEFINE(F44, pMEM_AC_PRM, nHardType),
    MEM_OFF_INFO_DEFINE(F45, pMEM_AC_PRM, nCTRate),                                                   
    MEM_OFF_INFO_DEFINE(F46, pMEM_AC_PRM, nPTRate),                                                   
    MEM_OFF_INFO_DEFINE(F47, pMEM_AC_PRM, pTransR[0]),                                                
    MEM_OFF_INFO_DEFINE(F48, pMEM_AC_PRM, pTransX[0]),                                                
    MEM_OFF_INFO_DEFINE(F49, pMEM_AC_PRM, pTransG[0]),                                               
    MEM_OFF_INFO_DEFINE(F50, pMEM_AC_PRM, pTransB[0]),         
    MEM_OFF_INFO_DEFINE(F51, pMEM_AC_PRM, nDispModel),
    //MEM_OFF_INFO_DEFINE(F51, pMEM_AC_PRM, nHardType),
    
    {FEND, 1, 0, sizeof(MEM_AC_PRM), 1}
};

//14����������洢�ṹ
const MEM_OFF_INFO g_sExtOff[] =
{
	MEM_OFF_INFO_DEFINE(F1, pMEM_EXT_PRM, nTmp),	    							//F1 

	{FEND, 1, 0, sizeof(MEM_EXT_PRM), 0}
};


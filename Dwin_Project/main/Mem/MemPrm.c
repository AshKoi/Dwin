/******************************************************************************************************
* 2013-2014 益邦科技有限公司
* 文件名: 
* 创建人: 王一凡
* 日　期: 2013/09/21
* 修改人: 
* 日　期: 
* 描　述: 
* 版　本: V1.0
*****************************************************************************************************/
#include "MemPrm.h"
#include "MemDrv.h"
/**************************************************************************************************
                                        参数存储偏移信息
********************************************************************************************/
//1通讯参数存储结构
const MEM_OFF_INFO g_sTermCommOff[] =
{
	MEM_OFF_INFO_DEFINE(F94, pMEM_TERM_COMM_PRM, nAreaCode[0]),	         	    //F94行政区码和地址
    MEM_OFF_INFO_DEFINE(F95, pMEM_TERM_COMM_PRM, nAreaCodeTmp[0]),	         	//F95生产编号
    MEM_OFF_INFO_DEFINE(F85, pMEM_TERM_COMM_PRM, nCarrZbSelect),	         	//F85载波专变选择
    MEM_OFF_INFO_DEFINE(F1, pMEM_TERM_COMM_PRM, nRadioDlyMs),	         	    //F1通讯参数
	MEM_OFF_INFO_DEFINE(F3, pMEM_TERM_COMM_PRM, nMainIP[0]),	             	//F3主站IP地址和端口
	MEM_OFF_INFO_DEFINE(F4, pMEM_TERM_COMM_PRM, nGsmCdma[0]),				//F4主站电话号码和短信中心号码
    MEM_OFF_INFO_DEFINE(F7, pMEM_TERM_COMM_PRM, nTerminalIp[0]),		     	//F7终端IP地址和端口
    MEM_OFF_INFO_DEFINE(F8, pMEM_TERM_COMM_PRM, nWorkMode),		          	    //F8终端上行通信工作方式（以太专网或虚拟专网）
    MEM_OFF_INFO_DEFINE(F16, pMEM_TERM_COMM_PRM, nVirtuNetUser[0]),             //F16虚拟专网用户名、密码
	MEM_OFF_INFO_DEFINE(F36, pMEM_TERM_COMM_PRM, nFlowCommun[0]),          	    //F36通信流量
	
	{FEND, 1, 0, sizeof(MEM_TERM_COMM_PRM), 1}
};

//2电表参数存储结构
const MEM_OFF_INFO g_sMeterOff[] =
{
	MEM_OFF_INFO_DEFINE(F10, pMEM_METER_EXT_PRM, sMeterPrm),	                //F10电表参数
    //MEM_OFF_INFO_DEFINE(F11, pMEM_METER_EXT_PRM, sPulsePrm),	                //F11脉冲参数
	MEM_OFF_INFO_DEFINE(F25, pMEM_METER_EXT_PRM, sMtBasePrm),	                //F25基本参数
	
	{FEND, 1, 0, sizeof(MEM_METER_EXT_PRM), 0}
};

//3总加组参数存储结构
const MEM_OFF_INFO g_sGrpOff[] =
{
    MEM_OFF_INFO_DEFINE_EXT(F11, MAX_PULSE_NUM, pMEM_GRP_PRM, sPulsePrm[0]),	                    //F11脉冲配置参数
    
    MEM_OFF_INFO_DEFINE(F12, pMEM_GRP_PRM, nStats),	                    		//F12终端状态量输入参数数
	MEM_OFF_INFO_DEFINE(F14, pMEM_GRP_PRM, nGrpSeq),	                    	//F14总加组参数
	
	MEM_OFF_INFO_DEFINE(F45, pMEM_GRP_PRM, nWattTurnBS),	                    //F45功控轮次
	MEM_OFF_INFO_DEFINE(F43, pMEM_GRP_PRM, nWattCtrlSlip),	                    //F43滑差
	MEM_OFF_INFO_DEFINE(F41, pMEM_GRP_PRM, nPeriodPrjBS),	                    	//F41时段控参数
	MEM_OFF_INFO_DEFINE(F42, pMEM_GRP_PRM, nFactoryDef[0]),	                    //F42厂休控参数
	MEM_OFF_INFO_DEFINE(F44, pMEM_GRP_PRM, sBusinessTimeS),	                    //F44报停控参数

	MEM_OFF_INFO_DEFINE(F48, pMEM_GRP_PRM, nEnergyTurnBS),	                    //F48电控轮次
	MEM_OFF_INFO_DEFINE(F46, pMEM_GRP_PRM, nEnergyDef[0]),	                    //F46月电控参数
	MEM_OFF_INFO_DEFINE(F47, pMEM_GRP_PRM, nBuyEnergyNo[0]),	                    //F47购电控参数

	{FEND, 1, 0, sizeof(MEM_GRP_PRM), 0}
};

//4终端运行参数存储结构
const MEM_OFF_INFO g_sRdMeterOff[] =
{
	MEM_OFF_INFO_DEFINE_EXT(F33, MAX_COM_NUM, pMEM_RDMETER_PRM, sComPrm),	                    	//F33终端运行参数

	{FEND, 1, 0, sizeof(MEM_GRP_PRM), 0}
};

//5大小类参数存储结构
const MEM_OFF_INFO g_sMeterTypeIOff[] =
{
	MEM_OFF_INFO_DEFINE(F38, pMEM_METER_TYPE_PRM, sMeterTypeInfoPrm[0]),	    	//F38终端运行参数

	{FEND, 1, 0, sizeof(MEM_METER_TYPE_PRM), 0}
};

//6大小类参数存储结构
const MEM_OFF_INFO g_sMeterTypeIIOff[] =
{
	MEM_OFF_INFO_DEFINE(F39, pMEM_METER_TYPE_PRM, sMeterTypeInfoPrm[0]),	    	//F39终端运行参数

	{FEND, 1, 0, sizeof(MEM_METER_TYPE_PRM), 0}
};

//7一类任务参数存储结构
const MEM_OFF_INFO g_sTaskIOff[] =
{
	MEM_OFF_INFO_DEFINE(F67, pMEM_TASK_PRM, nTaskValid),	    					//F67启动参数
	MEM_OFF_INFO_DEFINE(F65, pMEM_TASK_PRM, nCycleUnit),	    					//F65参数

	{FEND, 1, 0, sizeof(MEM_TASK_PRM), 0}
};

//8二类任务参数存储结构
const MEM_OFF_INFO g_sTaskIIOff[] =
{
	MEM_OFF_INFO_DEFINE(F68, pMEM_TASK_PRM, nTaskValid),	    					//F67启动参数
	MEM_OFF_INFO_DEFINE(F66, pMEM_TASK_PRM, nCycleUnit),	    					//F65参数

	{FEND, 1, 0, sizeof(MEM_TASK_PRM), 0}
};

//9控制类参数存储结构
const MEM_OFF_INFO g_sCtrlOff[] =
{
	MEM_OFF_INFO_DEFINE_EXT(F1, MAX_TURN_NUM, pMEM_CTRL_PRM, sCtrlTurnPrm[0]),	    			//F1 遥控参数
	MEM_OFF_INFO_DEFINE(F9, pMEM_CTRL_PRM, sCtrlGrpPrm[0]),	    				//F9~F16 控制投入
    MEM_OFF_INFO_DEFINE_EXT(F25, 1, pMEM_CTRL_PRM, sKeepPowerPrm),	    			//(F25/F33)保电投入/解除
    MEM_OFF_INFO_DEFINE(F26, pMEM_CTRL_PRM, nPressValid),	    				//(F26/F34)催肥投入/解除
    MEM_OFF_INFO_DEFINE(F27, pMEM_CTRL_PRM, nCommValid),	    				//(F27/F35)终端与主站通话投入/解除
    MEM_OFF_INFO_DEFINE(F28, pMEM_CTRL_PRM, nRejectValid),	    				//(F28/F36)剔除投入/解除
    MEM_OFF_INFO_DEFINE(F29, pMEM_CTRL_PRM, nReportValid),	    				//(F29/F37)主动上报投入/解除
    MEM_OFF_INFO_DEFINE(F149, pMEM_CTRL_PRM, nSelfFindMeterValid),	    		//(F149)自动维护功能

	{FEND, 1, 0, sizeof(MEM_CTRL_PRM), 0}
};


//10其他参数存储结构
const MEM_OFF_INFO g_sOtherPrmOff[] =
{
	MEM_OFF_INFO_DEFINE(F37, pMEM_OTHER_PRM, nSlaveCommPort),	            	//F37级联参数

	MEM_OFF_INFO_DEFINE(F5,  pMEM_OTHER_PRM, nSafeNo),	            			//F5消息认证
	MEM_OFF_INFO_DEFINE(F6,  pMEM_OTHER_PRM, nTermGrpAddr[0][0]),	            //F6组地址
	MEM_OFF_INFO_DEFINE(F9,  pMEM_OTHER_PRM, nErcRecordBS),	            		//F9事件记录
	MEM_OFF_INFO_DEFINE(F12, pMEM_OTHER_PRM, nStateVarFlgs),	            	//F12状态量
	MEM_OFF_INFO_DEFINE(F29, pMEM_OTHER_PRM, nTermDispMeterAddr[0]),	        //F29终端当地显示电能表号
	MEM_OFF_INFO_DEFINE(F30, pMEM_OTHER_PRM, nStopRdMeter),	            		//F30停抄标志
	MEM_OFF_INFO_DEFINE(F150,pMEM_OTHER_PRM, nMeterValidBS),	            	//F150测量点标志
	MEM_OFF_INFO_DEFINE(F61, pMEM_OTHER_PRM, nAnlogBS),	            			//F61直流模拟量接入
	MEM_OFF_INFO_DEFINE(F81, pMEM_OTHER_PRM, nAnlogValS[0]),	            	//F81直流模拟量量程起始
	MEM_OFF_INFO_DEFINE(F82, pMEM_OTHER_PRM, nAnlogLimitUp[0]),	            	//F82直流模拟量上限
	MEM_OFF_INFO_DEFINE(F83, pMEM_OTHER_PRM, nAnlogFrez),	            			//F83直流模拟量冻结参数

	MEM_OFF_INFO_DEFINE(F59, pMEM_OTHER_PRM, nMeterDiff1),	            		//F59超差飞走停走校时
	
	MEM_OFF_INFO_DEFINE(F23, pMEM_OTHER_PRM, nPressFeeAla[0]),	            	//F23催费告警允许∕禁止标志位
	MEM_OFF_INFO_DEFINE(F57, pMEM_OTHER_PRM, nSoundBS[0]),	            		//F57声音告警
	MEM_OFF_INFO_DEFINE(F58, pMEM_OTHER_PRM, nProtectMin),	            		//F58保电时间	

	MEM_OFF_INFO_DEFINE(F17, pMEM_OTHER_PRM, nWattSafeDef[0]),	            	//F17保安定值
	MEM_OFF_INFO_DEFINE(F19, pMEM_OTHER_PRM, nWattDefCoeff),	            		//F19时段控定值浮动系数
	MEM_OFF_INFO_DEFINE(F18, pMEM_OTHER_PRM, nWattPeriodBS[0]),	            	//F18时段控时段
	MEM_OFF_INFO_DEFINE(F49, pMEM_OTHER_PRM, nTurnAlmTime[0]),	            	//F49功控告警时间

	MEM_OFF_INFO_DEFINE(F20, pMEM_OTHER_PRM, nEnergyDefCoeff),	            	//F20月电控定值浮动系数
	//MEM_OFF_INFO_DEFINE(F21, pMEM_OTHER_PRM, nEnergyPeriodBS[0]),	            	//F21月电控时段
    MEM_OFF_INFO_DEFINE(F21, pMEM_OTHER_PRM, sPowerPerd[0]),	            	//F21月电控时段
	MEM_OFF_INFO_DEFINE(F22, pMEM_OTHER_PRM, nEnergyMFeeNum2),	            	//F22月电控费率参数	
	
	//MEM_OFF_INFO_DEFINE(F11, pMEM_OTHER_PRM, sPulsePrm[0]),		        		//F11脉冲参数
    MEM_OFF_INFO_DEFINE(F13, pMEM_OTHER_PRM, sAnlogPrm[0]),		        		//F13模拟量
    MEM_OFF_INFO_DEFINE(F15, pMEM_OTHER_PRM, sDiffPrm[0]),              			//F15有功总电能量差动越限事件参数设置

    MEM_OFF_INFO_DEFINE(F35, pMEM_OTHER_PRM, nImMeterNum),	            		//F35重点电表参数
	MEM_OFF_INFO_DEFINE(F2,  pMEM_OTHER_PRM, nRelayTermNum),	            		//F2中继数目
    MEM_OFF_INFO_DEFINE(F31, pMEM_OTHER_PRM, nColNodeNum),	            		//F31从节点附属节点数目
	

	{FEND, 1, 0, sizeof(MEM_OTHER_PRM), 0}
};

//12校表参数存储结构
const MEM_OFF_INFO g_sAcCalPrmOff[] =
{
	MEM_OFF_INFO_DEFINE(F1, pMEM_AC_CAL_PRM, nModeCfg),	    							   //F1模式相关控制                                           
    MEM_OFF_INFO_DEFINE(F2, pMEM_AC_CAL_PRM, nPGActrl),	    							   //F2ADC增益配置                                            
    MEM_OFF_INFO_DEFINE(F3, pMEM_AC_CAL_PRM, nEMUCfg),	    							   //F3EMU单元配置                                            
    MEM_OFF_INFO_DEFINE(F4, pMEM_AC_CAL_PRM, nPgain[0]),	    						   //F4有功功率增益                                           
    MEM_OFF_INFO_DEFINE(F5, pMEM_AC_CAL_PRM, nQgain[0]),	    						   //F5无功功率增益                                           
    MEM_OFF_INFO_DEFINE(F6, pMEM_AC_CAL_PRM, nSgain[0]),	    						   //F6视在功率增益                                           
    MEM_OFF_INFO_DEFINE(F7, pMEM_AC_CAL_PRM, nPhSregpq0[0]),	    					   //F7相位校正0                                              
    MEM_OFF_INFO_DEFINE(F8, pMEM_AC_CAL_PRM, nPhSregpq1[0]),	    					   //F8相位校正1                                              
    MEM_OFF_INFO_DEFINE(F9, pMEM_AC_CAL_PRM, nPoffset[0]),	    						   //F9有功功率offset校正                                     
    MEM_OFF_INFO_DEFINE(F10, pMEM_AC_CAL_PRM, nQPhscal),	    						   //F10无功相位校正                                           
    MEM_OFF_INFO_DEFINE(F11, pMEM_AC_CAL_PRM, nUgain[0]),	    						   //F11电压增益                                               
    MEM_OFF_INFO_DEFINE(F12, pMEM_AC_CAL_PRM, nIgain[0]),	    						   //F12电流增益                                               
    MEM_OFF_INFO_DEFINE(F13, pMEM_AC_CAL_PRM, nIstarup),	    						   //F13起动电流阀值设置                                       
    MEM_OFF_INFO_DEFINE(F14, pMEM_AC_CAL_PRM, nHfconst),	    						   //F14高频脉冲输出设置                                       
    MEM_OFF_INFO_DEFINE(F15, pMEM_AC_CAL_PRM, nFailVoltage),	    					   //F15失压阀值设置                                           
    MEM_OFF_INFO_DEFINE(F16, pMEM_AC_CAL_PRM, nGainADC7),	    						   //F16第七路ADC输入信号增益                                  
    MEM_OFF_INFO_DEFINE(F17, pMEM_AC_CAL_PRM, nQoffset[0]),	    						   //F17无功功率offset校正                                     
    MEM_OFF_INFO_DEFINE(F18, pMEM_AC_CAL_PRM, nURmsoffse[0]),	    					   //F18电压有效值offset校正                                   
    MEM_OFF_INFO_DEFINE(F19, pMEM_AC_CAL_PRM, nIRmsoffse[0]),	    					   //F19电流有效值offset校正                                   
    MEM_OFF_INFO_DEFINE(F20, pMEM_AC_CAL_PRM, nUoffset[0]),	    					       //F20电压通道ADC offset校正                                 
    MEM_OFF_INFO_DEFINE(F21, pMEM_AC_CAL_PRM, nIoffset[0]),	    						   //F21电流通道ADC offset校正                                 
    MEM_OFF_INFO_DEFINE(F22, pMEM_AC_CAL_PRM, nEMUIE),	    							   //F22中断使能                                               
    MEM_OFF_INFO_DEFINE(F23, pMEM_AC_CAL_PRM, nModuleCFG),	    						   //F23电路模块配置寄存器                                     
    MEM_OFF_INFO_DEFINE(F24, pMEM_AC_CAL_PRM, nAllGain),	    						   //F24全通道增益，用于Vref的温度校正                         
    MEM_OFF_INFO_DEFINE(F25, pMEM_AC_CAL_PRM, nHFDouble),	    						   //F25脉冲常数加倍选择                                       
    MEM_OFF_INFO_DEFINE(F26, pMEM_AC_CAL_PRM, nLineGain),	    						   //F26基波增益校正                                           
    MEM_OFF_INFO_DEFINE(F27, pMEM_AC_CAL_PRM, nPineCtrl),	    						   //F27数字Pin上下拉电阻选择控制                              
    MEM_OFF_INFO_DEFINE(F28, pMEM_AC_CAL_PRM, nPstart),	    							   //F28起动功率设置寄存器                                     
    MEM_OFF_INFO_DEFINE(F29, pMEM_AC_CAL_PRM, nIregion),	    						   //F29相位补偿区域设置寄存器                                 
    MEM_OFF_INFO_DEFINE(F30, pMEM_AC_CAL_PRM, nReserved[0]),	    					   //F30reserved                                               
                                                                                                                                                    
    MEM_OFF_INFO_DEFINE(F31, pMEM_AC_CAL_PRM, nCheckSumEntire),	    					   // F31所有校表寄存器的累加和                                
    MEM_OFF_INFO_DEFINE(F32, pMEM_AC_CAL_PRM, nCheckSumReset),	    					   // F32默认校表寄存器值                                      
    //MEM_OFF_INFO_DEFINE(F33, pMEM_AC_CAL_PRM, nChkCode),	    					       // F33Checksum code                                         

    {FEND, 1, 0, sizeof(MEM_AC_CAL_PRM), 1}
};

//13交采参数
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

//14外扩类参数存储结构
const MEM_OFF_INFO g_sExtOff[] =
{
	MEM_OFF_INFO_DEFINE(F1, pMEM_EXT_PRM, nTmp),	    							//F1 

	{FEND, 1, 0, sizeof(MEM_EXT_PRM), 0}
};


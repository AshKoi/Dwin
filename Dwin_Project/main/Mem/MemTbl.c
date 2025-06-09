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
#include "AppCfg.h"
#include "MemTbl.h"
#include "MemDrv.h"

/**************************************************************************************************
                                              �ڴ�ʵ��
***************************************************************************************************/
static RAM_MEM		g_sMem;
pRAM_MEM		    g_pMem = &g_sMem;


/**************************************************************************************************
                                        �����洢ƫ����Ϣ
***************************************************************************************************/
extern    const    MEM_OFF_INFO   g_sTermCommOff[];		        //�ն�ͨѶ����
extern    const    MEM_OFF_INFO   g_sMeterOff[];		        	//�ն˱�Ʋ���
extern	  const    MEM_OFF_INFO   g_sGrpOff[];						//�ܼ���
extern	  const    MEM_OFF_INFO   g_sRdMeterOff[];					//�ն�����
extern	  const    MEM_OFF_INFO   g_sMeterTypeIOff[];				//I���С��
extern	  const    MEM_OFF_INFO   g_sMeterTypeIIOff[];			//II���С��
extern	  const    MEM_OFF_INFO   g_sTaskIOff[];					//I������
extern	  const    MEM_OFF_INFO   g_sTaskIIOff[];					//II������
extern	  const    MEM_OFF_INFO   g_sCtrlOff[];					//���Ʋ���
extern	  const    MEM_OFF_INFO   g_sAcCalPrmOff[];						//����У�����
extern	  const    MEM_OFF_INFO   g_sAcPrmOff[];						//���ɲ���
extern   const    MEM_OFF_INFO   g_sOtherPrmOff[];				//��������
extern	  const    MEM_OFF_INFO   g_sExtOff[];						//���������


extern    const    MEM_OFF_INFO   g_sMpRealDataOff[];		        //ʵʱ����
extern    const    MEM_OFF_INFO   g_sMpDayDataOff[];		        //������
extern    const    MEM_OFF_INFO   g_sMpMonDataOff[];		        //������
extern    const    MEM_OFF_INFO   g_sMpCrvDataOff[];		        //��������
extern    const    MEM_OFF_INFO   g_sMpRDayDataOff[];		        //����������

extern    const    MEM_OFF_INFO   g_sXRamDataOff[];		        //RAM��ʱ��������
extern    const    MEM_OFF_INFO   g_sXFlhDataOff[];		        //FLH��ʱ��������
extern    const    MEM_OFF_INFO   g_sXFramDataOff[];		        //FRAM��ʱ��������

extern    const    MEM_OFF_INFO   g_sACInstDataOff[];
extern    const    MEM_OFF_INFO   g_sACRealDataOff[];	
extern    const    MEM_OFF_INFO   g_sACRealBackOff[];

#if 0
extern    const    MEM_OFF_INFO   g_sACDayDataOff[];	
extern    const    MEM_OFF_INFO   g_sACRDayDataOff[];	
extern    const    MEM_OFF_INFO   g_sACDjDataOff[];
#endif

extern    const    MEM_OFF_INFO   g_sACHistDataOff[];
extern    const    MEM_OFF_INFO   g_sACEvtDataOff[];
extern    const    MEM_OFF_INFO   g_sACBackDataOff[];
extern    const    MEM_OFF_INFO   g_sACBackFlhDataOff[];





/**************************************************************************************************
                                          �洢������Ϣ
***************************************************************************************************/
//��ַ��λ��Ϣ
const MEM_INFO g_sTblMem[] =
{
    //���ݿ���				//����					//����Ϣ			//������Ϣ(���ʡ���ַ���ļ���)
	
	//����
	{MEM_OBJ_PRM_AC_CAL,		1,						(pMEM_OFF_INFO)(g_sAcCalPrmOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sAcCalPrm)},
                                                                            {MEM_MEDIUM_FLASH, 0, AC_CAL_FILE},   	
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sAcCalPrm)} 
                                                                        }	},
    {MEM_OBJ_PRM_AC,			1,						(pMEM_OFF_INFO)(g_sAcPrmOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sAcPrm)},
                                                                            {MEM_MEDIUM_FLASH, 0, AC_FILE},   	
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sAcPrm)} 
                                                                        }	},
	{MEM_OBJ_PRM_TERM_COMM,		1,						(pMEM_OFF_INFO)(g_sTermCommOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sTermComm)},    		
					                                						{MEM_MEDIUM_FLASH, 0, COMM_PRM_FILE},
					                                						{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sTermCommPrm)} 
                                                                        }	},
    {MEM_OBJ_PRM_METER,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMeterOff), 		
																		{ 	{MEM_MEDIUM_FLASH, 0, METER_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_GRP,			MAX_GRP_NUM,			(pMEM_OFF_INFO)(g_sGrpOff), 		
																		{ 	{MEM_MEDIUM_FLASH, 0, GRP_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_RDMETER,		PORT_NUM,				(pMEM_OFF_INFO)(g_sRdMeterOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, RDMETER_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_METER_TYPE_I,	MAX_METER_MAXTYPE_NUM,	(pMEM_OFF_INFO)(g_sMeterTypeIOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, METER_TYPE_I_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_METER_TYPE_II,	MAX_METER_MAXTYPE_NUM,	(pMEM_OFF_INFO)(g_sMeterTypeIIOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, METER_TYPE_II_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_TASK_I,		MAX_TASK_NUM,			(pMEM_OFF_INFO)(g_sTaskIOff), 		
																		{ 	{MEM_MEDIUM_FLASH, 0, TASK_I_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_TASK_II,		MAX_TASK_NUM,			(pMEM_OFF_INFO)(g_sTaskIIOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, TASK_II_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_CTRL,			1,						(pMEM_OFF_INFO)(g_sCtrlOff),
                                                                        {    {MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sCtrlPrm)},
																		 	{MEM_MEDIUM_FLASH, 0, CTRL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    {MEM_OBJ_PRM_OTHER,			1,						(pMEM_OFF_INFO)(g_sOtherPrmOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, OTHER_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                       
	{MEM_OBJ_PRM_EXT,			1,						(pMEM_OFF_INFO)(g_sExtOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, EXT_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
																				
	//����
	//ʵʱ
	{MEM_OBJ_REAL_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpRealDataOff),
																		{ 	{MEM_MEDIUM_FLASH, 0, REAL_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	//��
	{MEM_OBJ_DAY_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpDayDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, DAY_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	//��
	{MEM_OBJ_MON_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpMonDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, MON_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	//����
	{MEM_OBJ_CRV_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpCrvDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, CRV_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},																		
	//������
	{MEM_OBJ_RDAY_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpRDayDataOff),
																		{ 	{MEM_MEDIUM_FLASH, 0, RDAY_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},	
																			
	//��������
	{MEM_OBJ_X_RAM_DATA,		1,						(pMEM_OFF_INFO)(g_sXRamDataOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sXRamData)},
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_X_FLH_DATA,		1,						(pMEM_OFF_INFO)(g_sXFlhDataOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sXFlhData)},     		
										                                	{MEM_MEDIUM_FLASH, 0, FLASH_XMP_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_X_FRAM_DATA,		1,						(pMEM_OFF_INFO)(g_sXFramDataOff), 
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sXFramData)},     		
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sXFramData)},   	
										                                	{MEM_MEDIUM_INVLD} }	},
                                                                                                                                                                                                                     
    //��������
    //AC˲ʱ����
    {MEM_OBJ_AC_INSTANT,		1,						(pMEM_OFF_INFO)(g_sACInstDataOff), 
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACInstData)},     		
										                                	{MEM_MEDIUM_INVLD},   	
										                                	{MEM_MEDIUM_INVLD} }	},
     //ACʵʱ����
    {MEM_OBJ_AC_REAL,		    1,						(pMEM_OFF_INFO)(g_sACRealDataOff), 
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACRealData)},     		
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sAcRealData)},   
										                                	{MEM_MEDIUM_INVLD} }	},
    //ACʵʱ����
    {MEM_OBJ_AC_BACK,		    1,						(pMEM_OFF_INFO)(g_sACRealBackOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_REAL_BACK_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                      
#if 0
     //AC������
    {MEM_OBJ_AC_DAY,		    1,						(pMEM_OFF_INFO)(g_sACDayDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_DAY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                       
    //AC����������
    {MEM_OBJ_AC_RDAY,		    1,						(pMEM_OFF_INFO)(g_sACRDayDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_RD_DAY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
                                                                            
    //AC��������
    {MEM_OBJ_AC_DSDJ,		    1,						(pMEM_OFF_INFO)(g_sACDjDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_DSDJ_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
    //AC��������
    {MEM_OBJ_AC_SQDJ,		    1,						(pMEM_OFF_INFO)(g_sACDjDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_SQDJ_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},  
    //AC��������
    {MEM_OBJ_AC_SDDJ,		    1,						(pMEM_OFF_INFO)(g_sACDjDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_SDDJ_FILE},   	
									                                	{MEM_MEDIUM_INVLD} }	}, 
#endif
    //AC��������
    {MEM_OBJ_AC_HIST_DATA,		    1,						(pMEM_OFF_INFO)(g_sACHistDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_HIST_DATA_FILE},   	
									                                	{MEM_MEDIUM_INVLD} }	},
    //ʧѹ�¼�(A)
    {MEM_OBJ_AC_EVT,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
#if 0    
    //ʧѹ�¼�(A)
    {MEM_OBJ_AC_EVT_AXSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_AXSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //ʧѹ�¼�(B)
    {MEM_OBJ_AC_EVT_BXSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BXSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //ʧѹ�¼�(C)
    {MEM_OBJ_AC_EVT_CXSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_CXSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //�����¼�(A)
    {MEM_OBJ_AC_EVT_AXDX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_AXDX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //�����¼�(B)
    {MEM_OBJ_AC_EVT_BXDX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BXDX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //�����¼�(C)
    {MEM_OBJ_AC_EVT_CXDX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_CXDX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
    //ȫʧѹ�¼�
    {MEM_OBJ_AC_EVT_QSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_QSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //��ѹ�����¼�
    {MEM_OBJ_AC_EVT_DYNX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_DYNX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //ʧ���¼�(A)
    {MEM_OBJ_AC_EVT_AXSL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_AXSL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //ʧ���¼�(B)
    {MEM_OBJ_AC_EVT_BXSL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BXSL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //ʧ���¼�(C)
    {MEM_OBJ_AC_EVT_CXSL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_CXSL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //�����¼�
    {MEM_OBJ_AC_EVT_DD,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_DD_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},  
    //���������¼�
    {MEM_OBJ_AC_EVT_XLQL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_XLQL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //��������¼�
    {MEM_OBJ_AC_EVT_DBQL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_DBQL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //�¼������¼�
    {MEM_OBJ_AC_EVT_SJQL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_SJQL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //����¼�
    {MEM_OBJ_AC_EVT_BC,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BC_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //Уʱ�¼�
    {MEM_OBJ_AC_EVT_JS,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_JS_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                          
#endif
    //(FRAM����)��������
    {MEM_OBJ_AC_X_BACK,		    1,						(pMEM_OFF_INFO)(g_sACBackDataOff), 
                                                                        { 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACBackData)},
																		 	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sACBackData)},   	
										                                	{MEM_MEDIUM_INVLD} }	},
    //(FLASH����)��������
    {MEM_OBJ_AC_X_BACK_FLH,		    1,						(pMEM_OFF_INFO)(g_sACBackFlhDataOff), 
                                                                        { 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACBackFlhData)},
																		 	{MEM_MEDIUM_FLASH, 0, AC_EVT_X_BACK_FLH_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                        
    {MEM_OBJ_TBL_NUM}
};


#if 0
#endif

/********************************************************************************
* ������: MemInfoChk
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �˲�
*********************************************************************************/
void*	MemTblGet(void)
{
	return (void*)(&g_sTblMem[0]);
}

/********************************************************************************
* ������: MemObjTblNumGet
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �˲�
*********************************************************************************/
INT32S  MemObjTblNumGet(void)
{
	return 	MEM_OBJ_TBL_NUM;
}


/********************************************************************************
* ������: MemInfoChk
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �˲�
*********************************************************************************/
static INT32S	MemOffChk(pMEM_OFF_INFO pMemOff)
{
	INT32S		i	= 0;
	INT32S		j	= 0;

	while(1)
	{
		if (pMemOff[i].nItmTblId == FEND)
		{
			break;
		}
		
		j = i + 1;
		while(1)
		{
			if (pMemOff[j].nItmTblId == FEND)
			{
				break;
			}
			//1.nFn�������
			if (pMemOff[i].nItmTblId == pMemOff[j].nItmTblId)
			{
				return -1;
			}
			j++;
		}

		//2.ǰһ��ƫ�Ƶ�ַҪС�ں�һ��
		if (pMemOff[i + 1].nItmTblId != FEND)
		{
			if (pMemOff[i].nAddr >= pMemOff[i + 1].nAddr)
			{
				return -1;
			}
		}
		
		i++;
	}
	return 0;
}


/********************************************************************************
* ������: MemInfoChk
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �˲�
*********************************************************************************/
INT32S MemInfoChk(void)
{
	pMEM_INFO					pMemInfo1		= (pMEM_INFO)(&g_sTblMem[0]);
	pMEM_INFO					pMemInfo2		= NULL;
	INT32S						i				= 0;

   	while(1)
	{
		if (pMemInfo1->nTblId == MEM_OBJ_TBL_NUM)
		{
			break;
		}
		
		//1.ID���밴˳������
		pMemInfo2 = pMemInfo1;
		while(1)
		{
			pMemInfo2++;
			if (pMemInfo2->nTblId == MEM_OBJ_TBL_NUM)
			{
				break;
			}
			if (pMemInfo1->nTblId == pMemInfo2->nTblId)
			{
				return -1;
			}
		}
		//2.������������Ϊ0�����ܴ��ڽ��������3
		/*
		if ((0 == pMemInfo1->nBackNum) || (pMemInfo1->nBackNum > MEM_MAX_BACK_NUM))
		{
			return -1;
		}
        */
		//3.��������Ϊ0
		if (0 == pMemInfo1->nRowMaxNum)
		{
			return -1;
		}
		//4.ÿ�ű����ƫ����Ϣ�˲�
		if (MemOffChk(pMemInfo1->pCol) < 0)
		{
			return -1;
		}
		//5.���ļ���ʱ������Ϊflash�Ļ���ַΪ0
		for (i = 0; i < MEM_MAX_BACK_NUM/*pMemInfo1->nBackNum*/; i++)
		{
			if (MEM_MEDIUM_FLASH == pMemInfo1->sMemBaseInfoSub[i].nMedium)
			{
				if (NULL != pMemInfo1->sMemBaseInfoSub[i].pFileName)
				{
					if (0 != pMemInfo1->sMemBaseInfoSub[i].nAddr)
					{
						return -1;
					}
				}
			}
		}
		
		pMemInfo1++;
	}
	return 0;
}



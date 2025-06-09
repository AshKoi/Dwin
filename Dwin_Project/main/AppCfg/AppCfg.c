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
/**************************************************************************************************
                                           ��������
***************************************************************************************************/
//���������ʼ����Ϣ
const AREA_CFG    g_sAreaCfg[] = 
{
    //����      	����	����������Ӧ����	�˿� AC--RS4853��CARR                �½�����ʾ����ʾֵ����
    //            			(0�رա�1��)                                      	 (0�رա�1��)
	{AREA_JIANGSU, 	"����", 1 ,                	{1, 2, 3, 4, 31, 0xFF} ,                0},
	{AREA_ZHEJIANG,	"�㽭", 1 ,                	{0, 1, 2, 3, 31, 0xFF} ,                1},
	{AREA_SICHUAN, 	"�Ĵ�", 1,                	{1, 2, 3, 4, 31, 0xFF} ,                1},
    
  	{0xFF}
};
//����������Ϣ
const INT8S*	g_sAreaString[AREA_MAX_NUM] 	= {0};


//gprs�����ӡ����
static INT8U	gs_nRomtPrintFlg		        = 0;


//�Լ�
INT8U			g_nAutoTestFlg  				= 0;
//Ӳ���ͺ�
INT8U			g_nHardNo		                = 0;

//ר����ʾ
INT8U           g_nZbDispFlg                    = 0;




/**************************************************************************************************
                                          �ļ�����
***************************************************************************************************/
const char* 	g_pFlashFiles[]				= {
	//G2U_U2G_FILE,

	//SELF_UPDATA_FILE,
	//CARR_UPDATA_FILE,	
	//CARR_DOWN_FILE,
    AC_CAL_FILE,
    AC_FILE,
	COMM_PRM_FILE,
	METER_PRM_FILE,
	GRP_PRM_FILE,
	RDMETER_PRM_FILE,
	METER_TYPE_I_FILE,
	METER_TYPE_II_FILE,
	TASK_I_FILE,
	TASK_II_FILE,
	CTRL_FILE,

	OTHER_PRM_FILE,
	EXT_PRM_FILE,

	REAL_DATA_FILE,
	DAY_DATA_FILE,
	MON_DATA_FILE,
	CRV_DATA_FILE,
	RDAY_DATA_FILE,

	FLASH_XMP_DATA_FILE,
    
    
    AC_REAL_BACK_FILE,
#if 0
    AC_DAY_FILE,
    AC_RD_DAY_FILE,
    AC_DSDJ_FILE,
    AC_SQDJ_FILE,
    AC_SDDJ_FILE,
#endif
    AC_HIST_DATA_FILE,
 
    AC_EVT_FILE,
#if 0
    AC_EVT_AXSY_FILE,
    AC_EVT_BXSY_FILE,
    AC_EVT_CXSY_FILE,
    AC_EVT_AXDX_FILE,
    AC_EVT_BXDX_FILE,
    AC_EVT_CXDX_FILE,
    AC_EVT_QSY_FILE,
    AC_EVT_DYNX_FILE,
    AC_EVT_AXSL_FILE,
    AC_EVT_BXSL_FILE,
    AC_EVT_CXSL_FILE,
    AC_EVT_DD_FILE,
    AC_EVT_XLQL_FILE,
    AC_EVT_DBQL_FILE,
    AC_EVT_SJQL_FILE,
    AC_EVT_BC_FILE,
    AC_EVT_JS_FILE,
#endif
    //AC_EVT_X_BACK_FILE,
    AC_EVT_X_BACK_FLH_FILE,
	
	NULL
};
	
/**************************************************************************************************
                                           ����
***************************************************************************************************/
extern INT8U AreaRunGet(INT8U nType);



/**************************************************************************************************
                                           ����
***************************************************************************************************/
char** FlashFileTblGet(void)
{
	return (char**)g_pFlashFiles;
}

void AreaStringMake(void)
{
	pAREA_CFG 	pAreaCfg 	= (pAREA_CFG)(&g_sAreaCfg[0]);
	INT8U		i			= 0;

	for (i = 0; i < AREA_MAX_NUM; i++)
	{
		g_sAreaString[i] = 0;
	}
	
	i = 0;
	while(1)
    {
        if (0xFF == pAreaCfg->nArea)
		{
			return;
		}
		if (i >= AREA_MAX_NUM)
		{
			return;
		}
		g_sAreaString[i] = pAreaCfg->nAreaString;
		
        pAreaCfg++;
		i++;
    }
}




/********************************************************************************
* ������:AreaRunGetExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
static INT8U AreaRunGetExt(void)
{
	pAREA_CFG 	pAreaCfg 	= (pAREA_CFG)(&g_sAreaCfg[0]);
	INT8U 		nAreaRun	= 0;
	
	nAreaRun = AreaRunGet(0);
    while(1)
    {
        if (0xFF == pAreaCfg->nArea)
		{
			return 0;
		}
        if (nAreaRun == pAreaCfg->nArea)
        {
            return nAreaRun;
        }
        pAreaCfg++;
    }
}











/********************************************************************************
* ������:GetCommBpsAutoSel
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S GetCommBpsAutoSel(void)
{
    pAREA_CFG 	pAreaCfg 	= (pAREA_CFG)(&g_sAreaCfg[0]);
	INT8U		nAreaRun 	= 0;

	nAreaRun = AreaRunGetExt();
    while(1)
    {
        if (0xFF == pAreaCfg->nArea)
		{
			return -1;
		}
        if (nAreaRun == pAreaCfg->nArea)
        {
            return pAreaCfg->nBpsAutoSel;
        }
        pAreaCfg++;
    }
}


/********************************************************************************
* ������:GetDispEnergySel
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S GetDispEnergySel(void)
{
    pAREA_CFG 	pAreaCfg 	= (pAREA_CFG)(&g_sAreaCfg[0]);
	INT8U		nAreaRun 	= 0;

	nAreaRun = AreaRunGetExt();
    
    while(1)
    {
        if (0xFF == pAreaCfg->nArea)
		{
			return -1;
		}
        if (nAreaRun == pAreaCfg->nArea)
        {
            return pAreaCfg->nDispEnergySel;
        }
        pAreaCfg++;
    }
}

INT32S GetPortIdx(INT8U nPort)
{
    pAREA_CFG 	pAreaCfg 	= (pAREA_CFG)(&g_sAreaCfg[0]);
	INT8U		nAreaRun 	= 0;
	//INT32S      i 			= 0;

	nAreaRun = AreaRunGetExt();
    while(1)
    {
        if (0xFF == pAreaCfg->nArea)
		{
			return -1;
		}
        if (nAreaRun == pAreaCfg->nArea)
        {
            break;
        }
        pAreaCfg++;
    }
    if (0xFF == pAreaCfg->nPortBuf[nPort])
    {
        return -1;
    }
    
    return pAreaCfg->nPortBuf[nPort];
    /*while(1)
    {
        if (0xFF == pAreaCfg->nPortBuf[i])
		{
			return -1;
		} 
        if (nPort == pAreaCfg->nPortBuf[i])
        {
            return i;    
        }
        
        i++;
    }*/
}

INT32S GetPort(INT8U nPortIdx)
{
    pAREA_CFG 	pAreaCfg 	= (pAREA_CFG)(&g_sAreaCfg[0]);
	INT8U		nAreaRun 	= 0;
	INT32S      i 			= 0;

	nAreaRun = AreaRunGetExt();
    while(1)
    {
        if (0xFF == pAreaCfg->nArea)
		{
			return -1;
		}
        if (nAreaRun == pAreaCfg->nArea)
        {
            break;
        }
        pAreaCfg++;
    } 

    while(1)
    {
        if (0xFF == pAreaCfg->nPortBuf[i])
		{
			break;
		} 
        i++;
    }
    if (nPortIdx >= i)
    {
        return -1;   
    }
    return pAreaCfg->nPortBuf[nPortIdx];   
}


INT32S GetRmotCmdPrintFlg(void)
{
	return gs_nRomtPrintFlg;
}

void SetRmotCmdPrintFlg(INT8U nFlg)
{
	gs_nRomtPrintFlg = nFlg;
}


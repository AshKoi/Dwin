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
#ifndef _MEM_TBL_H_
#define _MEM_TBL_H_

//#include "AppHead.h"
//#include "AppCfg.h"
#include "MemPrm.h"
#include "MemData.h"


/**************************************************************************************************
                                          �洢������
***************************************************************************************************/
enum _tagMEM_OBJ_TBL
{
    //����
    MEM_OBJ_PRM_AC_CAL=0,                   //У�����
    MEM_OBJ_PRM_AC,                         //���ɲ���
	MEM_OBJ_PRM_TERM_COMM,				    //�ն�ͨѶ����
    MEM_OBJ_PRM_METER,					    //������
    MEM_OBJ_PRM_GRP,						//�ܼ������
    MEM_OBJ_PRM_RDMETER,					//�ն����в���
    MEM_OBJ_PRM_METER_TYPE_I,			    //��С��
    MEM_OBJ_PRM_METER_TYPE_II,			    //��С��
    MEM_OBJ_PRM_TASK_I,					    //һ������
    MEM_OBJ_PRM_TASK_II,					//��������
    MEM_OBJ_PRM_CTRL,						//���������
    
    MEM_OBJ_PRM_OTHER,					    //��������
    MEM_OBJ_PRM_EXT,						//���������

	//����
	MEM_OBJ_REAL_DATA,					    //ʵʱ����
	MEM_OBJ_DAY_DATA,						//������
	MEM_OBJ_MON_DATA,						//������
	MEM_OBJ_CRV_DATA,						//��������
	MEM_OBJ_RDAY_DATA,					    //����������

	//��������
	MEM_OBJ_X_RAM_DATA,					    //RAM����
	MEM_OBJ_X_FLH_DATA,					    //FLASH����
	MEM_OBJ_X_FRAM_DATA,					//FRAM����

//�¼���¼ID���밴˳�ŷţ����м䲻���������ID
    //��������
	MEM_OBJ_AC_INSTANT,		            //AC˲ʱ����
	MEM_OBJ_AC_REAL,                    //ACʵʱ����
	MEM_OBJ_AC_BACK,					//ACʵʱ����
#if 0
	MEM_OBJ_AC_DAY,         			//������
    MEM_OBJ_AC_RDAY,        			//����������
    MEM_OBJ_AC_SSDJ,        			//˲ʱ��������
    MEM_OBJ_AC_DSDJ,        			//��ʱ��������
    MEM_OBJ_AC_SQDJ,					//Լ����������
    MEM_OBJ_AC_SDDJ,					//Լ����������
#endif
    
    MEM_OBJ_AC_HIST_DATA,
    
//�¼���¼ID���밴˳�ŷţ����м䲻���������ID
    MEM_OBJ_AC_EVT,	                    //�����¼�
#if 0
    MEM_OBJ_AC_EVT_AXSY,	            //ʧѹ�¼�(A)
	MEM_OBJ_AC_EVT_BXSY,	            //ʧѹ�¼�(B)
	MEM_OBJ_AC_EVT_CXSY,	            //ʧѹ�¼�(C)
	MEM_OBJ_AC_EVT_AXDX,	            //�����¼�(A)
	MEM_OBJ_AC_EVT_BXDX,	            //�����¼�(B)
	MEM_OBJ_AC_EVT_CXDX,	            //�����¼�(C)
	MEM_OBJ_AC_EVT_QSY,		            //ȫʧѹ�¼�
	MEM_OBJ_AC_EVT_DYNX,	            //��ѹ�����¼�
	MEM_OBJ_AC_EVT_AXSL,	            //ʧ���¼�(A)
	MEM_OBJ_AC_EVT_BXSL,	            //ʧ���¼�(B)
	MEM_OBJ_AC_EVT_CXSL,	            //ʧ���¼�(C)
	MEM_OBJ_AC_EVT_DD,		            //�����¼�
	MEM_OBJ_AC_EVT_XLQL,                //���������¼�
	MEM_OBJ_AC_EVT_DBQL,                //��������¼�
	MEM_OBJ_AC_EVT_SJQL,                //�¼������¼�
	MEM_OBJ_AC_EVT_BC,                  //����¼�
	MEM_OBJ_AC_EVT_JS,                  //Уʱ�¼�
#endif
    MEM_OBJ_AC_X_BACK,		            //(FRAM����)��������
	MEM_OBJ_AC_X_BACK_FLH,	            //(FLASH����)��������
    
	
    MEM_OBJ_TBL_NUM 						//����
};


/**************************************************************************************************
                                        ���ݽṹ�洢��װ����
***************************************************************************************************/

/**************************************************************************************************
                                          SDRAM�洢������
***************************************************************************************************/
//SDRAM�洢����
typedef struct _tagRAM_MEM
{
	//�ն˲���
    MEM_AC_CAL_PRM          sAcCalPrm;                              	//����У�����
    MEM_AC_PRM              sAcPrm;                                 	//���ɲ���
	MEM_TERM_COMM_PRM  		sTermComm;									//1.�ն�ͨѶ����
    //MEM_METER_PRM			sMeter[MAX_METER_NUM];				        //2.������
    MEM_CTRL_PRM            sCtrlPrm;                              	    //���Ʋ���
    
	//��������
	MEM_X_RAM_DATA			sXRamData;									//�ڴ�
	MEM_X_FLH_DATA			sXFlhData;									//FLASH
	MEM_X_FRAM_DATA			sXFramData;								    //FRAM
    
    //��������
    MEM_AC_INST_DATA		sACInstData;							    //AC˲ʱ���ݣ�����Ҫ���籣�棬ֻ��ram�洢����
  	MEM_AC_REAL_DATA		sACRealData;							    //ACʵʱ���ݣ�����Ƶ�����豣�棬�����籸��;ͬʱ��ʱ��flash����
    
    MEM_AC_BACK_DATA		sACBackData;							//(FRAM����)AC��������
    MEM_AC_BACK_FLH_DATA	sACBackFlhData;							//(FLASH����)AC��������
}RAM_MEM, *pRAM_MEM;

/**************************************************************************************************
                                          Flash�洢������
***************************************************************************************************/
//Flash�ն����ݴ洢����    ʹ�����ļ�ϵͳ������Ҫ����ṹ
typedef struct _tagMEM_FLH_DATA_MAP
{
	INT8U					nData;					
    
}MEM_FLH_DATA_MAP, *pMEM_FLH_DATA_MAP;


/**************************************************************************************************
                                         FRAM�洢������
***************************************************************************************************/
//FRAM�洢����
typedef struct _tagMEM_FRAM_STORE
{
	//INT8U					nData[8];											//
    MEM_AC_CAL_PRM          sAcCalPrm;                                          //����У�����
    MEM_AC_PRM              sAcPrm;                                             //���ɲ���
    MEM_TERM_COMM_PRM       sTermCommPrm;                                       //�ն�ͨ�Ų���
	MEM_X_FRAM_DATA			sXFramData;     									//�������ݣ����磩sXBackData
    
    MEM_AC_REAL_DATA        sAcRealData;                                        //����ʵʱ����
    MEM_AC_BACK_DATA		sACBackData;							            //(FRAM����)AC��������
}MEM_FRAM_STORE, *pMEM_FRAM_STORE;




/**************************************************************************************************
                                          �ⲿ����
***************************************************************************************************/
extern pRAM_MEM				g_pMem;											//ram����


/**************************************************************************************************
                                          ��غ���˵��
***************************************************************************************************/
void*	MemTblGet(void);
INT32S  MemObjTblNumGet(void);


INT32S 	MemInfoChk(void);


#endif










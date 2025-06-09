#ifndef GW_698_H
#define GW_698_H

#include "Base.h"
#include "SysRes.h"
#include "linkcomExt.h"
#include "AssitProt.h"

#include "GuiwinCfg.h"
#include "Guiwin.h"

#include "Fun.h"
#include "Guimenu.h"
#include "GB3761CtrlCmd.h"
#include "GB3761GetClassI.h"
#include "GB3761GetClassII.h"
#include "GB3761GetPrm.h"
#include "GB3761SetPrm.h"

#include "GB3761GetPrmExt.h"
#include "GB3761SetPrmExt.h"
#include "FileTrans.h"
#include "GB3761Trans.h"

#include "Login.h"

#include "MemTbl.h"

#include "meter645.h"

#include "Reset.h"
#include "AutoTest.h"
#include "Telit.h"

#include "Login.h"
#include "Version.h"

#define         GB698_SAADDR_LEN    		        0x06		//��ַ����
extern INT16U   g_nHeadLen;

/**************************************************************************************************
                                             ���Ͷ���
***************************************************************************************************/
//698��ԼЭ��
typedef struct _tagGW698_PROT
{
	INT8U					nHead1;
	INT8U					nDataLen[2];
	INT8U					nCtrl;
//    INT8U                   nAddrLen;//��ַ���ֽ���,�߼���ַ,��������ַ�ĵ�ַ����
    INT8U                   nMeterAddrSAFlg;//��ַ����
    INT8U					nMeterAddr[GB698_SAADDR_LEN];// ��һ����5 ��
    INT8U                   nClientAddr;
    INT8U					nHCS[2];
    //INT8U                   nRequestType[2];
    //INT8U                   nPIID;
    //INT8U                   nOAD;
	INT8U					nDadtData[1];
}GW698_PROT, *pGW698_PROT;



//GB698֡Map��Ϣ
typedef struct  _tagGB698_FRM_MAP
{
	INT8U               nHead;
	union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nProtId : 2;//����
            INT8U       nL1     : 6;
        }sDet;
    }uLen;
    INT8U               nLen;
    union
    {
        INT8U           nWord;
        struct
        {
            INT8U       nFun : 3;
            INT8U       nFcv : 2;
			INT8U       nFcb : 1;
            INT8U       nPrm : 1;
            INT8U       nDir : 1;
        }sDet;
    }uCtrl;
    INT8U               nSAFlg;
    INT8U       		nSAAddr[6];
    INT8U       		nCA;
    INT8U       		nlHCS;
    INT8U       		nhHCS;
    INT8U       		nRequestType;
    INT8U       		nRequestObject;
    INT8U               nPIID;
    INT8U       		nData[2];
    INT8U               nResultType;
    INT8U               nFollwReport;
    INT8U               nTimeTag;
}GB698_FRM_MAP, *pGB698_FRM_MAP;

//GB698֡��Ϣ
typedef struct  _tagGB698_FRM_INFO
{
    BOOLEAN             bAllMpFlag;                        //ȫ��������־
    BOOLEAN             bSendFlag;                         //�ڲ����ͱ�־
    BOOLEAN             bFirstFlag;                        //��һ֡��־
    BOOLEAN             bSucFlag;                          //TRUE  �гɹ�DADT
    BOOLEAN             bFailFlag;                         //FALSE ��ʧ��DADT
    BOOLEAN             bNoDataFlag;                       //�����ٻ����ݱ�ʶ
    BOOLEAN             bAutoRptFlag;                      //�����ϱ�����
    INT16U              nAutoFrmNo;                        //�����ϱ���֡��
    INT8U               nAckType;                          //ȷ�Ϸ���֡����
    INT8U               nPFC;                              //����֡��ż�����  TP
    INT8U               nDelay;                            //�����ʹ�����ʱʱ�� TP
    INT16U              nRxDataLen;                        //�����峤��  DADT+���ݵ�Ԫ
    INT16U              nTxDataLen;                        //����֡����  DADT+���ݵ�Ԫ
}GB698_FRM_INFO, *pGB698_FRM_INFO;

//698��ԼЭ����غ�
#define GW698_HEAD_LEN 			((INT32S)(sizeof(GW698_PROT) - 1))					//����ͷ����:dadt֮ǰ�ĳ���
#define GW698_HEAD_TAIL_LEN 	((INT32S)(GW698_HEAD_LEN + 2))						//��ͷ��β����
#define GW698_BASE_LEN 			((INT32S)(GW698_HEAD_TAIL_LEN + 4))					//���Ļ�������:��ͷ��β+dadt
#define GW698_DATA_POS 			((INT32S)(GW698_HEAD_LEN + 4))						//����dadt������λ��

#define GW698_APDU_LEN1 		3                                               //APDU Dataǰ�ĳ���

#define         GB698_REQUEST_CONNECT    		    0x02		//����Ӧ����������
#define         GB698_REQUEST_RELEASE    		    0x03		//�Ͽ�Ӧ����������
#define         GB698_REQUEST_GET    		        0x05		//��ȡ����
#define         GB698_REQUEST_SET    		        0x06		//��������
#define         GB698_REQUEST_ACTION    		    0x07		//��������
#define         GB698_REQUEST_REPORT    		    0x08		//�ϱ�Ӧ��
#define         GB698_REQUEST_PROXY    		        0x09		//��������
#define         GB698_REQUEST_SECURITY    		        0x10		//��������


enum    _tagGW698DATATYPE
{
    e_null=0,
    e_array,
    e_structure,
    e_bool,
    e_bit_string,
    e_double_long,
    e_double_long_unsigned,
    e_octet_string=9,
    e_visible_string,
    e_UTF8_string = 12,
    e_integer=15,
    e_long,
    e_unsigned,
    e_long_unsigned,
    e_long64=20,
    e_long64_unsigned,
    e_enum,
    e_ufloat32,
    e_float64,
    e_date_time,
    e_date,
    e_time,
    e_date_time_s,
    e_OI = 80,
    e_OAD,
    e_ROAD,
    e_OMD,
    e_TI,
    e_TSA,
    e_MAC,
    e_RN,
    e_Region,
    e_Scaler_Unit,
    e_RSD,
    e_CSD,
    e_MS,
    e_SID,
    e_SID_MAC,
    e_COMDCB,
    e_RCSD,
    e_SRD,
    e_SI,
    e_END     
};


enum    _tagGW698DARTYPE
{
    e_Suc = 0,
    e_HardFault,
    e_RefuseRW = 3,
    e_NoDefObject,
    e_NoObject = 6,
    e_TypeErr,
    e_BeyondLim,
    e_DarEND
};

/**************************************************************************************************
                                             ����
***************************************************************************************************/
INT32S Gw698AsynCheckPackage(INT32U nDataNum, INT8U* pBuf);
INT32S Gw698AsynProcSub(INT8U* pRcv, INT8U* pSnd, INT32S nSndlen);
#endif
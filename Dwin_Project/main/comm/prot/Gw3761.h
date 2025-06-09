/******************************************************************************************************
* Copyright (c) 2014
* �ļ���:
* ������: ��һ��
* �ա���: 2014/01/18
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#ifndef GW_376_1_H
#define GW_376_1_H
#include "Os_cpu.h"
#include "Macro.h"

extern INT8U 				g_nAsynFlg;
extern INT8U				g_nPackAssitFlg;
extern INT8U 				g_nReport;

extern INT8U               Ascend;
extern INT8U               Descend;

extern INT8U           g_nGprsWatchFlg;
/**************************************************************************************************
                                             ���Ͷ���
***************************************************************************************************/
//376.1��ԼЭ��
typedef struct _tagGW376_1_PROT
{
	INT8U					nHead1;
	INT8U					nLen1[2];
	INT8U					nLen2[2];
	INT8U					nHead2;
	INT8U					nCtrl;
	INT8U					nTermAddr[4];
	INT8U					nMasterAddr;
	INT8U					nAfn;
	INT8U					nSeq;
	INT8U					nDadtData[1];
}GW376_1_PROT, *pGW376_1_PROT;

typedef INT32S (*pGW3761PROC_AFN)(INT8U* pRcv, INT32S nRcvlen, INT8U* pSnd, INT32S nSndlen);
typedef struct _tagGW3761_PROC_AFN
{
	INT8U					nAfn;
	void*					pGw3761ProcAfnSub;
}GW3761_PROC_AFN, *pGW3761_PROC_AFN;

typedef INT32S (*pGW3761PROC_AFN_SUB)(INT16U nPn, INT8U nFn, INT8U* pRcv, INT32S nRcvlen, INT8U* pSnd, INT32S nSndlen, INT16U *pOffset);


//376.1��ԼЭ����غ�
#define GW376_1_HEAD_LEN 			((INT32S)(sizeof(GW376_1_PROT) - 1))					//����ͷ����:dadt֮ǰ�ĳ���
#define GW376_1_HEAD_TAIL_LEN 	((INT32S)(GW376_1_HEAD_LEN + 2))						//��ͷ��β����
#define GW376_1_BASE_LEN 			((INT32S)(GW376_1_HEAD_TAIL_LEN + 4))					//���Ļ�������:��ͷ��β+dadt
#define GW376_1_DATA_POS 			((INT32S)(GW376_1_HEAD_LEN + 4))						//����dadt������λ��



    




  
//��һ��Լ��
/**************************************************************************************************
                                          �� �� �� �� ��
***************************************************************************************************/
#define		    GB3761_DATA_LEN			1024		//GB3761�����峤��
#define		    GB3761_PWD_LEN			16			//PWD����
#define		    GB3761_EC_LEN			2			//EC����
#define		    GB3761_TP_LEN			6			//TP����
#define         GB3761_TAIL_LEN         2           //֡β���� CS 16
#define         GB3761_HEAD_LEN         6           //֡ͷ���� 68 L L L L 68
#define         GB3761_PROT_HEAD_OFFSET GY_PROT_HEAD_OFFSET //��Լͷ��ʼλ��

#define         GB3761_HEADER               0x68               //����ͷ
#define         GB3761_TAILER               0x16               //����β

//ƫ�Ƴ���
#define         GB3761_HEAD1_IDX                0           //0x68 ƫ��
#define			GB3761_LEN_IDX     				1			//Lenƫ��
#define         GB3761_HEAD2_IDX                5           //0x68 ƫ��
#define			GB3761_CTRL_IDX					6			//Ctrlƫ��
#define			GB3761_AREA_IDX  				7			//Areaƫ��
#define			GB3761_ADDR_IDX					9			//Addrƫ��
#define			GB3761_ADDR_FLAG_IDX			11			//AddrFlagƫ��
#define			GB3761_AFN_IDX					12			//Afnƫ��
#define			GB3761_SEQ_IDX					13			//Seqƫ��
#define			GB3761_DATA_IDX					14			//Dataƫ��

//AFN������
#define         GB3761_AFN_ACK           		0x00		//ȷ�Ϸ���֡
#define         GB3761_AFN_RESET         		0x01		//��λ����
#define         GB3761_AFN_LINK          		0x02		//��·�ӿڼ��
#define         GB3761_AFN_RELAY         		0x03		//�м��ն�����
#define         GB3761_AFN_SET_PARAM     		0x04		//�ն˲�������
#define         GB3761_AFN_CTRL          		0x05		//��������
#define         GB3761_AFN_CHECK_PWD     		0x06		//������֤
#define         GB3761_AFN_REQ_RPT       		0x08		//���󱻼����ն������ϱ�
#define         GB3761_AFN_REQ_CFG       		0x09		//��ѯ�ն�����״̬
#define         GB3761_AFN_QUERY_PARAM   		0x0A		//�ն˲�����ѯ
#define         GB3761_AFN_REQ_TASK      		0x0B		//�������ݲ�ѯ
#define         GB3761_AFN_REQ_CLASS_I   		0x0C		//����I������
#define         GB3761_AFN_REQ_CLASS_II  		0x0D		//����II������
#define         GB3761_AFN_REQ_CLASS_III 		0x0E		//����III������
#define         GB3761_AFN_TRANS_FILE    	   	0x2F//0x0F  //�ļ�����,�������ն˺ͱ�ƶ��������ϣ���Ҫ����
                                                            //I�ͼ�������0x0F��II�ͼ�����:0x1F,���:0x2F
#define         GB3761_AFN_TRANS_DATA    		0x10		//����ת��
#define         GB3761_AFN_TEST_EXT    		    0xDF		//�ڲ����
//��չAFN
#define         GB3761_AFN_SET_PRM_EXT          0xF4//0xA4        //��չ��������
#define         GB3761_AFN_QUERY_PRM_EXT        0xFA//0xAA        //��չ������ѯ
#define         GB3761_AFN_GY_EXPAND_II         0xF2        //��һ��չ

//GB3761������վ����·������
#define         GB3761_LFN_RESET            0x01            //��λ
#define         GB3761_LFN_SEND_DATA        0x04            //�û�����
#define         GB3761_LFN_REQ_LINK         0x09            //��·����
#define         GB3761_LFN_REQ_LEVEL1       0x0A            //����1������
#define         GB3761_LFN_REQ_LEVEL2       0x0B            //����2������

//GB3761�ĴӶ�վ����·������
#define         GB3761_LFN_ACK              0x0             //��λȷ��
#define         GB3761_LFN_RESP_DATA        0x08            //�û�����
#define         GB3761_LFN_RESP_DENY        0x09            //���������ٻ�����
#define         GB3761_LFN_RESP_LINK        0x0B            //��·״̬
#define         GB3761_LFN_RESP_NULL        0x1E            //��ЧLFN

//GB3761��Ӧȷ�Ϸ���֡����
#define         GB3761_RESP_DENY            0x02              //����֡
#define         GB3761_RESP_NO_DATA         0x03              //���������ٻ�����
#define         GB3761_RESP_ACK             0x01              //ȷ��֡
#define         GB3761_RESP_PART            0x04              //����ȷ��֡
#define         GB3761_RESP_PWD_DENY        0x08              //�����֤����
#define         GB3761_RESP_DATA            0x05              //��������
#define         GB3761_RESP_TERM_REG        0x06              //�ն�ע��

//����ȷ��֡��������
#define         GB3761_PART_ACK             0x0               //��ȷ
#define         GB3761_PART_OTHER           0x01              //��������
#define         GB3761_PART_ADDR            0x02              //���ַ�ظ�

//�����֤������
#define         GB3761_CHECK_SIGN_ERROR    0x01               //ǩ��У�����
#define         GB3761_CHECK_CIPHER_ERROR  0x02               //����У�����
#define         GB3761_CHECK_MAC_ERROR     0x03               //MAC��֤(����)����

//��Լ����ֵ
#define			GB3761_MAX_FN_NUM				8			//���Fn����
#define         GB3761_FRM_RES_LEN              (2 + GB3761_DATA_IDX + GB3761_EC_LEN + GB3761_TP_LEN + GB3761_TAIL_LEN)//��䱨��ʱ֡βԤ���ĳ��� �̶�֡ͷ����+�¼�����+ʱ���ǩ����
#define         GB3761_MIN_FRM_LEN              20          //��С֡����

//��Ч��������
#define         GB3761_INVLD_BCD          INVLD_DATA
#define         GB3761_INVLD_BS8          0x0
#define         GB3761_INVLD_BIN          0x0
#define         GB3761_INVLD_ASC          '\0'

//����ת����ʱ
#define         GB3761_MSG_CARR_TIMEOUT1    (60 * OS_TICKS_PER_SEC)  //��͸�����ز���ʱ1 60��
#define         GB3761_MSG_CARR_TIMEOUT2    (40 * OS_TICKS_PER_SEC)  //��͸�����ز���ʱ2 40��
#define         GB3761_MSG_485_TIMEOUT      (30 * OS_TICKS_PER_SEC)  //��͸����485��ʱ   30��
#define         GB3761_MSG_DEAFULT_TIMEOUT  (1 * OS_TICKS_PER_SEC)   //��͸����Ĭ����ʱ  1��

#define         GB3761_FILETRANS_HEAD_LEN       16                   //�ļ���������ͷ����

//�ȴ�FTP��ʱ
#define         GB3761_FTP_TIMEOUT          30                       //FTP����Ĭ����ʱ30��
/**************************************************************************************************
                                          �� �� �� �� �� �� �� ��
***************************************************************************************************/
//GB3761֡Map��Ϣ
typedef struct  _tagGB3761_FRM_MAP
{
	INT8U               nHead1;
	union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nProtId : 2;//0������ 1:Q/GDW 130-2005  2:GB3761-2009  3:����
            INT8U       nL1     : 6;
        }sDet;
    }uLen1;
    INT8U               nLen1;
    union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nProtId : 2;//0������ 1:Q/GDW 3761-2005  2:GB3761  3:����
            INT8U       nL2     : 6;
        }sDet;
    }uLen2;
    INT8U               nLen2;
    INT8U               nHead2;
    union
    {
        INT8U           nWord;
        struct
        {
            INT8U       nFun : 4;
            INT8U       nFcv : 1;
			INT8U       nFcb : 1;
            INT8U       nPrm : 1;
            INT8U       nDir : 1;
        }sDet;
    }uCtrl;
    INT8U       		nArea[LEN_AREA_CODE];
    INT8U       		nAddr[LEN_TERM_ADDR];
    union
    {
        INT8U		    nWord;
        struct
        {
            INT8U       nGrpAddFlag : 1;
            INT8U       nMSA     : 7;
        }sDet;
    }uA3;
    INT8U               nAfn;
    union
    {
        INT8U           nWord;
        struct
        {
            INT8U       nPRSeq : 4;
            INT8U       nCon   : 1;
            INT8U       nFin   : 1;
            INT8U       nFir   : 1;
            INT8U       nTpv   : 1;
        }sDet;
    }uSEQ;
}GB3761_FRM_MAP, *pGB3761_FRM_MAP;

//GB3761֡��Ϣ
typedef struct  _tagGB3761_FRM_INFO
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
}GB3761_FRM_INFO, *pGB3761_FRM_INFO;

//Ӧ��ͨѶ
typedef struct _tagAPP_COMM
{
	INT8U       *pTxFrmBuf;                     //����֡��Ϣ
	INT8U       *pRxFrmBuf;                     //����֡��Ϣ
    void        *pFrmInfo;                      //����֡��Ϣ
	INT16U		nFrmLen;						//��֡����
}APP_COMM, *pAPP_COMM;

//������Լѡ��
#define TERM_QGDW130_SCH				0          				//����2005����
#define TERM_QGDW3761_SCH           	1          				//����2009����

#define TERM_SCH_TYPE   				TERM_QGDW3761_SCH       		//������Լ






/**************************************************************************************************
                                             ����
***************************************************************************************************/
INT32S Gw3761FrmWrite(INT8U nCom, INT8U* pData, INT32S nDatalen);
INT32S Gw3761FrmRead(INT8U nCom, INT8U* pData, INT32S nMaxDatalen, INT32U iInterval, INT32U ms);

INT32S GB3761PnFn2DADT(INT16U nPn, INT8U nFn, INT8U* pOutBuf);
INT8S GB3761DaDt2PnFn(const INT8U *pDaDt, INT16U *pPn, INT16U *pPnNum, INT16U *pFn, INT8U *pFnNum, BOOLEAN *pAllMp);
INT8S GB3761GetAllMpNo(INT8U nAfn, INT16U nFn, INT16U *pPnID, INT16U *pPnNum, INT16U nMaxLen);

INT32S Gw3761MakeFrm(INT8U nCtrl, INT8U nAfn, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen);

INT32S Gw3761CheckPackage(INT8U* pData, INT32S nDatalen);
INT32S Gw3761Proc(INT8U nCom, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen);

INT32S Gw3761AsynCheckPackageTmp(void* pPara);
INT32S Gw3761AsynCheckPackage(void* pPara);
INT32S Gw3761AsynProcTmp(void* pPara);
INT32S Gw3761AsynProc(void* pPara);

INT32S ForwardMakeFrmSub(INT8U nBoradFlg, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U nTimeOutSec, INT8U* pBuf, INT32S nMaxDatalen);
INT32S ForwardMakeFrmSub2(INT8U nProt, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U nTimeOutSec, INT8U* pBuf, INT32S nMaxDatalen);
INT32S Gw3761SelfForwardMakeFrm(INT8U nPort, INT8U* pBuf, INT32S nMaxDatalen);
INT32S Gw3761ForwardMakeFrm(INT8U nProt, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U* pBuf, INT32S nMaxDatalen);

INT32S GetTermAreaAddr(void);

INT8S GB3761TermTest(pAPP_COMM pComm);
INT8S GB3761TermTestProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761TermTestFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT32S GB3761TermTestF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset, INT8U* pBuf);

INT8S GB3761TermReset(pAPP_COMM pComm);
INT8S GB3761TermResetProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761TermResetFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


INT32S Gw3761AsynProcSub(INT8U* pRcv, INT8U* pSnd, INT32S nSndlen);

#endif

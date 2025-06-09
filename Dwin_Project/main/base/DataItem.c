/**************************************************************************************************
* Copyright (c) 2010-2011 ��һ�Ƽ��ɷ����޹�˾�з�����
* �ļ���:
* ������: �з�����
* �ա���: 2010/08/18
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
***************************************************************************************************/
#include "AppCfg.h"
#include "DataItem.h"
#include "Base.h"
#include "MemDrvExt.h"
#include "Meter645.h"
#include "MemTbl.h"

/**************************************************************************************************
                                          ���ȫ�ֱ�������
***************************************************************************************************/
#if 0
#endif
//��ͨ�û���������������
const INT16U g_nNormUserMtCfg[] =
{
    //1.ʵʱ
    RD_ZXYGJH,			    //(901F)�����й�����ʾֵ����
    RD_FXYGJH,


	RD_DBRQ,            	//(C010)�������
    RD_DBSJ,            	//(C011)���ʱ��

    RD_DBYXZTZ,			    //(C020)�������״̬��
    RD_DBBCJL,				//(B21F)����̼�¼

    //2.��
    DD_ZXYGJH,              //�������й�����ʾֵ����

    //3.��
    MD_ZXYGJH,              //(941f)���������й�����ʾֵ


    INVLD_DATA_ITEM    
};
//�ص��û���������������
const INT16U g_nImpUserMtCfg[] =
{
    //1.ʵʱ
    RD_ZXYGJH,			    //(901F)�����й�����ʾֵ����
    RD_FXYGJH,

    RD_SYJH,				//(E133)ʧѹ����
    RD_SLJH,				//(E122)ʧ������
    RD_DLFXJH,				//(E142)�����������

	RD_DBRQ,            	//(C010)�������
    RD_DBSJ,            	//(C011)���ʱ��

    RD_DBYXZTZ,			    //(C020)�������״̬��
    RD_DBBCJL,				//(B21F)����̼�¼

    //2.��
    DD_ZXYGJH,              //�������й�����ʾֵ����

    //3.��
    MD_ZXYGJH,              //(941f)���������й�����ʾֵ


    INVLD_DATA_ITEM    
};



#if 0
#endif
//485��������������
const INT16U g_nRs485MtCfg[] =
{
    //1.ʵʱ
    RD_ZXYGJH,			    //(901F)�����й�����ʾֵ����
    RD_FXYGJH,
	RD_DBRQ,            	//(C010)�������
    RD_DBSJ,            	//(C011)���ʱ��

    //2.��
    DD_ZXYGJH,              //�������й�����ʾֵ����

    //3.��
    MD_ZXYGJH,              //(941f)���������й�����ʾֵ


    INVLD_DATA_ITEM    
};

//carrier��������������
const INT16U g_nCarrierMtCfg[] =
{
    //1.ʵʱ
    RD_ZXYGJH,			    //(901F)�����й�����ʾֵ����
    
	RD_DBRQ,            	//(C010)�������
    RD_DBSJ,            	//(C011)���ʱ��

    //2.��
    DD_ZXYGJH,              //�������й�����ʾֵ����

    //3.��
    MD_ZXYGJH,              //(941f)���������й�����ʾֵ


    INVLD_DATA_ITEM    
};

#if 0
#endif
//ʵʱ���ݳ�������������
const INT16U g_nRealMtCfg[] =
{
    RD_ZXYGJH,			    //(901F)�����й�����ʾֵ����
    RD_FXYGJH,

    INVLD_DATA_ITEM

};

//�����ݳ�������������
const INT16U g_nDayMtCfg[] =
{
    DD_ZXYGJH,              //�������й�����ʾֵ����
    DD_FXYGJH,
    
    RD_DBRQ,            	//(C010)�������
    RD_DBSJ,            	//(C011)���ʱ��

    INVLD_DATA_ITEM

};


//�����ݳ�������������
const INT16U g_nMonMtCfg[] =
{
    MD_ZXYGJH,              //(941f)���������й�����ʾֵ
    MD_FXYGJH,
    
	RD_DBRQ,            	//(C010)�������
    RD_DBSJ,            	//(C011)���ʱ��

    INVLD_DATA_ITEM

};

#if 0
//�������ݳ�������������
const INT16U g_nCrvMtCfg[] =
{
    RD_ZXYGJH,			    //(901F)�����й�����ʾֵ����

    INVLD_DATA_ITEM

};
#endif

//���������ݳ�������������
const INT16U g_nRDayMtCfg[] =
{
    MD_ZXYGJH,              //(941f)���������й�����ʾֵ

    INVLD_DATA_ITEM

};

/**************************************************************************************************
                                         AC�¼�����������
***************************************************************************************************/
//ʧѹ��
const INT8U g_nACSY[53] =
{
	6,//����1�Σ�A��ʧѹ����ʱ��                   01
	4,//����1�Σ�A��ʧѹ����ʱ�������й��ܵ���     02
	4,//����1�Σ�A��ʧѹ����ʱ�̷����й��ܵ���     03
	4,//����1�Σ�A��ʧѹ����ʱ������޹�1�ܵ���    04
	4,//����1�Σ�A��ʧѹ����ʱ������޹�2�ܵ���    05
	4,//����1�Σ�A��ʧѹ����ʱ��A�������й�����    06
	4,//����1�Σ�A��ʧѹ����ʱ��A�෴���й�����    07
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�1����   08
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�2����   09
	2,//����1�Σ�A��ʧѹ����ʱ��A���ѹ            0A
	3,//����1�Σ�A��ʧѹ����ʱ��A�����            0B
	3,//����1�Σ�A��ʧѹ����ʱ��A���й�����        0C
	3,//����1�Σ�A��ʧѹ����ʱ��A���޹�����        0D
	2,//����1�Σ�A��ʧѹ����ʱ��A�๦������        0E
	4,//����1�Σ�A��ʧѹ����ʱ��B�������й�����    0F
	4,//����1�Σ�A��ʧѹ����ʱ��B�෴���й�����    10
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�1����   11
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�2����   12
	2,//����1�Σ�A��ʧѹ����ʱ��B���ѹ            13
	3,//����1�Σ�A��ʧѹ����ʱ��B�����            14
	3,//����1�Σ�A��ʧѹ����ʱ��B���й�����        15
	3,//����1�Σ�A��ʧѹ����ʱ��B���޹�����        16
	2,//����1�Σ�A��ʧѹ����ʱ��B�๦������        17
	4,//����1�Σ�A��ʧѹ����ʱ��C�������й�����    18
	4,//����1�Σ�A��ʧѹ����ʱ��C�෴���й�����    19
	4,//����1�Σ�A��ʧѹ����ʱ��C������޹�1����   1A
	4,//����1�Σ�A��ʧѹ����ʱ��C������޹�2����   1B
	2,//����1�Σ�A��ʧѹ����ʱ��C���ѹ            1C
	3,//����1�Σ�A��ʧѹ����ʱ��C�����            1D
	3,//����1�Σ�A��ʧѹ����ʱ��C���й�����        1E
	3,//����1�Σ�A��ʧѹ����ʱ��C���޹�����        1F
	2,//����1�Σ�A��ʧѹ����ʱ��C�๦������        20
	4,//����1�Σ�A��ʧѹ�ڼ��ܰ�ʱ��               21
	4,//����1�Σ�A��ʧѹ�ڼ�A�లʱ��              22
	4,//����1�Σ�A��ʧѹ�ڼ�B�లʱ��              23
	4,//����1�Σ�A��ʧѹ�ڼ�C�లʱ��              24
	6,//����1�Σ�A��ʧѹ����ʱ��                   25
	4,//����1�Σ�A��ʧѹ����ʱ�������й��ܵ���     26
	4,//����1�Σ�A��ʧѹ����ʱ�̷����й��ܵ���     27
	4,//����1�Σ�A��ʧѹ����ʱ������޹�1�ܵ���    28
	4,//����1�Σ�A��ʧѹ����ʱ������޹�2�ܵ���    29
	4,//����1�Σ�A��ʧѹ����ʱ��A�������й�����    2A
	4,//����1�Σ�A��ʧѹ����ʱ��A�෴���й�����    2B
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�1����   2C
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�2����   2D
	4,//����1�Σ�A��ʧѹ����ʱ��B�������й�����    2E
	4,//����1�Σ�A��ʧѹ����ʱ��B�෴���й�����    2F
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�1����   30
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�2����   31
	4,//����1�Σ�A��ʧѹ����ʱ��C�������й�����    32
	4,//����1�Σ�A��ʧѹ����ʱ��C�෴���й�����    33
	4,//����1�Σ�A��ʧѹ����ʱ��C������޹�1����   34
	4 //����1�Σ�A��ʧѹ����ʱ��C������޹�2����   35
};

//ʧ����
const INT8U g_nACSL[49] =
{
	6,//����1�Σ�A��ʧ������ʱ��                     01
	4,//����1�Σ�A��ʧ������ʱ�������й��ܵ���       02
	4,//����1�Σ�A��ʧ������ʱ�̷����й��ܵ���       03
	4,//����1�Σ�A��ʧ������ʱ������޹�1�ܵ���      04
	4,//����1�Σ�A��ʧ������ʱ������޹�2�ܵ���      05
	4,//����1�Σ�A��ʧ������ʱ��A�������й�����      06
	4,//����1�Σ�A��ʧ������ʱ��A�෴���й�����      07
	4,//����1�Σ�A��ʧ������ʱ��A������޹�1����     08
	4,//����1�Σ�A��ʧ������ʱ��A������޹�2����     09
	2,//����1�Σ�A��ʧ������ʱ��A���ѹ              0A
	3,//����1�Σ�A��ʧ������ʱ��A�����              0B
	3,//����1�Σ�A��ʧ������ʱ��A���й�����          0C
	3,//����1�Σ�A��ʧ������ʱ��A���޹�����          0D
	2,//����1�Σ�A��ʧ������ʱ��A�๦������          0E
	4,//����1�Σ�A��ʧ������ʱ��B�������й�����      0F
	4,//����1�Σ�A��ʧ������ʱ��B�෴���й�����      10
	4,//����1�Σ�A��ʧ������ʱ��B������޹�1����     11
	4,//����1�Σ�A��ʧ������ʱ��B������޹�2����     12
	2,//����1�Σ�A��ʧ������ʱ��B���ѹ              13
	3,//����1�Σ�A��ʧ������ʱ��B�����              14
	3,//����1�Σ�A��ʧ������ʱ��B���й�����          15
	3,//����1�Σ�A��ʧ������ʱ��B���޹�����          16
	2,//����1�Σ�A��ʧ������ʱ��B�๦������          17
	4,//����1�Σ�A��ʧ������ʱ��C�������й�����      18
	4,//����1�Σ�A��ʧ������ʱ��C�෴���й�����      19
	4,//����1�Σ�A��ʧ������ʱ��C������޹�1����     1A
	4,//����1�Σ�A��ʧ������ʱ��C������޹�2����     1B
	2,//����1�Σ�A��ʧ������ʱ��C���ѹ              1C
	3,//����1�Σ�A��ʧ������ʱ��C�����              1D
	3,//����1�Σ�A��ʧ������ʱ��C���й�����          1E
	3,//����1�Σ�A��ʧ������ʱ��C���޹�����          1F
	2,//����1�Σ�A��ʧ������ʱ��C�๦������          20
	6,//����1�Σ�A��ʧ������ʱ��                     21
	4,//����1�Σ�A��ʧ������ʱ�������й��ܵ���       22
	4,//����1�Σ�A��ʧ������ʱ�̷����й��ܵ���       23
	4,//����1�Σ�A��ʧ������ʱ������޹�1�ܵ���      24
	4,//����1�Σ�A��ʧ������ʱ������޹�2�ܵ���      25
	4,//����1�Σ�A��ʧ������ʱ��A�������й�����      26
	4,//����1�Σ�A��ʧ������ʱ��A�෴���й�����      27
	4,//����1�Σ�A��ʧ������ʱ��A������޹�1����     28
	4,//����1�Σ�A��ʧ������ʱ��A������޹�2����     29
	4,//����1�Σ�A��ʧ������ʱ��B�������й�����      2A
	4,//����1�Σ�A��ʧ������ʱ��B�෴���й�����      2B
	4,//����1�Σ�A��ʧ������ʱ��B������޹�1����     2C
	4,//����1�Σ�A��ʧ������ʱ��B������޹�2����     2D
	4,//����1�Σ�A��ʧ������ʱ��C�������й�����      2E
	4,//����1�Σ�A��ʧ������ʱ��C�෴���й�����      2F
	4,//����1�Σ�A��ʧ������ʱ��C������޹�1����     30
	4,//����1�Σ�A��ʧ������ʱ��C������޹�2����     31
};

//�����--ͬʧѹ��
const INT8U g_nACDX[53] =
{
	6,//����1�Σ�A��ʧѹ����ʱ��                   01
	4,//����1�Σ�A��ʧѹ����ʱ�������й��ܵ���     02
	4,//����1�Σ�A��ʧѹ����ʱ�̷����й��ܵ���     03
	4,//����1�Σ�A��ʧѹ����ʱ������޹�1�ܵ���    04
	4,//����1�Σ�A��ʧѹ����ʱ������޹�2�ܵ���    05
	4,//����1�Σ�A��ʧѹ����ʱ��A�������й�����    06
	4,//����1�Σ�A��ʧѹ����ʱ��A�෴���й�����    07
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�1����   08
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�2����   09
	2,//����1�Σ�A��ʧѹ����ʱ��A���ѹ            0A
	3,//����1�Σ�A��ʧѹ����ʱ��A�����            0B
	3,//����1�Σ�A��ʧѹ����ʱ��A���й�����        0C
	3,//����1�Σ�A��ʧѹ����ʱ��A���޹�����        0D
	2,//����1�Σ�A��ʧѹ����ʱ��A�๦������        0E
	4,//����1�Σ�A��ʧѹ����ʱ��B�������й�����    0F
	4,//����1�Σ�A��ʧѹ����ʱ��B�෴���й�����    10
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�1����   11
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�2����   12
	2,//����1�Σ�A��ʧѹ����ʱ��B���ѹ            13
	3,//����1�Σ�A��ʧѹ����ʱ��B�����            14
	3,//����1�Σ�A��ʧѹ����ʱ��B���й�����        15
	3,//����1�Σ�A��ʧѹ����ʱ��B���޹�����        16
	2,//����1�Σ�A��ʧѹ����ʱ��B�๦������        17
	4,//����1�Σ�A��ʧѹ����ʱ��C�������й�����    18
	4,//����1�Σ�A��ʧѹ����ʱ��C�෴���й�����    19
	4,//����1�Σ�A��ʧѹ����ʱ��C������޹�1����   1A
	4,//����1�Σ�A��ʧѹ����ʱ��C������޹�2����   1B
	2,//����1�Σ�A��ʧѹ����ʱ��C���ѹ            1C
	3,//����1�Σ�A��ʧѹ����ʱ��C�����            1D
	3,//����1�Σ�A��ʧѹ����ʱ��C���й�����        1E
	3,//����1�Σ�A��ʧѹ����ʱ��C���޹�����        1F
	2,//����1�Σ�A��ʧѹ����ʱ��C�๦������        20
	4,//����1�Σ�A��ʧѹ�ڼ��ܰ�ʱ��               21
	4,//����1�Σ�A��ʧѹ�ڼ�A�లʱ��              22
	4,//����1�Σ�A��ʧѹ�ڼ�B�లʱ��              23
	4,//����1�Σ�A��ʧѹ�ڼ�C�లʱ��              24
	6,//����1�Σ�A��ʧѹ����ʱ��                   25
	4,//����1�Σ�A��ʧѹ����ʱ�������й��ܵ���     26
	4,//����1�Σ�A��ʧѹ����ʱ�̷����й��ܵ���     27
	4,//����1�Σ�A��ʧѹ����ʱ������޹�1�ܵ���    28
	4,//����1�Σ�A��ʧѹ����ʱ������޹�2�ܵ���    29
	4,//����1�Σ�A��ʧѹ����ʱ��A�������й�����    2A
	4,//����1�Σ�A��ʧѹ����ʱ��A�෴���й�����    2B
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�1����   2C
	4,//����1�Σ�A��ʧѹ����ʱ��A������޹�2����   2D
	4,//����1�Σ�A��ʧѹ����ʱ��B�������й�����    2E
	4,//����1�Σ�A��ʧѹ����ʱ��B�෴���й�����    2F
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�1����   30
	4,//����1�Σ�A��ʧѹ����ʱ��B������޹�2����   31
	4,//����1�Σ�A��ʧѹ����ʱ��C�������й�����    32
	4,//����1�Σ�A��ʧѹ����ʱ��C�෴���й�����    33
	4,//����1�Σ�A��ʧѹ����ʱ��C������޹�1����   34
	4 //����1�Σ�A��ʧѹ����ʱ��C������޹�2����   35
};

//��ѹ�������
const INT8U g_nACDYNX[34] =
{
	6,// ����1�Σ���ѹ��������ʱ��                    01
	4,// ����1�Σ���ѹ��������ʱ�������й��ܵ���      02
	4,// ����1�Σ���ѹ��������ʱ�̷����й��ܵ���      03
	4,// ����1�Σ���ѹ��������ʱ������޹�1�ܵ���     04
	4,// ����1�Σ���ѹ��������ʱ������޹�2�ܵ���     05
	4,// ����1�Σ���ѹ��������ʱ��A�������й�����     06
	4,// ����1�Σ���ѹ��������ʱ��A�෴���й�����     07
	4,// ����1�Σ���ѹ��������ʱ��A������޹�1����    08
	4,// ����1�Σ���ѹ��������ʱ��A������޹�2����    09
	4,// ����1�Σ���ѹ��������ʱ��B�������й�����     0A
	4,// ����1�Σ���ѹ��������ʱ��B�෴���й�����     0B
	4,// ����1�Σ���ѹ��������ʱ��B������޹�1����    0C
	4,// ����1�Σ���ѹ��������ʱ��B������޹�2����    0D
	4,// ����1�Σ���ѹ��������ʱ��C�������й�����     0E
	4,// ����1�Σ���ѹ��������ʱ��C�෴���й�����     0F
	4,// ����1�Σ���ѹ��������ʱ��C������޹�1����    10
	4,// ����1�Σ���ѹ��������ʱ��C������޹�2����    11
	6,// ����1�Σ���ѹ���������ʱ��                    12
	4,// ����1�Σ���ѹ���������ʱ�������й��ܵ���      13
	4,// ����1�Σ���ѹ���������ʱ�̷����й��ܵ���      14
	4,// ����1�Σ���ѹ���������ʱ������޹�1�ܵ���     15
	4,// ����1�Σ���ѹ���������ʱ������޹�2�ܵ���     16
	4,// ����1�Σ���ѹ���������ʱ��A�������й�����     17
	4,// ����1�Σ���ѹ���������ʱ��A�෴���й�����     18
	4,// ����1�Σ���ѹ���������ʱ��A������޹�1����    19
	4,// ����1�Σ���ѹ���������ʱ��A������޹�2����    1A
	4,// ����1�Σ���ѹ���������ʱ��B�������й�����     1B
	4,// ����1�Σ���ѹ���������ʱ��B�෴���й�����     1C
	4,// ����1�Σ���ѹ���������ʱ��B������޹�1����    1D
	4,// ����1�Σ���ѹ���������ʱ��B������޹�2����    1E
	4,// ����1�Σ���ѹ���������ʱ��C�������й�����     1F
	4,// ����1�Σ���ѹ���������ʱ��C�෴���й�����     20
	4,// ����1�Σ���ѹ���������ʱ��C������޹�1����    21
	4 // ����1�Σ���ѹ���������ʱ��C������޹�2����    22
};



/**************************************************************************************************
                                           
***************************************************************************************************/
static INT16U  	g_nMeterMp[MAX_METER_NUM]						= {0};
static INT16U  	g_nImMeterSeq[MAX_IM_METER_NUM]				= {0};

static INT8U  	g_nMeterRdFlags[MAX_METER_NUM]					= {0};
static INT8U	g_nMeterBlock[(MAX_METER_NUM - 1) / 8 + 1]	= {0};				//0,���Գ�����1�鳭



/**************************************************************************************************
                                         ����
***************************************************************************************************/
#if 0

#endif

/********************************************************************************
* ������:GetItemMapRdTdPtr
* �䡡��:
*		INT8U					nRdTd 		ҵ���������ͣ��ǰ���λֵ  RD_REAL��RD_DAY
* ��  ��:
* ��  ��:�������
* ��  ��:
*********************************************************************************/
static INT8S GetItemMapRdTdPtr(INT8U nRdTd, INT8U* pItemMap)
{
	INT32S			i 		= 0;
	INT16U*			pMtCfg	= NULL;
	
	if ((NULL == pItemMap))
	{
		return -1;
	}

	if (nRdTd == RD_REAL)
	{
		pMtCfg = (INT16U*)g_nRealMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else if (nRdTd == RD_DAY)
	{
		pMtCfg = (INT16U*)g_nDayMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else if (nRdTd == RD_MON)
	{
		pMtCfg = (INT16U*)g_nMonMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	#if 0
	else if (nRdTd == RD_CRV)
	{
		pMtCfg = (INT16U*)g_nCrvMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	#endif
	else if (nRdTd == RD_RDAY)
	{
		pMtCfg = (INT16U*)g_nRDayMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else
	{
		return -1;
	}
}


/********************************************************************************
* ������:GetItemMapRdFlagsPtr
* �䡡��:
*		INT8U					RdFlags 		ҵ���������ͣ��ǰ���λ?
* ��  ��:
* ��  ��:�������
* ��  ��:
*********************************************************************************/
static INT8S GetItemMapRdFlagsPtr(INT8U nRdFlags, INT8U* pItemMap)
{
	INT32S	i				= 0;

	if ((NULL == pItemMap))
	{
		return -1;
	}

	memset(pItemMap,0 ,DATA_ITEM_MAP_LEN);
	for (i = 0; i < RD_TD_NUM; i++)
	{
		if (TestBufBit(&nRdFlags, 1, i))
		{
			GetItemMapRdTdPtr(i, pItemMap);
		}
	}

	return 0;
}


/********************************************************************************
* ������:GetItemMapImUserPtr
* �䡡��:
*		INT8U					nImUser 		
* ��  ��:
* ��  ��:�������
* ��  ��:
*********************************************************************************/
static INT8S GetItemMapImUserPtr(INT8U nImUser, INT8U* pItemMap)
{
	INT32S			i 		= 0;
	INT16U*			pMtCfg	= NULL;
	
	if ((NULL == pItemMap))
	{
		return -1;
	}

	memset(pItemMap,0 ,DATA_ITEM_MAP_LEN);
	if (nImUser)
	{
		pMtCfg = (INT16U*)g_nImpUserMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else
	{
		pMtCfg = (INT16U*)g_nNormUserMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
}



/********************************************************************************
* ������:GetItemMapRs485Ptr
* �䡡��:
*		INT8U					nRs485 		ҵ���������ͣ�
* ��  ��:
* ��  ��:�������
* ��  ��:
*********************************************************************************/
static INT8S GetItemMapRs485Ptr(INT8U nRs485, INT8U* pItemMap)
{
	INT32S			i 		= 0;
	INT16U*			pMtCfg	= NULL;
	
	if ((NULL == pItemMap))
	{
		return -1;
	}

	memset(pItemMap,0 ,DATA_ITEM_MAP_LEN);
	if (nRs485)
	{
		pMtCfg = (INT16U*)g_nRs485MtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else
	{
		pMtCfg = (INT16U*)g_nCarrierMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
}


/********************************************************************************
* ������:GetTmpDataItem
* �䡡��:
*       pDATA_ITEM 				pDataItemS       	��ǰλͼ
*		pDATA_ITEM 				pDataItemD	
* ��  ��:
* ��  ��:�������
* ��  ��:����ʱ-1;��ʱ0  pDataItem ----> pDataItemTmp
*********************************************************************************/
static INT32S GetTmpDataItem(const pDATA_ITEM pDataItemS, pDATA_ITEM pDataItemD)
{
	pDATA_ITEM	pDataItem1 = pDataItemS;
	
	if (NULL == pDataItemS)
	{
		return -1;
	}
	if (NULL == pDataItemD)
	{
		return -1;
	}
	if (COL_END_ITEM == pDataItemS->nStdItem)
	{
		return -1;
	}
	memmove(pDataItemD, pDataItemS, sizeof(DATA_ITEM));
	pDataItem1 += pDataItemS->nItemNum;
	pDataItemD->nItemLen = pDataItem1->nOffset - pDataItemS->nOffset;

	return 0;
}



#if 0
#endif
/********************************************************************************
* ������:GetItemMap
* �䡡��:
*       INT8U                  	nRdFlags         	ҵ���������ͣ�����ֵ�����֣����ǰ���λ��
*		INT8U					nIsImUser			
*		INT8U					nIsRs485
* ��  ��:pItems      �������б�
* ��  ��:�������
* ��  ��:��ò����㳭���ı�׼�������б�
*********************************************************************************/
INT32S GetItemMap(INT8U nRdFlags, INT8U nIsImUser, INT8U nIsRs485, INT8U* pItemMap)
{
    INT16U          i           					= 0;
    INT8U           bsItemMap[DATA_ITEM_MAP_LEN] 	= {0};
	INT16U			nSize							= sizeof(bsItemMap);

    if( (NULL == pItemMap))
    {
        return -1;
    }

    memset(pItemMap, 0, nSize);

	GetItemMapRdFlagsPtr(nRdFlags, &bsItemMap[0]);
	memcpy(pItemMap, &bsItemMap[0], nSize);
	
	GetItemMapImUserPtr(nIsImUser, &bsItemMap[0]);
	for (i = 0; i < nSize; i++)
	{
		pItemMap[i] &= bsItemMap[i];
	}
	
	GetItemMapRs485Ptr(nIsRs485, &bsItemMap[0]);
	for (i = 0; i < nSize; i++)
	{
		pItemMap[i] &= bsItemMap[i];
	}

	return 0;
}


/********************************************************************************
* ������:IsCurrRdItem
* �䡡��:
*       INT8U* 					pItemMap        	��ǰλͼ
*		INT32U					nStdItem		
* ��  ��:
* ��  ��:�������
* ��  ��:����ʱ-1;��ʱ0
*********************************************************************************/
INT32S IsCurrRdItem(INT8U* pItemMap, INT32U nStdItem)
{
	if (TestBufBit(pItemMap, DATA_ITEM_MAP_LEN, nStdItem))
	{
		return 0;
	}
	else
	{
		return -1;	
	}
}


#if 0
#endif
/********************************************************************************
* ������:Gb645ToColData
* �䡡��:

* ��  ��:
* ��  ��:�������
* ��  ��:
*********************************************************************************/
INT32S Gb645ToColData(INT8U nProt, INT8U* pGb645Frm, void* pDataItem, void* pColData)
{
	pDATA_ITEM		pDataItemTmp 	= (pDATA_ITEM)pDataItem;
	DATA_ITEM		sDataItem;	
	INT8U*			pColDataTmp 	= (INT8U*)pColData;
	INT8U			nDiLen			= 0;
    INT8U           i               = 0;
	
	if (NULL == pGb645Frm)
	{
		return -1;
	}
	if (NULL == pDataItem)
	{
		return -1;
	}
	if (NULL == pColData)
	{
		return -1;
	}
	if (GB_97_PROT == nProt)
	{
		nDiLen = 2;
	}
	else
	{
		nDiLen = 4;
	}
	if (GetTmpDataItem(pDataItemTmp, &sDataItem) < 0)
	{
		return -1;
	}
    
    for (i = 0; i < sDataItem.nItemLen; i++)
    {
        pGb645Frm[METER_645_BASE_LEN + nDiLen + i] -= 0x33;
    }

	memmove(&pColDataTmp[sDataItem.nOffset], &pGb645Frm[METER_645_BASE_LEN + nDiLen], sDataItem.nItemLen);
	
	return sDataItem.nItemLen;
}

/********************************************************************************
* ������:MeterMpInit
* �䡡��:
* �䡡��:
* ��  ��:0
* ��  ��: ������ʼ��ʱ����
*********************************************************************************/
INT32S MeterMpInit(void)
{
	memset(&g_nMeterMp[0], 0, sizeof(g_nMeterMp));
	memset(&g_nImMeterSeq[0], 0, sizeof(g_nImMeterSeq));
	memset(&g_nMeterRdFlags[0], 0, sizeof(g_nMeterRdFlags));
	memset(&g_nMeterBlock[0], 0, sizeof(g_nMeterBlock));
	
	return 0;
}

/********************************************************************************
* ������:MeterMpMap
* �䡡��:
*			INT8U			nType				1ʱ������SetRdMeterFlags������ʵʱ����
* �䡡��:
* ��  ��:0
* ��  ��: �ϵ�ʱ���á�����F10��������
*********************************************************************************/
INT32S MeterMpMap(INT8U nType)
{
	INT32S				i					= 0;
	INT16U				nSeq				= 0;
	INT8U				nBuf[GET_MAX(sizeof(METER_INFO), MAX_IM_METER_NUM * 2 + 1)] = {0};
	INT32U				nSize				= sizeof(nBuf);
	pMETER_INFO			pMeterInfo			= (pMETER_INFO)(&nBuf[0]);
	INT8U*				pData 				= &nBuf[0];
	
	MeterMpInit();
	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		if (GetMeterInfo(nSeq, pMeterInfo) < 0)
		{
			continue;
		}
		g_nMeterMp[i] = pMeterInfo->nMp;	

		if (nType)
		{
			SetRdMeterFlags(nSeq, RD_REAL);
		}
	}
    //RdMeterSemProc();

	memset(pData, 0, nSize);
	MemReadOtherPrm(F35, pData, nSize);
	memset(&g_nImMeterSeq[0], 0, sizeof(g_nImMeterSeq));
	for (i = 0; i < GET_MIN(MAX_IM_METER_NUM, pData[0]); i++)
	{
		g_nImMeterSeq[i] = Buf2INT16U(&pData[1 + i * 2]);
	}

	return 0;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ȫ��������
*********************************************************************************/
INT8S SetRdMeterFlagsAllMp(INT8U nRdTd)
{
	INT16U				i 				= 0;
    INT16U              nSeq            = 0;
	METER_INFO			sMeterInfo;
	pMETER_INFO			pMeterInfo		= &sMeterInfo;

	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		if (GetMeterInfo(nSeq, pMeterInfo) < 0)
		{
			continue;
		}
		SetRdMeterFlags(nSeq, nRdTd);
	}

	return RET_ERR_OK;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ȫ��������
*********************************************************************************/
INT8S ClrRdMeterFlagsAllMp(INT8U nRdTd)
{
	INT16U				i 				= 0;
    INT16U              nSeq            = 0;
	METER_INFO			sMeterInfo;
	pMETER_INFO			pMeterInfo		= &sMeterInfo;

	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		if (GetMeterInfo(nSeq, pMeterInfo) < 0)
		{
			continue;
		}
		ClrRdMeterFlags(nSeq, nRdTd);
	}

	return RET_ERR_OK;
}



/********************************************************************************
* ������:SetRdMeterFlags
* �䡡��:
*				INT16U 				nSeq			���
*				INT8U				nRdTd			RD_REAL/RD_DAY
* �䡡��:
* ��  ��:0
* ��  ��: 
*********************************************************************************/
INT32S SetRdMeterFlags(INT16U nSeq, INT8U nRdTd)
{
	INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	
	nRdFlags = g_nMeterRdFlags[nSeq - 1];
	SetBufBit(&nRdFlags, 1, nRdTd);
	g_nMeterRdFlags[nSeq - 1] = nRdFlags;

	return 0;
}

/********************************************************************************
* ������:ClrRdMeterFlags
* �䡡��:
*				INT16U 				nSeq			���
*				INT8U				nRdTd			RD_REAL/RD_DAY
* �䡡��:
* ��  ��:0
* ��  ��: 
*********************************************************************************/
INT32S ClrRdMeterFlags(INT16U nSeq, INT8U nRdTd)
{
	INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	
	nRdFlags = g_nMeterRdFlags[nSeq - 1];
	ClrBufBit(&nRdFlags, 1, nRdTd);
	g_nMeterRdFlags[nSeq - 1] = nRdFlags;

	return 0;
}

/********************************************************************************
* ������:GetRdMeterFlags
* �䡡��:
*				INT16U 				nSeq			���
* �䡡��:
* ��  ��:0
* ��  ��: 
*********************************************************************************/
INT32S GetRdMeterFlags(INT16U nSeq, INT8U* pRdFlags)
{
	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	if (NULL != pRdFlags)
	{
		*pRdFlags = g_nMeterRdFlags[nSeq - 1];
	}
	
	return 0;
}


/********************************************************************************
* ������:SetBlockFlg
* �䡡��:
*				INT16U 				nSeq			���
* �䡡��:
* ��  ��:0
* ��  ��: 
*********************************************************************************/
INT32S SetBlockFlg(INT16U nSeq, INT8U nBlk)
{
	//INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	if (nBlk)
	{
		SetBufBit(&g_nMeterBlock[0], sizeof(g_nMeterBlock), nSeq - 1);
	}
	else
	{
		ClrBufBit(&g_nMeterBlock[0], sizeof(g_nMeterBlock), nSeq - 1);
	}
	//g_nMeterBlock[nSeq - 1] = nBlk;

	return 0;
}

/********************************************************************************
* ������:GetBlockFlg
* �䡡��:
*				INT16U 				nSeq			���
* �䡡��:
* ��  ��:0,���ԣ�1���鳭
* ��  ��: 
*********************************************************************************/
INT32S GetBlockFlg(INT16U nSeq)
{
	INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return 0;
	}
	//nRdFlags = g_nMeterBlock[nSeq - 1];
	//if (nRdFlags > 1)
	//{
	//	nRdFlags = 0;
	//}
	if (TestBufBit(&g_nMeterBlock[0], sizeof(g_nMeterBlock), nSeq - 1))
	{
		nRdFlags = 1;
	}
	else
	{
		nRdFlags = 0;
	}

	return nRdFlags;
}

/********************************************************************************
* ������:GetSeqFromAddr
* �䡡��:
*				INT16U 				nMp			������
* �䡡��:
* ��  ��:���   0,��Ч
* ��  ��: 
*********************************************************************************/
INT32S GetSeqFromAddr(INT8U* pAddr)
{
	INT16U          i = 0;
    INT16U			nSeq	= 0;
    MEM_METER_PRM   sMeterPrm;
    pMEM_METER_PRM  pMeterPrm = &sMeterPrm;

	for (i = 0; i < MAX_METER_NUM; i++)
    {
        nSeq = i + 1;
        MemReadMeterPrm(nSeq, F10, pMeterPrm, sizeof(MEM_METER_PRM));
        if(0 == memcmp(pAddr, &pMeterPrm->nAddr[0], 6))
        {
            return nSeq;
        }
    }
	return -1;
}

/********************************************************************************
* ������:GetMpFromSeq
* �䡡��:
*				INT16U 				nMp			������
* �䡡��:
* ��  ��:���   0,��Ч
* ��  ��: 
*********************************************************************************/
INT16U GetMpFromSeq(INT16U nSeq)
{
	INT16U			nMp	= 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return 0;
	}
	// ��ȡRam  �ϵ�ʱ�γ�ӳ��
	nMp = g_nMeterMp[nSeq - 1];
	if (!ChkRange(nMp, 1, MAX_MP))
	{
		return 0;
	}
	
	return nMp;
}


/********************************************************************************
* ������:GetSeqFromMp
* �䡡��:
*				INT16U 				nMp			������
* �䡡��:
* ��  ��:���   0,��Ч
* ��  ��: 
*********************************************************************************/
INT16U GetSeqFromMp(INT16U nMp)
{
	INT32S			i 		= 0;
	INT16U			nMpTmp	= 0;
	INT16U			nSeq	= 0;
	
	if (!ChkRange(nMp, 1, MAX_MP))
	{
		return 0;
	}

	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		nMpTmp = GetMpFromSeq(nSeq);
		if (nMpTmp == nMp)
		{
			return nSeq;
		}
	}

	return 0;
}

/********************************************************************************
* ������:GetSeqFromMp
* �䡡��:
*				INT16U 				nMp			������
* �䡡��:
* ��  ��:���   0,��Ч
* ��  ��: 
*********************************************************************************/
INT16U GetImSeqFromSeq(INT16U nSeq)
{
	INT32S			i 			= 0;
	INT16U			nSeqTmp		= 0;
	
	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return 0;
	}

	for (i = 0; i < MAX_IM_METER_NUM; i++)
	{
		nSeqTmp = g_nImMeterSeq[i];
		if (nSeqTmp == nSeq)
		{
			return i + 1;
		}
	}

	return 0;
}


/********************************************************************************
* ������:GetMeterInfo
* �䡡��:
*				INT16U 				nSeq			���
* �䡡��:
*				void* 				pInfo			pMETER_INFO
* ��  ��:0
* ��  ��: 
*********************************************************************************/
INT32S GetMeterInfo(INT16U nSeq, void* pInfo)
{
	pMETER_INFO		pMeterInfo = (pMETER_INFO)(pInfo);
	INT8U 			nRdFlags 	= 0;
	MEM_METER_PRM	sMeterPrm;
	pMEM_METER_PRM	pMeterPrm	= &sMeterPrm;
	INT16U			nTmp		= 0;
    INT32U          nBaud       = 0;
	
	memset(pMeterInfo, 0, sizeof(METER_INFO));

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	memset(pMeterPrm, 0, sizeof(MEM_METER_PRM));
	// ����Ʋ���
	MemReadMeterPrm(nSeq, F10, pMeterPrm, sizeof(MEM_METER_PRM));
	nTmp = Buf2INT16U(&pMeterPrm->nSeq[0]);
	if (nTmp != nSeq)
	{
		return -1;
	}
	nTmp = Buf2INT16U(&pMeterPrm->nMp[0]);
	if (!ChkRange(nTmp, 1, MAX_MP))
	{
		return -1;
	}
	pMeterInfo->nMp  = nTmp;
	pMeterInfo->nSeq = nSeq;
	memcpy(&pMeterInfo->nMeterAddr[0], &pMeterPrm->nAddr[0], sizeof(pMeterInfo->nMeterAddr));
	//pMeterInfo->nProt	= 0;
	//pMeterInfo->nPort	= 2;
    pMeterInfo->nProt = pMeterPrm->nProt;
    pMeterInfo->nPort = pMeterPrm->nCom;

	// ������
	GetRdMeterFlags(nSeq, &nRdFlags);
	pMeterInfo->nRdFlgs = nRdFlags;
    
    nBaud = Buf2Long(&pMeterPrm->nBaud[0], 4);

	return nBaud;
}


/********************************************************************************
* ������:GetPulsePrm
* �䡡��:
*				INT16U 				nSeq			���
* �䡡��:
*				
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S GetPulsePrm(INT16U nSeq, pMEM_PULSE_PRM pPulsePrm)
{
    INT16U			nTmp		= 0;
    
    if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
    
    memset(pPulsePrm, 0, sizeof(MEM_PULSE_PRM));
	//���������
	MemReadGrpPrm(F11, nSeq, pPulsePrm, sizeof(MEM_PULSE_PRM));
    if (!pPulsePrm->bVld)
    {
        return -1;
    }
	//nTmp = Buf2INT16U(&pPulsePrm->nPulseMp[0]);
    nTmp = pPulsePrm->nPulseMp;
	if (!ChkRange(nTmp, 1, MAX_MP))
	{
		return -1;
	}
    return 0;
}



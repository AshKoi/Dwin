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
#ifndef _DATA_ITEM_H_
#define _DATA_ITEM_H_

#include "Os_cpu.h"
#include "AppCfg.h"
#include "RX8025T.h"
#include "Macro.h"
#include "Table.h"

/**************************************************************************************************
                                     �����궨��
***************************************************************************************************/
#define 	INVLD_ITEM              		0xFFFFFFFF          					//��Ч��
#define  	INVLD_PROT_ITEM         		0xFFFFFFFF
#define  	INVLD_DATA_ITEM         		0xFFFF              					//��Ч��Լ��
#define 	DATA_ITEM_MAP_LEN       		64                  					//����λͼ����
#define 	DATA_ITEM_LEN           		(DATA_ITEM_MAP_LEN * 8)                 //�������



/**************************************************************************************************
                                  ���ݱ�׼��궨��
***************************************************************************************************/
//ҵ�������±�(ע����׼�������޸�ʱ��ر�֤�±�ͬ��)
#define     DATA_ITEM_REAL_START    		RD_ZXYGJH           	//ʵʱ���ݿ�ʼ�±�
#define     DATA_ITEM_REAL_END      		RD_ZJYCWGDKSJ			//ʵʱ���ݽ����±�
#define     DATA_ITEM_DAY_START     		DD_ZXYGJH           	//�����ݿ�ʼ�±�
#define     DATA_ITEM_DAY_END       		DD_FXYGXLJH         	//�����ݽ����±�
#define     DATA_ITEM_MON_START     		MD_ZXYGJH           	//�����ݿ�ʼ�±�
#define     DATA_ITEM_MON_END       		MD_FXYGXLJH         	//�����ݽ����±�



//ҵ���׼������:���ܳ���256��
enum _tagCOL_DATA_ITEM_TBL
{
    //********************************************************************************
    //					       ʵʱ���ݱ�׼��
    //********************************************************************************
    RD_ZHYGJH=0,			//(0000FF00)����й�����ʾֵ����
    RD_ZXYGJH,			    //(0001FF00)�����й�����ʾֵ����
    RD_FXYGJH,				//(0002FF00)�����й�����ʾֵ����
    RD_ZXWGJH,				//(0003FF00)����޹�1(����)����ʾֵ����
    RD_FXWGJH,				//(0004FF00)����޹�2(����)����ʾֵ����

    RD_X1WGJH,              //(0005FF00)һ�����޹�����ʾֵ����
    RD_X2WGJH,              //(0006FF00)�������޹�����ʾֵ����
    RD_X3WGJH,              //(0007FF00)�������޹�����ʾֵ����
    RD_X4WGJH,              //(0008FF00)�������޹�����ʾֵ����

    RD_ZXSZJH,			    //(0009FF00)�������ڵ���ʾֵ����
    RD_FXSZJH,				//(000AFF00)�������ڵ���ʾֵ����
    RD_GLZDNJH,				//(00800000)�����ܵ��ܼ���
    RD_AXDNJH,				//(00150000)A����ܼ���
    RD_BXDNJH,				//(00290000)B����ܼ���
    RD_CXDNJH,				//(003D0000)C����ܼ���

    RD_ZXYGXLJH,            //(0101FF00)�����й��������������ʱ�伯��
    RD_FXYGXLJH,            //(0102FF00)�����й��������������ʱ�伯��
    RD_ZXWGXLJH,            //(0103FF00)����޹�1�������������ʱ�伯��
    RD_FXWGXLJH,            //(0104FF00)����޹�2�������������ʱ�伯��

    RD_DYJH,              	//(0201FF00)��ѹ����
    RD_DLJH,                //(0202FF00)��������
    RD_YGGLJH,              //(0203FF00)�й����ʼ���
    RD_WGGLJH,              //(0204FF00)�޹����ʼ���
    RD_SZGLJH,              //(0205FF00)���ڹ��ʼ���
    RD_GLYSJH,              //(0206FF00)������������
    RD_XWJJH,               //(0207FF00)��λ�Ǽ���
    RD_DYBXSZJH,            //(0208FF00)��ѹ����ʧ�漯��
    RD_DLBXSZJH,            //(0209FF00)��������ʧ�漯��

    RD_LXDL,                //(02800001)���ߵ���
    RD_DWPL,                //(02800002)����Ƶ��
    RD_YFZPJGL,             //(02800003)һ�����й�ƽ������
    RD_DQYGXL,              //(02800004)��ǰ�й�����
    RD_DQWGXL,              //(02800005)��ǰ�޹�����
    RD_DQSZXL,              //(02800006)��ǰ��������
    RD_BLWD,                //(02800007)�����¶�
    RD_LBSZDY,              //(02800008)ʱ�ӵ�ص�ѹ���ڲ���
    RD_WBSZDY,              //(02800009)ͣ�糭���ص�ѹ���ⲿ��
    RD_LBDCGZSJ,            //(0280000A)�ڲ���ع���ʱ��

    RD_SBLJH,               //(028001FF)����������

    RD_YGDBQSDS,			//(E1000001)�й����ܱ���ʼ����
    RD_WGDBQSDS,			//(E1000002)�޹����ܱ���ʼ����
    RD_DBYGCS,			    //(E1000003)�����(�й�)
    RD_DBWGCS,			    //(E1000004)�����(�޹�)

    RD_AXDYXBJH,            //(020A01FF)A���ѹг������
    RD_BXDYXBJH,            //(020A02FF)B���ѹг������
    RD_CXDYXBJH,            //(020A03FF)C���ѹг������
    RD_AXDLXBJH,            //(020B01FF)A�����г������
    RD_BXDLXBJH,            //(020B02FF)B�����г������
    RD_CXDLXBJH,            //(020B03FF)C�����г������

	RD_DBRQ,            	//(04000101)�������
    RD_DBSJ,            	//(04000102)���ʱ��

    RD_SYJH,				//(03010000)ʧѹ����
    RD_SLJH,				//(030B0000)ʧ������
    RD_DLFXJH,				//(030E0000)�����������

    RD_DBBCJL,              //(03300000)������ܴ���
    RD_DBYXZTZ,			    //(040005FF)�������״̬��
    RD_SYJE,			    //(E1000008)ʣ����

    RD_RDYHGLJH,			//(E1000010)�յ�ѹ�ϸ���ͳ��
    RD_RDLJXZJH,			//(E1000014)�յ�������ֵͳ��

    RD_RYGGLJXZJH,			//(E1000018)���й����ʼ���ֵͳ��
    RD_RWGGLJXZJH,			//(E1000019)���޹����ʼ���ֵͳ��
    RD_RGLYSJXZJH,			//(E100001A)�չ�����������ֵͳ��
    RD_RDLYXTJJH,			//(E100001B)�յ�����ƽ��Խ��ͳ��
    RD_RDYYXTJJH,			//(E100001C)�յ�ѹ��ƽ��Խ��ͳ��

    RD_RTDZCS,			    //(E1000020)��ͣ���ܴ���
    RD_RTDZSJ,			    //(E1000021)��ͣ����ʱ��
    RD_RDXSJJH,			    //(E1000030)�ն������ݼ���
    RD_RSYSJJH,			    //(E1000031)��ʧѹ���ݼ���
    RD_RSLSJJH,			    //(E1000032)��ʧ�����ݼ���

    RD_RAXDYBDZ,            //(0xE1000040)��A���ѹ����ֵ
    RD_RBXDYBDZ,            //(0xE1000041)��B���ѹ����ֵ
    RD_RCXDYBDZ,            //(0xE1000042)��C���ѹ����ֵ
    RD_RAXDYSBZ,            //(0xE1000043)��A���ѹ����ֵ
    RD_RBXDYSBZ,            //(0xE1000044)��B���ѹ����ֵ
    RD_RCXDYSBZ,            //(0xE1000045)��C���ѹ����ֵ
    RD_RDYDLZDZJH,          //(0xE10001FF)���յ�ѹ����ƫ�����ֵ
    RD_RDYDLZXZJH,          //(0xE10002FF)���յ�ѹ����ƫ����Сֵ
    RD_RDYDLPJZJH,          //(0xE10002FF)���յ�ѹ����ƫ��ƽ��ֵ

    RD_YDYHGLJH,			//(03100010)�µ�ѹ�ϸ���ͳ��
    RD_YAXDYHGLJH,			//(03100110)��A���ѹ�ϸ���ͳ��
    RD_YBXDYHGLJH,			//(03100210)��B���ѹ�ϸ���ͳ��
    RD_YCXDYHGLJH,			//(03100310)��C���ѹ�ϸ���ͳ��

    RD_YDLJXZJH,			//(E1004014)�µ�������ֵͳ��
    RD_YAXDLJXZJH,			//(E1004015)��A���������ֵͳ��
    RD_YBXDLJXZJH,			//(E1004016)��B���������ֵͳ��
    RD_YCXDLJXZJH,			//(E1004017)��C���������ֵͳ��

    RD_YYGGLJXZJH,			//(E1004018)���й����ʼ���ֵͳ��
    RD_YWGGLJXZJH,			//(E1004019)���޹����ʼ���ֵͳ��
    RD_YGLYSJXZJH,			//(E100401A)�¹�����������ֵͳ��
    RD_YDLYXTJJH,			//(E100401B)�µ�����ƽ��Խ��ͳ��
    RD_YDYYXTJJH,			//(E100401C)�µ�ѹ��ƽ��Խ��ͳ��

    RD_YTDZCS,			    //(E1004020)��ͣ���ܴ���
    RD_YTDZSJ,			    //(E1004021)��ͣ����ʱ��
    RD_YDXSJJH,			    //(E1004030)�¶������ݼ���
    RD_YSYSJJH,			    //(E1004031)��ʧѹ���ݼ���
    RD_YSLSJJH,			    //(E1004032)��ʧ�����ݼ���

    RD_DBBGDKCS,			//(F165)����Ǵ򿪴���
    RD_DBBGDKSJ1,			//(F165)����Ǵ��¼�1
    RD_DBBGDKSJ2,			//(F165)����Ǵ��¼�2
    RD_DBBGDKSJ3,			//(F165)����Ǵ��¼�3
    RD_DBBGDKSJ4,			//(F165)����Ǵ��¼�4
    RD_DBBGDKSJ5,			//(F165)����Ǵ��¼�5
    RD_DBBGDKSJ6,			//(F165)����Ǵ��¼�6
    RD_DBBGDKSJ7,			//(F165)����Ǵ��¼�7
    RD_DBBGDKSJ8,			//(F165)����Ǵ��¼�8
    RD_DBBGDKSJ9,			//(F165)����Ǵ��¼�9
    RD_DBBGDKSJ10,			//(F165)����Ǵ��¼�10
    RD_DBWGDKCS,			//(F165)���β�Ǵ򿪴���(��ť��)
    RD_DBWGDKSJ1,			//(F165)���β�Ǵ��¼�1(��ť��)
    RD_DBWGDKSJ2,			//(F165)���β�Ǵ��¼�2(��ť��)
    RD_DBWGDKSJ3,			//(F165)���β�Ǵ��¼�3(��ť��)
    RD_DBWGDKSJ4,			//(F165)���β�Ǵ��¼�4(��ť��)
    RD_DBWGDKSJ5,			//(F165)���β�Ǵ��¼�5(��ť��)
    RD_DBWGDKSJ6,			//(F165)���β�Ǵ��¼�6(��ť��)
    RD_DBWGDKSJ7,			//(F165)���β�Ǵ��¼�7(��ť��)
    RD_DBWGDKSJ8,			//(F165)���β�Ǵ��¼�8(��ť��)
    RD_DBWGDKSJ9,			//(F165)���β�Ǵ��¼�9(��ť��)
    RD_DBWGDKSJ10,			//(F165)���β�Ǵ��¼�10(��ť��)
    RD_ZJYCWGDKSJ,			//(F165)���һ��β�Ǵ�ʱ��


	//********************************************************************************
    //					    �����ݱ�׼��
    //********************************************************************************
	DD_FRZTIME,			//(05060001)����ʱ��

    DD_ZXYGJH,			    //(05060101)�����й�����ʾֵ����
    DD_FXYGJH,				//(05060201)�����й�����ʾֵ����
    DD_ZXWGJH,			    //(05060301)����޹�1�����򣩵���ʾֵ����
    DD_FXWGJH,			    //(05060401)����޹�2�����򣩵���ʾֵ����
    DD_X1WGJH,              //(05060501)һ�����޹�����ʾֵ����
    DD_X2WGJH,              //(05060601)�������޹�����ʾֵ����
    DD_X3WGJH,              //(05060701)�������޹�����ʾֵ����
    DD_X4WGJH,              //(05060801)�������޹�����ʾֵ����

    DD_ZXYGXLJH,            //(05060901)�����й��������������ʱ�伯��
    DD_FXYGXLJH,            //(05060A01)�����й��������������ʱ�伯��
    DD_ZXWGXLJH,            //(05060B01)����޹�1�������������ʱ�伯��
    DD_FXWGXLJH,            //(05060C01)����޹�2�������������ʱ�伯��

    DD_RDYHGLJH,			//(E1008010)�յ�ѹ�ϸ���ͳ��
    DD_RAXDYHGLJH,			//(E1008011)��A���ѹ�ϸ���ͳ��
    DD_RBXDYHGLJH,			//(E1008012)��B���ѹ�ϸ���ͳ��
    DD_RCXDYHGLJH,			//(E1008013)��C���ѹ�ϸ���ͳ��

    DD_RPLHGLJH,			//(E1008014)��Ƶ�ʺϸ���ͳ������
    DD_RAXDLJXZJH,			//(E1008015)��A���������ֵͳ��
    DD_RBXDLJXZJH,			//(E1008016)��B���������ֵͳ��
    DD_RCXDLJXZJH,			//(E1008017)��C���������ֵͳ��

    DD_RYGGLJXZJH,			//(E1008018)���й����ʼ���ֵͳ��
    DD_RWGGLJXZJH,			//(E1008019)���޹����ʼ���ֵͳ��
    DD_RGLYSJXZJH,			//(E100801A)�չ�����������ֵͳ��
    DD_RZXDLJZJH,			//(E100801B)�����������ֵͳ��
    DD_RFXDLJZJH,			//(E100801C)�ո��������ֵͳ��
    DD_RLXDLJZJH,			//(E100801D)�����������ֵͳ������

    DD_RDLYXTJJH,			//(E1008020)�յ�����ƽ��Խ��ͳ��
    DD_RDLHGLTJJH,			//(E1008021)�յ�����ƽ��ϸ���ͳ��

    DD_RAXDYSBHGL,          //(0xE1008022)��A���ѹ����ϸ���ͳ��
    DD_RBXDYSBHGL,          //(0xE1008023)��B���ѹ����ϸ���ͳ��
    DD_RCXDYSBHGL,          //(0xE1008024)��C���ѹ����ϸ���ͳ��
    DD_RAXDYJBHGL,          //(0xE1008025)��A���ѹ����ϸ���ͳ��
    DD_RBXDYJBHGL,          //(0xE1008026)��B���ѹ����ϸ���ͳ��
    DD_RCXDYJBHGL,          //(0xE1008027)��C���ѹ����ϸ���ͳ��

    DD_RAXDYBDZ,            //(0xE1008028)��A���ѹ����ֵ
    DD_RBXDYBDZ,            //(0xE1008029)��B���ѹ����ֵ
    DD_RCXDYBDZ,            //(0xE100802A)��C���ѹ����ֵ
    DD_RTDZCS,			    //(E1008030)��ͣ���ܴ���
    DD_RTDZSJ,			    //(E1008031)��ͣ����ʱ��

    DD_RDXSJJH,			    //(E1008040)�ն������ݼ���
    DD_RSYSJJH,			    //(E1008041)��ʧѹ���ݼ���
    DD_RSLSJJH,			    //(E1008042)��ʧ�����ݼ���

    DD_AXDYXBZDZJH,         //(E10081FF)A���ѹг�����ֵ
    DD_BXDYXBZDZJH,         //(E10082FF)B���ѹг�����ֵ
    DD_CXDYXBZDZJH,         //(E10083FF)C���ѹг�����ֵ
    DD_AXDLXBZDZJH,         //(E10084FF)A�����г�����ֵ
    DD_BXDLXBZDZJH,         //(E10085FF)B�����г�����ֵ
    DD_CXDLXBZDZJH,         //(E10086FF)C�����г�����ֵ

    DD_AXDYXBZXZJH,         //(E10087FF)A���ѹг����Сֵ
    DD_BXDYXBZXZJH,         //(E10088FF)B���ѹг����Сֵ
    DD_CXDYXBZXZJH,         //(E10089FF)C���ѹг����Сֵ
    DD_AXDLXBZXZJH,         //(E1008AFF)A�����г����Сֵ
    DD_BXDLXBZXZJH,         //(E1008BFF)B�����г����Сֵ
    DD_CXDLXBZXZJH,         //(E1008CFF)C�����г����Сֵ

    DD_AXDYXBPJZJH,         //(E10090FF)A���ѹг��ƽ��ֵ
    DD_BXDYXBPJZJH,         //(E10091FF)B���ѹг��ƽ��ֵ
    DD_CXDYXBPJZJH,         //(E10092FF)C���ѹг��ƽ��ֵ
    DD_AXDLXBPJZJH,         //(E10093FF)A�����г��ƽ��ֵ
    DD_BXDLXBPJZJH,         //(E10094FF)B�����г��ƽ��ֵ
    DD_CXDLXBPJZJH,         //(E10095FF)C�����г��ƽ��ֵ

    DD_AXDYXB95JH,          //(E10096FF)A���ѹг��95%ֵ
    DD_BXDYXB95JH,          //(E10097FF)B���ѹг��95%ֵ
    DD_CXDYXB95JH,          //(E10098FF)C���ѹг��95%ֵ
    DD_AXDLXB95JH,          //(E10099FF)A�����г��95%ֵ
    DD_BXDLXB95JH,          //(E1009AFF)B�����г��95%ֵ
    DD_CXDLXB95JH,          //(E1009BFF)C�����г��95%ֵ

    DD_DRQLJTRSJJH,         //(E1009C0F)������Ͷ���ۼ�ʱ��
    DD_DRQLJTRCSJH,         //(E1009C1F)������Ͷ���ۼƴ���
    DD_WGBCDNL,             //(E1009C20)�� �������޹�������

    DD_DRLDDLJH,            //(9090)����������������
    DD_S1RLDDLJH,           //(9091)��1��������������
    DD_S2RLDDLJH,           //(9092)��2��������������
    DD_S3RLDDLJH,           //(9093)��3��������������
    DD_S4RLDDLJH,           //(9094)��4��������������
    DD_S5RLDDLJH,           //(9095)��5��������������
    DD_S6RLDDLJH,           //(9096)��6��������������
    DD_S7RLDDLJH,           //(9097)��7��������������

    //********************************************************************************
    //					       �����ݱ�׼��
    //********************************************************************************

    MD_ZHYGJH,			    //(0000FF01)����й�����ʾֵ����
    MD_ZXYGJH,			    //(0001FF01)�����й�����ʾֵ����
    MD_FXYGJH,				//(0002FF01)�����й�����ʾֵ����
    MD_ZXWGJH,				//(0003FF01)�����޹�����ʾֵ����
    MD_FXWGJH,				//(0004FF01)�����޹�����ʾֵ����
    MD_X1WGJH,              //(0005FF01)һ�����޹�����ʾֵ����
    MD_X2WGJH,              //(0006FF01)�������޹�����ʾֵ����
    MD_X3WGJH,              //(0007FF01)�������޹�����ʾֵ����
    MD_X4WGJH,              //(0008FF01)�������޹�����ʾֵ����

    MD_ZXSZJH,			    //(0009FF01)�������ڵ���ʾֵ����
    MD_FXSZJH,				//(000AFF01)�������ڵ���ʾֵ����
    MD_GLZDNJH,				//(00800001)�����ܵ��ܼ���
    MD_AXDNJH,				//(00150001)A����ܼ���
    MD_BXDNJH,				//(00290001)B����ܼ���
    MD_CXDNJH,				//(003D0001)C����ܼ���

    MD_ZXYGXLJH,            //(0101FF01)�����й��������������ʱ�伯��
    MD_FXYGXLJH,            //(0102FF01)�����й��������������ʱ�伯��
    MD_ZXWGXLJH,            //(0103FF01)����޹�1�������������ʱ�伯��
    MD_FXWGXLJH,            //(0104FF01)����޹�2�������������ʱ�伯��
    MD_X1WGXLJH,            //(0105FF01)һ�����޹��������������ʱ�伯��
    MD_X2WGXLJH,            //(0106FF01)�������޹��������������ʱ�伯��
    MD_X3WGXLJH,            //(0107FF01)�������޹��������������ʱ�伯��
    MD_X4WGXLJH,            //(0108FF01)�������޹��������������ʱ�伯��
    MD_ZXSZXLJH,			//(0109FF01)������������������ʱ�伯��
    MD_FXSZXLJH,			//(010AFF01)������������������ʱ�伯��
    MD_AXXLJH,				//(01150001)A������������ʱ�伯��
    MD_BXXLJH,				//(01290001)B������������ʱ�伯��
    MD_CXXLJH,				//(013D0001)C������������ʱ�伯��


    MD_YDYHGLJH,			//(E100C010)�µ�ѹ�ϸ���ͳ��
    MD_YAXDYHGLJH,			//(E100C011)��A���ѹ�ϸ���ͳ��
    MD_YBXDYHGLJH,			//(E100C012)��B���ѹ�ϸ���ͳ��
    MD_YCXDYHGLJH,			//(E100C013)��C���ѹ�ϸ���ͳ��

    MD_YPLHGLJH,			//(E100C014)��Ƶ�ʺϸ���ͳ������
    MD_YAXDLJXZJH,			//(E100C015)��A���������ֵͳ��
    MD_YBXDLJXZJH,			//(E100C016)��B���������ֵͳ��
    MD_YCXDLJXZJH,			//(E100C017)��C���������ֵͳ��

    MD_YYGGLJXZJH,			//(E100C018)���й����ʼ���ֵͳ��
    MD_YWGGLJXZJH,			//(E100C019)���޹����ʼ���ֵͳ��
    MD_YGLYSJXZJH,			//(E100C01A)�¹�����������ֵͳ��
    MD_YZXDLJZJH,			//(E100C01B)�����������ֵͳ��
    MD_YFXDLJZJH,			//(E100C01C)�¸��������ֵͳ��
    MD_YLXDLJZJH,			//(E100C01D)�����������ֵͳ������

    MD_YDLYXTJJH,			//(E100C020)�µ�����ƽ��Խ��ͳ��
    MD_YDLHGLTJJH,			//(E100C021)�µ�����ƽ��ϸ���ͳ��

    MD_YAXDYSBHGL,          //(0xE100C022)��A���ѹ����ϸ���ͳ��
    MD_YBXDYSBHGL,          //(0xE100C023)��B���ѹ����ϸ���ͳ��
    MD_YCXDYSBHGL,          //(0xE100C024)��C���ѹ����ϸ���ͳ��
    MD_YAXDYJBHGL,          //(0xE100C025)��A���ѹ����ϸ���ͳ��
    MD_YBXDYJBHGL,          //(0xE100C026)��B���ѹ����ϸ���ͳ��
    MD_YCXDYJBHGL,          //(0xE100C027)��C���ѹ����ϸ���ͳ��

    MD_YAXDYBDZ,            //(0xE100C028)��A���ѹ����ֵ
    MD_YBXDYBDZ,            //(0xE100C029)��B���ѹ����ֵ
    MD_YCXDYBDZ,            //(0xE100C02A)��C���ѹ����ֵ
    MD_YTDZCS,			    //(E100C030)��ͣ���ܴ���
    MD_YTDZSJ,			    //(E100C031)��ͣ����ʱ��

    MD_YDXSJJH,			    //(E100C040)�¶������ݼ���
    MD_YSYSJJH,			    //(E100C041)��ʧѹ���ݼ���
    MD_YSLSJJH,			    //(E100C042)��ʧ�����ݼ���

    MD_AXDYXBZDZJH,         //(E100C1FF)A���ѹг�����ֵ
    MD_BXDYXBZDZJH,         //(E100C2FF)B���ѹг�����ֵ
    MD_CXDYXBZDZJH,         //(E100C3FF)C���ѹг�����ֵ
    MD_AXDLXBZDZJH,         //(E100C4FF)A�����г�����ֵ
    MD_BXDLXBZDZJH,         //(E100C5FF)B�����г�����ֵ
    MD_CXDLXBZDZJH,         //(E100C6FF)C�����г�����ֵ

    MD_AXDYXBZXZJH,         //(E100C7FF)A���ѹг����Сֵ
    MD_BXDYXBZXZJH,         //(E100C8FF)B���ѹг����Сֵ
    MD_CXDYXBZXZJH,         //(E100C9FF)C���ѹг����Сֵ
    MD_AXDLXBZXZJH,         //(E100CAFF)A�����г����Сֵ
    MD_BXDLXBZXZJH,         //(E100CBFF)B�����г����Сֵ
    MD_CXDLXBZXZJH,         //(E100CCFF)C�����г����Сֵ

    MD_AXDYXBPJZJH,         //(E100D0FF)A���ѹг��ƽ��ֵ
    MD_BXDYXBPJZJH,         //(E100D1FF)B���ѹг��ƽ��ֵ
    MD_CXDYXBPJZJH,         //(E100D2FF)C���ѹг��ƽ��ֵ
    MD_AXDLXBPJZJH,         //(E100D3FF)A�����г��ƽ��ֵ
    MD_BXDLXBPJZJH,         //(E100D4FF)B�����г��ƽ��ֵ
    MD_CXDLXBPJZJH,         //(E100D5FF)C�����г��ƽ��ֵ

    MD_AXDYXB95JH,          //(E100D6FF)A���ѹг��95%ֵ
    MD_BXDYXB95JH,          //(E100D7FF)B���ѹг��95%ֵ
    MD_CXDYXB95JH,          //(E100D8FF)C���ѹг��95%ֵ
    MD_AXDLXB95JH,          //(E100D9FF)A�����г��95%ֵ
    MD_BXDLXB95JH,          //(E100DAFF)B�����г��95%ֵ
    MD_CXDLXB95JH,          //(E100DBFF)C�����г��95%ֵ

    MD_WGBCDNL,             //(E100DB20)�� �������޹�������

	//********************************************************************************
    //					     �������ݱ�׼��
    //********************************************************************************

    //
    CD_ZXYGZ1,				//��1��
    CD_ZXYGZ2,				//��2��
    CD_ZXYGZ3,				//��3��
    CD_ZXYGZ4,				//��4��
    CD_ZXYGZ5,				//��5��
    CD_ZXYGZ6,				//��6��
    CD_ZXYGZ7,				//��7��
    CD_ZXYGZ8,				//��8��
    CD_ZXYGZ9,				//��9��
    CD_ZXYGZ10,				//��10��
    CD_ZXYGZ11,				//��11��
    CD_ZXYGZ12,				//��12��
    CD_ZXYGZ13,				//��13��
    CD_ZXYGZ14,				//��14��
    CD_ZXYGZ15,				//��15��
    CD_ZXYGZ16,				//��16��
    CD_ZXYGZ17,				//��17��
    CD_ZXYGZ18,				//��18��
    CD_ZXYGZ19,				//��19��
    CD_ZXYGZ20,				//��20��
    CD_ZXYGZ21,				//��21��
    CD_ZXYGZ22,				//��22��
    CD_ZXYGZ23,				//��23��
    CD_ZXYGZ24,				//��24��
    CD_ZXYGZ25,				//��25��
    CD_ZXYGZ26,				//��26��
    CD_ZXYGZ27,				//��27��
    CD_ZXYGZ28,				//��28��
    CD_ZXYGZ29,				//��29��
    CD_ZXYGZ30,				//��30��
    CD_ZXYGZ31,				//��31��
    CD_ZXYGZ32,				//��32��
    CD_ZXYGZ33,				//��33��
    CD_ZXYGZ34,				//��34��
    CD_ZXYGZ35,				//��35��
    CD_ZXYGZ36,				//��36��
    CD_ZXYGZ37,				//��37��
    CD_ZXYGZ38,				//��38��
    CD_ZXYGZ39,				//��39��
    CD_ZXYGZ40,				//��40��
    CD_ZXYGZ41,				//��41��
    CD_ZXYGZ42,				//��42��
    CD_ZXYGZ43,				//��43��
    CD_ZXYGZ44,				//��44��
    CD_ZXYGZ45,				//��45��
    CD_ZXYGZ46,				//��46��
    CD_ZXYGZ47,				//��47��
    CD_ZXYGZ48,				//��48��


    CD_ZXYGZ,               //(9010)���й�����ʾֵ��
    CD_FXYGZ,				//(9020)�����й�����ʾֵ��
    CD_ZXWGZ,				//(9110)�����޹�����ʾֵ��
    CD_FXWGZ,				//(9120)�����޹�����ʾֵ��
    CD_X1WGZ,               //(9130)һ�����޹�����ʾֵ��
    CD_X2WGZ,               //(9150)�������޹�����ʾֵ��
    CD_X3WGZ,               //(9160)�������޹�����ʾֵ��
    CD_X4WGZ,               //(9140)�������޹�����ʾֵ��

    CD_DYJH,                //(B61F)��ѹ����
    CD_DLJH,                //(B62F)��������
    CD_YGGLJH,              //(B63F)�й����ʼ���
    CD_WGGLJH,              //(B64F)�޹����ʼ���
    CD_GLYSJH,              //(B65F)������������

    //********************************************************************************
    //					     �㲥��ʱ��׼��
    //********************************************************************************
    BROADCAST_TIME,            	//�㲥��ʱ
	//********************************************************************************
    //					     �ز�Уʱ��׼��
    //********************************************************************************
    CA_RD_DBRQ,                 //���ܱ�����
    CA_RD_DBSJ,
    //********************************************************************************
    //					     �������ñ�׼��
    //********************************************************************************
    SET_PRM_MP,              //����������

    //********************************************************************************
    //					      ����������
    //********************************************************************************
    COL_END_ITEM
};

/**************************************************************************************************
                                          ������ݽṹ����
***************************************************************************************************/
//�����������
typedef struct _tagDATA_ITEM
{
	INT32U	    nStdItem;	        	//��׼��
	INT32U	    nProtItem;		    	//��Լ��
	INT32U      nProtItem1;		    //��Լ��1
	INT32U		nOffset;
    INT32S      nItemLen;		    	//���ݳ���
	INT8U       nItemNum;		    	//����
}DATA_ITEM, *pDATA_ITEM;

// ����97������ݽṹ
typedef struct _tagCOL_97_DATA
{
    //INT8U       		        nMeterAddr[LEN_METER_ADDR];		//����ַ(��ǩ)
    STD_TIME    		        sReadTime;                      //����ʱ��
    //STD_TIME    		        sMeterTime;                     //�������ʱ��
    
	INT8U						nEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyNN[MAX_FEE_NUM + 1][4];

	INT8U                       nQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNNDemand[MAX_FEE_NUM + 1][8];
	
	INT8U                       nVolt[MAX_PHASE_NUM][2];
	INT8U                       nCurr[MAX_PHASE_NUM][3];
	INT8U						nPWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nNWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nSWatt[1 + MAX_PHASE_NUM][3];
	INT8U                       nFactor[1 + MAX_PHASE_NUM][2];

	//ʵʱ
	INT8U						nDate[4];										//�������
	INT8U						nTime[3];										//���ʱ��

	//��
	INT8U						nDayTime[5];									//�ն���ʱ��
	INT8U                       nDEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nDQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nDPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nDNPDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDPNDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDNNDemand[MAX_FEE_NUM + 1][8];

	//��
	INT8U                       nMEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nMQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nMPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNNDemand[MAX_FEE_NUM + 1][8];

	
}COL_97_DATA, *pCOL_97_DATA;

// ��07������ݽṹ
typedef struct _tagCOL_07_DATA
{
    //INT8U       		        nMeterAddr[LEN_METER_ADDR];		//����ַ(��ǩ)
    STD_TIME    		        sReadTime;                      //����ʱ��
    //STD_TIME    		        sMeterTime;                     //�������ʱ��
    
	INT8U						nEnergyPP[MAX_FEE_NUM + 1][4];	//���й�
	INT8U						nEnergyNP[MAX_FEE_NUM + 1][4];	//���й�
	INT8U						nEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyNN[MAX_FEE_NUM + 1][4];
						
	INT8U                       nQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNNDemand[MAX_FEE_NUM + 1][8];
	
	INT8U                       nVolt[MAX_PHASE_NUM][2];
	INT8U                       nCurr[MAX_PHASE_NUM][3];
	INT8U						nPWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nNWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nSWatt[1 + MAX_PHASE_NUM][3];
	INT8U                       nFactor[1 + MAX_PHASE_NUM][2];

	//ʵʱ
	INT8U						nDate[4];										//�������
	INT8U						nTime[3];										//���ʱ��

	//��
	INT8U						nDayTime[5];									//�ն���ʱ��
	INT8U                       nDEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nDQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nDPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nDNPDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDPNDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDNNDemand[MAX_FEE_NUM + 1][8];

	//��
	INT8U                       nMEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nMQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nMPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNNDemand[MAX_FEE_NUM + 1][8];

}COL_07_DATA, *pCOL_07_DATA;


// �������ݽṹ
typedef union _tagCOL_DATA
{
	COL_97_DATA					uCol97Data;
	COL_07_DATA					uCol07Data;
}COL_DATA, *pCOL_DATA;



//�����Ϣ
typedef struct _tagMETER_INFO
{
	INT16U					nSeq;							// 0ʱ��Ч
	INT16U					nMp;
	INT8U					nMeterAddr[6];
	INT8U					nProt;
	INT8U					nPort;							// ����˿�
	INT8U					nImUser;
	INT8U					nRdFlgs;						// �����־
}METER_INFO, *pMETER_INFO;

/**************************************************************************************************
                                         AC�¼�������
***************************************************************************************************/
#define         MAX_AC_SY_NUM           0x35                //ACʧѹ���������
#define         MAX_AC_GY_NUM           0x35                //AC��ѹ���������
#define         MAX_AC_SL_NUM           0x31                //ACʧ�����������
#define         MAX_AC_GL_NUM           0x31                //AC�������������
#define         MAX_AC_CL_NUM           0x31                //AC�����������������
#define         MAX_AC_GZ_NUM           0x31                //AC�������������
#define         MAX_AC_DX_NUM           0x35                //AC�������������
#define         MAX_AC_DYNX_NUM	        0x22                //AC��ѹ���������������

extern    const     INT8U       g_nACSY[];                  //ACʧѹ
extern    const     INT8U       g_nACGY[];                  //AC��ѹ
extern    const     INT8U       g_nACSL[];                  //ACʧ��
extern    const     INT8U       g_nACGL[];                  //AC����
extern    const     INT8U       g_nACGZ[];                  //AC����
extern    const     INT8U       g_nACDX[];                  //AC����
extern    const     INT8U       g_nACDYNX[];                //AC��ѹ������


/**************************************************************************************************
                                          ���ȫ�ֱ�������
***************************************************************************************************/

/**************************************************************************************************
                                         ��ʼ��������
***************************************************************************************************/

/**************************************************************************************************
                                         ���䳭��������
***************************************************************************************************/
INT32S 			GetItemMap(INT8U nRdFlags, INT8U nIsImUser, INT8U nIsRs485, INT8U* pItemMap);
INT32S 			IsCurrRdItem(INT8U* pItemMap, INT32U nStdItem);

INT32S 			Gb645ToColData(INT8U nProt, INT8U* pGb645Frm, void* pDataItem, void* pColData);
INT32S 			MeterMpInit(void);
INT32S 			MeterMpMap(INT8U nType);
INT32S 			SetRdMeterFlags(INT16U nSeq, INT8U nFlgBit);
INT32S          ClrRdMeterFlags(INT16U nSeq, INT8U nRdTd);
INT32S 			GetRdMeterFlags(INT16U nSeq, INT8U* pRdFlags);
INT32S 			SetBlockFlg(INT16U nSeq, INT8U nBlk);
INT32S 			GetBlockFlg(INT16U nSeq);
INT32S 			GetMeterInfo(INT16U nSeq, void* pInfo);
INT32S          GetSeqFromAddr(INT8U* pAddr);
INT16U 			GetMpFromSeq(INT16U nSeq);
INT16U 			GetSeqFromMp(INT16U nMp);
INT16U 			GetImSeqFromSeq(INT16U nSeq);

INT32S          GetPulsePrm(INT16U nSeq, pMEM_PULSE_PRM pPulsePrm);

INT8S           SetRdMeterFlagsAllMp(INT8U nRdTd);
INT8S           ClrRdMeterFlagsAllMp(INT8U nRdTd);


#endif
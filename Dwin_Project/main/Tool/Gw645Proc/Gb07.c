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
#include "Base.h"
#include "DataItem.h"
#include "Gb07.h"



/**************************************************************************************************
                                          ��
***************************************************************************************************/
#define GB_07DATA_OFF(item)											((INT32U)(&(((pCOL_DATA)0)->uCol07Data.item)))
#ifndef DATAITEM_DEFINE
#define DATAITEM_DEFINE(nStdItem, nProtItem, nUnit, nItemNum)		{nStdItem, nProtItem, INVLD_ITEM, GB_07DATA_OFF(nUnit), 0, nItemNum}
#endif

/**************************************************************************************************
                                           ����
***************************************************************************************************/

const DATA_ITEM	g_sGb07Item[] = {
#if 0
	//			ʵʱ����
#endif
			DATAITEM_DEFINE(RD_ZXYGJH, 0x0001FF00, nEnergyPP[0][0], 6),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x00010000, nEnergyPP[0][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x00010100, nEnergyPP[1][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x00010200, nEnergyPP[2][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x00010300, nEnergyPP[3][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x00010400, nEnergyPP[4][0], 1),

			DATAITEM_DEFINE(RD_FXYGJH, 0x0002FF00, nEnergyNP[0][0], 6),
			DATAITEM_DEFINE(RD_FXYGJH, 0x00020000, nEnergyNP[0][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x00020100, nEnergyNP[1][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x00020200, nEnergyNP[2][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x00020300, nEnergyNP[3][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x00020400, nEnergyNP[4][0], 1),

			DATAITEM_DEFINE(RD_ZXWGJH, 0x0003FF00, nEnergyPN[0][0], 6),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x00030000, nEnergyPN[0][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x00030100, nEnergyPN[1][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x00030200, nEnergyPN[2][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x00030300, nEnergyPN[3][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x00030400, nEnergyPN[4][0], 1),

			DATAITEM_DEFINE(RD_FXWGJH, 0x0004FF00, nEnergyNN[0][0], 6),
			DATAITEM_DEFINE(RD_FXWGJH, 0x00040000, nEnergyNN[0][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x00040100, nEnergyNN[1][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x00040200, nEnergyNN[2][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x00040300, nEnergyNN[3][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x00040400, nEnergyNN[4][0], 1),

			DATAITEM_DEFINE(RD_X1WGJH, 0x0005FF00, nQ1Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X1WGJH, 0x00050000, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x00050100, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x00050200, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x00050300, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x00050400, nQ1Energy[0][0], 1),

			DATAITEM_DEFINE(RD_X2WGJH, 0x0006FF00, nQ2Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X2WGJH, 0x00060000, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x00060100, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x00060200, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x00060300, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x00060400, nQ2Energy[0][0], 1),

			DATAITEM_DEFINE(RD_X3WGJH, 0x0007FF00, nQ3Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X3WGJH, 0x00070000, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x00070100, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x00070200, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x00070300, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x00070400, nQ3Energy[0][0], 1),

			DATAITEM_DEFINE(RD_X4WGJH, 0x0008FF00, nQ4Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X4WGJH, 0x00080000, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x00080100, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x00080200, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x00080300, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x00080400, nQ4Energy[0][0], 1),

			DATAITEM_DEFINE(RD_ZXYGXLJH, 0x0101FF00, nPPDemand[0][0], 6),//��ǰ�����й��������������ʱ��: 07��������ʱ����һ�� 97���Ƿֿ�
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0x01010000, nPPDemand[0][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0x01010100, nPPDemand[1][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0x01010200, nPPDemand[2][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0x01010300, nPPDemand[3][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0x01010400, nPPDemand[4][0], 1),

			DATAITEM_DEFINE(RD_FXYGXLJH, 0x0102FF00, nNPDemand[0][0], 6),//��ǰ�����й����������������ʱ��
            DATAITEM_DEFINE(RD_FXYGXLJH, 0x01020000, nNPDemand[0][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0x01020100, nNPDemand[1][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0x01020200, nNPDemand[2][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0x01020300, nNPDemand[3][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0x01020400, nNPDemand[4][0], 1),

			DATAITEM_DEFINE(RD_ZXWGXLJH, 0x0103FF00, nPNDemand[0][0], 6),//��ǰ�����޹��������������ʱ��
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0x01030000, nPNDemand[0][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0x01030100, nPNDemand[1][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0x01030200, nPNDemand[2][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0x01030300, nPNDemand[3][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0x01030400, nPNDemand[4][0], 1),

			DATAITEM_DEFINE(RD_FXWGXLJH, 0x0104FF00, nNNDemand[0][0], 6),//��ǰ�����޹��������������ʱ��
            DATAITEM_DEFINE(RD_FXWGXLJH, 0x01040000, nNNDemand[0][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0x01040100, nNNDemand[1][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0x01040200, nNNDemand[2][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0x01040300, nNNDemand[3][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0x01040400, nNNDemand[4][0], 1),



			DATAITEM_DEFINE(RD_DYJH, 0x0201FF00, nVolt[0][0], 4), //�����ѹ����
			DATAITEM_DEFINE(RD_DYJH, 0x02010100, nVolt[0][0], 1),//A���ѹ
			DATAITEM_DEFINE(RD_DYJH, 0x02010200, nVolt[1][0], 1),//B���ѹ
			DATAITEM_DEFINE(RD_DYJH, 0x02010300, nVolt[2][0], 1),//C���ѹ

			DATAITEM_DEFINE(RD_DLJH, 0x0202FF00, nCurr[0][0], 4),//�����������
			DATAITEM_DEFINE(RD_DLJH, 0x02020100, nCurr[0][0], 1),//A�����
			DATAITEM_DEFINE(RD_DLJH, 0x02020200, nCurr[1][0], 1),//B�����
			DATAITEM_DEFINE(RD_DLJH, 0x02020300, nCurr[2][0], 1),//C�����
            //DATAITEM_DEFINE(RD_DLJH, 0x02800001, nEnergyPC[3][0], 1),//�������

			DATAITEM_DEFINE(RD_YGGLJH, 0x0203FF00, nPWatt[0][0], 5),//�й����ʼ���
			DATAITEM_DEFINE(RD_YGGLJH, 0x02030000, nPWatt[0][0], 1),//���й�����
			DATAITEM_DEFINE(RD_YGGLJH, 0x02030100, nPWatt[1][0], 1),//A���й�����
			DATAITEM_DEFINE(RD_YGGLJH, 0x02030200, nPWatt[2][0], 1),//B���й�����
            DATAITEM_DEFINE(RD_YGGLJH, 0x02030300, nPWatt[3][0], 1),//C���й�����

			DATAITEM_DEFINE(RD_WGGLJH, 0x0204FF00, nNWatt[0][0], 5),//�޹����ʼ���
			DATAITEM_DEFINE(RD_WGGLJH, 0x02040000, nNWatt[0][0], 1),//���޹�����
			DATAITEM_DEFINE(RD_WGGLJH, 0x02040100, nNWatt[1][0], 1),//A���޹�����
			DATAITEM_DEFINE(RD_WGGLJH, 0x02040200, nNWatt[2][0], 1),//B���޹�����
			DATAITEM_DEFINE(RD_WGGLJH, 0x02040300, nNWatt[3][0], 1),//C���޹�����

			DATAITEM_DEFINE(RD_SZGLJH, 0x0205FF00, nSWatt[0][0], 5),//���ڹ��ʼ���
			DATAITEM_DEFINE(RD_SZGLJH, 0x02050000, nSWatt[0][0], 1),//�����ڹ���
			DATAITEM_DEFINE(RD_SZGLJH, 0x02050100, nSWatt[1][0], 1),//A�����ڹ���
			DATAITEM_DEFINE(RD_SZGLJH, 0x02050200, nSWatt[2][0], 1),//B�����ڹ���
			DATAITEM_DEFINE(RD_SZGLJH, 0x02050300, nSWatt[3][0], 1),//C�����ڹ���


            DATAITEM_DEFINE(RD_GLYSJH, 0x0206FF00, nFactor[0][0], 5),//������������
            DATAITEM_DEFINE(RD_GLYSJH, 0x02060000, nFactor[0][0], 1),//�ܹ�������
            DATAITEM_DEFINE(RD_GLYSJH, 0x02060100, nFactor[1][0], 1),//A�๦������
            DATAITEM_DEFINE(RD_GLYSJH, 0x02060200, nFactor[2][0], 1),//B�๦������
            DATAITEM_DEFINE(RD_GLYSJH, 0x02060300, nFactor[3][0], 1),//C�๦������

			DATAITEM_DEFINE(RD_DBRQ, 0x04000101, nDate[0], 1),			//���ܱ�����ʱ�ӵ�����
			DATAITEM_DEFINE(RD_DBSJ, 0x04000102, nTime[0], 1),			//���ܱ�����ʱ�ӵ�ʱ��

#if 0
// 			�ն�������
#endif
			DATAITEM_DEFINE(DD_FRZTIME, 0x05060001, nDayTime[0], 1),		//����ʱ��
			
			DATAITEM_DEFINE(DD_ZXYGJH, 0x05060101, nDEnergyPP[0][0], 1),//�ϴ������й�����ʾֵ
            DATAITEM_DEFINE(DD_FXYGJH, 0x05060201, nDEnergyNP[0][0], 1),//�ϴη����й�����ʾֵ
            DATAITEM_DEFINE(DD_ZXWGJH, 0x05060301, nDEnergyPN[0][0], 1),//�ϴ������޹�����ʾֵ
            DATAITEM_DEFINE(DD_FXWGJH, 0x05060401, nDEnergyNN[0][0], 1),//�ϴη����޹�����ʾֵ
            
            DATAITEM_DEFINE(DD_X1WGJH, 0x05060501, nDQ1Energy[0][0], 1),//�ϴ�һ���޹�����ʾֵ
            DATAITEM_DEFINE(DD_X2WGJH, 0x05060601, nDQ2Energy[0][0], 1),//�ϴζ����޹�����ʾֵ
            DATAITEM_DEFINE(DD_X3WGJH, 0x05060701, nDQ3Energy[0][0], 1),//�ϴ������޹�����ʾֵ
            DATAITEM_DEFINE(DD_X4WGJH, 0x05060801, nDQ4Energy[0][0], 1),//�ϴ������޹�����ʾֵ

			DATAITEM_DEFINE(DD_ZXYGXLJH, 0x05060901, nDPPDemand[0][0], 1),//�ϴ������й����������ʱ��
			DATAITEM_DEFINE(DD_FXYGXLJH, 0x05060A01, nDNPDemand[0][0], 1),//�ϴη����й����������ʱ��
			//DATAITEM_DEFINE(DD_ZXWGXLJH, 0x0103FF00, nDPNDemand[0][0], 1),//�ϴ������޹����������ʱ��
			//DATAITEM_DEFINE(DD_FXWGXLJH, 0x0104FF00, nDNNDemand[0][0], 1),//�ϴη����޹����������ʱ��

#if 0
			// �¶�������
#endif
			DATAITEM_DEFINE(MD_ZXYGJH, 0x0001FF01, nMEnergyPP[0][0], 6),//���������й�����ʾֵ
			DATAITEM_DEFINE(MD_ZXYGJH, 0x00010001, nMEnergyPP[0][0], 1),//���������й���
			DATAITEM_DEFINE(MD_ZXYGJH, 0x00010101, nMEnergyPP[1][0], 1),//���������й���
			DATAITEM_DEFINE(MD_ZXYGJH, 0x00010201, nMEnergyPP[2][0], 1),//���������й���
			DATAITEM_DEFINE(MD_ZXYGJH, 0x00010301, nMEnergyPP[3][0], 1),//���������й�ƽ
			DATAITEM_DEFINE(MD_ZXYGJH, 0x00010401, nMEnergyPP[4][0], 1),//���������й���

			DATAITEM_DEFINE(MD_FXYGJH, 0x0002FF01, nMEnergyNP[0][0], 6),//���·����й�����ʾֵ
			DATAITEM_DEFINE(MD_FXYGJH, 0x00020001, nMEnergyNP[0][0], 1),//���·����й���
			DATAITEM_DEFINE(MD_FXYGJH, 0x00020101, nMEnergyNP[1][0], 1),//���·����й���
			DATAITEM_DEFINE(MD_FXYGJH, 0x00020201, nMEnergyNP[2][0], 1),//���·����й���
			DATAITEM_DEFINE(MD_FXYGJH, 0x00020301, nMEnergyNP[3][0], 1),//���·����й�ƽ
			DATAITEM_DEFINE(MD_FXYGJH, 0x00020401, nMEnergyNP[4][0], 1),//���·����й���

			DATAITEM_DEFINE(MD_ZXWGJH, 0x0003FF01, nMEnergyPN[0][0], 6),//���������޹�����ʾֵ
			DATAITEM_DEFINE(MD_ZXWGJH, 0x00030001, nMEnergyPN[0][0], 1),//���������޹���
			DATAITEM_DEFINE(MD_ZXWGJH, 0x00030101, nMEnergyPN[1][0], 1),//���������޹���
			DATAITEM_DEFINE(MD_ZXWGJH, 0x00030201, nMEnergyPN[2][0], 1),//���������޹���
			DATAITEM_DEFINE(MD_ZXWGJH, 0x00030301, nMEnergyPN[3][0], 1),//���������޹�ƽ
			DATAITEM_DEFINE(MD_ZXWGJH, 0x00030401, nMEnergyPN[4][0], 1),//���������޹���

			DATAITEM_DEFINE(MD_FXWGJH, 0x0004FF01, nMEnergyNN[0][0], 6),//���·����޹�����ʾֵ
            DATAITEM_DEFINE(MD_FXWGJH, 0x00040001, nMEnergyNN[0][0], 1),//���·����޹���
            DATAITEM_DEFINE(MD_FXWGJH, 0x00040101, nMEnergyNN[0][0], 1),//���·����޹���
            DATAITEM_DEFINE(MD_FXWGJH, 0x00040201, nMEnergyNN[0][0], 1),//���·����޹���
            DATAITEM_DEFINE(MD_FXWGJH, 0x00040301, nMEnergyNN[0][0], 1),//���·����޹�ƽ
            DATAITEM_DEFINE(MD_FXWGJH, 0x00040401, nMEnergyNN[0][0], 1),//���·����޹���

			DATAITEM_DEFINE(MD_X1WGJH, 0x0005FF01, nMQ1Energy[0][0], 6),//����һ���޹�����ʾֵ
			DATAITEM_DEFINE(MD_X1WGJH, 0x00050001, nMQ1Energy[0][0], 1),//����һ���޹���
			DATAITEM_DEFINE(MD_X1WGJH, 0x00050001, nMQ1Energy[1][0], 1),//����һ���޹���
			DATAITEM_DEFINE(MD_X1WGJH, 0x00050001, nMQ1Energy[2][0], 1),//����һ���޹���
			DATAITEM_DEFINE(MD_X1WGJH, 0x00050001, nMQ1Energy[3][0], 1),//����һ���޹�ƽ
			DATAITEM_DEFINE(MD_X1WGJH, 0x00050001, nMQ1Energy[4][0], 1),//����һ���޹���

			DATAITEM_DEFINE(MD_X2WGJH, 0x0006FF01, nMQ2Energy[0][0], 6),//���¶����޹�����ʾֵ
			DATAITEM_DEFINE(MD_X2WGJH, 0x00060001, nMQ2Energy[0][0], 1),//���¶����޹���
			DATAITEM_DEFINE(MD_X2WGJH, 0x00060101, nMQ2Energy[1][0], 1),//���¶����޹���
			DATAITEM_DEFINE(MD_X2WGJH, 0x00060201, nMQ2Energy[2][0], 1),//����һ���޹���
			DATAITEM_DEFINE(MD_X2WGJH, 0x00060301, nMQ2Energy[3][0], 1),//���¶����޹�ƽ
			DATAITEM_DEFINE(MD_X2WGJH, 0x00060401, nMQ2Energy[4][0], 1),//���¶����޹���

			DATAITEM_DEFINE(MD_X3WGJH, 0x0007FF01, nMQ3Energy[0][0], 6),//���������޹�����ʾֵ
			DATAITEM_DEFINE(MD_X3WGJH, 0x00070001, nMQ3Energy[0][0], 1),//���������޹���
			DATAITEM_DEFINE(MD_X3WGJH, 0x00070101, nMQ3Energy[1][0], 1),//���������޹���
			DATAITEM_DEFINE(MD_X3WGJH, 0x00070201, nMQ3Energy[2][0], 1),//���������޹���
			DATAITEM_DEFINE(MD_X3WGJH, 0x00070301, nMQ3Energy[3][0], 1),//���������޹�ƽ
			DATAITEM_DEFINE(MD_X3WGJH, 0x00070401, nMQ3Energy[4][0], 1),//���������޹���

			DATAITEM_DEFINE(MD_X4WGJH, 0x0008FF01, nMQ4Energy[0][0], 6),//���������޹�����ʾֵ
		    DATAITEM_DEFINE(MD_X4WGJH, 0x00080001, nMQ4Energy[0][0], 1),//���������޹���
	        DATAITEM_DEFINE(MD_X4WGJH, 0x00080101, nMQ4Energy[1][0], 1),//���������޹���
		    DATAITEM_DEFINE(MD_X4WGJH, 0x00080201, nMQ4Energy[2][0], 1),//���������޹���
		    DATAITEM_DEFINE(MD_X4WGJH, 0x00080301, nMQ4Energy[3][0], 1),//���������޹�ƽ
			DATAITEM_DEFINE(MD_X4WGJH, 0x00080401, nMQ4Energy[4][0], 1),//���������޹���

			DATAITEM_DEFINE(MD_ZXYGXLJH, 0x0101FF01, nMPPDemand[0][0], 6),//���������й��������������ʱ��
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0x01010001, nMPPDemand[0][0], 1),//���������й����������������ʱ��
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0x01010101, nMPPDemand[1][0], 1),//���������й����������������ʱ��
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0x01010201, nMPPDemand[2][0], 1),//���������й����������������ʱ��
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0x01010301, nMPPDemand[3][0], 1),//���������й�ƽ�������������ʱ��
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0x01010401, nMPPDemand[4][0], 1),//���������й����������������ʱ��

			DATAITEM_DEFINE(MD_FXYGXLJH, 0x0102FF01, nMNPDemand[0][0], 6),//���·����й��������������ʱ��
			DATAITEM_DEFINE(MD_FXYGXLJH, 0x01020001, nMNPDemand[0][0], 1),//���·����й����������������ʱ��
			DATAITEM_DEFINE(MD_FXYGXLJH, 0x01020101, nMNPDemand[1][0], 1),//���·����й����������������ʱ��
			DATAITEM_DEFINE(MD_FXYGXLJH, 0x01020201, nMNPDemand[2][0], 1),//���·����й����������������ʱ��
			DATAITEM_DEFINE(MD_FXYGXLJH, 0x01020301, nMNPDemand[3][0], 1),//���·����й�ƽ�������������ʱ��
			DATAITEM_DEFINE(MD_FXYGXLJH, 0x01020401, nMNPDemand[4][0], 1),//���·����й����������������ʱ��


			DATAITEM_DEFINE(MD_ZXWGXLJH, 0x0103FF01, nMPNDemand[0][0], 6),//���������޹��������������ʱ��
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0x01030001, nMPNDemand[0][0], 1),//���������޹����������������ʱ��
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0x01030101, nMPNDemand[1][0], 1),//���������޹����������������ʱ��
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0x01030201, nMPNDemand[2][0], 1),//���������޹����������������ʱ��
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0x01030301, nMPNDemand[3][0], 1),//���������޹�ƽ�������������ʱ��
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0x01030401, nMPNDemand[4][0], 1),//���������޹����������������ʱ��

            DATAITEM_DEFINE(MD_FXWGXLJH, 0x0104FF01, nMNNDemand[0][0], 6),//���·����޹��������������ʱ��
			DATAITEM_DEFINE(MD_FXWGXLJH, 0x01040001, nMNNDemand[0][0], 1),//���·����޹����������������ʱ��
			DATAITEM_DEFINE(MD_FXWGXLJH, 0x01040101, nMNNDemand[1][0], 1),//���·����޹����������������ʱ��
			DATAITEM_DEFINE(MD_FXWGXLJH, 0x01040201, nMNNDemand[2][0], 1),//���·����޹����������������ʱ��
			DATAITEM_DEFINE(MD_FXWGXLJH, 0x01040301, nMNNDemand[3][0], 1),//���·����޹�ƽ�������������ʱ��
			DATAITEM_DEFINE(MD_FXWGXLJH, 0x01040401, nMNNDemand[4][0], 1),//���·����޹����������������ʱ��

			

			//********************************************************************************
			//									����������
			//********************************************************************************
			{COL_END_ITEM,	 INVLD_ITEM,	INVLD_ITEM,    sizeof(COL_07_DATA),	 sizeof(COL_07_DATA),  1}
};



/**************************************************************************************************
                                          ����
***************************************************************************************************/
/********************************************************************************
* ������:GetGb07IdTbl
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ07��Tbl
*********************************************************************************/
void* GetGb07IdTbl(void)
{
	return (void*)((pDATA_ITEM)&g_sGb07Item[0]);
}


/********************************************************************************
* ������:Gb07ColDataToRealData
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S Gb07ColDataToRealData(void* pColData)
{
	return 0;
}



/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �˲麯��
*********************************************************************************/
INT32S Gb07ItemInfoChk(void)
{
	pDATA_ITEM          pGb07Item	= (pDATA_ITEM)(&g_sGb07Item[0]);
	pDATA_ITEM          pGb07Item1	= NULL;
	pDATA_ITEM          pGb07Item2	= NULL;
	INT32S				i			= 0;
	
	// 1. �����ж�: ����Ϊ0/�ۼӲ�������Խ�����һ��   ���ݱ�ʶ�ж� ƫ�����ж�: һֱ���
	pGb07Item1	= pGb07Item;
	while(1)
	{
		if (COL_END_ITEM == pGb07Item1->nStdItem)
		{
			break;
		}
		// 1.1. ��������Ϊ0
		if (0 == pGb07Item1->nItemNum)
		{
			return -1;
		}

		// 1.2. �����Ϸ����жϣ��ۼӲ�������Խ�����һ��
		pGb07Item2 = pGb07Item1;
		for (i = 0; i < pGb07Item1->nItemNum; i++)
		{
			pGb07Item2++; 
			if (i != pGb07Item1->nItemNum - 1)
			{
				if (COL_END_ITEM == pGb07Item2->nStdItem)
				{
					return -1;
				}
			}
		}

		// 1.3. ���ݱ�ʶ�ж�
		pGb07Item2 = pGb07Item1;
		while(1)
		{
			pGb07Item2++;
			if (COL_END_ITEM == pGb07Item2->nStdItem)
			{
				break;	
			}
			if (pGb07Item2->nProtItem == pGb07Item1->nProtItem)
			{
				return -1;
			}
		}

		// 1.4 ƫ�����ж�: һֱ���
		pGb07Item2 = pGb07Item1 + 1;
		if (pGb07Item2->nOffset < pGb07Item1->nOffset)
		{
			return -1;
		}
		
		pGb07Item1++;
	}

	// 2. ��׼���ж�:Խ��������ı�׼���뱾������ͬ
	pGb07Item1 = pGb07Item;
	while(1)
	{
		if (COL_END_ITEM == pGb07Item1->nStdItem)
		{
			break;
		}
		pGb07Item2 = pGb07Item1;
		while(1)
		{
			pGb07Item2 += pGb07Item2->nItemNum;
			if (COL_END_ITEM == pGb07Item2->nStdItem)
			{
				break;
			}
			if (pGb07Item2->nStdItem == pGb07Item1->nStdItem)
			{
				return -1;
			}
		}
		
		pGb07Item1 += pGb07Item1->nItemNum;
	}

	return 0;
}








/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#ifndef ZBMENU_H
#define ZBIMENU_H

#include "Os_cpu.h"

/**************************************************************************************************
											 ��غ궨�弰�ṹö�ٶ���
***************************************************************************************************/
#define SUPER_PSW			"58715883"					// ��������

#define M_PSW_RET			0xEC						// ����Ա
#define S_PSW_RET			0xED						// ��������


/**************************************************************************************************
                                             ��������
***************************************************************************************************/

void CurrWatt(void);
void CurrPower(void);
void LoadCurve(void);
void SwitchState(void);
void PowerContrlRd(void);
void ElectronicRd(void);
void RemotRd(void);
void LosePowerRd(void);


void ContrlPara(void);
void FactoryPara(void);
void BusinessPara(void);
void FloatPara(void);
void MeterContlr(void);
void KvKiKp(void);
void MeterPara(void);
void ConfigPara(void);


void ContrlState(void);
void MeterData(void);
void ChineseInfo(void);
void BuyInfo(void);
void TerminalInfo(void);


#endif

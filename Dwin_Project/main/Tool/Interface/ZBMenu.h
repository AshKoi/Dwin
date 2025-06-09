/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#ifndef ZBMENU_H
#define ZBIMENU_H

#include "Os_cpu.h"

/**************************************************************************************************
											 相关宏定义及结构枚举定义
***************************************************************************************************/
#define SUPER_PSW			"58715883"					// 超级密码

#define M_PSW_RET			0xEC						// 管理员
#define S_PSW_RET			0xED						// 超级密码


/**************************************************************************************************
                                             变量声明
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

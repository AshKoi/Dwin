#ifndef TIMER_H
#define TIMER_H

#include "Timer.h"
#include "JDQCtrl.h"
#include "board.h"
#include "gprs.h"
#include "global.h"
//#include "Main.h"
#include "config.h"
#include "emac_api.h"
#include <stdio.h>
#include <string.h>
#include "os_cpu.h"


/**************************************************************************************************
                                           相关函数定义
***************************************************************************************************/

void show_ini(void);    
void timer_Ctrl(INT8U function);
void xianshi(INT8U Dot);


#endif
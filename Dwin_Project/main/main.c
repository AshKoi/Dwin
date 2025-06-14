/*******************************************************************************
* File Name          : main.c
* Version            : V1.0.0
* Date               : 2012.10.24
* Description        : Define main module.

*******************************************************************************/
#include "AppHead.h"

#define hardreset(a)             ((void (*)())(a))()


#ifndef SEC_APP
#define VECTOR_TABLE_ADDRESS_DEFAULT    0x8000
#else
#define VECTOR_TABLE_ADDRESS_DEFAULT    0x40000
#endif
/*      在程序不是在0地址开始烧写的时候，需要修改中断向量表的地址；       
        a.配置 VECTOR_TABLE_ADDRESS 的值为程序起始地址，b.同时需要修改
        ICF文件中的中断向量表地址和Rom起始地址*/

 
/****************************************************************************************************
//                                           函数实现
****************************************************************************************************/

/********************************************************************************
* 函数名: main
* 输　入:
*
* 输　出:
* 返  回:
* 功  能: 程序入口
********************************************************************************/
int main (void)
{ 
	SelAppAddr(VECTOR_TABLE_ADDRESS_DEFAULT);
	SystemCoreClockUpdate();    

	OSInit();																										
	if (RET_ERR_OK != SysTaskCreate(TASK_SEC))
    {
        
        return 0;
    }
	OSStart();
	 __enable_irq();
	return 0;
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

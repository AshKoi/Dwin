/*
************************************************************************
*
*                  发射机监控系统3.0__嵌入式系统构件        
*
************************************************************************

------------------------------------------------------------------------
文 件 名	: debug.h
文件描述	: debug相关处理函数
创建作者	: XXXX
创建日期	: 2010.09.26
************************************************************************
版本历史	: 
------------------------------------------------------------------------
修改日期	: 
修改内容	: 
========================================================================
*/

#ifndef _DEBUG_H_
#define _DEBUG_H_
/*
**********************************************************************
*							宏定义
**********************************************************************
*/

#define  DEBUG_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}	 //DEBUG 外设驱动临界段保护
#define  DEBUG_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}	 //DEBUG 外设驱动临界段退出

extern unsigned int OS_CPU_SR_Save(void);
extern void OS_CPU_SR_Restore(unsigned int sr);				 

/*
**********************************************************************
*							函数声明
**********************************************************************
*/
//extern void DEBUG_PRINTF( unsigned  char *str,unsigned  char str_len);
extern void DEBUG_IrqHandle(void);
extern unsigned int DEBUG_GetChar(unsigned char *pCh);
extern unsigned int DEBUG_PutChar(unsigned char ch);
extern unsigned int DEBUG_SendStr(unsigned char *pBuf,unsigned int len);
extern void DEBUG_Init(unsigned int baudrate,unsigned int mck);

#endif /*_DEBUG_H_*/

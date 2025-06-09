/*
************************************************************************
*
*                  ��������ϵͳ3.0__Ƕ��ʽϵͳ����        
*
************************************************************************

------------------------------------------------------------------------
�� �� ��	: debug.h
�ļ�����	: debug��ش�����
��������	: XXXX
��������	: 2010.09.26
************************************************************************
�汾��ʷ	: 
------------------------------------------------------------------------
�޸�����	: 
�޸�����	: 
========================================================================
*/

#ifndef _DEBUG_H_
#define _DEBUG_H_
/*
**********************************************************************
*							�궨��
**********************************************************************
*/

#define  DEBUG_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}	 //DEBUG ���������ٽ�α���
#define  DEBUG_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}	 //DEBUG ���������ٽ���˳�

extern unsigned int OS_CPU_SR_Save(void);
extern void OS_CPU_SR_Restore(unsigned int sr);				 

/*
**********************************************************************
*							��������
**********************************************************************
*/
//extern void DEBUG_PRINTF( unsigned  char *str,unsigned  char str_len);
extern void DEBUG_IrqHandle(void);
extern unsigned int DEBUG_GetChar(unsigned char *pCh);
extern unsigned int DEBUG_PutChar(unsigned char ch);
extern unsigned int DEBUG_SendStr(unsigned char *pBuf,unsigned int len);
extern void DEBUG_Init(unsigned int baudrate,unsigned int mck);

#endif /*_DEBUG_H_*/

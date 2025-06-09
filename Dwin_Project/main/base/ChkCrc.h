/******************************************************************************************************
* 2013-2014 益邦科技有限公司
* 文件名: 
* 创建人: 王一凡
* 日　期: 2013/09/21
* 修改人: 
* 日　期: 
* 描　述: 
* 版　本: V1.0
*****************************************************************************************************/
#ifndef _CHK_CRC_H_
#define _CHK_CRC_H_
#include "Os_cpu.h"
/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
//将输入的一串字节计算出CRC校验码
INT16U GetCrc16(const INT8U * pData, INT16U nLength);
//判断输入的一串字节CRC校验是否通过
BOOLEAN IsCrc16Good(const INT8U * pData, INT16U nLength);
//将输入的一串字节处理为携带2字节CRC校验码
void PutCrcProc(const INT8U * pSrc, INT8U * pDest, INT16U Len);
//
void PutCrc16Proc(INT8U *pSrc, INT32U nLen);
void getCrcProc(const INT8U * pSrc, INT8U * plHFS, INT8U * phHFS, INT16U Len);

INT16U crc16(INT8U *data, INT16U length) ;
#endif //_CHK_CRC_H_


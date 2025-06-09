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
#ifndef _FILETRANS_H_
#define _FILETRANS_H_

#include "Os_cpu.h"
#include "Gw3761.h"

/**************************************************************************************************
                                             宏定义
***************************************************************************************************/
#ifndef FLASH_BLOCK
#define FLASH_BLOCK							4096											//扇区大小
#endif

//flash地址划分
#define FLASH_BASE_DOWNLOAD					0												//flash基本基地址


#define MEM_FLH_UPPROGRAM_ADDR				(FLASH_BASE_DOWNLOAD + FLASH_BLOCK)				//下载帧存储基址，第一帧前16个字节是标志区
#define MEM_FLH_DEBUG_UPDATA_SIZE			(((320 * 1024 - 1) / FLASH_BLOCK + 1) * FLASH_BLOCK)//升级区大小，根据终端程序的大小调整

#define	GB3761_LEN_FILETRANS_HEAD       	16												//下载文件打包字头


/**************************************************************************************************
                                             函数声明
***************************************************************************************************/
INT8S GB3761FileTrans(pAPP_COMM pComm);
INT8S GB3761FileTransProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761FileTransFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761FileTransGWF1(INT16U nPn, INT8U *pRcv, INT16U nRcvlen, INT16U *pOffset, INT8U *pSnd, INT16U nSndlen);
#endif

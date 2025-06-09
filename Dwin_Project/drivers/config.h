/***************************************************************
* Copyright(c) 2010 光一科技股份有限公司 研发中心 - 基础技术部
* 文件名: config.h
* 创建人: lightrain
* 日　期: 2010-08-04
* 修改人:
* 日　期:
* 描　述: 系统配置文件
* 版　本: Version 1.0
****************************************************************/
/*
 * 文件名： config.h
 * 功能:	宏定义
 * 作者：	Light Rain
 * 时间：	2010-04-12
 *
 * 描述：	
 *
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifndef NULL
#define NULL    0
#endif

#ifndef TRUE
#define TRUE    1
#endif                                                                  /*  TRUE                        */

#ifndef FALSE
#define FALSE   0
#endif                                                                  /*  FALSE                       */

/*
*********************************************************************************************************
*                                          STANDARD DEFINES
*********************************************************************************************************
*/


#define  DEF_DISABLED                                      0
#define  DEF_ENABLED                                       1

#define  DEF_FALSE                                         0
#define  DEF_TRUE                                          1

#define  DEF_NO                                            0
#define  DEF_YES                                           1

#define  DEF_OFF                                           0
#define  DEF_ON                                            1

#define  DEF_CLR                                           0
#define  DEF_SET                                           1

#define  DEF_ACTIVE                                        0
#define  DEF_INACTIVE                                      1

#define  DEF_FAIL                                          0
#define  DEF_OK                                            1

#if 0
typedef unsigned char  uint8;                                           /*  无符号8位整型变量           */
typedef signed   char  int8;                                            /*  有符号8位整型变量           */
typedef unsigned short uint16;                                          /*  无符号16位整型变量          */
typedef signed   short int16;                                           /*  有符号16位整型变量          */
typedef unsigned int   uint32;                                          /*  无符号32位整型变量          */
typedef signed   int   int32;                                           /*  有符号32位整型变量          */
typedef float          fp32;                                            /*  单精度浮点数（32位长度）    */
typedef double         fp64;                                            /*  双精度浮点数（64位长度）    */
#endif


/**********************************************************************************************************
*                    定义与编译器无关的数据类型
**********************************************************************************************************/
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */



typedef unsigned char  UINT8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  INT8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short UINT16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short INT16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   UINT32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   INT32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */

#define  XUINT8  UINT8
#define  XUINT16 UINT16
#define  XUINT32 UINT32
#define  XINT8   INT8
#define  XINT16  INT16
#define  XINT32  INT32
#define  xdata   

typedef            void       CPU_VOID;
typedef  unsigned  char       CPU_CHAR;                         /*  8-bit character                                     */
typedef  unsigned  char       CPU_BOOLEAN;                      /*  8-bit boolean or logical                            */
typedef  unsigned  char       CPU_INT08U;                       /*  8-bit unsigned integer                              */
typedef    signed  char       CPU_INT08S;                       /*  8-bit   signed integer                              */
typedef  unsigned  short      CPU_INT16U;                       /* 16-bit unsigned integer                              */
typedef    signed  short      CPU_INT16S;                       /* 16-bit   signed integer                              */
typedef  unsigned  int        CPU_INT32U;                       /* 32-bit unsigned integer                              */
typedef    signed  int        CPU_INT32S;                       /* 32-bit   signed integer                              */
typedef  unsigned  long long  CPU_INT64U;                       /* 64-bit unsigned integer                              */
typedef    signed  long long  CPU_INT64S;                       /* 64-bit   signed integer                              */

typedef            float      CPU_FP32;                         /* 32-bit floating point                                */
typedef            double     CPU_FP64;                         /* 64-bit floating point                                */


typedef            void     (*CPU_FNCT_VOID)(void);             /* See Note #2a.                                        */
typedef            void     (*CPU_FNCT_PTR )(void *);           /* See Note #2b.                                        */


#define BOOL boolean_t

/*********************************************************************************************************
  uC/OS-II specital code
  uC/OS-II的特殊代码
*********************************************************************************************************/
#include "ucos_ii.h"

/*********************************************************************************************************
  ARM的特殊代码
  ARM specital code
*********************************************************************************************************/
#include "cpu.h"
//#include "lib_def.h"

/* build CMD*/
/*  "$PROJ_DIR$\config\AddCrcTail.exe" "$EXE_DIR$\$PROJ_FNAME$.bin"  */


#endif                                                                  /*  __CONFIG_H                  */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

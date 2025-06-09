/*!
 ******************************************************************************
 **
 ** \file board.h
 **
 ** \brief options for different hardware board
 ** 
 **
 ** \author 
 **
 ** \version V1.0
 **
 ** \date 2012-05-28
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */ 
 
 #ifndef __BOARD_H__
#define __BOARD_H__

/*---------------------------------------------------------------------------*/
/* include files                                                                                                                 */
/*---------------------------------------------------------------------------*/
#include "mb9bf218s.h"
#include <stdint.h>


/*---------------------------------------------------------------------------*/
/* constants and macros                                                                                                   */
/*---------------------------------------------------------------------------*/

#define EVB_FM3_ETH
#define RMII_MODE   //RMII mode enable

#define EMAC_RXBUFNB        	2
#define EMAC_TXBUFNB        	2


#define USING_MAC0
#define EXT_MAINCLOCK  ( 4000000UL)
#define PLL_CTL1_CURRENT 0x00000001

//#define TICK_PER_SECOND	1000 // Tick per Second


#define ETHTYPE_IP  0x0800
#define IP_PROTO_ICMP 1


/* Compiler Related Definitions */
#ifdef __CC_ARM                         /* ARM Compiler */
	#include <stdarg.h>
	#define SECTION(x)                  __attribute__((section(x)))
	#define UNUSED                      __attribute__((unused))
	#define ALIGN(n)                    __attribute__((aligned(n)))
	#define rt_inline                   static __inline
	/* module compiling */
	#ifdef RT_USING_MODULE
		#define RTT_API __declspec(dllimport)
	#else
		#define RTT_API __declspec(dllexport)
	#endif

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */  //__ICCARM__
	#include <stdarg.h>
	#define SECTION(x)                  @ x
	#define UNUSED
	#define PRAGMA(x)                   _Pragma(#x)
	#define ALIGN(n)                    PRAGMA(data_alignment=n)

	#define rt_inline                   static inline
	#define RTT_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
	#ifdef RT_USING_NEWLIB
		#include <stdarg.h>
	#else
		#if __GNUC__ < 4
			typedef void *__sys_va_list;
			typedef __sys_va_list va_list;
			#define __va_rounded_size(type)	\
				(((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
			#define va_start(ap, lastarg)	\
				(ap = ((char *) &(lastarg) + __va_rounded_size(lastarg)))
			#define va_end(ap)	((void)0)
			/*	little endian */
			#define va_arg(ap, type)	\
				(ap = (__sys_va_list) ((char *)(ap) + __va_rounded_size(type)),  \
				*((type *) (void *) ((char *)(ap) - __va_rounded_size(type))))
		#else
			typedef __builtin_va_list __gnuc_va_list;
			typedef __gnuc_va_list va_list;
			#define va_start(v,l)   __builtin_va_start(v,l)
			#define va_end(v)       __builtin_va_end(v)
			#define va_arg(v,l)     __builtin_va_arg(v,l)
		#endif
	#endif

	#define SECTION(x)                  __attribute__((section(x)))
	#define UNUSED                      __attribute__((unused))
	#define ALIGN(n)                    __attribute__((aligned(n)))
	#define rt_inline                   static __inline
	#define RTT_API
#endif


#endif   /* __BOARD_H__ */


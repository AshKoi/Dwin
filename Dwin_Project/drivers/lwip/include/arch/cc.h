/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: cc.h, v1.2.0 2007/08/02 9527 $
 */

#ifndef __CC_H__
#define __CC_H__


#include "lwip/include/lwip/opt.h"

/*	type define	*/
typedef unsigned char      	u8_t;
typedef signed char      	s8_t;
typedef unsigned short    	u16_t;
typedef signed   short    	s16_t;

typedef unsigned int    	u32_t;
typedef signed   int    	s32_t;

typedef unsigned char      	u_char;
typedef unsigned short    	u_short;
typedef unsigned int    	u_int;
/*	2007-08-02	add typedef mem_ptr_t by 9527 that use in mem	*/
typedef unsigned int 		mem_ptr_t;

#define __sio_fd_t_defined

typedef struct _sio_fd {
    INT32U  readflag;
    INT32U  (* readchar)(INT8U *c);
    INT32U  (* writechar)(INT8U c);
} sio_fd, *sio_fd_t;

/*	2006-08-02	add by 9527 Define (sn)printf formatters for these lwIP types	*/
#define U16_F 	"hu"
#define S16_F 	"hd"
#define X16_F 	"hx"
#define U32_F 	"lu"
#define S32_F 	"ld"
#define X32_F 	"lx"

/* 	2007-08-02	add by 9527	临界代码保护宏	*/
#if SYS_LIGHTWEIGHT_PROT
#define	SYS_ARCH_DECL_PROTECT(cpu_sr)		OS_CPU_SR	cpu_sr = 0;
#define	SYS_ARCH_PROTECT(cpu_sr)			(cpu_sr = OS_CPU_SR_Save())
#define	SYS_ARCH_UNPROTECT(cpu_sr)			(OS_CPU_SR_Restore(cpu_sr))
#else
#define	SYS_ARCH_DECL_PROTECT(cpu_sr)		
#define	SYS_ARCH_PROTECT(cpu_sr)			
#define	SYS_ARCH_UNPROTECT(cpu_sr)			
#endif /* SYS_LIGHTWEIGHT_PROT */

/*	LwIP网络数据包结构体封装宏，与编译器相关	*/
#define PACK_STRUCT_FIELD(x)  	__packed x
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_BEGIN  		__packed
#define PACK_STRUCT_END

/*	define debug outprint funtion */
#define LWIP_PLATFORM_DIAG(x) 		/*	非故障，输出一条提示信息。	*/
#define LWIP_PLATFORM_ASSERT(x)		/*	故障，输出一条故障信息并放弃执行。	*/

/*	2007-08-02	add byte order define by 9527	*/
/*	define byte order for CPU	*/
#define LITTLE_ENDIAN 	1234	/*	小端字节序	*/
#define BIG_ENDIAN 		4321	/*	大端字节序	*/

#define BYTE_ORDER 		LITTLE_ENDIAN

/*	2007-08-02	add by 9527	usb lwip self define error code	*/
#define	LWIP_PROVIDE_ERRNO

#endif /* __CC_H__ */

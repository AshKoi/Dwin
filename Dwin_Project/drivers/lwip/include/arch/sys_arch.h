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
 * $Id: sys_arch.h, v1.2.0 2007/08/02 9527 $
 */
#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#include "lwip/include/arch/cc.h"
#include "ucos_ii.h"


#ifndef   UDP_THREAD_NAME
#define   UDP_THREAD_NAME   	 "udp_test_thread"
#endif

#ifndef   HTTP_THREAD_NAME
#define   HTTP_THREAD_NAME 	  	 "http_thread"
#endif


#ifndef   TELNET_THREAD_NAME
#define   TELNET_THREAD_NAME      "telnet_thread"
#endif
/*	define the max size stack for lwip task	*/
#ifndef LWIP_THREAD_STK_SIZE
#define LWIP_THREAD_STK_SIZE      	128
#endif /*	LWIP_THREAD_STK_SIZE	*/

/*	defines for sys_thread_new() & sys_arch_timeouts()	*/
/*	max number of lwip tasks	*/
#ifndef LWIP_THREAD_MAX
#define LWIP_THREAD_MAX    			1
#endif /*	LWIP_THREAD_MAX	*/

/*	first prio of lwip tasks	*/
#ifndef LWIP_THREAD_START_PRIO
#define LWIP_THREAD_START_PRIO  	11
#endif /*	LWIP_THREAD_START_PRIO	*/

/*	define null mailbox and semaphore	*/
#define SYS_MBOX_NULL   			(void*)0
#define SYS_SEM_NULL    			(void*)0

/*	define max number of mailbox queues and max entries of each queue	*/
#define LWIP_MBOX_MAX        		8
#define LWIP_MBOX_MSG_SIZE			16

/*	define struct of lwip mailbox	*/
typedef struct __LWIPMBOX {
    struct __LWIPMBOX *next;
    OS_EVENT*   pmbox;
    void*       pmsg[LWIP_MBOX_MSG_SIZE];
} LWIPMBOX, *pLWIPMBOX;

typedef OS_EVENT* sys_sem_t;		/*	lwip semaphore	*/
typedef pLWIPMBOX sys_mbox_t;		/*	lwip mailbox	*/
typedef u8_t      sys_thread_t;		/*	lwip task ID	*/
typedef u32_t	  sys_prot_t;		/*	lwip prot for cpu register	*/
sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio);
sys_thread_t sys_thread_free(char *name, int prio);

#endif	/*	__SYS_ARCH_H__	*/

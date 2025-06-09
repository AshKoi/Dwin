/*
 * Copyright (c) 2001-2004, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: sys_arch.c,v 1.2.0 2007/08/02  9527 $
 * describe: 	����ϵͳģ���ӿں���ʵ���ļ�
 */
#include "lwip/include/lwip/debug.h"

#include "lwip/include/lwip/def.h"
#include "lwip/include/lwip/sys.h"
#include "lwip/include/lwip/mem.h"
#include "lwip/include/lwip/memp.h"
#include "lwip/include/lwip/pbuf.h"
#include "lwip/include/lwip/sio.h"
#include "lwip/include/lwip/tcpip.h"

#include "lwip/include/arch/sys_arch.h"

//#include "sys_includes.h"
#include <lwip/utility/trace.h>
#include <lwip/utility/assert.h>
//	notice: we use OSTaskQuery in sys_arch_timeouts() !

//	����������Ϣ����Ŀռ�
static LWIPMBOX Lwip_mbox[LWIP_MBOX_MAX];
//	ָ��ǰ��������
static pLWIPMBOX Lwip_curfreembox;

//	��ʱ�¼��������һ��Ϊ��ʱ�����׼���ġ�
struct sys_timeouts lwip_timeouts[LWIP_THREAD_MAX + 1];

//	LwIP�߳�ʹ�õĶ�ջ
static OS_STK Lwip_thread_stk[LWIP_THREAD_MAX][LWIP_THREAD_STK_SIZE];


unsigned int lwip_con_timout=0;

/************************************************************
sys_ms_to_ticks
����������
	������ת���ʱ�ӽ���

��ڲ�����
	������

���ڲ�����
	�δ���

������Ϣ��
	2007-08-02	����

ע�����
************************************************************/
u16_t sys_ms_to_ticks(u32_t time_ms)
{
	u32_t time_ticks;
	
	if(time_ms != 0) {
		time_ticks = (time_ms * OS_TICKS_PER_SEC)/1000;
		
		if(time_ticks < 1) {
			time_ticks = 1;
		} else if(time_ticks > 65535) {
			time_ticks = 65535;
		}
	} else {
		time_ticks = 0;
	}		
		
	return (u16_t)time_ticks;
}


/************************************************************
sys_mbox_new
����������
	����һ���յ�����

��ڲ�����
	��

���ڲ�����
	!= SYS_MBOX_NULL: 	��������ɹ�������һ��ָ�����������ָ��
	 = SYS_MBOX_NULL: 	����û������ɹ�

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
sys_mbox_t sys_mbox_new(int size)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  		cpu_sr = 0;
#endif
    sys_mbox_t plwipbox;

    plwipbox = SYS_MBOX_NULL;
    //	ask for a new mailbox
    OS_ENTER_CRITICAL()
    {
	    if(Lwip_curfreembox != NULL) {
	    	plwipbox = Lwip_curfreembox;
	    	Lwip_curfreembox = plwipbox->next;
	    }
	}
	OS_EXIT_CRITICAL()
	
    return plwipbox;
}



/************************************************************
sys_mbox_free
����������
	�ͷ����䣬������黹����������ͷ�ʱ�����л�����Ϣ����
	����LwIP�д���һ����̴���Ӧ��֪ͨ�����ߡ�

��ڲ�����
	<mbox>[in]:		Ҫ�黹������

���ڲ�����
	��
	
������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
void sys_mbox_free(sys_mbox_t mbox)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  		cpu_sr = 0;
#endif

    //	Ϊ�˷�ֹ����������������������һ������
    OSQFlush(mbox->pmbox);
    //	del OSQ EVENT
	OS_ENTER_CRITICAL()
	{
		mbox->next = Lwip_curfreembox;
		Lwip_curfreembox = mbox;
	}
	OS_EXIT_CRITICAL()
}


/************************************************************
sys_mbox_post
����������
	����ϢͶ�ݵ�ָ��������

��ڲ�����
	<mbox>[in]:		ָ��ҪͶ�ݵ�����
	 <msg>[in]:		ָ��ҪͶ�ݵ���Ϣ

���ڲ�����
	��
	
������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
void sys_mbox_post(sys_mbox_t mbox, void *msg)
{		
		#if  LWIP_TRACE_ERROR
		if(OSPrioCur==10)
		{
			TRACE_ERROR("��������������\n");
		
		}
		else if(OSPrioCur==11)
		{
		
			TRACE_ERROR("�������������\n");
		}
		else if(OSPrioCur==15)
		{
			TRACE_ERROR("����һ��������\n");
		
		}
		#endif
	while(OSQPost(mbox->pmbox, msg) == OS_Q_FULL) {
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}



/************************************************************
sys_arch_mbox_fetch
����������
	��ָ�������������Ϣ���ú����������߳�ֱ����ʱ������������
	�յ�һ����Ϣ

��ڲ�����
	   <mbox>[in]:	ָ��������Ϣ������
	   <msg>[out]:	���������������յ�����Ϣָ��
	<timeout>[in]:	ָ���ȴ����յ��ʱ�䣬Ϊ0����һֱ�ȴ�ֱ������
					����Ϣ����λΪ����
				
���ڲ�����
	0: 					��ָ��ʱ�����յ���Ϣ
	SYS_ARCH_TIMEOUT: 	��ָ��ʱ����û���յ���Ϣ
	
������Ϣ��
	2007-08-02	����
	
ע�����		  	#if  LWIP_TRACE_ERROR
	TRACE_ERROR("low_level_input\n");
	#endif
************************************************************/
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
	u8_t err;
	unsigned int n_tim=0;

	if(msg != NULL) {
	//	*msg =LWIP_OSQPend(mbox->pmbox, sys_ms_to_ticks(timeout),&n_tim,&err);
		*msg =OSQPend(mbox->pmbox, sys_ms_to_ticks(timeout),&err);
	} else {
	//	OSQPend(mbox->pmbox, sys_ms_to_ticks(timeout),&n_tim,&err);
		*msg =OSQPend(mbox->pmbox, sys_ms_to_ticks(timeout),&err);
	}
	
	if(err == OS_NO_ERR) {
		#if  LWIP_TRACE_ERROR
		if(OSPrioCur==10)
		{
			TRACE_ERROR("���������յ�����\n");
		
		}
		else if(OSPrioCur==11)
		{
		
			TRACE_ERROR("��������յ�����\n");
		}
		else if(OSPrioCur==15)
		{
			TRACE_ERROR("����һ���յ�����\n");
		
		}
		#endif
		return n_tim;
	} else {
		return SYS_ARCH_TIMEOUT;
	}
}

err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg)
{
	#if  LWIP_TRACE_ERROR
	if(OSPrioCur==10)
	{
		TRACE_ERROR("��������������\n");
	
	}
	else if(OSPrioCur==11)
	{
	
	   TRACE_ERROR("�������������\n");
	}
	else if(OSPrioCur==15)
	{
		TRACE_ERROR("����һ��������\n");
	
	}
	#endif
	OSQPost(mbox->pmbox, msg) ;
	return ERR_OK;

}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t mbox, void **msg)
{
	u8_t err;
	if(msg != NULL) {
		*msg = OSQAccept(mbox->pmbox, &err);
	} else {
		OSQAccept(mbox->pmbox, &err);
	}
	
	if(err == OS_NO_ERR) {
		
		return 0;
	} else {
		return SYS_MBOX_EMPTY;
	}
}
/************************************************************
sys_sem_new
����������
	����������һ���µ��ź���

��ڲ�����
	<count>[in]:	ָ���ź����ĳ�ʼ״̬��OS�ź����������

���ڲ�����
	�����µ��ź���

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
sys_sem_t sys_sem_new(u8_t count)
{	

	#if  LWIP_TRACE_ERROR
	if(OSPrioCur==10)
	{
		TRACE_ERROR("�������½��ź���\n");
	
	}
	else if(OSPrioCur==11)
	{
	
		TRACE_ERROR("������½��ź���\n");
	}
	else if(OSPrioCur==15)
	{
		TRACE_ERROR("����һ�½��ź���\n");
	
	}
	#endif
    return OSSemCreate(count);
}



/************************************************************
sys_sem_signal
����������
	�����ź�

��ڲ�����
	<sem>[in]:		ָ��ָ��Ҫ���͵��źţ�OS�źŷ������

���ڲ�����
	��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
void sys_sem_signal(sys_sem_t sem)
{	

    OSSemPost(sem);
}


/************************************************************
sys_arch_sem_wait
����������
	�ȴ�ָ�����źŲ������̡߳�timeout����Ϊ0���̻߳�һֱ������
	���յ�ָ�����źţ���0�����߳̽���������ָ����timeoutʱ��
	����λΪ���룩����timeout����ֵ��0������£�����ֵΪ�ȴ�ָ
	�����ź������ĵĺ������������ָ����ʱ���ڲ�û���յ��źţ�
	����ֵΪ SYS_ARCH_TIMEOUT������̲߳����ٵȴ�����źţ�Ҳ
	����˵���Ѿ��յ��źţ�������ֵҲ����Ϊ0��ע�⣬LwIPʵ����
	һ��������֮���Ƶĺ������������������sys_sem_wait()��ע��
	����

��ڲ�����
	    <sem>[in]:	ָ��ָ��Ҫ���͵��źţ�OS�źŷ������
	<timeout>[in]:	ָ���ȴ����ʱ�䣨��λΪ���룩��
					 0: �̻߳�һֱ������ֱ���յ�ָ�����źţ�
					!0:	ָ���߳���ȴ�ʱ��

���ڲ�����
	0: 					��ָ��ʱ���ڵȵ�ָ���ź�
	SYS_ARCH_TIMEOUT: 	��ָ��ʱ����û�еȵ�ָ���ź�

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
	u8_t 	err;

  	OSSemPend(sem, sys_ms_to_ticks(timeout), &err);
	#if  LWIP_TRACE_ERROR
	if(OSPrioCur==10)
	{
		TRACE_ERROR("���������յ��ź���\n");
	
	}
	else if(OSPrioCur==11)
	{
	
		TRACE_ERROR("��������յ��ź���\n");
	}
	else if(OSPrioCur==15)
	{
		TRACE_ERROR("����һ���յ��ź���\n");
	
	}
	#endif
  	//	should not return 0 when wait time is 0, only when timeout!
  	//	see sys_arch.txt in lwip/doc
  	if(err == OS_NO_ERR) {
  		return 0;
  	}
  	else {
  		return SYS_ARCH_TIMEOUT;
  	}
}



/************************************************************
sys_sem_free
����������
	�����ź�

��ڲ�����
	<sem>[in]:	ָ��ָ��Ҫ�ͷŵ��źţ�OS�ź�ɾ�����

���ڲ�����
	��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
void sys_sem_free(sys_sem_t sem)
{
    u8_t err;

    while(OSSemDel((OS_EVENT *)sem, OS_DEL_NO_PEND, &err) != NULL) {
    	OSTimeDlyHMSM(0, 0, 0, 100);
    }
}



/************************************************************
sys_init
����������
	����sys_arch.txt�ļ�����������Ϊ��ʼ��sys_arch��

��ڲ�����
	��

���ڲ�����
	��
	
������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
void sys_init(void)
{
	u8_t i;
	
	// �������������
	for(i=0; i<(LWIP_MBOX_MAX-1); i++)
	{
		// �������еĸ���Ա������һ��
		Lwip_mbox[i].next = &Lwip_mbox[i+1];
		// �������䣬ϵͳ���뱣֤�����ܹ�˳��������������ִ������ǳ���BUG��Ӧ���ڵ��Խ׶��ų�
		Lwip_mbox[i].pmbox = OSQCreate(Lwip_mbox[i].pmsg, LWIP_MBOX_MSG_SIZE);
	}

	// ���������һ��Ԫ�أ�����û�н���������
	Lwip_mbox[LWIP_MBOX_MAX-1].next = (pLWIPMBOX)NULL;
	Lwip_mbox[LWIP_MBOX_MAX-1].pmbox = OSQCreate(Lwip_mbox[LWIP_MBOX_MAX - 1].pmsg, LWIP_MBOX_MSG_SIZE);
	// ��ǰ���������׵�ַ
	Lwip_curfreembox = Lwip_mbox;

	// ��ʼ��sys_timeouts���飬��ÿ�������Ա����������ַ����ΪNULL
	for(i=0; i<LWIP_THREAD_MAX+1; i++)
	{
		lwip_timeouts[i].next = NULL;
	}
	
	//curr_prio_offset = 0;
}

/************************************************************
sys_arch_timeouts
����������
	��ȡ��ǰ�߳�ʹ�õ�sys_timeouts�ṹ��ָ��

��ڲ�����
	��

���ڲ�����
	����һ��ָ��ǰ�߳�ʹ�õ�sys_timeouts�ṹ��ָ��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
struct sys_timeouts * sys_arch_timeouts(void)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  		cpu_sr = 0;
#endif
	u16_t os_prio;
  	u8_t lwip_prio;

  	//	get current task prio
	OS_ENTER_CRITICAL()
	{
		os_prio = OSTCBCur->OSTCBPrio;
	}
	OS_EXIT_CRITICAL()

  	lwip_prio = os_prio - LWIP_THREAD_START_PRIO;
  	//	not called by a lwip task, return timeouts->NULL
  	if(lwip_prio < LWIP_THREAD_MAX) {
    	//	��ǰ�߳���ָ����LwIP�߳����ȼ��ŷ�Χ֮��
  		return &lwip_timeouts[lwip_prio];
  	}
	//	������������������ǰ�̲߳�����ָ����LwIP�߳����ȼ���Դ֮��
	// �򷵻�lwip_timeouts��������һ����Ա
	return &lwip_timeouts[LWIP_THREAD_MAX];
}



/************************************************************
sys_thread_new
����������
	����һ�����߳�

��ڲ�����
	<thread>[in]:	���̵߳���ڵ�ַ
	   <arg>[in]:	���ݸ����̵߳Ĳ���
	  <prio>[in]:	��LwIPָ�������߳����ȼ���������ȼ���1��ʼ
	  				���Ҳ���������OS���ȼ���Ҫ����ת����

���ڲ�����
	�ɹ�:	�����߳����ȼ���ע������prio��ͬ��
			���ֵʵ�ʵ��� T_LWIP_THREAD_START_PRIO + prio
    ���ɹ�: ����0

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/


sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
	sys_thread_t os_prio;
	
	//	������ȼ�����û�г���ϵͳ����ķ�Χ
	if(prio > 0 && prio <= LWIP_THREAD_MAX)
	{
		//	change to OS prio
		os_prio = LWIP_THREAD_START_PRIO + (prio - 1);
		//	create task
		//sys_general_memset(&Lwip_thread_stk[prio-1][0],0x00,LWIP_THREAD_STK_SIZE*4);
    	if(OSTaskCreate(thread,arg,&Lwip_thread_stk[prio-1][LWIP_THREAD_STK_SIZE-1],os_prio) == OS_NO_ERR)
    	{
    		return os_prio;
    	}
  	}
  	//	lwip task prio out of range ! error!
    return 0;
     /* u8_t ubPrio = 0;
    u8_t ucErr;

    arg = arg;

    if((prio > 0) && (prio <= 6))
    {
        ubPrio = (u8_t)(14 + prio - 1);

        if(stacksize > 300)
            stacksize = 300;

#if (OS_TASK_STAT_EN == 0)
        OSTaskCreate(thread,arg,&Lwip_thread_stk[prio-1][LWIP_THREAD_STK_SIZE-1],os_prio);
#else
        OSTaskCreateExt(thread, (void *)arg, &Lwip_thread_stk[prio-1][stacksize-1],ubPrio
                        ,ubPrio,&Lwip_thread_stk[prio-1][0],stacksize,(void *)0,OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
#endif
        OSTaskNameSet(ubPrio, (u8_t*)name, &ucErr);

    }
        return ubPrio;*/
}

sys_thread_t sys_thread_free(char *name, int prio)
{
	sys_thread_t os_prio;
	os_prio = LWIP_THREAD_START_PRIO + (prio - 1);
	OSTaskDel( os_prio );
}

/************************************************************
sio_open
����������
	��IO����Ӳ���ӿڣ���ʼ��Ӳ����

��ڲ�����
	<dev>[in]:	Ӳ���豸ID

���ڲ�����
	�豸��ID��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
sio_fd_t sio_open(u8_t dev)
{
	sio_fd_t fd;
	
	return fd;
}


/************************************************************
sio_send
����������
	ͨ��IO�ڷ�������

��ڲ�����
	<dev>[in]:	Ӳ���豸ID
	<fd>[in]:	Ӳ������ID

���ڲ�����
	��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
void sio_send(u8_t dev, sio_fd_t fd)
{

}


/************************************************************
sio_recv
����������
	ͨ��IO�ڷ�������

��ڲ�����
	<fd>[in]:	Ӳ������ID

���ڲ�����
	SLIP_END     0300
	SLIP_ESC     0333
	SLIP_ESC_END 0334
	SLIP_ESC_ESC 0335

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
u8_t sio_recv(sio_fd_t fd)
{
	u8_t ret;
	
	ret = 0;
	
	return ret;
}


/************************************************************
sio_read
����������
	ͨ��IO�ڶ�ȡ���ݵ�ָ���Ŀռ���

��ڲ�����
	<fd>[in]:		Ӳ������ID
	<payload>[in]:	��ȡ���ݵĻ�����
	<len>[in]:		����������󳤶�

���ڲ�����
	��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
u32_t sio_read(sio_fd_t fd, u8_t *payload, u32_t len)
{
	u8_t temp_uchar;
	u32_t temp_len;
	u32_t timeout;

	temp_len = 0;
	
	if(fd == NULL) {
	    return 0;
	}
	
	if(len == 0)
	{
		return 0;
	}
	
	if(!fd->readchar(&temp_uchar))
	{
		return 0;
	}

    if(temp_uchar != 0x7E)
    {
        return 0;
    }

    payload[temp_len] = temp_uchar;
    temp_len++;

	sys_set_timeout(timeout,200);
	while((!sys_detect_timeout(timeout)) && fd->readflag)
	{
        if (temp_len >= len)
        {
            break;
        }

        if(fd->readchar(&temp_uchar))
        {
            payload[temp_len] = temp_uchar;
            temp_len++;

            if(temp_uchar == 0x7E)
            {
                break;
            }
        }
	}

	return temp_len;
}


/************************************************************
sio_write
����������
	ͨ��IO��д������

��ڲ�����
	<fd>[in]:		Ӳ������ID
	<payload>[in]:	��д������ݻ�����
	<len>[in]:		�����������ݳ���

���ڲ�����
	��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
u32_t sio_write(sio_fd_t fd, u8_t *payload, u32_t len)
{
	u32_t cnt;

	if(fd == NULL) {
	    return 0;
	}
		
	for(cnt = 0; cnt < len; cnt++) {
		fd->writechar(payload[cnt]);
	}
	
	return len;
}


/************************************************************
sio_read_abort
����������
	��ȡ��ֹ

��ڲ�����
	<fd>[in]:		Ӳ������ID

���ڲ�����
	��

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
void sio_read_abort(sio_fd_t fd)
{	
	fd->readflag = 0;
	return;
}


/************************************************************
sys_jiffies
����������
	��ȡ��ֹ

��ڲ�����
	��

���ڲ�����
	�Կ���������ʱ���жϴ���

������Ϣ��
	2007-08-02	����
	
ע�����
************************************************************/
u32_t sys_jiffies(void)
{
	return OSTimeGet();
}


void sys_free(void)
{
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register               */
    OS_CPU_SR  cpu_sr = 0;
#endif
	s8_t i;
	u8_t err;
	
	OS_ENTER_CRITICAL();
		
	tcpip_deinit();
	pbuf_deinit();
    memp_deinit();
    mem_deinit();
	
	// ������������
	for( i=LWIP_MBOX_MAX-1; i>=0; i-- )
	{
		OSQDel( Lwip_mbox[i].pmbox, OS_DEL_ALWAYS, &err );
		Lwip_mbox[i].pmbox = NULL;
		Lwip_mbox[i].next = NULL;
	}
	Lwip_curfreembox = Lwip_mbox;

	// ���sys_timeouts���飬��ÿ�������Ա����������ַ����ΪNULL
	for(i=0; i<LWIP_THREAD_MAX+1; i++)
		lwip_timeouts[i].next = NULL;

	OS_EXIT_CRITICAL();
}

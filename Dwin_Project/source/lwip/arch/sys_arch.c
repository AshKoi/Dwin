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
 * describe: 	操作系统模拟层接口函数实现文件
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

//	用来创建消息邮箱的空间
static LWIPMBOX Lwip_mbox[LWIP_MBOX_MAX];
//	指向当前空闲邮箱
static pLWIPMBOX Lwip_curfreembox;

//	定时事件链表，最后一个为定时错误的准备的。
struct sys_timeouts lwip_timeouts[LWIP_THREAD_MAX + 1];

//	LwIP线程使用的堆栈
static OS_STK Lwip_thread_stk[LWIP_THREAD_MAX][LWIP_THREAD_STK_SIZE];


unsigned int lwip_con_timout=0;

/************************************************************
sys_ms_to_ticks
功能描述：
	将毫秒转变成时钟节拍

入口参数：
	毫秒数

出口参数：
	滴答数

更新信息：
	2007-08-02	创建

注意事项：
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
功能描述：
	建立一个空的邮箱

入口参数：
	无

出口参数：
	!= SYS_MBOX_NULL: 	邮箱申请成功，返回一个指向被申请邮箱的指针
	 = SYS_MBOX_NULL: 	邮箱没有申请成功

更新信息：
	2007-08-02	创建
	
注意事项：
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
功能描述：
	释放邮箱，将邮箱归还给链表，如果释放时邮箱中还有消息，它
	表明LwIP中存在一个编程错误，应该通知开发者。

入口参数：
	<mbox>[in]:		要归还的邮箱

出口参数：
	无
	
更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
void sys_mbox_free(sys_mbox_t mbox)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  		cpu_sr = 0;
#endif

    //	为了防止意外情况发生，再主动清空一次邮箱
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
功能描述：
	将消息投递到指定的邮箱

入口参数：
	<mbox>[in]:		指定要投递的邮箱
	 <msg>[in]:		指定要投递的消息

出口参数：
	无
	
更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
void sys_mbox_post(sys_mbox_t mbox, void *msg)
{		
		#if  LWIP_TRACE_ERROR
		if(OSPrioCur==10)
		{
			TRACE_ERROR("任务三发送邮箱\n");
		
		}
		else if(OSPrioCur==11)
		{
		
			TRACE_ERROR("任务二发送邮箱\n");
		}
		else if(OSPrioCur==15)
		{
			TRACE_ERROR("任务一发送邮箱\n");
		
		}
		#endif
	while(OSQPost(mbox->pmbox, msg) == OS_Q_FULL) {
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}



/************************************************************
sys_arch_mbox_fetch
功能描述：
	在指定的邮箱接收消息，该函数会阻塞线程直至超时或者邮箱至少
	收到一条消息

入口参数：
	   <mbox>[in]:	指定接收消息的邮箱
	   <msg>[out]:	结果参数，保存接收到的消息指针
	<timeout>[in]:	指定等待接收的最长时间，为0表明一直等待直至接收
					到消息，单位为毫秒
				
出口参数：
	0: 					在指定时间内收到消息
	SYS_ARCH_TIMEOUT: 	在指定时间内没有收到消息
	
更新信息：
	2007-08-02	创建
	
注意事项：		  	#if  LWIP_TRACE_ERROR
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
			TRACE_ERROR("任务三接收到邮箱\n");
		
		}
		else if(OSPrioCur==11)
		{
		
			TRACE_ERROR("任务二接收到邮箱\n");
		}
		else if(OSPrioCur==15)
		{
			TRACE_ERROR("任务一接收到邮箱\n");
		
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
		TRACE_ERROR("任务三发送邮箱\n");
	
	}
	else if(OSPrioCur==11)
	{
	
	   TRACE_ERROR("任务二发送邮箱\n");
	}
	else if(OSPrioCur==15)
	{
		TRACE_ERROR("任务一发送邮箱\n");
	
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
功能描述：
	建立并返回一个新的信号量

入口参数：
	<count>[in]:	指定信号量的初始状态，OS信号量创建入口

出口参数：
	返回新的信号量

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
sys_sem_t sys_sem_new(u8_t count)
{	

	#if  LWIP_TRACE_ERROR
	if(OSPrioCur==10)
	{
		TRACE_ERROR("任务三新建信号量\n");
	
	}
	else if(OSPrioCur==11)
	{
	
		TRACE_ERROR("任务二新建信号量\n");
	}
	else if(OSPrioCur==15)
	{
		TRACE_ERROR("任务一新建信号量\n");
	
	}
	#endif
    return OSSemCreate(count);
}



/************************************************************
sys_sem_signal
功能描述：
	发送信号

入口参数：
	<sem>[in]:		指定指定要发送的信号，OS信号发送入口

出口参数：
	无

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
void sys_sem_signal(sys_sem_t sem)
{	

    OSSemPost(sem);
}


/************************************************************
sys_arch_sem_wait
功能描述：
	等待指定的信号并阻塞线程。timeout参数为0，线程会一直被阻塞
	至收到指定的信号；非0，则线程仅被阻塞至指定的timeout时间
	（单位为毫秒）。在timeout参数值非0的情况下，返回值为等待指
	定的信号所消耗的毫秒数。如果在指定的时间内并没有收到信号，
	返回值为 SYS_ARCH_TIMEOUT。如果线程不必再等待这个信号（也
	就是说，已经收到信号），返回值也可以为0。注意，LwIP实现了
	一个名称与之相似的函数来调用这个函数，sys_sem_wait()，注意
	区别。

入口参数：
	    <sem>[in]:	指定指定要发送的信号，OS信号发送入口
	<timeout>[in]:	指定等待的最长时间（单位为毫秒）。
					 0: 线程会一直被阻塞直至收到指定的信号；
					!0:	指定线程最长等待时间

出口参数：
	0: 					在指定时间内等到指定信号
	SYS_ARCH_TIMEOUT: 	在指定时间内没有等到指定信号

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
	u8_t 	err;

  	OSSemPend(sem, sys_ms_to_ticks(timeout), &err);
	#if  LWIP_TRACE_ERROR
	if(OSPrioCur==10)
	{
		TRACE_ERROR("任务三接收到信号量\n");
	
	}
	else if(OSPrioCur==11)
	{
	
		TRACE_ERROR("任务二接收到信号量\n");
	}
	else if(OSPrioCur==15)
	{
		TRACE_ERROR("任务一接收到信号量\n");
	
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
功能描述：
	发送信号

入口参数：
	<sem>[in]:	指定指定要释放的信号，OS信号删除入口

出口参数：
	无

更新信息：
	2007-08-02	创建
	
注意事项：
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
功能描述：
	根据sys_arch.txt文件建立，功能为初始化sys_arch层

入口参数：
	无

出口参数：
	无
	
更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
void sys_init(void)
{
	u8_t i;
	
	// 建立链表和邮箱
	for(i=0; i<(LWIP_MBOX_MAX-1); i++)
	{
		// 把数组中的各成员链接在一起
		Lwip_mbox[i].next = &Lwip_mbox[i+1];
		// 建立邮箱，系统必须保证邮箱能够顺利建立，如果出现错误，那是程序BUG，应该在调试阶段排除
		Lwip_mbox[i].pmbox = OSQCreate(Lwip_mbox[i].pmsg, LWIP_MBOX_MSG_SIZE);
	}

	// 数组中最后一个元素，它还没有建立邮箱呢
	Lwip_mbox[LWIP_MBOX_MAX-1].next = (pLWIPMBOX)NULL;
	Lwip_mbox[LWIP_MBOX_MAX-1].pmbox = OSQCreate(Lwip_mbox[LWIP_MBOX_MAX - 1].pmsg, LWIP_MBOX_MSG_SIZE);
	// 当前空闲链表首地址
	Lwip_curfreembox = Lwip_mbox;

	// 初始化sys_timeouts数组，将每个数组成员保存的链表地址设置为NULL
	for(i=0; i<LWIP_THREAD_MAX+1; i++)
	{
		lwip_timeouts[i].next = NULL;
	}
	
	//curr_prio_offset = 0;
}

/************************************************************
sys_arch_timeouts
功能描述：
	获取当前线程使用的sys_timeouts结构的指针

入口参数：
	无

出口参数：
	返回一个指向当前线程使用的sys_timeouts结构的指针

更新信息：
	2007-08-02	创建
	
注意事项：
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
    	//	当前线程在指定的LwIP线程优先级号范围之内
  		return &lwip_timeouts[lwip_prio];
  	}
	//	如果出现意外情况，当前线程并不在指定的LwIP线程优先级资源之内
	// 则返回lwip_timeouts数组的最后一个成员
	return &lwip_timeouts[LWIP_THREAD_MAX];
}



/************************************************************
sys_thread_new
功能描述：
	建立一个新线程

入口参数：
	<thread>[in]:	新线程的入口地址
	   <arg>[in]:	传递给新线程的参数
	  <prio>[in]:	由LwIP指定的新线程优先级，这个优先级从1开始
	  				而且并非真正的OS优先级，要经过转化！

出口参数：
	成功:	返回线程优先级，注意这与prio不同。
			这个值实际等于 T_LWIP_THREAD_START_PRIO + prio
    不成功: 返回0

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/


sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
	sys_thread_t os_prio;
	
	//	如果优先级定义没有超出系统允许的范围
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
功能描述：
	打开IO输入硬件接口，初始化硬件。

入口参数：
	<dev>[in]:	硬件设备ID

出口参数：
	设备的ID号

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
sio_fd_t sio_open(u8_t dev)
{
	sio_fd_t fd;
	
	return fd;
}


/************************************************************
sio_send
功能描述：
	通过IO口发送数据

入口参数：
	<dev>[in]:	硬件设备ID
	<fd>[in]:	硬件特征ID

出口参数：
	无

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
void sio_send(u8_t dev, sio_fd_t fd)
{

}


/************************************************************
sio_recv
功能描述：
	通过IO口发送数据

入口参数：
	<fd>[in]:	硬件特征ID

出口参数：
	SLIP_END     0300
	SLIP_ESC     0333
	SLIP_ESC_END 0334
	SLIP_ESC_ESC 0335

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
u8_t sio_recv(sio_fd_t fd)
{
	u8_t ret;
	
	ret = 0;
	
	return ret;
}


/************************************************************
sio_read
功能描述：
	通过IO口读取数据到指定的空间中

入口参数：
	<fd>[in]:		硬件特征ID
	<payload>[in]:	读取数据的缓存区
	<len>[in]:		缓存区的最大长度

出口参数：
	无

更新信息：
	2007-08-02	创建
	
注意事项：
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
功能描述：
	通过IO口写入数据

入口参数：
	<fd>[in]:		硬件特征ID
	<payload>[in]:	待写入的数据缓存区
	<len>[in]:		缓存区的数据长度

出口参数：
	无

更新信息：
	2007-08-02	创建
	
注意事项：
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
功能描述：
	读取终止

入口参数：
	<fd>[in]:		硬件特征ID

出口参数：
	无

更新信息：
	2007-08-02	创建
	
注意事项：
************************************************************/
void sio_read_abort(sio_fd_t fd)
{	
	fd->readflag = 0;
	return;
}


/************************************************************
sys_jiffies
功能描述：
	读取终止

入口参数：
	无

出口参数：
	自开机以来的时钟中断次数

更新信息：
	2007-08-02	创建
	
注意事项：
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
	
	// 清空链表和邮箱
	for( i=LWIP_MBOX_MAX-1; i>=0; i-- )
	{
		OSQDel( Lwip_mbox[i].pmbox, OS_DEL_ALWAYS, &err );
		Lwip_mbox[i].pmbox = NULL;
		Lwip_mbox[i].next = NULL;
	}
	Lwip_curfreembox = Lwip_mbox;

	// 清空sys_timeouts数组，将每个数组成员保存的链表地址设置为NULL
	for(i=0; i<LWIP_THREAD_MAX+1; i++)
		lwip_timeouts[i].next = NULL;

	OS_EXIT_CRITICAL();
}

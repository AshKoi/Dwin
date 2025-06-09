/*
************************************************************************
*
*                  发射机监控系统3.0__嵌入式系统构件
*
************************************************************************

------------------------------------------------------------------------
文 件 名	: debug.c
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

#include <boot/board.h>
#include <AT91/periph/pio/pio.h>
#include <AT91/periph/aic/aic.h>
#include <AT91/periph/dbgu/dbgu.h>
/*
************************************************************************
*							宏定义
************************************************************************
*/
/*define for debug registers*/
#define DEBUG_MODE              AT91C_US_PAR_NONE    /*DEBUG模式寄存设置值          */
#define DEBUG_AIC_SMR           0x01                 /*DEBUG AIC中断触发模式和优先级*/


/*define for debug api function return result*/
#define DEBUG_DONE_OK           1	                 /*DEBUG API操作成功*/
#define DEBUG_DONE_FAIL         0                    /*DEBUG API操作失败*/
															


/*define for tx buf*/
#define DEBUG_TXBUF_SIZE        512                  /*DEBUG 发送缓冲区的大小            */
#define DEBUG_PDCTXBUF_SIZE     64	                 /*DEBUG PDC发送缓冲区的大小(不能为0)*/
#define DEBUG_TXBUF_TOTALSIZE   (DEBUG_TXBUF_SIZE+2*DEBUG_PDCTXBUF_SIZE+1)

/*define for rx buf*/
#define DEBUG_RXBUF_SIZE        512	                 /*DEBUG 接收缓冲区的大小            */
#define DEBUG_PDCRXBUF_SIZE     64                   /*DEBUG PDC接收缓冲区的大小(不能为0)*/
#define DEBUG_RXBUF_TOTALSIZE   (DEBUG_RXBUF_SIZE+2*DEBUG_PDCRXBUF_SIZE+1)

/*
************************************************************************
*							变量定义
************************************************************************
*/
static unsigned char Debug_TxBuf[DEBUG_TXBUF_TOTALSIZE];
static unsigned char Debug_RxBuf[DEBUG_RXBUF_TOTALSIZE];

static struct _DEBUG_MANAGE
{
    unsigned int WrIndex;
	unsigned int RdIndex;
	unsigned int CurrPdcLen;
	unsigned int NextPdcLen;	
	unsigned char *pBuf;

}DebugTxBufManage,DebugRxBufManage;

/*
************************************************************************
*	函数名称:	DEBUG_IrqHandle
*	函数功能:	debug外设中断处理函数
*	入口参数:	无
*	出口参数:	无
************************************************************************
*/
void DEBUG_IrqHandle(void)
{
    unsigned int temp;
	unsigned int next_wrindex;
	unsigned int next_rdindex;
    unsigned int debug_isr;

	//禁止DEBUG AIC中断位


	//读取DEBUG相关的状态寄存器
	debug_isr = AT91C_BASE_DBGU->DBGU_CSR & AT91C_BASE_DBGU->DBGU_IMR;


	/*接收中断处理*/
	//////////////////////////////////////////////////////////////////
	//数据接收错误处理
	if(debug_isr & (AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE)) {
	    //读取错误的数据
	    temp = AT91C_BASE_DBGU->DBGU_RHR;
		//清除错误标志
	    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTSTA;
	}
	//接收缓冲区满中断
	else if(debug_isr & AT91C_US_RXBUFF) {
	    //接收缓冲区已满
		temp = DebugRxBufManage.WrIndex+1;
		if(temp >= DEBUG_RXBUF_TOTALSIZE) {
		    temp = 0;
		}
	    if(temp == DebugRxBufManage.RdIndex) {
		    DebugRxBufManage.CurrPdcLen = 1;
			DebugRxBufManage.NextPdcLen = 0;
		    AT91C_BASE_DBGU->DBGU_RPR = (unsigned int)(DebugRxBufManage.pBuf) + DebugRxBufManage.WrIndex;
		    AT91C_BASE_DBGU->DBGU_RCR = 1;
		}
		else {

		    //调整接收缓冲区写偏移量
		    DebugRxBufManage.WrIndex += DebugRxBufManage.CurrPdcLen+DebugRxBufManage.NextPdcLen;
			//DebugRxBufManage.WrIndex %= DEBUG_RXBUF_TOTALSIZE;
			if(DebugRxBufManage.WrIndex >= DEBUG_RXBUF_TOTALSIZE){
		        DebugRxBufManage.WrIndex -= DEBUG_RXBUF_TOTALSIZE;
			}

			//调整当前PDC接收缓冲区指针
			AT91C_BASE_DBGU->DBGU_RPR = (unsigned int)(DebugRxBufManage.pBuf)+DebugRxBufManage.WrIndex;

			//接收缓冲区可用的空间
			temp = (DebugRxBufManage.RdIndex+DEBUG_RXBUF_TOTALSIZE)-DebugRxBufManage.WrIndex;
			if(temp > DEBUG_RXBUF_TOTALSIZE){
			    temp -= DEBUG_RXBUF_TOTALSIZE;
			}

			//接收缓冲区还有可用的空间: *缓冲区预留一字节不可用*
			if(--temp != 0) {
				//temp > 2*DEBUG_PDCRXBUF_SIZE
				if(temp > (DEBUG_PDCRXBUF_SIZE << 1)) {
				    DebugRxBufManage.NextPdcLen = DEBUG_PDCRXBUF_SIZE;
				    DebugRxBufManage.CurrPdcLen = DEBUG_PDCRXBUF_SIZE;	
				}
				else {
				    //DebugRxBufManage.NextPdcLen = temp/2;
				    DebugRxBufManage.NextPdcLen = (temp >> 1);
				    DebugRxBufManage.CurrPdcLen = temp - DebugRxBufManage.NextPdcLen;	
				}

				//调整PDC接收缓冲区的长度和指针
				next_wrindex = DebugRxBufManage.WrIndex+DebugRxBufManage.CurrPdcLen;
				
				if(next_wrindex >= DEBUG_RXBUF_TOTALSIZE ){
				    DebugRxBufManage.CurrPdcLen = DEBUG_RXBUF_TOTALSIZE-DebugRxBufManage.WrIndex;
					AT91C_BASE_DBGU->DBGU_RNPR = (unsigned int)(DebugRxBufManage.pBuf);
				}
				else {
				    AT91C_BASE_DBGU->DBGU_RNPR = (unsigned int)(DebugRxBufManage.pBuf)+ next_wrindex;
					if(next_wrindex + DebugRxBufManage.NextPdcLen >= DEBUG_RXBUF_TOTALSIZE) {
					    DebugRxBufManage.NextPdcLen = DEBUG_RXBUF_TOTALSIZE-next_wrindex;
					}
				}
				
				//当前PDC接收缓冲区的长度
				AT91C_BASE_DBGU->DBGU_RCR = DebugRxBufManage.CurrPdcLen;

				//下一PDC接收缓冲区的长度
				AT91C_BASE_DBGU->DBGU_RNCR = DebugRxBufManage.NextPdcLen;			
			}
			else {
			    DebugRxBufManage.CurrPdcLen = 1;
				DebugRxBufManage.NextPdcLen = 0;	
				AT91C_BASE_DBGU->DBGU_RCR = DebugRxBufManage.CurrPdcLen;
			}		
		}	
	}
	//接收完成中断
	else if(debug_isr & AT91C_US_ENDRX) {
	    //调整写偏移量
		DebugRxBufManage.WrIndex += DebugRxBufManage.CurrPdcLen;
		//DebugRxBufManage.WrIndex %= DEBUG_RXBUF_TOTALSIZE;
		if(DebugRxBufManage.WrIndex >= DEBUG_RXBUF_TOTALSIZE) {
		    DebugRxBufManage.WrIndex = 0;
		}
				
		//调整当前PDC接收缓冲区的长度为下一个接收缓冲区的长度
		DebugRxBufManage.CurrPdcLen = DebugRxBufManage.NextPdcLen;
	
		//调整PDC下一个接收缓冲区的长度和指针
		next_wrindex = DebugRxBufManage.WrIndex+DebugRxBufManage.CurrPdcLen;
		//next_wrindex = (DebugRxBufManage.WrIndex+DebugRxBufManage.CurrPdcLen) % DEBUG_RXBUF_TOTALSIZE;
		if(next_wrindex >= DEBUG_RXBUF_TOTALSIZE) {
		    next_wrindex = 0;
		}

		temp = (DebugRxBufManage.RdIndex+DEBUG_RXBUF_TOTALSIZE)-next_wrindex;
		if(temp > DEBUG_RXBUF_TOTALSIZE) {
		    temp -= DEBUG_RXBUF_TOTALSIZE;
		}

		//接收缓冲区还有可用的空间: *缓冲区预留一字节不可用*
		if(--temp != 0) {
			if(temp > DEBUG_PDCRXBUF_SIZE) {
			    temp = DEBUG_PDCRXBUF_SIZE;
			}
			if(next_wrindex + temp >= DEBUG_RXBUF_TOTALSIZE) {
			    DebugRxBufManage.NextPdcLen = DEBUG_RXBUF_TOTALSIZE - next_wrindex;
			}
			else {
			    DebugRxBufManage.NextPdcLen = temp;
			}

			AT91C_BASE_DBGU->DBGU_RNPR = (unsigned int)(DebugRxBufManage.pBuf)+ next_wrindex;
			AT91C_BASE_DBGU->DBGU_RNCR = DebugRxBufManage.NextPdcLen;
		}
		else {
		    DebugRxBufManage.NextPdcLen = 0;
		}		
	}
	///////////////////////////////////////////////////


	/*****发送中断处理*******/
	///////////////////////////////////////////////////
    //PDC发送缓冲区为空中断
	if(debug_isr & AT91C_US_TXBUFE) {
		//调整发送缓冲区读偏移量
        DebugTxBufManage.RdIndex += DebugTxBufManage.CurrPdcLen+DebugTxBufManage.NextPdcLen;
		//DebugTxBufManage.RdIndex %= DEBUG_TXBUF_TOTALSIZE;
		if(DebugTxBufManage.RdIndex >= DEBUG_TXBUF_TOTALSIZE) {
	        DebugTxBufManage.RdIndex -= DEBUG_TXBUF_TOTALSIZE;
		}
	
		//发送缓冲区待发数据的空间
		temp = (DebugTxBufManage.WrIndex+DEBUG_TXBUF_TOTALSIZE)-DebugTxBufManage.RdIndex;
		if(temp >= DEBUG_TXBUF_TOTALSIZE) {
		    temp -= DEBUG_TXBUF_TOTALSIZE;
		}

		//发送缓冲区还有待发送的数据
		if(temp > 0) {
            //temp > 2*DEBUG_PDCTXBUF_SIZE
            if(temp > (DEBUG_PDCTXBUF_SIZE << 1)) {
		        DebugTxBufManage.NextPdcLen = DEBUG_PDCTXBUF_SIZE;
				DebugTxBufManage.CurrPdcLen = DEBUG_PDCTXBUF_SIZE;
			}
			else {
		        DebugTxBufManage.NextPdcLen = (temp >> 1);
				DebugTxBufManage.CurrPdcLen = temp - DebugTxBufManage.NextPdcLen;
			}

			//调整当前PDC发送缓冲区的指针
			AT91C_BASE_DBGU->DBGU_TPR = (unsigned int)(DebugTxBufManage.pBuf)+DebugTxBufManage.RdIndex;

			//调整PDC发送缓冲区的长度和指针
			next_rdindex = DebugTxBufManage.RdIndex + DebugTxBufManage.CurrPdcLen;
			if(next_rdindex >= DEBUG_TXBUF_TOTALSIZE) {
			    DebugTxBufManage.CurrPdcLen = DEBUG_TXBUF_TOTALSIZE - DebugTxBufManage.RdIndex;
				AT91C_BASE_DBGU->DBGU_TNPR = (unsigned int)(DebugTxBufManage.pBuf);
			}
			else {
			    AT91C_BASE_DBGU->DBGU_TNPR = (unsigned int)(DebugTxBufManage.pBuf)+next_rdindex;
				if(next_rdindex + DebugTxBufManage.NextPdcLen >= DEBUG_TXBUF_TOTALSIZE) {
				    DebugTxBufManage.NextPdcLen = DEBUG_TXBUF_TOTALSIZE - next_rdindex;
				}
			}

			//当前PDC发送缓冲区的长度
			AT91C_BASE_DBGU->DBGU_TCR = DebugTxBufManage.CurrPdcLen;

			//下一PDC发送缓冲区的长度
			AT91C_BASE_DBGU->DBGU_TNCR = DebugTxBufManage.NextPdcLen;
		}
		else {
		
			DebugTxBufManage.CurrPdcLen = 0;

			DebugTxBufManage.NextPdcLen = 0;

	        //关闭PDC发送完和数据为空中断
			AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_TXBUFE | AT91C_US_ENDTX;
		    //禁止PDC发送
			AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTDIS;
		}	
	}
	//数据发送完中断
	else if(debug_isr & AT91C_US_ENDTX) {
        //调整发送缓冲区读偏移量
		DebugTxBufManage.RdIndex += DebugTxBufManage.CurrPdcLen;
		if(DebugTxBufManage.RdIndex >= DEBUG_TXBUF_TOTALSIZE) {
		    DebugTxBufManage.RdIndex = 0;
		}

		//调整当前PDC发送缓冲区的长度为下一个缓冲区的长度
		DebugTxBufManage.CurrPdcLen = DebugTxBufManage.NextPdcLen;

		//调整PDC下一个发送缓冲区的长度和指针
		next_rdindex = DebugTxBufManage.RdIndex+DebugTxBufManage.CurrPdcLen;
		if(next_rdindex >= DEBUG_TXBUF_TOTALSIZE) {
		    next_rdindex = 0;
		}

		temp = (DebugTxBufManage.WrIndex+DEBUG_TXBUF_TOTALSIZE)-next_rdindex;
		if(temp >= DEBUG_TXBUF_TOTALSIZE) {
		    temp -= DEBUG_TXBUF_TOTALSIZE;
		}

		if(temp > 0) {
			if(temp > DEBUG_PDCTXBUF_SIZE) {
			    temp = DEBUG_PDCTXBUF_SIZE;
			}

			if(next_rdindex + temp >= DEBUG_TXBUF_TOTALSIZE) {
			    DebugTxBufManage.NextPdcLen = DEBUG_TXBUF_TOTALSIZE-next_rdindex;
			}
			else {
			    DebugTxBufManage.NextPdcLen = temp;
			}

			AT91C_BASE_DBGU->DBGU_TNPR = (unsigned int)(DebugTxBufManage.pBuf)+next_rdindex;
			AT91C_BASE_DBGU->DBGU_TNCR = DebugTxBufManage.NextPdcLen;	
		}
		else {

		    DebugTxBufManage.NextPdcLen = 0;
		}
	}
	///////////////////////////////////////////////////

	//使能DEBUG AIC中断
}

/*
**********************************************************************
*	函数名称:	DEBUG_GetChar
*	函数功能:	向debug端口获取一个字符
*	入口参数:	*pCh: 存放获取到的字符
*	出口参数:	DEBUG_DONE_OK  -- 获取成功
*           :   DEBUG_DONE_FAIL-- 获取失败
**********************************************************************
*/
unsigned int DEBUG_GetChar(unsigned char *pCh)
{
  #if DEBUG_PDCRXBUF_SIZE > 1
    unsigned int temp_len;
	unsigned int cpu_sr;
  #endif

    //缓冲区中有可用的数据
    if(DebugRxBufManage.RdIndex != DebugRxBufManage.WrIndex) {
	    *pCh = DebugRxBufManage.pBuf[DebugRxBufManage.RdIndex];
		if(++DebugRxBufManage.RdIndex >= DEBUG_RXBUF_TOTALSIZE) {
		    DebugRxBufManage.RdIndex = 0;
		}
	
		return DEBUG_DONE_OK;
	}

  #if DEBUG_PDCRXBUF_SIZE > 1
	//由于DEBUG没有接收超时定时器，所以需增加对PDC缓冲区的数据查询功能
	else {

	    //临界段保护
	    DEBUG_ENTER_CRITICAL();

		//////////////////////////////////////////////////////////////////////////////////////////
		//1、这里对DBGU_RCR只进行一次读取：PDC正在接收数据时多次读取DBGU_RCR的结果
		//   存在不一样的可能性而导致程序判断错误。
		//2、这里读取DBGU_RCR时，DBGU_RCR有可能已经指向下一PDC缓冲区的大小但不影响程序的判断
		////////////////////////////////////////////////////////////////////////////////////////
		temp_len = AT91C_BASE_DBGU->DBGU_RCR;

		//这里有必要进行一次判断
	    if(DebugRxBufManage.CurrPdcLen > temp_len) {
		    temp_len = DebugRxBufManage.CurrPdcLen-temp_len;

		    DebugRxBufManage.CurrPdcLen -= temp_len;
			//调整写偏移量
			DebugRxBufManage.WrIndex += temp_len;
			if(DebugRxBufManage.WrIndex >= DEBUG_RXBUF_TOTALSIZE){
			    DebugRxBufManage.WrIndex = 0;
			}

		    //退出临界段
	        DEBUG_EXIT_CRITICAL();


			*pCh = DebugRxBufManage.pBuf[DebugRxBufManage.RdIndex];
		    if(++DebugRxBufManage.RdIndex >= DEBUG_RXBUF_TOTALSIZE) {
		        DebugRxBufManage.RdIndex = 0;
		    }

		    return DEBUG_DONE_OK;

		}
		else {
            //退出临界段
	        DEBUG_EXIT_CRITICAL();
		}
	}
  #endif /*#if DEBUG_PDCRXBUF_SIZE > 1*/

    return DEBUG_DONE_FAIL;
}
/*
**********************************************************************
*	函数名称:	DEBUG_IsTxBufReady
*	函数功能:	检测DEBUG发送缓冲区是否可用
*	入口参数:	无
*	出口参数:	0--发送缓冲区不可用  !0-- 有可用的发送缓冲区空间
**********************************************************************
*/
unsigned int DEBUG_IsTxBufReady(void)
{
    unsigned int temp;

	temp = (DebugTxBufManage.RdIndex+DEBUG_TXBUF_TOTALSIZE)-DebugTxBufManage.WrIndex;
	if(temp > DEBUG_TXBUF_TOTALSIZE) {
	    temp -= DEBUG_TXBUF_TOTALSIZE;
	}

	//发送缓冲区还有可用的空间: *缓冲区预留一字节不可用*
	if(--temp > DEBUG_TXBUF_TOTALSIZE-1) {
	    temp = DEBUG_TXBUF_TOTALSIZE-1;
	}

	return temp;
}

/*
**********************************************************************
*	函数名称:	DEBUG_PutChar
*	函数功能:	向debug端口发送一个字符
*	入口参数:	ch: 待发送的字符
*	出口参数:	DEBUG_DONE_OK   -- 发送成功
*           :   DEBUG_DONE_FAIL -- 发送失败
**********************************************************************
*/
unsigned int DEBUG_PutChar(unsigned char ch)
{
    //缓冲区是否可用: 之所以不直接判断是为了防止被编译器优化
	while(!DEBUG_IsTxBufReady()) {
	   AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTEN;
	   AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXBUFE | AT91C_US_ENDTX;
	}

	DebugTxBufManage.pBuf[DebugTxBufManage.WrIndex] = ch;
	if(++DebugTxBufManage.WrIndex >= DEBUG_TXBUF_TOTALSIZE) {
        DebugTxBufManage.WrIndex = 0;
	}

	//发送中断未使能
	if((AT91C_BASE_DBGU->DBGU_IMR & (AT91C_US_TXBUFE | AT91C_US_ENDTX)) == 0) {
	    DebugTxBufManage.CurrPdcLen = 1;
		AT91C_BASE_DBGU->DBGU_TPR = (unsigned int)(DebugTxBufManage.pBuf)+DebugTxBufManage.RdIndex;
		AT91C_BASE_DBGU->DBGU_TCR = 1;

	    //启动PDC发送
	    AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTEN;
        //PDC发送中断使能
	    AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXBUFE | AT91C_US_ENDTX;
    }

	return DEBUG_DONE_OK;
}

/*
**********************************************************************
*	函数名称:	DEBUG_SendStr
*	函数功能:	向debug端口发送一组数据
*	入口参数:   pBuf: 待发送数据的首地址	
*           ：  len:   待发送数据的长度
*   出口参数:	DEBUG_DONE_OK   -- 发送成功
*           :   DEBUG_DONE_FAIL -- 发送失败	
**********************************************************************
*/
unsigned int DEBUG_SendStr(unsigned char *pBuf,unsigned int len)
{
    unsigned int i;
	unsigned int offer;
    unsigned int availlen;

	offer = 0;
    while(len) {
        //缓冲区是否可用://之所以不直接判断是为了防止被编译器优化
	    while(!(availlen = DEBUG_IsTxBufReady())) {
	        AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTEN;
		    AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXBUFE | AT91C_US_ENDTX;
	    }

	    if(availlen > len) {
	        availlen = len;
	    }

	    for(i = 0; i < availlen; i++) {
	        DebugTxBufManage.pBuf[DebugTxBufManage.WrIndex] = pBuf[offer++];
	        if(++DebugTxBufManage.WrIndex >= DEBUG_TXBUF_TOTALSIZE) {
                DebugTxBufManage.WrIndex = 0;
	        }
	    }


		len -= availlen;

		if((AT91C_BASE_DBGU->DBGU_IMR & (AT91C_US_TXBUFE | AT91C_US_ENDTX)) == 0) {
		    if(availlen > DEBUG_PDCTXBUF_SIZE) {
			    availlen = DEBUG_PDCTXBUF_SIZE;
			}
		    DebugTxBufManage.CurrPdcLen = availlen;
		    AT91C_BASE_DBGU->DBGU_TPR = (unsigned int)(DebugTxBufManage.pBuf)+DebugTxBufManage.RdIndex;
		    AT91C_BASE_DBGU->DBGU_TCR = availlen;

	        //启动PDC发送
	        AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTEN;
            //PDC发送中断使能
	        AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXBUFE | AT91C_US_ENDTX;
		}
    }

	return DEBUG_DONE_OK;
}

/*
**********************************************************************
*	函数名称:	DEBUG_Init
*	函数功能:	debug端口初始化
*	入口参数:	baudrate  波特率(e.g. 115200)
*           ：  mck       系统主机时钟(Hz)
*	出口参数:	无
**********************************************************************
*/
void DEBUG_Init(unsigned int baudrate,unsigned int mck)
{
    const Pin pinsDbgu[] = {PINS_DBGU};
		
    //配置DEBUG 外设I/O
	PIO_Configure(pinsDbgu, PIO_LISTSIZE(pinsDbgu));

	//禁止DEBUG所有的中断
    AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;

    //复位&禁接收和发送 逻辑
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS;

	//配置波特率(四舍五入) mck/(baudrate * 16) + 0.5;
	AT91C_BASE_DBGU->DBGU_BRGR = (mck*10 / (baudrate * 16) + 5)/10;

	//配置模式寄存器
	AT91C_BASE_DBGU->DBGU_MR = DEBUG_MODE;

	////////////////////////////////////////////////////////////////////
	//初始化缓冲管理
	DebugTxBufManage.pBuf = Debug_TxBuf;
	DebugTxBufManage.WrIndex = 0;
	DebugTxBufManage.RdIndex = 0;
	DebugTxBufManage.CurrPdcLen = 0;
	DebugTxBufManage.NextPdcLen = 0;
    /////////////////////////////////////////////////////////////////////
	DebugRxBufManage.pBuf = Debug_RxBuf;
	DebugRxBufManage.WrIndex = 0;
	DebugRxBufManage.RdIndex = 0;
	DebugRxBufManage.CurrPdcLen = DEBUG_PDCRXBUF_SIZE;
	DebugRxBufManage.NextPdcLen = DEBUG_PDCRXBUF_SIZE;
	/////////////////////////////////////////////////////////////////////

	
	//禁用PDC收发通道
	AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;
	//复位DEBUG所有PDC指针和计数器
 	AT91C_BASE_DBGU->DBGU_TPR = 0;
	AT91C_BASE_DBGU->DBGU_TCR = 0;
	AT91C_BASE_DBGU->DBGU_TNPR = 0;	
	AT91C_BASE_DBGU->DBGU_TNCR = 0;
	////////////////////////////////		
	AT91C_BASE_DBGU->DBGU_RPR = (unsigned int)(DebugRxBufManage.pBuf);
	AT91C_BASE_DBGU->DBGU_RCR = DebugRxBufManage.CurrPdcLen;
	AT91C_BASE_DBGU->DBGU_RNPR = (unsigned int)(DebugRxBufManage.pBuf)+DebugRxBufManage.CurrPdcLen;
	AT91C_BASE_DBGU->DBGU_RNCR = DebugRxBufManage.NextPdcLen;
	//使能PDC接收通道
	AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTEN;


    //配置DEBUG AIC中断
	AIC_ConfigureIT(AT91C_ID_SYS, DEBUG_AIC_SMR, at91_aic_sysirq_handle);
	//DEBUG AIC中断使能
	AIC_EnableIT(AT91C_ID_SYS);

	//配置DEBUG中断
	AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXBUFF | AT91C_US_ENDRX | AT91C_US_OVRE | AT91C_US_FRAME; //|AT91C_US_PARE;

	//允许接收
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN;
	//允许发送
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;

}

/*
--------------------------------------------------------------------------------
                            库函数接口
--------------------------------------------------------------------------------
*/
#ifndef NOFPUT
#include <stdarg.h>
#include <stdio.h>

//------------------------------------------------------------------------------
/// \exclude
/// Implementation of fputc using the DBGU as the standard output. Required
/// for printf().
/// \param c  Character to write.
/// \param pStream  Output stream.
/// \param The character written if successful, or -1 if the output stream is
/// not stdout or stderr.
//------------------------------------------------------------------------------
signed int fputc(signed int c, FILE *pStream)
{
    if ((pStream == stdout) || (pStream == stderr)) {

        //DEBUG_PutChar(c);
		extern void dbgu_putc( unsigned char c );
		dbgu_putc( c );
        return c;
    }
    else {

        return EOF;
    }
}

//------------------------------------------------------------------------------
/// \exclude
/// Implementation of fputs using the DBGU as the standard output. Required
/// for printf(). Does NOT currently use the PDC.
/// \param pStr  String to write.
/// \param pStream  Output stream.
/// \return Number of characters written if successful, or -1 if the output
/// stream is not stdout or stderr.
//------------------------------------------------------------------------------
signed int fputs(const char *pStr, FILE *pStream)
{
    signed int num = 0;

    while (*pStr != 0) {

        if (fputc(*pStr, pStream) == -1) {

            return -1;
        }
        num++;
        pStr++;
    }

    return num;
}

#undef putchar

//------------------------------------------------------------------------------
/// \exclude
/// Outputs a character on the DBGU.
/// \param c  Character to output.
/// \return The character that was output.
//------------------------------------------------------------------------------
signed int putchar(signed int c)
{
    return fputc(c, stdout);
}

#endif //#ifndef NOFPUT

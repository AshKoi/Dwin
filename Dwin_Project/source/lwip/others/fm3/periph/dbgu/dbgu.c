/*
************************************************************************
*
*                  ��������ϵͳ3.0__Ƕ��ʽϵͳ����
*
************************************************************************

------------------------------------------------------------------------
�� �� ��	: debug.c
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

#include <boot/board.h>
#include <AT91/periph/pio/pio.h>
#include <AT91/periph/aic/aic.h>
#include <AT91/periph/dbgu/dbgu.h>
/*
************************************************************************
*							�궨��
************************************************************************
*/
/*define for debug registers*/
#define DEBUG_MODE              AT91C_US_PAR_NONE    /*DEBUGģʽ�Ĵ�����ֵ          */
#define DEBUG_AIC_SMR           0x01                 /*DEBUG AIC�жϴ���ģʽ�����ȼ�*/


/*define for debug api function return result*/
#define DEBUG_DONE_OK           1	                 /*DEBUG API�����ɹ�*/
#define DEBUG_DONE_FAIL         0                    /*DEBUG API����ʧ��*/
															


/*define for tx buf*/
#define DEBUG_TXBUF_SIZE        512                  /*DEBUG ���ͻ������Ĵ�С            */
#define DEBUG_PDCTXBUF_SIZE     64	                 /*DEBUG PDC���ͻ������Ĵ�С(����Ϊ0)*/
#define DEBUG_TXBUF_TOTALSIZE   (DEBUG_TXBUF_SIZE+2*DEBUG_PDCTXBUF_SIZE+1)

/*define for rx buf*/
#define DEBUG_RXBUF_SIZE        512	                 /*DEBUG ���ջ������Ĵ�С            */
#define DEBUG_PDCRXBUF_SIZE     64                   /*DEBUG PDC���ջ������Ĵ�С(����Ϊ0)*/
#define DEBUG_RXBUF_TOTALSIZE   (DEBUG_RXBUF_SIZE+2*DEBUG_PDCRXBUF_SIZE+1)

/*
************************************************************************
*							��������
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
*	��������:	DEBUG_IrqHandle
*	��������:	debug�����жϴ�����
*	��ڲ���:	��
*	���ڲ���:	��
************************************************************************
*/
void DEBUG_IrqHandle(void)
{
    unsigned int temp;
	unsigned int next_wrindex;
	unsigned int next_rdindex;
    unsigned int debug_isr;

	//��ֹDEBUG AIC�ж�λ


	//��ȡDEBUG��ص�״̬�Ĵ���
	debug_isr = AT91C_BASE_DBGU->DBGU_CSR & AT91C_BASE_DBGU->DBGU_IMR;


	/*�����жϴ���*/
	//////////////////////////////////////////////////////////////////
	//���ݽ��մ�����
	if(debug_isr & (AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE)) {
	    //��ȡ���������
	    temp = AT91C_BASE_DBGU->DBGU_RHR;
		//��������־
	    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTSTA;
	}
	//���ջ��������ж�
	else if(debug_isr & AT91C_US_RXBUFF) {
	    //���ջ���������
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

		    //�������ջ�����дƫ����
		    DebugRxBufManage.WrIndex += DebugRxBufManage.CurrPdcLen+DebugRxBufManage.NextPdcLen;
			//DebugRxBufManage.WrIndex %= DEBUG_RXBUF_TOTALSIZE;
			if(DebugRxBufManage.WrIndex >= DEBUG_RXBUF_TOTALSIZE){
		        DebugRxBufManage.WrIndex -= DEBUG_RXBUF_TOTALSIZE;
			}

			//������ǰPDC���ջ�����ָ��
			AT91C_BASE_DBGU->DBGU_RPR = (unsigned int)(DebugRxBufManage.pBuf)+DebugRxBufManage.WrIndex;

			//���ջ��������õĿռ�
			temp = (DebugRxBufManage.RdIndex+DEBUG_RXBUF_TOTALSIZE)-DebugRxBufManage.WrIndex;
			if(temp > DEBUG_RXBUF_TOTALSIZE){
			    temp -= DEBUG_RXBUF_TOTALSIZE;
			}

			//���ջ��������п��õĿռ�: *������Ԥ��һ�ֽڲ�����*
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

				//����PDC���ջ������ĳ��Ⱥ�ָ��
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
				
				//��ǰPDC���ջ������ĳ���
				AT91C_BASE_DBGU->DBGU_RCR = DebugRxBufManage.CurrPdcLen;

				//��һPDC���ջ������ĳ���
				AT91C_BASE_DBGU->DBGU_RNCR = DebugRxBufManage.NextPdcLen;			
			}
			else {
			    DebugRxBufManage.CurrPdcLen = 1;
				DebugRxBufManage.NextPdcLen = 0;	
				AT91C_BASE_DBGU->DBGU_RCR = DebugRxBufManage.CurrPdcLen;
			}		
		}	
	}
	//��������ж�
	else if(debug_isr & AT91C_US_ENDRX) {
	    //����дƫ����
		DebugRxBufManage.WrIndex += DebugRxBufManage.CurrPdcLen;
		//DebugRxBufManage.WrIndex %= DEBUG_RXBUF_TOTALSIZE;
		if(DebugRxBufManage.WrIndex >= DEBUG_RXBUF_TOTALSIZE) {
		    DebugRxBufManage.WrIndex = 0;
		}
				
		//������ǰPDC���ջ������ĳ���Ϊ��һ�����ջ������ĳ���
		DebugRxBufManage.CurrPdcLen = DebugRxBufManage.NextPdcLen;
	
		//����PDC��һ�����ջ������ĳ��Ⱥ�ָ��
		next_wrindex = DebugRxBufManage.WrIndex+DebugRxBufManage.CurrPdcLen;
		//next_wrindex = (DebugRxBufManage.WrIndex+DebugRxBufManage.CurrPdcLen) % DEBUG_RXBUF_TOTALSIZE;
		if(next_wrindex >= DEBUG_RXBUF_TOTALSIZE) {
		    next_wrindex = 0;
		}

		temp = (DebugRxBufManage.RdIndex+DEBUG_RXBUF_TOTALSIZE)-next_wrindex;
		if(temp > DEBUG_RXBUF_TOTALSIZE) {
		    temp -= DEBUG_RXBUF_TOTALSIZE;
		}

		//���ջ��������п��õĿռ�: *������Ԥ��һ�ֽڲ�����*
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


	/*****�����жϴ���*******/
	///////////////////////////////////////////////////
    //PDC���ͻ�����Ϊ���ж�
	if(debug_isr & AT91C_US_TXBUFE) {
		//�������ͻ�������ƫ����
        DebugTxBufManage.RdIndex += DebugTxBufManage.CurrPdcLen+DebugTxBufManage.NextPdcLen;
		//DebugTxBufManage.RdIndex %= DEBUG_TXBUF_TOTALSIZE;
		if(DebugTxBufManage.RdIndex >= DEBUG_TXBUF_TOTALSIZE) {
	        DebugTxBufManage.RdIndex -= DEBUG_TXBUF_TOTALSIZE;
		}
	
		//���ͻ������������ݵĿռ�
		temp = (DebugTxBufManage.WrIndex+DEBUG_TXBUF_TOTALSIZE)-DebugTxBufManage.RdIndex;
		if(temp >= DEBUG_TXBUF_TOTALSIZE) {
		    temp -= DEBUG_TXBUF_TOTALSIZE;
		}

		//���ͻ��������д����͵�����
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

			//������ǰPDC���ͻ�������ָ��
			AT91C_BASE_DBGU->DBGU_TPR = (unsigned int)(DebugTxBufManage.pBuf)+DebugTxBufManage.RdIndex;

			//����PDC���ͻ������ĳ��Ⱥ�ָ��
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

			//��ǰPDC���ͻ������ĳ���
			AT91C_BASE_DBGU->DBGU_TCR = DebugTxBufManage.CurrPdcLen;

			//��һPDC���ͻ������ĳ���
			AT91C_BASE_DBGU->DBGU_TNCR = DebugTxBufManage.NextPdcLen;
		}
		else {
		
			DebugTxBufManage.CurrPdcLen = 0;

			DebugTxBufManage.NextPdcLen = 0;

	        //�ر�PDC�����������Ϊ���ж�
			AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_TXBUFE | AT91C_US_ENDTX;
		    //��ֹPDC����
			AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTDIS;
		}	
	}
	//���ݷ������ж�
	else if(debug_isr & AT91C_US_ENDTX) {
        //�������ͻ�������ƫ����
		DebugTxBufManage.RdIndex += DebugTxBufManage.CurrPdcLen;
		if(DebugTxBufManage.RdIndex >= DEBUG_TXBUF_TOTALSIZE) {
		    DebugTxBufManage.RdIndex = 0;
		}

		//������ǰPDC���ͻ������ĳ���Ϊ��һ���������ĳ���
		DebugTxBufManage.CurrPdcLen = DebugTxBufManage.NextPdcLen;

		//����PDC��һ�����ͻ������ĳ��Ⱥ�ָ��
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

	//ʹ��DEBUG AIC�ж�
}

/*
**********************************************************************
*	��������:	DEBUG_GetChar
*	��������:	��debug�˿ڻ�ȡһ���ַ�
*	��ڲ���:	*pCh: ��Ż�ȡ�����ַ�
*	���ڲ���:	DEBUG_DONE_OK  -- ��ȡ�ɹ�
*           :   DEBUG_DONE_FAIL-- ��ȡʧ��
**********************************************************************
*/
unsigned int DEBUG_GetChar(unsigned char *pCh)
{
  #if DEBUG_PDCRXBUF_SIZE > 1
    unsigned int temp_len;
	unsigned int cpu_sr;
  #endif

    //���������п��õ�����
    if(DebugRxBufManage.RdIndex != DebugRxBufManage.WrIndex) {
	    *pCh = DebugRxBufManage.pBuf[DebugRxBufManage.RdIndex];
		if(++DebugRxBufManage.RdIndex >= DEBUG_RXBUF_TOTALSIZE) {
		    DebugRxBufManage.RdIndex = 0;
		}
	
		return DEBUG_DONE_OK;
	}

  #if DEBUG_PDCRXBUF_SIZE > 1
	//����DEBUGû�н��ճ�ʱ��ʱ�������������Ӷ�PDC�����������ݲ�ѯ����
	else {

	    //�ٽ�α���
	    DEBUG_ENTER_CRITICAL();

		//////////////////////////////////////////////////////////////////////////////////////////
		//1�������DBGU_RCRֻ����һ�ζ�ȡ��PDC���ڽ�������ʱ��ζ�ȡDBGU_RCR�Ľ��
		//   ���ڲ�һ���Ŀ����Զ����³����жϴ���
		//2�������ȡDBGU_RCRʱ��DBGU_RCR�п����Ѿ�ָ����һPDC�������Ĵ�С����Ӱ�������ж�
		////////////////////////////////////////////////////////////////////////////////////////
		temp_len = AT91C_BASE_DBGU->DBGU_RCR;

		//�����б�Ҫ����һ���ж�
	    if(DebugRxBufManage.CurrPdcLen > temp_len) {
		    temp_len = DebugRxBufManage.CurrPdcLen-temp_len;

		    DebugRxBufManage.CurrPdcLen -= temp_len;
			//����дƫ����
			DebugRxBufManage.WrIndex += temp_len;
			if(DebugRxBufManage.WrIndex >= DEBUG_RXBUF_TOTALSIZE){
			    DebugRxBufManage.WrIndex = 0;
			}

		    //�˳��ٽ��
	        DEBUG_EXIT_CRITICAL();


			*pCh = DebugRxBufManage.pBuf[DebugRxBufManage.RdIndex];
		    if(++DebugRxBufManage.RdIndex >= DEBUG_RXBUF_TOTALSIZE) {
		        DebugRxBufManage.RdIndex = 0;
		    }

		    return DEBUG_DONE_OK;

		}
		else {
            //�˳��ٽ��
	        DEBUG_EXIT_CRITICAL();
		}
	}
  #endif /*#if DEBUG_PDCRXBUF_SIZE > 1*/

    return DEBUG_DONE_FAIL;
}
/*
**********************************************************************
*	��������:	DEBUG_IsTxBufReady
*	��������:	���DEBUG���ͻ������Ƿ����
*	��ڲ���:	��
*	���ڲ���:	0--���ͻ�����������  !0-- �п��õķ��ͻ������ռ�
**********************************************************************
*/
unsigned int DEBUG_IsTxBufReady(void)
{
    unsigned int temp;

	temp = (DebugTxBufManage.RdIndex+DEBUG_TXBUF_TOTALSIZE)-DebugTxBufManage.WrIndex;
	if(temp > DEBUG_TXBUF_TOTALSIZE) {
	    temp -= DEBUG_TXBUF_TOTALSIZE;
	}

	//���ͻ��������п��õĿռ�: *������Ԥ��һ�ֽڲ�����*
	if(--temp > DEBUG_TXBUF_TOTALSIZE-1) {
	    temp = DEBUG_TXBUF_TOTALSIZE-1;
	}

	return temp;
}

/*
**********************************************************************
*	��������:	DEBUG_PutChar
*	��������:	��debug�˿ڷ���һ���ַ�
*	��ڲ���:	ch: �����͵��ַ�
*	���ڲ���:	DEBUG_DONE_OK   -- ���ͳɹ�
*           :   DEBUG_DONE_FAIL -- ����ʧ��
**********************************************************************
*/
unsigned int DEBUG_PutChar(unsigned char ch)
{
    //�������Ƿ����: ֮���Բ�ֱ���ж���Ϊ�˷�ֹ���������Ż�
	while(!DEBUG_IsTxBufReady()) {
	   AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTEN;
	   AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXBUFE | AT91C_US_ENDTX;
	}

	DebugTxBufManage.pBuf[DebugTxBufManage.WrIndex] = ch;
	if(++DebugTxBufManage.WrIndex >= DEBUG_TXBUF_TOTALSIZE) {
        DebugTxBufManage.WrIndex = 0;
	}

	//�����ж�δʹ��
	if((AT91C_BASE_DBGU->DBGU_IMR & (AT91C_US_TXBUFE | AT91C_US_ENDTX)) == 0) {
	    DebugTxBufManage.CurrPdcLen = 1;
		AT91C_BASE_DBGU->DBGU_TPR = (unsigned int)(DebugTxBufManage.pBuf)+DebugTxBufManage.RdIndex;
		AT91C_BASE_DBGU->DBGU_TCR = 1;

	    //����PDC����
	    AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTEN;
        //PDC�����ж�ʹ��
	    AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXBUFE | AT91C_US_ENDTX;
    }

	return DEBUG_DONE_OK;
}

/*
**********************************************************************
*	��������:	DEBUG_SendStr
*	��������:	��debug�˿ڷ���һ������
*	��ڲ���:   pBuf: ���������ݵ��׵�ַ	
*           ��  len:   ���������ݵĳ���
*   ���ڲ���:	DEBUG_DONE_OK   -- ���ͳɹ�
*           :   DEBUG_DONE_FAIL -- ����ʧ��	
**********************************************************************
*/
unsigned int DEBUG_SendStr(unsigned char *pBuf,unsigned int len)
{
    unsigned int i;
	unsigned int offer;
    unsigned int availlen;

	offer = 0;
    while(len) {
        //�������Ƿ����://֮���Բ�ֱ���ж���Ϊ�˷�ֹ���������Ż�
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

	        //����PDC����
	        AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTEN;
            //PDC�����ж�ʹ��
	        AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXBUFE | AT91C_US_ENDTX;
		}
    }

	return DEBUG_DONE_OK;
}

/*
**********************************************************************
*	��������:	DEBUG_Init
*	��������:	debug�˿ڳ�ʼ��
*	��ڲ���:	baudrate  ������(e.g. 115200)
*           ��  mck       ϵͳ����ʱ��(Hz)
*	���ڲ���:	��
**********************************************************************
*/
void DEBUG_Init(unsigned int baudrate,unsigned int mck)
{
    const Pin pinsDbgu[] = {PINS_DBGU};
		
    //����DEBUG ����I/O
	PIO_Configure(pinsDbgu, PIO_LISTSIZE(pinsDbgu));

	//��ֹDEBUG���е��ж�
    AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;

    //��λ&�����պͷ��� �߼�
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS;

	//���ò�����(��������) mck/(baudrate * 16) + 0.5;
	AT91C_BASE_DBGU->DBGU_BRGR = (mck*10 / (baudrate * 16) + 5)/10;

	//����ģʽ�Ĵ���
	AT91C_BASE_DBGU->DBGU_MR = DEBUG_MODE;

	////////////////////////////////////////////////////////////////////
	//��ʼ���������
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

	
	//����PDC�շ�ͨ��
	AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;
	//��λDEBUG����PDCָ��ͼ�����
 	AT91C_BASE_DBGU->DBGU_TPR = 0;
	AT91C_BASE_DBGU->DBGU_TCR = 0;
	AT91C_BASE_DBGU->DBGU_TNPR = 0;	
	AT91C_BASE_DBGU->DBGU_TNCR = 0;
	////////////////////////////////		
	AT91C_BASE_DBGU->DBGU_RPR = (unsigned int)(DebugRxBufManage.pBuf);
	AT91C_BASE_DBGU->DBGU_RCR = DebugRxBufManage.CurrPdcLen;
	AT91C_BASE_DBGU->DBGU_RNPR = (unsigned int)(DebugRxBufManage.pBuf)+DebugRxBufManage.CurrPdcLen;
	AT91C_BASE_DBGU->DBGU_RNCR = DebugRxBufManage.NextPdcLen;
	//ʹ��PDC����ͨ��
	AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTEN;


    //����DEBUG AIC�ж�
	AIC_ConfigureIT(AT91C_ID_SYS, DEBUG_AIC_SMR, at91_aic_sysirq_handle);
	//DEBUG AIC�ж�ʹ��
	AIC_EnableIT(AT91C_ID_SYS);

	//����DEBUG�ж�
	AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXBUFF | AT91C_US_ENDRX | AT91C_US_OVRE | AT91C_US_FRAME; //|AT91C_US_PARE;

	//�������
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN;
	//������
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;

}

/*
--------------------------------------------------------------------------------
                            �⺯���ӿ�
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

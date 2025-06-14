/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "AT91/periph/aic/aic.h"
#include <AT91/periph/pit/pit.h>
#include <boot/board.h>

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configures an interrupt in the AIC. The interrupt is identified by its
/// source (AT91C_ID_xxx) and is configured to use the specified mode and
/// interrupt handler function. Mode is the value that will be put in AIC_SMRx
/// and the function address will be set in AIC_SVRx.
/// The interrupt is disabled before configuration, so it is useless
/// to do it before calling this function. When AIC_ConfigureIT returns, the
/// interrupt will always be disabled and cleared; it must be enabled by a
/// call to AIC_EnableIT().
/// \param source  Interrupt source to configure.
/// \param mode  Triggering mode and priority of the interrupt.
/// \param handler  Interrupt handler function.
//------------------------------------------------------------------------------
void AIC_ConfigureIT(
    unsigned int source,
    unsigned int mode,
    void (*handler)(void))
{
    // Disable the interrupt first
    AT91C_BASE_AIC->AIC_IDCR = 1 << source;

    // Configure mode and handler
    AT91C_BASE_AIC->AIC_SMR[source] = mode;
    AT91C_BASE_AIC->AIC_SVR[source] = (unsigned int) handler;

    // Clear interrupt
    AT91C_BASE_AIC->AIC_ICCR = 1 << source;
}

//------------------------------------------------------------------------------
/// Enables interrupts coming from the given (unique) source (AT91C_ID_xxx).
/// \param source  Interrupt source to enable.
//------------------------------------------------------------------------------
void AIC_EnableIT(unsigned int source)
{
    AT91C_BASE_AIC->AIC_IECR = 1 << source;
}

//------------------------------------------------------------------------------
/// Disables interrupts coming from the given (unique) source (AT91C_ID_xxx).
/// \param source  Interrupt source to enable.
//------------------------------------------------------------------------------
void AIC_DisableIT(unsigned int source)
{
    AT91C_BASE_AIC->AIC_IDCR = 1 << source;
}

void AT91F_AIC_AcknowledgeIt()
{
    AT91C_BASE_AIC->AIC_EOICR  =  AT91C_BASE_AIC->AIC_EOICR;
}
/*
**********************************************************************
*	函数名称:	at91_aic_sysirq_handle
*	函数功能:	aic系统中断处理函数
*	入口参数:	无
*	出口参数:	无
**********************************************************************
*/
void at91_aic_sysirq_handle(void)
{    

	AIC_DisableIT(AT91C_ID_SYS);
	at91_pit_tick_isr();
	DEBUG_IrqHandle();

    AIC_EnableIT( AT91C_ID_SYS);
	AT91F_AIC_AcknowledgeIt();
}

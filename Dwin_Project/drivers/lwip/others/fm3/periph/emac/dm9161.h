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

#ifndef _DM9161_H
#define _DM9161_H
#include "others/fm3/periph/emac/emac.h"
#define BMSR_100BASE_T4        (1 << 15) // 100BASE-T4 Capable
#define BMSR_100BASE_TX_FD     (1 << 14) // 100BASE-TX Full Duplex Capable
#define BMSR_100BASE_T4_HD     (1 << 13) // 100BASE-TX Half Duplex Capable
#define BMSR_10BASE_T_FD       (1 << 12) // 10BASE-T Full Duplex Capable
#define BMSR_10BASE_T_HD       (1 << 11) // 10BASE-T Half Duplex Capable
//      Reserved                  10 to 7  // Read as 0, ignore on write
#define BMSR_MF_PREAMB_SUPPR   (1 << 6)  // MII Frame Preamble Suppression
#define BMSR_AUTONEG_COMP      (1 << 5)  // Auto-negotiation Complete
#define BMSR_REMOTE_FAULT      (1 << 4)  // Remote Fault
#define BMSR_AUTONEG_ABILITY   (1 << 3)  // Auto Configuration Ability
#define BMSR_LINK_STATUS       (1 << 2)  // Link Status
#define BMSR_JABBER_DETECT     (1 << 1)  // Jabber Detect
#define BMSR_EXTEND_CAPAB      (1 << 0)  // Extended Capability

static void DM9161_DumpRegisters(Dm9161 * pDm);
static unsigned char DM9161_FindValidPhy(Dm9161 * pDm);
void DM9161_SetupTimeout(Dm9161 *pDm, unsigned int toMax);
void DM9161_Init(Dm9161 *pDm, unsigned char phyAddress);
static unsigned char DM9161_ResetPhy(Dm9161 *pDm);
unsigned char DM9161_InitPhy(Dm9161       *pDm);
unsigned char DM9161_AutoNegotiate(Dm9161 *pDm);
unsigned char DM9161_GetLinkSpeed(Dm9161 *pDm, unsigned char applySetting);







#endif // #ifndef _DM9161_H
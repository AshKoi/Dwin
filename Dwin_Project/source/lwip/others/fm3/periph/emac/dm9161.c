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

//#include <boot/board.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include "others/fm3/periph/emac/dm9161.h"
#include "others/fm3/periph/emac/dm9161_define.h"

#define DM9161_RESET_LENGTH         0xD
//#define DM9161_RETRY_MAX            500
//-----------------------------------------------------------------------------
//         Definitions
//-----------------------------------------------------------------------------

/// Default max retry count
#define DM9161_RETRY_MAX            500

#include "emac_api.h"
unsigned char gDriverEmacDebugLevel = 0;       // no debug info
unsigned int  gDriverEmacTimeout = 60;         // 60 second

//
#define PHY_TYPE_DM9161     0
#define PHY_TYPE_LAN8720A   1

unsigned char gDriverEmacPhyType = PHY_TYPE_DM9161;

void DM9161_SetParam( ENET_PARAM *pEnetParam )
{
    gDriverEmacDebugLevel = pEnetParam->nDebugLevel;
    traceLevel = pEnetParam->nDebugLevel;
    if( (pEnetParam->nTimeout >= 10) && (pEnetParam->nTimeout < 5*60*60) )
    {
        gDriverEmacTimeout = pEnetParam->nTimeout;
    }
}

void  lwipDelay(unsigned ms)
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<10000;j++);
}
unsigned long g_DumpCount = 0;

//-----------------------------------------------------------------------------
/// Dump all the useful registers
/// \param pDm          Pointer to the Dm9161 instance
//-----------------------------------------------------------------------------
static void DM9161_DumpRegisters(Dm9161 * pDm)
{
    unsigned char phyAddress;
    unsigned int retryMax;
    unsigned int value;

   // EMAC_EnableMdio();
    phyAddress = pDm->phyAddress;
    retryMax = pDm->retryMax;

    g_DumpCount++;
    EMAC_ReadPhy(phyAddress, DM9161_BMCR, &value, retryMax);
    TRACE_INFO(" _BMCR   : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, DM9161_BMSR, &value, retryMax);
    TRACE_INFO(" _BMSR   : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, DM9161_ANAR, &value, retryMax);
    TRACE_INFO(" _ANAR   : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, DM9161_ANLPAR, &value, retryMax);
    TRACE_INFO(" _ANLPAR : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, DM9161_ANER, &value, retryMax);
    TRACE_INFO(" _ANER   : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, 17, &value, retryMax);
    TRACE_INFO(" _MCSR   : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, 18, &value, retryMax);
    TRACE_INFO(" _SM     : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, 26, &value, retryMax);
    TRACE_INFO(" _SECR   : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, 27, &value, retryMax);
    TRACE_INFO(" _CSIR   : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, 29, &value, retryMax);
    TRACE_INFO(" _ISR    : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, 30, &value, retryMax);
    TRACE_INFO(" _IMR    : 0x%X\n\r", value);
    EMAC_ReadPhy(phyAddress, 31, &value, retryMax);
    TRACE_INFO(" _PSCSR  : 0x%X\n\r", value);
}

void DM9161_ShowConnectStatus( Dm9161 * pDm )
{
    unsigned int bmsr_value, aner_value;

    EMAC_ReadPhy( pDm->phyAddress, DM9161_BMSR, &bmsr_value, pDm->retryMax );
    EMAC_ReadPhy( pDm->phyAddress, DM9161_ANER, &aner_value, pDm->retryMax );

    if( bmsr_value & DM9161_LINK_STATUS )
    {
        TRACE_INFO( "Link ok\n\r" );
    }
    else
    {
        if( aner_value & DM9161_LP_AN_ABLE )
        {
            TRACE_ERROR( "Link err, connected\n\r" );
        }
        else
        {
            TRACE_ERROR( "Link err, not connected\n\r" );
        }
    }
}

//-----------------------------------------------------------------------------
/// Find a valid PHY Address ( from 0 to 31 ).
/// Check BMSR register ( not 0 nor 0xFFFF )
/// Return 0xFF when no valid PHY Address found.
/// \param pDm          Pointer to the Dm9161 instance
//-----------------------------------------------------------------------------
static unsigned char DM9161_FindValidPhy(Dm9161 * pDm)
{
    unsigned int  retryMax;
    unsigned int  value=0;
    unsigned char rc;
    unsigned char phyAddress;
    unsigned char cnt;


    phyAddress = pDm->phyAddress;
    retryMax = pDm->retryMax;

    // Check current phyAddress
    rc = phyAddress;
    if( EMAC_ReadPhy(phyAddress, DM9161_PHYID1, &value, retryMax) == 0 ) {
        TRACE_ERROR("DM9161 PROBLEM\n\r");
    }
    TRACE_DEBUG("_PHYID1  : 0x%X, addr: %d\n\r", value, phyAddress);

    // Find another one
    if (value != DM9161_OUI_MSB) {

        rc = 0xFF;
        for(cnt = 0; cnt < 32; cnt ++) {

            phyAddress = (phyAddress + 1) & 0x1F;
            value = 0xFFFF;
            if( EMAC_ReadPhy(phyAddress, DM9161_PHYID1, &value, retryMax) == 0 ) {
                TRACE_ERROR("DM9161 PROBLEM\n\r");
            }
            TRACE_DEBUG("_PHYID1  : 0x%X, addr: %d\n\r", value, phyAddress);
            if ( (value != 0xFFFF) && (value) ){//if (value == DM9161_OUI_MSB) {

                rc = phyAddress;
                if( value == 0x0007 )
                    gDriverEmacPhyType = PHY_TYPE_LAN8720A;
                break;
            }
        }
    }
    if (rc != 0xFF) {

       // TRACE_INFO("** Valid PHY Found: %d\n\r", rc);
        EMAC_ReadPhy(phyAddress, DM9161_DSCSR, &value, retryMax);
      //  TRACE_DEBUG("_DSCSR  : 0x%X, addr: %d\n\r", value, phyAddress);

    }
    return rc;
}


//-----------------------------------------------------------------------------
//         Exported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Setup the maximum timeout count of the driver.
/// \param pDm   Pointer to the Dm9161 instance
/// \param toMax Timeout maxmum count.
//-----------------------------------------------------------------------------
void DM9161_SetupTimeout(Dm9161 *pDm, unsigned int toMax)
{

    pDm->retryMax = toMax;
}

//-----------------------------------------------------------------------------
/// Initialize the Dm9161 instance
/// \param pDm          Pointer to the Dm9161 instance
/// \param pEmac        Pointer to the Emac instance for the Dm9161
/// \param phyAddress   The PHY address used to access the PHY
///                     ( pre-defined by pin status on PHY reset )
//-----------------------------------------------------------------------------
void DM9161_Init(Dm9161 *pDm, unsigned char phyAddress)
{

    pDm->phyAddress = phyAddress;

    // Initialize timeout by default
    pDm->retryMax = DM9161_RETRY_MAX;
}


//-----------------------------------------------------------------------------
/// Issue a SW reset to reset all registers of the PHY
/// Return 1 if successfully, 0 if timeout.
/// \param pDm   Pointer to the Dm9161 instance
//-----------------------------------------------------------------------------
static unsigned char DM9161_ResetPhy(Dm9161 *pDm)
{
    unsigned int retryMax;
    unsigned int bmcr = DM9161_RESET;
    unsigned char phyAddress;
    unsigned int timeout = 10;
    unsigned char ret = 1;


    phyAddress = pDm->phyAddress;
    retryMax = pDm->retryMax;

    bmcr = DM9161_RESET;
    EMAC_WritePhy(phyAddress, DM9161_BMCR, bmcr, retryMax);

    do {
        EMAC_ReadPhy(phyAddress, DM9161_BMCR, &bmcr, retryMax);
        timeout--;
    } while ((bmcr & DM9161_RESET) && timeout);


    if (!timeout) {
        ret = 0;
    }

    return( ret );
}

//-----------------------------------------------------------------------------
/// Do a HW initialize to the PHY ( via RSTC ) and setup clocks & PIOs
/// This should be called only once to initialize the PHY pre-settings.
/// The PHY address is reset status of CRS,RXD[3:0] (the emacPins' pullups).
/// The COL pin is used to select MII mode on reset (pulled up for Reduced MII)
/// The RXDV pin is used to select test mode on reset (pulled up for test mode)
/// The above pins should be predefined for corresponding settings in resetPins
/// The EMAC peripheral pins are configured after the reset done.
/// Return 1 if RESET OK, 0 if timeout.
/// \param pDm         Pointer to the Dm9161 instance
/// \param mck         Main clock setting to initialize clock
/// \param resetPins   Pointer to list of PIOs to configure before HW RESET
///                       (for PHY power on reset configuration latch)
/// \param nbResetPins Number of PIO items that should be configured
/// \param emacPins    Pointer to list of PIOs for the EMAC interface
/// \param nbEmacPins  Number of PIO items that should be configured
//-----------------------------------------------------------------------------

unsigned char DM9161_InitPhy(Dm9161       *pDm)
{
    unsigned char rc = 1;
    unsigned char phy;


    // Configure EMAC runtime pins
    if (rc) {

        // Check PHY Address
        phy = DM9161_FindValidPhy(pDm);
        if (phy == 0xFF) {

            TRACE_ERROR("PHY Access fail\n\r");
            return 0;
        }
        //printf( "PHY ADDR:%d\n\r", phy );
        if(phy != pDm->phyAddress) {

            pDm->phyAddress = phy;

            //DM9161_ResetPhy(pDm);

        }
        // RESET DM9161
        unsigned char ret = DM9161_ResetPhy( pDm );
        if( ret == 0 )
        {
            TRACE_ERROR( "ResetPhy error!!!\n\r" );
        }
        //else
        //    printf( "ResetPhy ok!\n" );

    }
    else {

        TRACE_ERROR("PHY Reset Timeout\n\r");
    }

    return rc;
}

//-----------------------------------------------------------------------------
/// Issue a Auto Negotiation of the PHY
/// Return 1 if successfully, 0 if timeout.
/// \param pDm   Pointer to the Dm9161 instance
//-----------------------------------------------------------------------------
unsigned char DM9161_AutoNegotiate(Dm9161 *pDm)
{
    unsigned int retryMax;
    unsigned int value;
    unsigned int phyAnar;
    unsigned int phyAnalpar;
    unsigned int retryCount= 0;
    unsigned char phyAddress;
    unsigned char rc = 1;

    phyAddress = pDm->phyAddress;
    retryMax = pDm->retryMax;

    if (!EMAC_ReadPhy(phyAddress, DM9161_PHYID1, &value, retryMax)) {
        TRACE_ERROR("Pb EMAC_ReadPhy Id1\n\r");
        rc = 0;
        goto AutoNegotiateExit;
    }
    TRACE_DEBUG("ReadPhy Id1 0x%X, addresse: %d\n\r", value, phyAddress);
    if (!EMAC_ReadPhy(phyAddress, DM9161_PHYID2, &phyAnar, retryMax)) {
        TRACE_ERROR("Pb EMAC_ReadPhy Id2\n\r");
        rc = 0;
        goto AutoNegotiateExit;
    }
    TRACE_DEBUG("ReadPhy Id2 0x%X\n\r", phyAnar);

    if( ( value != 0xFFFF )
     && ( ((phyAnar>>10)&DM9161_LSB_MASK) != DM9161_LSB_MASK ) ) {

        TRACE_DEBUG("Vendor Number Model = 0x%X\n\r", ((phyAnar>>4)&0x3F));
        TRACE_DEBUG("Model Revision Number = 0x%X\n\r", (phyAnar&0xF));
    }
    else {
        TRACE_ERROR("Problem OUI value\n\r");
    }

    // Setup control register
    rc  = EMAC_ReadPhy(phyAddress, DM9161_BMCR, &value, retryMax);
    if (rc == 0) {

        goto AutoNegotiateExit;
    }

    value &= ~DM9161_AUTONEG;   // Remove autonegotiation enable
    value &= ~(DM9161_LOOPBACK|DM9161_POWER_DOWN);
    value |=  DM9161_ISOLATE;   // Electrically isolate PHY
    rc = EMAC_WritePhy(phyAddress, DM9161_BMCR, value, retryMax);
    if (rc == 0) {

        goto AutoNegotiateExit;
    }

    // Set the Auto_negotiation Advertisement Register
    // MII advertising for Next page
    // 100BaseTxFD and HD, 10BaseTFD and HD, IEEE 802.3
    phyAnar = DM9161_TX_FDX | DM9161_TX_HDX |
              DM9161_10_FDX | DM9161_10_HDX | DM9161_AN_IEEE_802_3;
    if( gDriverEmacPhyType == PHY_TYPE_DM9161 )
        phyAnar |= DM9161_NP;
    rc = EMAC_WritePhy(phyAddress, DM9161_ANAR, phyAnar, retryMax);
    if (rc == 0) {

        goto AutoNegotiateExit;
    }

    // Read & modify control register
    rc  = EMAC_ReadPhy(phyAddress, DM9161_BMCR, &value, retryMax);
    if (rc == 0) {

        goto AutoNegotiateExit;
    }

    value |= DM9161_SPEED_SELECT | DM9161_AUTONEG | DM9161_DUPLEX_MODE;
    rc = EMAC_WritePhy(phyAddress, DM9161_BMCR, value, retryMax);
    if (rc == 0) {

        goto AutoNegotiateExit;
    }

    // Restart Auto_negotiation
    value |=  DM9161_RESTART_AUTONEG;
    value &= ~DM9161_ISOLATE;
    rc = EMAC_WritePhy(phyAddress, DM9161_BMCR, value, retryMax);
    if (rc == 0) {

        goto AutoNegotiateExit;
    }
    TRACE_DEBUG(" _BMCR: 0x%X\n\r", value);

    // Check AutoNegotiate complete
    while (1) {

        rc  = EMAC_ReadPhy(phyAddress, DM9161_BMSR, &value, retryMax);
        if (rc == 0) {

            TRACE_ERROR("rc==0\n\r");
            goto AutoNegotiateExit;
        }
        // Done successfully
        if (value & DM9161_AUTONEG_COMP) {

          //  TRACE_INFO("AutoNegotiate complete\n\r");
            break;
        }
        // Timeout check
        if (retryMax) {

            if (++ retryCount >=  (DM9161_RETRY_MAX / 25 * gDriverEmacTimeout) ) {

                DM9161_DumpRegisters(pDm);
                TRACE_ERROR( "AutoNeg TimeOut\n\r" );
                return (0);
            }
        }
    }

    // Get the AutoNeg Link partner base page
    rc  = EMAC_ReadPhy(phyAddress, DM9161_ANLPAR, &phyAnalpar, retryMax);
    if (rc == 0) {

        goto AutoNegotiateExit;
    }

    // Setup the EMAC link speed
    /*if ((phyAnar & phyAnalpar) & DM9161_TX_FDX) {

        // set MII for 100BaseTX and Full Duplex
        EMAC_SetLinkSpeed(1, 1);
    }
    else if ((phyAnar & phyAnalpar) & DM9161_10_FDX) {

        // set MII for 10BaseT and Full Duplex
        EMAC_SetLinkSpeed(0, 1);
    }
    else if ((phyAnar & phyAnalpar) & DM9161_TX_HDX) {

        // set MII for 100BaseTX and half Duplex
        EMAC_SetLinkSpeed(1, 0);
    }
    else if ((phyAnar & phyAnalpar) & DM9161_10_HDX) {

        // set MII for 10BaseT and half Duplex
        EMAC_SetLinkSpeed(0, 0);
    }*/


AutoNegotiateExit:
    return rc;
}

//-----------------------------------------------------------------------------
/// Get the Link & speed settings, and automatically setup the EMAC with the
/// settings.
/// Return 1 if link found, 0 if no ethernet link.
/// \param pDm          Pointer to the Dm9161 instance
/// \param applySetting Apply the settings to EMAC interface
//-----------------------------------------------------------------------------
unsigned char DM9161_GetLinkSpeed(Dm9161 *pDm, unsigned char applySetting)
{
    unsigned int retryMax;
    unsigned int stat1;
    unsigned int stat2;
    unsigned char phyAddress;
    unsigned char rc = 1;
    phyAddress = pDm->phyAddress;
    retryMax = pDm->retryMax;
    rc  = EMAC_ReadPhy(phyAddress, DM9161_BMSR, &stat1, retryMax);
    if (rc == 0) {

        goto GetLinkSpeedExit;
    }

    if ((stat1 & DM9161_LINK_STATUS) == 0) {

        //TRACE_ERROR("Pb: LinkStat: 0x%x\n\r", stat1);

        rc = 0;
        goto GetLinkSpeedExit;
    }

    if (applySetting == 0) {

        TRACE_ERROR("Pb: applySetting: 0x%x\n\r", applySetting);
        goto GetLinkSpeedExit;
    }
    rc  = EMAC_ReadPhy(phyAddress, 31, &stat2, retryMax);   // PHY Special Control/Status Register
    if (rc == 0) {
        TRACE_ERROR("Pb: rc: 0x%x\n\r", rc);
        goto GetLinkSpeedExit;
    }

    // Start the EMAC transfers
    TRACE_DEBUG("DM9161_GetLinkSpeed passed\n\r");

GetLinkSpeedExit:
    return rc;
}

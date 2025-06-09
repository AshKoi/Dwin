/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)           */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user's obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* FSEU does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law FSEU disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, FSEU's liability  */
/* is restricted to intentional misconduct and gross negligence.        */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.5)                                                               */
/************************************************************************/
/** \file UsbSofTimeout.c
 **
 ** USB Registers File
 **
 ** History:
 **   - 2010-09-24  1.0  MSc  USB Sof Timeout Feature
 **   - 2011-03-30  1.0  MSc  Some Corrections
 *************************************************************************/

#include "UsbSofTimeout.h"
#include "dbg.h"

void (** pUsbSOFHandler)(uint16_t u16FrameNumber) = 0;
stc_timeout_handler_t stcUsbTimeOutHandler[10] = 
               { 
                    {0,0},
                    {0,0},
                    {0,0},
                    {0,0},
                    {0,0},
                    {0,0},
                    {0,0},
                    {0,0},
                    {0,0},
                    {0,0}
               };

void Usb_SetSOFTimeOutHandler(void (** pSofHandler)(uint16_t u16FrameNumber))
{
    pUsbSOFHandler = pSofHandler;
}
/**
 ******************************************************************************
 ** SOF controlled Timeout Scheduler
 ** 
 ** \param ticks SOF counter
 ** 
 *****************************************************************************/

void Usb_TimeoutHandler(uint16_t ticks)
{
    uint8_t i;
    boolean_t bNothingToBeDone = TRUE;
    ticks = ticks;
    for(i = 0;i<10;i++)
    {
        if (stcUsbTimeOutHandler[i].Handler != 0)
        {
            bNothingToBeDone = FALSE;
            if (stcUsbTimeOutHandler[i].u16TimeOut == 0)
            {
                stcUsbTimeOutHandler[i].Handler();
                stcUsbTimeOutHandler[i].Handler = 0;
                return;
            }
            else
            {
                stcUsbTimeOutHandler[i].u16TimeOut--;
            }
        }
    }
    if (bNothingToBeDone == TRUE)
    {
         *pUsbSOFHandler = 0;
         dbg("Not Timouthandlers, removing SOF Handler\n");
    }
}

/**
 ******************************************************************************
 ** Add a timeout handler
 **
 ** 
 ** \param Handler handler executed after timeout ( void handler(void) )
 ** 
 ** \param u16TimeOut timeout in ms
 **
 ** \return TRUE if command was successful
 **
 *****************************************************************************/
 
boolean_t Usb_AddTimeOut(void (* Handler)(void), uint16_t u16TimeOut)
{
    uint8_t i;
    boolean_t bAdded = FALSE;
    for(i = 0;i<10;i++)
    {
        if (stcUsbTimeOutHandler[i].Handler == 0)
        {
            stcUsbTimeOutHandler[i].Handler = Handler;
            stcUsbTimeOutHandler[i].u16TimeOut = u16TimeOut;
            bAdded = TRUE;
            break;
        }
    }
    *pUsbSOFHandler = Usb_TimeoutHandler;
    return bAdded;
}

/**
 ******************************************************************************
 ** Remove a timeout handler
 **
 ** 
 ** \param Handler handler executed after timeout ( void handler(void) )
 ** 
 ** \return none
 **
 *****************************************************************************/
 
void Usb_RemoveTimeOut(void (* Handler)(void))
{
    uint8_t i;
    for(i = 0;i<10;i++)
    {
        if (stcUsbTimeOutHandler[i].Handler == Handler)
        {
            stcUsbTimeOutHandler[i].Handler = 0;
            stcUsbTimeOutHandler[i].u16TimeOut = 0;
            break;
        }
    }
}

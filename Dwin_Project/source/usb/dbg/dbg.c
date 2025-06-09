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
/** \file dbg.c
 **
 ** - debug routines for use with uart module  
 **
 ** History:
 **   - 2010-03-30    1.0  MSc  First version  (works with 16FX)
 *****************************************************************************/

#include "base_type.h"
#include "dbg.h"
#if (debug_enabled)
void bufferprintout(uint8_t* buffer, uint32_t size) {
  uint32_t i;
  dbg("Buffer (len=");
  dbghex(size);
  dbg("):\n");
  for(i=0;i<size;i++) {
    puthex((uint32_t)*buffer++,2);
    dbg(" ");
  }
  dbg("\n\n");
}
void _dbghex(uint32_t n)
{
    uint8_t i;
    uint32_t tmp = n;
    for(i=2;i<8;i += 2)
    {
        tmp = tmp >> 8;
        if (tmp == 0)
        {
            puthex(n,i);
            return;
        }
    }
    puthex(n,i);
}

#endif

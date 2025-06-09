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
/** \file dbg.h
 **
 ** - debug routines for use with uart module  
 **
 ** History:
 **   - 2010-03-30    1.2  MSc  First version  (works with 16FX)
 *****************************************************************************/

#ifndef _DBG_H
    #define _DBG_H
    #define puts0(x)  puts(x)
  
    #define debug_enabled    0
    #define use_printbuffer  0
  
  
    #if (debug_enabled)
      #define DEBUGLEVEL 15
      #define DBGOUT(dbglevel,x)   if (dbglevel <= DEBUGLEVEL) { x; }
      #define dbg(x)    puts(x)
      #define dbgsect(x,y)    dbg(x);dbg(y)
      #define dbgval(x,y)  dbg(x);dbghex(y);dbg("\n")
      #define dbgsectval(x,y,z)    dbg(x);dbgval(y,z)
      #define dbghex(x)    _dbghex((uint32_t)x)
      
      void _dbghex(uint32_t n);
      #if (use_printbuffer)
          void bufferprintout(uint8_t* buffer, uint32_t size);
          #define printBuffer(x,y)    bufferprintout(x,y);
      #else
          #define printBuffer(x,y) ;
      #endif
    #else
        #define dbg(x)   
        #define dbghex(x) 
        #define printBuffer(x,y)
        #define dbgval(x,y)
        #define DEBUGLEVEL
        #define DBGOUT(dbglevel,x)
    #endif
#endif

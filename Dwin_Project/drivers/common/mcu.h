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
/**
 ******************************************************************************
 ** \file   mcu.h
 **
 ** Header File for device dependent includes
 **
 ** History:
 ** 2011-07-07 V1.00 YI first version
 **
 ******************************************************************************/
//#include "board.h"  //to coordinate different target board
#include "mb9bf218s.h"

/**
 ******************************************************************************
 ** \brief MCU header file include
 **
 ******************************************************************************/ 

#if defined (EVB_FM3_ETH)

    #ifndef _MB9BF218S_H_
        #include "mb9bf218s.h"
    #endif

#elif defined (CUST_BOARD)

    #ifndef _MB9BF618T_H_
        #include "mb9bf618t.h"
    #endif

#endif   

/**
 ******************************************************************************
 ** \brief MCU system start-up header file include
 **
 ******************************************************************************/ 
#ifndef _SYSTEM_MB9BF21X_H_
  #include "system_mb9bf21x.h"
#endif



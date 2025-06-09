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
/** \file srecordparser.h
 **
 ** API to parse a motorola S-Record file, like *.mhx or *.srec
 **
 ** History:
 **   - 2012-03-16    1.0  MSc  First version
 *****************************************************************************/

#ifndef __SRECORDPARSER_H__
#define __SRECORDPARSER_H__

#include "base_type.h"

uint8_t SRecordParser_HexToByte(char_t* pcData);
void SRecordParser_OpenFile(char_t* FileName);
uint32_t SRecordParser_ReadLine(uint8_t* pu8Data, uint8_t* pu8DataLength);
void SRecordParser_CloseFile(char_t* FileName);

#define SRECORDPARSER_ENDOFFILE 0xFFFFFFFF
#define SRECORDPARSER_CRCERROR  0xFFFFFFFA

#endif

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
/** \file srecordparser.c
 **
 ** API to parse a motorola S-Record file, like *.mhx or *.srec
 **
 ** History:
 **   - 2012-03-16    1.0  MSc  First version
 *****************************************************************************/

#include "srecordparser.h"
#include "ff.h"
#include "global.h"
static FATFS fatfsHandle;
static FIL filHandle;

/**
 ******************************************************************************
 ** \brief  Covert string ex format to uint8_t (byte)
 **
 ** \param  pointer to string
 ** \return byte value
 ******************************************************************************/
uint8_t SRecordParser_HexToByte(char_t* pcData)
{
    uint8_t u8Temp = 0;
    if ((pcData[0] >= '0') & (pcData[0] <= '9'))
    {
        u8Temp += ((uint8_t)(pcData[0] - '0')) << 4;
    }
    if ((pcData[0] >= 'A') & (pcData[0] <= 'F'))
    {
        u8Temp += ((uint8_t)(pcData[0] - 'A' + 10)) << 4;
    }
    
    if ((pcData[1] >= '0') & (pcData[1] <= '9'))
    {
        u8Temp += ((uint8_t)(pcData[1] - '0'));
    }
    if ((pcData[1] >= 'A') & (pcData[1] <= 'F'))
    {
        u8Temp += ((uint8_t)(pcData[1] - 'A' + 10));
    }
    return u8Temp;
}

/**
 ******************************************************************************
 ** \brief  Open file
 **
 ** \param  FileName filename
 ** \return none
 ******************************************************************************/
void SRecordParser_OpenFile(char_t* FileName)
{
    f_mount(0,&fatfsHandle); // mounting workspace
    f_open(&filHandle,FileName,FA_OPEN_EXISTING | FA_READ |FA_WRITE); // open file for read
}

/**
 ******************************************************************************
 ** \brief  Read line of S-Record file
 **
 ** \param  pu8Data        buffer to write in converted data
 ** \param  pu8DataLength  pointer in which the datalength will be written
 ** \return start address, in case of error / end of file, *pu8DataLength = 0,
 **         return value contains the errorcode:
 **         SRECORDPARSER_CRCERROR in case of CRC error
 **         SRECORDPARSER_ENDOFFILE in case of end of file
 ******************************************************************************/
uint32_t SRecordParser_ReadLine(uint8_t* pu8Data, uint8_t* pu8DataLength)
{
    char_t pcTemp[2];
    char_t pWTemp[1500];
    UINT uintBytesRead, uintBytesWrite;
    uint8_t u8State = 0;
    uint8_t u8SRecordType = 0;
    uint8_t u8ByteCount = 0;
    uint32_t u32StartAddress = 0;
    uint8_t u8CRC = 0;
    *pu8DataLength = 0;
    while(1)
    {
        pcTemp[0] = 0;
        pcTemp[1] = 0;
        memset( pWTemp, 0x55, 1023 );
        f_write(&filHandle,pWTemp,1024,&uintBytesWrite);
        f_read(&filHandle,pcTemp,2,&uintBytesRead);
        /*puts("Data ");
        putch(pcTemp[0]);
        putch(pcTemp[1]);
        puts("\n");*/
        if (uintBytesRead == 0)
        {
            if (*pu8DataLength == 0)
            {
                u32StartAddress = SRECORDPARSER_ENDOFFILE;
            }
            break;
        }
        if ((pcTemp[0] == '\r') && (pcTemp[1] == '\n'))
        {
            break;
        }
        if ((u8State > 0) && (u8State < 7))
        {
          u8CRC += SRecordParser_HexToByte(pcTemp);
        }
        switch(u8State)
        {
            case 0: //state 0, wait for S-Record type
                if (pcTemp[0] == 'S')
                {
                   u8SRecordType = (uint8_t)(pcTemp[1] - '0');
                   u8State++;
                }
                if ((u8SRecordType < 1) || (u8SRecordType > 3))
                {
                  u8SRecordType = 0;
                  u8State = 0;
                } else
                {
                  /*puts("New S-Record: ");
                  putch(u8SRecordType + '0');
                  puts("\n");*/
                }
                break;
            case 1: //state 1, read length of data
              u8ByteCount = SRecordParser_HexToByte(pcTemp);
              u8ByteCount -= (u8SRecordType + 2); // remove checksum byte and address bytes
              *pu8DataLength = u8ByteCount;
              u8State++;
              break;
            case 2: //state 2..5 read address bytes
              u32StartAddress = 0;
            case 3:
            case 4:
            case 5:
              
              u32StartAddress += SRecordParser_HexToByte(pcTemp)  << (8 * (u8SRecordType - (u8State - 2)));
              if ((u8SRecordType - (u8State - 2)) == 0)
              {
                  u8State = 5;
              }
              u8State++;
              break;
            case 6: //state 6, read data bytes
              u8ByteCount--;
              if (u8ByteCount == 0)
              {
                  u8State++;
              }
              *pu8Data = SRecordParser_HexToByte(pcTemp);
              pu8Data++;
              break;
            case 7: //state 7, read CRC
              if ((0xFF ^ SRecordParser_HexToByte(pcTemp)) != u8CRC)
              {
                //puts("CRC Error");
                u32StartAddress = SRECORDPARSER_CRCERROR;
                *pu8DataLength = 0;
                f_sync(&filHandle);
                return u32StartAddress;
              }
              else
              {
                //puts("CRC OK");
              }
              break;
            default:
              break;
        }
    }
    /*puts("Address: 0x");
    puthex(u32StartAddress,8);
    puts("\n");*/
    f_sync(&filHandle);
    return u32StartAddress;
    
}

/**
 ******************************************************************************
 ** \brief  Close file
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SRecordParser_CloseFile(char_t* FileName)
{
    f_close(&filHandle);
}


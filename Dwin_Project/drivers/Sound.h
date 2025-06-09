/*!
 ******************************************************************************
 **
 ** \file io_fm3.c
 **
 ** \brief IO driver
 **
 ** 
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-18
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#ifndef _Sound_HEADER_
#define _Sound_HEADER_
 
#include "global.h"
#include "config.h"


#define SOUND_FRAME_BUF_SIZE	512
#define SOUND_FRAME	0xFD
#define HCWB	        01
#define BMGS            00


//»º³åÇø½á¹¹
typedef struct _tag_Sound_BUF
{
       	INT8U  FRAME_H;
        INT16U  nLen;
        INT8U  nCtrl;
        INT8U  nFormat;
        INT8U  *pBuf;	
}SOUND_BUF, *pSound_BUF;

//SOUND_BUF g_SoundBuf[];
void SoundPlay(void);
void SoundInit( INT8U port );


#endif


















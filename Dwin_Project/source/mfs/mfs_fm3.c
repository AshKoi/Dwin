/*!
 ******************************************************************************
 **
 ** \file mfs_fm3.c
 **
 ** \brief MFS ISR Handler
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-26
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
#include "mfs_fm3.h"
/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/
MFS_CallbackT g_pMFSIRQCallback[16] = 
{ 
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};
/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief MFS0 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS0RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh0RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh0RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS0 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS0TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh0TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh0TX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS1 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS1RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh1RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh1RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS1 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS1TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh1TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh1TX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS2 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS2RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh2RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh2RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS2 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS2TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh2TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh2TX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS3 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS3RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh3RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh3RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS3 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS3TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh3TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh3TX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS4 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS4RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh4RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh4RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS4 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS4TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh4TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh4TX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS5 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS5RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh5RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh5RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS5 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS5TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh5TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh5TX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS6 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS6RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh6RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh6RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS6 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS6TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh6TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh6TX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS7 RX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS7RX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh7RX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh7RX]();
    }
}
/*!
 ******************************************************************************
 ** \brief MFS7 TX Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS7TX_IRQHandler(void)
{
    if(g_pMFSIRQCallback[MFS_MFS_IntCh7TX] != NULL)
    {
        g_pMFSIRQCallback[MFS_MFS_IntCh7TX]();
    }
}
/*****************************************************************************/

/* END OF FILE */

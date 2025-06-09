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
#include "Io_fm3.h"

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#define GPIO_PFR_BASE_ADDR      (volatile uint32_t*)(&FM3_GPIO->PFR0)
#define GPIO_DDR_BASE_ADDR      (volatile uint32_t*)(&FM3_GPIO->DDR0)
#define GPIO_PDOR_BASE_ADDR     (volatile uint32_t*)(&FM3_GPIO->PDOR0)
#define GPIO_PDIR_BASE_ADDR     (volatile uint32_t*)(&FM3_GPIO->PDIR0)
#define GPIO_PCR_BASE_ADDR      (volatile uint32_t*)(&FM3_GPIO->PCR0)
/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/




/*!
 ******************************************************************************
 ** \brief Configure IO port  
 **
 ** \param ioport IO port number 
 ** \arg GPIO_PORT0
 ** \arg GPIO_PORT1
 ** \arg GPIO_PORT2
 ** \arg GPIO_PORT3
 ** \arg GPIO_PORT4
 ** \arg GPIO_PORT5
 ** \arg GPIO_PORT6
 ** \arg GPIO_PORT7
 ** \arg GPIO_PORT8
 **       
 ** \param iopin  IO pin number 
 ** \arg GPIO_ALL_PIN
 ** \arg GPIO_PIN_0
 ** \arg GPIO_PIN_1
 ** \arg GPIO_PIN_2
 ** \arg GPIO_PIN_3
 ** \arg GPIO_PIN_4
 ** \arg GPIO_PIN_5
 ** \arg GPIO_PIN_6
 ** \arg GPIO_PIN_7
 ** \arg GPIO_PIN_8
 ** \arg GPIO_PIN_9
 ** \arg GPIO_PIN_A
 ** \arg GPIO_PIN_B
 ** \arg GPIO_PIN_C
 ** \arg GPIO_PIN_D
 ** \arg GPIO_PIN_E
 ** \arg GPIO_PIN_F
 **
 ** \param  iomode IO mode 
 ** \arg IO_GPIO_MODE
 ** \arg IO_PERIPHERAL_MODE
 ** \retval None
 **
 ******************************************************************************
 */
void IO_ConfigPort(uint8_t ioport, uint32_t iopin, uint8_t iomode)
{
    /* Check the parameter */
    ASSERT (IS_IO_MODE(iomode));
    ASSERT (IS_GPIO_PORT(ioport));
    ASSERT (IS_GPIO_PIN(iopin));
	
    if(iomode == IO_PERIPHERAL_MODE)
    {
        switch(ioport)
    	{
    	    case IO_PORT0: 
    	        FM3_GPIO->PFR0 |= iopin;  
    	        break;
    	    case IO_PORT1:
    	        FM3_GPIO->PFR1 |= iopin; 
    	        FM3_GPIO->ADE &= ~iopin;
    	    	break;
    	    case IO_PORT2:
    	        FM3_GPIO->PFR2 |= iopin; 
    	        break;
    	    case IO_PORT3:
    	        FM3_GPIO->PFR3 |= iopin; 
    	        break;
    	    case IO_PORT4:
    	        FM3_GPIO->PFR4 |= iopin; 
    	        break;
    	    case IO_PORT5:
    	        FM3_GPIO->PFR5 |= iopin; 
    	        break;
    	    case IO_PORT6:
    	        FM3_GPIO->PFR6 |= iopin; 
    	        break;
    	    case IO_PORT7:
    	        FM3_GPIO->PFR7 |= iopin; 
    	        break;
    	    case IO_PORT8:
    	        FM3_GPIO->PFR8 |= iopin; 
    	        break;
    	    case IO_PORT9:
    	        FM3_GPIO->PFR9 |= iopin; 
    	        break;
    	    case IO_PORTA:
    	        FM3_GPIO->PFRA |= iopin; 
    	        break;
    	    case IO_PORTB:
    	        FM3_GPIO->PFRB |= iopin; 
    	        break;
    	    case IO_PORTC:
    	        FM3_GPIO->PFRC |= iopin; 
    	        break;
    	    case IO_PORTD:
    	        FM3_GPIO->PFRD |= iopin; 
    	        break;
    	    case IO_PORTE:
    	        FM3_GPIO->PFRE |= iopin; 
    	        break;
    	    case IO_PORTF:
    	        FM3_GPIO->PFRF |= iopin; 
    	        break;
    	    default: 
    	        break;
    	}

    }
    else
    {
    	switch(ioport)
    	{
    	    case IO_PORT0: 
    	        FM3_GPIO->PFR0 &= ~ iopin;  
    	        break;
    	    case IO_PORT1:
    	        FM3_GPIO->PFR1 &= ~ iopin; 
    	        FM3_GPIO->ADE &= ~iopin;
    	    	break;
    	    case IO_PORT2:
    	        FM3_GPIO->PFR2 &= ~ iopin; 
    	        break;
    	    case IO_PORT3:
    	        FM3_GPIO->PFR3 &= ~ iopin; 
    	        break;
    	    case IO_PORT4:
    	        FM3_GPIO->PFR4 &= ~ iopin; 
    	        break;
    	    case IO_PORT5:
    	        FM3_GPIO->PFR5 &= ~ iopin; 
    	        break;
    	    case IO_PORT6:
    	        FM3_GPIO->PFR6 &= ~ iopin; 
    	        break;
    	    case IO_PORT7:
    	        FM3_GPIO->PFR7 &= ~ iopin; 
    	        break;
    	    case IO_PORT8:
    	        FM3_GPIO->PFR8 &= ~ iopin; 
    	        break;
    	    case IO_PORT9:
    	        FM3_GPIO->PFR9 &= ~ iopin; 
    	        break;
    	    case IO_PORTA:
    	        FM3_GPIO->PFRA &= ~ iopin; 
    	        break;
    	    case IO_PORTB:
    	        FM3_GPIO->PFRB &= ~ iopin; 
    	        break;
    	    case IO_PORTC:
    	        FM3_GPIO->PFRC &= ~ iopin; 
    	        break;
    	    case IO_PORTD:
    	        FM3_GPIO->PFRD &= ~ iopin; 
    	        break;
    	    case IO_PORTE:
    	        FM3_GPIO->PFRE &= ~ iopin; 
    	        break;
    	    case IO_PORTF:
    	        FM3_GPIO->PFRF &= ~ iopin; 
    	        break;

    	    default: 
    	        break;
    	}

    }
}



/*!
 ******************************************************************************
 ** \brief Set GPIO as input  
 **
 ** \param ioport IO port number 
 ** \arg GPIO_PORT0
 ** \arg GPIO_PORT1
 ** \arg GPIO_PORT2
 ** \arg GPIO_PORT3
 ** \arg GPIO_PORT4
 ** \arg GPIO_PORT5
 ** \arg GPIO_PORT6
 ** \arg GPIO_PORT7
 ** \arg GPIO_PORT8
 **       
 ** \param iopin  IO pin number 
 ** \arg GPIO_ALL_PIN
 ** \arg GPIO_PIN_0
 ** \arg GPIO_PIN_1
 ** \arg GPIO_PIN_2
 ** \arg GPIO_PIN_3
 ** \arg GPIO_PIN_4
 ** \arg GPIO_PIN_5
 ** \arg GPIO_PIN_6
 ** \arg GPIO_PIN_7
 ** \arg GPIO_PIN_8
 ** \arg GPIO_PIN_9
 ** \arg GPIO_PIN_A
 ** \arg GPIO_PIN_B
 ** \arg GPIO_PIN_C
 ** \arg GPIO_PIN_D
 ** \arg GPIO_PIN_E
 ** \arg GPIO_PIN_F
 **
 ** \retval None
 **
 ******************************************************************************
 */
void IO_GPIOSetInput(uint8_t ioport, uint32_t iopin)
{
    /* Check the parameter */
    ASSERT (IS_GPIO_PORT(ioport));
    ASSERT (IS_GPIO_PIN(iopin));
	
    IO_ConfigPort(ioport, iopin, IO_GPIO_MODE);
    switch(ioport)
    {
        case GPIO_PORT0: 
    	     FM3_GPIO->DDR0 &= ~ iopin;  
    	     break;
    	case GPIO_PORT1:
    	     FM3_GPIO->DDR1 &= ~ iopin; 
    	     break;
    	case GPIO_PORT2:
    	     FM3_GPIO->DDR2 &= ~ iopin; 
    	     break;
    	case GPIO_PORT3:
    	     FM3_GPIO->DDR3 &= ~ iopin; 
    	     break;
    	case GPIO_PORT4:
    	     FM3_GPIO->DDR4 &= ~ iopin; 
    	     break;
    	case GPIO_PORT5:
    	     FM3_GPIO->DDR5 &= ~ iopin; 
    	     break;
    	case GPIO_PORT6:
    	     FM3_GPIO->DDR6 &= ~ iopin; 
    	     break;
    	case GPIO_PORT7:
    	     FM3_GPIO->DDR7 &= ~ iopin; 
    	     break;
    	case GPIO_PORT8:
    	     FM3_GPIO->DDR8 &= ~ iopin; 
    	     break;

    	default: 
    	     break;
    	}
    return;	
	
}


/*!
 ******************************************************************************
 ** \brief Set GPIO as output 
 **
 ** \param ioport IO port number 
 ** \arg GPIO_PORT0
 ** \arg GPIO_PORT1
 ** \arg GPIO_PORT2
 ** \arg GPIO_PORT3
 ** \arg GPIO_PORT4
 ** \arg GPIO_PORT5
 ** \arg GPIO_PORT6
 ** \arg GPIO_PORT7
 ** \arg GPIO_PORT8
 **       
 ** \param iopin  IO pin number 
 ** \arg GPIO_ALL_PIN
 ** \arg GPIO_PIN_0
 ** \arg GPIO_PIN_1
 ** \arg GPIO_PIN_2
 ** \arg GPIO_PIN_3
 ** \arg GPIO_PIN_4
 ** \arg GPIO_PIN_5
 ** \arg GPIO_PIN_6
 ** \arg GPIO_PIN_7
 ** \arg GPIO_PIN_8
 ** \arg GPIO_PIN_9
 ** \arg GPIO_PIN_A
 ** \arg GPIO_PIN_B
 ** \arg GPIO_PIN_C
 ** \arg GPIO_PIN_D
 ** \arg GPIO_PIN_E
 ** \arg GPIO_PIN_F
 **
 ** \retval None
 **
 ******************************************************************************
 */
void IO_GPIOSetOutput(uint8_t ioport, uint32_t iopin)
{
    /* Check the parameter */
    ASSERT (IS_GPIO_PORT(ioport));
    ASSERT (IS_GPIO_PIN(iopin));
    
    IO_ConfigPort(ioport, iopin, IO_GPIO_MODE);
    switch(ioport)
    {
        case GPIO_PORT0: 
    	     FM3_GPIO->DDR0 |=  iopin;  
    	     break;
    	case GPIO_PORT1:
    	     FM3_GPIO->DDR1 |=  iopin; 
    	     break;
    	case GPIO_PORT2:
    	     FM3_GPIO->DDR2 |=  iopin; 
    	     break;
    	case GPIO_PORT3:
    	     FM3_GPIO->DDR3 |=  iopin; 
    	     break;
    	case GPIO_PORT4:
    	     FM3_GPIO->DDR4 |=  iopin; 
    	     break;
    	case GPIO_PORT5:
    	     FM3_GPIO->DDR5 |=  iopin; 
    	     break;
    	case GPIO_PORT6:
    	     FM3_GPIO->DDR6 |=  iopin; 
    	     break;
    	case GPIO_PORT7:
    	     FM3_GPIO->DDR7 |=  iopin; 
    	     break;
    	case GPIO_PORT8:
    	     FM3_GPIO->DDR8 |=  iopin; 
    	     break;

    	default: 
    	     break;
    	}
    return;	
}

 
 
/*!
 ******************************************************************************
 ** \brief Read data from GPIO 
 **
 ** \param ioport IO port number 
 ** \arg GPIO_PORT0
 ** \arg GPIO_PORT1
 ** \arg GPIO_PORT2
 ** \arg GPIO_PORT3
 ** \arg GPIO_PORT4
 ** \arg GPIO_PORT5
 ** \arg GPIO_PORT6
 ** \arg GPIO_PORT7
 ** \arg GPIO_PORT8
 **       
 ** \param iopin  IO pin number 
 ** \arg GPIO_ALL_PIN
 ** \arg GPIO_PIN_0
 ** \arg GPIO_PIN_1
 ** \arg GPIO_PIN_2
 ** \arg GPIO_PIN_3
 ** \arg GPIO_PIN_4
 ** \arg GPIO_PIN_5
 ** \arg GPIO_PIN_6
 ** \arg GPIO_PIN_7
 ** \arg GPIO_PIN_8
 ** \arg GPIO_PIN_9
 ** \arg GPIO_PIN_A
 ** \arg GPIO_PIN_B
 ** \arg GPIO_PIN_C
 ** \arg GPIO_PIN_D
 ** \arg GPIO_PIN_E
 ** \arg GPIO_PIN_F
 **
 ** \retval Data read from GPIO
 **
 ******************************************************************************
 */ 
uint32_t IO_GPIOReadData(uint8_t ioport, uint32_t iopin)
{
    uint32_t data=0; 

    /* Check the parameter */
    ASSERT (IS_GPIO_PORT(ioport));
    ASSERT (IS_GPIO_PIN(iopin));
	
    if(iopin == GPIO_ALL_PIN)
    {    
        switch(ioport)
        {
            case GPIO_PORT0: 
    	        data = FM3_GPIO->PDIR0;  
    	        break;
    	    case GPIO_PORT1:
    	        data = FM3_GPIO->PDIR1; 
    	        break;
    	    case GPIO_PORT2:
    	        data = FM3_GPIO->PDIR2; 
    	        break;
    	    case GPIO_PORT3:
    	        data = FM3_GPIO->PDIR3; 
    	        break;
    	    case GPIO_PORT4:
    	        data = FM3_GPIO->PDIR4; 
    	        break;
    	    case GPIO_PORT5:
    	        data = FM3_GPIO->PDIR5; 
    	        break;
    	    case GPIO_PORT6:
    	        data = FM3_GPIO->PDIR6; 
    	        break;
    	    case GPIO_PORT7:
    	        data = FM3_GPIO->PDIR7; 
    	        break;
    	    case GPIO_PORT8:
    	        data = FM3_GPIO->PDIR8; 
    	        break;

    	    default: 
    	        break;
        	}
    	}
    else
    {
    	switch(ioport)
        {
            case GPIO_PORT0: 
    	        data = FM3_GPIO->PDIR0 & iopin;  
    	        break;
    	    case GPIO_PORT1:
    	        data = FM3_GPIO->PDIR1 & iopin; 
    	        break;
    	    case GPIO_PORT2:
    	        data = FM3_GPIO->PDIR2 & iopin; 
    	        break;
    	    case GPIO_PORT3:
    	        data = FM3_GPIO->PDIR3 & iopin; 
    	        break;
    	    case GPIO_PORT4:
    	        data = FM3_GPIO->PDIR4 & iopin; 
    	        break;
    	    case GPIO_PORT5:
    	        data = FM3_GPIO->PDIR5 & iopin; 
    	        break;
    	    case GPIO_PORT6:
    	        data = FM3_GPIO->PDIR6 & iopin; 
    	        break;
    	    case GPIO_PORT7:
    	        data = FM3_GPIO->PDIR7 & iopin; 
    	        break;
    	    case GPIO_PORT8:
    	        data = FM3_GPIO->PDIR8 & iopin; 
    	        break;

    	    default: 
    	        break;	
    	}
    }
    return data;	
}




/*!
 ******************************************************************************
 ** \brief Write data to GPIO 
 **
 ** \param ioport IO port number 
 ** \arg GPIO_PORT0
 ** \arg GPIO_PORT1
 ** \arg GPIO_PORT2
 ** \arg GPIO_PORT3
 ** \arg GPIO_PORT4
 ** \arg GPIO_PORT5
 ** \arg GPIO_PORT6
 ** \arg GPIO_PORT7
 ** \arg GPIO_PORT8
 **       
 ** \param iopin  IO pin number 
 ** \arg GPIO_ALL_PIN
 ** \arg GPIO_PIN_0
 ** \arg GPIO_PIN_1
 ** \arg GPIO_PIN_2
 ** \arg GPIO_PIN_3
 ** \arg GPIO_PIN_4
 ** \arg GPIO_PIN_5
 ** \arg GPIO_PIN_6
 ** \arg GPIO_PIN_7
 ** \arg GPIO_PIN_8
 ** \arg GPIO_PIN_9
 ** \arg GPIO_PIN_A
 ** \arg GPIO_PIN_B
 ** \arg GPIO_PIN_C
 ** \arg GPIO_PIN_D
 ** \arg GPIO_PIN_E
 ** \arg GPIO_PIN_F
 **
 ** \param  iodata Data written
 **
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_GPIOWriteData(uint8_t ioport, uint32_t iopin, uint32_t iodata)
{
    /* Check the parameter */
    ASSERT (IS_GPIO_PORT(ioport));
    ASSERT (IS_GPIO_PIN(iopin));
	
	
    if(iopin == GPIO_ALL_PIN)
    {    
        switch(ioport)
        {
            case GPIO_PORT0: 
    	        FM3_GPIO->PDOR0 = iodata;  
    	        break;
    	    case GPIO_PORT1:
    	        FM3_GPIO->PDOR1 = iodata; 
    	        break;
    	    case GPIO_PORT2:
    	        FM3_GPIO->PDOR2 = iodata;
    	        break;
    	    case GPIO_PORT3:
    	        FM3_GPIO->PDOR3 = iodata; 
    	        break;
    	    case GPIO_PORT4:
    	        FM3_GPIO->PDOR4 = iodata; 
    	        break;
    	    case GPIO_PORT5:
    	        FM3_GPIO->PDOR5 = iodata; 
    	        break;
    	    case GPIO_PORT6:
    	        FM3_GPIO->PDOR6 = iodata; 
    	        break;
    	    case GPIO_PORT7:
    	        FM3_GPIO->PDOR7 = iodata; 
    	        break;
    	    case GPIO_PORT8:
    	        FM3_GPIO->PDOR8 = iodata; 
    	        break;

    	    default: 
    	        break;
        	}
    	}
    else
    {
    	switch(ioport)
        {
            case GPIO_PORT0: 
				if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR0 |= iopin;
                else
                    FM3_GPIO->PDOR0 &= ~iopin;
    	        break;
    	    case GPIO_PORT1:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR1 |= iopin;
                else
                    FM3_GPIO->PDOR1 &= ~iopin;
    	        break;
    	    case GPIO_PORT2:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR2 |= iopin;
                else
                    FM3_GPIO->PDOR2 &= ~iopin; 
    	        break;
    	    case GPIO_PORT3:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR3 |= iopin;
                else
                    FM3_GPIO->PDOR3 &= ~iopin; 
    	        break;
    	    case GPIO_PORT4:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR4 |= iopin;
                else
                    FM3_GPIO->PDOR4 &= ~iopin; 
    	        break;
    	    case GPIO_PORT5:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR5 |= iopin;
                else
                    FM3_GPIO->PDOR5 &= ~iopin; 
    	        break;
    	    case GPIO_PORT6:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR6 |= iopin;
                else
                    FM3_GPIO->PDOR6 &= ~iopin; 
    	        break;
    	    case GPIO_PORT7:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR7 |= iopin;
                else
                    FM3_GPIO->PDOR7 &= ~iopin; 
    	        break;
    	    case GPIO_PORT8:
    	        if((iopin & iodata) == iopin )
                    FM3_GPIO->PDOR8 |= iopin;
                else
                    FM3_GPIO->PDOR8 &= ~iopin; 
    	        break;

    	    default: 
    	        break;	
    	}
    }
    return;        	
} 
 



/*!
 ******************************************************************************
 ** \brief Set IO port pull-up 
 **
 ** \param ioport IO port number 
 ** \arg GPIO_PORT0
 ** \arg GPIO_PORT1
 ** \arg GPIO_PORT2
 ** \arg GPIO_PORT3
 ** \arg GPIO_PORT4
 ** \arg GPIO_PORT5
 ** \arg GPIO_PORT6
 ** \arg GPIO_PORT7
 ** \arg GPIO_PORT8
 **       
 ** \param iopin  IO pin number 
 ** \arg GPIO_ALL_PIN
 ** \arg GPIO_PIN_0
 ** \arg GPIO_PIN_1
 ** \arg GPIO_PIN_2
 ** \arg GPIO_PIN_3
 ** \arg GPIO_PIN_4
 ** \arg GPIO_PIN_5
 ** \arg GPIO_PIN_6
 ** \arg GPIO_PIN_7
 ** \arg GPIO_PIN_8
 ** \arg GPIO_PIN_9
 ** \arg GPIO_PIN_A
 ** \arg GPIO_PIN_B
 ** \arg GPIO_PIN_C
 ** \arg GPIO_PIN_D
 ** \arg GPIO_PIN_E
 ** \arg GPIO_PIN_F
 **
 ** \param pulltype pull-up type
 ** \arg IO_PULLUP_CONN
 ** \arg IO_PULLUP_DISCONN
 ** \retval None
 **
 ******************************************************************************
 */  
void IO_GPIOConfigPullup(uint8_t ioport, uint32_t iopin, uint8_t pulltype)
{
    /* Check the parameter */
    ASSERT (IS_IO_PULLUP(pulltype));
    ASSERT (IS_GPIO_PORT(ioport));
    ASSERT (IS_GPIO_PIN(iopin));
	
    if(pulltype == IO_PULLUP_CONN)
    {    
        switch(ioport)
        {
            case GPIO_PORT0: 
    	        FM3_GPIO->PCR0 |= iopin ;  
    	        break;
    	    case GPIO_PORT1:
    	        FM3_GPIO->PCR1 |= iopin; 
    	        break;
    	    case GPIO_PORT2:
    	        FM3_GPIO->PCR2 |= iopin; 
    	        break;
    	    case GPIO_PORT3:
    	        FM3_GPIO->PCR3 |= iopin; 
    	        break;
    	    case GPIO_PORT4:
    	        FM3_GPIO->PCR4 |= iopin; 
    	        break;
    	    case GPIO_PORT5:
    	        FM3_GPIO->PCR5 |= iopin; 
    	        break;
    	    case GPIO_PORT6:
    	        FM3_GPIO->PCR6 |= iopin; 
    	        break;
    	    case GPIO_PORT7:
    	        FM3_GPIO->PCR7 |= iopin; 
    	        break;

    	    default: 
    	        break;
    	}
    }
    else
    {
    	switch(ioport)
        {
            case GPIO_PORT0: 
    	        FM3_GPIO->PCR0 &= ~ iopin;  
    	        break;
    	    case GPIO_PORT1:
    	        FM3_GPIO->PCR1 &= ~ iopin; 
    	    	break;
    	    case GPIO_PORT2:
    	        FM3_GPIO->PCR2 &= ~ iopin; 
    	        break;
    	    case GPIO_PORT3:
    	        FM3_GPIO->PCR3 &= ~ iopin; 
    	        break;
    	    case GPIO_PORT4:
    	        FM3_GPIO->PCR4 &= ~ iopin; 
    	        break;
    	    case GPIO_PORT5:
    	        FM3_GPIO->PCR5 &= ~ iopin; 
    	        break;
    	    case GPIO_PORT6:
    	        FM3_GPIO->PCR6 &= ~ iopin; 
    	        break;
    	    case GPIO_PORT7:
    	        FM3_GPIO->PCR7 &= ~ iopin; 
    	        break;

    	    default: 
    	        break;
    	}
    }
    return;	
	
}





/*!
 ******************************************************************************
 ** \brief Set IO pin as AD pin 
 **
 ** \param iopin  IO pin number 
 ** \arg GPIO_ALL_PIN
 ** \arg GPIO_PIN_0
 ** \arg GPIO_PIN_1
 ** \arg GPIO_PIN_2
 ** \arg GPIO_PIN_3
 ** \arg GPIO_PIN_4
 ** \arg GPIO_PIN_5
 ** \arg GPIO_PIN_6
 ** \arg GPIO_PIN_7
 ** \arg GPIO_PIN_8
 ** \arg GPIO_PIN_9
 ** \arg GPIO_PIN_A
 ** \arg GPIO_PIN_B
 ** \arg GPIO_PIN_C
 ** \arg GPIO_PIN_D
 ** \arg GPIO_PIN_E
 ** \arg GPIO_PIN_F
 **
 ** \param adtype ad pin 
 ** \arg IO_AD_SET
 ** \arg IO_AD_RELEASE
 ** \retval None
 **
 ******************************************************************************
 */  
void IO_ConfigAD(uint32_t iopin, uint8_t adtype)
{

    /* Check the parameter */
    ASSERT (IS_GPIO_PIN(iopin));
	ASSERT (IS_IO_AD(adtype));
	
    if(adtype == IO_AD_SET)
    {    
        FM3_GPIO->ADE |= iopin;
    }
    else
    {
    	FM3_GPIO->ADE &= ~ iopin;     	
    }
    return;	

}





/*!
 ******************************************************************************
 ** \brief Set IO pin as Input capture pin 
 **
 ** \param icuch  input capture channel
 ** \arg IO_ICU_IC00
 ** \arg IO_ICU_IC01
 ** \arg IO_ICU_IC02
 ** \arg IO_ICU_IC03
 ** \arg IO_ICU_IC10
 ** \arg IO_ICU_IC11
 ** \arg IO_ICU_IC12
 ** \arg IO_ICU_IC13
 **
 ** \param icupin input capture pin 
 ** \arg IO_ICU_PIN0
 ** \arg IO_ICU_PIN1
 ** \arg IO_ICU_PIN2
 ** \arg IO_ICU_3LSYN
 ** \arg IO_ICU_7LSYN
 ** \arg IO_ICU_2LSYN
 ** \arg IO_ICU_6LSYN
 ** \arg IO_ICU_1LSYN
 ** \arg IO_ICU_5LSYN
 ** \arg IO_ICU_0LSYN
 ** \arg IO_ICU_4LSYN
 ** \retval None
 **
 ******************************************************************************
 */
void IO_EPFR0102InputCapture(uint8_t icuch, uint32_t icupin)
{

	/* Check the parameter */
    ASSERT (IS_IO_ICUPIN(icupin));
    ASSERT (IS_IO_ICUCH(icuch));
    
    switch(icuch)
	{
		case IO_ICU_IC00: 
			FM3_GPIO->EPFR01 &= ~(0x00000007<< 20);
			FM3_GPIO->EPFR01 |= (icupin <<20);
			break;
					
		case IO_ICU_IC01:
			FM3_GPIO->EPFR01 &= ~(0x00000007 <<23);
			FM3_GPIO->EPFR01 |= (icupin<<23);
			break;
					
		case IO_ICU_IC02: 
			FM3_GPIO->EPFR01 &= ~(0x00000007 <<26);
			FM3_GPIO->EPFR01 |= (icupin<<26);
			break;
					
		case IO_ICU_IC03:
			FM3_GPIO->EPFR01 &= ~(0x00000007 <<29);
			FM3_GPIO->EPFR01 |= (icupin<<29);
			break;
	
		case IO_ICU_IC10: 
			FM3_GPIO->EPFR02 &= ~(0x00000007 <<20);
			FM3_GPIO->EPFR02 |= (icupin<<20);
			break;
					
		case IO_ICU_IC11:
			FM3_GPIO->EPFR02 &= ~(0x00000007 <<23);
			FM3_GPIO->EPFR02 |= (icupin<<23);
			break;

		case IO_ICU_IC12: 
			FM3_GPIO->EPFR02 &= ~(0x00000007<<26);
			FM3_GPIO->EPFR02 |= (icupin<<26);
			break;
							
		case IO_ICU_IC13:
			FM3_GPIO->EPFR02 &= ~(0x00000007 <<29);
			FM3_GPIO->EPFR02 |= (icupin<<29);
			break;
		
		default:
			break;
		
	}

}



/*!
 ******************************************************************************
 ** \brief Set IO pin as Waveform Generater  pin 
 **
 ** \param rotch  waveform generater channel
 ** \arg IO_RTO_CH00
 ** \arg IO_RTO_CH01
 ** \arg IO_RTO_CH02
 ** \arg IO_RTO_CH03
 ** \arg IO_RTO_CH04
 ** \arg IO_RTO_CH05
 ** \arg IO_RTO_CH10
 ** \arg IO_RTO_CH11
 ** \arg IO_RTO_CH12
 ** \arg IO_RTO_CH13
 ** \arg IO_RTO_CH14
 ** \arg IO_RTO_CH15
 **
 ** \param rotpin waveform generater channel 
 ** \arg IO_RTO_NOOUTPUT
 ** \arg IO_RTO_PIN0
 ** \arg IO_RTO_PIN1 
 ** \retval None
 **
 ******************************************************************************
 */

void IO_EPFR0102WaveFormGen(uint8_t rtoch,uint32_t rtopin)
{

	/* Check the parameter */
    ASSERT (IS_IO_RTOCH(rtoch));
	ASSERT (IS_IO_RTOPIN(rtopin));
	
	switch(rtoch)
	{
		case IO_RTO_CH00: 
			FM3_GPIO->EPFR01 &= ~0x00000003;
			FM3_GPIO->EPFR01 |= rtopin;
			break;
					
		case IO_RTO_CH01:
			FM3_GPIO->EPFR01 &= ~(0x00000003 <<2);
			FM3_GPIO->EPFR01 |= (rtopin<<2);
			break;
					
		case IO_RTO_CH02: 
			FM3_GPIO->EPFR01 &= ~(0x00000003 <<4);
			FM3_GPIO->EPFR01 |= (rtopin<<4);
			break;
					
		case IO_RTO_CH03:
			FM3_GPIO->EPFR01 &= ~(0x00000003 <<6);
			FM3_GPIO->EPFR01 |= (rtopin<<6);
			break;
	
		case IO_RTO_CH04: 
			FM3_GPIO->EPFR01 &= ~(0x00000003 <<8);
			FM3_GPIO->EPFR01 |= (rtopin<<8);
			break;
					
		case IO_RTO_CH05:
			FM3_GPIO->EPFR01 &= ~(0x00000003 <<10);
			FM3_GPIO->EPFR01 |= (rtopin<<10);
			break;

		case IO_RTO_CH10: 
			FM3_GPIO->EPFR02 &= ~0x00000003;
			FM3_GPIO->EPFR02 |= rtopin;
			break;
							
		case IO_RTO_CH11:
			FM3_GPIO->EPFR02 &= ~(0x00000003 <<2);
			FM3_GPIO->EPFR02 |= (rtopin<<2);
			break;
							
		case IO_RTO_CH12: 
			FM3_GPIO->EPFR02 &= ~(0x00000003 <<4);
			FM3_GPIO->EPFR02 |= (rtopin<<4);
			break;
							
		case IO_RTO_CH13:
			FM3_GPIO->EPFR02 &= ~(0x00000003 <<6);
			FM3_GPIO->EPFR02 |= (rtopin<<6);
			break;
			
		case IO_RTO_CH14: 
			FM3_GPIO->EPFR02 &= ~(0x00000003 <<8);
			FM3_GPIO->EPFR02 |= (rtopin<<8);
			break;
							
		case IO_RTO_CH15:
			FM3_GPIO->EPFR02 &= ~(0x00000003 <<10);
			FM3_GPIO->EPFR02 |= (rtopin<<10);
			break;

			
		default:
			break;
		
	}

}








/*!
 ******************************************************************************
 ** \brief Set IO pin as exteranl interrupt pin 
 **
 ** \param exintch  external interrupt channel 
 ** \arg IO_EXINT_CH0PIN0
 ** \arg IO_EXINT_CH0PIN1
 ** \arg IO_EXINT_CH0PIN2
 ** \arg IO_EXINT_CH1PIN0
 ** \arg IO_EXINT_CH1PIN1
 ** \arg IO_EXINT_CH1PIN2
 ** \arg IO_EXINT_CH2PIN0
 ** \arg IO_EXINT_CH2PIN1
 ** \arg IO_EXINT_CH2PIN2
 ** \arg IO_EXINT_CH3PIN0
 ** \arg IO_EXINT_CH3PIN1
 ** \arg IO_EXINT_CH3PIN2
 ** \arg IO_EXINT_CH4PIN0
 ** \arg IO_EXINT_CH4PIN1
 ** \arg IO_EXINT_CH4PIN2
 ** \arg IO_EXINT_CH5PIN0
 ** \arg IO_EXINT_CH5PIN1
 ** \arg IO_EXINT_CH5PIN2
 ** \arg IO_EXINT_CH6PIN1
 ** \arg IO_EXINT_CH6PIN2
 ** \arg IO_EXINT_CH7PIN2
 ** \arg IO_EXINT_CH8PIN1
 ** \arg IO_EXINT_CH8PIN2
 ** \arg IO_EXINT_CH9PIN1
 ** \arg IO_EXINT_CH9PIN2
 ** \arg IO_EXINT_CH10PIN1
 ** \arg IO_EXINT_CH10PIN2
 ** \arg IO_EXINT_CH11PIN1
 ** \arg IO_EXINT_CH11PIN2
 ** \arg IO_EXINT_CH12PIN1
 ** \arg IO_EXINT_CH12PIN2
 ** \arg IO_EXINT_CH13PIN1
 ** \arg IO_EXINT_CH13PIN2
 ** \arg IO_EXINT_CH14PIN1
 ** \arg IO_EXINT_CH14PIN2
 ** \arg IO_EXINT_CH15PIN1
 ** \arg IO_EXINT_CH15PIN2
 ** \retval None
 **
 ******************************************************************************
 */  
void IO_EPFR06ExtInt(uint32_t extintpin)
{

	/* Check the parameter */
    ASSERT (IS_IO_EXINTPIN(extintpin));

	FM3_GPIO->EPFR06 |= extintpin;

}






/*!
 ******************************************************************************
 ** \brief Set IO pin as base timer pin 
 **
 ** \param btch  base timer channel 
 ** \arg IO_BT_CH0A 
 ** \arg IO_BT_CH0B
 ** \arg IO_BT_CH1A
 ** \arg IO_BT_CH1B
 ** \arg IO_BT_CH2A
 ** \arg IO_BT_CH2B
 ** \arg IO_BT_CH3A
 ** \arg IO_BT_CH3B
 ** \arg IO_BT_CH4A
 ** \arg IO_BT_CH4B
 ** \arg IO_BT_CH5A
 ** \arg IO_BT_CH5B
 ** \arg IO_BT_CH6A
 ** \arg IO_BT_CH6B
 ** \arg IO_BT_CH7A
 ** \arg IO_BT_CH7B 
 ** 
 ** \param btpin  base timer pin 
 ** \arg IO_BT_CH0ANOOUTPUT 
 ** \arg IO_BT_CH0AOUTPIN0
 ** \arg IO_BT_CH0AOUTPIN1
 ** \arg IO_BT_CH0AOUTPIN2
 ** \arg IO_BT_CH0BINPIN0
 ** \arg IO_BT_CH0BINPIN1
 ** \arg IO_BT_CH0BINPIN2
 ** \arg IO_BT_CH1AINPIN0
 ** \arg IO_BT_CH1AINPIN1
 ** \arg IO_BT_CH1AINPIN2
 ** \arg IO_BT_CH1ANOOUTPUT
 ** \arg IO_BT_CH1AOUTPIN0
 ** \arg IO_BT_CH1AOUTPIN1
 ** \arg IO_BT_CH1AOUTPIN2
 ** \arg IO_BT_CH1BINPIN0
 ** \arg IO_BT_CH1BINPIN1
 ** \arg IO_BT_CH1BINPIN2
 ** \arg IO_BT_CH2ANOOUTPUT
 ** \arg IO_BT_CH2AOUTPIN0
 ** \arg IO_BT_CH2AOUTPIN1
 ** \arg IO_BT_CH2AOUTPIN2
 ** \arg IO_BT_CH2BINPIN0
 ** \arg IO_BT_CH2BINPIN1
 ** \arg IO_BT_CH2BINPIN2
 ** \arg IO_BT_CH3AINPIN0
 ** \arg IO_BT_CH3AINPIN1
 ** \arg IO_BT_CH3AINPIN2
 ** \arg IO_BT_CH3ANOOUTPUT
 ** \arg IO_BT_CH3AOUTPIN0
 ** \arg IO_BT_CH3AOUTPIN1
 ** \arg IO_BT_CH3AOUTPIN2
 ** \arg IO_BT_CH3BINPIN0
 ** \arg IO_BT_CH3BINPIN1
 ** \arg IO_BT_CH3BINPIN2
 ** \arg IO_BT_CH4ANOOUTPUT
 ** \arg IO_BT_CH4AOUTPIN0
 ** \arg IO_BT_CH4AOUTPIN1
 ** \arg IO_BT_CH4AOUTPIN2
 ** \arg IO_BT_CH4BINPIN0
 ** \arg IO_BT_CH4BINPIN1
 ** \arg IO_BT_CH4BINPIN2
 ** \arg IO_BT_CH5AINPIN0
 ** \arg IO_BT_CH5AINPIN1
 ** \arg IO_BT_CH5AINPIN2
 ** \arg IO_BT_CH5ANOOUTPUT
 ** \arg IO_BT_CH5AOUTPIN0
 ** \arg IO_BT_CH5AOUTPIN1
 ** \arg IO_BT_CH5AOUTPIN2
 ** \arg IO_BT_CH5BINPIN0
 ** \arg IO_BT_CH5BINPIN1
 ** \arg IO_BT_CH5BINPIN2
 ** \arg IO_BT_CH6ANOOUTPUT
 ** \arg IO_BT_CH6AOUTPIN0
 ** \arg IO_BT_CH6AOUTPIN1
 ** \arg IO_BT_CH6AOUTPIN2
 ** \arg IO_BT_CH6BINPIN0
 ** \arg IO_BT_CH6BINPIN1
 ** \arg IO_BT_CH6BINPIN2
 ** \arg IO_BT_CH7AINPIN0
 ** \arg IO_BT_CH7AINPIN1
 ** \arg IO_BT_CH7AINPIN2
 ** \arg IO_BT_CH7ANOOUTPUT
 ** \arg IO_BT_CH7AOUTPIN0
 ** \arg IO_BT_CH7AOUTPIN1
 ** \arg IO_BT_CH7AOUTPIN2
 ** \arg IO_BT_CH7BINPIN0
 ** \arg IO_BT_CH7BINPIN1
 ** \arg IO_BT_CH7BINPIN2
 
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR0405BT(uint8_t btch, uint32_t btpin)
{
    
	/* Check the parameter */
    ASSERT (IS_IO_BTPIN(btpin));
    ASSERT (IS_IO_BTCH(btch));
        
    switch(btch)
    {
        case IO_BT_CH0A: 
			FM3_GPIO->EPFR04 &= ~0x00000000f;
			FM3_GPIO->EPFR04 |= btpin;
		    break;
        case IO_BT_CH0B:
			FM3_GPIO->EPFR04 &= ~(0x00000000f<<4);
			FM3_GPIO->EPFR04 |= btpin;
		    break;
		case IO_BT_CH1A: 
			FM3_GPIO->EPFR04 &= ~(0x00000000f<<8);
			FM3_GPIO->EPFR04 |= btpin;
		    break;
        case IO_BT_CH1B:
			FM3_GPIO->EPFR04 &= ~(0x00000000f<<12);
			FM3_GPIO->EPFR04 |= btpin;
		    break;
		case IO_BT_CH2A: 
			FM3_GPIO->EPFR04 &= ~(0x00000000f<<16);
			FM3_GPIO->EPFR04 |= btpin;
		    break;
        case IO_BT_CH2B:
			FM3_GPIO->EPFR04 &= ~(0x00000000f<<20);
			FM3_GPIO->EPFR04 |= btpin;
		    break;
		case IO_BT_CH3A: 
			FM3_GPIO->EPFR04 &= ~(0x00000000f<<24);
			FM3_GPIO->EPFR04 |= btpin;
		    break;
        case IO_BT_CH3B:
			FM3_GPIO->EPFR04 &= ~(0x00000000f<<28);
			FM3_GPIO->EPFR04 |= btpin;
		    break;
		case IO_BT_CH4A: 
			FM3_GPIO->EPFR05 &= ~0x00000000f;
			FM3_GPIO->EPFR05 |= btpin;
		    break;
        case IO_BT_CH4B:
			FM3_GPIO->EPFR05 &= ~(0x00000000f<<4);
			FM3_GPIO->EPFR05 |= btpin;
		    break;
		case IO_BT_CH5A: 
			FM3_GPIO->EPFR05 &= ~(0x00000000f<<8);
			FM3_GPIO->EPFR05 |= btpin;
		    break;
        case IO_BT_CH5B:
			FM3_GPIO->EPFR05 &= ~(0x00000000f<<12);
			FM3_GPIO->EPFR05 |= btpin;
		    break;
		case IO_BT_CH6A: 
			FM3_GPIO->EPFR05 &= ~(0x00000000f<<16);
			FM3_GPIO->EPFR05 |= btpin;
		    break;
        case IO_BT_CH6B:
			FM3_GPIO->EPFR05 &= ~(0x00000000f<<20);
			FM3_GPIO->EPFR05 |= btpin;
		    break;
		case IO_BT_CH7A: 
			FM3_GPIO->EPFR05 &= ~(0x00000000f<<24);
			FM3_GPIO->EPFR05 |= btpin;
		    break;
        case IO_BT_CH7B:
			FM3_GPIO->EPFR05 &= ~(0x00000000f<<28);
			FM3_GPIO->EPFR05 |= btpin;
		    break;

        default: 
            break;

    }


}






/*!
 ******************************************************************************
 ** \brief Set IO pin as AD trigger pin 
 **
 ** \param adunit ad unit number 
 ** \arg IO_AD_UNIT0
 ** \arg IO_AD_UNIT1
 ** \arg IO_AD_UNIT2
 **
 ** \param adtrgpin  ad trigger pin number 
 ** \arg IO_ADTRG_PIN0
 ** \arg IO_ADTRG_PIN1
 ** \arg IO_ADTRG_PIN2
 ** \arg IO_ADTRG_PIN3
 ** \arg IO_ADTRG_PIN4
 ** \arg IO_ADTRG_PIN5
 ** \arg IO_ADTRG_PIN6
 ** \arg IO_ADTRG_PIN7
 ** \arg IO_ADTRG_PIN8
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR09ADTrigger(uint32_t adtrgpin, uint8_t adunit)
{
    /* Check the parameter */
    ASSERT (IS_IO_ADTRGPIN(adtrgpin));
    ASSERT (IS_IO_ADUNIT(adunit));

	switch(adunit)
	{
        case IO_AD_UNIT0: 
			FM3_GPIO->EPFR09 &= ~(0x0000000f<<12);
            FM3_GPIO->EPFR09 |= (adtrgpin<<12);
			break;
			
        case IO_AD_UNIT1:
			FM3_GPIO->EPFR09 &= ~(0x0000000f<<16);
			FM3_GPIO->EPFR09 |= (adtrgpin<<16);
			break;

		case IO_AD_UNIT2:
			FM3_GPIO->EPFR09 &= ~(0x0000000f<<20);
			FM3_GPIO->EPFR09 |= (adtrgpin<<20);
			break;

        default:
			break;

	}

}







/*!
 ******************************************************************************
 ** \brief Set IO pin as CAN input pin 
 **
 ** \param canpin can pin number 
 ** \arg IO_CAN_PIN0
 ** \arg IO_CAN_PIN1
 ** \arg IO_CAN_PIN2
 **
 ** \param canch  can channel 
 ** \arg IO_CAN_CH0
 ** \arg IO_CAN_CH1
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR09CANSetInput(uint32_t canpin, uint8_t canch)
{
    /* Check the parameter */
    ASSERT (IS_IO_CANPIN(canpin));
    ASSERT (IS_IO_CANCH(canch));

    switch(canch)
	{
        case IO_CAN_CH0: 
			FM3_GPIO->EPFR09 &= ~(IO_CAN_PIN2 <<24);
            FM3_GPIO->EPFR09 |= (canpin<<24);
			break;
			
        case IO_CAN_CH1:
			FM3_GPIO->EPFR09 &= ~(IO_CAN_PIN2 <<28);
			FM3_GPIO->EPFR09 |= (canpin<<28);
			break;

        default:
			break;

	}

}



/*!
 ******************************************************************************
 ** \brief Set IO pin as CAN output pin 
 **
 ** \param canpin can pin number 
 ** \arg IO_CAN_NOPIN
 ** \arg IO_CAN_PIN0
 ** \arg IO_CAN_PIN1
 ** \arg IO_CAN_PIN2
 **
 ** \param canch  can channel 
 ** \arg IO_CAN_CH0
 ** \arg IO_CAN_CH1
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR09CANSetOutput(uint32_t canpin, uint8_t canch)
{
    /* Check the parameter */
    ASSERT (IS_IO_CANPIN(canpin));
    ASSERT (IS_IO_CANCH(canch));

    switch(canch)
	{
        case IO_CAN_CH0: 
			FM3_GPIO->EPFR09 &= ~(IO_CAN_PIN2 <<26);
            FM3_GPIO->EPFR09 |= (canpin<<27);
			break;
			
        case IO_CAN_CH1:
			FM3_GPIO->EPFR09 &= ~(IO_CAN_PIN2 <<30);
			FM3_GPIO->EPFR09 |= (canpin<<31);
			break;

        default:
			break;

	}




}






/*!
 ******************************************************************************
 ** \brief Set IO pin as qprc input pin 
 **
 ** \param qprcpin qprc pin number 
 ** \arg IO_QPRC_PIN0
 ** \arg IO_QPRC_PIN1
 ** \arg IO_QPRC_PIN2
 **
 ** \param qprcch  qprc channel 
 ** \arg IO_QPRC_CH0A
 ** \arg IO_QPRC_CH0B
 ** \arg IO_QPRC_CH0Z
 ** \arg IO_QPRC_CH1A
 ** \arg IO_QPRC_CH1B
 ** \arg IO_QPRC_CH1Z
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR09QPRCSetInput(uint32_t qprcpin, uint8_t qprcch)
{
	/* Check the parameter */
		ASSERT (IS_IO_QPRCPIN(qprcpin));
		ASSERT (IS_IO_QPRCCH(qprcch));
	
		switch(qprcch)
		{
			case IO_QPRC_CH0A: 
				FM3_GPIO->EPFR09 &= ~IO_QPRC_PIN2;
				FM3_GPIO->EPFR09 |= qprcpin;
				break;
				
			case IO_QPRC_CH0B:
				FM3_GPIO->EPFR09 &= ~(IO_QPRC_PIN2 <<2);
				FM3_GPIO->EPFR09 |= (qprcpin<<2);
				break;
				
	        case IO_QPRC_CH0Z: 
				FM3_GPIO->EPFR09 &= ~(IO_QPRC_PIN2 <<4);
				FM3_GPIO->EPFR09 |= (qprcpin<<4);
				break;
				
			case IO_QPRC_CH1A:
				FM3_GPIO->EPFR09 &= ~(IO_QPRC_PIN2 <<6);
				FM3_GPIO->EPFR09 |= (qprcpin<<6);
				break;

			case IO_QPRC_CH1B: 
				FM3_GPIO->EPFR09 &= ~(IO_QPRC_PIN2 <<8);
				FM3_GPIO->EPFR09 |= (qprcpin<<8);
				break;
				
			case IO_QPRC_CH1Z:
				FM3_GPIO->EPFR09 &= ~(IO_QPRC_PIN2 <<10);
				FM3_GPIO->EPFR09 |= (qprcpin<<10);
				break;
			default:
				break;
	
		}



}






/*!
 ******************************************************************************
 ** \brief Set IO pin as MFS pin 
 **
 ** \param MFSCH channel
 ** \arg IO_MFS_CH0
 ** \arg IO_MFS_CH1
 ** \arg IO_MFS_CH2
 ** \arg IO_MFS_CH3
 ** \arg IO_MFS_CH4
 ** \arg IO_MFS_CH5
 ** \arg IO_MFS_CH6
 ** \arg IO_MFS_CH7
 **
 ** \param MFSPIN pin number
 ** \arg IO_MFSCH0SIN0NOSOTNOSCK
 ** \arg IO_MFSCH0SIN0NOSOTSCK0
 ** \arg IO_MFSCH0SIN0NOSOTSCK1
 ** \arg IO_MFSCH0SIN0NOSOTSCK2
 ** \arg IO_MFSCH0SIN0SOT0NOSCK
 ** \arg IO_MFSCH0SIN0SOT0SCK0
 ** \arg IO_MFSCH0SIN0SOT0SCK1
 ** \arg IO_MFSCH0SIN0SOT0SCK2
 ** \arg IO_MFSCH0SIN0SOT1NOSCK
 ** \arg IO_MFSCH0SIN0SOT1SCK0
 ** \arg IO_MFSCH0SIN0SOT1SCK1
 ** \arg IO_MFSCH0SIN0SOT1SCK2
 ** \arg IO_MFSCH0SIN0SOT2NOSCK
 ** \arg IO_MFSCH0SIN0SOT2SCK0
 ** \arg IO_MFSCH0SIN0SOT2SCK1
 ** \arg IO_MFSCH0SIN0SOT2SCK2
 ** \arg IO_MFSCH0SIN1NOSOTNOSCK
 ** \arg IO_MFSCH0SIN1NOSOTSCK0
 ** \arg IO_MFSCH0SIN1NOSOTSCK1
 ** \arg IO_MFSCH0SIN1NOSOTSCK2
 ** \arg IO_MFSCH0SIN1SOT0NOSCK
 ** \arg IO_MFSCH0SIN1SOT0SCK0
 ** \arg IO_MFSCH0SIN1SOT0SCK1
 ** \arg IO_MFSCH0SIN1SOT0SCK2
 ** \arg IO_MFSCH0SIN1SOT1NOSCK
 ** \arg IO_MFSCH0SIN1SOT1SCK0
 ** \arg IO_MFSCH0SIN1SOT1SCK1
 ** \arg IO_MFSCH0SIN1SOT1SCK2
 ** \arg IO_MFSCH0SIN1SOT2NOSCK
 ** \arg IO_MFSCH0SIN1SOT2SCK0
 ** \arg IO_MFSCH0SIN1SOT2SCK1
 ** \arg IO_MFSCH0SIN1SOT2SCK2
 ** \arg IO_MFSCH0SIN2NOSOTNOSCK
 ** \arg IO_MFSCH0SIN2NOSOTSCK0
 ** \arg IO_MFSCH0SIN2NOSOTSCK1
 ** \arg IO_MFSCH0SIN2NOSOTSCK2
 ** \arg IO_MFSCH0SIN2SOT0NOSCK
 ** \arg IO_MFSCH0SIN2SOT0SCK0
 ** \arg IO_MFSCH0SIN2SOT0SCK1
 ** \arg IO_MFSCH0SIN2SOT0SCK2
 ** \arg IO_MFSCH0SIN2SOT1NOSCK
 ** \arg IO_MFSCH0SIN2SOT1SCK0
 ** \arg IO_MFSCH0SIN2SOT1SCK1
 ** \arg IO_MFSCH0SIN2SOT1SCK2
 ** \arg IO_MFSCH0SIN2SOT2NOSCK
 ** \arg IO_MFSCH0SIN2SOT2SCK0
 ** \arg IO_MFSCH0SIN2SOT2SCK1
 ** \arg IO_MFSCH0SIN2SOT2SCK2
 ** \arg IO_MFSCH1SIN0NOSOTNOSCK
 ** \arg IO_MFSCH1SIN0NOSOTSCK0
 ** \arg IO_MFSCH1SIN0NOSOTSCK1
 ** \arg IO_MFSCH1SIN0NOSOTSCK2
 ** \arg IO_MFSCH1SIN0SOT0NOSCK
 ** \arg IO_MFSCH1SIN0SOT0SCK0
 ** \arg IO_MFSCH1SIN0SOT0SCK1
 ** \arg IO_MFSCH1SIN0SOT0SCK2
 ** \arg IO_MFSCH1SIN0SOT1NOSCK
 ** \arg IO_MFSCH1SIN0SOT1SCK0
 ** \arg IO_MFSCH1SIN0SOT1SCK1
 ** \arg IO_MFSCH1SIN0SOT1SCK2
 ** \arg IO_MFSCH1SIN0SOT2NOSCK
 ** \arg IO_MFSCH1SIN0SOT2SCK0
 ** \arg IO_MFSCH1SIN0SOT2SCK1
 ** \arg IO_MFSCH1SIN0SOT2SCK2
 ** \arg IO_MFSCH1SIN1NOSOTNOSCK
 ** \arg IO_MFSCH1SIN1NOSOTSCK0
 ** \arg IO_MFSCH1SIN1NOSOTSCK1
 ** \arg IO_MFSCH1SIN1NOSOTSCK2
 ** \arg IO_MFSCH1SIN1SOT0NOSCK
 ** \arg IO_MFSCH1SIN1SOT0SCK0
 ** \arg IO_MFSCH1SIN1SOT0SCK1
 ** \arg IO_MFSCH1SIN1SOT0SCK2
 ** \arg IO_MFSCH1SIN1SOT1NOSCK
 ** \arg IO_MFSCH1SIN1SOT1SCK0
 ** \arg IO_MFSCH1SIN1SOT1SCK1
 ** \arg IO_MFSCH1SIN1SOT1SCK2
 ** \arg IO_MFSCH1SIN1SOT2NOSCK
 ** \arg IO_MFSCH1SIN1SOT2SCK0
 ** \arg IO_MFSCH1SIN1SOT2SCK1
 ** \arg IO_MFSCH1SIN1SOT2SCK2
 ** \arg IO_MFSCH1SIN2NOSOTNOSCK
 ** \arg IO_MFSCH1SIN2NOSOTSCK0
 ** \arg IO_MFSCH1SIN2NOSOTSCK1
 ** \arg IO_MFSCH1SIN2NOSOTSCK2
 ** \arg IO_MFSCH1SIN2SOT0NOSCK
 ** \arg IO_MFSCH1SIN2SOT0SCK0
 ** \arg IO_MFSCH1SIN2SOT0SCK1
 ** \arg IO_MFSCH1SIN2SOT0SCK2
 ** \arg IO_MFSCH1SIN2SOT1NOSCK
 ** \arg IO_MFSCH1SIN2SOT1SCK0
 ** \arg IO_MFSCH1SIN2SOT1SCK1
 ** \arg IO_MFSCH1SIN2SOT1SCK2
 ** \arg IO_MFSCH1SIN2SOT2NOSCK
 ** \arg IO_MFSCH1SIN2SOT2SCK0
 ** \arg IO_MFSCH1SIN2SOT2SCK1
 ** \arg IO_MFSCH1SIN2SOT2SCK2
 ** \arg IO_MFSCH2SIN0NOSOTNOSCK
 ** \arg IO_MFSCH2SIN0NOSOTSCK0
 ** \arg IO_MFSCH2SIN0NOSOTSCK1
 ** \arg IO_MFSCH2SIN0NOSOTSCK2
 ** \arg IO_MFSCH2SIN0SOT0NOSCK
 ** \arg IO_MFSCH2SIN0SOT0SCK0
 ** \arg IO_MFSCH2SIN0SOT0SCK1
 ** \arg IO_MFSCH2SIN0SOT0SCK2
 ** \arg IO_MFSCH2SIN0SOT1NOSCK
 ** \arg IO_MFSCH2SIN0SOT1SCK0
 ** \arg IO_MFSCH2SIN0SOT1SCK1
 ** \arg IO_MFSCH2SIN0SOT1SCK2
 ** \arg IO_MFSCH2SIN0SOT2NOSCK
 ** \arg IO_MFSCH2SIN0SOT2SCK0
 ** \arg IO_MFSCH2SIN0SOT2SCK1
 ** \arg IO_MFSCH2SIN0SOT2SCK2
 ** \arg IO_MFSCH2SIN1NOSOTNOSCK
 ** \arg IO_MFSCH2SIN1NOSOTSCK0
 ** \arg IO_MFSCH2SIN1NOSOTSCK1
 ** \arg IO_MFSCH2SIN1NOSOTSCK2
 ** \arg IO_MFSCH2SIN1SOT0NOSCK
 ** \arg IO_MFSCH2SIN1SOT0SCK0
 ** \arg IO_MFSCH2SIN1SOT0SCK1
 ** \arg IO_MFSCH2SIN1SOT0SCK2
 ** \arg IO_MFSCH2SIN1SOT1NOSCK
 ** \arg IO_MFSCH2SIN1SOT1SCK0
 ** \arg IO_MFSCH2SIN1SOT1SCK1
 ** \arg IO_MFSCH2SIN1SOT1SCK2
 ** \arg IO_MFSCH2SIN1SOT2NOSCK
 ** \arg IO_MFSCH2SIN1SOT2SCK0
 ** \arg IO_MFSCH2SIN1SOT2SCK1
 ** \arg IO_MFSCH2SIN1SOT2SCK2
 ** \arg IO_MFSCH2SIN2NOSOTNOSCK
 ** \arg IO_MFSCH2SIN2NOSOTSCK0
 ** \arg IO_MFSCH2SIN2NOSOTSCK1
 ** \arg IO_MFSCH2SIN2NOSOTSCK2
 ** \arg IO_MFSCH2SIN2SOT0NOSCK
 ** \arg IO_MFSCH2SIN2SOT0SCK0
 ** \arg IO_MFSCH2SIN2SOT0SCK1
 ** \arg IO_MFSCH2SIN2SOT0SCK2
 ** \arg IO_MFSCH2SIN2SOT1NOSCK
 ** \arg IO_MFSCH2SIN2SOT1SCK0
 ** \arg IO_MFSCH2SIN2SOT1SCK1
 ** \arg IO_MFSCH2SIN2SOT1SCK2
 ** \arg IO_MFSCH2SIN2SOT2NOSCK
 ** \arg IO_MFSCH2SIN2SOT2SCK0
 ** \arg IO_MFSCH2SIN2SOT2SCK1
 ** \arg IO_MFSCH2SIN2SOT2SCK2
 ** \arg IO_MFSCH3SIN0NOSOTNOSCK
 ** \arg IO_MFSCH3SIN0NOSOTSCK0
 ** \arg IO_MFSCH3SIN0NOSOTSCK1
 ** \arg IO_MFSCH3SIN0NOSOTSCK2
 ** \arg IO_MFSCH3SIN0SOT0NOSCK
 ** \arg IO_MFSCH3SIN0SOT0SCK0
 ** \arg IO_MFSCH3SIN0SOT0SCK1
 ** \arg IO_MFSCH3SIN0SOT0SCK2
 ** \arg IO_MFSCH3SIN0SOT1NOSCK
 ** \arg IO_MFSCH3SIN0SOT1SCK0
 ** \arg IO_MFSCH3SIN0SOT1SCK1
 ** \arg IO_MFSCH3SIN0SOT1SCK2
 ** \arg IO_MFSCH3SIN0SOT2NOSCK
 ** \arg IO_MFSCH3SIN0SOT2SCK0
 ** \arg IO_MFSCH3SIN0SOT2SCK1
 ** \arg IO_MFSCH3SIN0SOT2SCK2
 ** \arg IO_MFSCH3SIN1NOSOTNOSCK
 ** \arg IO_MFSCH3SIN1NOSOTSCK0
 ** \arg IO_MFSCH3SIN1NOSOTSCK1
 ** \arg IO_MFSCH3SIN1NOSOTSCK2
 ** \arg IO_MFSCH3SIN1SOT0NOSCK
 ** \arg IO_MFSCH3SIN1SOT0SCK0
 ** \arg IO_MFSCH3SIN1SOT0SCK1
 ** \arg IO_MFSCH3SIN1SOT0SCK2
 ** \arg IO_MFSCH3SIN1SOT1NOSCK
 ** \arg IO_MFSCH3SIN1SOT1SCK0
 ** \arg IO_MFSCH3SIN1SOT1SCK1
 ** \arg IO_MFSCH3SIN1SOT1SCK2
 ** \arg IO_MFSCH3SIN1SOT2NOSCK
 ** \arg IO_MFSCH3SIN1SOT2SCK0
 ** \arg IO_MFSCH3SIN1SOT2SCK1
 ** \arg IO_MFSCH3SIN1SOT2SCK2
 ** \arg IO_MFSCH3SIN2NOSOTNOSCK
 ** \arg IO_MFSCH3SIN2NOSOTSCK0
 ** \arg IO_MFSCH3SIN2NOSOTSCK1
 ** \arg IO_MFSCH3SIN2NOSOTSCK2
 ** \arg IO_MFSCH3SIN2SOT0NOSCK
 ** \arg IO_MFSCH3SIN2SOT0SCK0
 ** \arg IO_MFSCH3SIN2SOT0SCK1
 ** \arg IO_MFSCH3SIN2SOT0SCK2
 ** \arg IO_MFSCH3SIN2SOT1NOSCK
 ** \arg IO_MFSCH3SIN2SOT1SCK0
 ** \arg IO_MFSCH3SIN2SOT1SCK1
 ** \arg IO_MFSCH3SIN2SOT1SCK2
 ** \arg IO_MFSCH3SIN2SOT2NOSCK
 ** \arg IO_MFSCH3SIN2SOT2SCK0
 ** \arg IO_MFSCH3SIN2SOT2SCK1
 ** \arg IO_MFSCH3SIN2SOT2SCK2
 ** \arg IO_MFSCH4SIN0NOSOTNOSCK
 ** \arg IO_MFSCH4SIN0NOSOTSCK0
 ** \arg IO_MFSCH4SIN0NOSOTSCK1
 ** \arg IO_MFSCH4SIN0NOSOTSCK2
 ** \arg IO_MFSCH4SIN0SOT0NOSCK
 ** \arg IO_MFSCH4SIN0SOT0SCK0
 ** \arg IO_MFSCH4SIN0SOT0SCK1
 ** \arg IO_MFSCH4SIN0SOT0SCK2
 ** \arg IO_MFSCH4SIN0SOT1NOSCK
 ** \arg IO_MFSCH4SIN0SOT1SCK0
 ** \arg IO_MFSCH4SIN0SOT1SCK1
 ** \arg IO_MFSCH4SIN0SOT1SCK2
 ** \arg IO_MFSCH4SIN0SOT2NOSCK
 ** \arg IO_MFSCH4SIN0SOT2SCK0
 ** \arg IO_MFSCH4SIN0SOT2SCK1
 ** \arg IO_MFSCH4SIN0SOT2SCK2
 ** \arg IO_MFSCH4SIN1NOSOTNOSCK
 ** \arg IO_MFSCH4SIN1NOSOTSCK0
 ** \arg IO_MFSCH4SIN1NOSOTSCK1
 ** \arg IO_MFSCH4SIN1NOSOTSCK2
 ** \arg IO_MFSCH4SIN1SOT0NOSCK
 ** \arg IO_MFSCH4SIN1SOT0SCK0
 ** \arg IO_MFSCH4SIN1SOT0SCK1
 ** \arg IO_MFSCH4SIN1SOT0SCK2
 ** \arg IO_MFSCH4SIN1SOT1NOSCK
 ** \arg IO_MFSCH4SIN1SOT1SCK0
 ** \arg IO_MFSCH4SIN1SOT1SCK1
 ** \arg IO_MFSCH4SIN1SOT1SCK2
 ** \arg IO_MFSCH4SIN1SOT2NOSCK
 ** \arg IO_MFSCH4SIN1SOT2SCK0
 ** \arg IO_MFSCH4SIN1SOT2SCK1
 ** \arg IO_MFSCH4SIN1SOT2SCK2
 ** \arg IO_MFSCH4SIN2NOSOTNOSCK
 ** \arg IO_MFSCH4SIN2NOSOTSCK0
 ** \arg IO_MFSCH4SIN2NOSOTSCK1
 ** \arg IO_MFSCH4SIN2NOSOTSCK2
 ** \arg IO_MFSCH4SIN2SOT0NOSCK
 ** \arg IO_MFSCH4SIN2SOT0SCK0
 ** \arg IO_MFSCH4SIN2SOT0SCK1
 ** \arg IO_MFSCH4SIN2SOT0SCK2
 ** \arg IO_MFSCH4SIN2SOT1NOSCK
 ** \arg IO_MFSCH4SIN2SOT1SCK0
 ** \arg IO_MFSCH4SIN2SOT1SCK1
 ** \arg IO_MFSCH4SIN2SOT1SCK2
 ** \arg IO_MFSCH4SIN2SOT2NOSCK
 ** \arg IO_MFSCH4SIN2SOT2SCK0
 ** \arg IO_MFSCH4SIN2SOT2SCK1
 ** \arg IO_MFSCH4SIN2SOT2SCK2
 ** \arg IO_MFSCH5SIN0NOSOTNOSCK
 ** \arg IO_MFSCH5SIN0NOSOTSCK0
 ** \arg IO_MFSCH5SIN0NOSOTSCK1
 ** \arg IO_MFSCH5SIN0NOSOTSCK2
 ** \arg IO_MFSCH5SIN0SOT0NOSCK
 ** \arg IO_MFSCH5SIN0SOT0SCK0
 ** \arg IO_MFSCH5SIN0SOT0SCK1
 ** \arg IO_MFSCH5SIN0SOT0SCK2
 ** \arg IO_MFSCH5SIN0SOT1NOSCK
 ** \arg IO_MFSCH5SIN0SOT1SCK0
 ** \arg IO_MFSCH5SIN0SOT1SCK1
 ** \arg IO_MFSCH5SIN0SOT1SCK2
 ** \arg IO_MFSCH5SIN0SOT2NOSCK
 ** \arg IO_MFSCH5SIN0SOT2SCK0
 ** \arg IO_MFSCH5SIN0SOT2SCK1
 ** \arg IO_MFSCH5SIN0SOT2SCK2
 ** \arg IO_MFSCH5SIN1NOSOTNOSCK
 ** \arg IO_MFSCH5SIN1NOSOTSCK0
 ** \arg IO_MFSCH5SIN1NOSOTSCK1
 ** \arg IO_MFSCH5SIN1NOSOTSCK2
 ** \arg IO_MFSCH5SIN1SOT0NOSCK
 ** \arg IO_MFSCH5SIN1SOT0SCK0
 ** \arg IO_MFSCH5SIN1SOT0SCK1
 ** \arg IO_MFSCH5SIN1SOT0SCK2
 ** \arg IO_MFSCH5SIN1SOT1NOSCK
 ** \arg IO_MFSCH5SIN1SOT1SCK0
 ** \arg IO_MFSCH5SIN1SOT1SCK1
 ** \arg IO_MFSCH5SIN1SOT1SCK2
 ** \arg IO_MFSCH5SIN1SOT2NOSCK
 ** \arg IO_MFSCH5SIN1SOT2SCK0
 ** \arg IO_MFSCH5SIN1SOT2SCK1
 ** \arg IO_MFSCH5SIN1SOT2SCK2
 ** \arg IO_MFSCH5SIN2NOSOTNOSCK
 ** \arg IO_MFSCH5SIN2NOSOTSCK0
 ** \arg IO_MFSCH5SIN2NOSOTSCK1
 ** \arg IO_MFSCH5SIN2NOSOTSCK2
 ** \arg IO_MFSCH5SIN2SOT0NOSCK
 ** \arg IO_MFSCH5SIN2SOT0SCK0
 ** \arg IO_MFSCH5SIN2SOT0SCK1
 ** \arg IO_MFSCH5SIN2SOT0SCK2
 ** \arg IO_MFSCH5SIN2SOT1NOSCK
 ** \arg IO_MFSCH5SIN2SOT1SCK0
 ** \arg IO_MFSCH5SIN2SOT1SCK1
 ** \arg IO_MFSCH5SIN2SOT1SCK2
 ** \arg IO_MFSCH5SIN2SOT2NOSCK
 ** \arg IO_MFSCH5SIN2SOT2SCK0
 ** \arg IO_MFSCH5SIN2SOT2SCK1
 ** \arg IO_MFSCH5SIN2SOT2SCK2
 ** \arg IO_MFSCH6SIN0NOSOTNOSCK
 ** \arg IO_MFSCH6SIN0NOSOTSCK0
 ** \arg IO_MFSCH6SIN0NOSOTSCK1
 ** \arg IO_MFSCH6SIN0NOSOTSCK2
 ** \arg IO_MFSCH6SIN0SOT0NOSCK
 ** \arg IO_MFSCH6SIN0SOT0SCK0
 ** \arg IO_MFSCH6SIN0SOT0SCK1
 ** \arg IO_MFSCH6SIN0SOT0SCK2
 ** \arg IO_MFSCH6SIN0SOT1NOSCK
 ** \arg IO_MFSCH6SIN0SOT1SCK0
 ** \arg IO_MFSCH6SIN0SOT1SCK1
 ** \arg IO_MFSCH6SIN0SOT1SCK2
 ** \arg IO_MFSCH6SIN0SOT2NOSCK
 ** \arg IO_MFSCH6SIN0SOT2SCK0
 ** \arg IO_MFSCH6SIN0SOT2SCK1
 ** \arg IO_MFSCH6SIN0SOT2SCK2
 ** \arg IO_MFSCH6SIN1NOSOTNOSCK
 ** \arg IO_MFSCH6SIN1NOSOTSCK0
 ** \arg IO_MFSCH6SIN1NOSOTSCK1
 ** \arg IO_MFSCH6SIN1NOSOTSCK2
 ** \arg IO_MFSCH6SIN1SOT0NOSCK
 ** \arg IO_MFSCH6SIN1SOT0SCK0
 ** \arg IO_MFSCH6SIN1SOT0SCK1
 ** \arg IO_MFSCH6SIN1SOT0SCK2
 ** \arg IO_MFSCH6SIN1SOT1NOSCK
 ** \arg IO_MFSCH6SIN1SOT1SCK0
 ** \arg IO_MFSCH6SIN1SOT1SCK1
 ** \arg IO_MFSCH6SIN1SOT1SCK2
 ** \arg IO_MFSCH6SIN1SOT2NOSCK
 ** \arg IO_MFSCH6SIN1SOT2SCK0
 ** \arg IO_MFSCH6SIN1SOT2SCK1
 ** \arg IO_MFSCH6SIN1SOT2SCK2
 ** \arg IO_MFSCH6SIN2NOSOTNOSCK
 ** \arg IO_MFSCH6SIN2NOSOTSCK0
 ** \arg IO_MFSCH6SIN2NOSOTSCK1
 ** \arg IO_MFSCH6SIN2NOSOTSCK2
 ** \arg IO_MFSCH6SIN2SOT0NOSCK
 ** \arg IO_MFSCH6SIN2SOT0SCK0
 ** \arg IO_MFSCH6SIN2SOT0SCK1
 ** \arg IO_MFSCH6SIN2SOT0SCK2
 ** \arg IO_MFSCH6SIN2SOT1NOSCK
 ** \arg IO_MFSCH6SIN2SOT1SCK0
 ** \arg IO_MFSCH6SIN2SOT1SCK1
 ** \arg IO_MFSCH6SIN2SOT1SCK2
 ** \arg IO_MFSCH6SIN2SOT2NOSCK
 ** \arg IO_MFSCH6SIN2SOT2SCK0
 ** \arg IO_MFSCH6SIN2SOT2SCK1
 ** \arg IO_MFSCH6SIN2SOT2SCK2
 ** \arg IO_MFSCH7SIN0NOSOTNOSCK
 ** \arg IO_MFSCH7SIN0NOSOTSCK0
 ** \arg IO_MFSCH7SIN0NOSOTSCK1
 ** \arg IO_MFSCHSIN0NOSOTSCK2
 ** \arg IO_MFSCH76SIN0SOT0NOSCK
 ** \arg IO_MFSCH7SIN0SOT0SCK0
 ** \arg IO_MFSCH7SIN0SOT0SCK1
 ** \arg IO_MFSCH7SIN0SOT0SCK2
 ** \arg IO_MFSCH7SIN0SOT1NOSCK
 ** \arg IO_MFSCH7SIN0SOT1SCK0
 ** \arg IO_MFSCH7SIN0SOT1SCK1
 ** \arg IO_MFSCH7SIN0SOT1SCK2
 ** \arg IO_MFSCH7SIN0SOT2NOSCK
 ** \arg IO_MFSCH7SIN0SOT2SCK0
 ** \arg IO_MFSCH7SIN0SOT2SCK1
 ** \arg IO_MFSCH7SIN0SOT2SCK2
 ** \arg IO_MFSCH7SIN1NOSOTNOSCK
 ** \arg IO_MFSCH7SIN1NOSOTSCK0
 ** \arg IO_MFSCH7SIN1NOSOTSCK1
 ** \arg IO_MFSCHSSIN1NOSOTSCK2
 ** \arg IO_MFSCH7SIN1SOT0NOSCK
 ** \arg IO_MFSCH7SIN1SOT0SCK0
 ** \arg IO_MFSCH7SIN1SOT0SCK1
 ** \arg IO_MFSCH7SIN1SOT0SCK2
 ** \arg IO_MFSCH7SIN1SOT1NOSCK
 ** \arg IO_MFSCH7SIN1SOT1SCK0
 ** \arg IO_MFSCH7SIN1SOT1SCK1
 ** \arg IO_MFSCH7SIN1SOT1SCK2
 ** \arg IO_MFSCH7SIN1SOT2NOSCK
 ** \arg IO_MFSCH7SIN1SOT2SCK0
 ** \arg IO_MFSCH7SIN1SOT2SCK1
 ** \arg IO_MFSCH7SIN1SOT2SCK2
 ** \arg IO_MFSCH7SIN2NOSOTNOSCK
 ** \arg IO_MFSCH7SIN2NOSOTSCK0
 ** \arg IO_MFSCH7SIN2NOSOTSCK1
 ** \arg IO_MFSCHSSIN2NOSOTSCK2
 ** \arg IO_MFSCH7SIN2SOT0NOSCK
 ** \arg IO_MFSCH7SIN2SOT0SCK0
 ** \arg IO_MFSCH7SIN2SOT0SCK1
 ** \arg IO_MFSCH7SIN2SOT0SCK2
 ** \arg IO_MFSCH7SIN2SOT1NOSCK
 ** \arg IO_MFSCH7SIN2SOT1SCK0
 ** \arg IO_MFSCH7SIN2SOT1SCK1
 ** \arg IO_MFSCH7SIN2SOT1SCK2
 ** \arg IO_MFSCH7SIN2SOT2NOSCK
 ** \arg IO_MFSCH7SIN2SOT2SCK0
 ** \arg IO_MFSCH7SIN2SOT2SCK1
 ** \arg IO_MFSCH7SIN2SOT2SCK2

 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR0708MFS(uint8_t mfsch, uint32_t mfspin)
{

    /* Check the parameter */
	ASSERT (IS_IO_MFSCH(mfsch));
	ASSERT (IS_IO_MFSPIN(mfspin));
	
	switch(mfsch)
	{
		case IO_MFS_CH0: 
				FM3_GPIO->EPFR07 &= ~0x000003f0;
				FM3_GPIO->EPFR07 |= mfspin;
				break;
				
		case IO_MFS_CH1:
				FM3_GPIO->EPFR07 &= ~(0x000003f0 <<6);
				FM3_GPIO->EPFR07 |= mfspin;
				break;

		case IO_MFS_CH2:
                FM3_GPIO->EPFR07 &= ~(0x000003f0 <<12);
				FM3_GPIO->EPFR07 |= mfspin;
				break;
      
		case IO_MFS_CH3:
				FM3_GPIO->EPFR07 &= ~(0x000003f0 <<18);
				FM3_GPIO->EPFR07 |= mfspin;
				break;

		case IO_MFS_CH4:
                FM3_GPIO->EPFR08 &= ~0x000003f0;
				FM3_GPIO->EPFR08 |= mfspin;
				break;

		case IO_MFS_CH5:
				FM3_GPIO->EPFR08 &= ~(0x000003f0 <<6);
				FM3_GPIO->EPFR08 |= mfspin;
				break;

		case IO_MFS_CH6:
				FM3_GPIO->EPFR08 &= ~(0x000003f0 <<12);
				FM3_GPIO->EPFR08 |= mfspin;
				break;

		case IO_MFS_CH7:
				FM3_GPIO->EPFR08 &= ~(0x000003f0 <<18);
				FM3_GPIO->EPFR08 |= mfspin;
				break;
		default:
				break;
	
	}
    
}







/*!
 ******************************************************************************
 ** \brief Set IO pin as CR output pin 
 **
 ** \param croutpin set nmix cr output function 
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR00CR(uint8_t croutpin)
{

    /* Check the parameter */
    ASSERT (IS_IO_CRPIN(croutpin));
    
    if(croutpin == ENABLE)
    {
//        bFM3_GPIO_EPFR00_CROUTE = 1;
    }
	else
	{
//        bFM3_GPIO_EPFR00_CROUTE = 0;
	}
	return;

}




/*!
 ******************************************************************************
 ** \brief Set IO pin as NMIX pin 
 **
 ** \param nmixmode set nmix pin function 
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */ 
void IO_EPFR00ConfigNMIX(uint8_t nmixmode)
{

  /* Check the parameter */
    ASSERT (IS_IO_NMIX(nmixmode));
    
    if(nmixmode == ENABLE)
    {
        bFM3_GPIO_EPFR00_NMIS = 1;
    }
	else
	{
        bFM3_GPIO_EPFR00_NMIS = 0;
	}
	return;


}


/*!
 ******************************************************************************
 ** \brief Set IO pin as USB D+ resistor control signal output pin 
 **
 ** \param ctrlsgnl  enable/disable USB D+ control signal output 
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */
void IO_EPFR00ConfigUSBCtlSgnl(uint8_t ctrlsgnl)
{

    /* Check the parameter */
    ASSERT (IS_IO_USB0CTL(ctrlsgnl));
    
    if(ctrlsgnl == ENABLE)
    {
//        bFM3_GPIO_EPFR00_USB0PE = 1;
    }
	else
	{
//        bFM3_GPIO_EPFR00_USB0PE = 0;
	}
	return;


}



/*!
 ******************************************************************************
 ** \brief Set IO pin as Jtag pin 
 **
 ** \param Jtagmode  enable/disable Jtag pin 
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */
 void IO_EPFR00ConfigJtag(uint8_t Jtagmode)
{


   /* Check the parameter */
    ASSERT (IS_IO_JTAGPIN(Jtagmode));
   
    if(Jtagmode == ENABLE)
    {
	    bFM3_GPIO_EPFR00_JTAGEN0B = 1;
	    bFM3_GPIO_EPFR00_JTAGEN1S = 1;
    }
    else
    {
	    bFM3_GPIO_EPFR00_JTAGEN0B = 0;
	    bFM3_GPIO_EPFR00_JTAGEN1S = 0;
    }
    return;

}



/*!
 ******************************************************************************
 ** \brief Set IO pin as Trace pin 
 **
 ** \param tracemode  enable/disable trace pin
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */
void IO_EPFR00ConfigTrace(uint8_t tracemode)
{

    /* Check the parameter */
    ASSERT (IS_IO_TRACEPIN(tracemode));
  
    if(tracemode == ENABLE)
    {
	    bFM3_GPIO_EPFR00_TRC0E = 1;
	    bFM3_GPIO_EPFR00_TRC1E = 1;
    }
    else
    {
	    bFM3_GPIO_EPFR00_TRC0E = 0;
	    bFM3_GPIO_EPFR00_TRC1E = 0;
    }
    return;


}



/*!
 ******************************************************************************
 ** \brief Set IO pin as  SWD pin 
 **
 ** \param tracemode  enable/disable trace pin
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */
void IO_EPFR00ConfigSWD(uint8_t swdmode)
{

    /* Check the parameter */
    ASSERT (IS_IO_SWDPIN(swdmode));
  
    if(swdmode == ENABLE)
    {
	    bFM3_GPIO_EPFR00_TRC0E = 1;
	}
    else
    {
	    bFM3_GPIO_EPFR00_TRC0E = 0;
	    
    }
    return;
}



/*!
 ******************************************************************************
 ** \brief Set IO pin as USB pin 
 **
 ** \param usbpin  enable/disable usb pin
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */
void IO_EPFRSPSRConfigUSB(uint8_t usbpin)
{
    /* Check the parameter */
    ASSERT (IS_IO_USBPIN(usbpin));
    
    if(usbpin == ENABLE)
    {
        bFM3_GPIO_SPSR_USB0C = 1;
    }
	else
	{
        bFM3_GPIO_SPSR_USB0C = 0;
	}
	return;

}


/*!
 ******************************************************************************
 ** \brief Set IO pin as main clock oscillation pin 
 **
 ** \param mnxc  enable/disable main clock oscillation pin 
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */
void IO_EPFRSPSRCofigMainXC(uint8_t mnxc)
{



}


/*!
 ******************************************************************************
 ** \brief Set IO pin as sub clock oscillation pin  
 **
 ** \param usbpin  enable/disable sub clock oscillation pin 
 ** \arg ENABLE
 ** \arg DISABLE
 **
 ** \retval None
 **
 ******************************************************************************
 */void IO_EPFRSPSRCofigSubXC(uint8_t subxc)
{
	/* Check the parameter */
    ASSERT (IS_IO_SUBXC(subxc));
    
    if(subxc == ENABLE)
    {
        bFM3_GPIO_SPSR_SUBXC = 1;
    }
	else
	{
        bFM3_GPIO_SPSR_SUBXC = 0;
	}
	return;

}
 

/*   从新版本的驱动库中拷贝过来的函数   !!!*/
/*!
 ******************************************************************************
 ** \brief Write the data to a specific pin
 **
 ** \param Port Port index
 ** \arg IO_PORT0
 ** \arg IO_PORT1
 ** \arg ...
 ** \arg IO_PORT14
 ** \arg IO_PORT15
 ** \param Pin Pin index
 ** \arg IO_PINx0
 ** \arg IO_PINx1
 ** \arg ...
 ** \arg IO_PINx14
 ** \arg IO_PINx15
 ** \param Data the data output to the IO pin
 ** \arg IO_BIT_CLR
 ** \arg IO_BIT_SET
 **
 ** \return None
 ******************************************************************************
 */
void IO_WriteGPIOPin(uint8_t Port, uint32_t Pin, uint8_t Data )
{
    volatile uint32_t *ppdoraddr;
    ppdoraddr = GPIO_PDOR_BASE_ADDR + Port;
    if(Data == IO_BIT_CLR)
    {
        *ppdoraddr &= ~Pin;
    }
    else if(Data == IO_BIT_SET)
    {
        *ppdoraddr |= Pin;
    }
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Read the pin level of a specific pin
 **
 ** \param Port Port index
 ** \arg IO_PORT0
 ** \arg IO_PORT1
 ** \arg ...
 ** \arg IO_PORT14
 ** \arg IO_PORT15
 ** \param Pin Pin index
 ** \arg IO_PINx0
 ** \arg IO_PINx1
 ** \arg ...
 ** \arg IO_PINx14
 ** \arg IO_PINx15
 ** \param pData the data input from the IO pin
 **
 ** \return None
 ******************************************************************************
 */
uint8_t IO_ReadGPIOPin(uint8_t Port, uint32_t Pin)
{
    volatile uint32_t *ppdiraddr;
    uint8_t tempdata, cnt;
    ppdiraddr = GPIO_PDIR_BASE_ADDR + Port;
    for(cnt = 0; cnt<=16; cnt++)
    {
        if(0 == (Pin>>cnt))
        {
           cnt = cnt -1;
           break;
        }
    }
    tempdata =  (*ppdiraddr & Pin) >> cnt; 
    
    return (tempdata & 0x01); 
}

/*!
 ******************************************************************************
 ** \brief Configure general purpose IO function setting for a specific pin
 **
 ** \param Port Port index
 ** \arg IO_PORT0
 ** \arg IO_PORT1
 ** \arg ...
 ** \arg IO_PORT14
 ** \arg IO_PORT15
 ** \param Pin Pin index
 ** \arg IO_PINx0
 ** \arg IO_PINx1
 ** \arg ...
 ** \arg IO_PINx14
 ** \arg IO_PINx15
 ** \param Dir IO direction
 ** \arg IO_DIR_INPUT
 ** \arg IO_DIR_OUTPUT
 ** \param Pulllup Pull-up register selection
 ** \arg IO_PULLUP_OFF
 ** \arg IO_PULLUP_ON
 **
 ** \return None
 ******************************************************************************
 */
void IO_ConfigGPIOPin(uint8_t Port, uint32_t Pin, uint8_t Dir, uint8_t Pullup)
{
    volatile uint32_t *pddraddr, *ppcraddr;
    pddraddr = GPIO_DDR_BASE_ADDR + Port;
    ppcraddr = GPIO_PCR_BASE_ADDR + Port;
    if(Dir == IO_DIR_OUTPUT)
    {
        *pddraddr |= Pin;
    }
    else if(Dir == IO_DIR_INPUT)
    {
        *pddraddr &= ~Pin;
    }
    
    if(Pullup == IO_PULLUP_OFF)
    {
        *ppcraddr &= ~Pin;
    }
    else if(Pullup == IO_PULLUP_ON)
    {
        *ppcraddr |= Pin;
    }
    
    return;
}

 /*****************************************************************************/
/* END OF FILE */
 
 

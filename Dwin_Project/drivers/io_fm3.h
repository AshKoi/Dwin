/*!
 ******************************************************************************
 **
 ** \file io_fm3.h
 **
 ** \brief head file of io  driver
 **
 ** 
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-8
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
 
#ifndef _IO_FM3_H_
#define _IO_FM3_H_
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
 
 
 /* IO port index definition */
/*! \brief IO port0 */ 
#define IO_PORT0          (0x00u)
/*! \brief IO port1 */ 
#define IO_PORT1          (0x01u)
/*! \brief IO port2 */ 
#define IO_PORT2          (0x02u)
/*! \brief IO port3 */ 
#define IO_PORT3          (0x03u)
/*! \brief IO port4 */ 
#define IO_PORT4          (0x04u)
/*! \brief IO port5 */ 
#define IO_PORT5          (0x05u)
/*! \brief IO port6 */ 
#define IO_PORT6          (0x06u)
/*! \brief IO port7 */ 
#define IO_PORT7          (0x07u)
/*! \brief IO port8 */ 
#define IO_PORT8          (0x08u)
/*! \brief IO port9 */ 
#define IO_PORT9          (0x09u)
/*! \brief IO portA */ 
#define IO_PORTA          (0x0Au)
/*! \brief IO portB */ 
#define IO_PORTB          (0x0Bu)
/*! \brief IO portC */ 
#define IO_PORTC          (0x0Cu)
/*! \brief IO portD */ 
#define IO_PORTD          (0x0Du)
/*! \brief IO portE */ 
#define IO_PORTE          (0x0Eu)
/*! \brief IO portF */ 
#define IO_PORTF          (0x0Fu)

/* IO pin index definition */
/*! \brief IO Pin0 */ 
#define IO_PINx0          (1ul<<0)
/*! \brief IO Pin1 */ 
#define IO_PINx1          (1ul<<1)
/*! \brief IO Pin2 */ 
#define IO_PINx2          (1ul<<2)
/*! \brief IO Pin3 */ 
#define IO_PINx3          (1ul<<3)
/*! \brief IO Pin4 */ 
#define IO_PINx4          (1ul<<4)
/*! \brief IO Pin5 */ 
#define IO_PINx5          (1ul<<5)
/*! \brief IO Pin6 */ 
#define IO_PINx6          (1ul<<6)
/*! \brief IO Pin7 */ 
#define IO_PINx7          (1ul<<7)
/*! \brief IO Pin8 */ 
#define IO_PINx8          (1ul<<8)
/*! \brief IO Pin9 */ 
#define IO_PINx9          (1ul<<9)
/*! \brief IO PinA */ 
#define IO_PINxA          (1ul<<10)
/*! \brief IO PinB */ 
#define IO_PINxB          (1ul<<11)
/*! \brief IO PinC */ 
#define IO_PINxC          (1ul<<12)
/*! \brief IO PinD */ 
#define IO_PINxD          (1ul<<13)
/*! \brief IO PinE */ 
#define IO_PINxE          (1ul<<14)
/*! \brief IO PinF */ 
#define IO_PINxF          (1ul<<15)

/* IO pin direction definition */
/*! \brief IO input */ 
#define IO_DIR_INPUT       (0)
/*! \brief IO output */ 
#define IO_DIR_OUTPUT      (1)


/* IO pin pull-up register selection */
/*! \brief IO pull-up register disconnection */ 
#define IO_PULLUP_OFF      (0)
/*! \brief IO pull-up register connection */ 
#define IO_PULLUP_ON       (1)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief GPIO all pins 0~F */
#define GPIO_ALL_PIN            0x0000ffff  // GPIO pin 0~F
/*! \brief GPIO all pin 0 */
#define GPIO_PIN_0              0x00000001  // GPIO pin 0
/*! \brief GPIO all pin 1 */
#define GPIO_PIN_1              0x00000002  // GPIO pin 1
/*! \brief GPIO all pin 2 */
#define GPIO_PIN_2              0x00000004  // GPIO pin 2
/*! \brief GPIO all pin 3 */
#define GPIO_PIN_3              0x00000008  // GPIO pin 3
/*! \brief GPIO all pin 4 */
#define GPIO_PIN_4              0x00000010  // GPIO pin 4
/*! \brief GPIO all pin 5 */
#define GPIO_PIN_5              0x00000020  // GPIO pin 5
/*! \brief GPIO all pin 6 */
#define GPIO_PIN_6              0x00000040  // GPIO pin 6
/*! \brief GPIO all pin 7 */
#define GPIO_PIN_7              0x00000080  // GPIO pin 7
/*! \brief GPIO all pin 8 */
#define GPIO_PIN_8              0x00000100  // GPIO pin 8
/*! \brief GPIO all pin 9 */
#define GPIO_PIN_9              0x00000200  // GPIO pin 9
/*! \brief GPIO all pin A */
#define GPIO_PIN_A              0x00000400  // GPIO pin A
/*! \brief GPIO all pin B */
#define GPIO_PIN_B              0x00000800  // GPIO pin B
/*! \brief GPIO all pin C */
#define GPIO_PIN_C              0x00001000  // GPIO pin C
/*! \brief GPIO all pin D */
#define GPIO_PIN_D              0x00002000  // GPIO pin D
/*! \brief GPIO all pin E */
#define GPIO_PIN_E              0x00004000  // GPIO pin E
/*! \brief GPIO all pin F */
#define GPIO_PIN_F              0x00008000  // GPIO pin F
/*! \brief Check parameter */
#define IS_GPIO_PIN(gpiopin)                    ((gpiopin == GPIO_ALL_PIN) \
	                                            ||(gpiopin == GPIO_PIN_0)\
	                                            ||(gpiopin == GPIO_PIN_1)\
	                                            ||(gpiopin == GPIO_PIN_2)\
	                                            ||(gpiopin == GPIO_PIN_3)\
	                                            ||(gpiopin == GPIO_PIN_4)\
	                                            ||(gpiopin == GPIO_PIN_5)\
	                                            ||(gpiopin == GPIO_PIN_6)\
	                                            ||(gpiopin == GPIO_PIN_7)\
	                                            ||(gpiopin == GPIO_PIN_8)\
	                                            ||(gpiopin == GPIO_PIN_9)\
	                                            ||(gpiopin == GPIO_PIN_A)\
	                                            ||(gpiopin == GPIO_PIN_B)\
	                                            ||(gpiopin == GPIO_PIN_C)\
	                                            ||(gpiopin == GPIO_PIN_D)\
                                                ||(gpiopin == GPIO_PIN_E)\
                                                ||(gpiopin == GPIO_PIN_F))   



/* IO pin set definition */
/*! \brief IO pin clear */ 
#define IO_BIT_CLR         (0)
/*! \brief IO pin set */ 
#define IO_BIT_SET         (1)


/*! \brief GPIO Port P00 */
#define GPIO_PORT0           0x00  
/*! \brief GPIO Port P10 */
#define GPIO_PORT1           0x01 
/*! \brief GPIO Port P20 */
#define GPIO_PORT2           0x02  
/*! \brief GPIO Port P30 */
#define GPIO_PORT3           0x03 
/*! \brief GPIO Port P40 */ 
#define GPIO_PORT4           0x04  
/*! \brief GPIO Port P50 */
#define GPIO_PORT5           0x05 
/*! \brief GPIO Port P60 */ 
#define GPIO_PORT6           0x06  
/*! \brief GPIO Port P70 */
#define GPIO_PORT7           0x07  
/*! \brief GPIO Port P80 */
#define GPIO_PORT8           0x08  
/*! \brief Check parameter */
#define IS_GPIO_PORT(gpioport)                  ((gpioport == GPIO_PORT0) \
	                                            ||(gpioport == GPIO_PORT1)\
	                                            ||(gpioport == GPIO_PORT2)\
	                                            ||(gpioport == GPIO_PORT3)\
	                                            ||(gpioport == GPIO_PORT4)\
	                                            ||(gpioport == GPIO_PORT5)\
	                                            ||(gpioport == GPIO_PORT6)\
	                                            ||(gpioport == GPIO_PORT7)\
	                                            ||(gpioport == GPIO_PORT8))



/*! \brief GPIO Port low level */
#define LOW_LEVEL 	0
/*! \brief GPIO Port high level */
#define HIGH_LEVEL 	1
/*! \brief Check parameter */
#define IS_GPIO_LEVEL(gpiolevel)                ((gpiolevel == LOW_LEVEL) \
                                                ||(gpiolevel == HIGH_LEVEL))



/*! \brief GPIO Port output */
#define IO_GPIO_MODE	       0x00000000 
/*! \brief Peripheral function port */
#define IO_PERIPHERAL_MODE     0x00000001  
/*! \brief Check parameter */
#define IS_IO_MODE(mode)                      ((mode == IO_GPIO_MODE) \
                                                ||(mode == IO_PERIPHERAL_MODE))
                                              
/*! \brief Port pull-up connect */
#define IO_PULLUP_CONN	       0x00000000 
/*! \brief Port pull-up disconnect */
#define IO_PULLUP_DISCONN      0x00000001  
/*! \brief Check parameter */
#define IS_IO_PULLUP(mode)                   ((mode == IO_PULLUP_CONN) \
                                                ||(mode == IO_PULLUP_DISCONN))


/*! \brief AD pin set */
#define IO_AD_SET	       0x00000001 
/*! \brief AD pin disconnect */
#define IO_AD_RELEASE      0x00000000  
/*! \brief Check parameter */
#define IS_IO_AD(mode)                   ((mode == IO_AD_SET) \
                                                ||(mode == IO_AD_RELEASE))





	       
/*! \brief external interrupt channel 0 pin 0 */
#define IO_EXINT_CH0PIN0      0x00000001  
/*! \brief external interrupt channel 0 pin 1 */
#define IO_EXINT_CH0PIN1      0x00000002 
/*! \brief external interrupt channel 0 pin2 */
#define IO_EXINT_CH0PIN2      0x00000003 
/*! \brief external interrupt channel 1 pin 0 */
#define IO_EXINT_CH1PIN0      0x00000004  
/*! \brief external interrupt channel 1 pin 1 */
#define IO_EXINT_CH1PIN1      0x00000008 
/*! \brief external interrupt channel 1 pin 2 */
#define IO_EXINT_CH1PIN2      0x0000000c 
/*! \brief external interrupt channel 2 pin 0 */
#define IO_EXINT_CH2PIN0      0x00000010  
/*! \brief external interrupt channel 2 pin 1 */
#define IO_EXINT_CH2PIN1      0x00000020 
/*! \brief external interrupt channel 2 pin 2 */
#define IO_EXINT_CH2PIN2      0x00000030 
/*! \brief external interrupt channel 3 pin 0 */
#define IO_EXINT_CH3PIN0      0x00000040  
/*! \brief external interrupt channel 3 pin 1 */
#define IO_EXINT_CH3PIN1     0x00000080 
/*! \brief external interrupt channel 3 pin 2 */
#define IO_EXINT_CH3PIN2     0x000000c0 
/*! \brief external interrupt channel 4 pin 0 */
#define IO_EXINT_CH4PIN0     0x00000100 
/*! \brief external interrupt channel 4 pin 1 */
#define IO_EXINT_CH4PIN1     0x00000200  
/*! \brief external interrupt channel 4 pin 2 */
#define IO_EXINT_CH4PIN2     0x00000300 
/*! \brief external interrupt channel 5 pin 0 */
#define IO_EXINT_CH5PIN0     0x00000400 
/*! \brief external interrupt channel 5 pin 1 */
#define IO_EXINT_CH5PIN1     0x00000800
/*! \brief external interrupt channel 5 pin 2 */
#define IO_EXINT_CH5PIN2     0x00000c00
/*! \brief external interrupt channel 6 pin 1 */
#define IO_EXINT_CH6PIN1     0x00002000
/*! \brief external interrupt channel 6 pin 2 */
#define IO_EXINT_CH6PIN2     0x00003000
/*! \brief external interrupt channel 7 pin 2 */
#define IO_EXINT_CH7PIN2     0x0000c000
/*! \brief external interrupt channel 8 pin 0 */
#define IO_EXINT_CH8PIN1     0x00020000
/*! \brief external interrupt channel 8 pin 2 */
#define IO_EXINT_CH8PIN2     0x00030000
/*! \brief external interrupt channel 9 pin 1 */
#define IO_EXINT_CH9PIN1     0x00080000
/*! \brief external interrupt channel 9 pin 2 */
#define IO_EXINT_CH9PIN2     0x000c0000
/*! \brief external interrupt channel 10 pin 1 */
#define IO_EXINT_CH10PIN1    0x00200000
/*! \brief external interrupt channel 10 pin 2 */
#define IO_EXINT_CH10PIN2    0x00300000
/*! \brief external interrupt channel 11 pin 1 */
#define IO_EXINT_CH11PIN1    0x00800000
/*! \brief external interrupt channel 11 pin 2 */
#define IO_EXINT_CH11PIN2    0x00c00000
/*! \brief external interrupt channel 12 pin 1 */
#define IO_EXINT_CH12PIN1    0x02000000
/*! \brief external interrupt channel 12 pin 2 */
#define IO_EXINT_CH12PIN2    0x03000000
/*! \brief external interrupt channel 13 pin 1 */
#define IO_EXINT_CH13PIN1    0x08000000
/*! \brief external interrupt channel 13 pin 2 */
#define IO_EXINT_CH13PIN2    0x0c000000
/*! \brief external interrupt channel 14 pin 1 */
#define IO_EXINT_CH14PIN1   0x20000000
/*! \brief external interrupt channel 14 pin 2 */
#define IO_EXINT_CH14PIN2   0x30000000
/*! \brief external interrupt channel 15 pin 1 */
#define IO_EXINT_CH15PIN1   0x80000000
/*! \brief external interrupt channel 15 pin 2 */
#define IO_EXINT_CH15PIN2   0xc0000000

/*! \brief Check parameter */
#define IS_IO_EXINTPIN(mode)                    ((mode == IO_EXINT_CH0PIN0) \
												  ||(mode == IO_EXINT_CH0PIN1) \
												  ||(mode == IO_EXINT_CH0PIN2) \
												  ||(mode == IO_EXINT_CH1PIN0) \
												  ||(mode == IO_EXINT_CH1PIN1) \
												  ||(mode == IO_EXINT_CH1PIN2) \
												  ||(mode == IO_EXINT_CH2PIN0) \
												  ||(mode == IO_EXINT_CH2PIN1) \
												  ||(mode == IO_EXINT_CH2PIN2) \
												  ||(mode == IO_EXINT_CH3PIN0) \
												  ||(mode == IO_EXINT_CH3PIN1) \
												  ||(mode == IO_EXINT_CH3PIN2) \
												  ||(mode == IO_EXINT_CH4PIN0) \
												  ||(mode == IO_EXINT_CH4PIN1) \
												  ||(mode == IO_EXINT_CH4PIN2) \
												  ||(mode == IO_EXINT_CH5PIN0) \
												  ||(mode == IO_EXINT_CH5PIN1) \
												  ||(mode == IO_EXINT_CH5PIN2) \
												  ||(mode == IO_EXINT_CH6PIN1) \
												  ||(mode == IO_EXINT_CH6PIN2) \
												  ||(mode == IO_EXINT_CH7PIN2) \
												  ||(mode == IO_EXINT_CH8PIN1) \
												  ||(mode == IO_EXINT_CH8PIN2) \
												  ||(mode == IO_EXINT_CH9PIN1) \
												  ||(mode == IO_EXINT_CH9PIN2) \
												  ||(mode == IO_EXINT_CH10PIN1) \
												  ||(mode == IO_EXINT_CH10PIN2) \
												  ||(mode == IO_EXINT_CH11PIN1) \
												  ||(mode == IO_EXINT_CH11PIN2) \
												  ||(mode == IO_EXINT_CH12PIN1) \
												  ||(mode == IO_EXINT_CH12PIN2) \
												  ||(mode == IO_EXINT_CH13PIN1) \
												  ||(mode == IO_EXINT_CH13PIN2) \
												  ||(mode == IO_EXINT_CH14PIN1) \
												  ||(mode == IO_EXINT_CH14PIN2) \
												  ||(mode == IO_EXINT_CH15PIN1) \
												  ||(mode == IO_EXINT_CH15PIN2))



/*! \brief BT Channel 0A  */   
#define IO_BT_CH0A              0X00             
/*! \brief BT Channel 0B  */   
#define IO_BT_CH0B              0X01
/*! \brief BT Channel 1A  */   
#define IO_BT_CH1A              0X02
/*! \brief BT Channel 1B  */   
#define IO_BT_CH1B              0X03
/*! \brief BT Channel 2A  */   
#define IO_BT_CH2A              0X04
/*! \brief BT Channel 2B  */   
#define IO_BT_CH2B              0X05
/*! \brief BT Channel 3A  */   
#define IO_BT_CH3A              0X07
/*! \brief BT Channel 3B  */   
#define IO_BT_CH3B              0X08
/*! \brief BT Channel 4A  */   
#define IO_BT_CH4A              0X09
/*! \brief BT Channel 4B  */   
#define IO_BT_CH4B              0X0A
/*! \brief BT Channel 5A  */   
#define IO_BT_CH5A              0X0B
/*! \brief BT Channel 5B  */   
#define IO_BT_CH5B              0X0C
/*! \brief BT Channel 6A  */   
#define IO_BT_CH6A              0X0D
/*! \brief BT Channel 6B  */   
#define IO_BT_CH6B              0X0E
/*! \brief BT Channel 7A  */   
#define IO_BT_CH7A              0X0F
/*! \brief BT Channel 7B  */   
#define IO_BT_CH7B              0X10
/*! \brief BT Channel Check parameter */
#define IS_IO_BTCH(mode)                   ((mode == IO_BT_CH0A) \
												  ||(mode ==IO_BT_CH0B ) \
												  ||(mode ==IO_BT_CH1A ) \
												  ||(mode ==IO_BT_CH1B ) \
												  ||(mode ==IO_BT_CH2A ) \
												  ||(mode ==IO_BT_CH2B ) \
												  ||(mode ==IO_BT_CH3A ) \
												  ||(mode ==IO_BT_CH3B ) \
												  ||(mode ==IO_BT_CH4A ) \
												  ||(mode ==IO_BT_CH4B ) \
												  ||(mode ==IO_BT_CH5A ) \
												  ||(mode ==IO_BT_CH5B ) \
												  ||(mode ==IO_BT_CH6A ) \
												  ||(mode ==IO_BT_CH6B ) \
												  ||(mode ==IO_BT_CH7A ) \
												  ||(mode ==IO_BT_CH7B )) 
	


	       
/*! \brief BT Channel 0A no output */
#define IO_BT_CH0ANOOUTPUT      0x00000000 
/*! \brief BT Channel 0A output pin0 */
#define IO_BT_CH0AOUTPIN0		0x00000004
/*! \brief BT Channel 0A output pin1 */
#define IO_BT_CH0AOUTPIN1		0x00000008
/*! \brief BT Channel 0A output pin2 */
#define IO_BT_CH0AOUTPIN2		0x0000000c
/*! \brief BT Channel 0B input pin0*/
#define IO_BT_CH0BINPIN0		0x00000010	
/*! \brief BT Channel 0B input pin1 */
#define IO_BT_CH0BINPIN1		0x00000020
/*! \brief BT Channel 0B input pin2 */
#define IO_BT_CH0BINPIN2		0x00000030
/*! \brief BT Channel 1A input pin0*/
#define IO_BT_CH1AINPIN0		0x00000100
/*! \brief BT Channel 1A input pin1 */
#define IO_BT_CH1AINPIN1		0x00000200
/*! \brief BT Channel 1A input pin2 */
#define IO_BT_CH1AINPIN2		0x00000300
/*! \brief BT Channel 1A no output */
#define IO_BT_CH1ANOOUTPUT		0x00000000
/*! \brief BT Channel 1A output pin0  */
#define IO_BT_CH1AOUTPIN0		0x00000400
/*! \brief BT Channel 1A output pin1 */
#define IO_BT_CH1AOUTPIN1		0x00000800
/*! \brief BT Channel 1A output pin2 */
#define IO_BT_CH1AOUTPIN2		0x00000c00
/*! \brief BT Channel 1B input pin0 */
#define IO_BT_CH1BINPIN0		0x00001000
/*! \brief BT Channel 1B input pin1 */
#define IO_BT_CH1BINPIN1		0x00002000
/*! \brief BT Channel 1B input pin2 */
#define IO_BT_CH1BINPIN2		0x00003000
/*! \brief BT Channel 2A no output */
#define IO_BT_CH2ANOOUTPUT		0x00000000
/*! \brief BT Channel 2A output pin0 */
#define IO_BT_CH2AOUTPIN0		0x00040000
/*! \brief BT Channel 2A output pin1 */
#define IO_BT_CH2AOUTPIN1		0x00080000
/*! \brief BT Channel 2A output pin2 */
#define IO_BT_CH2AOUTPIN2		0x000c0000
/*! \brief BT Channel 2B input pin0 */
#define IO_BT_CH2BINPIN0		0x00100000
/*! \brief BT Channel 2B input pin1 */
#define IO_BT_CH2BINPIN1		0x00200000
/*! \brief BT Channel 2B input pin2 */
#define IO_BT_CH2BINPIN2		0x00300000
/*! \brief BT Channel 3A input pin0 */
#define IO_BT_CH3AINPIN0		0x01000000
/*! \brief BT Channel 3A input pin1 */
#define IO_BT_CH3AINPIN1		0x02000000
/*! \brief BT Channel 3A input pin2 */
#define IO_BT_CH3AINPIN2		0x03000000
/*! \brief BT Channel 3A no output */
#define IO_BT_CH3ANOOUTPUT		0x00000000
/*! \brief BT Channel 3A output pin0 */
#define IO_BT_CH3AOUTPIN0		0x04000000
/*! \brief BT Channel 3A output pin1 */
#define IO_BT_CH3AOUTPIN1		0x08000000
/*! \brief BT Channel 3A output pin2 */
#define IO_BT_CH3AOUTPIN2		0x0c000000
/*! \brief BT Channel 3B input pin0 */
#define IO_BT_CH3BINPIN0		0x10000000
/*! \brief BT Channel 3B input pin1 */
#define IO_BT_CH3BINPIN1		0x20000000
/*! \brief BT Channel 3B input pin2 */
#define IO_BT_CH3BINPIN2		0x30000000
/*! \brief BT Channel 4A no output */
#define IO_BT_CH4ANOOUTPUT		0x00000000
/*! \brief BT Channel 4A output pin0 */
#define IO_BT_CH4AOUTPIN0		0x00000004
/*! \brief BT Channel 4A output pin1 */
#define IO_BT_CH4AOUTPIN1		0x00000008
/*! \brief BT Channel 4A output pin2 */
#define IO_BT_CH4AOUTPIN2		0x0000000c
/*! \brief BT Channel 4B input pin0 */
#define IO_BT_CH4BINPIN0		0x00000010
/*! \brief BT Channel 4B input pin1 */
#define IO_BT_CH4BINPIN1		0x00000020
/*! \brief BT Channel 4B input pin2 */
#define IO_BT_CH4BINPIN2		0x00000030
/*! \brief BT Channel 5A input pin0 */
#define IO_BT_CH5AINPIN0		0x00000100
/*! \brief BT Channel 5A input pin1 */
#define IO_BT_CH5AINPIN1		0x00000200
/*! \brief BT Channel 5A input pin2 */
#define IO_BT_CH5AINPIN2		0x00000300
/*! \brief BT Channel 5A no output */
#define IO_BT_CH5ANOOUTPUT		0x00000000
/*! \brief BT Channel 5A output pin0 */
#define IO_BT_CH5AOUTPIN0		0x00000400
/*! \brief BT Channel 5A output pin1 */
#define IO_BT_CH5AOUTPIN1		0x00000800
/*! \brief BT Channel 5A output pin2 */
#define IO_BT_CH5AOUTPIN2		0x00000c00
/*! \brief BT Channel 5B input pin0 */
#define IO_BT_CH5BINPIN0		0x00001000
/*! \brief BT Channel 5B input pin1 */
#define IO_BT_CH5BINPIN1		0x00002000
/*! \brief BT Channel 5B input pin2 */
#define IO_BT_CH5BINPIN2		0x00003000
/*! \brief BT Channel 6A output pin0 */
#define IO_BT_CH6ANOOUTPUT		0x00000000
/*! \brief BT Channel 6A output pin0 */
#define IO_BT_CH6AOUTPIN0		0x00040000
/*! \brief BT Channel 6A output pin1 */
#define IO_BT_CH6AOUTPIN1		0x00080000
/*! \brief BT Channel 6A output pin2 */
#define IO_BT_CH6AOUTPIN2		0x000c0000
/*! \brief BT Channel 6B input pin0 */
#define IO_BT_CH6BINPIN0		0x00100000
/*! \brief BT Channel 6B input pin1 */
#define IO_BT_CH6BINPIN1		0x00200000
/*! \brief BT Channel 6B input pin2 */
#define IO_BT_CH6BINPIN2		0x00300000
/*! \brief BT Channel 7A input pin0 */
#define IO_BT_CH7AINPIN0		0x01000000
/*! \brief BT Channel 7A input pin1 */
#define IO_BT_CH7AINPIN1		0x02000000
/*! \brief BT Channel 7A input pin2 */
#define IO_BT_CH7AINPIN2		0x03000000
/*! \brief BT Channel 7A no output */
#define IO_BT_CH7ANOOUTPUT		0x00000000
/*! \brief BT Channel 7A output pin0 */
#define IO_BT_CH7AOUTPIN0		0x04000000
/*! \brief BT Channel 7A output pin1 */
#define IO_BT_CH7AOUTPIN1		0x08000000
/*! \brief BT Channel 07A output pin2 */
#define IO_BT_CH7AOUTPIN2		0x0c000000
/*! \brief BT Channel 7B input pin0 */
#define IO_BT_CH7BINPIN0		0x10000000
/*! \brief BT Channel 7B input pin1 */
#define IO_BT_CH7BINPIN1		0x20000000
/*! \brief BT Channel 7B input pin2 */
#define IO_BT_CH7BINPIN2		0x30000000

/*! \brief BT Pin Check parameter */
#define IS_IO_BTPIN(mode)                   ((mode == IO_BT_CH0ANOOUTPUT) \
												  ||(mode ==IO_BT_CH0AOUTPIN0 ) \
												  ||(mode ==IO_BT_CH0AOUTPIN1 ) \
												  ||(mode ==IO_BT_CH0AOUTPIN2 ) \
												  ||(mode ==IO_BT_CH0BINPIN0 ) \
												  ||(mode ==IO_BT_CH0BINPIN1 ) \
												  ||(mode ==IO_BT_CH0BINPIN2 ) \
												  ||(mode ==IO_BT_CH1AINPIN0 ) \
												  ||(mode ==IO_BT_CH1AINPIN1 ) \
												  ||(mode ==IO_BT_CH1AINPIN2 ) \
												  ||(mode ==IO_BT_CH1ANOOUTPUT ) \
												  ||(mode ==IO_BT_CH1AOUTPIN0 ) \
												  ||(mode ==IO_BT_CH1AOUTPIN1 ) \
												  ||(mode ==IO_BT_CH1AOUTPIN2 ) \
												  ||(mode ==IO_BT_CH1BINPIN0 ) \
                                                  ||(mode ==IO_BT_CH1BINPIN1 ) \
                                                  ||(mode ==IO_BT_CH1BINPIN2 ) \
												  ||(mode ==IO_BT_CH2ANOOUTPUT ) \
												  ||(mode ==IO_BT_CH2AOUTPIN0 ) \
												  ||(mode ==IO_BT_CH2AOUTPIN1 ) \
												  ||(mode ==IO_BT_CH2AOUTPIN2 ) \
												  ||(mode ==IO_BT_CH2BINPIN0 ) \
												  ||(mode ==IO_BT_CH2BINPIN1 ) \
												  ||(mode ==IO_BT_CH2BINPIN2 ) \
												  ||(mode ==IO_BT_CH3AINPIN0 ) \
												  ||(mode ==IO_BT_CH3AINPIN1 ) \
												  ||(mode ==IO_BT_CH3AINPIN2 ) \
												  ||(mode ==IO_BT_CH3ANOOUTPUT ) \
												  ||(mode ==IO_BT_CH3AOUTPIN0 ) \
												  ||(mode ==IO_BT_CH3AOUTPIN1 ) \
                                                  ||(mode ==IO_BT_CH3AOUTPIN2 ) \
                                                  ||(mode ==IO_BT_CH3BINPIN0 ) \
												  ||(mode ==IO_BT_CH3BINPIN1 ) \
												  ||(mode ==IO_BT_CH3BINPIN2 ) \
												  ||(mode ==IO_BT_CH4ANOOUTPUT ) \
												  ||(mode ==IO_BT_CH4AOUTPIN0 ) \
												  ||(mode ==IO_BT_CH4AOUTPIN1 ) \
												  ||(mode ==IO_BT_CH4AOUTPIN2 ) \
												  ||(mode ==IO_BT_CH4BINPIN0 ) \
												  ||(mode ==IO_BT_CH4BINPIN1 ) \
												  ||(mode ==IO_BT_CH4BINPIN2 ) \
												  ||(mode ==IO_BT_CH5AINPIN0 ) \
												  ||(mode ==IO_BT_CH5AINPIN1 ) \
												  ||(mode ==IO_BT_CH5AINPIN2 ) \
												  ||(mode ==IO_BT_CH5ANOOUTPUT ) \
                                                  ||(mode ==IO_BT_CH5AOUTPIN0 ) \
                                                  ||(mode ==IO_BT_CH5AOUTPIN1 ) \
												  ||(mode ==IO_BT_CH5AOUTPIN2 ) \
												  ||(mode ==IO_BT_CH5BINPIN0 ) \
												  ||(mode ==IO_BT_CH5BINPIN1 ) \
												  ||(mode ==IO_BT_CH5BINPIN2 ) \
												  ||(mode ==IO_BT_CH6ANOOUTPUT ) \
												  ||(mode ==IO_BT_CH6AOUTPIN0 ) \
												  ||(mode ==IO_BT_CH6AOUTPIN1 ) \
												  ||(mode ==IO_BT_CH6AOUTPIN2 ) \
												  ||(mode ==IO_BT_CH6BINPIN0 ) \
												  ||(mode ==IO_BT_CH6BINPIN1 ) \
												  ||(mode ==IO_BT_CH6BINPIN2 ) \
												  ||(mode ==IO_BT_CH7AINPIN0 ) \
												  ||(mode ==IO_BT_CH7AINPIN1 ) \
                                                  ||(mode ==IO_BT_CH7AINPIN2 ) \
                                                  ||(mode ==IO_BT_CH7ANOOUTPUT ) \
												  ||(mode ==IO_BT_CH7AOUTPIN0 ) \
												  ||(mode ==IO_BT_CH7AOUTPIN1 ) \
												  ||(mode ==IO_BT_CH7AOUTPIN2 ) \
												  ||(mode ==IO_BT_CH7BINPIN0 ) \
												  ||(mode ==IO_BT_CH7BINPIN1 )) 




       
/*! \brief AD trigger PIN 0 */
#define IO_ADTRG_PIN0      0x00000001  
/*! \brief AD trigger PIN 1 */
#define IO_ADTRG_PIN1      0x00000002 
/*! \brief AD trigger PIN 2 */
#define IO_ADTRG_PIN2      0x00000003 
/*! \brief AD trigger PIN 3 */
#define IO_ADTRG_PIN3      0x00000004  
/*! \brief AD trigger PIN 4 */
#define IO_ADTRG_PIN4      0x00000005 
/*! \brief AD trigger PIN 5 */
#define IO_ADTRG_PIN5      0x00000006 
/*! \brief AD trigger PIN 6 */
#define IO_ADTRG_PIN6      0x00000007  
/*! \brief AD trigger PIN 7 */
#define IO_ADTRG_PIN7      0x00000008 
/*! \brief AD trigger PIN 8 */
#define IO_ADTRG_PIN8      0x00000009 
/*! \brief Check parameter */
#define IS_IO_ADTRGPIN(mode)                   ((mode == IO_ADTRG_PIN0) \
												  ||(mode == IO_ADTRG_PIN1) \
												  ||(mode == IO_ADTRG_PIN2) \
												  ||(mode == IO_ADTRG_PIN3) \
												  ||(mode == IO_ADTRG_PIN4) \
												  ||(mode == IO_ADTRG_PIN5) \
												  ||(mode == IO_ADTRG_PIN6) \
												  ||(mode == IO_ADTRG_PIN7) \
                                                  ||(mode == IO_ADTRG_PIN8))



/*! \brief AD unit 0 */
#define IO_AD_UNIT0      0x00000000  
/*! \brief AD unit 0 */
#define IO_AD_UNIT1      0x00000001 
/*! \brief AD unit 0 */
#define IO_AD_UNIT2      0x00000002 
/*! \brief Check parameter */
#define IS_IO_ADUNIT(mode)                   ((mode == IO_AD_UNIT0) \
												  ||(mode == IO_AD_UNIT1) \
                                                  ||(mode == IO_AD_UNIT2))


/*! \brief CAN channel 0 */
#define IO_CAN_CH0      0x00000000  
/*! \brief CAN channel 1 */
#define IO_CAN_CH1      0x00000001 
/*! \brief Check parameter */
#define IS_IO_CANCH(mode)                   ((mode == IO_CAN_CH0) \
												  ||(mode == IO_CAN_CH1))

/*! \brief NO CAN pin  */
#define IO_CAN_NOPIN     0x00000000 
/*! \brief CAN pin 0 */
#define IO_CAN_PIN0      0x00000001  
/*! \brief CAN pin 1 */
#define IO_CAN_PIN1      0x00000002 
/*! \brief CAN pin 2 */
#define IO_CAN_PIN2      0x00000003 
/*! \brief Check parameter */
#define IS_IO_CANPIN(mode)                   ((mode == IO_CAN_NOPIN) \
	                                              ||(mode == IO_CAN_PIN0) \
												  ||(mode == IO_CAN_PIN1) \
                                                  ||(mode == IO_CAN_PIN2))






/*! \brief QPRC channel 0A */
#define IO_QPRC_CH0A     0x00000000  
/*! \brief QPRC channel 0B */
#define IO_QPRC_CH0B     0x00000001 
/*! \brief QPRC channel 0Z */
#define IO_QPRC_CH0Z     0x00000002  
/*! \brief QPRC channel 1A */
#define IO_QPRC_CH1A     0x00000003
/*! \brief QPRC channel 1B */
#define IO_QPRC_CH1B     0x00000004  
/*! \brief QPRC channel 1Z */
#define IO_QPRC_CH1Z     0x00000005
/*! \brief Check parameter */
#define IS_IO_QPRCCH(mode)                   ((mode == IO_QPRC_CH0A) \
	                                              ||(mode == IO_QPRC_CH0B) \
	                                              ||(mode == IO_QPRC_CH0Z) \
	                                              ||(mode == IO_QPRC_CH1A) \
	                                              ||(mode == IO_QPRC_CH1B) \
												  ||(mode == IO_QPRC_CH1Z))



/*! \brief QPRC pin 0 */
#define IO_QPRC_PIN0      0x00000001  
/*! \brief QPRC pin 1 */
#define IO_QPRC_PIN1      0x00000002 
/*! \brief QPRCpin 2 */
#define IO_QPRC_PIN2      0x00000003 
/*! \brief Check parameter */
#define IS_IO_QPRCPIN(mode)                   ((mode == IO_QPRC_PIN0) \
	                                              ||(mode == IO_QPRC_PIN1) \
                                                  ||(mode == IO_QPRC_PIN2))



/*! \brief MFS channel 0 */
#define IO_MFS_CH0     0x00000000  
/*! \brief MFS channel 1 */
#define IO_MFS_CH1     0x00000001 
/*! \brief MFS channel 2 */
#define IO_MFS_CH2     0x00000002  
/*! \brief MFS channel 3 */
#define IO_MFS_CH3     0x00000003
/*! \brief MFS channel 4 */
#define IO_MFS_CH4     0x00000004  
/*! \brief MFS channel 5 */
#define IO_MFS_CH5     0x00000005
/*! \brief MFS channel 6 */
#define IO_MFS_CH6     0x00000006
/*! \brief MFS channel 7 */
#define IO_MFS_CH7     0x00000007
/*! \brief Check parameter */
#define IS_IO_MFSCH(mode)                   ((mode == IO_MFS_CH0) \
	                                              ||(mode == IO_MFS_CH1) \
	                                              ||(mode == IO_MFS_CH2) \
	                                              ||(mode == IO_MFS_CH3) \
	                                              ||(mode == IO_MFS_CH4) \
	                                              ||(mode == IO_MFS_CH5) \
	                                              ||(mode == IO_MFS_CH6) \
												  ||(mode == IO_MFS_CH7))



/*! \brief mfs channel 0 sin pin0 sot no output sck no output */
#define IO_MFSCH0SIN0NOSOTNOSCK   	0x00000000
/*! \brief mfs channel 0 sin pin0 sot no output sck pin0 */
#define IO_MFSCH0SIN0NOSOTSCK0		0x00000100
/*! \brief mfs channel 0 sin pin0 sot no output sck pin1 */
#define IO_MFSCH0SIN0NOSOTSCK1		0x00000200	
/*! \brief mfs channel 0 sin pin0 sot no output sck pin2 */
#define IO_MFSCH0SIN0NOSOTSCK2		0x00000300
/*! \brief mfs channel 0 sin pin0 sot pin0 sck no output */
#define IO_MFSCH0SIN0SOT0NOSCK		0x00000040
/*! \brief mfs channel 0 sin pin0 sot pin0  sck pin0 */
#define IO_MFSCH0SIN0SOT0SCK0		0x00000140	
/*! \brief mfs channel 0 sin pin0 sot pin0  sck pin1 */
#define IO_MFSCH0SIN0SOT0SCK1		0x00000240
/*! \brief mfs channel 0 sin pin0 sot pin0  sck pin2 */
#define IO_MFSCH0SIN0SOT0SCK2		0x00000340
/*! \brief mfs channel 0 sin pin0 sot pin1 sck no output */
#define IO_MFSCH0SIN0SOT1NOSCK		0x00000080
/*! \brief mfs channel 0 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH0SIN0SOT1SCK0		0x00000180
/*! \brief mfs channel 0 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH0SIN0SOT1SCK1		0x00000280
/*! \brief mfs channel 0 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH0SIN0SOT1SCK2		0x00000380
/*! \brief mfs channel 0 sin pin0 sot pin2 sck no output */
#define IO_MFSCH0SIN0SOT2NOSCK		0x000000c0	
/*! \brief mfs channel 0 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH0SIN0SOT2SCK0		0x000001c0
/*! \brief mfs channel 0 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH0SIN0SOT2SCK1		0x000002c0
/*! \brief mfs channel 0 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH0SIN0SOT2SCK2		0x000003c0
/*! \brief mfs channel 0 sin pin1 sot no output sck no output */
#define IO_MFSCH0SIN1NOSOTNOSCK		0x00000020
/*! \brief mfs channel 0 sin pin1 sot no output sck pin0 */
#define IO_MFSCH0SIN1NOSOTSCK0		0x00000120
/*! \brief mfs channel 0 sin pin1 sot no output sck pin1 */
#define IO_MFSCH0SIN1NOSOTSCK1		0x00000220
/*! \brief mfs channel 0 sin pin1 sot no output sck pin2 */
#define IO_MFSCH0SIN1NOSOTSCK2		0x00000320
/*! \brief mfs channel 0 sin pin1 sot pin0 sck no output */
#define IO_MFSCH0SIN1SOT0NOSCK		0x00000060
/*! \brief mfs channel 0 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH0SIN1SOT0SCK0		0x00000160
/*! \brief mfs channel 0 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH0SIN1SOT0SCK1		0x00000260
/*! \brief mfs channel 0 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH0SIN1SOT0SCK2		0x00000360
/*! \brief mfs channel 0 sin pin1 sot pin1 sck no output */
#define IO_MFSCH0SIN1SOT1NOSCK		0x000000a0
/*! \brief mfs channel 0 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH0SIN1SOT1SCK0		0x000001a0
/*! \brief mfs channel 0 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH0SIN1SOT1SCK1		0x000002a0
/*! \brief mfs channel 0 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH0SIN1SOT1SCK2		0x000003a0
/*! \brief mfs channel 0 sin pin1 sot pin2 sck no output */
#define IO_MFSCH0SIN1SOT2NOSCK		0x000000e0
/*! \brief mfs channel 0 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH0SIN1SOT2SCK0		0x000001e0
/*! \brief mfs channel 0 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH0SIN1SOT2SCK1		0x000002e0
/*! \brief mfs channel 0 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH0SIN1SOT2SCK2		0x000003e0
/*! \brief mfs channel 0 sin pin2 sot no output sck no output */
#define IO_MFSCH0SIN2NOSOTNOSCK		0x00000030
/*! \brief mfs channel 0 sin pin2 sot no output sck pin0 */
#define IO_MFSCH0SIN2NOSOTSCK0		0x00000130
/*! \brief mfs channel 0 sin pin2 sot no output sck pin1 */
#define IO_MFSCH0SIN2NOSOTSCK1		0x00000230
/*! \brief mfs channel 0 sin pin2 sot no output sck pin2 */
#define IO_MFSCH0SIN2NOSOTSCK2		0x00000330
/*! \brief mfs channel 0 sin pin2 sot pin0 sck no output */
#define IO_MFSCH0SIN2SOT0NOSCK		0x00000070
/*! \brief mfs channel 0 sin pin2 sot pin0 sck pin0 */
#define IO_MFSCH0SIN2SOT0SCK0		0x00000170
/*! \brief mfs channel 0 sin pin2 sot pin0 sck pin1 */
#define IO_MFSCH0SIN2SOT0SCK1		0x00000270
/*! \brief mfs channel 0 sin pin2 sot pin0 sck pin2 */
#define IO_MFSCH0SIN2SOT0SCK2		0x00000370
/*! \brief mfs channel 0 sin pin2 sot pin1 sck no output */
#define IO_MFSCH0SIN2SOT1NOSCK		0x000000b0
/*! \brief mfs channel 0 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH0SIN2SOT1SCK0		0x000001b0
/*! \brief mfs channel 0 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH0SIN2SOT1SCK1		0x000002b0
/*! \brief mfs channel 0 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH0SIN2SOT1SCK2		0x000003b0
/*! \brief mfs channel 0 sin pin2 sot pin2 sck no output */
#define IO_MFSCH0SIN2SOT2NOSCK		0x000000f0
/*! \brief mfs channel 0 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH0SIN2SOT2SCK0		0x000001f0
/*! \brief mfs channel 0 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH0SIN2SOT2SCK1		0x000002f0
/*! \brief mfs channel 0 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH0SIN2SOT2SCK2		0x000003f0
/*! \brief mfs channel 1 sin pin0 sot no output sck no output */
#define IO_MFSCH1SIN0NOSOTNOSCK		0x00000000
/*! \brief mfs channel 1 sin pin0 sot no output sck pin0 */
#define IO_MFSCH1SIN0NOSOTSCK0		0x00004000
/*! \brief mfs channel 1 sin pin0 sot no output sck pin1 */
#define IO_MFSCH1SIN0NOSOTSCK1		0x00008000
/*! \brief mfs channel 1 sin pin0 sot no output sck pin2 */
#define IO_MFSCH1SIN0NOSOTSCK2		0x0000c000
/*! \brief mfs channel 1 sin pin0 sot pin0 sck no output */
#define IO_MFSCH1SIN0SOT0NOSCK		0x00001000
/*! \brief mfs channel 1 sin pin0 sot pin0 sck pin0 */
#define IO_MFSCH1SIN0SOT0SCK0		0x00005000
/*! \brief mfs channel 1 sin pin0 sot pin0 sck pin1 */
#define IO_MFSCH1SIN0SOT0SCK1		0x00009000
/*! \brief mfs channel 1 sin pin0 sot pin0 sck pin2 */
#define IO_MFSCH1SIN0SOT0SCK2		0x0000d000	
/*! \brief mfs channel 1 sin pin0 sot pin1 sck no output */
#define IO_MFSCH1SIN0SOT1NOSCK		0x00002000
/*! \brief mfs channel 1 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH1SIN0SOT1SCK0		0x00006000
/*! \brief mfs channel 1 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH1SIN0SOT1SCK1		0x0000a000
/*! \brief mfs channel 1 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH1SIN0SOT1SCK2		0x0000e000
/*! \brief mfs channel 1 sin pin0 sot pin2 sck no output */
#define IO_MFSCH1SIN0SOT2NOSCK		0x00003000
/*! \brief mfs channel 1 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH1SIN0SOT2SCK0		0x00007000
/*! \brief mfs channel 1 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH1SIN0SOT2SCK1		0x0000b000
/*! \brief mfs channel 1 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH1SIN0SOT2SCK2		0x0000f000
/*! \brief mfs channel 1 sin pin1 sot no output sck no output */
#define IO_MFSCH1SIN1NOSOTNOSCK		0x00000800	
/*! \brief mfs channel 1 sin pin1 sot no output sck pin0 */
#define IO_MFSCH1SIN1NOSOTSCK0		0x00004800		
/*! \brief mfs channel 1 sin pin1 sot no output sck pin1 */
#define IO_MFSCH1SIN1NOSOTSCK1		0x00008800
/*! \brief mfs channel 1 sin pin1 sot no output sck pin2 */
#define IO_MFSCH1SIN1NOSOTSCK2		0x0000c800
/*! \brief mfs channel 1 sin pin1 sot pin0 sck no output */
#define IO_MFSCH1SIN1SOT0NOSCK		0x00001800
/*! \brief mfs channel 1 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH1SIN1SOT0SCK0		0x00005800
/*! \brief mfs channel 1 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH1SIN1SOT0SCK1		0x00009800
/*! \brief mfs channel 1 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH1SIN1SOT0SCK2		0x0000d800
/*! \brief mfs channel 1 sin pin1 sot pin1 sck no output */
#define IO_MFSCH1SIN1SOT1NOSCK		0x00002800
/*! \brief mfs channel 1 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH1SIN1SOT1SCK0		0x00006800
/*! \brief mfs channel 1 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH1SIN1SOT1SCK1		0x0000a800
/*! \brief mfs channel 1 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH1SIN1SOT1SCK2		0x0000e800
/*! \brief mfs channel 1 sin pin1 sot pin2 sck no output */
#define IO_MFSCH1SIN1SOT2NOSCK		0x00003800
/*! \brief mfs channel 1 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH1SIN1SOT2SCK0		0x00007800
/*! \brief mfs channel 1 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH1SIN1SOT2SCK1		0x0000b800
/*! \brief mfs channel 1 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH1SIN1SOT2SCK2		0x0000f800
/*! \brief mfs channel 1 sin pin2 sot no output sck no output */
#define IO_MFSCH1SIN2NOSOTNOSCK		0x00000c00
/*! \brief mfs channel 1 sin pin2 sot no output sck pin0 */
#define IO_MFSCH1SIN2NOSOTSCK0		0x00004c00
/*! \brief mfs channel 1 sin pin2 sot no output sck pin1 */
#define IO_MFSCH1SIN2NOSOTSCK1		0x00008c00
/*! \brief mfs channel 1 sin pin2 sot no output sck pin2 */
#define IO_MFSCH1SIN2NOSOTSCK2		0x0000cc00
/*! \brief mfs channel 1 sin pin2 sot pin0 sck no output */
#define IO_MFSCH1SIN2SOT0NOSCK		0x00001c00
/*! \brief mfs channel 1 sin pin2 sot pin0 sck pin0 */
#define IO_MFSCH1SIN2SOT0SCK0		0x00005c00
/*! \brief mfs channel 1 sin pin2 sot pin0 sck pin1 */
#define IO_MFSCH1SIN2SOT0SCK1		0x00009c00
/*! \brief mfs channel 1 sin pin2 sot pin0 sck pin2 */
#define IO_MFSCH1SIN2SOT0SCK2		0x0000dc00
/*! \brief mfs channel 1 sin pin2 sot pin1 sck no output */
#define IO_MFSCH1SIN2SOT1NOSCK		0x00002c00
/*! \brief mfs channel 1 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH1SIN2SOT1SCK0		0x00006c00
/*! \brief mfs channel 1 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH1SIN2SOT1SCK1		0x0000ac00
/*! \brief mfs channel 1 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH1SIN2SOT1SCK2		0x0000ec00
/*! \brief mfs channel 1 sin pin2 sot pin2 sck no output */
#define IO_MFSCH1SIN2SOT2NOSCK		0x00003c00
/*! \brief mfs channel 1 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH1SIN2SOT2SCK0		0x00007c00
/*! \brief mfs channel 1 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH1SIN2SOT2SCK1		0x0000bc00
/*! \brief mfs channel 1 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH1SIN2SOT2SCK2		0x0000fc00
/*! \brief mfs channel 2 sin pin0 sot no output sck no output */
#define IO_MFSCH2SIN0NOSOTNOSCK		0x00000000
/*! \brief mfs channel 2 sin pin0 sot no output sck pin0 */
#define IO_MFSCH2SIN0NOSOTSCK0		0x00100000
/*! \brief mfs channel 2 sin pin0 sot no output sck pin1 */
#define IO_MFSCH2SIN0NOSOTSCK1		0x00200000
/*! \brief mfs channel 2 sin pin0 sot no output sck pin2 */
#define IO_MFSCH2SIN0NOSOTSCK2		0x00300000
/*! \brief mfs channel 2 sin pin0 sot pin0 sck no output */
#define IO_MFSCH2SIN0SOT0NOSCK		0x00040000
/*! \brief mfs channel 2 sin pin0 sot pin0 sck pin0 */
#define IO_MFSCH2SIN0SOT0SCK0		0x00140000	
/*! \brief mfs channel 2 sin pin0 sot pin0 sck pin1 */
#define IO_MFSCH2SIN0SOT0SCK1		0x00240000
/*! \brief mfs channel 2 sin pin0 sot pin0 sck pin2 */
#define IO_MFSCH2SIN0SOT0SCK2		0x00340000
/*! \brief mfs channel 2 sin pin0 sot pin1 sck no output */
#define IO_MFSCH2SIN0SOT1NOSCK		0x00080000
/*! \brief mfs channel 2 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH2SIN0SOT1SCK0		0x00180000
/*! \brief mfs channel 2 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH2SIN0SOT1SCK1		0x00280000
/*! \brief mfs channel 2 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH2SIN0SOT1SCK2		0x00380000
/*! \brief mfs channel 2 sin pin0 sot pin2 sck no output */
#define IO_MFSCH2SIN0SOT2NOSCK		0x000c0000
/*! \brief mfs channel 2 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH2SIN0SOT2SCK0		0x001c0000
/*! \brief mfs channel 2 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH2SIN0SOT2SCK1		0x002c0000
/*! \brief mfs channel 2 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH2SIN0SOT2SCK2		0x003c0000
/*! \brief mfs channel 2 sin pin1 sot no output sck no output */
#define IO_MFSCH2SIN1NOSOTNOSCK		0x00020000
/*! \brief mfs channel 2 sin pin1 sot no output sck pin0 */
#define IO_MFSCH2SIN1NOSOTSCK0		0x00120000
/*! \brief mfs channel 2 sin pin1 sot no output sck pin1 */
#define IO_MFSCH2SIN1NOSOTSCK1		0x00220000
/*! \brief mfs channel 2 sin pin1 sot no output sck pin2 */
#define IO_MFSCH2SIN1NOSOTSCK2		0x00320000
/*! \brief mfs channel 2 sin pin1 sot pin0 sck no output */
#define IO_MFSCH2SIN1SOT0NOSCK		0x00060000
/*! \brief mfs channel 2 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH2SIN1SOT0SCK0		0x00160000
/*! \brief mfs channel 2 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH2SIN1SOT0SCK1		0x00260000
/*! \brief mfs channel 2 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH2SIN1SOT0SCK2		0x00360000
/*! \brief mfs channel 2 sin pin1 sot pin1 sck no output */
#define IO_MFSCH2SIN1SOT1NOSCK		0x000a0000
/*! \brief mfs channel 2 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH2SIN1SOT1SCK0		0x001a0000
/*! \brief mfs channel 2 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH2SIN1SOT1SCK1		0x002a0000
/*! \brief mfs channel 2 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH2SIN1SOT1SCK2		0x003a0000
/*! \brief mfs channel 2 sin pin1 sot pin2 sck no output */
#define IO_MFSCH2SIN1SOT2NOSCK		0x000e0000
/*! \brief mfs channel 2 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH2SIN1SOT2SCK0		0x001e0000
/*! \brief mfs channel 2 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH2SIN1SOT2SCK1		0x002e0000
/*! \brief mfs channel 2 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH2SIN1SOT2SCK2		0x003e0000
/*! \brief mfs channel 2 sin pin2 sot no output sck no output */
#define IO_MFSCH2SIN2NOSOTNOSCK		0x00030000
/*! \brief mfs channel 2 sin pin2 sot no output sck pin0 */
#define IO_MFSCH2SIN2NOSOTSCK0		0x00130000
/*! \brief mfs channel 2 sin pin2 sot no output sck pin1 */
#define IO_MFSCH2SIN2NOSOTSCK1		0x00230000
/*! \brief mfs channel 2 sin pin2 sot no output sck pin2 */
#define IO_MFSCH2SIN2NOSOTSCK2		0x00330000
/*! \brief mfs channel 2 sin pin2 sot pin0 sck no output */
#define IO_MFSCH2SIN2SOT0NOSCK		0x00070000
/*! \brief mfs channel 2 sin pin2 sot pin0 sck pin0 */
#define IO_MFSCH2SIN2SOT0SCK0		0x00170000
/*! \brief mfs channel 2 sin pin2 sot pin0 sck pin1 */
#define IO_MFSCH2SIN2SOT0SCK1		0x00270000
/*! \brief mfs channel 2 sin pin2 sot pin0 sck pin2 */
#define IO_MFSCH2SIN2SOT0SCK2		0x00370000
/*! \brief mfs channel 2 sin pin2 sot pin1 sck no output */
#define IO_MFSCH2SIN2SOT1NOSCK		0x000b0000
/*! \brief mfs channel 2 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH2SIN2SOT1SCK0		0x001b0000
/*! \brief mfs channel 2 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH2SIN2SOT1SCK1		0x002b0000
/*! \brief mfs channel 2 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH2SIN2SOT1SCK2		0x003b0000
/*! \brief mfs channel 2 sin pin2 sot pin2 sck no output */
#define IO_MFSCH2SIN2SOT2NOSCK		0x000f0000
/*! \brief mfs channel 2 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH2SIN2SOT2SCK0		0x001f0000
/*! \brief mfs channel 2 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH2SIN2SOT2SCK1		0x002f0000
/*! \brief mfs channel 2 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH2SIN2SOT2SCK2		0x003f0000
/*! \brief mfs channel 3 sin pin0 sot no output sck no output */
#define IO_MFSCH3SIN0NOSOTNOSCK		0x00000000
/*! \brief mfs channel 3 sin pin0 sot no output sck pin0 */
#define IO_MFSCH3SIN0NOSOTSCK0		0x04000000
/*! \brief mfs channel 3 sin pin0 sot no output sck pin1 */
#define IO_MFSCH3SIN0NOSOTSCK1		0x08000000
/*! \brief mfs channel 3 sin pin0 sot no output sck pin2 */
#define IO_MFSCH3SIN0NOSOTSCK2		0x0c000000
/*! \brief mfs channel 3 sin pin0 sot pin0 sck no output */
#define IO_MFSCH3SIN0SOT0NOSCK		0x01000000
/*! \brief mfs channel 3 sin pin0 sot pin0 sck pin0 */
#define IO_MFSCH3SIN0SOT0SCK0		0x05000000
/*! \brief mfs channel 3 sin pin0 sot pin0 sck pin1 */
#define IO_MFSCH3SIN0SOT0SCK1		0x09000000
/*! \brief mfs channel 3 sin pin0 sot pin0 sck pin2 */
#define IO_MFSCH3SIN0SOT0SCK2		0x0d000000
/*! \brief mfs channel 3 sin pin0 sot pin1 sck no output */
#define IO_MFSCH3SIN0SOT1NOSCK		0x02000000
/*! \brief mfs channel 3 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH3SIN0SOT1SCK0		0x06000000
/*! \brief mfs channel 3 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH3SIN0SOT1SCK1		0x0a000000
/*! \brief mfs channel 3 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH3SIN0SOT1SCK2		0x0e000000
/*! \brief mfs channel 3 sin pin0 sot pin2 sck no output */
#define IO_MFSCH3SIN0SOT2NOSCK		0x03000000
/*! \brief mfs channel 3 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH3SIN0SOT2SCK0		0x07000000
/*! \brief mfs channel 3 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH3SIN0SOT2SCK1		0x0b000000
/*! \brief mfs channel 3 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH3SIN0SOT2SCK2		0x0f000000
/*! \brief mfs channel 3 sin pin1 sot no output sck no output */
#define IO_MFSCH3SIN1NOSOTNOSCK		0x00800000
/*! \brief mfs channel 3 sin pin1 sot no output sck pin0 */
#define IO_MFSCH3SIN1NOSOTSCK0		0x04800000
/*! \brief mfs channel 3 sin pin1 sot no output sck pin1 */
#define IO_MFSCH3SIN1NOSOTSCK1		0x08800000
/*! \brief mfs channel 3 sin pin1 sot no output sck pin2 */
#define IO_MFSCH3SIN1NOSOTSCK2		0x0c800000
/*! \brief mfs channel 3 sin pin1 sot pin0 sck no output */
#define IO_MFSCH3SIN1SOT0NOSCK		0x01800000
/*! \brief mfs channel 3 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH3SIN1SOT0SCK0		0x05800000
/*! \brief mfs channel 3 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH3SIN1SOT0SCK1		0x09800000
/*! \brief mfs channel 3 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH3SIN1SOT0SCK2		0x0d800000
/*! \brief mfs channel 3 sin pin1 sot pin1 sck no output */
#define IO_MFSCH3SIN1SOT1NOSCK		0x02800000
/*! \brief mfs channel 3 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH3SIN1SOT1SCK0		0x06800000
/*! \brief mfs channel 3 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH3SIN1SOT1SCK1		0x0a800000
/*! \brief mfs channel 3 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH3SIN1SOT1SCK2		0x0e800000
/*! \brief mfs channel 3 sin pin1 sot pin2 sck no output */
#define IO_MFSCH3SIN1SOT2NOSCK		0x03800000
/*! \brief mfs channel 3 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH3SIN1SOT2SCK0		0x07800000
/*! \brief mfs channel 3 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH3SIN1SOT2SCK1		0x0b800000
/*! \brief mfs channel 3 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH3SIN1SOT2SCK2		0x0f800000
/*! \brief mfs channel 3 sin pin2 sot no output sck no output */
#define IO_MFSCH3SIN2NOSOTNOSCK		0x00c00000
/*! \brief mfs channel 3 sin pin2 sot no output sck pin0 */
#define IO_MFSCH3SIN2NOSOTSCK0		0x04c00000
/*! \brief mfs channel 3 sin pin2 sot no output sck pin1 */
#define IO_MFSCH3SIN2NOSOTSCK1		0x08c00000
/*! \brief mfs channel 3 sin pin2 sot no output sck pin2 */
#define IO_MFSCH3SIN2NOSOTSCK2		0x0cc00000
/*! \brief mfs channel 3 sin pin2 sot pin0 sck no output */
#define IO_MFSCH3SIN2SOT0NOSCK		0x01c00000
/*! \brief mfs channel 3 sin pin2 sot pin0 sck no pin0 */
#define IO_MFSCH3SIN2SOT0SCK0		0x05c00000
/*! \brief mfs channel 3 sin pin2 sot pin0 sck no pin1 */
#define IO_MFSCH3SIN2SOT0SCK1		0x09c00000
/*! \brief mfs channel 3 sin pin2 sot pin0 sck no pin2 */
#define IO_MFSCH3SIN2SOT0SCK2		0x0dc00000
/*! \brief mfs channel 3 sin pin2 sot pin1 sck no output */
#define IO_MFSCH3SIN2SOT1NOSCK		0x02c00000
/*! \brief mfs channel 3 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH3SIN2SOT1SCK0		0x06c00000
/*! \brief mfs channel 3 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH3SIN2SOT1SCK1		0x0ac00000
/*! \brief mfs channel 3 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH3SIN2SOT1SCK2		0x0ec00000
/*! \brief mfs channel 3 sin pin2 sot pin2 sck no output */
#define IO_MFSCH3SIN2SOT2NOSCK		0x03c00000
/*! \brief mfs channel 3 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH3SIN2SOT2SCK0		0x07c00000
/*! \brief mfs channel 3 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH3SIN2SOT2SCK1		0x0bc00000
/*! \brief mfs channel 3 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH3SIN2SOT2SCK2		0x0fc00000
/*! \brief mfs channel 4 sin pin0 sot no output sck no output */
#define IO_MFSCH4SIN0NOSOTNOSCK		0x00000000
/*! \brief mfs channel 4 sin pin0 sot no output sck pin0 */
#define IO_MFSCH4SIN0NOSOTSCK0		0x00000100
/*! \brief mfs channel 4 sin pin0 sot no output sck pin1 */
#define IO_MFSCH4SIN0NOSOTSCK1		0x00000200
/*! \brief mfs channel 4 sin pin0 sot no output sck pin2 */
#define IO_MFSCH4SIN0NOSOTSCK2		0x00000300
/*! \brief mfs channel 4 sin pin0 sot pin0 sck no output */
#define IO_MFSCH4SIN0SOT0NOSCK		0x00000040
/*! \brief mfs channel 4 sin pin0 sot pin0 sck pin0 */
#define IO_MFSCH4SIN0SOT0SCK0		0x00000140
/*! \brief mfs channel 4 sin pin0 sot pin0 sck pin1 */
#define IO_MFSCH4SIN0SOT0SCK1		0x00000240
/*! \brief mfs channel 4 sin pin0 sot pin0 sck pin2 */
#define IO_MFSCH4SIN0SOT0SCK2		0x00000340
/*! \brief mfs channel 4 sin pin0 sot pin1 sck no output */
#define IO_MFSCH4SIN0SOT1NOSCK		0x00000080
/*! \brief mfs channel 4 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH4SIN0SOT1SCK0		0x00000180
/*! \brief mfs channel 4 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH4SIN0SOT1SCK1		0x00000280
/*! \brief mfs channel 4 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH4SIN0SOT1SCK2		0x00000380
/*! \brief mfs channel 4 sin pin0 sot pin2 sck no output */
#define IO_MFSCH4SIN0SOT2NOSCK		0x000000c0
/*! \brief mfs channel 4 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH4SIN0SOT2SCK0		0x000001c0
/*! \brief mfs channel 4 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH4SIN0SOT2SCK1		0x000002c0
/*! \brief mfs channel 4 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH4SIN0SOT2SCK2		0x000003c0
/*! \brief mfs channel 4 sin pin1 sot no output sck no output */
#define IO_MFSCH4SIN1NOSOTNOSCK		0x00000020
/*! \brief mfs channel 4 sin pin1 sot no output sck pin0 */
#define IO_MFSCH4SIN1NOSOTSCK0		0x00000120
/*! \brief mfs channel 4 sin pin1 sot no output sck pin1 */
#define IO_MFSCH4SIN1NOSOTSCK1		0x00000220
/*! \brief mfs channel 4 sin pin1 sot no output sck pin2 */
#define IO_MFSCH4SIN1NOSOTSCK2		0x00000320
/*! \brief mfs channel 4 sin pin1 sot pin0 sck no output */
#define IO_MFSCH4SIN1SOT0NOSCK		0x00000060
/*! \brief mfs channel 4 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH4SIN1SOT0SCK0		0x00000160
/*! \brief mfs channel 4 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH4SIN1SOT0SCK1		0x00000260
/*! \brief mfs channel 4 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH4SIN1SOT0SCK2		0x00000360
/*! \brief mfs channel 4 sin pin1 sot pin1 sck no output */
#define IO_MFSCH4SIN1SOT1NOSCK		0x000000a0
/*! \brief mfs channel 4 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH4SIN1SOT1SCK0		0x000001a0
/*! \brief mfs channel 4 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH4SIN1SOT1SCK1		0x000002a0
/*! \brief mfs channel 4 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH4SIN1SOT1SCK2		0x000003a0
/*! \brief mfs channel 4 sin pin1 sot pin2 sck no output */
#define IO_MFSCH4SIN1SOT2NOSCK		0x000000e0
/*! \brief mfs channel 4 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH4SIN1SOT2SCK0		0x000001e0
/*! \brief mfs channel 4 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH4SIN1SOT2SCK1		0x000002e0
/*! \brief mfs channel 4 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH4SIN1SOT2SCK2		0x000003e0
/*! \brief mfs channel 4 sin pin2 sot no output sck no output */
#define IO_MFSCH4SIN2NOSOTNOSCK		0x00000030
/*! \brief mfs channel 4 sin pin2 sot no output sck pin0 */
#define IO_MFSCH4SIN2NOSOTSCK0		0x00000130
/*! \brief mfs channel 4 sin pin2 sot no output sck pin1 */
#define IO_MFSCH4SIN2NOSOTSCK1		0x00000230
/*! \brief mfs channel 4 sin pin2 sot no output sck pin2 */
#define IO_MFSCH4SIN2NOSOTSCK2		0x00000330
/*! \brief mfs channel 4 sin pin2 sot pin0 sck no output */
#define IO_MFSCH4SIN2SOT0NOSCK		0x00000070
/*! \brief mfs channel 4 sin pin2 sot pin0 sck pin0 */
#define IO_MFSCH4SIN2SOT0SCK0		0x00000170
/*! \brief mfs channel 4 sin pin2 sot pin0 sck pin1 */
#define IO_MFSCH4SIN2SOT0SCK1		0x00000270
/*! \brief mfs channel 4 sin pin2 sot pin0 sck pin2 */
#define IO_MFSCH4SIN2SOT0SCK2		0x00000370
/*! \brief mfs channel 4 sin pin2 sot pin1 sck no output */
#define IO_MFSCH4SIN2SOT1NOSCK		0x000000b0
/*! \brief mfs channel 4 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH4SIN2SOT1SCK0		0x000001b0
/*! \brief mfs channel 4 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH4SIN2SOT1SCK1		0x000002b0
/*! \brief mfs channel 4 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH4SIN2SOT1SCK2		0x000003b0
/*! \brief mfs channel 4 sin pin2 sot pin2 sck no output */
#define IO_MFSCH4SIN2SOT2NOSCK		0x000000f0
/*! \brief mfs channel 4 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH4SIN2SOT2SCK0		0x000001f0
/*! \brief mfs channel 4 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH4SIN2SOT2SCK1		0x000002f0
/*! \brief mfs channel 4 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH4SIN2SOT2SCK2		0x000003f0
/*! \brief mfs channel 5 sin pin0 sot no output sck no output */
#define IO_MFSCH5SIN0NOSOTNOSCK		0x00000000
/*! \brief mfs channel 5 sin pin0 sot no output sck pin0 */
#define IO_MFSCH5SIN0NOSOTSCK0		0x00004000
/*! \brief mfs channel 5 sin pin0 sot no output sck pin1 */
#define IO_MFSCH5SIN0NOSOTSCK1		0x00008000
/*! \brief mfs channel 5 sin pin0 sot no output sck pin2 */
#define IO_MFSCH5SIN0NOSOTSCK2		0x0000c000
/*! \brief mfs channel 5 sin pin0 sot pin0 sck no output */
#define IO_MFSCH5SIN0SOT0NOSCK		0x00001000
/*! \brief mfs channel 5 sin pin0 sot pin0 sck pin0 */
#define IO_MFSCH5SIN0SOT0SCK0		0x00005000
/*! \brief mfs channel 5 sin pin0 sot pin0 sck pin1 */
#define IO_MFSCH5SIN0SOT0SCK1		0x00009000
/*! \brief mfs channel 5 sin pin0 sot pin0 sck pin2 */
#define IO_MFSCH5SIN0SOT0SCK2		0x0000d000
/*! \brief mfs channel 5 sin pin0 sot pin1 sck no output */
#define IO_MFSCH5SIN0SOT1NOSCK		0x00002000
/*! \brief mfs channel 5 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH5SIN0SOT1SCK0		0x00006000
/*! \brief mfs channel 5 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH5SIN0SOT1SCK1		0x0000a000
/*! \brief mfs channel 5 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH5SIN0SOT1SCK2		0x0000e000
/*! \brief mfs channel 5 sin pin0 sot pin2 sck no output */
#define IO_MFSCH5SIN0SOT2NOSCK		0x00003000
/*! \brief mfs channel 5 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH5SIN0SOT2SCK0		0x00007000
/*! \brief mfs channel 5 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH5SIN0SOT2SCK1		0x0000b000
/*! \brief mfs channel 5 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH5SIN0SOT2SCK2		0x0000f000
/*! \brief mfs channel 5 sin pin1 sot no output sck no output */
#define IO_MFSCH5SIN1NOSOTNOSCK		0x00000800
/*! \brief mfs channel 5 sin pin1 sot no output sck pin0 */
#define IO_MFSCH5SIN1NOSOTSCK0		0x00004800
/*! \brief mfs channel 5 sin pin1 sot no output sck pin1 */
#define IO_MFSCH5SIN1NOSOTSCK1		0x00008800
/*! \brief mfs channel 5 sin pin1 sot no output sck pin2 */
#define IO_MFSCH5SIN1NOSOTSCK2		0x0000c800
/*! \brief mfs channel 5 sin pin1 sot pin0 sck no output */
#define IO_MFSCH5SIN1SOT0NOSCK		0x00001800
/*! \brief mfs channel 5 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH5SIN1SOT0SCK0		0x00005800
/*! \brief mfs channel 5 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH5SIN1SOT0SCK1		0x00009800
/*! \brief mfs channel 5 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH5SIN1SOT0SCK2		0x0000d800
/*! \brief mfs channel 5 sin pin1 sot pin1 sck no output */
#define IO_MFSCH5SIN1SOT1NOSCK		0x00002800
/*! \brief mfs channel 5 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH5SIN1SOT1SCK0		0x00006800
/*! \brief mfs channel 5 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH5SIN1SOT1SCK1		0x0000a800
/*! \brief mfs channel 5 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH5SIN1SOT1SCK2		0x0000e800
/*! \brief mfs channel 5 sin pin1 sot pin2 sck no output */
#define IO_MFSCH5SIN1SOT2NOSCK		0x00003800
/*! \brief mfs channel 5 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH5SIN1SOT2SCK0		0x00007800
/*! \brief mfs channel 5 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH5SIN1SOT2SCK1		0x0000b800
/*! \brief mfs channel 5 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH5SIN1SOT2SCK2		0x0000f800
/*! \brief mfs channel 5 sin pin2 sot no output sck no output */
#define IO_MFSCH5SIN2NOSOTNOSCK		0x00000c00
/*! \brief mfs channel 5 sin pin2 sot no output sck pin0 */
#define IO_MFSCH5SIN2NOSOTSCK0		0x00004c00
/*! \brief mfs channel 5 sin pin2 sot no output sck pin1 */
#define IO_MFSCH5SIN2NOSOTSCK1		0x00008c00
/*! \brief mfs channel 5 sin pin2 sot no output sck pin2 */
#define IO_MFSCH5SIN2NOSOTSCK2		0x0000cc00
/*! \brief mfs channel 5 sin pin2 sot pin0 sck no output */
#define IO_MFSCH5SIN2SOT0NOSCK		0x00001c00
/*! \brief mfs channel 5 sin pin2 sot pin0 sck pin0 */
#define IO_MFSCH5SIN2SOT0SCK0		0x00005c00
/*! \brief mfs channel 5 sin pin2 sot pin0 sck pin1 */
#define IO_MFSCH5SIN2SOT0SCK1		0x00009c00
/*! \brief mfs channel 5 sin pin2 sot pin0 sck pin2 */
#define IO_MFSCH5SIN2SOT0SCK2		0x0000dc00
/*! \brief mfs channel 5 sin pin2 sot pin1 sck no output */
#define IO_MFSCH5SIN2SOT1NOSCK		0x00002c00
/*! \brief mfs channel 5 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH5SIN2SOT1SCK0		0x00006c00
/*! \brief mfs channel 5 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH5SIN2SOT1SCK1		0x0000ac00
/*! \brief mfs channel 5 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH5SIN2SOT1SCK2		0x0000ec00
/*! \brief mfs channel 5 sin pin2 sot pin2 sck no output */
#define IO_MFSCH5SIN2SOT2NOSCK		0x00003c00
/*! \brief mfs channel 5 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH5SIN2SOT2SCK0		0x00007c00
/*! \brief mfs channel 5 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH5SIN2SOT2SCK1		0x0000bc00
/*! \brief mfs channel 5 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH5SIN2SOT2SCK2		0x0000fc00
/*! \brief mfs channel 6 sin pin0 sot no output sck no output */
#define IO_MFSCH6SIN0NOSOTNOSCK		0x00000000
/*! \brief mfs channel 6 sin pin0 sot no output sck pin0 */
#define IO_MFSCH6SIN0NOSOTSCK0		0x00100000
/*! \brief mfs channel 6 sin pin0 sot no output sck pin1 */
#define IO_MFSCH6SIN0NOSOTSCK1		0x00200000
/*! \brief mfs channel 6 sin pin0 sot no output sck pin2 */
#define IO_MFSCH6SIN0NOSOTSCK2		0x00300000
/*! \brief mfs channel 6 sin pin0 sot pin0 sck no output */
#define IO_MFSCH6SIN0SOT0NOSCK		0x00040000
/*! \brief mfs channel 6 sin pin0 sot pin0 sck pin0 */
#define IO_MFSCH6SIN0SOT0SCK0		0x00140000
/*! \brief mfs channel 6 sin pin0 sot pin0 sck pin1 */
#define IO_MFSCH6SIN0SOT0SCK1		0x00240000
/*! \brief mfs channel 6 sin pin0 sot pin0 sck pin2 */
#define IO_MFSCH6SIN0SOT0SCK2		0x00340000
/*! \brief mfs channel 6 sin pin0 sot pin1 sck no output */
#define IO_MFSCH6SIN0SOT1NOSCK		0x00080000
/*! \brief mfs channel 6 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH6SIN0SOT1SCK0		0x00180000
/*! \brief mfs channel 6 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH6SIN0SOT1SCK1		0x00280000
/*! \brief mfs channel 6 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH6SIN0SOT1SCK2		0x00380000
/*! \brief mfs channel 6 sin pin0 sot pin2 sck no output */
#define IO_MFSCH6SIN0SOT2NOSCK		0x000c0000
/*! \brief mfs channel 6 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH6SIN0SOT2SCK0		0x001c0000
/*! \brief mfs channel 6 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH6SIN0SOT2SCK1		0x002c0000
/*! \brief mfs channel 6 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH6SIN0SOT2SCK2		0x003c0000
/*! \brief mfs channel 6 sin pin1 sot no output sck no output */
#define IO_MFSCH6SIN1NOSOTNOSCK		0x00020000
/*! \brief mfs channel 6 sin pin1 sot no output sck pin0 */
#define IO_MFSCH6SIN1NOSOTSCK0		0x00120000
/*! \brief mfs channel 6 sin pin1 sot no output sck pin1 */
#define IO_MFSCH6SIN1NOSOTSCK1		0x00220000
/*! \brief mfs channel 6 sin pin1 sot no output sck pin2 */
#define IO_MFSCH6SIN1NOSOTSCK2		0x00320000
/*! \brief mfs channel 6 sin pin1 sot pin0 sck no output */
#define IO_MFSCH6SIN1SOT0NOSCK		0x00060000
/*! \brief mfs channel 6 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH6SIN1SOT0SCK0		0x00160000
/*! \brief mfs channel 6 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH6SIN1SOT0SCK1		0x00260000
/*! \brief mfs channel 6 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH6SIN1SOT0SCK2		0x00360000
/*! \brief mfs channel 6 sin pin1 sot pin1 sck no output */
#define IO_MFSCH6SIN1SOT1NOSCK		0x000a0000
/*! \brief mfs channel 6 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH6SIN1SOT1SCK0		0x001a0000
/*! \brief mfs channel 6 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH6SIN1SOT1SCK1		0x002a0000
/*! \brief mfs channel 6 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH6SIN1SOT1SCK2		0x003a0000
/*! \brief mfs channel 6 sin pin1 sot pin2 sck no output */
#define IO_MFSCH6SIN1SOT2NOSCK		0x000e0000
/*! \brief mfs channel 6 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH6SIN1SOT2SCK0		0x001e0000
/*! \brief mfs channel 6 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH6SIN1SOT2SCK1		0x002e0000
/*! \brief mfs channel 6 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH6SIN1SOT2SCK2		0x003e0000
/*! \brief mfs channel 6 sin pin2 sot no output sck no output */
#define IO_MFSCH6SIN2NOSOTNOSCK		0x00030000
/*! \brief mfs channel 6 sin pin2 sot no output sck pin0 */
#define IO_MFSCH6SIN2NOSOTSCK0		0x00130000
/*! \brief mfs channel 6 sin pin2 sot no output sck pin1 */
#define IO_MFSCH6SIN2NOSOTSCK1		0x00230000
/*! \brief mfs channel 6 sin pin2 sot no output sck pin2 */
#define IO_MFSCH6SIN2NOSOTSCK2		0x00330000
/*! \brief mfs channel 6 sin pin2 sot pin0 sck no output */
#define IO_MFSCH6SIN2SOT0NOSCK		0x00070000
/*! \brief mfs channel 6 sin pin2 sot pin0 sck pin0 */
#define IO_MFSCH6SIN2SOT0SCK0		0x00170000
/*! \brief mfs channel 6 sin pin2 sot pin0 sck pin1 */
#define IO_MFSCH6SIN2SOT0SCK1		0x00270000
/*! \brief mfs channel 6 sin pin2 sot pin0 sck pin2 */
#define IO_MFSCH6SIN2SOT0SCK2		0x00370000
/*! \brief mfs channel 6 sin pin2 sot pin1 sck no output */
#define IO_MFSCH6SIN2SOT1NOSCK		0x000b0000
/*! \brief mfs channel 6 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH6SIN2SOT1SCK0		0x001b0000
/*! \brief mfs channel 6 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH6SIN2SOT1SCK1		0x002b0000
/*! \brief mfs channel 6 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH6SIN2SOT1SCK2		0x003b0000
/*! \brief mfs channel 6 sin pin2 sot pin2 sck no output */
#define IO_MFSCH6SIN2SOT2NOSCK		0x000f0000
/*! \brief mfs channel 6 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH6SIN2SOT2SCK0		0x001f0000
/*! \brief mfs channel 6 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH6SIN2SOT2SCK1		0x002f0000
/*! \brief mfs channel 6 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH6SIN2SOT2SCK2		0x003f0000
/*! \brief mfs channel 7 sin pin0 sot no output sck no output */
#define IO_MFSCH7SIN0NOSOTNOSCK		0x00000000
/*! \brief mfs channel 7 sin pin0 sot no output sck pin0 */
#define IO_MFSCH7SIN0NOSOTSCK0		0x04000000
/*! \brief mfs channel 7 sin pin0 sot no output sck pin1 */
#define IO_MFSCH7SIN0NOSOTSCK1		0x08000000
/*! \brief mfs channel 7 sin pin0 sot no output sck pin2 */
#define IO_MFSCH7SIN0NOSOTSCK2		0x0c000000
/*! \brief mfs channel 7 sin pin0 sot pin0 sck no output */
#define IO_MFSCH7SIN0SOT0NOSCK		0x01000000
/*! \brief mfs channel 7 sin pin0 sot pin0 sck pin0 */
#define IO_MFSCH7SIN0SOT0SCK0		0x05000000
/*! \brief mfs channel 7 sin pin0 sot pin0 sck pin1 */
#define IO_MFSCH7SIN0SOT0SCK1		0x09000000
/*! \brief mfs channel 7 sin pin0 sot pin0 sck pin2 */
#define IO_MFSCH7SIN0SOT0SCK2		0x0d000000
/*! \brief mfs channel 7 sin pin0 sot pin1 sck no output */
#define IO_MFSCH7SIN0SOT1NOSCK		0x02000000
/*! \brief mfs channel 7 sin pin0 sot pin1 sck pin0 */
#define IO_MFSCH7SIN0SOT1SCK0		0x06000000
/*! \brief mfs channel 7 sin pin0 sot pin1 sck pin1 */
#define IO_MFSCH7SIN0SOT1SCK1		0x0a000000
/*! \brief mfs channel 7 sin pin0 sot pin1 sck pin2 */
#define IO_MFSCH7SIN0SOT1SCK2		0x0e000000
/*! \brief mfs channel 7 sin pin0 sot pin2 sck no output */
#define IO_MFSCH7SIN0SOT2NOSCK		0x03000000
/*! \brief mfs channel 7 sin pin0 sot pin2 sck pin0 */
#define IO_MFSCH7SIN0SOT2SCK0		0x07000000
/*! \brief mfs channel 7 sin pin0 sot pin2 sck pin1 */
#define IO_MFSCH7SIN0SOT2SCK1		0x0b000000
/*! \brief mfs channel 7 sin pin0 sot pin2 sck pin2 */
#define IO_MFSCH7SIN0SOT2SCK2		0x0f000000
/*! \brief mfs channel 7 sin pin1 sot no output sck no output */
#define IO_MFSCH7SIN1NOSOTNOSCK		0x00800000
/*! \brief mfs channel 7 sin pin1 sot no output sck pin0 */
#define IO_MFSCH7SIN1NOSOTSCK0		0x04800000
/*! \brief mfs channel 7 sin pin1 sot no output sck pin1 */
#define IO_MFSCH7SIN1NOSOTSCK1		0x08800000
/*! \brief mfs channel 7 sin pin1 sot no output sck pin2 */
#define IO_MFSCHSSIN1NOSOTSCK2		0x0c800000
/*! \brief mfs channel 7 sin pin1 sot pin0 sck no output */
#define IO_MFSCH7SIN1SOT0NOSCK		0x01800000
/*! \brief mfs channel 7 sin pin1 sot pin0 sck pin0 */
#define IO_MFSCH7SIN1SOT0SCK0		0x05800000
/*! \brief mfs channel 7 sin pin1 sot pin0 sck pin1 */
#define IO_MFSCH7SIN1SOT0SCK1		0x09800000
/*! \brief mfs channel 7 sin pin1 sot pin0 sck pin2 */
#define IO_MFSCH7SIN1SOT0SCK2		0x0d800000
/*! \brief mfs channel 7 sin pin1 sot pin1 sck no output */
#define IO_MFSCH7SIN1SOT1NOSCK		0x02800000
/*! \brief mfs channel 7 sin pin1 sot pin1 sck pin0 */
#define IO_MFSCH7SIN1SOT1SCK0		0x06800000
/*! \brief mfs channel 7 sin pin1 sot pin1 sck pin1 */
#define IO_MFSCH7SIN1SOT1SCK1		0x0a800000
/*! \brief mfs channel 7 sin pin1 sot pin1 sck pin2 */
#define IO_MFSCH7SIN1SOT1SCK2		0x0e800000
/*! \brief mfs channel 7 sin pin1 sot pin2 sck no output */
#define IO_MFSCH7SIN1SOT2NOSCK		0x03800000
/*! \brief mfs channel 7 sin pin1 sot pin2 sck pin0 */
#define IO_MFSCH7SIN1SOT2SCK0		0x07800000
/*! \brief mfs channel 7 sin pin1 sot pin2 sck pin1 */
#define IO_MFSCH7SIN1SOT2SCK1		0x0b800000
/*! \brief mfs channel 7 sin pin1 sot pin2 sck pin2 */
#define IO_MFSCH7SIN1SOT2SCK2		0x0f800000
/*! \brief mfs channel 7 sin pin2 sot no output sck no output */
#define IO_MFSCH7SIN2NOSOTNOSCK		0x00c00000
/*! \brief mfs channel 7 sin pin2 sot no output sck pin0 */
#define IO_MFSCH7SIN2NOSOTSCK0		0x04c00000
/*! \brief mfs channel 7 sin pin2 sot no output sck pin1 */
#define IO_MFSCH7SIN2NOSOTSCK1		0x08c00000
/*! \brief mfs channel 7 sin pin2 sot no output sck pin2 */
#define IO_MFSCHSSIN2NOSOTSCK2		0x0cc00000
/*! \brief mfs channel 7 sin pin2 sot pin0 sck no output */
#define IO_MFSCH7SIN2SOT0NOSCK		0x01c00000
/*! \brief mfs channel 7 sin pin2 sot pin0 sck pin0 */
#define IO_MFSCH7SIN2SOT0SCK0		0x05c00000
/*! \brief mfs channel 7 sin pin2 sot pin0 sck pin1 */
#define IO_MFSCH7SIN2SOT0SCK1		0x09c00000
/*! \brief mfs channel 7 sin pin2 sot pin0 sck pin2 */
#define IO_MFSCH7SIN2SOT0SCK2		0x0dc00000
/*! \brief mfs channel 7 sin pin2 sot pin1 sck no output */
#define IO_MFSCH7SIN2SOT1NOSCK		0x02c00000
/*! \brief mfs channel 7 sin pin2 sot pin1 sck pin0 */
#define IO_MFSCH7SIN2SOT1SCK0		0x06c00000
/*! \brief mfs channel 7 sin pin2 sot pin1 sck pin1 */
#define IO_MFSCH7SIN2SOT1SCK1		0x0ac00000
/*! \brief mfs channel 7 sin pin2 sot pin1 sck pin2 */
#define IO_MFSCH7SIN2SOT1SCK2		0x0ec00000
/*! \brief mfs channel 7 sin pin2 sot pin2 sck no output */
#define IO_MFSCH7SIN2SOT2NOSCK		0x03c00000
/*! \brief mfs channel 7 sin pin2 sot pin2 sck pin0 */
#define IO_MFSCH7SIN2SOT2SCK0		0x07c00000
/*! \brief mfs channel 7 sin pin2 sot pin2 sck pin1 */
#define IO_MFSCH7SIN2SOT2SCK1		0x0bc00000
/*! \brief mfs channel 7 sin pin2 sot pin2 sck pin2 */
#define IO_MFSCH7SIN2SOT2SCK2		0x0fc00000
	
#define IS_IO_MFSPIN(mode)                   ((mode == IO_MFSCH0SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH0SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH0SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH0SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH0SIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH0SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH0SIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH0SIN2SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH1SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH1SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH1SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH1SIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH1SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH1SIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH1SIN2SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH2SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH2SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH2SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH2SIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH2SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH2SIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH2SIN2SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH3SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH3SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH3SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH3SIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH3SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH3SIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH3SIN2SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH4SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH4SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH4SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH4SIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH4SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH4SIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH4SIN2SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH5SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH5SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH5SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH5SIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH5SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH5SIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH5SIN2SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH6SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH6SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH6SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH6SIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH6SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH6SIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH6SIN2SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN0NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN0NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH7SIN0NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCH7SIN0NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN0SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN1NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN1NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH7SIN1NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCHSSIN1NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN1SOT2SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN2NOSOTNOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN2NOSOTSCK0) \
	                                              ||(mode ==IO_MFSCH7SIN2NOSOTSCK1) \
	                                              ||(mode ==IO_MFSCHSSIN2NOSOTSCK2) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT0NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT0SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT0SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT0SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT1NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT1SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT1SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT1SCK2) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT2NOSCK) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT2SCK0) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT2SCK1) \
	                                              ||(mode ==IO_MFSCH7SIN2SOT2SCK2))




/*! \brief input capture IC00 */
#define IO_ICU_IC00		0x00
/*! \brief input capture IC01 */
#define IO_ICU_IC01		0x01
/*! \brief input capture IC02 */
#define IO_ICU_IC02		0x02
/*! \brief input capture IC03 */
#define IO_ICU_IC03		0x03
/*! \brief input capture IC10 */
#define IO_ICU_IC10		0x04
/*! \brief input capture IC11 */
#define IO_ICU_IC11		0x05
/*! \brief input capture IC12 */
#define IO_ICU_IC12		0x06
/*! \brief input capture IC13 */
#define IO_ICU_IC13		0x07
#define IS_IO_ICUCH(mode)                   ((mode == IO_ICU_IC00) \
	                                              ||(mode ==IO_ICU_IC01) \
	                                              ||(mode ==IO_ICU_IC02) \
                                                  ||(mode ==IO_ICU_IC03) \
                                                  ||(mode ==IO_ICU_IC10) \
                                                  ||(mode ==IO_ICU_IC11) \
	                                              ||(mode ==IO_ICU_IC12) \
                                                  ||(mode ==IO_ICU_IC13))



/*! \brief input capture pin0 */
#define IO_ICU_PIN0		0x00000001
/*! \brief input capture pin1 */
#define IO_ICU_PIN1		0x00000002
/*! \brief input capture pin2 for IC00,IC01,IC02 */
#define IO_ICU_PIN2		0x00000003
/*! \brief internal macro MFSch.3LSYN for IC03, IC13 */
#define IO_ICU_3LSYN	0x00000004
/*! \brief internal macro MFSch.7LSYN for IC03, IC13 */
#define IO_ICU_7LSYN	0x00000005
/*! \brief internal macro MFSch.2LSYN for IC02, IC12 */
#define IO_ICU_2LSYN	0x00000004
/*! \brief internal macro MFSch.6LSYN for IC02, IC12 */
#define IO_ICU_6LSYN	0x00000005
/*! \brief internal macro MFSch.1LSYN for IC01, IC11 */
#define IO_ICU_1LSYN	0x00000004
/*! \brief internal macro MFSch.5LSYN for IC01, IC11 */
#define IO_ICU_5LSYN	0x00000005
/*! \brief internal macro MFSch.0LSYN for IC00, IC10 */
#define IO_ICU_0LSYN	0x00000004
/*! \brief internal macro MFSch.4LSYN for IC00, IC10 */
#define IO_ICU_4LSYN	0x00000005
#define IS_IO_ICUPIN(mode)                   ((mode == IO_ICU_PIN0) \
	                                              ||(mode ==IO_ICU_PIN1) \
	                                              ||(mode ==IO_ICU_PIN2) \
                                                  ||(mode ==IO_ICU_3LSYN) \
                                                  ||(mode ==IO_ICU_7LSYN) \
                                                  ||(mode ==IO_ICU_2LSYN) \
	                                              ||(mode ==IO_ICU_6LSYN) \
	                                              ||(mode ==IO_ICU_1LSYN) \
                                                  ||(mode ==IO_ICU_5LSYN) \
	                                              ||(mode ==IO_ICU_0LSYN) \
                                                  ||(mode ==IO_ICU_4LSYN))


/*! \brief waveout generator  RTO00  */
#define IO_RTO_CH00		0x00000000
/*! \brief waveout generator  RTO01  */
#define IO_RTO_CH01		0x00000001
/*! \brief waveout generator  RTO02  */
#define IO_RTO_CH02		0x00000002
/*! \brief waveout generator  RTO03  */
#define IO_RTO_CH03		0x00000003
/*! \brief waveout generator  RTO04  */
#define IO_RTO_CH04		0x00000004
/*! \brief waveout generator  RTO05  */
#define IO_RTO_CH05		0x00000005
/*! \brief waveout generator  RTO10  */
#define IO_RTO_CH10		0x00000006
/*! \brief waveout generator  RTO11  */
#define IO_RTO_CH11		0x00000007
/*! \brief waveout generator  RTO12  */
#define IO_RTO_CH12		0x00000008
/*! \brief waveout generator  RTO13  */
#define IO_RTO_CH13		0x00000009
/*! \brief waveout generator  RTO14  */
#define IO_RTO_CH14		0x0000000a
/*! \brief waveout generator  RTO15  */
#define IO_RTO_CH15		0x0000000b
#define IS_IO_RTOCH(mode)                   ((mode == IO_RTO_CH00) \
	                                              ||(mode ==IO_RTO_CH01) \
	                                              ||(mode ==IO_RTO_CH02) \
                                                  ||(mode ==IO_RTO_CH03) \
                                                  ||(mode ==IO_RTO_CH04) \
                                                  ||(mode ==IO_RTO_CH05) \
	                                              ||(mode ==IO_RTO_CH10) \
	                                              ||(mode ==IO_RTO_CH11) \
                                                  ||(mode ==IO_RTO_CH12) \
	                                              ||(mode ==IO_RTO_CH13) \
	                                              ||(mode ==IO_RTO_CH14) \
                                                  ||(mode ==IO_RTO_CH15))
                                                  

/*! \brief waveform generater pin no output */
#define IO_RTO_NOOUTPUT	0x00000000
/*! \brief waveform generater pin0 */
#define IO_RTO_PIN0		0x00000001
/*! \brief waveform generater pin1 */
#define IO_RTO_PIN1		0x00000002
#define IS_IO_RTOPIN(mode)                   ((mode == IO_RTO_NOOUTPUT) \
	                                              ||(mode ==IO_RTO_PIN0) \
	                                              ||(mode ==IO_RTO_PIN1))
                                                 


/*! \brief Check parameter */
#define IS_IO_CRPIN(mode)                    ((mode == ENABLE) \
											 	  ||(mode ==DISABLE))


/*! \brief Check parameter */
#define IS_IO_SUBXC(mode)                    ((mode == ENABLE) \
											 	  ||(mode ==DISABLE))

/*! \brief Check parameter */
#define IS_IO_USBPIN(mode)                   ((mode == ENABLE) \
												  ||(mode ==DISABLE))

/*! \brief Check parameter */
#define IS_IO_USB0CTL(mode)                   ((mode == ENABLE) \
												  ||(mode ==DISABLE))

/*! \brief Check parameter */
#define IS_IO_NMIX(mode)                      ((mode == ENABLE) \
												  ||(mode ==DISABLE))

/*! \brief Check parameter */
#define IS_IO_CRPIN(mode)                      ((mode == ENABLE) \
												  ||(mode ==DISABLE))

/*! \brief Check parameter */
#define IS_IO_JTAGPIN(mode)                    ((mode == ENABLE) \
												  ||(mode ==DISABLE))

/*! \brief Check parameter */
#define IS_IO_SWDPIN(mode)                    ((mode == ENABLE) \
												  ||(mode ==DISABLE))

/*! \brief Check parameter */
#define IS_IO_TRACEPIN(mode)                   ((mode == ENABLE) \
												  ||(mode ==DISABLE))


/*--------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
	
#endif
void IO_ConfigPort(uint8_t ioport, uint32_t iopin, uint8_t iomode);
void IO_GPIOSetInput(uint8_t ioport, uint32_t iopin);
void IO_GPIOSetOutput(uint8_t ioport, uint32_t iopin);
uint32_t IO_GPIOReadData(uint8_t ioport, uint32_t iopin);
void IO_GPIOWriteData(uint8_t ioport, uint32_t iopin, uint32_t iodata);
void IO_GPIOConfigPullup(uint8_t ioport, uint32_t iopin, uint8_t pulltype);

void IO_ConfigAD(uint32_t iopin, uint8_t adtype);
void IO_EPFR0102InputCapture(uint8_t icuch, uint32_t icupin);
void IO_EPFR0102WaveFormGen(uint8_t rtoch,uint32_t rtopin);
void IO_EPFR06ExtInt(uint32_t extintpin);
void IO_EPFR0405BT(uint8_t btch,uint32_t btpin);
void IO_EPFR09ADTrigger(uint32_t adtrgpin, uint8_t adunit);
void IO_EPFR09CANSetInput(uint32_t canpin, uint8_t canch);
void IO_EPFR09CANSetOutput(uint32_t canpin, uint8_t canch);
void IO_EPFR09QPRCSetInput(uint32_t qprcpin, uint8_t qprcch);
void IO_EPFR0708MFS(uint8_t mfsch, uint32_t mfspin);
void IO_EPFR00ConfigUSBCtlSgnl(uint8_t ctrlsgnl);
void IO_EPFR00CR(uint8_t croutpin);
void IO_EPFR00ConfigNMIX(uint8_t nmixmode);
void IO_EPFR00ConfigJtag(uint8_t Jtagmode);
void IO_EPFR00ConfigTrace(uint8_t tracemode);
void IO_EPFR00ConfigSWD(uint8_t swdmode);
void IO_EPFRSPSRConfigUSB(uint8_t usbpin);
void IO_EPFRSPSRCofigMainXC(uint8_t mnxc);
void IO_EPFRSPSRCofigSubXC(uint8_t subxc);
void IO_WriteGPIOPin(uint8_t Port, uint32_t Pin, uint8_t Data );
uint8_t IO_ReadGPIOPin(uint8_t Port, uint32_t Pin);
void IO_ConfigGPIOPin(uint8_t Port, uint32_t Pin, uint8_t Dir, uint8_t Pullup);




#ifdef __cplusplus
}
#endif


#endif /* _LVD_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
 

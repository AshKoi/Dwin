
#ifndef _MAIN_H_
#define _MAIN_H_

#define 		LED_RUN_ON()		  IO_GPIOWriteData(GPIO_PORT3,GPIO_PIN_C,0x0000)
#define 		LED_RUN_OFF()		  IO_GPIOWriteData(GPIO_PORT3,GPIO_PIN_C,0xFFFF)

#define 		LED_ERR_ON()		  IO_GPIOWriteData(GPIO_PORT3,GPIO_PIN_D,0x0000)
#define 		LED_ERR_OFF()		  IO_GPIOWriteData(GPIO_PORT3,GPIO_PIN_D,0xFFFF)

void TaskStart(void *pdata);	

#endif
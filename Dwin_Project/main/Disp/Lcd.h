/***************************************************************
****************************************************************/
#ifndef __LCD_H
#define __LCD_H	
#include "Os_cpu.h"
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "config.h"
#include "board.h"

 //定义LCD的尺寸	
#define LCD_W 				160
#define LCD_H 				160
#define LCD_CHAR_W			8//16      //8
#define LCD_CHAR_H			16

#define HZ_BYTES			32       // 24 * 24 字库

#define HZK_OFFSET			0xA0000 //(704 * 1024)  261k
#define CHAR_K_OFFSET		(HZK_OFFSET + 0x41560) 

//#define HZK_OFFSET			0x80000 //(512 * 1024)  261k
//#define CHAR_K_OFFSET		0xFA000 //(1000 * 1024) 

//画笔颜色
#define R_COLOR				0xF800
#define G_COLOR				0x07E0
#define B_COLOR				0x001F

#define WHITE         	 	0xF7DE//0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE         	 	0x001F  
#define BRED             	0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED           	 	0xF800
#define MAGENTA       	 	0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 			 	0XBC40 //棕色
#define BRRED 			 	0XFC07 //棕红色
#define GRAY  			 	0X8430 //灰色

// LCD clear type
#define LCD_CLEAR_ALL           0			//无用，为应用避免编译错误
#define LCD_CLEAR_DATA          1
#define LCD_CLEAR_ICON          2

// LCD display mode
#define LCD_DISP_NORMAL         0
#define LCD_DISP_INVERT         1

//============================================================
// 寄存器指令
//============================================================
#define UC1698_SetCAL		0x00	//set column address lsb
#define UC1698_SetCAM		0x10	//set column address msb
#define UC1698_SetTC		0x24	//set temp. compensation
#define UC1698_SetPC		0x28	//set power control
#define UC1698_SetAPC		0x30	//set adv. program control,**do not using.for testing.
#define UC1698_SetSLL		0x40	//set scroll line lsb
#define UC1698_SetSLM		0x50	//set scroll line msb
#define UC1698_SetRAL		0x60	//set row address lsb
#define UC1698_SetRAM		0x70	//set row address msb
#define UC1698_SetPM		0x81	//set v-bias potentiometer
#define UC1698_SetPDC		0x84	//set partial display control
#define UC1698_SetRAC		0x88	//set ram address control
#define UC1698_SetFL		0x90	//set fixed lines
#define UC1698_SetLR		0xa0	//set line rate
#define UC1698_SetAPO		0xa4	//set all-pixel on
#define UC1698_SetINV		0xa6	//set inverse display
#define UC1698_SetDE		0xa8	//set display enable
#define UC1698_SetMAP		0xc0	//set lcd maping control
#define UC1698_SetNIV		0xc8	//set n-line inversion
#define UC1698_SetCP		0xd0	//set color pattern
#define UC1698_SetCM		0xd4	//set color mode
#define UC1698_SetCS		0xd8	//set com scan funtion
#define UC1698_SetRST		0xe2	//set system reset
#define UC1698_SetNOP		0xe3	//no operation
#define UC1698_SetBR		0xe8	//set lcd bias ratio
#define UC1698_SetCEND		0xf1	//set com end
#define UC1698_SetDST		0xf2	//set partial display start
#define UC1698_SetDEND		0xf3	//set partial display end
#define UC1698_SetWC0		0xf4	//set window program starting column address
#define UC1698_SetWR0		0xf5	//set window program starting row address
#define UC1698_SetWC1		0xf6	//set window program ending column address
#define UC1698_SetWR1		0xf7	//set window program ending row address
#define UC1698_SetWMODE		0xf8	//set window program mode
#define UC1698_SetMTPC		0xb8	//set MTP operation control
#define UC1698_SetMTPWM		0xb9	//set MTP Write Mask

//============================================================
// 指令参数
//============================================================
#define UC1698_Bias_5		0
#define UC1698_Bias_10		1
#define UC1698_Bias_11		2
#define UC1698_Bias_12		3

#define UC1698_LcdCapSmall	0
#define UC1698_LcdCapLarge	1
#define UC1698_VlcdInter	0x02
#define UC1698_VlcdExter	0

#define UC1698_TC00	0	//v-bias temp compensation coefficient -0.00%/co
#define UC1698_TC05	1	//v-bias temp compensation coefficient -0.05%/co
#define UC1698_TC15	2	//v-bias temp compensation coefficient -0.15%/co
#define UC1698_TC25	3	//v-bias temp compensation coefficient -0.25%/co

#define UC1698_SPI_PORT         SPI_CHL_1
#define UC1698_SPI_CS_LOW()     LCD_CS_PDOR=0x00
#define UC1698_SPI_CS_HIGH()    LCD_CS_PDOR=0x01
#define UC1698_SPI_CMD_EN()     LCD_CD_PDOR=0x00
#define UC1698_SPI_DATA_EN()    LCD_CD_PDOR=0x01

#define LCD_BG_PFR                bFM3_GPIO_PFR7_P5
#define LCD_BG_DDR                bFM3_GPIO_DDR7_P5
#define LCD_BG_PDOR               bFM3_GPIO_PDOR7_P5
#define LCD_BG_PDIR               bFM3_GPIO_PDIR7_P5

#define LCD_CD_PFR                bFM3_GPIO_PFR7_P7
#define LCD_CD_DDR                bFM3_GPIO_DDR7_P7
#define LCD_CD_PDOR               bFM3_GPIO_PDOR7_P7
#define LCD_CD_PDIR               bFM3_GPIO_PDIR7_P7

#define LCD_CK_PFR                bFM3_GPIO_PFR7_P8
#define LCD_CK_DDR                bFM3_GPIO_DDR7_P8
#define LCD_CK_PDOR               bFM3_GPIO_PDOR7_P8
#define LCD_CK_PDIR               bFM3_GPIO_PDIR7_P8

#define LCD_CS_PFR                bFM3_GPIO_PFR7_PA
#define LCD_CS_DDR                bFM3_GPIO_DDR7_PA
#define LCD_CS_PDOR               bFM3_GPIO_PDOR7_PA
#define LCD_CS_PDIR               bFM3_GPIO_PDIR7_PA

#define LCD_RST_PFR                bFM3_GPIO_PFR7_P6
#define LCD_RST_DDR                bFM3_GPIO_DDR7_P6
#define LCD_RST_PDOR               bFM3_GPIO_PDOR7_P6
#define LCD_RST_PDIR               bFM3_GPIO_PDIR7_P6

#define LCD_DATA_PFR                bFM3_GPIO_PFR7_P9
#define LCD_DATA_DDR                bFM3_GPIO_DDR7_P9
#define LCD_DATA_PDOR               bFM3_GPIO_PDOR7_P9
#define LCD_DATA_PDIR               bFM3_GPIO_PDIR7_P9

#define  LCD_BG_ON()			LCD_BG_PDOR=0x01;	
#define  LCD_BG_OFF()			LCD_BG_PDOR=0x00;

#define LCD_RD_DATA(data)           data = 0;          

extern INT8U           g_nScreenBuf[160][20];

void LcdInit(void);
void LcdRecClear(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U Color);
void LcdClear(INT16U Color);
void Address_set(INT16U x1,INT16U y1,INT16U x2,INT16U y2);

void LCD_DrawPoint(INT16U x,INT16U y, INT16U nColour);
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour);
void LCD_DrawXieLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour);
void LCD_DrawRectangle(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour);

void LCD_DrawPoint2(INT16U x,INT16U y, INT16U nColour);
void LCD_DrawLine2(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour);
void LCD_DrawXieLine2(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour);
void LCD_DrawRectangle2(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour);


void LCD_ShowChar(INT16U x,INT16U y,INT8U num, INT16U nBackColour, INT16U nDrawColour, INT8U nMode);
void LCD_ShowHZ(INT16U x,INT16U y,const INT8U *p, INT16U nBackColour, INT16U nDrawColour, INT8U nMode);
void LCD_ShowHzString(INT16U x,INT16U y,const INT8U *p, INT16U nBackColour, INT16U nDrawColour, INT8U nMode);


void LCD_ShowLogo(INT16U x,INT16U y, INT16U nOnceLen, INT32U nSize, INT8U* pLogo, INT16U nBackColour, INT16U nDrawColour, INT8U nMode);
void LCD_ShowAsciiLogo(INT16U x,INT16U y, INT16U nOnceLen, INT32U nSize, INT8U* pLogo1, INT16U nBackColour, INT16U nDrawColour, INT8U nMode);

void ClearScreenUC1698U(INT8U nData);
void UC1698SetRamStart( INT8U x, INT8U y );
#endif  

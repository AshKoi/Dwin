/***************************************************************
****************************************************************/

#include "Lcd.h"
#include "Macro.h"
#include "Base.h"
#include "GuiwinCfg.h"


#ifndef GPIO_PDOR_BASE_ADDR
#define GPIO_PDOR_BASE_ADDR     (volatile uint32_t*)(&FM3_GPIO->PDOR0)
#endif
#define  UC1698WriteCmd		LCD_WR_REG
#define  UC1698WriteData	LCD_WR_DATA
const   INT8U g_nBit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
INT8U         g_nScreenBuf[160][20]   = {0};

void LCD_WR_REG(unsigned int comm)//write command
{    
	INT8U i; 
	LCD_CD_PDOR=0; 
	LCD_CS_PDOR = 0x00;
	for(i=0;i<8;i++) 
	{ 
		if((comm&0x80)==0x80) 
		{ 
			LCD_DATA_PDOR=1; 
		} 
		else LCD_DATA_PDOR=0; 
		LCD_CK_PDOR=0; 
		LCD_CK_PDOR=1; 
		comm=comm<<1; 
	} 
	LCD_CS_PDOR = 0x01;
}

void LCD_WR_DATA_SUB(unsigned int data)//write data
{    	
	INT8U i; 
	LCD_CD_PDOR=1; 
	LCD_CS_PDOR=0; 
	for(i=0;i<8;i++) 
	{ 
		if((data&0x80)==0x80) 
		{ 
			LCD_DATA_PDOR=1; 
		} 
		else LCD_DATA_PDOR=0; 
		LCD_CK_PDOR=0; 
		LCD_CK_PDOR=1; 
		data=data<<1; 
	} 
	LCD_CS_PDOR=1; 
}

void LCD_WR_DATA(INT16U x,INT16U y, INT8U nColor)
{
    INT8U   nTmpBuf[3]      = {0};
    INT8U   x_Bit_Base      = 0;
    INT8U   nX              = 0;
    INT8U   nX1             = 0;
    INT8U   nX2             = 0;
    INT8U   i               = 0;
    INT16U  nTmp            = 0;

	if( x >= LCD_W )
	{
		x = LCD_W - 1;	
	}	
    if( y >= LCD_H )
	{
		x = LCD_H - 1;	
	}
     
    x_Bit_Base = x / 6 * 6; 
    
    UC1698SetRamStart(x_Bit_Base, y); 

    for (i = 0; i < 6; i++)
    {
        nTmp = x_Bit_Base + i;
        if ((g_nScreenBuf[y][nTmp / 8] & (1 << (nTmp % 8))) == (1 << (nTmp % 8)))
        {
             if (i % 2)
             {
                 nTmpBuf[i / 2] |= 0x0F;
             }
             else
             {
                 nTmpBuf[i / 2] |= 0xF0;
             }
        }
    }

    nX = x - x_Bit_Base;
    nX1 = nX / 2;
    nX2 = nX % 2;
    if (nColor == 1)
    {
        if (nX2)
        {
             nTmpBuf[nX1] |= 0x0F;
        }
        else
        {
             nTmpBuf[nX1] |= 0xF0;
        }
    }
    else
    {
        if (nX2)
        {
             nTmpBuf[nX1] &= 0xF0;
        }
        else
{     
             nTmpBuf[nX1] &= 0x0F;
        }
    }

    //3次为完整一组
    LCD_WR_DATA_SUB(nTmpBuf[0]);
    LCD_WR_DATA_SUB(nTmpBuf[1]);
    LCD_WR_DATA_SUB(nTmpBuf[2]);

    if (nColor == 1)
    {
        g_nScreenBuf[y][x / 8] |= (1 << (x % 8));
    }
    else
    {
        g_nScreenBuf[y][x / 8] &= ~(1 << (x % 8));
    }

}

// 将输入的屏幕坐标(x=0~159)转换成实际的控制器坐标(x'=0x25~0x5A)
// dir = 0  x'向0取整
//       1  x'向正无穷取整
INT8U ConvertAxisX( INT8U x, INT8U dir )
{
	INT8U tmp,ret;

	if( x >= LCD_W )
		tmp = LCD_W - 1;
	else
		tmp = x;

	if( dir == 0 )
	{
		ret = tmp / 3;
	}
	else
	{
		ret = tmp / 3 + 1;
		if( ret > 0x35 )
			ret = 0x35;
	}

	return (ret + 0x25);
}

// 根据给定的坐标设置起始地址
void UC1698SetRamStart( INT8U x, INT8U y )
{
	INT8U tmp;
	    
	tmp = ConvertAxisX( x, 0 );

	UC1698WriteCmd( UC1698_SetCAL + (tmp & 0x0F) );
	UC1698WriteCmd( UC1698_SetCAM + ((tmp & 0x70)>>4) );

	UC1698WriteCmd( UC1698_SetRAL + (y & 0x0F) );
	UC1698WriteCmd( UC1698_SetRAM + ((y & 0xF0)>>4) );
}

void LcdConfig(void)
{     
	//
	LCD_BG_ON();

	LCD_RST_PDOR = 0x01;//硬件复位
	OSTimeDlyHMSM (0, 0, 0, 100);

	LCD_RST_PDOR = 0x00;
	OSTimeDlyHMSM (0, 0, 0, 100);

	LCD_RST_PDOR = 0x01;
	OSTimeDlyHMSM (0, 0, 0, 100);
	
//	ContrastLevel=0xb0; 
	LCD_WR_REG(0x25);  // 设置温度补偿系数001001xxb,00b=-0.00%,01b=0.05%,10b=0.15,11b=-0.25% 
	LCD_WR_REG(0x2b);  // 内部DC-DC 
	LCD_WR_REG(0xc4);  //  LCD映像MY=1，MX=0，LC0=0 
	LCD_WR_REG(0xa3);  // 设置行扫描频率
	LCD_WR_REG(0xd1);  // 彩色数据格式R-G-B 
	LCD_WR_REG(0xd5);  // 设置数据位为12位RRRR-GGGG-BBBB 
	LCD_WR_REG(0xc8);LCD_WR_REG(0x1f); // 设置M信号为0行翻砖
	LCD_WR_REG(0xf1);LCD_WR_REG(159);  //set duty 1/160 
	LCD_WR_REG(0xe9);  // 设置偏压比1/10 
	LCD_WR_REG(0xa6);  // 正性显示
	LCD_WR_REG(0x81); 
	//LCD_WR_REG(ContrastLevel); //设置对比度bf 
	LCD_WR_REG(0xb0); //设置对比度bf 
	LCD_WR_REG(0xd8);  // 设置扫描模式
	LCD_WR_REG(0xad);  // 开显示

	#define  UC1698WriteCmd		LCD_WR_REG

	 UC1698WriteCmd(0x40);     /* set scroll line to 0 */ //LSB [0100xxxx] SL[3:0]
    UC1698WriteCmd(0x50);     //MSB [0101xxxx] SL[7:4]
    UC1698WriteCmd(0x84);     /* set partial display control to 0 */ //LC[8] [1000010x]
    UC1698WriteCmd(0x90);     /* set fixed lines to 0 */
    UC1698WriteCmd(0x00);    // set {FLT.FLB}  FLT[3:0],FLB[3:0]
    UC1698WriteCmd(0xa4);     /* set All-Pixel-ON to 0 */  //DC[1] [1010010x]
    //UC1698WriteCmd(0xa6+1);     /* set inverse display to 0 */  //DC[0] [1010011x]
    UC1698WriteCmd(0x31);     /* set Adv. program control to 0x08 */  //R [0011000x]
    UC1698WriteCmd(0x08);     //APC[R][7:0]
    UC1698WriteCmd(0x2b);     /* set power control to 0b'11 */ //PC[1:0] [001010xx]
    UC1698WriteCmd((0x24 + UC1698_TC15));   /* set temp compensation */  //TC[1:0] [001001xx]
    UC1698WriteCmd(0xa3);     /* set line rate to 0b'00 */  //LC[4:3] [101000xx]
    UC1698WriteCmd((0xe8 + 1));  /* set LCD bias ratio */  //BR[1:0] [111010xx]
    UC1698WriteCmd(0xf1);     /* set COM end to 159 */
    UC1698WriteCmd((160 - 1));   //CEN[6:0] [-xxxxxxx] 0~159
    UC1698WriteCmd(0x81);     /* set V-bias potentiometer */
    UC1698WriteCmd(175);  //PM[7:0] 0~255
    UC1698WriteCmd(0xc4);     /* set LCD mapping control to 4 */  //LC[2:0] [11000 MY MX x]
    UC1698WriteCmd(0x89);     /* set RAM address control to 0b'001 */ //AC[2:0] [10001xxx]
    UC1698WriteCmd(0xd1);     /* set color pattern to 0b'1 */  //LC[5] [1101000x] 0:BGR ,1:RGB
    UC1698WriteCmd(0xd5);     /* set color mode to 0b'01 */  /*LC[7:6] [110101xx]  DC[4]=1:  01b:RRRR-GGGG-BBBB,4k-color
                                                               //10b:*/
    UC1698WriteCmd(0xc8);     /* set N-Line inversion to 0b'11000 */
    UC1698WriteCmd(0x18);     //NIV[4:0] [- - - x x xxx]
    UC1698WriteCmd(0xB8);     //MTP功能设置
    UC1698WriteCmd(0x10);    //MTP值有效       MTPC[4:0] [000xxxxx]

    UC1698WriteCmd(UC1698_SetDE|0x05);
    //SetWindows(0,0,160,160);


#define BR 0X01     //Set LCD Bias Ratio:1/10 bias    
#define PC1     0X03        //power control set as internal power 
#define TC      0x01        //set temperate compensation as 0% 
#define PM      0xbf         //Set Vbias Potentiometer      
#define LC_210  0X04        //set LCD Control 
#define LC_43   0x03 
#define LC_5    0X01 
#define LC_76   0x01 
#define LC_8    0X00 
#define NIV     0X10        //n-line inversion 
#define CSF     0X02        //23:enable FRC,PWM,LRM sequence       
#define WPC0    0x25        //Starting Column Address 
#define WPP0    0           //Starting Row Address 
#define WPC1 0x5A //Ending Column Address 
#define WPP1 159     //Ending Row Address 
#define AC     0x01 //Address  Control    
#define CEN  159  //COM scanning end (last COM with full line cycle,0 based index) 

	/*power control*/ 
	LCD_WR_REG(0xE8|(BR&0X03));       //Bias Ratio:1/10 bias 
	LCD_WR_REG(0x28|(PC1&0X03));      //power control set as internal power 
	LCD_WR_REG(0x24|(TC&0x03)); 
	LCD_WR_REG(0x81); 
	LCD_WR_REG(PM );   

	/*lcd control*/ 
	LCD_WR_REG(0xc0|(LC_210&0X07) );     //0xc5,Enable the top FLTx2 and bottom FLBx2 lines in partial display;MY=1 
	LCD_WR_REG(0xa0|(LC_43 &0X03) );  
	LCD_WR_REG(0xd0|(LC_5  &0X01) );  
	LCD_WR_REG(0xd4|(LC_76 &0X03) ); 
	LCD_WR_REG(0x84|(LC_8  &0X01) ); 

	/*n-line inversion*/ 
	LCD_WR_REG(0xc8 );   
	LCD_WR_REG( NIV ); 

	/*com scan fuction*/   
	LCD_WR_REG(0xd8|(CSF&0X07) );   

	/*window*/ 
	LCD_WR_REG(0xf4 );   
	LCD_WR_REG(WPC0 );   
	LCD_WR_REG(0xf6 );   
	LCD_WR_REG(WPC1 );   
	LCD_WR_REG(0xf5 );     
	LCD_WR_REG(WPP0 );   
	LCD_WR_REG(0xf7 );     
	LCD_WR_REG(WPP1 );  

	LCD_WR_REG(0xf8|((AC>>3)&0x01) );      //inside mode，（34）   
	LCD_WR_REG(0x88| (0x07&AC) );   
	LCD_WR_REG(0xf1 ); //com end 
	LCD_WR_REG(CEN );   
	LCD_WR_REG(0xad ); //18:display on,select on/off mode.Green Enhance mode disable 

}


/********************************************************************************
* 函数名:LcdInit
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
void LcdInit(void)
{
	//
    LCD_RANG_PEND(1);
	LCD_BG_PFR = 0x00;
	LCD_BG_DDR = 0x01;
	LCD_BG_PDOR = 0x01;

	LCD_CS_PFR = 0x00;
	LCD_CS_DDR = 0x01;
	LCD_CS_PDOR = 0x01;

	LCD_RST_PFR = 0x00;
	LCD_RST_DDR = 0x01;
	LCD_RST_PDOR = 0x01;

        
    LCD_CD_PFR = 0x00;
	LCD_CD_DDR = 0x01;
	LCD_CD_PDOR = 0x01;
        
	LCD_DATA_PFR = 0x00;
	LCD_DATA_DDR = 0x01;
	LCD_DATA_PDOR = 0x01;
	
	LcdConfig();	
	//LCD_RANG_PEND(1);
    ClearScreenUC1698U(0);
	LCD_RANG_POST();
}


void LcdRecClear(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U Color)
{
	#if 0
	INT16U i,j;
	
	LCD_RANG_PEND(1);
    for (j = y1; j <= y2; j++)
	{
		for (i = x1; i <= x2; i++)
	   	{
            LCD_WR_DATA(i, j, Color);
	    }
	}
	LCD_RANG_POST();
	#else
	INT16U 	i				= 0;
	INT16U	j				= 0;
	INT8U   k           	= 0;
	INT16U 	xx1 			= x1;
	INT16U 	xx2 			= x2;

	if (x1 >= x2)
	{
		return;
	}
	if (y1 >= y2)
	{
		return;
	}

	LCD_RANG_PEND(1);
	if ((x1 + 6) < x2)
	{
		if (x1 % 6)
		{
			xx1 = (x1 / 6 + 1) * 6;
		}
		xx2 = x2 / 6 * 6;
		for (j = y1; j <= y2; j++)
		{
			if (x1 < xx1)
			{
				for (i = x1; i <= xx1; i++)
			   	{
		            LCD_WR_DATA(i, j, Color);
			    }
			}
			if (xx2 < x2)
			{
				for (i = xx2; i <= x2; i++)
			   	{
		            LCD_WR_DATA(i, j, Color);
			    }
			}
		}

		for (j = y1; j <= y2; j++)
		{
			UC1698SetRamStart(xx1, j); 
			for (i = xx1; i < xx2; i += 6)
		   	{
				//3次为完整一组
			    LCD_WR_DATA_SUB(0);
			    LCD_WR_DATA_SUB(0);
			    LCD_WR_DATA_SUB(0);

				for (k = 0; k < 6; k++)
				{
					if (Color == 1)
				    {
				        g_nScreenBuf[j][(i + k) / 8] |= (1 << ((i + k) % 8));
				    }
					else
					{
						g_nScreenBuf[j][(i + k) / 8] &= ~(1 << ((i + k) % 8));
					}
				}
		    }
		}
	}
	else
	{
		for (j = y1; j <= y2; j++)
		{
			for (i = x1; i <= x2; i++)
		   	{
	            LCD_WR_DATA(i, j, Color);
		    }
		}
	}		
	LCD_RANG_POST();
	#endif
    
}

/********************************************************************************
* 函数名:LcdClear
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
void LcdClear(INT16U Color)
{
    LcdRecClear(0, 0, LCD_W - 1, LCD_H - 1, Color); 
    
}

/********************************************************************************
* 函数名:LCD_DrawPoint
* 输　入:POINT_COLOR:此点的颜色
* 输　出:
* 返  回:
* 功  能:画点
*********************************************************************************/
void LCD_DrawPoint(INT16U x,INT16U y, INT16U nColour)
{
    LCD_WR_DATA(x, y, nColour);
} 

/********************************************************************************
* 函数名:LCD_DrawPoint
* 输　入:POINT_COLOR:此点的颜色
* 输　出:
* 返  回:
* 功  能:画点
*********************************************************************************/
void LCD_DrawPoint2(INT16U x,INT16U y, INT16U nColour)
{
	LCD_RANG_PEND(1);
    LCD_DrawPoint(x, y, nColour);
	LCD_RANG_POST();
} 


//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour)
{
    INT16U t; 
	INT16U xerr=0,yerr=0,delta_x,delta_y,distance; 
	INT16U incx,incy,uRow,uCol;  
    
    delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
    {
        LCD_DrawPoint(uRow,uCol, nColour);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
    }
}

void LCD_DrawLine2(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour)
{
    
    LCD_RANG_PEND(1);
    LCD_DrawLine(x1, y1, x2, y2, nColour);
	LCD_RANG_POST();
}


//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawXieLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour)
{
	FP32 	fY	= 0.0;
	FP32 	fK	= 0.0;
	FP32 	fB	= 0.0;
	INT16U	nY	= 0.0;
	INT32S	i 	= 0;
    INT32S	j 	= 0;
    INT16U  nX  = 0;
    INT16U  nXdiff = 0;
    INT16U  nYPre = 0;

	if (x1 == x2)
	{
        if (y2 >= y1)
        {
		    LCD_DrawLine(x1, y1, x2, y2, nColour);
        }
        else
        {
            LCD_DrawLine(x1, y2, x2, y1, nColour);
        }
		return;
	}
    if (y1 == y2)
	{
        if (x2 >= x1)
        {
		    LCD_DrawLine(x1, y1, x2, y2, nColour);
        }
        else
        {
            LCD_DrawLine(x2, y1, x1, y2, nColour);
        }
		return;
	}
	LCD_DrawPoint(x1, y1, nColour);
    nYPre = y1;
	y1 = LCD_H - 1 - y1;
	y2 = LCD_H - 1 - y2;
	
	fK = ((FP32)(y2 - y1)) / ((FP32)(x2 - x1));
	fB = ((FP32)(x2 * y1 - x1 * y2 )) / ((FP32)(x2 - x1));
    
    if (x2 > x1)
    {
        nXdiff = (x2 - x1);
        nX = x1;
    }
    else
    {
        nXdiff = (x1 - x2);
        nX = x2;
    }
	for (i = 1; i <= nXdiff; i++)
	{
		fY = fK * (nX + i) + fB;
		nY = (INT16U)fY;
		if (fY - nY > 0.499)
		{
			nY++;
		}
		nY = LCD_H - 1 - nY;
        if (nY >= nYPre)
        {
            for (j = nYPre; j < nY; j++)
            {
                LCD_DrawPoint(nX + i, j, nColour);
            }
        }
        else
        {
            for (j = nY; j < nYPre; j++)
            {
                LCD_DrawPoint(nX + i, j, nColour);
            }
        }
        
        nYPre = nY;
	}
	LCD_DrawPoint(x2, y2, nColour);
}	

void LCD_DrawXieLine2(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour)
{
	LCD_RANG_PEND(1);
    LCD_DrawXieLine(x1, y1, x2, y2, nColour);
	LCD_RANG_POST();	
}	



//画矩形
void LCD_DrawRectangle(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour)
{
	LCD_DrawLine(x1,y1,x2,y1, nColour);
	LCD_DrawLine(x1,y1,x1,y2, nColour);
	LCD_DrawLine(x1,y2,x2,y2, nColour);
	LCD_DrawLine(x2,y1,x2,y2, nColour);
}

//画矩形
void LCD_DrawRectangle2(INT16U x1, INT16U y1, INT16U x2, INT16U y2, INT16U nColour)
{
	LCD_RANG_PEND(1);
    LCD_DrawRectangle(x1, y1, x2, y2, nColour);
	LCD_RANG_POST();
}



/********************************************************************************
* 函数名:Address_set
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
void Address_set(INT16U x1,INT16U y1, INT16U x2,INT16U y2)
{
#if 0
	LCD_WR_REG(0x2a);                   // column set 
	LCD_WR_DATA(HIBYTE(x1));          //if MADCTL BIT5=0, EC[15:0]=0x00ef
	LCD_WR_DATA(LOBYTE(x1));          //if MADCTL BIT5=1, EC[15:0]=0x013f
	LCD_WR_DATA(HIBYTE(x2));         //01
	LCD_WR_DATA(LOBYTE(x2));         //3f                

	LCD_WR_REG(0x2b);                // page address set 
	LCD_WR_DATA(HIBYTE(y1));          //if MADCTL BIT5=0, EC[15:0]=0x00ef
	LCD_WR_DATA(LOBYTE(y1));          //if MADCTL BIT5=1, EC[15:0]=0x013f
	LCD_WR_DATA(HIBYTE(y2));         //01
	LCD_WR_DATA(LOBYTE(y2));         //3f  

	UC1698SetRamStart(x1, y1); 
#endif
}

/********************************************************************************
* 函数名:LCD_ShowChar
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
void LCD_ShowChar(INT16U x,INT16U y, INT8U nChar, INT16U nBackColour, INT16U nDrawColour, INT8U nMode)
{
    INT8U       i 			= 0;
	INT8U       j 			= 0;
	//INT8U		k 			= 0;
    //INT8U       nData           = 0;
    INT16U 		nColrTmp1, nColrTmp2;
	INT8U 		nTmp[16]	= {0}; // 16 * 24
	INT32U 		offset		= 0;
    //INT8U       nColor        = 0;
	
    nBackColour = 0;
    nDrawColour = 1;
	
    if ((x > LCD_W - LCD_CHAR_W) || (y > LCD_H - LCD_CHAR_H / 2))
    {
        return;
    }
    if(!nMode)
    {
        nColrTmp1 = nDrawColour;
        nColrTmp2 = nBackColour;        
    }
    else
    {
        nColrTmp1 = nBackColour;
        nColrTmp2 = nDrawColour;     
    }

    //设置窗口		   
	//offset = (nChar - 0x20) * sizeof(nTmp);//得到偏移后的值          nChar = nChar - 0x20 
    offset = nChar * 0x10;
	memcpy(&nTmp[0], (INT8U*)(CHAR_K_OFFSET + offset), sizeof(nTmp));
    //memcpy(&nTmp[0], &ASCII816[offset], sizeof(nTmp));
	LCD_RANG_PEND(1);
    for(i = 0; i < sizeof(nTmp); i++)
    {
        for(j = 0;j < 8;j++)
		{
            if (nTmp[i] & g_nBit[7 - j]) 
        {
                LCD_WR_DATA(x + j, y + i, nColrTmp1);
            }
            else
            {
                LCD_WR_DATA(x + j, y + i, nColrTmp2);
            }
            
        }
    }  
    LCD_RANG_POST();
}

/********************************************************************************
* 函数名:LCD_ShowHZ
* 输　入:x,y:起点坐标 *p:字符串起始地址 用16字体
* 输　出:
* 返  回:
* 功  能:显示字符串
*********************************************************************************/
void LCD_ShowHZ(INT16U x, INT16U y, const INT8U* p, INT16U nBackColour, INT16U nDrawColour, INT8U nMode)
{  

   	INT8U    	i				= 0;
	INT8U		j				= 0;
    INT8U 		qh 				= 0; 
	INT8U 		wh 				= 0;  
    //INT8U       nData           = 0;
    INT32U		offset 			= 0;  
    //INT8U       nColor            = 0;
    INT16U 		nColrTmp1, nColrTmp2;
    INT8U  		nTmp[HZ_BYTES] 	= {0};
    
    nBackColour = 0;
    nDrawColour = 1;
    if ( (x > LCD_W - LCD_CHAR_W * 2) || (y > LCD_H - LCD_CHAR_H))
    {
        return;
    }
	
    //每个汉字，有2个字节标识来表示地址(它在字库中的位置)                      // shankr
    qh = p[0] - 0xa0; 	//获得区码 (S1 - (0x20 - 0x80))
    wh = p[1] - 0xa0;	//获得位码 (S2 - (0x20 - 0x80))

	/* 16*16 点阵字库中， 以行分布，共16行。每行16个点，所以每个字符占32字节
	* 每94个字符为一区，共87个区 (1～15区为常用符号区， 16～86区为常用汉字 )
	*获取位置: P = [(S1 -160 -1)*94 + (S2 -160 - 1)]*32      ()
	* 
	*
	* 24*24点阵字库中， 以列分布，共24列。每行24个点， 所以每个字符占72字节
	* 72个区，都为汉字区
	* 获取位置: P = [(S1 -160 -1 -15)*94 + (S2 -160-1)]*72
	*/
  
    offset = (94 * (qh - 1) + (wh - 1)) * sizeof(nTmp); //得到偏移位置
   	memcpy(&nTmp[0], (INT8U*)(HZK_OFFSET + offset), sizeof(nTmp));
    
    if(!nMode)
    {
        nColrTmp1 = nDrawColour;
        nColrTmp2 = nBackColour;        
    }
    else
    {
        nColrTmp1 = nBackColour;
        nColrTmp2 = nDrawColour;     
    }
	LCD_RANG_PEND(1);
    for (i = 0; i < sizeof(nTmp); i++)       
    {																		// nTmp[i / 8] & g_nBit[i % 8 - 1]
        for(j = 0;j < 8;j++)
        {
            if (nTmp[i] & g_nBit[7 - j]) 
            {
                LCD_WR_DATA(x + j + (i % 2) * 8, y + i / 2, nColrTmp1);
            }
            else
            {
                LCD_WR_DATA(x + j + (i % 2) * 8, y + i / 2, nColrTmp2);
            }
            
        }
    }
    LCD_RANG_POST();
}
    
 

/********************************************************************************
* 函数名:LCD_ShowHzString
* 输　入:x,y:起点坐标 *p:字符串起始地址 用16字体
* 输　出:
* 返  回:
* 功  能:显示字符串
*********************************************************************************/
void LCD_ShowHzString(INT16U x,INT16U y, const INT8U *p, INT16U nBackColour, INT16U nDrawColour, INT8U nMode)
{
	#if 0
    while(*p != 0)
    {
        if (x > LCD_W - LCD_CHAR_W)
        {   
            return;
        }
        if (y > (LCD_H - LCD_CHAR_H / 2))
        {
            return;
        }
        
        if ((p[0] & 0x80) == 0)    //字符的编码范围是0~0x7F
        {
            LCD_ShowChar(x, y, *p, nBackColour, nDrawColour, nMode);
            x += LCD_CHAR_W;             // 字符间距 可以自定义 
            p++;
        }
        else                    //汉子
        {
            LCD_ShowHZ(x, y, p, nBackColour, nDrawColour, nMode);
            x += LCD_CHAR_W * 2;
            p += 2;    
        }
    }
	#else
	INT32U 		nStringLen 	= 0;
	INT8U  		nBuf[81] 		= {0};
	INT8U  		k  				= 0;
	INT8U* 		pp  			= (INT8U*)p;
	INT8U 		qh 				= 0; 
	INT8U 		wh 				= 0; 
	INT32U		offset 			= 0;
	INT8U  		nTmp[HZ_BYTES] 	= {0};
	INT16U		ii 				= 0;
	INT8U    	i				= 0;
	INT8U    	xx				= 0;
	INT8U    	x2				= 0;
    
    LCD_RANG_PEND(1);
    if ( (x > LCD_W - LCD_CHAR_W * 2) || (y > LCD_H - LCD_CHAR_H))
    {
        LCD_RANG_POST();
        return;
    }

	nStringLen = strlen((char*)p);
	if (nStringLen > LCD_W / 8)
	{
		nStringLen = LCD_W / 8;
	}
	x2 = x + nStringLen * 8;
	for (ii = 0; ii < 16; ii++)
	{
		k  = 0;
		pp = (INT8U*)p;
		memset(&nBuf[0], 0, sizeof(nBuf));
		while(1)
		{
			if (k >= nStringLen)
			{
				break;
			}
			if ((pp[0] & 0x80) == 0)
			{
				offset = (*pp) * 0x10;
				memcpy(&nTmp[0], (INT8U*)(CHAR_K_OFFSET + offset), 16);
				
				nBuf[k] = SwapBit(nTmp[ii]);
				pp += 1; 
				k += 1;
			}
			else
			{
				qh = pp[0] - 0xa0; 	//获得区码 (S1 - (0x20 - 0x80))
			    wh = pp[1] - 0xa0;	//获得位码 (S2 - (0x20 - 0x80))

			    offset = (94 * (qh - 1) + (wh - 1)) * 32; //得到偏移位置
			   	memcpy(&nTmp[0], (INT8U*)(HZK_OFFSET + offset), 32);

				nBuf[k] = SwapBit(nTmp[ii * 2]);
				nBuf[k + 1] = SwapBit(nTmp[ii * 2 + 1]);

				pp += 2; 
				k += 2;
			}
		}

		for (i = 0; i < nStringLen * 8; i++)
		{
			if (!nMode)
			{
				ClrBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				if (TestBufBit(&nBuf[0], sizeof(nBuf), i))
				{
					SetBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				}
			}
			else
			{
				SetBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				if (TestBufBit(&nBuf[0], sizeof(nBuf), i))
				{
					ClrBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				}
			}
		}
		
		memset(&nBuf[0], 0, sizeof(nBuf));
		for (i = 0; i < 20 * 8; i++)
		{
			if (TestBufBit(&g_nScreenBuf[ii + y][0], 20, i))
			{
				if (i % 2)
				{
					nBuf[i / 2] |= 0x0F;
				}
				else
				{
					nBuf[i / 2] |= 0xF0;
				}
			}
		}
		xx = 0;
		for (i = 0; i < 27; i++) 
		{
			if ((xx + 6) > x)
			{
				UC1698SetRamStart(xx, y + ii); 
				break;
			}
			xx += 6;
		}
		//LCD_RANG_PEND(1);
		xx = 0;
	    for (i = 0; i < 27; i++)       
	    {
			if ((xx + 6) > x)
			{
				LCD_WR_DATA_SUB(nBuf[i * 3 + 0]);
			    LCD_WR_DATA_SUB(nBuf[i * 3 + 1]);
			    LCD_WR_DATA_SUB(nBuf[i * 3 + 2]);
			}
			xx += 6;
			if (xx >= x2)
			{
				break;
			}
	    }
	    //LCD_RANG_POST();
	}
    LCD_RANG_POST();
	#endif
}


/********************************************************************************
* 函数名:LCD_ShowCharString
* 输　入:x,y:起点坐标 *p:字符串起始地址 用16字体
* 输　出:
* 返  回:
* 功  能:显示字符串
*********************************************************************************/
void LCD_ShowCharString(INT16U x,INT16U y, INT8U nRows, const INT8U *p, INT8U nMode)
//void LCD_ShowCharString(INT16U x,INT16U y, const INT8U *p, INT8U nMode)
{
	INT32U 		nStringLen 	= 0;
	INT8U  		nBuf[81] 		= {0};
	INT8U  		k  				= 0;
	INT8U* 		pp  			= (INT8U*)p;
	INT8U 		qh 				= 0; 
	INT8U 		wh 				= 0; 
	INT32U		offset 			= 0;
	INT8U  		nTmp[HZ_BYTES] 	= {0};
	INT16U		ii 				= 0;
	INT8U    	i				= 0;
	INT8U    	xx				= 0;
	INT8U    	x2				= 0;
	INT8U		nAscCnt			= 0;

    LCD_RANG_PEND(1);
    if ( (x > LCD_W - LCD_CHAR_W * 2) || (y > LCD_H - LCD_CHAR_H))
    {
        LCD_RANG_POST();
        return;
    }

	nStringLen = strlen((char*)p);
	if (nStringLen > LCD_W / 8)
	{
		nStringLen = LCD_W / 8;
	}
	x2 = x + nStringLen * 8;
	
	//for (ii = 0; ii < 16; ii++)
    for (ii = 0; ii < nRows; ii++)
	{
		nAscCnt = 0;
		k  = 0;
		pp = (INT8U*)p;
		memset(&nBuf[0], 0, sizeof(nBuf));
		while(1)
		{
			if (k >= nStringLen)
			{
				break;
			}
			if ((pp[0] & 0x80) == 0)
			{
				offset = (*pp) * 0x10;
				memcpy(&nTmp[0], (INT8U*)(CHAR_K_OFFSET + offset), 16);
				
				nBuf[k] = SwapBit(nTmp[ii]);
				pp += 1; 
				k += 1;
				nAscCnt++;
			}
			else
			{
				qh = pp[0] - 0xa0; 	//获得区码 (S1 - (0x20 - 0x80))
			    wh = pp[1] - 0xa0;	//获得位码 (S2 - (0x20 - 0x80))

			    offset = (94 * (qh - 1) + (wh - 1)) * 32; //得到偏移位置
			   	memcpy(&nTmp[0], (INT8U*)(HZK_OFFSET + offset), 32);

				nBuf[k] = SwapBit(nTmp[ii * 2]);
				nBuf[k + 1] = SwapBit(nTmp[ii * 2 + 1]);

				pp += 2; 
				k += 2;
			}
		}
		if (nAscCnt < nStringLen)
		{
            LCD_RANG_POST();
			return;
		}
		if (ii < 2)
		{
			continue;
		}
		
		if (ii >= 14)
		{
			continue;
		}

		for (i = 0; i < nStringLen * 8; i++)
		{
			if (!nMode)
			{
				ClrBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				if (TestBufBit(&nBuf[0], sizeof(nBuf), i))
				{
					SetBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				}
			}
			else
			{
				SetBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				if (TestBufBit(&nBuf[0], sizeof(nBuf), i))
				{
					ClrBufBit(&g_nScreenBuf[ii + y][0], 20, x + i);
				}
			}
		}
		
		memset(&nBuf[0], 0, sizeof(nBuf));
		for (i = 0; i < 20 * 8; i++)
		{
			if (TestBufBit(&g_nScreenBuf[ii + y][0], 20, i))
			{
				if (i % 2)
				{
					nBuf[i / 2] |= 0x0F;
				}
				else
				{
					nBuf[i / 2] |= 0xF0;
				}
			}
		}
		xx = 0;
		for (i = 0; i < 27; i++) 
		{
			if ((xx + 6) > x)
			{
				UC1698SetRamStart(xx, y + ii); 
				break;
			}
			xx += 6;
		}
		//LCD_RANG_PEND(1);
		xx = 0;
	    for (i = 0; i < 27; i++)       
	    {
			if ((xx + 6) > x)
			{
				LCD_WR_DATA_SUB(nBuf[i * 3 + 0]);
			    LCD_WR_DATA_SUB(nBuf[i * 3 + 1]);
			    LCD_WR_DATA_SUB(nBuf[i * 3 + 2]);
			}
			xx += 6;
			if (xx >= x2)
			{
				break;
			}
	    }
	    //LCD_RANG_POST();
	}
    LCD_RANG_POST();
}


void LCD_ShowLogo(INT16U x,INT16U y, INT16U nOnceLen, INT32U nSize, INT8U* pLogo, INT16U nBackColour, INT16U nDrawColour, INT8U nMode)
{
    INT32U      i 			= 0;
	INT8U		j			= 0;
    INT16U 		nColrTmp1, nColrTmp2;
	
    if(!nMode)
    {
        nColrTmp1 = nDrawColour;
        nColrTmp2 = nBackColour;        
    }
    else
    {
        nColrTmp1 = nBackColour;
        nColrTmp2 = nDrawColour;     
    }
    //设置窗口	
    LCD_RANG_PEND(1);
    //Address_set(x, y, x + nOnceLen - 1, y + nSize / nOnceLen - 1);      //设置光标位置 
    for(i = 0; i < nSize / 8; i++)
    {
        if (((i * 8) % nOnceLen) == 0)
        {
            UC1698SetRamStart(x + ((i * 8) % nOnceLen), y + (i * 8) / nOnceLen);
        }
        for(j = 0;j < 8;j++)
        {
#if 1
            if (pLogo[i] & g_nBit[7 - j])
            {
                LCD_WR_DATA(x + ((i * 8) % nOnceLen) + j, y + (i * 8) / nOnceLen, nColrTmp1);
            }
            else
            {
                LCD_WR_DATA(x + ((i * 8) % nOnceLen) + j, y + (i * 8) / nOnceLen, nColrTmp2);
            }
#else
            if (pLogo[i] & g_nBit[7 - j]) 
            {
                LCD_WR_DATA(x + j + (i % 2) * 8, y + i / 2, nColrTmp1);
            }
            else
            {
                LCD_WR_DATA(x + j + (i % 2) * 8, y + i / 2, nColrTmp2);
            }
#endif
            
        }
    } 
	LCD_RANG_POST();
	
}

void LCD_ShowAsciiLogo(INT16U x,INT16U y, INT16U nOnceLen, INT32U nSize, INT8U* pLogo1, INT16U nBackColour, INT16U nDrawColour, INT8U nMode)
{
#define 	ASCII_DISP_NUM    16
	INT32U      i 			= 0;
	INT8U		j			= 0;
    INT16U 		nColrTmp1, nColrTmp2;
	INT8U*		pLogo		= NULL;
	
	pLogo = (INT8U*)malloc(nSize / 8);
	memset(pLogo, 0, nSize / 8);
	for (i = 0; i < ASCII_DISP_NUM; i++)
	{
		for (j = 0; j < (nSize / 8 / ASCII_DISP_NUM); j++)
		{
			pLogo[i * (nSize / 8 / ASCII_DISP_NUM) + j] = pLogo1[ASCII_DISP_NUM * j + i];
		}
	}
	
    if(!nMode)
    {
        nColrTmp1 = nDrawColour;
        nColrTmp2 = nBackColour;        
    }
    else
    {
        nColrTmp1 = nBackColour;
        nColrTmp2 = nDrawColour;     
    }

    //设置窗口	
    LCD_RANG_PEND(1);
    Address_set(x, y, x + nOnceLen - 1, y + nSize / nOnceLen - 1);      //设置光标位置 
    for(i = 0; i < nSize / 8; i++)
    {
        for(j = 0;j < 8;j++)
        {
            if (pLogo1[i] & g_nBit[7 - j]) 
            {
                LCD_WR_DATA(x + j + (i % 2) * 8, y + i / 2, nColrTmp1);
            }
            else
            {
                LCD_WR_DATA(x + j + (i % 2) * 8, y + i / 2, nColrTmp2);
            }
            
        }
    } 
    LCD_RANG_POST();
	free(pLogo);
}

void ClearScreenUC1698U(INT8U nData)
{
	INT16U i;
	 	
    UC1698WriteCmd(0x60);			//row address LSB	
    UC1698WriteCmd(0x70);			//row address MSB
	
    UC1698WriteCmd(0x05);			//Column address LSB	
    UC1698WriteCmd(0x12);			//Column address MSB
    
    memset(g_nScreenBuf,0,sizeof(g_nScreenBuf));        //清空显示屏
    
	for(i=0;i<12960;i++)
    {
        LCD_WR_DATA_SUB(nData);			    
    }
}



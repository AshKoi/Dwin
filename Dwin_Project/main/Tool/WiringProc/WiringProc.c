#include "WiringProc.h"
#include "base.h"



INT8U wiringStatusBitmap    = 0;
INT8U g_WiringErr           = 0;
INT8S secondaryResult       = -1;

/*读引脚P40~P4A的staus（GPIO默认输入，上拉使能）*/
INT8U ReadWiringStatus(void)
{
	INT8U ret = 0;  // 初始化返回值为 0
	
	// 读取 STA1 
    if( GpioGetInput( IO_PORT4, IO_PINx0 ) == 1 )   
        ret |= 0x01;
    else 
        ret &=~ 0x01;
    // 读取 STA2
    if( GpioGetInput( IO_PORT4, IO_PINx1 ) == 1 )
        ret |= 0x02;
    else 
        ret &=~ 0x02;
    // 读取 STA3
    if( GpioGetInput( IO_PORT4, IO_PINx2 ) == 1 )
        ret |= 0x04;
    else 
        ret &=~ 0x04;
    // 读取 STA4
    if( GpioGetInput( IO_PORT4, IO_PINx3 ) == 1 )
        ret |= 0x08;
    else 
        ret &=~ 0x08;
    // 读取 STA5
    if( GpioGetInput( IO_PORT4, IO_PINx4 ) == 1 )
        ret |= 0x10;
    else 
        ret &=~ 0x10;
    // 读取 STA6
    if( GpioGetInput( IO_PORT4, IO_PINx9 ) == 1 )
        ret |= 0x20;        
    else 
        ret &=~ 0x20;
	
    return ret;
}

// 初判函数
INT8S initialJudgment() 
{
    INT8U status = 0;        
	
	wiringStatusBitmap = ReadWiringStatus();
	status = wiringStatusBitmap & 0x3F;				// STA7不判
    switch (status) {
        case 0x38:
            return 0;                                // 初判，LN无反接
        case 0x20:
            return 1;                                // 初判，LN反接
        default:
            return -1;                               // 接线错误
    }
}


// 继电器动作后再次判断
INT8S secondaryJudgment(INT8S initialResult) 
{
	INT8U status = 0;
		
    if (initialResult == 0) 
	{
        // 继电器控制 IC_CTL4 动作（置 0）
        GpioClearOutput(IO_PORT7, IO_PINx7);
    } 
	else if (initialResult == 1) 
	{
        // 继电器控制 IC_CTL5 动作（置 0）
        GpioClearOutput(IO_PORT7, IO_PINx6);
    }
	OSTimeDlyHMSM(0, 0, 0, 5); // 等待一段时间
    wiringStatusBitmap = ReadWiringStatus();
    status	= wiringStatusBitmap & 0x3F; // 忽略 STA7 位
    switch (status) {
        case 0x11:
            return 2; // A相接L2，N接L1
        case 0x01:
            return 3; // A相接L1，N接L2
        case 0x12:
            return 4; // B相接L2，N接L1
        case 0x02:
            return 5; // B相接L1，N接L2
        case 0x14:
            return 6; // C相接L2，N接L1
        case 0x04:
            return 7; // C相接L1，N接L2
        default:
            return -1; // 接线错误
    }
}

// 判断线路是否正常
BOOLEAN isWiringNormal() 
{
    INT8S initialResult = initialJudgment();
    //不是LN正常接线或者反接，则一直提示接线错误，不进行下一步
    if (initialResult == -1) {
        // 初判接线错误
        secondaryResult = -1;
        return FALSE;
    }

    secondaryResult = secondaryJudgment(initialResult);
    //不是状态2~状态7的值，提示接线错误，不进行下一步
    if (secondaryResult == -1) {
        // 二次判断接线错误
        return FALSE;
    }

    // 连接状态正常
    return TRUE;
}





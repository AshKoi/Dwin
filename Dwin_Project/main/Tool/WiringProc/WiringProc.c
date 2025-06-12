#include "WiringProc.h"
#include "base.h"



INT8U wiringStatusBitmap    = 0;
INT8U g_WiringErr           = 0;
INT8S secondaryResult       = -1;

/*������P40~P4A��staus��GPIOĬ�����룬����ʹ�ܣ�*/
INT8U ReadWiringStatus(void)
{
	INT8U ret = 0;  // ��ʼ������ֵΪ 0
	
	// ��ȡ STA1 
    if( GpioGetInput( IO_PORT4, IO_PINx0 ) == 1 )   
        ret |= 0x01;
    else 
        ret &=~ 0x01;
    // ��ȡ STA2
    if( GpioGetInput( IO_PORT4, IO_PINx1 ) == 1 )
        ret |= 0x02;
    else 
        ret &=~ 0x02;
    // ��ȡ STA3
    if( GpioGetInput( IO_PORT4, IO_PINx2 ) == 1 )
        ret |= 0x04;
    else 
        ret &=~ 0x04;
    // ��ȡ STA4
    if( GpioGetInput( IO_PORT4, IO_PINx3 ) == 1 )
        ret |= 0x08;
    else 
        ret &=~ 0x08;
    // ��ȡ STA5
    if( GpioGetInput( IO_PORT4, IO_PINx4 ) == 1 )
        ret |= 0x10;
    else 
        ret &=~ 0x10;
    // ��ȡ STA6
    if( GpioGetInput( IO_PORT4, IO_PINx9 ) == 1 )
        ret |= 0x20;        
    else 
        ret &=~ 0x20;
	
    return ret;
}

// ���к���
INT8S initialJudgment() 
{
    INT8U status = 0;        
	
	wiringStatusBitmap = ReadWiringStatus();
	status = wiringStatusBitmap & 0x3F;				// STA7����
    switch (status) {
        case 0x38:
            return 0;                                // ���У�LN�޷���
        case 0x20:
            return 1;                                // ���У�LN����
        default:
            return -1;                               // ���ߴ���
    }
}


// �̵����������ٴ��ж�
INT8S secondaryJudgment(INT8S initialResult) 
{
	INT8U status = 0;
		
    if (initialResult == 0) 
	{
        // �̵������� IC_CTL4 �������� 0��
        GpioClearOutput(IO_PORT7, IO_PINx7);
    } 
	else if (initialResult == 1) 
	{
        // �̵������� IC_CTL5 �������� 0��
        GpioClearOutput(IO_PORT7, IO_PINx6);
    }
	OSTimeDlyHMSM(0, 0, 0, 5); // �ȴ�һ��ʱ��
    wiringStatusBitmap = ReadWiringStatus();
    status	= wiringStatusBitmap & 0x3F; // ���� STA7 λ
    switch (status) {
        case 0x11:
            return 2; // A���L2��N��L1
        case 0x01:
            return 3; // A���L1��N��L2
        case 0x12:
            return 4; // B���L2��N��L1
        case 0x02:
            return 5; // B���L1��N��L2
        case 0x14:
            return 6; // C���L2��N��L1
        case 0x04:
            return 7; // C���L1��N��L2
        default:
            return -1; // ���ߴ���
    }
}

// �ж���·�Ƿ�����
BOOLEAN isWiringNormal() 
{
    INT8S initialResult = initialJudgment();
    //����LN�������߻��߷��ӣ���һֱ��ʾ���ߴ��󣬲�������һ��
    if (initialResult == -1) {
        // ���н��ߴ���
        secondaryResult = -1;
        return FALSE;
    }

    secondaryResult = secondaryJudgment(initialResult);
    //����״̬2~״̬7��ֵ����ʾ���ߴ��󣬲�������һ��
    if (secondaryResult == -1) {
        // �����жϽ��ߴ���
        return FALSE;
    }

    // ����״̬����
    return TRUE;
}





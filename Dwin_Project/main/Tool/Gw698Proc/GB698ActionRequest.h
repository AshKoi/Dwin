#ifndef _GB698_ACTION_REQUEST_H_
#define _GB698_ACTION_REQUEST_H_

#include "Gw698.h"

/**************************************************************************************************
                                             类型定义
***************************************************************************************************/
enum    _tagGW698COMDCB
{
    e_UART_BPS_300 = 300,
    e_UART_BPS_600 = 600,
    e_UART_BPS_1200 = 1200,
    e_UART_BPS_2400 = 2400,
    e_UART_BPS_4800 = 4800,
    e_UART_BPS_7200 = 7200,
    e_UART_BPS_9600 = 9600,
    e_UART_BPS_19200 = 19200,
    e_UART_BPS_38400 = 38400,
    e_UART_BPS_57600 = 57600,
    e_UART_BPS_115200 = 115200,
    e_UART_BPS_END
};
/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
INT8S GB698ActionRequest(pAPP_COMM pComm);
INT8S GB698ActionRequestSendResp(INT8U*  pRecData, INT8U *pData, INT8U *pDatalen);
INT8S GB698AddAssociationObject(INT8U*  pRecData, INT8U nFn);
INT8S GB698RemoveAssociationObject(INT8U*  pRecData, INT8U nFn);
#endif
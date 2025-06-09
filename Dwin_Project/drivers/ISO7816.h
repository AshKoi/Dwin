/***************************************************************
****************************************************************/
#ifndef _ISO7816_HEADER_
#define _ISO7816_HEADER_


#define ESAMSPI     1

#define ESAMSPIBAUDRATE		500000



BOOLEAN ISO7816Reset( void );
INT8U ESAMGetID( INT8U *buff );
INT8U ISO7816Init( void );
BOOLEAN ESAM_Exec_Command(INT8U *comm, INT16U len, INT8U *resp, INT16U *lenr);
void TestESAMLongPacket( void );

////////////////////////////////////////////////////////////////////////////////
/* 复位ESAM芯片 */
void ESAMReset( void );

/* 读取ESAM ID
 *
 * 参数：buff: ID指针，存放ID数据
 * 返回：ID长度
 */
INT8U ESAMGetID( INT8U *buff );

/* 向ESAM发送命令并接收返回信息
 *
 * 参数：comm: 命令数据指针
 *        len: 命令长度
 *       resp: 返回数据指针
 *       lenr: 返回数据长度指针
 *
 * 返回：TRUE：命令执行成功
 *      FALSE：命令执行失败
 */
BOOLEAN ESAMExecCommand(INT8U *comm, INT16U len, INT8U *resp, INT16U *lenr);


#endif
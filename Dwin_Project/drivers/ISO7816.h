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
/* ��λESAMоƬ */
void ESAMReset( void );

/* ��ȡESAM ID
 *
 * ������buff: IDָ�룬���ID����
 * ���أ�ID����
 */
INT8U ESAMGetID( INT8U *buff );

/* ��ESAM����������շ�����Ϣ
 *
 * ������comm: ��������ָ��
 *        len: �����
 *       resp: ��������ָ��
 *       lenr: �������ݳ���ָ��
 *
 * ���أ�TRUE������ִ�гɹ�
 *      FALSE������ִ��ʧ��
 */
BOOLEAN ESAMExecCommand(INT8U *comm, INT16U len, INT8U *resp, INT16U *lenr);


#endif
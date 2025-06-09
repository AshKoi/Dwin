
#ifndef _USBAPI_HEADER_
#define	_USBAPI_HEADER_

//#include "config.h"
//#include "usbhost_fat.h"
#include "global.h"
#include "ff.h"

#define USB_MODE_RO			1
#define USB_MODE_RW			2
#define USB_MODE_APPEND		3

#define SEEK_FROM_BEGIN		0
#define SEEK_FROM_CURRENT	1
#define SEEK_FROM_END		2

#define USB_ERROR_SEEK      4

/* USB Host */
#define ERRORNO_BASE				0x80000000
#define ERROR(x)					((x)&ERRORNO_BASE)



typedef struct _tagFatInfo {
	INT32U nBytesPerSec;	// ÿ�����ֽ���
	INT32U nBytesPerClus;	// ÿ���ֽ���
	INT32U nSecPerClus;		// ÿ��������

	INT32U nFatTblNums;		// fat�������һ��Ϊ2
	INT32U nFatTblSize;		// ����fat��ռ�õ�������
	
	INT32U nFirstDataSec;	// ��һ��������ݵ�������

	INT32U nTotalSec;		// ������������
	
	INT32U nRootClusNo;		// ��Ŀ¼��ʼ�غţ�ͨ��Ϊ2
	INT32U nRootDirSec;		// ��Ŀ¼��ʼ������

	INT32U nRsvdSecCnt;		// ����������Ŀ

	INT8U  FatType;         // FAT����: FAT_12, FAT_16, FAT_32
    INT8U  nFatItemBytes;   // ����FAT����ռ���ֽ���: FAT16 - 2 bytes
                            //                        FAT32 - 4 bytes
} FATINFO, *PFATINFO;


/* ��ʼ�� USB Host    ��׼����дU�̣� mount�ļ�ϵͳ
 * ��������
 *
 * ����ֵ��TRUE  - ��ʼ���ɹ�
 *         
 */
BOOLEAN USBHostInit(void);



/* ����USB�豸
 * ��������
 *
 * ����ֵ��TRUE  - �ҵ��豸�������豸��ʼ�������
 *         FALSE - δ�ҵ��豸�������豸��δ��ʼ�����
 */
BOOLEAN	USBScan( void );

/*
 * ֹͣUSB�豸
 * ��������
 *
 * ����ֵ��TRUE  - ��ֹͣ
 *         FALSE - δֹͣ
 */
BOOLEAN USBStop( void );

/*
 * ��ѯUSB�豸�Ƿ����
 * ��������
 *
 * ����ֵ��TRUE  - �Ѿ���
 *         FALSE - δ����
 */
BOOLEAN USBIsReady( void );


/* ���ص�ǰ���̷�����Ϣ
 * ������  [in][out] fatinfo
 * ����ֵ��USB_SUCC
 */
INT32U USBGetFatInfo( FATINFO* fatinfo );


/* ��U�̸�Ŀ¼��һ���ļ�
 *
 * ������[in] filename �ļ����ƣ���ʱֻ֧��MS-DOS��8.3��ʽ����֧�ֳ��ļ�������֧��·����
 *       [in] mode     ����ģʽ
 *
 *            USB_MODE_RW  - ��дģʽ
 *            USB_MODE_RO  - ֻ��ģʽ
 *            USB_MODE_CR  - ����ģʽ
 *
 * ����ֵ���ļ��򿪳ɹ����򷵻�һ����С������ļ������
 *         ���򷵻�һ��С����Ĵ����롣
 *
 */
INT32U USBFileOpen( INT8U* filename, INT8U mode );


/* ��U�̸�Ŀ¼��һ��ָ�����ļ��ж�ȡ����
 *
 * ������[in]     fd     �ļ������
 *       [in,out] buffer ��Ŷ������ݵĻ�����
 *       [in]     len    ���볤��
 *
 *
 * ����ֵ��ʵ�ʶ�������ݳ���
 *
 * ע�ͣ� �ú������ܶ�ȡ�����ڵ�ǰ����һ���صĳ��ȣ��������£�
 *        ���赱ǰ��һ���صĴ�СΪ 4096 �ֽڣ�
 *
 *        USBFileSeek( fHandle, 0, SEEK_FROM_BEGIN );
 *        nCurrRead = USBFileOpen( fHandle, buffer, 4100 );    // ��������������򷵻�ֵӦΪ��4096��
 *
 *        USBFileSeek( fHandle, 32, SEEK_FROM_BEGIN );
 *        nCurrRead = USBFileOpen( fHandle, buffer, 4100 );    // ��������������򷵻�ֵӦΪ��4096 - 32��
 *
 */
INT32U USBFileRead( INT32U fd, INT8U* buffer, INT32U len );


/* ��U�̸�Ŀ¼��һ��ָ�����ļ���д������
 *
 * ������[in]     fd     �ļ������
 *       [in]     buffer ���д�����ݵĻ�����
 *       [in]     len    д�볤��
 *
 *
 * ����ֵ��ʵ��д������ݳ��ȣ�ʹ�÷���ͬ USBFileRead��
 *
 */
INT32U USBFileWrite( INT32U fd, INT8U* buffer, INT32U len );


/* �ر��ļ����
 *
 * ������[in]     fd     �ļ������
 *
 * ����ֵ����������δʹ��
 */
INT32U  USBFileClose( INT32U  fd );


/*
 * ����ļ�����
 *
 * ������[in]     fd     �ļ������
 *
 * ����ֵ���ļ�����
 */
INT32U USBFileLength( INT32U fd );


/* �ƶ��ļ���дָ��
 *
 * ������[in]     fd     �ļ������
 *       [in] offset     ƫ����
 *       [in]   mode     ƫ��ģʽ
 *
 *              SEEK_FROM_BEGIN		 ���ļ�ͷ����ʼ
 *				SEEK_FROM_CURRENT	 �ӵ�ǰָ�봦��ʼ
 *				SEEK_FROM_END		 ���ļ�β����ʼ
 *
 * ����ֵ����ǰָ������λ��
 */
INT32U USBFileSeek( INT32U fd, INT32S offset, INT8U mode );


/* �鿴�ļ��Ƿ����
 *
 * ������[in] filename �ļ�����
 *
 * ����ֵ�� TRUE - �ļ�����
 *          FALSE- �ļ�������
 */
BOOLEAN USBFileExists( INT8U* filename );


////////////////////////////////////////////////////////////////////////////////
BOOLEAN _API_USBHostInit( void );
INT32U  _API_USBFileOpen( INT8U* filename, INT8U mode );
INT32U  _API_USBFileRead( INT32U fd, INT8U* buffer, INT32U len );
INT32U  _API_USBFileWrite( INT32U fd, INT8U* buffer, INT32U len );
INT32U  _API_USBFileClose( INT32U  fd );
INT32U  _API_USBFileLength( INT32U fd );
INT32U  _API_USBFileSeek( INT32U fd, INT32S offset, INT8U mode );
BOOLEAN _API_USBFileExists( INT8U* filename );
BOOLEAN	_API_USBScan( void );
BOOLEAN _API_USBStop( void );
BOOLEAN _API_USBHostReset( INT32U time );

BOOLEAN _API_USBIsReady( void );
INT32U  _API_USBGetFatInfo( FATINFO* fatinfo );

#endif

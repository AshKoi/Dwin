
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
	INT32U nBytesPerSec;	// 每扇区字节数
	INT32U nBytesPerClus;	// 每簇字节数
	INT32U nSecPerClus;		// 每簇扇区数

	INT32U nFatTblNums;		// fat表个数，一般为2
	INT32U nFatTblSize;		// 单个fat表占用的扇区数
	
	INT32U nFirstDataSec;	// 第一个存放数据的扇区号

	INT32U nTotalSec;		// 磁盘总扇区数
	
	INT32U nRootClusNo;		// 根目录起始簇号，通常为2
	INT32U nRootDirSec;		// 根目录起始扇区号

	INT32U nRsvdSecCnt;		// 保留扇区数目

	INT8U  FatType;         // FAT类型: FAT_12, FAT_16, FAT_32
    INT8U  nFatItemBytes;   // 单个FAT表项占用字节数: FAT16 - 2 bytes
                            //                        FAT32 - 4 bytes
} FATINFO, *PFATINFO;


/* 初始化 USB Host    ，准备读写U盘， mount文件系统
 * 参数：无
 *
 * 返回值：TRUE  - 初始化成功
 *         
 */
BOOLEAN USBHostInit(void);



/* 查找USB设备
 * 参数：无
 *
 * 返回值：TRUE  - 找到设备，并且设备初始化已完成
 *         FALSE - 未找到设备，或者设备尚未初始化完成
 */
BOOLEAN	USBScan( void );

/*
 * 停止USB设备
 * 参数：无
 *
 * 返回值：TRUE  - 已停止
 *         FALSE - 未停止
 */
BOOLEAN USBStop( void );

/*
 * 查询USB设备是否就绪
 * 参数：无
 *
 * 返回值：TRUE  - 已就绪
 *         FALSE - 未就绪
 */
BOOLEAN USBIsReady( void );


/* 返回当前磁盘分区信息
 * 参数：  [in][out] fatinfo
 * 返回值：USB_SUCC
 */
INT32U USBGetFatInfo( FATINFO* fatinfo );


/* 打开U盘根目录的一个文件
 *
 * 参数：[in] filename 文件名称，暂时只支持MS-DOS的8.3格式，不支持长文件名，不支持路径；
 *       [in] mode     操作模式
 *
 *            USB_MODE_RW  - 读写模式
 *            USB_MODE_RO  - 只读模式
 *            USB_MODE_CR  - 创建模式
 *
 * 返回值：文件打开成功，则返回一个不小于零的文件句柄；
 *         否则返回一个小于零的错误码。
 *
 */
INT32U USBFileOpen( INT8U* filename, INT8U mode );


/* 从U盘根目录的一个指定的文件中读取数据
 *
 * 参数：[in]     fd     文件句柄；
 *       [in,out] buffer 存放读入数据的缓冲区
 *       [in]     len    读入长度
 *
 *
 * 返回值：实际读入的数据长度
 *
 * 注释： 该函数仅能读取不大于当前磁盘一个簇的长度，举例如下，
 *        假设当前盘一个簇的大小为 4096 字节；
 *
 *        USBFileSeek( fHandle, 0, SEEK_FROM_BEGIN );
 *        nCurrRead = USBFileOpen( fHandle, buffer, 4100 );    // 如果操作正常，则返回值应为（4096）
 *
 *        USBFileSeek( fHandle, 32, SEEK_FROM_BEGIN );
 *        nCurrRead = USBFileOpen( fHandle, buffer, 4100 );    // 如果操作正常，则返回值应为（4096 - 32）
 *
 */
INT32U USBFileRead( INT32U fd, INT8U* buffer, INT32U len );


/* 向U盘根目录的一个指定的文件中写入数据
 *
 * 参数：[in]     fd     文件句柄；
 *       [in]     buffer 存放写入数据的缓冲区
 *       [in]     len    写入长度
 *
 *
 * 返回值：实际写入的数据长度，使用方法同 USBFileRead；
 *
 */
INT32U USBFileWrite( INT32U fd, INT8U* buffer, INT32U len );


/* 关闭文件句柄
 *
 * 参数：[in]     fd     文件句柄；
 *
 * 返回值：保留，暂未使用
 */
INT32U  USBFileClose( INT32U  fd );


/*
 * 获得文件长度
 *
 * 参数：[in]     fd     文件句柄；
 *
 * 返回值：文件长度
 */
INT32U USBFileLength( INT32U fd );


/* 移动文件读写指针
 *
 * 参数：[in]     fd     文件句柄；
 *       [in] offset     偏移量
 *       [in]   mode     偏移模式
 *
 *              SEEK_FROM_BEGIN		 从文件头部开始
 *				SEEK_FROM_CURRENT	 从当前指针处开始
 *				SEEK_FROM_END		 从文件尾部开始
 *
 * 返回值：当前指针所在位置
 */
INT32U USBFileSeek( INT32U fd, INT32S offset, INT8U mode );


/* 查看文件是否存在
 *
 * 参数：[in] filename 文件名称
 *
 * 返回值： TRUE - 文件存在
 *          FALSE- 文件不存在
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

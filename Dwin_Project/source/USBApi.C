
#include "global.h"
#include "apierrno.h"
#include <string.h>
#include "USBClassSupervisor.h"

#define VFAT_MAXLEN_BYTES   256
#define MAX_FILE_ENTRY		1
static FIL FAT_FileHandles[MAX_FILE_ENTRY];

//#define TASK_USBHOST_PRIO           40
//#define TASK_USBHOST_STK_SIZE       (256)
//OS_STK TaskUSBHostStk[TASK_USBHOST_STK_SIZE];


INT8U UpperCase( INT8U ch )
{
	if( (ch >= 0x61) && (ch<=0x7A) )
		return (ch-0x20);
	return ch;
}

/*#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"*/

//#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
//  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
//    #pragma data_alignment=4
//  #endif
//#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
//__ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;
//
//#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
//  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
//    #pragma data_alignment=4
//  #endif
//#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
//__ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;

/*
#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
  USBH_OTG_ISR_Handler(&USB_OTG_Core);
}
*/
/*
void TaskUSBHost( void *parg )
{
    while( 1 )
    {
        USBH_Process(&USB_OTG_Core, &USB_Host);
        OSTimeDly( OS_TICKS_PER_SEC/100 );
    }
}
*/
/*
BOOLEAN USBHostInit( void )
{
  // Init Host Library 
  USBH_Init(&USB_OTG_Core,
#ifdef USE_USB_OTG_FS
            USB_OTG_FS_CORE_ID,
#else
            USB_OTG_HS_CORE_ID,
#endif
            &USB_Host,
            &USBH_MSC_cb,
            &USR_cb);	

    OSTaskCreateExt((void (*)(void *)) TaskUSBHost,
                         (void          * ) 0,
                         (OS_STK        * )&TaskUSBHostStk[TASK_USBHOST_STK_SIZE - 1],
                         (INT8U           ) TASK_USBHOST_PRIO,
                         (INT16U          ) TASK_USBHOST_PRIO,
                         (OS_STK        * )&TaskUSBHostStk[0],
                         (INT32U          ) TASK_USBHOST_STK_SIZE,
                         (void          * )0,
                         (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

	return TRUE;
}
*/

extern FATFS fatfs;

FATFS fat_fs[1];                //在初始化的时候mount驱动
BOOLEAN USBHostInit( void )
{
    UsbInit();
    UsbHost_Init();
    f_mount(0,fat_fs);   
    return TRUE;
}


BOOLEAN	USBScan( void )
{
    if( UsbClassSupervisor_GetActiveDriver() > 0 )
    {
        
        return TRUE;
    }
	return FALSE;
}

/*
 * 停止USB设备
 * 参数：无
 *
 * 返回值：TRUE  - 已停止
 *         FALSE - 未停止
 */
BOOLEAN USBStop( void )
{

    /* umount file system */
    f_mount( 0, NULL );
    UsbHost_DeInit();

    /* Manage User disconnect operations*/
//    phost->usr_cb->DeviceDisconnected();

    /* Re-Initilaize Host for new Enumeration */
//    USBH_DeInit(pdev, phost);
//    phost->usr_cb->DeInit();
//    phost->class_cb->DeInit(pdev, &phost->device_prop);
//    USBH_DeAllocate_AllChannel(pdev);
//    phost->gState = HOST_IDLE;

	//BSP_GPIO_Set( BSP_GPIO_PORT_B, DEF_BIT_29 );	// 关闭 USB Led 指示灯
	//BSP_GPIO_Set( BSP_GPIO_PORT_C, DEF_BIT_20 );	// 关闭 USB Host 电源
	return TRUE;
}


/*
 * 查询USB设备是否就绪
 * 参数：无
 *
 * 返回值：TRUE  - 已就绪
 *         FALSE - 未就绪
 */
BOOLEAN USBIsReady( void )
{
//    if( HCD_IsDeviceConnected(&USB_OTG_Core) == 1 )
//        return TRUE;
//    else
//        return FALSE;
    
    if(BootloaderAPI_InitMassStorage() == TRUE)
        return TRUE;
    else
        return FALSE;
    
}


/*
 * 返回当前磁盘分区信息
 */
INT32U USBGetFatInfo( FATINFO* fatinfo )
{
/*    INT32U vol = 0;
    if( fatinfo == NULL )
        return USB_ERROR_ARGUMENTS;

    extern FATFS *GetVolumeInfo( UINT vol );
    FATFS *pFatFs = GetVolumeInfo( 0 );

	fatinfo->nSecPerClus  = pFatFs->csize;		        // 每簇扇区数
	fatinfo->nBytesPerSec = 512;	                    // 每扇区字节数
	fatinfo->nBytesPerClus = fatinfo->nBytesPerSec * fatinfo->nSecPerClus;	// 每簇字节数

	fatinfo->nFatTblNums= pFatFs->n_fats;		// fat表个数，一般为2
	fatinfo->nFatTblSize= pFatFs->sects_fat;	// 单个fat表占用的扇区数
	
	fatinfo->nFirstDataSec = pFatFs->database;	// 第一个存放数据的扇区号

	fatinfo->nTotalSec = 0;		// 磁盘总扇区数
	
	fatinfo->nRootClusNo = pFatFs->dirbase;		// 根目录起始簇号，通常为2
	fatinfo->nRootDirSec = 0;		// 根目录起始扇区号

	fatinfo->nRsvdSecCnt = 0;		// 保留扇区数目

	fatinfo->FatType = pFatFs->fs_type;         // FAT类型: FAT_12, FAT_16, FAT_32
    fatinfo->nFatItemBytes = 0;   // 单个FAT表项占用字节数: FAT16 - 2 bytes
                                //                        FAT32 - 4 bytes*/

    return USB_SUCC;
}

////////////////////////////////////////////////////////////////////////////////

// 获取空闲的文件句柄，如果全部用完，则返回(-1)。
INT32S _GetFreeFileEntry( void )
{
	INT32U i;
	
	for( i=0; i<MAX_FILE_ENTRY; i++ )
	{
		if( FAT_FileHandles[i].fs == NULL )			/* If handles[i] = 0 means, we can open a file      */
			return i;
	}
	return (-1);
}

INT32U USBFileOpen( INT8U* filename, INT8U nFlags )
{
    INT32S fd;
    FIL *file;
    FRESULT result;
    INT8U   fn[VFAT_MAXLEN_BYTES+1];
    INT32U i;

    fd = _GetFreeFileEntry();
    if( fd < 0 )
    	return (USB_ERROR_OPEN_MAX_REACHED);
    file = &FAT_FileHandles[fd];

    memset( fn, 0x20, VFAT_MAXLEN_BYTES+1 );
    fn[0] = '0';
    fn[1] = ':';
	for( i=0; i<strlen((char*)filename); i++ )
	{
		fn[i+2] = UpperCase( filename[i] );
	}
    fn[i+2] = 0;

    switch( nFlags )
    {
    case USB_MODE_RO:
        result = f_open( file, (const XCHAR*)fn, FA_OPEN_EXISTING | FA_READ );
        break;
    case USB_MODE_RW:
        result = f_open( file, (const XCHAR*)fn, FA_CREATE_ALWAYS | FA_WRITE );
        break;
    case USB_MODE_APPEND:
    default:
        return USB_ERROR_OPEN;
    }

    if( result == FR_OK )
        return fd;
    else
        return USB_ERROR_OPEN;
}


INT32U USBFileReadLarge( INT32U fd, INT8U* buffer, INT32U len )
{
    return USB_ERROR_ARGUMENTS;
}

INT32U USBFileRead( INT32U fd, INT8U* buffer, INT32U len )
{
    FRESULT res;
    FIL *file;
    INT32U numOfReadBytes = 0;

   if( ERROR(fd) || (fd >= MAX_FILE_ENTRY) )
        return USB_ERROR_HANDLE;

    file = &FAT_FileHandles[fd];
    res = f_read ( file, buffer, len, (void *)&numOfReadBytes);
    if( res == FR_OK )
        return numOfReadBytes;
    else
        return USB_ERROR_READ;
}

INT32U USBFileWriteLarge( INT32U fd, INT8U* buffer, INT32U len )
{
    return USB_ERROR_ARGUMENTS;
}

INT32U USBFileWrite( INT32U fd, INT8U* buffer, INT32U len )
{
    FRESULT res;
    FIL *file;
    INT32U bytesWritten = 0;

    if( ERROR(fd) || (fd >= MAX_FILE_ENTRY) )
        return USB_ERROR_HANDLE;

    file = &FAT_FileHandles[fd];
    res = f_write ( file, buffer, len, (void *)&bytesWritten);
    if( res == FR_OK )
        return bytesWritten;
    else
        return USB_ERROR_WRITE;
}

INT32U  USBFileClose( INT32U  fd )
{
    FRESULT res;
    FIL *file;

    if( ERROR(fd) || (fd >= MAX_FILE_ENTRY) )
        return USB_ERROR_HANDLE;

    file = &FAT_FileHandles[fd];
    res = f_close ( file );
    if( res == FR_OK )
        return USB_SUCC;
    else
        return USB_ERROR_CLOSE;
}


INT32U USBFileLength( INT32U fd )
{
    if( ERROR(fd) || (fd >= MAX_FILE_ENTRY) )
        return USB_ERROR_HANDLE;

    return FAT_FileHandles[fd].fsize;
}

/*
 *
 *
 * 返回当前指针所在位置
 */
INT32U USBFileSeek( INT32U fd, INT32S offset, INT8U mode )
{
    FRESULT res;
    FIL *file;

    if( ERROR(fd) || (fd >= MAX_FILE_ENTRY) )
        return USB_ERROR_HANDLE;

    file = &FAT_FileHandles[fd];

    switch( mode )
    {
    case SEEK_FROM_BEGIN:
        res = f_lseek( file, offset );
        break;
    case SEEK_FROM_CURRENT:
    case SEEK_FROM_END:
    default:
        return USB_ERROR_ARGUMENTS;
    }

    if( res == FR_OK )
        return USB_SUCC;
    else
        return USB_ERROR_SEEK;
}


/*
 *
 *
 * 文件存在，返回TRUE； 否则返回FALSE。
 */
BOOLEAN USBFileExists( INT8U* filename )
{
    FILINFO fno;
    if( f_stat( (const XCHAR*)filename, &fno ) == FR_OK )
    {
        if( strlen(fno.fname) > 0 )
		    return TRUE;
    }
    return FALSE;

}

////////////////////////////////////////////////////////////////////////////////
BOOLEAN _API_USBHostInit( void )
{
	return USBHostInit();
}

BOOLEAN _API_USBHostReset( INT32U time )
{
	return USBHostInit();
}

BOOLEAN	_API_USBScan( void )
{
	return USBScan();
}

BOOLEAN _API_USBStop( void )
{
	return USBStop();
}

INT32U  _API_USBFileOpen( INT8U* filename, INT8U mode )
{
	return USBFileOpen( filename, mode );
}

INT32U  _API_USBFileRead( INT32U fd, INT8U* buffer, INT32U len )
{
	return USBFileRead( fd, buffer, len );
}

INT32U  _API_USBFileWrite( INT32U fd, INT8U* buffer, INT32U len )
{
	return USBFileWrite( fd, buffer, len );
}

INT32U _API_USBFileClose( INT32U  fd )
{
	return USBFileClose( fd );
}

INT32U  _API_USBFileLength( INT32U fd )
{
	return USBFileLength( fd );
}

INT32U  _API_USBFileSeek( INT32U fd, INT32S offset, INT8U mode )
{
	return USBFileSeek( fd, offset, mode );
}

BOOLEAN _API_USBFileExists( INT8U* filename )
{
	return USBFileExists( filename );
}

BOOLEAN _API_USBIsReady( void )
{
    return USBIsReady();
}

INT32U  _API_USBGetFatInfo( FATINFO* fatinfo )
{
    return USBGetFatInfo( fatinfo );
}

/* minigzip.c -- simulate gzip using the zlib compression library
 * Copyright (C) 1995-2006, 2010 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/*
 * minigzip is a minimal implementation of the gzip utility. This is
 * only an example of using zlib and isn't meant to replace the
 * full-featured gzip. No attempt is made to deal with file systems
 * limiting names to 14 or 8+3 characters, etc... Error checking is
 * very limited. So use minigzip only for testing; use gzip for the
 * real thing. On MSDOS, use only on file names without extension
 * or in pipe mode.
 */

/* @(#) $Id$ */

#include "zlib.h"
#include <stdio.h>
//#include "bsp_ser.h"
#ifdef STDC
#  include <string.h>
#  include <stdlib.h>
#endif

#define SET_BINARY_MODE(file)

#if !defined(Z_HAVE_UNISTD_H) && !defined(_LARGEFILE64_SOURCE)
#ifndef WIN32 /* unlink already in stdio.h for WIN32 */
  extern int unlink OF((const char *));
#endif
#endif

#ifndef GZ_SUFFIX
#  define GZ_SUFFIX ".gz"
#endif
#define SUFFIX_LEN (sizeof(GZ_SUFFIX)-1)

#define BUFLEN      16384
#define MAX_NAME_LEN 1024
#define local

char *prog;

void error            OF((const char *msg));
void gz_compress      OF((FILE   *in, gzFile out));
#ifdef USE_MMAP
int  gz_compress_mmap OF((FILE   *in, gzFile out));
#endif
void gz_uncompress    OF((gzFile in, FILE   *out));
void file_compress    OF((char  *file, char *mode));
void file_uncompress  OF((char  *file));
//int  zmain            OF((int argc, char *argv[]));
int zip_main          OF(( unsigned char* srcBuff, unsigned long srcLen, unsigned char dstBuff, unsigned long* dstLen ));


// add by LIGHTRAIN
int  access(const char *a, int b){return 0;}
int  chmod(const char *a, int b){return 0;}
int  chsize(int a, long b){return 0;}
int  close(int a){return 0;}
int  creat(const char *a, int b){return 0;}
int  dup(int a){return 0;}
int  dup2(int a, int b){return 0;}
int  eof(int a){return 0;}
long  filelength(int a){return 0;}
int  isatty(int a){return 0;}
int  locking(int a, int b, long c){return 0;}
long  lseek(int a, long b, int c){return 0;}
char *  mktemp(char *a){return NULL;}
int  open(const char *a, int b, ...){return 0;}
int  read(int a, void *b, unsigned int c){return 0;}
int  setmode(int a, int b){return 0;}
int  sopen(const char *a, int b, int c, ...){return 0;}
long  tell(int a){return 0;}
int  umask(int a){return 0;}
int  unlink(const char *a){return 0;}
int  write(int a, const void *b, unsigned int c){return 0;}
// end of add

/* ===========================================================================
 * Display error message and exit
 */
void error(msg)
    const char *msg;
{
    fprintf(stderr, "%s: %s\n", prog, msg);
    exit(1);
}

/* ===========================================================================
 * Usage:  zip_main
 */
//#include "AT45DB321D.h"
typedef struct _ZipHeaderInfo{
	unsigned char  szSignature[2];
	unsigned short wZipFormat;
	unsigned long  dwZipParam;
	unsigned long  dwBeforeZipSize;
	unsigned long  dwBeforeZipCRC;
	unsigned long  dwAfterZipSize;
	unsigned long  dwAfterZipCRC;
	unsigned long  dwReserved;
	unsigned long  dwHeaderCRC;
}ZipHeaderInfo, *PZipHeaderInfo;

#define BOOT_FLAG_NEED_UPDATE	1		/* 有新程序更新 */
#define BOOT_FLAG_RUN_SECOND	2		/* 运行第二应用 */
#define BOOT_FLAG_TRY_SECOND	3		/* 尝试运行第二应用 */
#define BOOT_FLAG_RUN_FIRST		4		/* 运行第一应用 */

struct _sUpgradeFlagArea
{
	unsigned long flag;
	unsigned long uAppLen;
	unsigned long uAppCrc;
	unsigned long uFlagCrc;
};

extern void WatchDogClear( INT8U index );

#define GZIP_DEBUGOUT(...)
#define GZIP_WD_CLEAR()			WatchDogClear( 1 )
#define GZIP_FLASH_MAX_SIZE		4096

INT32U gzSPIFlashEraseWrite( INT8U* buff, INT32U addr, INT32U len )
{
	INT32U tmp, total = 0;
	while( len > 0 )
	{
		tmp = ( len > GZIP_FLASH_MAX_SIZE ) ? GZIP_FLASH_MAX_SIZE : len;
		total += SPIFlashEraseWrite( buff, addr, tmp );
		buff += tmp;
		addr += tmp;
		len -= tmp;
		GZIP_WD_CLEAR();
	}
	return total;
}

INT32U gzSPIFlashRead( INT8U* buff, INT32U addr, INT32U len )
{
	INT32U tmp, total = 0;
	while( len > 0 )
	{
		tmp = ( len > GZIP_FLASH_MAX_SIZE ) ? GZIP_FLASH_MAX_SIZE : len;
		total += SPIFlashRead( buff, addr, tmp );
		buff += tmp;
		addr += tmp;
		len -= tmp;
		GZIP_WD_CLEAR();
	}
	return total;
}

/* 更新标识区信息
 *      __________________________________________
 *     |      |         |         |               |
 *     | Flag | App len | App crc | Flag area crc |
 *     |      |         |         |               |
 *     |1 Byte| 4 Bytes | 4 Bytes |   4 Bytes     |
 *     |______|_________|_________|_______________|
 *
 *
 *    Flag = 1     有新程序更新
 *           2     运行第二应用
 *           3     尝试运行第二应用
 *           4     运行第一应用
 *           5     运行片内应用
 *           其它  运行第一应用
 */
#define UPGRADE_FLAG_OFFSET					0x7000
INT32U UpgradeFlagArea( unsigned long flag, unsigned long uAppLen, unsigned long uAppCRC )
{
	unsigned int crc = 0;
	struct _sUpgradeFlagArea ufa, ufa2;
	
	ufa.flag    = flag;
	ufa.uAppLen = uAppLen;
	ufa.uAppCrc = uAppCRC;
	
	crc = crc32( crc, (Byte*)&ufa, sizeof(ufa)-4 );
	ufa.uFlagCrc = crc;
	
	if( gzSPIFlashEraseWrite( (Byte*)&ufa, UPGRADE_FLAG_OFFSET, sizeof(ufa) ) != sizeof(ufa) )
		return UPGRADE_ERROR_FLAG_WRITE;
	if( gzSPIFlashRead( (Byte*)&ufa2, UPGRADE_FLAG_OFFSET, sizeof(ufa2) ) != sizeof(ufa2) )
		return UPGRADE_ERROR_FLAG_READ;
	if( memcmp( (Byte*)&ufa, (Byte*)&ufa2, sizeof(ufa) != 0 ) )
		return UPGRADE_ERROR_FLAG_VERIFY;

	return UPGRADE_SUCC;
}

/* 只更新Boot标志位(1字节) */
INT32U UpdateBootFlag( INT8U flag )
{
	unsigned int crc = 0;
	struct _sUpgradeFlagArea ufa, ufa2;
	
	if( gzSPIFlashRead( (Byte*)&ufa, UPGRADE_FLAG_OFFSET, sizeof(ufa) ) != sizeof(ufa) )
		return UPGRADE_ERROR_FLAG_READ;
	
	ufa.flag = flag;
	crc = 0;
	crc = crc32( crc, (Byte*)&ufa, sizeof(ufa)-4 );
	ufa.uFlagCrc = crc;

	if( gzSPIFlashEraseWrite( (Byte*)&ufa, UPGRADE_FLAG_OFFSET, sizeof(ufa) ) != sizeof(ufa) )
		return UPGRADE_ERROR_FLAG_WRITE;
	if( gzSPIFlashRead( (Byte*)&ufa2, UPGRADE_FLAG_OFFSET, sizeof(ufa2) ) != sizeof(ufa2) )
		return UPGRADE_ERROR_FLAG_READ;
	if( memcmp( (Byte*)&ufa, (Byte*)&ufa2, sizeof(ufa) != 0 ) )
		return UPGRADE_ERROR_FLAG_VERIFY;

	return UPGRADE_SUCC;
}

//int zip_main( unsigned char* srcBuff, unsigned long srcLen, unsigned char dstBuff, unsigned long* dstLen )
#ifdef CFG_GW_ZBI
	#define UPGRADE_HEADER_LEN				sizeof(ZipHeaderInfo)
	#define UPGRADE_MAX_DATA_LEN			0x200000
	
	#define UPGRADE_SECAPP_ADDR				0x200000
	#define UPGRADE_TEMP_ADDR				0x300000
	#define UPGRADE_COMPRESSED_DATA_ADDR	0x20C00000
	#define UPGRADE_UNCOMPRESSED_DATA_ADDR	0x20E00000
#else
	#define UPGRADE_HEADER_LEN				sizeof(ZipHeaderInfo)
	#define UPGRADE_MAX_DATA_LEN			0x180000
	
	#define UPGRADE_SECAPP_ADDR				0x200000
	#define UPGRADE_TEMP_ADDR				0x300000
	#define UPGRADE_COMPRESSED_DATA_ADDR	0x20400000
	#define UPGRADE_UNCOMPRESSED_DATA_ADDR	0x20580000
#endif

INT32U UpgradeMain( void )
{
	ZipHeaderInfo zip;
	unsigned int addr;
    unsigned int len, iRealRead;
	unsigned int iDestLen;
    int  err;
	unsigned int crc;
	unsigned long ret;
	
	GZIP_DEBUGOUT( "\nUpgradeMain<" );
	// 读入文件头
	addr = UPGRADE_TEMP_ADDR;
	len = UPGRADE_HEADER_LEN;
	iRealRead = gzSPIFlashRead( &zip.szSignature[0], addr, sizeof(ZipHeaderInfo) );
	GZIP_DEBUGOUT( "1" );
	if( iRealRead != len )
		return (UPGRADE_ERROR_HEADER_READ);
	
	//计算文件头CRC并与文件头中相应信息比较
	crc = 0;
	crc = crc32( crc, &zip.szSignature[0], sizeof(ZipHeaderInfo)-4 );
	GZIP_DEBUGOUT( "2" );
	if( crc != zip.dwHeaderCRC )
		return (UPGRADE_ERROR_HEADER_CRC);
	
	// 从SPI Flash将升级文件读入到SDRAM的相应区域
	addr = UPGRADE_TEMP_ADDR + UPGRADE_HEADER_LEN;
	len  = zip.dwAfterZipSize;
	iRealRead = gzSPIFlashRead( (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, addr, len );
	GZIP_DEBUGOUT( "3" );
	if( iRealRead != len )
		return (UPGRADE_ERROR_ZIP_READ);
	
	//计算解压缩前所有数据CRC并与文件头中相应信息比较
	crc = 0;
	crc = crc32( crc, (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, len );
	GZIP_DEBUGOUT( "4" );
	if( crc != zip.dwAfterZipCRC )
		return (UPGRADE_ERROR_ZIP_CRC);

	switch( zip.wZipFormat )
	{
	case 0x0001:
		// 将 UPGRADE_COMPRESSED_DATA_ADDR 解压数据至 UPGRADE_UNCOMPRESSED_DATA_ADDR
		iDestLen = zip.dwBeforeZipSize;
		err = uncompress( (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, (uLong*)&iDestLen,
						  (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, zip.dwAfterZipSize );
		GZIP_DEBUGOUT( "5" );
		if( err != Z_OK )
			return (UPGRADE_ERROR_UNZIP);
		if( iDestLen != zip.dwBeforeZipSize )
			return (UPGRADE_ERROR_UNZIP_LEN);
		break;
	default:
		len = zip.dwAfterZipSize;
		memcpy( (void*)UPGRADE_UNCOMPRESSED_DATA_ADDR, (void*)UPGRADE_COMPRESSED_DATA_ADDR,	len );
		break;
	}
	
	// 计算解压缩后所有数据CRC并与文件头中相应信息比较
	crc = 0;
	crc = crc32( crc, (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, iDestLen );
	GZIP_DEBUGOUT( "6" );
	if( crc != zip.dwBeforeZipCRC )
		return (UPGRADE_ERROR_UNZIP_CRC);
	
	// 修改启动标志，防止出现意外死机
	ret = UpdateBootFlag( BOOT_FLAG_RUN_FIRST );
	if( ret & 0x80000000 )
		return UPGRADE_ERROR_BOOTFLAG;
	GZIP_DEBUGOUT( "7" );
	
	// 将解压缩后的数据写入第二应用所在Flash区域
	addr = UPGRADE_SECAPP_ADDR;
	len = gzSPIFlashEraseWrite( (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, addr, iDestLen );
	GZIP_DEBUGOUT( "8" );
	if( len != iDestLen )
		return (UPGRADE_ERROR_UNZIP_WRITE);
	
	// 读入第二应用，并计算CRC
	addr = UPGRADE_SECAPP_ADDR;
	iRealRead = gzSPIFlashRead( (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, addr, iDestLen );
	GZIP_DEBUGOUT( "9" );
	if( iRealRead != iDestLen )
		return (UPGRADE_ERROR_COMPARE_LEN);
	
	crc = 0;
	crc = crc32( crc, (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, iDestLen );
	GZIP_DEBUGOUT( "A" );
	if( crc != zip.dwBeforeZipCRC )
		return (UPGRADE_ERROR_COMPARE_CRC);

	// 修改启动标志，尝试运行第二应用
	ret = UpgradeFlagArea( BOOT_FLAG_TRY_SECOND, zip.dwBeforeZipSize, zip.dwBeforeZipCRC );
	if( ret & 0x80000000 )		
		return UPGRADE_ERROR_BOOTFLAG;
	GZIP_DEBUGOUT( "B>" );
	return 0;
}

INT32U _API_ZLIB_Init( void )
{
	return ZLIB_SUCC;
}

INT32U _API_ZLIB_Compress( INT8U* dstBuf, INT32U *dstLen, INT8U* srcBuf, INT32U srcLen )
{
	int err;
	err = compress( dstBuf, (uLong*)dstLen, srcBuf, srcLen );
	if( err != Z_OK )
		return ZLIB_COMPRESS_ERROR;
	return ZLIB_SUCC;
}

INT32U _API_ZLIB_Uncompress( INT8U* dstBuf, INT32U *dstLen, INT8U* srcBuf, INT32U srcLen )
{
	int err;
	err = uncompress( dstBuf, (uLong*)dstLen, srcBuf, srcLen );
	if( err != Z_OK )
		return ZLIB_UNCOMPRESS_ERROR;
	return ZLIB_SUCC;
}

INT32U _API_ZLIB_CRC32( INT32U *crc, INT8U* buff, INT32U len )
{
	*crc = crc32( *crc, buff, len );
	return ZLIB_SUCC;
}

/////////////////////////////////////////////////////////////////////////////////
// 
//#include "FS.h"
//#include "yaffsfs.h"

#define UPGRADE_BOOTFLAG_FILENAME   "/app/bootflag.bin"

INT32U UpgradeInitToYAFFS( void )
{
    yaffs_mount( "/app" );
    return 0;
}

INT32U UpgradeFlagAreaToYAFFS( unsigned char flag, unsigned long uAppLen, unsigned long uAppCRC )
{
	unsigned int crc = 0;
	struct _sUpgradeFlagArea ufa, ufa2;
    int fhandle;
    //unsigned int ret;
    int size;
	
	ufa.flag    = flag;
	ufa.uAppLen = uAppLen;
	ufa.uAppCrc = uAppCRC;
	
	crc = crc32( crc, (Byte*)&ufa, sizeof(ufa)-4 );
	ufa.uFlagCrc = crc;

    if( yaffs_fexist( UPGRADE_BOOTFLAG_FILENAME ) )
        fhandle = yaffs_open( UPGRADE_BOOTFLAG_FILENAME, O_RDWR, S_IREAD | S_IWRITE );
    else
        fhandle = yaffs_open( UPGRADE_BOOTFLAG_FILENAME, O_CREAT|O_RDWR|O_TRUNC, S_IREAD | S_IWRITE );
    if( fhandle < 0 )
        return UPGRADE_ERROR_FILE_OPEN;
    
    yaffs_lseek( fhandle, 0, SEEK_SET ) ;
    size = yaffs_write( fhandle, (unsigned char*)&ufa, sizeof(ufa) );
    if( size != sizeof(ufa) )
    {
        yaffs_close(fhandle);
        return UPGRADE_ERROR_FILE_WRITE;
    }
    yaffs_close( fhandle );
        
    // 重新打开
    fhandle = yaffs_open( UPGRADE_BOOTFLAG_FILENAME, O_RDWR, S_IREAD | S_IWRITE );
    if( fhandle < 0 )
        return UPGRADE_ERROR_FILE_OPEN;

    yaffs_lseek( fhandle, 0, SEEK_SET ) ;
    size = yaffs_read( fhandle, (unsigned char*)&ufa2, sizeof(ufa2) );
    if( size != sizeof(ufa) )
    {
        yaffs_close(fhandle);
        return UPGRADE_ERROR_FILE_READ;
    }
    yaffs_close(fhandle);
        
	if( memcmp( &ufa, &ufa2, sizeof(ufa) != 0 ) )
		return UPGRADE_ERROR_FLAG_VERIFY;

	return UPGRADE_SUCC;
}

/* 只更新Boot标志位(1字节) */
INT32U UpdateBootFlagToYAFFS( INT8U flag )
{
	unsigned int crc = 0;
	struct _sUpgradeFlagArea ufa, ufa2;
    int fhandle, size;
    //unsigned int ret;
	
    UpgradeInitToYAFFS();
    
    fhandle = yaffs_open( UPGRADE_BOOTFLAG_FILENAME, O_RDWR, S_IREAD | S_IWRITE );
    if( fhandle < 0 )
        return UPGRADE_ERROR_FILE_OPEN;
    
    // 读取 bootflag
    yaffs_lseek( fhandle, 0, SEEK_SET );
    size = yaffs_read( fhandle, (unsigned char*)&ufa, sizeof(ufa) );
    if( size != sizeof(ufa) )
    {
        yaffs_close( fhandle );
        return UPGRADE_ERROR_FILE_READ;
    }
    
    // add at 2011.05.31
    crc = 0;
    crc = crc32( crc, (Byte*)&ufa, sizeof(ufa)-4 );
    if( (ufa.uFlagCrc == crc) && (flag == ufa.flag) )
    {
        yaffs_close( fhandle );
        return UPGRADE_SUCC;
    }
    // end of add

    // 修改 bootflag
   	ufa.flag = flag;
	crc = 0;
	crc = crc32( crc, (Byte*)&ufa, sizeof(ufa)-4 );
	ufa.uFlagCrc = crc;

    // 写入 bootflag
    yaffs_lseek( fhandle, 0, SEEK_SET );
    size = yaffs_write( fhandle, (unsigned char*)&ufa, sizeof(ufa) );
    if( size != sizeof(ufa) )
    {
        yaffs_close( fhandle );
        return UPGRADE_ERROR_FLAG_WRITE;
    }
    yaffs_close( fhandle );
        
    // 重新打开
    fhandle = yaffs_open( UPGRADE_BOOTFLAG_FILENAME, O_RDWR, S_IREAD | S_IWRITE );
    if( fhandle < 0 )
        return UPGRADE_ERROR_FILE_OPEN;
    
    // 再次读出
    yaffs_lseek( fhandle, 0, SEEK_SET );
    size = yaffs_read( fhandle, (unsigned char*)&ufa2, sizeof(ufa2) );
    if( size != sizeof(ufa2) )
    {
        yaffs_close( fhandle );
        return UPGRADE_ERROR_FLAG_READ;
    }
    yaffs_close(fhandle);
    
    // 比较
	if( memcmp( &ufa.flag, &ufa2.flag, sizeof(ufa) != 0 ) )
		return UPGRADE_ERROR_FLAG_VERIFY;
    
	return UPGRADE_SUCC;
}

// copy '/usr/app2.efz' to '/app/app2.bin' 
INT32U UpgradeMainToYAFFS( const INT8U* srcfile )
{
	ZipHeaderInfo zip;
	//unsigned int addr;
    unsigned int len, iRealRead;
	unsigned int iDestLen;
    int  err;
	unsigned int crc;
	unsigned long ret;
    int fhandleSrc, fhandleDst, size;
    struct yaffs_stat fstat;
	const INT8U* dstfile = "/app/app2.bin";
    INT8U strFileName[64];

#if FILENAME_CASE_SENSITIVE
    memset( strFileName, 0, 64 );
    strcpy( strFileName, srcfile );
#else
    len = 0;
    while( *srcfile )
    {
        strFileName[len++] = tolower( *srcfile );
        srcfile++;
    }
    strFileName[len] = 0;
#endif
    
    UpgradeInitToYAFFS();
    
	GZIP_DEBUGOUT( "\nUpgradeMain<" );
    //fhandleSrc = yaffs_open( (char*)srcfile, O_RDWR, S_IREAD | S_IWRITE );
    fhandleSrc = yaffs_open( (char*)strFileName, O_RDWR, S_IREAD | S_IWRITE );
    if( fhandleSrc < 0 )
        return UPGRADE_ERROR_FILE_OPEN;
    
	// 读入文件头
    GZIP_DEBUGOUT( "1" );
    yaffs_lseek( fhandleSrc, 0, SEEK_SET );
    size = yaffs_read( fhandleSrc, &zip.szSignature[0], sizeof(ZipHeaderInfo) );
    if( size != sizeof(ZipHeaderInfo) )
    {
        yaffs_close( fhandleSrc );
        return UPGRADE_ERROR_FILE_READ;
    }
    
	//计算文件头CRC并与文件头中相应信息比较
	GZIP_DEBUGOUT( "2" );
	crc = 0;
	crc = crc32( crc, &zip.szSignature[0], sizeof(ZipHeaderInfo)-4 );
	if( crc != zip.dwHeaderCRC )
    {
        yaffs_close( fhandleSrc );
        return UPGRADE_ERROR_HEADER_CRC;
    }
	
	// 将升级文件读入到SDRAM的相应区域
	GZIP_DEBUGOUT( "3" );
	//addr = UPGRADE_TEMP_ADDR + UPGRADE_HEADER_LEN;
	len  = zip.dwAfterZipSize;
	iRealRead = yaffs_read( fhandleSrc, (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, len );
	if( iRealRead != len )
    {
        yaffs_close( fhandleSrc );
        return UPGRADE_ERROR_ZIP_READ;
    }
	yaffs_close( fhandleSrc );
    
	//计算解压缩前所有数据CRC并与文件头中相应信息比较
	GZIP_DEBUGOUT( "4" );
	crc = 0;
	crc = crc32( crc, (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, len );
	if( crc != zip.dwAfterZipCRC )
		return (UPGRADE_ERROR_ZIP_CRC);

	switch( zip.wZipFormat )
	{
	case 0x0001:
		// 将 UPGRADE_COMPRESSED_DATA_ADDR 解压数据至 UPGRADE_UNCOMPRESSED_DATA_ADDR
		GZIP_DEBUGOUT( "5" );
		iDestLen = zip.dwBeforeZipSize;
		err = uncompress( (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, (uLong*)&iDestLen,
						  (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, zip.dwAfterZipSize );
		if( err != Z_OK )
			return (UPGRADE_ERROR_UNZIP);
		if( iDestLen != zip.dwBeforeZipSize )
			return (UPGRADE_ERROR_UNZIP_LEN);
		break;
	default:
		len = zip.dwAfterZipSize;
		memcpy( (void*)UPGRADE_UNCOMPRESSED_DATA_ADDR, (void*)UPGRADE_COMPRESSED_DATA_ADDR,	len );
		break;
	}
	
	// 计算解压缩后所有数据CRC并与文件头中相应信息比较
	GZIP_DEBUGOUT( "6" );
	crc = 0;
	crc = crc32( crc, (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, iDestLen );
	if( crc != zip.dwBeforeZipCRC )
		return (UPGRADE_ERROR_UNZIP_CRC);
	
	// 将解压缩后的数据写入第二应用所在Flash区域
	GZIP_DEBUGOUT( "7" );
    fhandleDst = yaffs_open( (char*)dstfile, O_CREAT|O_RDWR|O_TRUNC, S_IREAD | S_IWRITE );
    if( fhandleDst < 0 )
    {
        yaffs_close( fhandleDst );
        return UPGRADE_ERROR_FILE_OPEN;
    }
    
	GZIP_DEBUGOUT( "8" );
    yaffs_lseek( fhandleDst, 0, SEEK_SET );
    size = yaffs_write( fhandleDst, (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, iDestLen );
    if( size != iDestLen )
    {
        yaffs_close( fhandleDst );
        return (UPGRADE_ERROR_UNZIP_WRITE);
    }
    yaffs_close( fhandleDst );
	
	// 重新读入第二应用，并计算CRC
	GZIP_DEBUGOUT( "9" );
    fhandleDst = yaffs_open( (char*)dstfile, O_RDWR, S_IREAD | S_IWRITE );
    if( fhandleDst < 0 )
        return UPGRADE_ERROR_FILE_READ;
    yaffs_fstat( fhandleDst, &fstat );
    yaffs_lseek( fhandleDst, 0, SEEK_SET );
    iDestLen = yaffs_read( fhandleDst, (unsigned char*)UPGRADE_COMPRESSED_DATA_ADDR, fstat.yst_size );
	GZIP_DEBUGOUT( "A" );
	if( iDestLen != fstat.yst_size )
    {
        yaffs_close( fhandleDst );
        return UPGRADE_ERROR_COMPARE_LEN;
    }
    yaffs_close( fhandleDst );
    
	crc = 0;
	crc = crc32( crc, (unsigned char*)UPGRADE_UNCOMPRESSED_DATA_ADDR, iDestLen );
	GZIP_DEBUGOUT( "B" );
	if( crc != zip.dwBeforeZipCRC )
		return (UPGRADE_ERROR_COMPARE_CRC);

	// 修改启动标志，尝试运行第二应用
	GZIP_DEBUGOUT( "C" );
	ret = UpgradeFlagAreaToYAFFS( BOOT_FLAG_TRY_SECOND, zip.dwBeforeZipSize, zip.dwBeforeZipCRC );
	if( ret & 0x80000000 )		
		return UPGRADE_ERROR_BOOTFLAG;

	GZIP_DEBUGOUT( "D>" );
	return 0;
}
